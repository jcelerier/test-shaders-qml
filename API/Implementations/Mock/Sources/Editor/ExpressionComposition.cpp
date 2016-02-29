#include "Editor/ExpressionComposition.h"
#include "Network/AddressValue.h"

using namespace OSSIA;
using namespace std;

class MockExpressionComposition : public ExpressionComposition {

public:

  // Dummy members
  shared_ptr<Expression> exp;

  // Constructors, destructor, cloning
  MockExpressionComposition(shared_ptr<Expression> exp1,
				Operator op,
				shared_ptr<Expression> exp2) {}
  MockExpressionComposition(const MockExpressionComposition * other) {}
  virtual ~MockExpressionComposition() {}
  virtual shared_ptr<ExpressionComposition> clone() const override {
	return shared_ptr<ExpressionComposition>(new MockExpressionComposition(this));
  }

  // Lecture
  virtual bool evaluate() const override {return true;}

  // Accessors
  virtual const shared_ptr<Expression> & getFirstOperand() const override {
	return exp;
  }
  virtual const shared_ptr<Expression> & getSecondOperand() const {
	return exp;
  }
  virtual Operator getOperator() const override {return Operator::AND;};

};

shared_ptr<ExpressionComposition> ExpressionComposition::create(
		std::shared_ptr<Expression> addr,
		Operator op,
		std::shared_ptr<Expression> val) {
  return shared_ptr<ExpressionComposition>(new MockExpressionComposition(addr, op, val));
}
