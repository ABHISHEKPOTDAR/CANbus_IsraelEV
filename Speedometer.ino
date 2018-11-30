#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

char timeinmin[32];
String speedinkm;
int f=0;
int valid=0;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;

SoftwareSerial Serial1(10, 11); // RX, TX

void Display_data(){
  display.clearDisplay(); 
  // text display tests
  display.setTextSize(3);
  display.setTextColor(WHITE);
  if(f<10){display.setCursor(103,0);}
  if(f>=10){if(f<100){display.setCursor(85,0);} else{display.setCursor(67,0); }}
  display.println(speedinkm);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(95,22);
  display.println("KM/H");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,17);
  display.println(timeinmin);
   display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,8);
  display.println("Time");
    if(valid==0){ 
   display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25,0);
  display.println("Syncing");
    }
        if(valid!=0){ 
   display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25,0);
  display.println("      ");
    }
  display.display();
  delay(2000);
    }
    
  
  
 
void setup()
{  Serial.begin(115200);
  Serial1.begin(GPSBaud);
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  }

void loop()
{
  Time(gps.time);
valid=gps.location.lat();
Serial.print(valid);
f = round( gps.speed.kmph());
speedinkm = String(f);
Serial.print(speedinkm);
  Serial.println();
  smartDelay(500);
  Display_data();
}

static void Time(TinyGPSTime &t)
{
    
    sprintf(timeinmin, "%02d:%02d", t.hour(), t.minute());
  //  timeinmin[0]='1';
   //timeinmin[1]='9';
    Serial.print(timeinmin);
    //Serial.print("  ");
   // Serial.print(timeinmin[1]);
    int temp1 = (int)timeinmin[0];
    temp1=temp1-48;
    temp1=temp1*10;
    temp1=temp1+((int)timeinmin[1]-48);
    temp1=temp1+5;
    if(temp1>=24){temp1=temp1-24;}
    char a[2];
    itoa(temp1, a, 10);
    timeinmin[0]=a[0];
   timeinmin[1]=a[1];
   
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}

static void printFloat1(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }
  
  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartDelay(0);
}
