#ifndef DBUSMANAGER_H
#define DBUSMANAGER_H

#include <QObject>
#include <QQmlEngine>

#include <sdbus-c++/sdbus-c++.h>

#define DBUS_SERVICE_NAME   "sgy.pine.screenLock"
#define DBUS_OBJECT_PATH    "/sgy/pine/screenLock"
#define DBUS_INTERFACE_NAME "sgy.pine.screenLock"

#define BUTTOND_DBUS_SERVICE_NAME   "sgy.pine.buttond"
#define BUTTOND_DBUS_OBJECT_PATH    "/sgy/pine/buttond"
#define BUTTOND_DBUS_INTERFACE_NAME "sgy.pine.buttond"


class DbusManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
private:
    void onLockStateChanged(sdbus::Signal& signal);
    std::function<void(sdbus::Signal&)> signalHandler;
    std::unique_ptr<sdbus::IProxy> dbusProxy;

    std::unique_ptr<sdbus::IObject> dbusObject;
    std::unique_ptr<sdbus::IConnection> dbusConnection;

public:
    explicit DbusManager(QObject *parent = nullptr);
    Q_INVOKABLE void screenLocked();
    Q_INVOKABLE void screenUnlocked();

signals:
    void lockStateChanged(bool state);

};

#endif // DBUSMANAGER_H
