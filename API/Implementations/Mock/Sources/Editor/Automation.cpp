#include "TimeProcess.cpp"
#include "../Misc/Container.cpp"
#include "Editor/Automation.h"
#include "Network/AddressValue.h"

using namespace OSSIA;
using namespace std;

class MockAutomation :
    public Automation<double>, public MockTimeProcess {

public:

  // Dummy members
  shared_ptr<Curve<double>> curve;
  shared_ptr<Address> addr;

  // Constructors, destructor, cloning
  MockAutomation() {}
  MockAutomation(const MockAutomation * other) :
      curve{other->curve},
      addr{other->addr}  {}

  virtual ~MockAutomation() {}
  virtual shared_ptr<Automation<double>> clone() const override {
    return shared_ptr<Automation<double>>(new MockAutomation(this));
  }

  // Lecture
  virtual void play(bool log = false, string name = "") const override {}

  // Accessors

  virtual AddressValue getStartValue() const override {return AddressValue();}
  virtual void setStartValue(AddressValue) override {}
  virtual AddressValue getEndValue() const override {return AddressValue();}
  virtual void setEndValue(AddressValue) override {}
  virtual const shared_ptr<Curve<double>> & getCurve() const override {return curve;}
  virtual void setCurve(shared_ptr<Curve<double>> cv) override {curve = cv;}
  virtual const shared_ptr<Address> & getInputAddress() const override {return addr;}
  virtual void setInputAddress(shared_ptr<Address> a) override {addr = a;}
    //TODO : why 'input'address -> if mapping
};

namespace OSSIA {
    template<> shared_ptr<Automation<double>> Automation<double>::create() {
      return shared_ptr<Automation<double>>(new MockAutomation());
    }
}
