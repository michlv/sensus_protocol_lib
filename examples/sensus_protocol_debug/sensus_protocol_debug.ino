#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define DEBUG_MSG(...)
#endif

#include <SensusProtocol.h>

SensusProtocol meter;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(3000);
  DEBUG_MSG("setup...\n");
  meter.setup();
  DEBUG_MSG("setup done...\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  DEBUG_MSG("entering loop...\n");
  String data = meter.readData();
  DEBUG_MSG("data: %i, %s\n", data.length(), data.c_str());
  delay(15000);
}