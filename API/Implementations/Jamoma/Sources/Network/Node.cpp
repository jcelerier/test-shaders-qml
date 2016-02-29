#include "Network/JamomaNode.h"
#include "Network/JamomaDevice.h"
#include <assert.h>

# pragma mark -
# pragma mark Life cycle

JamomaNode::JamomaNode(TTNodeDirectory * aDirectory, TTNode * aNode, shared_ptr<Device> aDevice, shared_ptr<JamomaNode> aParent) :
mDirectory(aDirectory),
mNode(aNode),
mObject("NodeInfo"),
mDevice(aDevice),
mParent(aParent)
{
    // if no object add a default NodeInfo object
    // otherwise TTNodeDirectory automatically removes
    // empty parent binding on no object when destroying the last child
    if (mNode->getObject() == nullptr)
      mNode->setObject(mObject);
}

JamomaNode::~JamomaNode()
{
  m_children.clear();

  if (!mIsDevice)
  {
    TTAddress adrs;
    mNode->getAddress(adrs);
    mDirectory->TTNodeRemove(adrs);
  }
}

Node::~Node()
{
}

# pragma mark -
# pragma mark Navigation

shared_ptr<Device> JamomaNode::getDevice() const
{
  return mDevice.lock();
}

shared_ptr<Node> JamomaNode::getParent() const
{
  return mParent.lock();
}

# pragma mark -
# pragma mark Accessors

string JamomaNode::getName() const
{
  if (mNode)
  {
    TTString nameInstance = mNode->getName().c_str();
    if (mNode->getInstance() != kTTSymEmpty)
    {
      nameInstance += ".";
      nameInstance += mNode->getInstance().c_str();
    }

    return nameInstance.data();
  }

  return "";
}

Node & JamomaNode::setName(std::string name)
{
  TTAddress nameInstance(name.data());
  TTSymbol newInstance;
  TTBoolean newInstanceCreated;
  
  string oldName = getName();

  mNode->setNameInstance(nameInstance, newInstance, &newInstanceCreated);

  // notify observers
  send(*this, oldName, NodeChange::RENAMED);

  return *this;
}

shared_ptr<Address> JamomaNode::getAddress() const
{
  return mAddress;
}

# pragma mark -
# pragma mark Address

shared_ptr<Address> JamomaNode::createAddress(Value::Type type)
{
  // clear former address
  removeAddress();

  if (mNode)
  {
    TTSymbol applicationType = getApplicationType();

    // for local application case
    if (applicationType == kTTSym_local)
    {
      mObject = TTObject("Data", "parameter");
      mObject.set("rampDrive", kTTSym_none);
      mNode->setObject(mObject);
    }

    // for proxy application
    else if (applicationType == kTTSym_proxy)
    {
      TTAddress nodeAddress;
      mNode->getAddress(nodeAddress);
      TTValue args(nodeAddress, "parameter");
      mObject = getApplication().send("ProxyDataInstantiate", args);
      mObject.set("rampDrive", kTTSym_none);
    }

    // for mirror application
    else if (applicationType == kTTSym_mirror)
    {
      TTAddress nodeAddress;
      mNode->getAddress(nodeAddress);
      TTValue args(nodeAddress, "Data");
      mObject = getApplication().send("MirrorObjectInstantiate", args);

      //! \see in Device::create method, when creating Minuit protocol, some attributes are setup to be cached
      // all attributes handled by Address class should be in this list
    }

    // edit new address
    mAddress = make_shared<JamomaAddress>(shared_from_this(), mObject);

    // set type
    mAddress->setValueType(type);

    // notify observers
    send(*this, getName(), NodeChange::ADDRESS_CREATED);
  }

  return mAddress;
}

bool JamomaNode::removeAddress()
{
  if (mNode)
  {
    // use the device protocol to stop address value observation
    if (mAddress)
    {
      if (auto device = getDevice())
      {
        device->getProtocol()->observeAddressValue(mAddress, false);
      }
    }

    mAddress.reset();

    // add a NodeInfo object otherwise TTNodeDirectory
    // automatically removes empty parent binding on no object
    // when destroying the last child
    mObject = TTObject("NodeInfo");
    TTErr err = mNode->setObject(mObject);

    // notify observers
    send(*this, getName(), NodeChange::ADDRESS_REMOVED);

    return !err;
  }

  return false;
}

