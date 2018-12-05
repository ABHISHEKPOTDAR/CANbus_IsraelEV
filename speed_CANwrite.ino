// If you're using a GPS module:
// Connect the GPS Power pin to 5V
// Connect the GPS Ground pin to ground
// If using software serial (sketch example default):
//   Connect the GPS TX (transmit) pin to Digital 3
//   Connect the GPS RX (receive) pin to Digital 2

// Libraries
#include <SPI.h>
#include <mcp2515.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

struct can_frame canMsg1;
MCP2515 mcp2515(10);

// If using software serial, keep this line enabled
// (you can change the pin numbers to match your wiring):
SoftwareSerial mySerial(3, 2);
Adafruit_GPS GPS(&mySerial);

//-----------------------------------------------------------------------//
// Current CAN Message ID = 0xEF
//-----------------------------------------------------------------------//

// Variables
int speedPin = A0;    // select the input pin for the potentiometer
int raw_speedValue = 0;  // variable to store the value coming from the sensor
int16_t speedValue =0;

uint8_t Byte5=0;
uint8_t Byte6=0;

//-----------------------------------------------------------------------//

void setup() {

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);

  while (!Serial);
  Serial.begin(115200);
  SPI.begin();
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  
  Serial.println("Example: Write to CAN");
  delay(1000);
}

void loop() {

  // Wheel speed Sensor
  speedValue = map(GPS.speed, 0, 100, 0, 65535);

  // Byte Writing 
  Byte5 = speedValue >> 8;
  Byte6 = speedValue & 0xff;
  
  // Building up the CAN message
  canMsg1.can_id  = 0xEF;
  canMsg1.can_dlc = 8;
  canMsg1.data[0] = Byte5;
  canMsg1.data[1] = Byte6;
  canMsg1.data[2] = 0x00;
  canMsg1.data[3] = 0x00;
  canMsg1.data[4] = 0x00;
  canMsg1.data[5] = 0x00;
  canMsg1.data[6] = 0x00;
  canMsg1.data[7] = 0x00;
  mcp2515.sendMessage(&canMsg1);

  for (int i = 0; i<canMsg1.can_dlc; i++)  {  // print the data
      
    Serial.print(canMsg1.data[i],HEX);
    Serial.print(" ");

  }
   Serial.println();

  delay(50);
}
