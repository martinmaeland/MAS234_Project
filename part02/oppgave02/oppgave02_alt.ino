#include <FlexCAN.h>

#ifndef __MK66FX1M0__
  #error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif

// Function init
static void hexDump(uint8_t dumpLen, uint8_t *bytePtr);

static CAN_message_t msg;
static uint8_t hex[17] = "0123456789abcdef";

void setup() {
    delay(1000);
  Serial.println(F("Hello Teensy 3.6 dual CAN Test."));

  Can0.begin();  
  Can1.begin();

  //if using enable pins on a transceiver they need to be set on
  pinMode(2, OUTPUT);
  pinMode(35, OUTPUT);

  digitalWrite(2, HIGH);
  digitalWrite(35, HIGH);

  msg.ext = 0;
  msg.id = 0x100;
  msg.len = 8;
  msg.buf[0] = 10;
  msg.buf[1] = 20;
  msg.buf[2] = 0;
  msg.buf[3] = 100;
  msg.buf[4] = 128;
  msg.buf[5] = 64;
  msg.buf[6] = 32;
  msg.buf[7] = 16;

}

void loop() {
  CAN_message_t inMsg;
  while (Can0.available()) 
  {
    Can0.read(inMsg);
    Serial.print("CAN bus 0: "); hexDump(8, inMsg.buf);
  }
  msg.buf[0]++;
  Can1.write(msg);
  msg.buf[0]++;
  Can1.write(msg);
  msg.buf[0]++;
  Can1.write(msg);
  msg.buf[0]++;
  Can1.write(msg);
  msg.buf[0]++;
  Can1.write(msg);  
  delay(20);

}

static void hexDump(uint8_t dumpLen, uint8_t *bytePtr)
{
  uint8_t working;
  while( dumpLen-- ) {
    working = *bytePtr++;
    Serial.write( hex[ working>>4 ] );
    Serial.write( hex[ working&15 ] );
  }
  Serial.write('\r');
  Serial.write('\n');
}
