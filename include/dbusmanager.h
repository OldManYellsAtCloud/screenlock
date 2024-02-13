#ifndef DBUSMANAGER_H
#define DBUSMANAGER_H

#include <QObject>
#include <QQmlEngine>

#include <sdbus-c++/sdbus-c++.h>

class DbusManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
private:
    void onLockStateChanged(sdbus::Signal& signal);
    std::function<void(sdbus::Signal&)> signalHandler;
    std::unique_ptr<sdbus::IProxy> dbusProxy;

public:
    explicit DbusManager(QObject *parent = nullptr);

signals:
    void lockStateChanged(bool state);
};

#endif // DBUSMANAGER_H
