#include <MPU6050_tockn.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <SPI.h>
#include "RTClib.h"
#include "Common.h"


MPU6050 mpu6050(Wire);
RTC_DS3231 rtc;
Common:: Common() {}
Common common;


char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

long timer = 0;

void setup() {
  Serial.begin(115200);
  common.setUpMPU();
  
}

void loop() {
  mpu6050.update();
  if(millis() - timer > 1000){
  }

}

void Common:: setUpMPU() {
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

float Common:: getPitch() {
  float accX = mpu6050.getAccX();
  float accY = mpu6050.getAccY();
  float accZ = mpu6050.getAccZ();

  // Tính góc pitch và roll
  float pitch = atan2(accX, sqrt(accY * accY + accZ * accZ)) * 180.0 / PI;

  return pitch;
}

float Common:: getRoll() {
  float accX = mpu6050.getAccX();
  float accY = mpu6050.getAccY();
  float accZ = mpu6050.getAccZ();
  return atan2(accY, sqrt(accX * accX + accZ * accZ)) * 180.0 / PI;

}
