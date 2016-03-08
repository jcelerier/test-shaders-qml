#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <QObject>
#include <QString>
#include <QtQml>
#include <coppa/ossia/device/minuit_listening_local_device.hpp>
#include <coppa/map.hpp>

#include "transmitter.h"
using namespace coppa;
using namespace coppa::ossia;
class Publisher : public Transmitter
{
        Q_OBJECT

    public:
        explicit Publisher(int, int);
        Q_INVOKABLE void relayChanges(const QPointF&);
        Q_INVOKABLE void clicked(const QPointF&);
        Q_INVOKABLE void released(const QPointF&);
        void init();

    signals:
        void blurChanged(double value);
        void clear();

    private:

        basic_map<ParameterMapType<Parameter>> m_base_map;
        locked_map<basic_map<ParameterMapType<Parameter>>> m_map{m_base_map};
        minuit_listening_local_device m_dev;
};

#endif // PUBLISHER_H
