TARGET       = arduino-munin-node
ARDUINO_LIBS = SPI Ethernet Ethernet/utility

BOARD_TAG = ethernet
ARDUINO_PORT = /dev/ttyUSB0

ARDUINO_DIR = /usr/share/arduino/

# linux
AVR_TOOLS_PATH = /usr/bin/
AVRDUDE_CONF = /etc/avrdude.conf

include Arduino.mk
