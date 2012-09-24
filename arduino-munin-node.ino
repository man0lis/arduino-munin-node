#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1, 123);
IPAddress gateway(192,168,1, 1);
IPAddress subnet(255, 255, 255, 0);

// munin default to port 4949
EthernetServer server(4949);

void setup() {
  // initialize the ethernet device
  Ethernet.begin(mac, ip, gateway, subnet);
  // start listening for clients
  server.begin();
}

void loop() {
}
