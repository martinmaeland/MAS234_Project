

#include <FlexCAN.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#include <Fonts/FreeMono9pt7b.h>
const int JOY_LEFT = 18;
const int JOY_RIGHT = 17;
const int JOY_CLICK =19;
const int JOY_UP  = 22;
const int JOY_DOWN = 23;

#define OLED_MOSI   11
#define OLED_CLK   13
#define OLED_DC     6
#define OLED_CS     10
#define OLED_RESET  5
//Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

static CAN_message_t msg,rxmsg;
volatile uint32_t count = 0;
IntervalTimer TX_timer;
String CANStr(""); 
volatile uint32_t can_msg_count = 0;

// Variables
 int i=0;

void setup(){

  Serial.begin(9600);
  
  Can0.begin(500000); //set speed here. 
  Can1.begin(500000); //set speed here.  
  
  pinMode(JOY_LEFT, INPUT_PULLUP);
  pinMode(JOY_RIGHT, INPUT_PULLUP);
  pinMode(JOY_CLICK, INPUT_PULLUP);
  pinMode(JOY_UP, INPUT_PULLUP);
  pinMode(JOY_DOWN, INPUT_PULLUP); 
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // Clear the buffer.
  display.clearDisplay();
  
  delay(1000);
  Serial.println(F("CAN Bus Tx test"));

  updateScreen(0, 0);
  
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
  TX_timer.begin(tx_CAN, 1000000);    /* Start interrutp timer */
  
 
}

/* From Timer Interrupt */
void tx_CAN(void)
{
  msg.len = 8;
  msg.id = 0x7DF;
  msg.buf[0] = 0x02;
  msg.buf[1] = 0x01;
  msg.buf[2] = 0x0c;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;
  /*
  msg.buf[4] = can_msg_count >> 24;
  msg.buf[5] = can_msg_count >> 16;
  msg.buf[6] = can_msg_count >> 8;
  msg.buf[7] = can_msg_count;
  */
  Can0.write(msg);
  Can1.write(msg);
  can_msg_count++;
}

void loop() {
  
  while(Can0.read(rxmsg))
  { 
     i++;  
     String CANStr(""); 
     for (int i=0; i < 8; i++) {     

         CANStr += String(rxmsg.buf[i],HEX);
         CANStr += (" ") ;
     }

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
