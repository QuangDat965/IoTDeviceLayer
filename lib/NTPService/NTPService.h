#ifndef NTPService_h
#define NTPService_h

#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>

class NTPService {
public:
  NTPService(WiFiUDP &udp);

  void begin(const char* ntpServer, int timeZone);
  void update();

  time_t getCurrentTime();

  String getTimeString();

private:
  NTPClient timeClient;
};

#endif
