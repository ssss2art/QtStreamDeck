#include "StreamDeckManager.h"
#include "StreamDeckDevice.h"
#include <QString>
#include <QList>

QList<QString> StreamDeckManager::availableSerialNumbers()
{
    QList<QString> serials;
    hid_device_info *info = hid_enumerate(0x0, 0x0);
    hid_device_info *cur = info;
    while (cur) {
        QString manufacturer = QString::fromWCharArray(cur->manufacturer_string ? cur->manufacturer_string : L"");
        QString product = QString::fromWCharArray(cur->product_string ? cur->product_string : L"");
        if (manufacturer.contains("Elgato", Qt::CaseInsensitive) && product.contains("Stream Deck", Qt::CaseInsensitive)) {
            if (cur->serial_number)
                serials.append(QString::fromWCharArray(cur->serial_number));
        }
        cur = cur->next;
    }
    hid_free_enumeration(info);
    return serials;
}

std::unique_ptr<StreamDeckDevice> StreamDeckManager::openDevice(const QString &serial, QObject *parent)
{
    hid_device *dev = hid_open(0x0fd9, 0x0060, serial.isEmpty() ? nullptr : reinterpret_cast<const wchar_t*>(serial.utf16()));
    if (!dev)
        return nullptr;
    return std::make_unique<StreamDeckDevice>(serial, dev, parent);
}
