// Libraries
#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg1;
MCP2515 mcp2515(10);

//-----------------------------------------------------------------------//
// Voltage CAN message ID = 0xAB
//-----------------------------------------------------------------------//

// Variables
int vtgPin = A0;    // select the input pin for the potentiometer
int raw_vtgValue = 0;  // variable to store the value coming from the sensor
int16_t vtgValue =0;

uint8_t Byte1=0;
uint8_t Byte2=0;

//-----------------------------------------------------------------------//
void setup() {

  while (!Serial);
  Serial.begin(115200);
  SPI.begin();
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  
  Serial.println("Example: Write to CAN");
}

void loop() {

  // Voltage Sensor
  raw_vtgValue = analogRead(vtgPin);
  vtgValue = map(raw_vtgValue, 0, 1023, 0, 65535);

  // Byte Writing of voltage read
  Byte1 = vtgValue >>8;
  Byte2 = vtgValue &0xff;
  
  // Building up the CAN message
  canMsg1.can_id  = 0xAB;
  canMsg1.can_dlc = 8;
  canMsg1.data[0] = Byte1;
  canMsg1.data[1] = Byte2;
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

  delay(10);
}
