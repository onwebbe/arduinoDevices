#include <Adafruit_PCF8574.h>
#include "CarRadar.h"
#include "EchoControl.h"
#include "Arduino.h"

CarRadar::CarRadar(Adafruit_PCF8574 *pcf, int trigPin, int echoPin, int servoPin) {
  init(pcf, trigPin, echoPin, servoPin);
}
void CarRadar::init(Adafruit_PCF8574 *pcf, int trigPin, int echoPin, int servoPin) {
  _radarData = new float[18];
  _trigPin = trigPin;
  _echoPin = echoPin;
  _servoPin = servoPin;
  _pcf = pcf;

  _echo = new EchoControl(_pcf, _trigPin, _echoPin);
  _echo->setup();
  _myservo = new Servo();
	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	_myservo->setPeriodHertz(50);    // standard 50 hz servo
	_myservo->attach(_servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
  _lastTime = millis();
}


void CarRadar::scan(int angle) {
  Serial.print("scanning angle:");
  Serial.println(angle);
  // _myservo->write(angle);
  float distance = _echo->getDistance();
  Serial.print("get distance:");
  Serial.println(distance);
  _radarData[angle] = distance;
}

void CarRadar::loop() {
  long currentTime = millis();
  if ((currentTime - _lastTime) > _timeGap || (currentTime - _lastTime) < 0) {
    Serial.println("radar loop start");
    scan(_currentAngle);
    if (_isGo) {
      _currentAngle += 10;
      if (_currentAngle > 180) {
        _currentAngle -= 20;
        _isGo = false;
      }
    } else {
      _currentAngle -= 10;
      if (_currentAngle < 0) {
        _currentAngle += 20;
        _isGo = true;
      }
    }
    _lastTime = millis();
  }
}

void CarRadar::setTrigPinAPCF(bool isAPCF) {
  _trigPinAPCF = isAPCF;
  _echo->setTrigPinAPCF(_trigPinAPCF);
}