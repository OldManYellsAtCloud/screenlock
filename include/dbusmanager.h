#ifndef DBUSMANAGER_H
#define DBUSMANAGER_H

#include <QObject>
#include <QQmlEngine>

#include <sdbus-c++/sdbus-c++.h>

#define DBUS_SERVICE_NAME   "org.gspine.display"
#define DBUS_OBJECT_PATH    "/org/gspine/display"
#define DBUS_INTERFACE_NAME "org.gspine.display"

#define BUTTON_DBUS_SERVICE_NAME   "org.gspine.hardware"
#define BUTTON_DBUS_OBJECT_PATH    "/org/gspine/hardware"
#define BUTTON_DBUS_INTERFACE_NAME "org.gspine.Hardware"

class DbusManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
private:
    void onPowerButtonReleasedSignal(sdbus::Signal& signal);
    void onScreenStateChangedSignal(sdbus::Signal& signal);
    std::function<void(sdbus::Signal)> buttonSignalHandler;
    std::function<void(sdbus::Signal)> screenSignalHandler;
    std::unique_ptr<sdbus::IProxy> dbusProxyBtn;
    std::unique_ptr<sdbus::IProxy> dbusProxyScr;

    std::unique_ptr<sdbus::IObject> dbusObject;
    std::unique_ptr<sdbus::IConnection> dbusConnection;

    bool locked;

public:
    explicit DbusManager(QObject *parent = nullptr);
    Q_INVOKABLE void screenLocked();
    Q_INVOKABLE void screenUnlocked();
    Q_INVOKABLE void idleTimeout();

signals:
    void lockStateChanged(bool state);
    void screenStateChanged(bool state);

};

#endif // DBUSMANAGER_H
