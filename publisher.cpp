#include "publisher.h"
#include <sstream>
#include <string>
#include <QDebug>
#include <iostream>
using namespace std;

Publisher::Publisher(int pPort, int rPort) : Transmitter(pPort, rPort),
    m_dev(m_base_map, "newDevice", 9998, "127.0.0.1", 13579)
{

}

void Publisher::init() {
    /*
    _localProtocol = Local::create();
    _localDevice = Device::create(_localProtocol, "Publisher");

    {
        std::shared_ptr<OSSIA::Node> blurNode = *(_localDevice->emplace(_localDevice->children().cend(), "blurRadius"));
        auto blurAddr = blurNode->createAddress(Value::Type::FLOAT);
        blurAddr->setDomain(OSSIA::Domain::create(new OSSIA::Float(0), new OSSIA::Float(64)));
        blurAddr->addCallback([=] (const OSSIA::Value* val) {
            if(val->getType() == OSSIA::Value::Type::FLOAT)
                emit blurChanged(static_cast<const OSSIA::Float*>(val)->value);
        });
    }

    {
        std::shared_ptr<OSSIA::Node> clearNode = *(_localDevice->emplace(_localDevice->children().cend(), "clear"));
        auto clearAddr = clearNode->createAddress(Value::Type::IMPULSE);
        clearAddr->addCallback([=] (const OSSIA::Value* val) {
            emit clear();
        });
    }

    _posNode = *(_localDevice->emplace(_localDevice->children().cend(), "mousePos"));
    _xNode = *(_posNode->emplace(_posNode->children().cend(), "x_pos"));
    _yNode = *(_posNode->emplace(_posNode->children().cend(), "y_pos"));
    auto click_n = *(_posNode->emplace(_posNode->children().cend(), "clicked"));
    auto rel_n = *(_posNode->emplace(_posNode->children().cend(), "released"));

    _xAddress = _xNode->createAddress(Value::Type::FLOAT);
    _yAddress = _yNode->createAddress(Value::Type::FLOAT);
    _clicked = click_n->createAddress(Value::Type::IMPULSE);
    _released = rel_n->createAddress(Value::Type::IMPULSE);

    _minuitProtocol = Minuit::create("127.0.0.1", _pubPort, _recPort);
    _minuitDevice = Device::create(_minuitProtocol, "i-score");*/
}

void Publisher::relayChanges(const QPointF& newXYValue)
{/*
    OSSIA::Float x(newXYValue.x());
    OSSIA::Float y(newXYValue.y());

    _xAddress->pushValue(&x);
    _yAddress->pushValue(&y);*/
}

void Publisher::clicked(const QPointF& newXYValue)
{/*
    OSSIA::Impulse i;

    _clicked->pushValue(&i);

    relayChanges(newXYValue);*/
}

void Publisher::released(const QPointF& newXYValue)
{/*
    OSSIA::Impulse i;

    _released->pushValue(&i);

    relayChanges(newXYValue);*/
}

