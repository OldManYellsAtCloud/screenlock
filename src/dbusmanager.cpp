#include "dbusmanager.h"

#include <loglibrary.h>

DbusManager::DbusManager(QObject *parent)
    : QObject{parent}
{
    dbusProxyBtn = sdbus::createProxy(BUTTON_DBUS_SERVICE_NAME, BUTTON_DBUS_OBJECT_PATH);
    buttonSignalHandler = [&](sdbus::Signal &signal){onPowerButtonReleasedSignal(signal);};
    dbusProxyBtn->registerSignalHandler(BUTTON_DBUS_INTERFACE_NAME, "powerButtonRelease", buttonSignalHandler);
    dbusProxyBtn->finishRegistration();

    dbusConnection = sdbus::createSystemBusConnection(DBUS_SERVICE_NAME);

    dbusObject = sdbus::createObject(*dbusConnection, DBUS_OBJECT_PATH);
    dbusObject->registerSignal(DBUS_INTERFACE_NAME, "screenLocked", "b");
    dbusObject->finishRegistration();

    // to avoid clashing, the screen event proxy is reusing the connection from
    // dbusConnection object, as they are using the same interface
    dbusProxyScr = sdbus::createProxy(*dbusConnection.get(), DBUS_SERVICE_NAME, DBUS_OBJECT_PATH);
    screenSignalHandler = [&](sdbus::Signal &signal){onScreenStateChangedSignal(signal);};
    dbusProxyScr->registerSignalHandler(DBUS_INTERFACE_NAME, "screenOff", screenSignalHandler);
    dbusProxyScr->finishRegistration();

    dbusConnection->enterEventLoopAsync();
}

void DbusManager::screenLocked()
{
    auto signal = dbusObject->createSignal(DBUS_INTERFACE_NAME, "screenLocked");
    signal << true;
    dbusObject->emitSignal(signal);
    locked = true;
}

void DbusManager::onPowerButtonReleasedSignal(sdbus::Signal &signal)
{
    LOG("Power button signal arrived");
    if (!locked){
        emit lockStateChanged(true);
    }
}

void DbusManager::onScreenStateChangedSignal(sdbus::Signal &signal)
{
    bool state;
    signal >> state;
    DBG("Screen state change signal arrived: {}", state);
    emit screenStateChanged(state);
}

void DbusManager::screenUnlocked()
{
    auto signal = dbusObject->createSignal(DBUS_INTERFACE_NAME, "screenLocked");
    signal << false;
    dbusObject->emitSignal(signal);
    locked = false;
}

void DbusManager::idleTimeout()
{
    auto signal = dbusObject->createSignal(DBUS_INTERFACE_NAME, "screenLockTimeout");
    dbusObject->emitSignal(signal);
}
