#include "dbusmanager.h"

#include <loglibrary.h>

DbusManager::DbusManager(QObject *parent)
    : QObject{parent}
{
    const std::string destinationName = "sgy.pine.screenLock";
    const std::string objectPath = "/sgy/pine/screenLock";
    dbusProxy = sdbus::createProxy(destinationName, objectPath);

    const std::string interfaceName = "sgy.pine.screenLock";

    signalHandler = [&](sdbus::Signal &signal){onLockStateChanged(signal);};

    dbusProxy->registerSignalHandler(interfaceName, "lockStateChanged", signalHandler);
    dbusProxy->finishRegistration();
}

void DbusManager::onLockStateChanged(sdbus::Signal &signal)
{
    bool lockState;
    signal >> lockState;
    emit lockStateChanged(lockState);
}
