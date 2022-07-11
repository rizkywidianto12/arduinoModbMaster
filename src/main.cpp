#include <Arduino.h>
#include <ModbusMaster.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

#define re 2
#define de 3
#define ss 4
#define modbus Serial
#define mvcc 5
#define mgnd 6

#define mbaud 19200
#define cbaud 9600
#define mParity SERIAL_8E1
#define timeout 100
#define polling 100
#define retry_count 10

#define ID_M 1
#define ToR 4

#define reg1 452
#define reg2 452
#define reg3 452
#define reg4 452

uint16_t reg_addr[4]{
  reg1,
  reg2,
  reg3,
  reg4,
};

float DATA_MODBUS[ToR];
File myFile;
int z = 0;


void preTransmission(){
  digitalWrite(re,1);
  digitalWrite(de,1);
}
void postTransmission(){
  digitalWrite(re,0);
  digitalWrite(de,0);

  
}SoftwareSerial com(8,9);
ModbusMaster node;

float readModbusFloat(char addr,uint16_t REG){
  float i = 0;
  uint8_t result,j;

  uint16_t data[4];
  uint32_t value = 0;
  node.begin(ID_M,modbus);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  result = node.readHoldingRegisters(REG,4);
  delay(polling);
  if(result == node.ku8MBSuccess){
    for(j=0;j<4;j++){
      data[j] = node.getResponseBuffer(j);
    }
    value = data[0];
    int hasil = data[0];
    return hasil;
  }
  else{
    com.print(F("Fail addr>>> ")); com.println(REG);
    delay(1000);
    return 0;
  }
}

void getModbus(){
  delay(polling);
  for(char i = 0;i<ToR;i++){
    DATA_MODBUS[i] = readModbusFloat(ID_M,reg_addr[i]);
  }
}

void SDwrite(String q){
  myFile = SD.open("XYLoad.txt", FILE_WRITE);
  if (myFile) {
    if(myFile.println(q)){
      com.println(F("Ok"));
    }
  }
  myFile.close();
}

void SDwriteraw(String v){
  myFile.close();
  myFile = SD.open("XYLoadraw.txt", FILE_WRITE);
  if (myFile) {
    myFile.println(v);
    com.println(F("Ok"));
  }
  myFile.close();
}

void(* resetFunc) (void) = 0;  // declare reset fuction at address 0


void setup() {
  com.begin(cbaud);
  modbus.begin(mbaud,mParity);
  pinMode(re,OUTPUT);
  pinMode(de,OUTPUT);

  pinMode(mvcc,1);
  pinMode(mgnd,1);

  digitalWrite(mvcc,1);
  digitalWrite(mgnd,0);

  postTransmission();

  if(SD.begin(ss)) //cek SDcard
  {
    com.println(F("SD Ready!"));
    SDwrite("\n\n\n############## START ###############\n\n\n");

  }
  else{
    com.println(F("SD ERROR!"));
    delay(1000);
    resetFunc();
  }

  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  z = readModbusFloat(1,452);
  com.print(z);
  com.print("   ");
  z = abs(z);
  delay(1000);
  z=z/10;
  com.println(z);
  SDwriteraw(String(z));
  SDwrite(String(z));
  delay(1000);
}