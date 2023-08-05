#ifndef Common_h
#define Common_h



class Common {
public:
  Common();


  void setUpMPU();

  void setUpModuleSD();

  float getPitch();
  float getRoll();
  String getDateTimeString();
  void getDataFromPreviousMinute();
  DateTime getOneMinuteAgo(DateTime currentTime);
  void smoothDataUsingMovingAverage(int *inputData, int *outputData, int dataSize, int windowSize);
  void getDataFromPreviousHour();
  int processLine(String line, DateTime previousHour);
  void appenData(const char *fileName,const char *data);
  String getLine(const char *fileName);
  int findPeak(int arr[], int size);
  void medianFilter(int *data, int size, int *output);
  void timerEvent();
  void EraSetup();
};

#endif