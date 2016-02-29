#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "publisher.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    int p = DEFAULT_PPORT, r = DEFAULT_RPORT;
    if (argc == 3) {
        p = atoi(argv[1]);
        r = atoi(argv[2]);
    }

    Publisher pub(p, r);
    pub.init();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("publisher", &pub);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    return app.exec();
}
