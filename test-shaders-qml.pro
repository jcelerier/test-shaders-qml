TEMPLATE = app

QT += qml quick
CONFIG += c++11
QMAKE_CXXFLAGS += -Ofast -march=armv7-a
SOURCES += main.cpp \
    publisher.cpp \
    transmitter.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    publisher.h \
    ossia_includes.h \
    transmitter.h
