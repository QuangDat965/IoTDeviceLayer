#include "NTPService.h"

NTPService::NTPService(WiFiUDP &udp) : timeClient(udp) {}

void NTPService::begin(const char* ntpServer, int timeZone) {
  timeClient.begin();
  timeClient.setUpdateInterval(60000); // Cập nhật thời gian mỗi 60 giây
  timeClient.setTimeOffset(timeZone * 3600); // Đặt múi giờ

  // Cài đặt máy chủ NTP
  timeClient.setPoolServerName(ntpServer);
}

void NTPService::update() {
  timeClient.update();
}

time_t NTPService::getCurrentTime() {
  return timeClient.getEpochTime();
}

String NTPService::getTimeString() {
  time_t currentTime = getCurrentTime();
  int hours = hour(currentTime);
  int minutes = minute(currentTime);
  int seconds = second(currentTime);
  int years = year(currentTime);
  int months = month(currentTime);
  int days = year(currentTime);
  

  char timeString[22];
  snprintf(timeString, sizeof(timeString), "%04d-%02d-%02d/%02d:%02d:%02d", years, months, days, hours, minutes, seconds);

  return String(timeString);
}