# pragma mark -
# pragma mark Children

Container<Node>::iterator JamomaNode::emplace(Container<Node>::const_iterator pos, string name)
{
  if (name.empty())
    throw runtime_error("can't create a node with empty name");

  TTAddress nodeAddress;
  mNode->getAddress(nodeAddress);

  TTAddress   address = nodeAddress.appendAddress(TTAddress(name.data()));
  TTObject    empty;
  TTPtr       context = NULL;
  TTNodePtr   node;
  TTBoolean   newInstanceCreated;

  TTErr err = mDirectory->TTNodeCreate(address, empty, context, &node, &newInstanceCreated);

  if (!err)
  {
    // store the new node into the Container
    auto newNode = make_shared<JamomaNode>(mDirectory, node, mDevice.lock(), shared_from_this());
    Container<Node>::iterator it = m_children.insert(pos, newNode);

    // notify observers
    send(*newNode, newNode->getName(), NodeChange::EMPLACED);

    // start child changes observation if needed
    if (callbacks().size() >= 1)
    {
      pair<shared_ptr<Node>, Node::iterator> p(newNode, newNode->addCallback(std::bind(&JamomaNode::childNodeChangeCallback, this, _1, _2, _3)));
      mChildNodeChangeCallbackIndexes.emplace(p);
    }

    return it;
  }

  return Container<Node>::iterator();
}

Container<Node>::iterator JamomaNode::emplace(Container<Node>::const_iterator pos,
                                              const string& name,
                                              Value::Type type,
                                              AccessMode access,
                                              const std::shared_ptr<Domain>& domain,
                                              BoundingMode bm,
                                              bool repetitionFilter)
{
  if (name.empty())
    throw runtime_error("can't create a node with empty name");

  TTAddress nodeAddress;
  mNode->getAddress(nodeAddress);

  TTAddress   address = nodeAddress.appendAddress(TTAddress(name.data()));
  TTObject    empty;
  TTPtr       context = NULL;
  TTNodePtr   node;
  TTBoolean   newInstanceCreated;

  TTErr err = mDirectory->TTNodeCreate(address, empty, context, &node, &newInstanceCreated);

  if (!err)
  {
    // create a new node and its address
    auto newNode = make_shared<JamomaNode>(mDirectory, node, mDevice.lock(), shared_from_this());
    std::shared_ptr<OSSIA::Address> addr = newNode->createAddress(type);

    addr->setBoundingMode(bm);
    addr->setAccessMode(access);
    if(domain)
        addr->setDomain(domain);
    addr->setBoundingMode(bm);
    addr->setRepetitionFilter(repetitionFilter);

    // store the new node into the Container
    Container<Node>::iterator it = m_children.insert(pos, newNode);

    // notify observers
    send(**it, getName(), NodeChange::EMPLACED);

    // start child changes observation if needed
    if (callbacks().size() >= 1)
    {
      pair<shared_ptr<Node>, Node::iterator> p(newNode, newNode->addCallback(std::bind(&JamomaNode::childNodeChangeCallback, this, _1, _2, _3)));
      mChildNodeChangeCallbackIndexes.emplace(p);
    }

    return it;
  }

  return Container<Node>::iterator();
}

Container<Node>::iterator JamomaNode::insert(Container<Node>::const_iterator pos, shared_ptr<Node> node, std::string name)
{
  assert(!name.empty());

  //! \todo here the alias is not effective into the Jamoma tree so the given name is not used ...

  return m_children.insert(pos, node);
}

Container<Node>::iterator JamomaNode::erase(Container<Node>::const_iterator requested_it)
{
  // \todo shouldn't we remove the parent here, in case another class keeps a shared_ptr ?
  // (which would cause the node to still be alive)

  // remove the all addresses below this node
  auto child = *requested_it;
  auto& jnode = dynamic_cast<JamomaNode&>(*child);
  jnode.removeAddresses();

  // remove the node
  Container<Node>::iterator it = m_children.erase(requested_it);

  // notify observers
  send(*child, child->getName(), NodeChange::ERASED);

  // stop child observation if needed
  if (callbacks().size() >= 1)
  {
    Node::iterator callbackIndex = mChildNodeChangeCallbackIndexes.find(child)->second;
    child->removeCallback(callbackIndex);
  }

  return it;
}

