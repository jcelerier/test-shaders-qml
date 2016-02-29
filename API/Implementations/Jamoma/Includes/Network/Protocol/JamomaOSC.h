/*!
 * \file JamomaOSC.h
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

#include <string>

#include "Network/Protocol/OSC.h"

#include "Network/JamomaProtocol.h"

#include "TTModular.h"

using namespace OSSIA;
using namespace std;

class JamomaOSC final : public OSC, public JamomaProtocol
{

private:

# pragma mark -
# pragma mark Implementation specific

  string    mIp;
  int       mInPort{};            /// the port that a remote device open to receive OSC messages
  int       mOutPort{};           /// the port where a remote device sends OSC messages to give some feeback (like "echo")
  bool      mLearning{};          /// if the device is currently learning from inbound messages.

  TTObject  mNamespaceObserver;
public:

# pragma mark -
# pragma mark Life cycle

  JamomaOSC(string, int, int);

  ~JamomaOSC();

# pragma mark -
# pragma mark Accessors

  std::string getIp() const override;

  Protocol & setIp(std::string) override;

  int getInPort() const override;

  Protocol & setInPort(int) override;

  int getOutPort() const override;

  Protocol & setOutPort(int) override;

  bool getLearningStatus() const override;
  Protocol & setLearningStatus(OSSIA::Device&, bool) override;

# pragma mark -
# pragma mark Operation

  bool pullAddressValue(Address&) const override;

  bool pushAddressValue(const Address&) const override;

  bool observeAddressValue(shared_ptr<Address>, bool) const override;

  bool updateChildren(Node& node) const override;
};
