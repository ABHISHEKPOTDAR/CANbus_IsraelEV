#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include <Adafruit_SSD1306.h>
#include <splash.h>

#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(10);

// voltage message ID = 0xAB
// current message ID = 0xCD
// wheel speed sensor message ID = 0xEF

uint16_t voltage_raw = 0;
uint16_t current_raw = 0;
uint16_t speed_raw = 0;
float voltage = 0;
float current = 0;
float wheelspeed = 0;

// for display
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  Serial.begin(115200);
  SPI.begin();
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  
  Serial.println("------- CAN Read ----------");
  Serial.println("ID  DLC   DATA");
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();
}

void loop() {
  
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {

    Serial.print(canMsg.can_id, HEX); // print ID
    Serial.print(" "); 
    Serial.print(canMsg.can_dlc, HEX); // print DLC
    Serial.print(" ");
    
    for (int i = 0; i<canMsg.can_dlc; i++)  {  // print the data
      Serial.print(canMsg.data[i],HEX);
      Serial.print(" ");
    }
    Serial.println();

    // reading first 2 bytes of raw data for voltage 
    voltage_raw = canMsg.data[1];
    voltage_raw <<= 8;
    voltage_raw = voltage_raw | canMsg.data[2];

    voltage = (52.0/65535.0) * voltage_raw;
    
    // reading next 2 bytes of raw data for current
    // current_raw is the current sensor voltage
    current_raw = canMsg.data[3];
    current_raw <<= 8;
    current_raw = current_raw | canMsg.data[4];
    
    current = (15.0 / 65535.0)* current_raw;// Read the voltage from CAN raw values
    
    display_data();
    delay(50); 
  }
}

void display_data(){
  display.clearDisplay();
  display.setTextColor(WHITE);
  
  //wheel Speed
  display.setCursor(20,0);
  display.setTextSize(2);
  display.println(wheelspeed);

  Serial.print("wheelspeed I: ");
  Serial.print(wheelspeed,DEC); // print the current with 2 decimal places
  Serial.print("\t");

  //current
  display.setCursor(0,20);
  display.setTextSize(1);
  display.print("I : ");
  display.println(current);

  Serial.print("Current I: ");
  Serial.print(current,2); // print the current with 2 decimal places
  Serial.print("\t");

  // voltage
  display.setCursor(60,20);
  display.setTextSize(1);
  display.print("V : ");
  display.println(voltage);

  Serial.print("Voltage V: ");
  Serial.print(voltage,2); // print the current with 2 decimal places
  Serial.println("\t");
  
  // display & clear the buffer
  display.display();
  display.clearDisplay();  
 }
