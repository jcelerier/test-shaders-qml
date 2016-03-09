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
template<typename T>
auto make_parameter(std::string name, T val)
{
  Parameter p;
  p.destination = name;
  p.value = val;

  return p;
}
void Publisher::init() {

    auto p1 = make_parameter("/blurRadius", 0.f);
    p1.min = 0.f;
    p1.max = 64.f;

    auto p2 = make_parameter("/clear", coppa::ossia::Impulse{});
    auto p3 = make_parameter("/mousePos/x_pos", 0.f);
    auto p4 = make_parameter("/mousePos/y_pos", 0.f);

    auto p5 = make_parameter("/mousePos/clicked", coppa::ossia::Impulse{});
    auto p6 = make_parameter("/mousePos/released", coppa::ossia::Impulse{});

    m_base_map.insert(p1);
    m_base_map.insert(p2);
    m_base_map.insert(p3);
    m_base_map.insert(p4);
    m_base_map.insert(p5);
    m_base_map.insert(p6);

    m_dev.on_value_changed.connect<Publisher, &Publisher::handleChange>(this);
}

void Publisher::handleChange(const coppa::ossia::Parameter &p)
{
    if(p.destination == "/blurRadius")
        emit blurChanged(eggs::variants::get<float>(p.value));
    else if(p.destination == "/clear")
        emit clear();
}

void Publisher::relayChanges(const QPointF& newXYValue)
{
    m_dev.push("/mousePos/x_pos", float(newXYValue.x()));
    m_dev.push("/mousePos/y_pos", float(newXYValue.y()));
}

void Publisher::clicked(const QPointF&)
{
    m_dev.push("/mousePos/clicked", coppa::ossia::Impulse{});
}

void Publisher::released(const QPointF&)
{
    m_dev.push("/mousePos/released", coppa::ossia::Impulse{});
}

