//#include <SoftwareSerial.h>
#include <ModbusRTUSlave.h>
#include <DMD2.h>
#include <fonts/Arial14.h>

unsigned long long previousMillisModbus = 0;
const long long periodModbus = 50;

unsigned long long previousMillisDMD2 = 0;
const long long periodDMD2 = 10000;

//const uint8_t rxPin = 2;
//const uint8_t txPin = 3;

//SoftwareSerial mySerial(rxPin,txPin);

const int quantity = 5;
const int width = 3;
ModbusRTUSlave modbus(Serial); // serial port, driver enable pin for rs-485 (optional) : Hardware Serial
//ModbusRTUSlave modbus(mySerial); // serial port, driver enable pin for rs-485 (optional) : Softwate Serial

uint16_t holdingRegisters[quantity];

SoftDMD dmd(width,1); // Width,Height

void drawText(uint16_t*);
void setup() {
  modbus.configureHoldingRegisters(holdingRegisters, quantity);
  modbus.begin(1, 115200);  // slave id, baud rate, config (optional)
  dmd.begin();
}

void loop() {
    unsigned long long currentMillis = millis(); // store the current time
    if (currentMillis - previousMillisModbus >= periodModbus) {
      previousMillisModbus = currentMillis;
      callModbus();
    }
    if (currentMillis - previousMillisDMD2 >= periodDMD2) {
      previousMillisDMD2 = currentMillis;
      drawText(holdingRegisters);
    }
}
void callModbus(){ 
  modbus.poll();
}
void drawText(uint16_t* values){
    //dmd.begin();
    dmd.setBrightness(30);
    dmd.selectFont(Arial14);
    dmd.clearScreen();
    
    int _date = values[0];
    int _month = values[1];
    int _year = values[2];
    int _hour = values[3];
    int _minute = values[4];
    String date = "";
    if (_month < 10 ){
       date = Padleft(_date) + "." + Padleft(_month);
    }else{
       date = Padleft(_date) + "-" + Padleft(_month);
    }
    
    String year = String(_year);
    String times = Padleft(_hour)+ ":" + Padleft(_minute);
    dmd.drawString((32 * 0) + 1,2,times,GRAPHICS_ON);
    dmd.drawString((32 * 1) + 1,2,year,GRAPHICS_ON);
    dmd.drawString((32 * 2) + 1,2,date,GRAPHICS_ON);
    //dmd.end();
}
 String Padleft(int n){
   if (n < 10){
     return "0" + String(n,DEC);
   }else{
     return String(n,DEC);
   }
 }
