/*
   All Rights Reserved by Vega Space & Vega Propulsion Laboratory
   Created and Written by Joey Heo
   Title: Nuri_Transmitter Software
   Last Updated: April 1st, 2022
   Version: v2
*/
#include "Wire.h" // This library allows you to communicate with I2C devices.
//For nRF24L01+
#include <SPI.h>
#include <RF24.h>

//For BME280
#include "Wire.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//For SD Card
#include <EEPROM.h> 
#include <SD.h>
int sdCS = 7;
//#include "SdFat.h";
//#include <SoftSPI.h>;
//File myFile;
//new SPI pins
//const uint8_t SD_CS_PIN     = 10;
//const uint8_t SOFT_MOSI_PIN = 2;
//const uint8_t SOFT_MISO_PIN = 1; //Note that Pin 1 and 2 is used for Serial communication, so Serial Print shouldn't be done in the code during actual deployment.
//const uint8_t SOFT_SCK_PIN  = 9;

//SoftSpiDriver <SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> softSpi;
//#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SD_SCK_MHZ(0), softSpi)
//SdFat sd;
File csvFile;
#define EEPROM_SIZE 1
int dataNumber = 0;
static const unsigned long REFRESH_INTERVAL = 1000; // ms
static unsigned long lastRefreshTime = 0;

//For Servo
#include <Servo.h>
Servo myservo;

//For BME280 Definition
#define BME_SCK 4// Serial Clock
#define BME_MISO 3// Serial Data Out
#define BME_MOSI 2// Serial Data In
#define BME_CS 8// Chip Select
#define SEALEVELPRESSURE_HPA (1013.25)
//BME stuff
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
unsigned long delayTime;
//Till here
static const unsigned long REFRESH_INTERVAL = 1000; // ms
static unsigned long lastRefreshTime = 0;
float startAltitude = 0;

RF24 radio(6, 5); // CE, CSN

const byte address[6] = "00001";
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.

int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data
char tmp_str[7]; // temporary variable used in convert function

char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

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


};
typedef struct package Package;
Package data;

float lastAltitude; 
float maxApogee;

void setup() {
  Serial.begin(115200);

  //Servo Initialization 
  myservo.attach(9);
  //Radio Initialization
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(address);
  radio.stopListening();
  bool status;

  //BME Initialization
  status = bme.begin();
  if (!status) {
    //Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  startAltitude = bme.readAltitude(1013.25);

    //Serial.println("-- Default Test --");
  delayTime = 1000;

  
  //SD Card Initialization
    //Serial.print("Initializing SD card...");
  if (!SD.begin(7)) {
    Serial.println("initialization failed!");
  }
  dataNumber = EEPROM.read(0) >= 255 ? 0 : EEPROM.read(0)+1;
  EEPROM.write(0, dataNumber);
  csvFile = SD.open("data"+String(dataNumber)+".csv", FILE_WRITE);
    //Serial.println( "WRITE --> " + String(dataNumber));

    //Serial.println("initialization done.");

  //MPU Initialization
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  
}
void readSensor()
{
  data.acX = accelerometer_x;
  data.acY = accelerometer_y;
  data.acZ = accelerometer_z;
  data.gyX = gyro_x;
  data.gyY = gyro_y;
  data.gyZ = gyro_z;
  data.temp = bme.readTemperature(); // Celsius
  data.pres = (bme.readPressure() / 100.0F); //unit hPa
  data.altitud = (bme.readAltitude(1013.25) - startAltitude); // meters
  data.humidity = bme.readHumidity(); // percentage

}
void apogeeDetection()
{
  float currentAltitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

  if (currentAltitude - lastAltitude <= -1) {
    delay(100);
    currentAltitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    if (currentAltitude - lastAltitude <= -2){
      delay(100);
      currentAltitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
      if (currentAltitude - lastAltitude <= -3){
        maxApogee = (currentAltitude - bme.readAltitude(1013.25)) + 3.0; 
        delay(150);
        deploy();
      }
    }
  }
  
}

void deploy(){
  myservo.write(0);
}
void loop() {

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7 * 2, true); // request a total of 7*2=14 registers

  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  accelerometer_x = Wire.read() << 8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer_y = Wire.read() << 8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer_z = Wire.read() << 8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  gyro_x = Wire.read() << 8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire.read() << 8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro_z = Wire.read() << 8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)

  readSensor();
  
  int roll = atan(data.acY / sqrt(pow(data.acX, 2) + pow(data.acZ, 2))) * 180 / PI;
  int pitch = atan(-1 * data.acX / sqrt(pow(data.acY, 2) + pow(data.acZ, 2))) * 180 / PI;
  int rollF = 0.94 * rollF + 0.06 * roll;
  int pitchF = 0.94 * pitchF + 0.06 * pitch;
 
  String dataLine = String(millis()) + "," + String(accelerometer_x) + "," + String(accelerometer_y) + "," + String(accelerometer_z)
                    + "," + String(gyro_x) + "," + String(gyro_y) + "," + String(gyro_z) + "," + String(bme.readTemperature()) + "," + String(bme.readPressure() / 100.0F) + "," + String(bme.readAltitude(SEALEVELPRESSURE_HPA))
                    + "," + String(bme.readHumidity())  + "," + String(pitchF) + "," + String(rollF);
  
  lastAltitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  apogeeDetection();
  
  csvFile.println(dataLine);
  radio.write(&data, sizeof(data));

  
  
  if(millis() - lastRefreshTime >= REFRESH_INTERVAL)
  {
  lastRefreshTime += REFRESH_INTERVAL;
  csvFile.flush();
    //Serial.println("DATA FLUSH OK...");
  }else{
    //Something is not working
  }
  delay(50);
  
  //Serial.println(dataLine);
  //Serial.println(sizeof(data));
  //print out data

  /*Serial.print("aX = "); Serial.print(convert_int16_to_str(accelerometer_x));
    Serial.print(" | aY = "); Serial.print(convert_int16_to_str(accelerometer_y));
    Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(accelerometer_z));
    // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
    Serial.print(" | gX = "); Serial.print(convert_int16_to_str(gyro_x));
    Serial.print(" | gY = "); Serial.print(convert_int16_to_str(gyro_y));
    Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gyro_z));
    Serial.println();

    Serial.print("Humidity: "); Serial.print(bme.readHumidity());
    Serial.print(" | Temperature: "); Serial.print(bme.readTemperature());
    Serial.print(" | Pressure: " ); Serial.print(bme.readPressure() / 100.0F);
    Serial.print(" | Altitude: " ); Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.print(" | Size of Data: "); Serial.print(sizeof(data));
    Serial.println();*/
 
}
