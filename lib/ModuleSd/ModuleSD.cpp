#include <ModuleSD.h>


ModuleSD::ModuleSD() {}

bool ModuleSD::begin(uint8_t chipSelectPin) {
  _chipSelectPin = chipSelectPin;
  return SD.begin(chipSelectPin);
}

bool ModuleSD::writeData(const char* filename, const char* data) {
  File file = SD.open(filename, FILE_WRITE);
  if (file) {
    size_t dataSize = strlen(data);
    size_t bytesWritten = file.write((const uint8_t*)data, dataSize);
    file.close();
    return (bytesWritten == dataSize);
  }
  return false;
}

bool ModuleSD::readData(const char* filename, char* buffer, size_t bufferSize) {
  File file = SD.open(filename);
  if (file) {
    size_t bytesRead = file.readBytes(buffer, bufferSize - 1);
    file.close();
    buffer[bytesRead] = '\0';
    return true;
  }
  return false;
}

bool ModuleSD::exists(const char* filename) {
  return SD.exists(filename);
}

bool ModuleSD::remove(const char* filename) {
  return SD.remove(filename);
}

bool ModuleSD::appendData(const char* filename, const char* data) {
  File file = SD.open(filename, FILE_APPEND);
  if (file) {
    size_t dataSize = strlen(data);
    size_t bytesWritten = file.write((const uint8_t*)data, dataSize);
    file.close();
    return (bytesWritten == dataSize);
  }
  return false;
}

bool ModuleSD:: appendLine(const char* fileName, const char* data) {
  // Mở file để ghi tiếp (append mode)
  File file = SD.open(fileName, FILE_APPEND);

  if (file) {
    // Ghi dữ liệu vào file
    file.println(data);

    // Đóng file sau khi ghi xong
    file.close();
    return true;
  } else {
    return false; // Không thể mở file
  }
}

String ModuleSD:: readLine(const char* fileName) {
  // Mở file để đọc (read mode)
  File file = SD.open(fileName);

  if (file) {
    // Đọc từng dòng và in lên Serial Monitor
    String temp="";
    while (file.available()) {
      temp+= file.readStringUntil('\n') +'\n';
    }

    // Đóng file sau khi đọc xong
    file.close();
    return temp;
  } else {
    Serial.println("Khong the mo file.");
  }
}
