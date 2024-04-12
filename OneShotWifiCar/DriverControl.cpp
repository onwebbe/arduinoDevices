#include <Adafruit_PCF8574.h>
#include "DriverControl.h"

// LM298
DriverControl::DriverControl(int leftA, int leftB, int rightA, int rightB, int enLeft, int enRight) {
  init(leftA, leftB, rightA, rightB, enLeft, enRight, _maxSpeedNum);
}
DriverControl::DriverControl(int leftA, int leftB, int rightA, int rightB, int enLeft, int enRight, int maxSpeedNum) {
  init(leftA, leftB, rightA, rightB, enLeft, enRight, maxSpeedNum);
}

void DriverControl::init(int leftA, int leftB, int rightA, int rightB, int enLeft, int enRight, int maxSpeedNum) {
  _leftPinA = leftA;
  _leftPinB = leftB;
  _rightPinA = rightA;
  _rightPinA = rightB;
  _enLeft = enLeft;
  _enRight = enRight;
  _maxSpeedNum = maxSpeedNum;
  _pcf = new Adafruit_PCF8574();
  _pcf->begin(0x20, &Wire);
}
void DriverControl::setup() {
  if (_leftPinAPCF) {
    _pcf->pinMode(_leftPinA, OUTPUT);
  } else {
    pinMode(_leftPinA, OUTPUT);
  }
  if (_leftPinBPCF) {
    _pcf->pinMode(_leftPinB, OUTPUT);
  } else {
    pinMode(_leftPinB, OUTPUT);
  }
  if (_rightPinAPCF) {
    _pcf->pinMode(_rightPinA, OUTPUT);
  } else {
    pinMode(_rightPinA, OUTPUT);
  }
  if (_rightPinBPCF) {
    _pcf->pinMode(_rightPinB, OUTPUT);
  } else {
    pinMode(_rightPinB, OUTPUT);
  }
  if (_enLeftPCF) {
    _pcf->pinMode(_enLeft, OUTPUT);
  } else {
    pinMode(_enLeft, OUTPUT);
  }
  if (_enRightPCF) {
    _pcf->pinMode(_enRight, OUTPUT);
  } else {
    pinMode(_enRight, OUTPUT);
  }
}
void DriverControl::goStaight(int speed) {
  if (_leftPinAPCF) {
    _pcf->digitalWrite(_leftPinA, HIGH);
  } else {
    digitalWrite(_leftPinA, HIGH);
  }
  if (_leftPinBPCF) {
    _pcf->digitalWrite(_leftPinB, LOW);
  } else {
    digitalWrite(_leftPinB, LOW);
  }
  if (_rightPinAPCF) {
    _pcf->digitalWrite(_rightPinA, HIGH);
  } else {
    digitalWrite(_rightPinA, HIGH);
  }
  if (_rightPinBPCF) {
    _pcf->digitalWrite(_rightPinB, LOW);
  } else {
    digitalWrite(_rightPinB, LOW);
  }
  if (_enLeftPCF) {
    
  } else {
    analogWrite(_enLeft, speed);
  }
  if (_enRightPCF) {
    
  } else {
    analogWrite(_enRight, speed);
  }
}
void DriverControl::goBack(int speed) {
  if (_leftPinAPCF) {
    _pcf->digitalWrite(_leftPinA, LOW);
  } else {
    digitalWrite(_leftPinA, LOW);
  }
  if (_leftPinBPCF) {
    _pcf->digitalWrite(_leftPinB, HIGH);
  } else {
    digitalWrite(_leftPinB, HIGH);
  }
  if (_rightPinAPCF) {
    _pcf->digitalWrite(_rightPinA, LOW);
  } else {
    digitalWrite(_rightPinA, LOW);
  }
  if (_rightPinBPCF) {
    _pcf->digitalWrite(_rightPinB, HIGH);
  } else {
    digitalWrite(_rightPinB, HIGH);
  }
  if (_enLeftPCF) {

  } else {
    analogWrite(_enLeft, speed);
  }
  if (_enRightPCF) {
    
  } else {
    analogWrite(_enRight, speed);
  }
}
void DriverControl::turnLeft(int speed) {
  if (_leftPinAPCF) {
    _pcf->digitalWrite(_leftPinA, HIGH);
  } else {
    digitalWrite(_leftPinA, HIGH);
  }
  if (_leftPinBPCF) {
    _pcf->digitalWrite(_leftPinB, LOW);
  } else {
    digitalWrite(_leftPinB, LOW);
  }
  if (_rightPinAPCF) {
    _pcf->digitalWrite(_rightPinA, LOW);
  } else {
    digitalWrite(_rightPinA, LOW);
  }
  if (_rightPinBPCF) {
    _pcf->digitalWrite(_rightPinB, HIGH);
  } else {
    digitalWrite(_rightPinB, HIGH);
  }
  if (_enLeftPCF) {
    
  } else {
    analogWrite(_enLeft, speed);
  }
  if (_enRightPCF) {
    
  } else {
    analogWrite(_enRight, speed);
  }
}
void DriverControl::turnRight(int speed) {
  if (_leftPinAPCF) {
    _pcf->digitalWrite(_leftPinA, LOW);
  } else {
    digitalWrite(_leftPinA, LOW);
  }
  if (_leftPinBPCF) {
    _pcf->digitalWrite(_leftPinB, HIGH);
  } else {
    digitalWrite(_leftPinB, HIGH);
  }
  if (_rightPinAPCF) {
    _pcf->digitalWrite(_rightPinA, LOW);
  } else {
    digitalWrite(_rightPinA, LOW);
  }
  if (_rightPinBPCF) {
    _pcf->digitalWrite(_rightPinB, HIGH);
  } else {
    digitalWrite(_rightPinB, HIGH);
  }
  if (_enLeftPCF) {
    
  } else {
    analogWrite(_enLeft, speed);
  }
  if (_enRightPCF) {
    
  } else {
    analogWrite(_enRight, speed);
  }
}
void DriverControl::turn(int leftA, int leftB, int rightA, int rightB, int enLeft, int enRight) {
  if (_leftPinAPCF) {
    _pcf->digitalWrite(_leftPinA, leftA);
  } else {
    digitalWrite(_leftPinA, leftA);
  }
  if (_leftPinBPCF) {
    _pcf->digitalWrite(_leftPinB, leftB);
  } else {
    digitalWrite(_leftPinB, leftB);
  }
  if (_rightPinAPCF) {
    _pcf->digitalWrite(_rightPinA, rightA);
  } else {
    digitalWrite(_rightPinA, rightA);
  }
  if (_rightPinBPCF) {
    _pcf->digitalWrite(_rightPinB, rightB);
  } else {
    digitalWrite(_rightPinB, rightB);
  }
  if (_enLeftPCF) {
    
  } else {
    analogWrite(_enLeft, enLeft);
  }
  if (_enRightPCF) {
    
  } else {
    analogWrite(_enRight, enRight);
  }
}
void DriverControl::setLeftAPCF(bool isPCF) {
  _leftPinAPCF = isPCF;
}
void DriverControl::setLeftBPCF(bool isPCF) {
  _leftPinBPCF = isPCF;
}
void DriverControl::setRightAPCF(bool isPCF) {
  _rightPinAPCF = isPCF;
}
void DriverControl::setRightBPCF(bool isPCF) {
  _rightPinBPCF = isPCF;
}
void DriverControl::setENLeftPCF(bool isPCF) {
  _enLeftPCF = isPCF;
}
void DriverControl::setENRightPCF(bool isPCF) {
  _enRightPCF = isPCF;
}
