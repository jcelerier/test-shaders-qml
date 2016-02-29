#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>

#include "transmitter.h"

class Receiver : public Transmitter
{
    Q_OBJECT

public:
    explicit Receiver();
    explicit Receiver(int, int);
    bool updateNamespace();
    void init();

signals:

public slots:

};

void printXValue(const OSSIA::Value*);
void printYValue(const OSSIA::Value*);

#endif // RECEIVER_H
