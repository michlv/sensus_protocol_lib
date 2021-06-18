#include "SensusProtocol.h"
#include <String>

#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define DEBUG_MSG(...)
#endif

void SensusProtocol::powerUp() {
  digitalWrite(clock_pin, clock_ON); // power on meter
  delay(3000);
}

void SensusProtocol::powerDown() {
  digitalWrite(clock_pin, clock_OFF); // power on meter
}

int SensusProtocol::readBit() {
  digitalWrite(clock_pin, clock_OFF);
  delay(1);
  digitalWrite(clock_pin, clock_ON);
  delay(1); // Seems to work even without it, but just for sure
  int val = digitalRead(read_pin); // LOW is 1 and HIGH is 0
  DEBUG_MSG("bit: %i\n", val);
  return val;
}

char SensusProtocol::readByte() {
  int bits[10];
  //while (readBit() != 1);
  for (int i = 0; i < 10; ++i) {
    bits[i] = readBit();
  }
  char result = 0;
  int startIdx = 1;
  int numOfBits = 7;
  for (int b = 0; b < 7; ++b) {
    int i = b + startIdx;
    if (bits[i]) {
      result |= (1 << b);
    }
  }
  DEBUG_MSG("byte: %i, %c\n", result, result);
  return result;    
}
  
SensusProtocol::SensusProtocol(int clock_pin, int read_pin, bool read_pin_pullup)
 : clock_pin(clock_pin), read_pin(read_pin), read_pin_pullup(read_pin_pullup) {}

void SensusProtocol::setup(int reset_wait) {
  DEBUG_MSG("setup using pins: clk: %i, read: %i, read_pullup: %i ...\n", clock_pin, read_pin, read_pin_pullup);
  pinMode(clock_pin, OUTPUT);
  digitalWrite(clock_pin, clock_OFF); // power off the meter
  auto input = INPUT;
  if (read_pin_pullup) {
    input = INPUT_PULLUP;
  }
  pinMode(read_pin, input);
  delay(reset_wait); // make sure that the meter is reset
}

int SensusProtocol::getClockPin() const { return clock_pin; }

String SensusProtocol::readData() {    
  String result;
  powerUp();
  for (int i = 0; i < 50; ++i) {
    char c = readByte();
    if (c == '\r')
      break;
    result += c;
  }
  powerDown();
  return result;
}

void SensusProtocol::slowBitRead(int wait) {
  powerUp();
  while (true) {
    readBit();
    delay(wait);
  }
}
