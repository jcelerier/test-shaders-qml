#include <unistd.h>
#include <iostream>

#include "receiver.h"

using namespace OSSIA;
using namespace std;

Receiver::Receiver() : Transmitter() {}
Receiver::Receiver(int pPort, int rPort) : Transmitter(pPort, rPort) {}

bool Receiver::updateNamespace() {
    return _minuitDevice->updateNamespace();
}

void Receiver::init() {
    _localProtocol = Local::create();
    _localDevice = Device::create(_localProtocol, "Receiver");

    _minuitProtocol = Minuit::create("127.0.0.1", _recPort, _pubPort);
    _minuitDevice = Device::create(_minuitProtocol, "Publisher");

    this->updateNamespace();

    _posNode = _minuitDevice->children()[0];
    if (_posNode) {
        _xNode = _posNode->children()[0];
        _xAddress = _xNode->getAddress();
        _yNode = _posNode->children()[1];
        _yAddress = _yNode->getAddress();

        _xAddress->addCallback(printXValue);
        _yAddress->addCallback(printYValue);
    }
}

void printXValue(const Value* v) {
    Float * val = (Float *) v;
    cout << "x: " << val->value << ", ";
}

void printYValue(const Value* v) {
    Float * val = (Float *) v;
    cout << "y: " << val->value << endl;
}

int main(int argc, char *argv[]) {
    int p = DEFAULT_PPORT, r = DEFAULT_RPORT;
    if (argc == 3) {
        p = atoi(argv[1]);
        r = atoi(argv[2]);
    }
    Receiver rec(p, r);
    rec.init();
    while(true) {
        sleep(1);
    }
    return 0;
}
