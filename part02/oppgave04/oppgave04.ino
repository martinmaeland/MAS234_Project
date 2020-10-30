// Del02, oppgave04a

// Include
#include <FlexCAN.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono9pt7b.h>

// Define
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_MOSI   11
#define OLED_CLK   13
#define OLED_DC     6
#define OLED_CS     10
#define OLED_RESET  5
//Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

int i = 0;

bool recieve = false;

// Gyro
const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

static CAN_message_t msg,rxmsg;
volatile uint32_t count = 0;
IntervalTimer TX_timer;
String CANStr(""); 
volatile uint32_t can_msg_count = 0;

void setup(){
  
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  updateScreen(0, 0, 0);
  
  Can0.begin(500000); //set speed here. 
    
  delay(1000);
  
  msg.buf[0] = 1;
  msg.buf[1] = 2;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.len = 8;
  msg.id = 0x7DF;
  msg.flags.extended = 0; 
  msg.flags.remote = 0;
//  msg.timeout = 500;
/* Start interrutp timer */
  TX_timer.begin(tx_gyro, 1000000);
}

void loop() {

  updateScreen(i, rxmsg.id, gyroZ());

  while(Can0.read(rxmsg))
  { 
     i++;
     String CANStr(""); 
     for (int i=0; i < 8; i++) {     
         CANStr += String(rxmsg.buf[i],HEX);
         CANStr += (" ") ;
     }

     Serial.print(rxmsg.id);
     if (rxmsg.id == 0x21) {
        recieve = true;
     }
     updateScreen(i, rxmsg.id, gyroZ());
  }
    
}

// Functions

void tx_gyro(void) {

  if (recieve == true) {
    msg.len = 8;
    msg.id = 0x22;
    int value = (int)(gyroZ()*10);
    if (value < 0) {
      msg.buf[0] = 0;
      msg.buf[1] = abs(value);
    } else {
      msg.buf[0] = value;
      msg.buf[1] = 0;
    }
    msg.buf[2] = 0;
    msg.buf[3] = 0;
    msg.buf[4] = 0;
    msg.buf[5] = 0;
    msg.buf[6] = 0;
    msg.buf[7] = 0;
    Can0.write(msg);
    can_msg_count++;
  }
}

void updateScreen(int msgCount, int msgAddr, double gyro) {

  display.clearDisplay();
 
  display.setTextSize(0);
  display.setTextColor(WHITE);
    
  display.setCursor(0,0);
  
  display.drawRoundRect(0,0, 128,64,6,WHITE);
  display.setCursor(13,5);
  display.println("MAS234 - gruppe 5");
  display.setCursor(5,15);
  display.println("CAN statistikk");
  display.setCursor(5,21);
  display.println("-----------------");
  display.setCursor(5,28);
  display.print("Antall motatt:  ");
  display.print(msgCount);
  display.setCursor(5,38);
  display.print("Mottok sist ID: ");
  display.print(msgAddr);
  display.setCursor(5,45);
  display.println("-----------------");
  display.setCursor(5,53);
  display.print("IMU-Z maaling: ");
  display.print(gyro);
  display.display();
}

double gyroZ() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true); 

  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();
  delay(333);
  return (AcZ/15500.0) * 9.81;
}
