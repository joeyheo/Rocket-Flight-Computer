/*
   All Rights Reserved by Vega Space & Vega Propulsion Laboratory
   Created and Written by Joey Heo
   Title: Nuri_Reciever Software (Supports Processing)
   Last Updated: August 21st, 2021
   Version: v1
*/
#include <SPI.h>
#include <RF24.h>
#define SEALEVELPRESSURE_HPA (1013.25)

RF24 radio(9, 10); // CE, CSN
float roll, pitch, rollF, pitchF;
const byte thisSlaveAddress[5] = {'R','x','A','A','A'};
struct package
{
  short int acX;
  short int acY;
  short int acZ;
  short int gyX;
  short int gyY;
  short int gyZ;
  float altitud;
  float humidity;
  float temp;
  float pres;
  bool deployed;

};
typedef struct package Package;
Package data;


void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setDataRate( RF24_250KBPS );
  radio.openReadingPipe(1, thisSlaveAddress);
  radio.startListening();

}

void loop() {
  if (radio.available()) {
    checkForWirelessData();
  }
}

void checkForWirelessData()
{
  
    radio.read( &data, sizeof(data));
     roll = atan(data.acY / sqrt(pow(data.acX, 2) + pow(data.acZ, 2))) * 180 / PI;
  pitch = atan(-1 * data.acX / sqrt(pow(data.acY, 2) + pow(data.acZ, 2))) * 180 / PI;
  rollF = 0.94 * rollF + 0.06 * roll;
  pitchF = 0.94 * pitchF + 0.06 * pitch;
  
  Serial.print("aX = "); Serial.print(data.acX);
    Serial.print(" | aY = "); Serial.print(data.acY);
    Serial.print(" | aZ = "); Serial.print(data.acZ);
    Serial.print(" | gX = "); Serial.print(data.gyX);
    Serial.print(" | gY = "); Serial.print(data.gyY);
    Serial.print(" | gZ = "); Serial.print(data.gyZ);
    Serial.print(" | Altitude = "); Serial.print(data.altitud);Serial.print("m ");
    Serial.print(" | Humidity = "); Serial.print(data.humidity); Serial.print("%");
    Serial.print(" | Temp = "); Serial.print(data.temp);Serial.print("C ");
    Serial.print(" | Pressure = "); Serial.print(data.pres);Serial.print("hPa ");
    Serial.print(" | Deployed = "); Serial.print(data.deployed);
    
  //Serial.println(data.altitud);
  /*
  Serial.print(rollF);
  Serial.print("/");
  Serial.println(pitchF);
  */
 
  Serial.println();

}
