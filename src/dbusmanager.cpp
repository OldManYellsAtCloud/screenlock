#include "dbusmanager.h"

#include <loglib/loglib.h>

DbusManager::DbusManager(QObject *parent)
    : QObject{parent}
{
    dbusProxyBtn = sdbus::createProxy(sdbus::ServiceName{BUTTON_DBUS_SERVICE_NAME}, sdbus::ObjectPath{BUTTON_DBUS_OBJECT_PATH});
    buttonSignalHandler = [&](sdbus::Signal signal){onPowerButtonReleasedSignal(signal);};
    dbusProxyBtn->registerSignalHandler(sdbus::InterfaceName{BUTTON_DBUS_INTERFACE_NAME}, sdbus::SignalName{"powerButtonRelease"}, buttonSignalHandler);

    dbusConnection = sdbus::createBusConnection(sdbus::ServiceName{DBUS_SERVICE_NAME});

    dbusObject = sdbus::createObject(*dbusConnection, sdbus::ObjectPath{DBUS_OBJECT_PATH});
    dbusObject->addVTable(sdbus::SignalVTableItem{sdbus::SignalName{"screenLocked"}, sdbus::Signature{"b"}, {}}).forInterface(sdbus::InterfaceName{DBUS_INTERFACE_NAME});



    // to avoid clashing, the screen event proxy is reusing the connection from
    // dbusConnection object, as they are using the same interface
    dbusProxyScr = sdbus::createProxy(*dbusConnection.get(), sdbus::ServiceName{DBUS_SERVICE_NAME}, sdbus::ObjectPath{DBUS_OBJECT_PATH});
    screenSignalHandler = [&](sdbus::Signal signal){onScreenStateChangedSignal(signal);};
    dbusProxyScr->registerSignalHandler(sdbus::InterfaceName{DBUS_INTERFACE_NAME}, sdbus::SignalName{"screenOff"}, screenSignalHandler);

    dbusConnection->enterEventLoopAsync();
}

void DbusManager::screenLocked()
{
    auto signal = dbusObject->createSignal(sdbus::InterfaceName{DBUS_INTERFACE_NAME}, sdbus::SignalName{"screenLocked"});
    signal << true;
    dbusObject->emitSignal(signal);
    locked = true;
}

void DbusManager::onPowerButtonReleasedSignal(sdbus::Signal &signal)
{
    LOG_INFO("Power button signal arrived");
    if (!locked){
        emit lockStateChanged(true);
    }
}

void DbusManager::onScreenStateChangedSignal(sdbus::Signal &signal)
{
    bool state;
    signal >> state;
    LOG_DEBUG_F("Screen state change signal arrived: {}", state);
    emit screenStateChanged(state);
}

void DbusManager::screenUnlocked()
{
    auto signal = dbusObject->createSignal(sdbus::InterfaceName{DBUS_INTERFACE_NAME}, sdbus::SignalName{"screenLocked"});
    signal << false;
    dbusObject->emitSignal(signal);
    locked = false;
}

void DbusManager::idleTimeout()
{
    auto signal = dbusObject->createSignal(sdbus::InterfaceName{DBUS_INTERFACE_NAME}, sdbus::SignalName{"screenLockTimeout"});
    dbusObject->emitSignal(signal);
}
