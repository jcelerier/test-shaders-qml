#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <QObject>
#include <QString>
#include <QtQml>

#include "transmitter.h"
namespace OSSIA
{
class Address;
}
class Publisher : public Transmitter
{
        Q_OBJECT

    public:
        explicit Publisher();
        explicit Publisher(int, int);
        Q_INVOKABLE void relayChanges(const QPointF&);
        Q_INVOKABLE void clicked(const QPointF&);
        Q_INVOKABLE void released(const QPointF&);
        void init();

    signals:
        void blurChanged(double value);
        void clear();

    private:
        std::shared_ptr<OSSIA::Address> _clicked;
        std::shared_ptr<OSSIA::Address> _released;
};

void printValueCallback(const OSSIA::Value*);

#endif // PUBLISHER_H
