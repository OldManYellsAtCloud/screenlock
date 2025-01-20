#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <loglib/loglib.h>

#include "dbusmanager.h"

int main(int argc, char *argv[])
{
    loglib::logger().setName("screenlock");
    loglib::logger().registerLogger(logging::LOGGER_FILE);

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
