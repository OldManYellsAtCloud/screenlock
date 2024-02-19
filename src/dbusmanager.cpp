#include "dbusmanager.h"

#include <loglibrary.h>

DbusManager::DbusManager(QObject *parent)
    : QObject{parent}
{
    dbusProxy = sdbus::createProxy(BUTTON_DBUS_SERVICE_NAME, BUTTON_DBUS_OBJECT_PATH);
    signalHandler = [&](sdbus::Signal &signal){onPowerButtonReleased(signal);};

    dbusProxy->registerSignalHandler(BUTTON_DBUS_INTERFACE_NAME, "powerButtonRelease", signalHandler);
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

void DbusManager::onPowerButtonReleased(sdbus::Signal &signal)
{
    LOG("Power button press signal arrived");
    emit lockStateChanged(true);
}

void DbusManager::screenUnlocked()
{
    auto signal = dbusObject->createSignal(DBUS_INTERFACE_NAME, "screenLocked");
    signal << false;
    dbusObject->emitSignal(signal);
}
