#include "Network/Protocol/JamomaMinuit.h"

using namespace OSSIA;

# pragma mark -
# pragma mark Life Cycle

shared_ptr<Minuit> Minuit::create(std::string ip, int in_port, int out_port)
{
  return make_shared<JamomaMinuit>(ip, in_port, out_port);
}

JamomaMinuit::JamomaMinuit(std::string ip, int in_port, int out_port) :
mIp(ip),
mInPort(in_port),
mOutPort(out_port)
{
  // create a Minuit protocol unit
  TTObject minuitObject = mApplicationManager.send("ProtocolFind", "Minuit");
  if (!minuitObject.valid())
    mApplicationManager.send("ProtocolInstantiate", "Minuit");
}

JamomaMinuit::~JamomaMinuit()
{}

Minuit::~Minuit()
{}

# pragma mark -
# pragma mark Accessors

std::string JamomaMinuit::getIp()
{
  return mIp;
}

Protocol & JamomaMinuit::setIp(std::string ip)
{
  TTObject minuitObject = mApplicationManager.send("ProtocolFind", "Minuit");
  
  mIp = ip;
  
  minuitObject.set("ip", TTSymbol(mIp));
  
  return *this;
}

int JamomaMinuit::getInPort()
{
  return mInPort;
}

Protocol & JamomaMinuit::setInPort(int in_port)
{
  TTObject minuitObject = mApplicationManager.send("ProtocolFind", "Minuit");
  
  mInPort = in_port;
  
  //! \note as Minuit is in Jamoma there only an in port (no out port)
  minuitObject.set("port", mInPort);
  
  return *this;
}

int JamomaMinuit::getOutPort()
{
  return mOutPort;
}

Protocol & JamomaMinuit::setOutPort(int out_port)
{
  TTObject minuitObject = mApplicationManager.send("ProtocolFind", "Minuit");
  
  mOutPort = out_port;
  
  //! \note as Minuit is in Jamoma there only an in port (no out port)
  // minuitObject.set("port", mOutPort);
  
  return *this;
}

# pragma mark -
# pragma mark Operation

bool JamomaMinuit::pullAddressValue(Address& address) const
{
  JamomaAddress& adrs = dynamic_cast<JamomaAddress&>(address);
  
  TTValue value;
  
  if (adrs.pullValue(value))
  {
    adrs.setValue(value);
    return true;
  }
  
  return false;
}

bool JamomaMinuit::pushAddressValue(const Address& address) const
{
  const JamomaAddress& adrs = dynamic_cast<const JamomaAddress&>(address);
  
  TTValue value;
  
  adrs.getValue(value);
  
  return adrs.pushValue(value);
}

bool JamomaMinuit::observeAddressValue(std::shared_ptr<Address> address, bool enable) const
{
  shared_ptr<JamomaAddress> adrs = dynamic_pointer_cast<JamomaAddress>(address);
  
  adrs->observeValue(enable);
  
  return true;
}

bool JamomaMinuit::updateChildren(Node& node) const
{
  JamomaNode& n = dynamic_cast<JamomaNode&>(node);
  
  return n.updateChildren();
}