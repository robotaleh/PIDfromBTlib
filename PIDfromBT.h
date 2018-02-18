#ifndef PIDfromBT_h
#define PIDfromBT_h
#define LIBRARY_VERSION 1.0
#include <SoftwareSerial.h>
class PIDfromBT
{

#define DEBUG 1
#define NO_DEBUG 0

#define MIN_IDEAL 0
#define MAX_IDEAL 500

#define TYPE_PIDV   1
#define TYPE_PIDVI  2
#define TYPE_PIDVIS 3

public:

  // PIDV
  PIDfromBT(float* kp, float* kd, float* ki, int* vel, bool debug);

  // PIDVI
  PIDfromBT(float* kp, float* kd, float* ki, int* vel, int* ideal, bool debug);

  // PIDVIS
  PIDfromBT(float* kp, float* kd, float* ki, int* vel, int* ideal, int* suction, bool debug);

  void setMinIdeal(int minIdeal);

  void setMaxIdeal(int maxIdeal);

  void update();

  void execute_task(char letter, float value);

private:
  void execute_task();
  String _value;
  char _letter;
  long _last_update;
  bool _debug;
  int _type;
  float *_kp;
  float *_ki;
  float *_kd;
  int *_vel;
  int *_ideal;
  int *_suction;
  int _maxIdeal;
  int _minIdeal;
};
#endif
