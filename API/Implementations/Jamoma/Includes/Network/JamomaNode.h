/*!
 * \file JamomaNode.h
 *
 * \brief
 *
 * \details
 *
 * \author Théo de la Hogue
 *
 * \copyright This code is licensed under the terms of the "CeCILL-C"
 * http://www.cecill.info
 */

#pragma once

#include "Network/Address.h"
#include "Network/Device.h"
#include "Network/Node.h"
#include "Network/Protocol.h"

#include "Network/JamomaAddress.h"

#include "Misc/JamomaContainer.h"

#include "TTModular.h"

#include <map>

using namespace OSSIA;
using namespace std;
using namespace std::placeholders;

class JamomaProtocol;

class JamomaNode : public virtual Node, public enable_shared_from_this<JamomaNode>
{

protected:

# pragma mark -
# pragma mark Implementation specific

  TTNodeDirectory *       mDirectory{};
  TTNode *                mNode{};
  TTObject                mObject;

  weak_ptr<Device>        mDevice;
  weak_ptr<JamomaNode>    mParent;
  shared_ptr<Address>     mAddress;

  bool                    mIsDevice{false};
  
  map<shared_ptr<Node>, Node::iterator> mChildNodeChangeCallbackIndexes;

public:

# pragma mark -
# pragma mark Life cycle

  JamomaNode(TTNodeDirectory * aDirectory = nullptr, TTNode * aNode = nullptr, shared_ptr<Device> aDevice = nullptr, shared_ptr<JamomaNode> aParent = nullptr);

  ~JamomaNode();

# pragma mark -
# pragma mark Network

  shared_ptr<Device> getDevice() const override;

  shared_ptr<Node> getParent() const override;

# pragma mark -
# pragma mark Accessors

  string getName() const override;

  Node & setName(std::string) override;

  shared_ptr<Address> getAddress() const override;

# pragma mark -
# pragma mark Address

  shared_ptr<Address> createAddress(Value::Type type) override;

  bool removeAddress() override;

# pragma mark -
# pragma mark Children

  Container<Node>::iterator emplace(Container<Node>::const_iterator pos, string name) override;
  
  Container<Node>::iterator emplace(Container<Node>::const_iterator pos,
                                    const string& name,
                                    Value::Type type,
                                    AccessMode,
                                    const std::shared_ptr<Domain>&,
                                    BoundingMode,
                                    bool repetitionFilter) override;

  Container<Node>::iterator insert(Container<Node>::const_iterator, shared_ptr<Node>, std::string) override;
  
  Container<Node>::iterator erase(Container<Node>::const_iterator) override;
  
# pragma mark -
# pragma mark Callback Container
  
  Node::iterator addCallback(NodeChangeCallback) override;
  
  void removeCallback(Node::iterator) override;

# pragma mark -
# pragma mark Implementation specific

  //! \note don't put implementation specifics stuff in private or protected as they are invisible for API users
  //! \note this allow to avoid friendship for each Protocol for example

  TTObject & getApplication() const;

  TTSymbol getApplicationType() const;

  /* start namespace update from this node */
  bool updateChildren();

  /* recursive method to build children from the Jamoma tree */
  void buildChildren();

  /* build the address depending on the Jamoma node object */
  void buildAddress();
  
  /* get any child change back */
  void childNodeChangeCallback(const Node&, const std::string&, NodeChange);

  /* remove all Addresses by closing the listening before deletion */
  void removeAddresses();
};


