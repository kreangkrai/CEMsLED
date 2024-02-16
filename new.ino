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

const int quantity = 7;
const int width = 6;
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
    //dmd.begin();
    dmd.setBrightness(30);
    dmd.selectFont(Arial14);
    dmd.clearScreen();

    for(int i=0;i<width;i++){
      int data = values[i];
      int padLeft = 0; 
      if (data < 100){
        padLeft = CalPixelXYFloat2_Arial(data);
      }else if (data > 99 && data < 1000){
        padLeft = CalPixelXYFloat3_Arial(data);
      }else if (data > 999 && data < 10000){
        padLeft = CalPixelXYFloat4_Arial(data);
      }
      dmd.drawString((32*i) + padLeft,2,String(data * 0.1,1),GRAPHICS_ON);
    }
    //dmd.end();
}

int CalPixelXYFloat2_Arial(int n)
{
  int m = n;
  int r = 100;
  int len = 2;
  int sum = 0;
  while(len>0){
    int x = (m % r) / (r/10);
    r /= 10;
    sum += width_num_arial[x];
    len--;
  }
  return 32 - (sum + 4);
}
int CalPixelXYFloat3_Arial(int n)
{
  int m = n;
  int r = 1000;
  int len = 3;
  int sum = 0;
  while(len>0){
    int x = (m % r) / (r/10);
    r /= 10;
    sum += width_num_arial[x];
    len--;
  }
  return 32 - (sum + 4) - 1;
}
int CalPixelXYFloat4_Arial(int n)
{
  int m = n;
  int r = 10000;
  int len = 4;
  int sum = 0;
  while(len>0){
    int x = (m % r) / (r/10);
    r /= 10;
    sum += width_num_arial[x];
    len--;
  }
  return 32 - (sum + 4) - 2;
}
