# QtStreamDeck

QtStreamDeck is a Qt 5.15 C++ wrapper around the Elgato Stream Deck HID
protocol. It allows applications to send images to keys and receive button
press events. Multiple Stream Decks can be used simultaneously and are
identified by their serial numbers.

## Building

This project uses CMake. Qt 5.15 and `hidapi` are required.

```bash
mkdir build && cd build
cmake ..
make
```

The `examples` directory contains a small example showing how to use the
library.
