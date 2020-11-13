#include <FlexCAN.h>

#ifndef __MK66FX1M0__
  #error "Teensy 3.6 with dual CAN bus is required to run this example"
#endif

void setup() {
  
  delay(1000);
  Can0.begin(); // Starter CAN-kommunikasjonen
  
}

void loop() {
  
  CAN_message_t inMsg; // Definerer en CAN-melding
  
  while (Can0.available()) // Leser meldinger når det er meldinger å lese
  {
    Can0.read(inMsg); // Skriver inkommende melding til inMsg
    Serial.print("CAN bus 0: "); 
    Serial.println(inMsg.buf); // Printer meldingen til serial-monitoren
}
