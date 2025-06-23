#include <QCoreApplication>
#include <QImage>
#include "StreamDeckManager.h"
#include "StreamDeckDevice.h"
#include <iostream>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    auto serials = StreamDeckManager::availableSerialNumbers();
    if (serials.isEmpty()) {
        std::cerr << "No Stream Deck found" << std::endl;
        return 0;
    }

    auto deck = StreamDeckManager::openDevice(serials.first());
    if (!deck) {
        std::cerr << "Failed to open device" << std::endl;
        return 0;
    }

    QObject::connect(deck.get(), &StreamDeckDevice::buttonPressed, [](int key, bool pressed){
        std::cout << "Key " << key << (pressed ? " down" : " up") << std::endl;
    });

    deck->start();

    QImage img(72,72,QImage::Format_RGB888);
    img.fill(Qt::blue);
    deck->setImage(0, img);

    return app.exec();
}
