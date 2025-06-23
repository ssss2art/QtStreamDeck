#ifndef STREAMDECKDEVICE_H
#define STREAMDECKDEVICE_H

#include <QObject>
#include <QImage>
#include <QThread>
#include <hidapi/hidapi.h>
#include <atomic>

class StreamDeckReadThread : public QThread
{
    Q_OBJECT
public:
    explicit StreamDeckReadThread(hid_device *dev, QObject *parent = nullptr);
    void run() override;
    void stop();

signals:
    void buttonEvent(int keyIndex, bool pressed);

private:
    hid_device *m_dev;
    std::atomic<bool> m_running{true};
};

class StreamDeckDevice : public QObject
{
    Q_OBJECT
public:
    explicit StreamDeckDevice(const QString &serial, hid_device *dev, QObject *parent = nullptr);
    ~StreamDeckDevice();

    QString serialNumber() const;

    bool start();
    void stop();

    void setImage(int keyIndex, const QImage &image);

signals:
    void buttonPressed(int keyIndex, bool pressed);

private:
    QString m_serial;
    hid_device *m_device{nullptr};
    StreamDeckReadThread *m_thread{nullptr};
};

#endif // STREAMDECKDEVICE_H
