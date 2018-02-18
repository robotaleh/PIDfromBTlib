
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <PIDfromBT.h>

PIDfromBT::PIDfromBT(float* kp, float* ki, float* kd, int* vel, bool debug)
{
  _kp = kp; // P
  _ki = ki; // I
  _kd = kd; // D
  _vel = vel; // V

  _debug = debug;
  _value = "";
  _letter = ' ';
  _last_update = millis();
  _type = TYPE_PIDV;
}

PIDfromBT::PIDfromBT(float* kp, float* ki, float* kd, int* vel, int* ideal, bool debug)
{
  _kp = kp; // P
  _ki = ki; // I
  _kd = kd; // D
  _vel = vel; // V
  _ideal = ideal; // I
  _maxIdeal = MAX_IDEAL;
  _minIdeal = MIN_IDEAL;

  _debug = debug;
  _value = "";
  _letter = ' ';
  _last_update = millis();
  _type = TYPE_PIDVI;
}

PIDfromBT::PIDfromBT(float* kp, float* ki, float* kd, int* vel, int* ideal, int* suction, bool debug)
{
  _kp = kp; // P
  _ki = ki; // I
  _kd = kd; // D
  _vel = vel; // V
  _ideal = ideal; // I
  _suction = suction; // S
  _maxIdeal = MAX_IDEAL;
  _minIdeal = MIN_IDEAL;

  _debug = debug;
  _value = "";
  _letter = ' ';
  _last_update = millis();
  _type = TYPE_PIDVIS;
}


void PIDfromBT::update() {
  if (Serial.available()) {
    // Si hay algo pendiente en el Buffer, lo lee y guarda el tiempo en el que lo leyo.
    _last_update = millis();
    byte readByte = Serial.read();

    if ((isDigit((char)readByte) || (char)readByte == '.' || (char)readByte == '-')) {
      // Si es un dígito, signo decimal o signo negativo, lo guarda como un valor.
      _value += (char)readByte;
    }else{
      if(_letter != ' '){
        // Si es una letra y ya hay otra letra ya leída, ejecuta la acción correspondiente a esa letra.
        execute_task(_letter, (_value!="")?_value.toFloat():0);
      }
      // Llega una nueva letra; limpia el _value anterior y lee la nueva letra.
      _value = "";
      _letter = (char)readByte;
    }
  }else if((millis()-_last_update) > 5 && _letter != ' '){
    // Si no se ha recibido nada en los últimos 5ms y hay alguna letra pendiente, ejecuta su acción correspondiente.
    execute_task(_letter, (_value!="")?_value.toFloat():0);
    _letter = ' ';
    _value = "";
  }
}


void PIDfromBT::execute_task(char letter, float value)
{
  switch (letter) {
    case 'P':
      if(_type < TYPE_PIDV)return;
      *_kp = value;
      if (_debug) {
        Serial.println("Kp: " + String(value));
      }
      break;
    case 'I':
      if(_type < TYPE_PIDV)return;
      *_ki = value;
      if (_debug) {
        Serial.println("Ki: " + String(value));
      }
      break;
    case 'D':
      if(_type < TYPE_PIDV)return;
      *_kd = value;
      if (_debug) {
        Serial.println("Kd: " + String(value));
      }
      break;
    case 'V':
      if(_type < TYPE_PIDV)return;
      *_vel = value;
      if (_debug) {
        Serial.println("Vel: " + String(*_vel));
      }
      break;
    case 'X':
      if(_type < TYPE_PIDVI)return;
      *_ideal = map(value, -500, 500, _minIdeal, _maxIdeal);;
      if (_debug) {
        Serial.println("Ideal: " + String(*_ideal));
      }
      break;
    case 'S':
      if(_type < TYPE_PIDVIS)return;
      *_suction = value;
      if (_debug) {
        Serial.println("Suc: " + String(*_suction));
      }
      break;
  }
}

void PIDfromBT::setMinIdeal(int minIdeal){
  _minIdeal = minIdeal;
}
void PIDfromBT::setMaxIdeal(int maxIdeal){
  _maxIdeal = maxIdeal;
}
