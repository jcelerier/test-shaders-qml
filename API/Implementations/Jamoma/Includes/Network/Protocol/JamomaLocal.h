/*!
 * \file JamomaLocal.h
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

#include "Network/Protocol/Local.h"

#include "Network/JamomaProtocol.h"

#include "TTModular.h"

using namespace OSSIA;
using namespace std;

class JamomaLocal final : public Local, public JamomaProtocol
{

public:

# pragma mark -
# pragma mark Life cycle

  JamomaLocal();

  ~JamomaLocal();

# pragma mark -
# pragma mark Operation

  bool pullAddressValue(Address&) const override;

  bool pushAddressValue(const Address&) const override;

  bool observeAddressValue(shared_ptr<Address>, bool) const override;

  bool updateChildren(Node& node) const override;
};
