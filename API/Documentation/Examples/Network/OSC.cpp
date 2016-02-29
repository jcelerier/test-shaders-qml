/*!
 * \file OSC.cpp
 *
 * \author Clément Bossut
 * \author Théo de la Hogue
 *
 * This code is licensed under the terms of the "CeCILL-C"
 * http://www.cecill.info
 */

#include <iostream>
#include <memory>
#include <functional>

#include "Network/Address.h"
#include "Network/Device.h"
#include "Network/Protocol/OSC.h"

using namespace OSSIA;
using namespace std;

void printValueCallback(const Value * v);
void printValue(const Value * v);

int main()
{
    // declare this program "P" as an OSC device
    auto oscProtocol = OSC::create("127.0.0.1", 9996, 9997);
    auto oscDevice = Device::create(oscProtocol, "P");

    /* publish each feature of program "P" as address into a tree
     /test
     /test/my_bang
     /test/my_bool
     /test/my_int
     /test/my_float
     /test/my_string
     /test/my_destination
     /test/my_tuple
     */

    auto oscTestNode = *(oscDevice->emplace(oscDevice->children().cend(), "test"));

    auto oscImpulseNode = *(oscTestNode->emplace(oscTestNode->children().cend(), "my_impulse"));
    auto oscImpulseAddress = oscImpulseNode->createAddress(Value::Type::IMPULSE);
    oscImpulseAddress->addCallback(printValueCallback);

    auto oscBoolNode = *(oscTestNode->emplace(oscTestNode->children().cend(), "my_bool"));
    auto oscBoolAddress = oscBoolNode->createAddress(Value::Type::BOOL);
    oscBoolAddress->addCallback(printValueCallback);

    auto oscIntNode = *(oscTestNode->emplace(oscTestNode->children().cend(), "my_int"));
    auto oscIntAddress = oscIntNode->createAddress(Value::Type::INT);
    oscIntAddress->addCallback(printValueCallback);

    auto oscFloatNode = *(oscTestNode->emplace(oscTestNode->children().cend(), "my_float"));
    auto oscFloatAddress = oscFloatNode->createAddress(Value::Type::FLOAT);
    oscFloatAddress->addCallback(printValueCallback);

    auto oscStringNode = *(oscTestNode->emplace(oscTestNode->children().cend(), "my_string"));
    auto oscStringAddress = oscStringNode->createAddress(Value::Type::STRING);
    oscStringAddress->addCallback(printValueCallback);

    auto oscDestinationNode = *(oscTestNode->emplace(oscTestNode->children().cend(), "my_destination"));
    auto oscDestinationAddress = oscDestinationNode->createAddress(Value::Type::DESTINATION);
    oscDestinationAddress->addCallback(printValueCallback);

    auto oscTupleNode = *(oscTestNode->emplace(oscTestNode->children().cend(), "my_tuple"));
    auto oscTupleAddress = oscTupleNode->createAddress(Value::Type::TUPLE);
    oscTupleAddress->addCallback(printValueCallback);

    // update tree value
    Impulse n;
    oscImpulseAddress->setValue(&n);

    Bool b(true);
    oscBoolAddress->setValue(&b);

    Int i(5);
    oscIntAddress->setValue(&i);

    Float f(0.5);
    oscFloatAddress->setValue(&f);

    String s("hello world !");
    oscStringAddress->setValue(&s);

    Destination d(oscFloatNode);
    oscDestinationAddress->setValue(&d);

    Tuple t = {new Float(0.1), new Float(0.2), new Float(0.3)};
    oscTupleAddress->setValue(&t);

    while (true)
        ;
}

void printValueCallback(const Value * v)
{
    printValue(v);
    cout << "\n";
}

void printValue(const Value * v)
{
    switch (v->getType())
    {
        case Value::Type::IMPULSE :
        {
            cout << "-";
            break;
        }
        case Value::Type::BOOL :
        {
            Bool * b = (Bool*)v;
            cout << b->value;
            break;
        }
        case Value::Type::INT :
        {
            Int * i = (Int*)v;
            cout << i->value;
            break;
        }
        case Value::Type::FLOAT :
        {
            Float * f = (Float*)v;
            cout << f->value;
            break;
        }
        case Value::Type::CHAR :
        {
            Char * c = (Char*)v;
            cout << c->value;
            break;
        }
        case Value::Type::STRING :
        {
            String * s = (String*)v;
            cout << s->value;
            break;
        }
        case Value::Type::DESTINATION :
        {
            Destination * d = (Destination*)v;
            cout << d->value;
            break;
        }
        case Value::Type::TUPLE :
        {
            Tuple * t = (Tuple*)v;
            bool first = true;
            for (const auto & e : t->value)
            {
                if (!first) cout << " ";
                printValue(e);
                first = false;
            }
            break;
        }
        case Value::Type::GENERIC :
        {
            // todo
            break;
        }
        default:
            break;
    }
}
