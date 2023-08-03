// #define DEFAULT_MQTT_HOST "mqtt1.eoh.io"

// // You should get Auth Token in the ERa App or ERa Dashboard
// #define ERA_AUTH_TOKEN "7be738b0-f63c-4a21-875a-6b73b7c06140"

// #include <Arduino.h>


// #include <ModuleSD.h>
// #include <Adafruit_I2CDevice.h>
// #include <NTPService.h>
// #include <WiFi.h>
// #include <WiFiUdp.h>
// #include <Common.h>
// #include <ERa.hpp>
// #include <ERa/ERaTimer.hpp>

// ModuleSD moduleSD;
// const char* filename = "/datbeo.txt";

// const char* ssid = "DatBeoDz";
// const char* password = "25312001";
// const char* ntpServer = "pool.ntp.org";
// const int timeZone = 7; // Múi giờ UTC+7

// WiFiUDP udp;
// NTPService ntpService(udp);
// Common::Common(){}

// ERaTimer timer;

// /* This function print uptime every second */
// void timerEvent() {
//     ERa.virtualWrite(V3,3); 
//     ERa.virtualWrite(V4,1); 
//     ERa.virtualWrite(V5,7); 
//     ERa.virtualWrite(V6,5); 
// }


// void setup() {
//   Serial.begin(115200);
//   if (!moduleSD.begin(5)) { // Thay 5 bằng chân chip select của thẻ SD
//     Serial.println("SD card initialization failed!");
//     while (1);
//   }
//   Serial.println("SD card initialized!");

//    WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.println("Connecting to WiFi...");
//   }

//   ERa.begin(ssid, password);

//     /* Setup timer called function every second */
//     timer.setInterval(1000L, timerEvent);

//   // Khởi tạo NTPService với máy chủ NTP và múi giờ
//   ntpService.begin(ntpServer, timeZone);

//   Serial.println("WiFi connected");

// }

// void loop() {

//   ERa.run();
//   timer.run();

//   // if (moduleSD.appendLine(filename,(ntpService.getTimeString()+ +","+"3").c_str())) {
//   //   Serial.println("Data written successfully!");
//   // } else {
//   //   Serial.println("Failed to write data!");
//   // }

//   // Đọc dữ liệu từ thẻ nhớ SD và in ra Serial Monitor

//     Serial.print("Read data: ");
//     Serial.println(moduleSD.readLine(filename));

//    ntpService.update();

//   // Lấy thời gian hiện tại
  
//   Serial.print("Current time: ");
//   Serial.println(ntpService.getTimeString());


//   delay(3000);
// }