# pragma mark -
# pragma mark Callback

Node::iterator JamomaNode::addCallback(NodeChangeCallback callback)
{
  auto it = CallbackContainer::addCallback(std::move(callback));

  if (callbacks().size() == 1)
  {
    // start children changes observation
    for (auto child : children())
    {
      pair<shared_ptr<Node>, Node::iterator> p(child, child->addCallback(std::bind(&JamomaNode::childNodeChangeCallback, this, _1, _2, _3)));
      mChildNodeChangeCallbackIndexes.emplace(p);
    }
  }

  return it;
}

void JamomaNode::removeCallback(Node::iterator callback)
{
  CallbackContainer::removeCallback(callback);

  if (callbacks().size() == 0)
  {
    // stop children changes observation
    for (auto child : children())
    {
      Node::iterator callbackIndex = mChildNodeChangeCallbackIndexes.find(child)->second;
      child->removeCallback(callbackIndex);
    }

    mChildNodeChangeCallbackIndexes.clear();
  }
}

# pragma mark -
# pragma mark Implementation specific

TTObject & JamomaNode::getApplication() const
{
  return mDirectory->getRoot()->getObject();
}

TTSymbol JamomaNode::getApplicationType() const
{
  TTSymbol type;
  mDirectory->getRoot()->getObject().get("type", type);

  return type;
}

bool JamomaNode::updateChildren()
{
  //! \note this method is only available for root node for the moment
  if (mNode->getObject() != getApplication())
    return false;

  // erase all former nodes
  m_children.clear();

  // tell our application to rebuild
  //! \todo rebuild from ourself
  TTErr err = getApplication().send("DirectoryBuild");

  // update root node
  this->mNode = this->mDirectory->getRoot();

  // rebuild from tree
  buildChildren();

  // is there children below ?
  if (children().size() == 0)
    throw runtime_error("children empty after the update");

  return err == kTTErrNone;
}

void JamomaNode::buildChildren()
{
  TTList childrenList;

  mNode->getChildren(S_WILDCARD, S_WILDCARD, childrenList);

  if (!childrenList.isEmpty())
  {
    // sort children by priority
    childrenList.sort(&compareNodePriorityThenNameThenInstance);

    // build a node for each child
    for (childrenList.begin(); childrenList.end(); childrenList.next())
    {
      TTNodePtr child = TTNodePtr(TTPtr(childrenList.current()[0]));

      // build child node
      shared_ptr<JamomaNode> newNode = make_shared<JamomaNode>(mDirectory, child, mDevice.lock(), shared_from_this());

      // build address if the node already manages a valid Data or Mirror object
      newNode->buildAddress();

      // store the child node
      m_children.push_back(newNode);

      // continue recursively
      newNode->buildChildren();
    }
  }
}

