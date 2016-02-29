/*!
 * \file ExpressionPulse.h
 *
 * \defgroup Editor
 *
 * \brief
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

#include "Editor/Expression.h"
#include "Network/Node.h"

namespace OSSIA
{

class ExpressionPulse : public Expression
{

public:

# pragma mark -
# pragma mark Life cycle
  
  /*! factory
   \param #Destination*
   \return std::shared_ptr<#ExpressionPulse> */
  static std::shared_ptr<ExpressionPulse> create(const Destination*);
  
  /*! clone
   \return std::shared_ptr<#ExpressionPulse> */
  virtual std::shared_ptr<ExpressionPulse> clone() const = 0;
  
  /*! destructor */
  virtual ~ExpressionPulse();

# pragma mark -
# pragma mark Execution
  
  /*! evaluate if a destination value change have been observed since expression creation or since the last update
   \return true after the destination being updated */
  virtual bool evaluate() const override = 0;
  
  /*! reset the evaluation to false until the next destination update */
  virtual void update() const override = 0;

# pragma mark -
# pragma mark Accessors
  
  /*! get the type of the expression
   \return #Type of the expression */
  Expression::Type getType() const override final
  {return Expression::Type::PULSE;}
  
  /*! get destination
   \return const #Destination* expression */
  virtual const Destination* getDestination() const = 0;

};
}
