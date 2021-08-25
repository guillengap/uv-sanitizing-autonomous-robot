//AUTHOR: GUILLERMO PEREZ GUILLEN

#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;      
#include <TouchScreen.h>

int16_t BOXSIZE;
uint16_t ID, currentcolor;
uint8_t Orientation = 0;    //PORTRAIT
String UVIndex = "0";
String Index = " ";  

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup()
{
    while (!Serial);
  Serial.begin(57600);
   
    uint16_t tmp;
    tft.reset();
    ID = tft.readID();
    tft.begin(ID);
    tft.setRotation(Orientation);
    tft.fillScreen(BLACK);
}
 
void loop()
{
  float sensorVoltage;
  float sensorValue;

  sensorValue = analogRead(A8);
  sensorVoltage = (sensorValue * (5.0 / 1023.0))*1000;  //Voltage in miliVolts

////////////////////////// UV Index

  if(sensorVoltage<50.0)
  {
    UVIndex = "0";
    Index = "LOW";
  }
  else if (sensorVoltage>=50.0 && sensorVoltage<227.0)
  {
    UVIndex = "0";
    Index = "LOW";
  }
  else if (sensorVoltage>=227 && sensorVoltage<318)
  {
    UVIndex = "1";
    Index = "LOW";    
  }
  else if (sensorVoltage>=318 && sensorVoltage<408)
  {
    UVIndex = "2";
    Index = "LOW";    
  }else if (sensorVoltage>=408 && sensorVoltage<503)
  {
    UVIndex = "3";
    Index = "MEDIUM";    
  }
  else if (sensorVoltage>=503 && sensorVoltage<606)
  {
    UVIndex = "4";
    Index = "MEDIUM";    
  }else if (sensorVoltage>=606 && sensorVoltage<696)
  {
    UVIndex = "5";
    Index = "MEDIUM";    
  }else if (sensorVoltage>=696 && sensorVoltage<795)
  {
    UVIndex = "6";
    Index = "HIGH";    
  }else if (sensorVoltage>=795 && sensorVoltage<881)
  {
    UVIndex = "7";
    Index = "HIGH";    
  }
  else if (sensorVoltage>=881 && sensorVoltage<976)
  {
    UVIndex = "8";
    Index = "VERY HIGH";    
  }
  else if (sensorVoltage>=976 && sensorVoltage<1079)
  {
    UVIndex = "9";
    Index = "VERY HIGH";    
  }
  else if (sensorVoltage>=1079 && sensorVoltage<1170)
  {
    UVIndex = "10";
    Index = "VERY HIGH";    
  }
  else if (sensorVoltage>=1170)
  {
    UVIndex = "11";
    Index = "THE HIGHEST";    // EXTREMELY HIGHEST
  }
  
/////////////////////////////////////
    
  Serial.print("sensor reading = ");
  Serial.print(sensorValue);
  Serial.println("");
  Serial.print("sensor voltage = ");
  Serial.print(sensorVoltage);
  Serial.println(" V");
  Serial.print("UV Index = ");
  Serial.print(UVIndex);

   tft.setCursor(0, 5);
   tft.setTextSize(3);   
   tft.setTextColor(MAGENTA, BLACK);   
   tft.println("  UV METER");
   tft.println(" ");   
   tft.setTextColor(YELLOW, BLACK);
   tft.println("mV: " + String(sensorVoltage) + "  ");
   tft.println(" ");
   tft.println("UVIndex: " + String(UVIndex) + " ");  
   tft.setTextColor(WHITE, BLACK);
   tft.println(" ");      
   tft.println(String(Index) + "     ");       
   delay(1000);
}