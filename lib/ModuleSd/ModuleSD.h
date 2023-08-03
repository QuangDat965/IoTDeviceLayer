#ifndef ModuleSD_h
#define ModuleSD_h

#include "Arduino.h"
#include <SD.h>


class ModuleSD {
public:
  ModuleSD();

  bool begin(uint8_t chipSelectPin);

  bool writeData(const char* filename, const char* data);
  bool readData(const char* filename, char* buffer, size_t bufferSize);

  bool exists(const char* filename);
  bool remove(const char* filename);
  bool appendData(const char* filename, const char* data);
  bool appendLine(const char* fileName, const char* data);
  String readLine(const char* fileName);

private:
  uint8_t _chipSelectPin;
};

#endif
