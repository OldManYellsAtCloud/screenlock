#include "dbusmanager.h"

#include <loglibrary.h>

DbusManager::DbusManager(QObject *parent)
    : QObject{parent}
{
    dbusProxy = sdbus::createProxy(BUTTOND_DBUS_SERVICE_NAME, BUTTOND_DBUS_OBJECT_PATH);
    signalHandler = [&](sdbus::Signal &signal){onLockStateChanged(signal);};

    dbusProxy->registerSignalHandler(BUTTOND_DBUS_INTERFACE_NAME, "screenOn", signalHandler);
    dbusProxy->finishRegistration();

    dbusConnection = sdbus::createSystemBusConnection(DBUS_SERVICE_NAME);

    dbusObject = sdbus::createObject(*dbusConnection, DBUS_OBJECT_PATH);
    dbusObject->registerSignal(DBUS_INTERFACE_NAME, "screenLocked", "b");
    dbusObject->finishRegistration();

    dbusConnection->enterEventLoopAsync();
}

void DbusManager::screenLocked()
{
    auto signal = dbusObject->createSignal(DBUS_INTERFACE_NAME, "screenLocked");
    signal << true;
    dbusObject->emitSignal(signal);
}

void DbusManager::onLockStateChanged(sdbus::Signal &signal)
{
    bool lockState;
    signal >> lockState;
    emit lockStateChanged(lockState);
}

void DbusManager::screenUnlocked()
{
    auto signal = dbusObject->createSignal(DBUS_INTERFACE_NAME, "screenLocked");
    signal << false;
    dbusObject->emitSignal(signal);
}
