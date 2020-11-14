#include <FlexCAN.h>
#include <SPI.h> // SPI communication
//#include <Wire.h> // I2C communication
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono9pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_MOSI   11
#define OLED_CLK   13
#define OLED_DC     6
#define OLED_CS     10
#define OLED_RESET  5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, 
                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#if (SSD1306_LCDHEIGHT != 64)
  #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

static CAN_message_t msg, rxmsg;
volatile uint32_t count = 0;
IntervalTimer TX_timer;
String CANStr(""); 
volatile uint32_t can_msg_count = 0;

// Functions
void updateScreen(int msgCount, int msgAddr);

// Variables
int i = 0;

void setup(){

  Serial.begin(9600);
  
  Can0.begin(500000); //set speed here.  
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // Clear the buffer.
  display.clearDisplay();
  
  delay(1000);

  updateScreen(0, 0);
}

void loop() {
  
  while(Can0.read(rxmsg))
  { 
     i++;  
     updateScreen(i, rxmsg.id);
  }
  
}

void updateScreen(int msgCount, int msgAddr) {
  display.setTextSize(0);
  display.setTextColor(WHITE);
  display.setCursor(0,5);
  
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
  display.println("IMU-Z maaling:  ");
  display.display();
  display.clearDisplay();
}
