/*!
 * \file Curve.h
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

#include <map>
#include <utility>
#include <memory>

#include "Value.h"
#include "Network/Node.h"
#include "Misc/Container.h"

namespace OSSIA
{
  
class CurveAbstract
{
    
public:
    
# pragma mark -
# pragma mark Life cycle
    
  /*! destructor 
   \todo remove = default */
  virtual ~CurveAbstract() = default;
};

template <typename Y>
class CurveSegment;

template <typename X, typename Y>
class Curve : public CurveAbstract
{

public:
  
  typedef X abscissa_type;
  typedef Y ordinate_type;
  
# pragma mark -
# pragma mark Life cycle
  
  /*! factory
   \return std::shared_ptr<#Curve> */
  static std::shared_ptr<Curve> create();
  
  /*! clone */
  virtual std::shared_ptr<Curve> clone() const = 0;
  
  /*! destructor */
  virtual ~Curve();
  
# pragma mark -
# pragma mark Edition
  
  /*! add a segment to reach a target point to the curve
   \param std::shared_ptr<#CurveSegment<Y>> segment to target point
   \param X target point abscissa
   \param Y target point ordinate
   \return bool */
  virtual bool addPoint(std::shared_ptr<CurveSegment<Y>>, X, Y) = 0;
  
  /*! remove a point from the curve
   \param X point abscissa
   \return bool */
  virtual bool removePoint(X) = 0;

# pragma mark -
# pragma mark Execution
  
  /*! get value at an abscissa
   \param X abscissa.
   \return Y ordinate */
  virtual Y valueAt(X) const = 0;

# pragma mark -
# pragma mark Accessors
  
  /*! get initial point abscissa
   \details if there is an initial abcissa destination, it will return the value of the address
   \return X value */
  virtual X getInitialPointAbscissa() const = 0;
  
  /*! get initial point ordinate
   \details if there is an initial ordinate destination, it will return the value of the address
   \return Y value */
  virtual Y getInitialPointOrdinate() const = 0;
  
  /*! set initial point abscissa
   \details if there is an initial abscissa destination, this accessor is useless
   \param X abscissa */
  virtual void setInitialPointAbscissa(X) = 0;
  
  /*! set initial point ordinate
   \details if there is an initial ordinate destination, this accessor is useless
   \param Y ordinate */
  virtual void setInitialPointOrdinate(Y) = 0;
  
  /*! get initial point abscissa destination
   \return const Destination* */
  virtual const Destination* getInitialPointAbscissaDestination() const = 0;
  
  /*! get initial point ordinate destination
   \return const Destination* */
  virtual const Destination* getInitialPointOrdinateDestination() const = 0;
  
  /*! set initial curve abscissa using a Destination
   \param const Destination* */
  virtual void setInitialPointAbscissaDestination(const Destination*) = 0;
  
  /*! set initial curve ordinate using a Destination
   \param const Destination* */
  virtual void setInitialPointOrdinateDestination(const Destination*) = 0;
  
  /*! get initial curve value
   \return std::map<X, std::pair<Y, std::shared_ptr<CurveSegment<Y>>>> map of {abscissa, {value, previous segment} */
  virtual std::map<X, std::pair<Y, std::shared_ptr<CurveSegment<Y>>>> getPointsMap() const = 0;

};
  
# pragma mark -
# pragma mark Behavior
  
/*! \details Behavior value */
struct Behavior : public Value
{
  std::shared_ptr<CurveAbstract> value;
  
  /*! constructor */
  Behavior(std::shared_ptr<CurveAbstract> v);
  
  /*! clone */
  Value * clone() const override;
  
  /*! equal operator */
  bool operator== (const Value&) const override;
  
  /*! different operator */
  bool operator!= (const Value&) const override;
  
  /*! greater than operator */
  bool operator> (const Value&) const override;
  
  /*! greater than and equal operator */
  bool operator>= (const Value&) const override;
  
  /*! less than operator */
  bool operator< (const Value&) const override;
  
  /*! less than and equal operator */
  bool operator<= (const Value&) const override;
};

}
