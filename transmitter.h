#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <QObject>
#include <memory>
/*
#include "ossia_includes.h"
*/
#define DEFAULT_PPORT 6666
#define DEFAULT_RPORT 9999

class Transmitter : public QObject
{
    Q_OBJECT

public:
    Transmitter();
    Transmitter(int, int);
    virtual void init() = 0;

protected:/*
    std::shared_ptr<OSSIA::Local> _localProtocol;
    std::shared_ptr<OSSIA::Device> _localDevice;

    std::shared_ptr<OSSIA::Minuit> _minuitProtocol;
    std::shared_ptr<OSSIA::Device> _minuitDevice;

    std::shared_ptr<OSSIA::Node> _posNode;
    std::shared_ptr<OSSIA::Node> _xNode;
    std::shared_ptr<OSSIA::Node> _yNode;

    std::shared_ptr<OSSIA::Address> _xAddress;
    std::shared_ptr<OSSIA::Address> _yAddress;
*/
    int _pubPort;
    int _recPort;

signals:

public slots:

};

#endif // TRANSMITTER_H
