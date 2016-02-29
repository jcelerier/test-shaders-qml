/*!
 * \file TimeProcess.h
 *
 * \defgroup Editor
 *
 * \brief #TimeProcess is the mother class to describe temporal content ...
 *
 * \details
 *
 * \author Clément Bossut
 * \author Théo de la Hogue
 *
 * \copyright This code is licensed under the terms of the "CeCILL-C"
 * http://www.cecill.info
 */

#pragma once

#include <memory>
#include <string>

namespace OSSIA
{
class StateElement;
class TimeConstraint;
class TimeValue;

class TimeProcess
{

public:

# pragma mark -
# pragma mark Life cycle

  /*! destructor */
  virtual ~TimeProcess();

# pragma mark -
# pragma mark Execution
  
  /*! get a #StateElement from the process on its parent #TimeConstraint offset
   \details don't call offset when the parent #TimeConstraint is running
   \param const #TimeValue offset date
   \return std::shared_ptr<#StateElement> */
  virtual std::shared_ptr<StateElement> offset(const TimeValue&) = 0;

  /*! get a #StateElement from the process depending on its parent #TimeConstraint date
   \details don't call state when the parent #TimeConstraint is not running
   \return std::shared_ptr<#StateElement> */
  virtual std::shared_ptr<StateElement> state() = 0;

# pragma mark -
# pragma mark Accessors

  std::shared_ptr<TimeConstraint> parent;

};
}
