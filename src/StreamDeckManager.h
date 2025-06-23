#ifndef STREAMDECKMANAGER_H
#define STREAMDECKMANAGER_H

#include <QObject>
#include <memory>
#include <hidapi/hidapi.h>

class StreamDeckDevice;

class StreamDeckManager
{
public:
    static QList<QString> availableSerialNumbers();
    static std::unique_ptr<StreamDeckDevice> openDevice(const QString &serial, QObject *parent = nullptr);
};

#endif // STREAMDECKMANAGER_H
