#include "StreamDeckDevice.h"
#include <QBuffer>
#include <QImageReader>
#include <QSvgRenderer>
#include <QPainter>

StreamDeckReadThread::StreamDeckReadThread(hid_device *dev, QObject *parent)
    : QThread(parent), m_dev(dev)
{
}

void StreamDeckReadThread::stop()
{
    m_running = false;
}

void StreamDeckReadThread::run()
{
    while (m_running) {
        unsigned char buf[64];
        int res = hid_read(m_dev, buf, sizeof(buf));
        if (res > 0) {
            int key = buf[1];
            bool state = buf[2];
            emit buttonEvent(key, state);
        } else if (res < 0) {
            break;
        }
    }
}

StreamDeckDevice::StreamDeckDevice(const QString &serial, hid_device *dev, QObject *parent)
    : QObject(parent), m_serial(serial), m_device(dev)
{
}

StreamDeckDevice::~StreamDeckDevice()
{
    stop();
    if (m_device)
        hid_close(m_device);
}

QString StreamDeckDevice::serialNumber() const
{
    return m_serial;
}

bool StreamDeckDevice::start()
{
    if (!m_device || m_thread)
        return false;
    m_thread = new StreamDeckReadThread(m_device);
    connect(m_thread, &StreamDeckReadThread::buttonEvent, this, &StreamDeckDevice::buttonPressed);
    m_thread->start();
    return true;
}

void StreamDeckDevice::stop()
{
    if (!m_thread)
        return;
    m_thread->stop();
    m_thread->wait();
    delete m_thread;
    m_thread = nullptr;
}

static QImage renderSvg(const QString &path, const QSize &size)
{
    QSvgRenderer renderer(path);
    QImage image(size, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    QPainter p(&image);
    renderer.render(&p);
    return image;
}

static QByteArray prepareImage(const QImage &img)
{
    QImage rgb = img.convertToFormat(QImage::Format_RGB888);
    QByteArray data(reinterpret_cast<const char*>(rgb.constBits()), rgb.sizeInBytes());
    return data;
}

void StreamDeckDevice::setImage(int keyIndex, const QImage &image)
{
    if (!m_device)
        return;
    QImage scaled = image.scaled(72, 72, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QByteArray payload = prepareImage(scaled);

    QByteArray packet;
    packet.resize(1 + payload.size());
    packet[0] = static_cast<char>(keyIndex);
    std::copy(payload.begin(), payload.end(), packet.begin() + 1);

    hid_write(m_device, reinterpret_cast<const unsigned char*>(packet.constData()), packet.size());
}
