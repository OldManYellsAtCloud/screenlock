#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "dbusmanager.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<DbusManager>("sgy.pine.screenlock", 1, 0, "DbusManager");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/screenlock/Main.qml"_qs);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
