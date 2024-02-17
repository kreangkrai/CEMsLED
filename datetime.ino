//#include <SoftwareSerial.h>
#include <ModbusRTUSlave.h>
#include <DMD2.h>
#include <fonts/Droid_Sans_16.h>
#include <fonts/Arial14.h>

unsigned long long previousMillisModbus = 0;
const long long periodModbus = 50;

unsigned long long previousMillisDMD2 = 0;
const long long periodDMD2 = 10000;

//const uint8_t rxPin = 2;
//const uint8_t txPin = 3;

//SoftwareSerial mySerial(rxPin,txPin);

const int quantity = 5;
const int width = 5;
ModbusRTUSlave modbus(Serial); // serial port, driver enable pin for rs-485 (optional) : Hardware Serial
//ModbusRTUSlave modbus(mySerial); // serial port, driver enable pin for rs-485 (optional) : Softwate Serial

uint16_t holdingRegisters[quantity];

SoftDMD dmd(width,1); // Width,Height
//int width_num_droid[10] = {7,4,7,7,9,7,7,7,7,7}; //Droid_Sans_16
int width_num_arial[10] = {6,3,6,6,7,6,6,6,6,6}; //Arial14

void drawText(uint16_t*);
void setup() {
  modbus.configureHoldingRegisters(holdingRegisters, quantity);
  modbus.begin(1, 9600);  // slave id, baud rate, config (optional)
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
    dmd.setBrightness(30);
    dmd.selectFont(Arial14);
    dmd.clearScreen();

   /*
    1. Date
    2. Month
    3. Year
    4. Hour
    5. Minute
   */

    int date = values[0];
    int month = values[1];
    int year = values[2];
    int hour = values[3];
    int minute = values[4];

    uint16_t datetimes[5] = {minute,hour,year,month,date};
    
    //DateTime
    for (int i=0;i<5;i++){
      int data = datetimes[i];
      int padLeft = 1; 
      dmd.drawString((32*i) + padLeft,2,String(data,1),GRAPHICS_ON);
    }
}