void JamomaNode::buildAddress()
{
  if (mNode)
  {
    TTObject object = mNode->getObject();
    if (object.valid())
    {
      TTSymbol objectName = object.name();

      if (objectName == kTTSym_Mirror)
        objectName = TTMirrorPtr(object.instance())->getName();

      if (objectName == "Data")
      {
        mAddress = make_shared<JamomaAddress>(shared_from_this(), object);

        // edit value type, access mode, bounding mode and repetition filter attribute
        TTSymbol type;
        object.get("type", type);

        if (type == kTTSym_none)
        {
          mAddress->setValue(new Impulse());
          mAddress->setValueType(Value::Type::IMPULSE);
        }
        else if (type == kTTSym_generic)
        {
          mAddress->setValue(new OSSIA::Tuple());
          mAddress->setValueType(Value::Type::TUPLE);
        }
        else if (type == kTTSym_boolean)
        {
          mAddress->setValue(new OSSIA::Bool());
          mAddress->setValueType(Value::Type::BOOL);
        }
        else if (type == kTTSym_integer)
        {
          mAddress->setValue(new OSSIA::Int());
          mAddress->setValueType(Value::Type::INT);
        }
        else if (type == kTTSym_decimal)
        {
          mAddress->setValue(new OSSIA::Float());
          mAddress->setValueType(Value::Type::FLOAT);
        }
        else if (type == kTTSym_array)
        {
          mAddress->setValue(new OSSIA::Tuple());
          mAddress->setValueType(Value::Type::TUPLE);
        }
        else if (type == kTTSym_string)
        {
          mAddress->setValue(new OSSIA::String());
          mAddress->setValueType(Value::Type::STRING);
        }

        TTSymbol service;
        object.get("service", service);

        if (service == kTTSym_parameter)
          mAddress->setAccessMode(OSSIA::AccessMode::BI);
        else if (service == kTTSym_message)
          mAddress->setAccessMode(OSSIA::AccessMode::SET);
        else if (service == kTTSym_return)
          mAddress->setAccessMode(OSSIA::AccessMode::GET);

        TTValue range;
        object.get("rangeBounds", range);

        if (type == kTTSym_none)
        {
          mAddress->setDomain(Domain::create());
        }
        else if (type == kTTSym_generic)
        {
          mAddress->setDomain(Domain::create());
        }
        else if (type == kTTSym_boolean)
        {
          if (range.size() == 2)
          {
            Value * min = new OSSIA::Bool(range[0]);
            Value * max = new OSSIA::Bool(range[1]);
            mAddress->setDomain(Domain::create(min, max));
          }
        }
        else if (type == kTTSym_integer)
        {
          if (range.size() == 2)
          {
            Value * min = new OSSIA::Int(range[0]);
            Value * max = new OSSIA::Int(range[1]);
            mAddress->setDomain(Domain::create(min, max));
          }
        }
        else if (type == kTTSym_decimal)
        {
          if (range.size() == 2)
          {
            Value * min = new OSSIA::Float(range[0]);
            Value * max = new OSSIA::Float(range[1]);
            mAddress->setDomain(Domain::create(min, max));
          }
        }
        else if (type == kTTSym_array)
        {
          // we need to know the size of the array to setup the domain
          TTValue v;
          object.get("value", v);

          vector<const Value*> tuple_min;
          vector<const Value*> tuple_max;
          for (unsigned long i = 0; i < v.size(); i++)
            tuple_min.push_back(new OSSIA::Float(range[0]));
          tuple_max.push_back(new OSSIA::Float(range[1]));

          mAddress->setDomain(Domain::create(new OSSIA::Tuple(tuple_min), new OSSIA::Tuple(tuple_max)));
        }
        else if (type == kTTSym_string)
        {
          // string values enumeration
          vector<const Value*> values;
          for (const auto & e : range)
          {
            TTSymbol s = e;
            values.push_back(new OSSIA::String(s.c_str()));
          }
          mAddress->setDomain(Domain::create(new OSSIA::Impulse(), new OSSIA::Impulse(), values));
        }
        else
        {
          mAddress->setDomain(Domain::create());
        }

        TTSymbol clipmode;
        object.get("rangeClipmode", clipmode);

        if (clipmode == kTTSym_none)
          mAddress->setBoundingMode(OSSIA::BoundingMode::FREE);
        else if (clipmode == kTTSym_low)
          mAddress->setBoundingMode(OSSIA::BoundingMode::CLIP);
        else if (clipmode == kTTSym_high)
          mAddress->setBoundingMode(OSSIA::BoundingMode::CLIP);
        else if (clipmode == kTTSym_both)
          mAddress->setBoundingMode(OSSIA::BoundingMode::CLIP);
        else if (clipmode == kTTSym_wrap)
          mAddress->setBoundingMode(OSSIA::BoundingMode::WRAP);
        else if (clipmode == kTTSym_fold)
          mAddress->setBoundingMode(OSSIA::BoundingMode::FOLD);

        TTBoolean repetitionFilter;
        object.get("repetitionFilter", repetitionFilter);

        mAddress->setRepetitionFilter(repetitionFilter);
      }
    }
  }
}

void JamomaNode::childNodeChangeCallback(const Node& child, const std::string& name, NodeChange change)
{
  // only notify tree structure changes to parent
  if (change == NodeChange::EMPLACED || change == NodeChange::ERASED || change == NodeChange::RENAMED)
    send(child, name, change);
}

void JamomaNode::removeAddresses()
{
  if(mAddress)
  {
    // close value listening
    getDevice()->getProtocol()->observeAddressValue(mAddress, false);

    // reset the shared_ptr
    mAddress.reset();
  }

  // do the same for all children
  for(auto& child : children())
  {
    auto& jnode = dynamic_cast<JamomaNode&>(*child);
    jnode.removeAddresses();
  }
}
