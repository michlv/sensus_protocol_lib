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
  if (result == 0) {
    result = 'x';
  }
  if (result == 0x7f) {
    result = 'y';
  }
  DEBUG_MSG("byte: %i, %c\n", result, result);
  return result;    
}
  
SensusProtocol::SensusProtocol(int clock_pin, int read_pin) : clock_pin(clock_pin), read_pin(read_pin) {}

void SensusProtocol::setup() {
  DEBUG_MSG("setup using pins: clk: %i, read: %i...\n", clock_pin, read_pin);
  pinMode(clock_pin, OUTPUT);
  digitalWrite(clock_pin, clock_OFF); // power off the meter
  if (read_pin == 3 /* RX */ ) {
    pinMode(read_pin, FUNCTION_3);
  }
  pinMode(read_pin, INPUT);
  delay(5000); // make sure that the meter is reset
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

void SensusProtocol::slowBitRead() {
  powerUp();
  while (true) {
    readBit();
    delay(5000);
  }
}
