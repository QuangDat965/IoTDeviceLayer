// #define ERA_DEBUG

#define DEFAULT_MQTT_HOST "mqtt1.eoh.io"

#define ERA_AUTH_TOKEN "0a2b0eec-4047-471f-9afb-b0a97f0ccc5f"

#include <MPU6050_tockn.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <SPI.h>
#include "RTClib.h"
#include "Common.h"
#include <Arduino.h>
#include <ERa.hpp>
#include <ERa/ERaTimer.hpp>
#include <SimpleKalmanFilter.h>

#include <SD.h>


const int chipSelect = 5;
File myFile;



MPU6050 mpu6050(Wire);
RTC_DS3231 rtc;
Common:: Common() {}
Common common;
SimpleKalmanFilter kalmanX(0.001, 0.01, 0.1); // Tham số của Kalman Filter
SimpleKalmanFilter kalmanY(0.001, 0.01, 0.1);

bool isWifiConnected =false;

const char* fileName="/test7.txt";
const char* headerLine = "DateTime,KalmaPitch,Pitch,KalmaRoll,Roll";
const char* ssid = "DatBeoDz";
const char* pass = "25312001";



ERaTimer timer;

void timerEvent() {
    ERa.virtualWrite(V5, common.getPitch()); 
    ERa.virtualWrite(V6, common.getRoll()); 
}

ERA_WRITE(V10){
  int value = param.getInt();
  if(value = 0){
    


  }
  if(value = 1){
    Serial.println("REtrieve data");
    ERa.virtualWrite(V2, random(100)); 
    int i =0;
    int maxSize = 125;
    int arrtemp[maxSize];
    bool isRun = true;
    while(isRun){
      ERa.virtualWrite(V5, common.getPitch()); 
    ERa.virtualWrite(V6, common.getRoll()); 
    mpu6050.update();
    float kalmanPitch = kalmanY.updateEstimate(common.getRoll());
    int numberAppend = (int)kalmanPitch;
    arrtemp[i] = numberAppend;
    i++;
    if(i>120){
        isRun = false;
      }
    delay(500);
    }

    int peaks = common.findPeak(arrtemp, i);
    ERa.virtualWrite(V1, peaks); 
  }
}

void setup() {
  Serial.begin(115200);
  common.setUpMPU();
  // common.setUpModuleSD();
  ERa.begin(ssid, pass);
  isWifiConnected = true;

  timer.setInterval(500,timerEvent);
}

void loop() {
 try {
   mpu6050.update();
  if(isWifiConnected){
    ERa.run();
    timer.run();
  }
  
  float kalmanRoll = kalmanX.updateEstimate(common.getPitch());
  float kalmanPitch = kalmanY.updateEstimate(common.getRoll());
  //save file
  String dataAppend = common.getDateTimeString()+","+kalmanPitch+","+common.getPitch()+","+kalmanRoll+","+common.getRoll();
  // common.appenData(fileName,dataAppend.c_str());
  //read each 1 hour
  DateTime now = rtc.now();
  static int lastMinute = -1; // Biến lưu trữ giá trị phút lưu trữ, được khởi tạo bằng -1 lần đầu tiên

  if (now.minute() != lastMinute) {
    // common.getDataFromPreviousMinute();
    lastMinute = now.minute(); 
  }}
 catch(const char* errorM){
    Serial.println(errorM);
 }
  delay(500);

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

void Common:: setUpModuleSD(){
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("1. is a card inserted?");
    Serial.println("2. is your wiring correct?");
    Serial.println("3. did you change the chipSelect pin to match your shield or module?");
    Serial.println("Note: press reset button on the board and reopen this serial monitor after fixing your issue!");
    while (1);
  }
  Serial.println("initialization done.");

  if(!SD.exists(fileName)){
  Serial.println("file not found create header");
  myFile = SD.open(fileName,FILE_WRITE);
  if(myFile){
    myFile.println(headerLine);
  }
  myFile.close();
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

String Common:: getDateTimeString() {
  DateTime now = rtc.now();

  return now.timestamp();
}

void Common:: getDataFromPreviousMinute() {
  DateTime now = rtc.now();

  // Tính thời gian 1 giờ trước
  DateTime previous = common.getOneMinuteAgo(now);

  // Mở file dữ liệu .txt
  File dataFile = SD.open(fileName, FILE_READ);
  if (dataFile) {
    int i =0;
    int maxSize = 120;
    int arrtemp[maxSize];
    while (dataFile.available()) {
      // Đọc mỗi dòng trong file
      String line = dataFile.readStringUntil('\n');
      try {
      int datatemp = common.processLine(line,previous);
      if(datatemp>=-1){
        if(datatemp<0){
          datatemp=0;
        }
        arrtemp[i]= datatemp;
        i++;
      }

      }
      catch(const char* exception){
          Serial.println("exceptions");
      }
    }
    dataFile.close();
    Serial.print("/n");
    Serial.print("=========================Start=====================");

    for(int j =0; j< i; j++){
      
      Serial.print(arrtemp[j]);
      Serial.print(",");
      

    }
    Serial.print("===========================End===================");
      Serial.print("/n");
    int numberOfPeek = common.findPeak(arrtemp,i);
    Serial.println("numberOfPeek");
    Serial.println(numberOfPeek);
    ERa.virtualWrite(V0, numberOfPeek); 
  } else {
    Serial.println("Khong the mo file!");
  }
}

DateTime Common:: getOneMinuteAgo(DateTime currentTime) {
  int newSecond = currentTime.second() - 60;
  int newMinute = currentTime.minute();
  int newHour = currentTime.hour();

  if (newSecond < 0) {
    // Trường hợp đặc biệt: nếu giây hiện tại lớn hơn hoặc bằng 60, thực hiện điều chỉnh giờ và phút
    newSecond += 60;
    newMinute--;
    if (newMinute < 0) {
      // Giảm phút nếu như phút hiện tại là 0
      newMinute = 59;
      newHour--;
      if (newHour < 0) {
        // Giảm giờ nếu như giờ hiện tại là 0
        newHour = 23;
      }
    }
  }

  return DateTime(currentTime.year(), currentTime.month(), currentTime.day(), newHour, newMinute, newSecond);
}

int Common::processLine(String line, DateTime previous) {
    String filterDate = previous.timestamp();
    String lineDate = line.substring(0,16);
    String chosenDate = filterDate.substring(0,16);
    if(lineDate == chosenDate){
      int dataPush = line.substring(20,25).toInt();

      Serial.println("lineDate");
    Serial.println(lineDate);
    Serial.println("chosenDate");
    Serial.println(chosenDate);
          Serial.println("line");
          Serial.println(line);

          Serial.println("dataPush");
          Serial.println(dataPush);

      return dataPush;
    }
    return -5;
    
}

void Common::appenData(const char* fileName,const char* data){
  myFile = SD.open(fileName,FILE_APPEND);
  if(myFile){
    myFile.println(data);
  }
  myFile.close();
}

String Common::getLine(const char* fileName){
  myFile = SD.open(fileName,FILE_READ);
  if(myFile){
    String temp ="";
    while (myFile.available()){
      temp+= myFile.readStringUntil('/n');
    }
    myFile.close();
    return temp;
  }
}

int Common:: findPeak(int arr[], int size) {
  if (size <= 2) {
    // Mảng có 2 phần tử trở xuống không thể có đỉnh
    return 0;
  }

  int peakCount = 0;

  // Xử lý phần tử đầu tiên
  if (arr[0] >= arr[1] && arr[0]>=30) {
    peakCount++;
  }

  // Duyệt qua các phần tử từ phần tử thứ 2 đến phần tử thứ (size - 2)
  for (int i = 1; i < size - 1; i++) {
    if (arr[i] >= arr[i - 1] && arr[i] >= arr[i + 1] && arr[i]>=30) {
      peakCount++; // Tìm thấy đỉnh tại vị trí i
    }
  }

  // Xử lý phần tử cuối cùng
  if (arr[size - 1] >= arr[size - 2] && arr[size -1 ]>=30) {
    peakCount++;
  }

  return peakCount;
}