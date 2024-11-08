#include <Adafruit_PCF8574.h>
#include "DriverControl.h"
#include <analogWrite.h>
// Adafruit_PCF8574 pcf;
// LM298
DriverControl::DriverControl(Adafruit_PCF8574 *pcf) {
  _pcf = pcf;
}
DriverControl::DriverControl(Adafruit_PCF8574 *pcf, int leftA, int leftB, int rightA, int rightB, int enLeft, int enRight) {
  init(pcf, leftA, leftB, rightA, rightB, enLeft, enRight, _maxSpeedNum);
}
DriverControl::DriverControl(Adafruit_PCF8574 *pcf, int leftA, int leftB, int rightA, int rightB, int enLeft, int enRight, int maxSpeedNum) {
  init(pcf, leftA, leftB, rightA, rightB, enLeft, enRight, maxSpeedNum);
}

void DriverControl::init(Adafruit_PCF8574 *pcf, int leftA, int leftB, int rightA, int rightB, int enLeft, int enRight, int maxSpeedNum) {
  _leftPinA = leftA;
  _leftPinB = leftB;
  _rightPinA = rightA;
  _rightPinB = rightB;
  _enLeft = enLeft;
  _enRight = enRight;
  _maxSpeedNum = maxSpeedNum;
  _pcf = pcf;
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
  if (_enLeft >= 0) {
    if (_enLeftPCF) {
      _pcf->pinMode(_enLeft, OUTPUT);
    } else {
      pinMode(_enLeft, OUTPUT);
    }
  }
  if (_enRight >= 0) {
    if (_enRightPCF) {
      _pcf->pinMode(_enRight, OUTPUT);
    } else {
      pinMode(_enRight, OUTPUT);
    }
  }
}
void DriverControl::goStaight(int speed) {
  if (_leftPinAPCF) {
    Serial.print("leftPinAPCF ");
    Serial.print(_leftPinA);
    Serial.println(" High.");
    _pcf->digitalWrite(_leftPinA, HIGH);
  } else {
    Serial.print("leftPinA ");
    Serial.print(_leftPinA);
    Serial.println(" High.");
    digitalWrite(_leftPinA, HIGH);
  }
  if (_leftPinBPCF) {
    Serial.print("leftPinBPCF ");
    Serial.print(_leftPinB);
    Serial.println(" Low.");
    _pcf->digitalWrite(_leftPinB, LOW);
  } else {
    Serial.print("leftPinB ");
    Serial.print(_leftPinB);
    Serial.println(" Low.");
    digitalWrite(_leftPinB, LOW);
  }
  if (_rightPinAPCF) {
    Serial.print("rightPinAPCF ");
    Serial.print(_rightPinA);
    Serial.println(" High.");
    _pcf->digitalWrite(_rightPinA, HIGH);
  } else {
    Serial.print("rightPinA ");
    Serial.print(_rightPinA);
    Serial.println(" High.");
    digitalWrite(_rightPinA, HIGH);
  }
  if (_rightPinBPCF) {
    Serial.print("rightPinBPCF ");
    Serial.print(_rightPinB);
    Serial.println(" Low.");
    _pcf->digitalWrite(_rightPinB, LOW);
  } else {
    Serial.print("rightPinB ");
    Serial.print(_rightPinB);
    Serial.println(" Low.");
    digitalWrite(_rightPinB, LOW);
  }
  if (_enLeft >= 0) {
    if (_enLeftPCF) {
    
    } else {
      digitalWrite(_enRight, HIGH);
    }
  }
  
  if (_enRight >= 0) {
    if (_enRightPCF) {
      
    } else {
      digitalWrite(_enRight, HIGH);
      //analogWrite(_enRight, speed);
    }
  }

  // Serial.println("HIGH");
  // _pcf->digitalWrite(2, HIGH);
}
void DriverControl::goBack(int speed) {
  if (_leftPinAPCF) {
    Serial.print("leftPinAPCF ");
    Serial.print(_leftPinA);
    Serial.println(" Low.");
    _pcf->digitalWrite(_leftPinA, LOW);
  } else {
    Serial.print("leftPinA ");
    Serial.print(_leftPinA);
    Serial.println(" Low.");
    digitalWrite(_leftPinA, LOW);
  }
  if (_leftPinBPCF) {
    Serial.print("leftPinBPCF ");
    Serial.print(_leftPinB);
    Serial.println(" High.");
    _pcf->digitalWrite(_leftPinB, HIGH);
  } else {
    Serial.print("leftPinB ");
    Serial.print(_leftPinB);
    Serial.println(" High.");
    digitalWrite(_leftPinB, HIGH);
  }
  if (_rightPinAPCF) {
    Serial.print("rightPinAPCF ");
    Serial.print(_rightPinA);
    Serial.println(" Low.");
    _pcf->digitalWrite(_rightPinA, LOW);
  } else {
    Serial.print("rightPinA ");
    Serial.print(_rightPinA);
    Serial.println(" Low.");
    digitalWrite(_rightPinA, LOW);
  }
  if (_rightPinBPCF) {
    Serial.print("rightPinBPCF ");
    Serial.print(_rightPinB);
    Serial.println(" High.");
    _pcf->digitalWrite(_rightPinB, HIGH);
  } else {
    Serial.print("rightPinB ");
    Serial.print(_rightPinB);
    Serial.println(" High.");
    digitalWrite(_rightPinB, HIGH);
  }
  if (_enLeft >= 0) {
      if (_enLeftPCF) {

    } else {
      digitalWrite(_enLeft, HIGH);
    }
  }
  
  if (_enRight >= 0) {
    if (_enRightPCF) {
      
    } else {
      digitalWrite(_enRight, HIGH);
    }
  }
  // Serial.println("LOW");
  // _pcf->digitalWrite(2, LOW);
}
void DriverControl::turnLeft(int speed) {
  if (_leftPinAPCF) {
    Serial.print("leftPinAPCF ");
    Serial.print(_leftPinA);
    Serial.println(" High.");
    _pcf->digitalWrite(_leftPinA, LOW);
  } else {
    Serial.print("leftPinA ");
    Serial.print(_leftPinA);
    Serial.println(" High.");
    digitalWrite(_leftPinA, LOW);
  }
  if (_leftPinBPCF) {
    Serial.print("leftPinBPCF ");
    Serial.print(_leftPinB);
    Serial.println(" Low.");
    _pcf->digitalWrite(_leftPinB, LOW);
  } else {
    Serial.print("leftPinB ");
    Serial.print(_leftPinB);
    Serial.println(" Low.");
    digitalWrite(_leftPinB, LOW);
  }
  if (_rightPinAPCF) {
    Serial.print("rightPinAPCF ");
    Serial.print(_rightPinA);
    Serial.println(" High.");
    _pcf->digitalWrite(_rightPinA, HIGH);
  } else {
    Serial.print("rightPinA ");
    Serial.print(_rightPinA);
    Serial.println(" High.");
    digitalWrite(_rightPinA, HIGH);
  }
  if (_rightPinBPCF) {
    Serial.print("rightPinBPCF ");
    Serial.print(_rightPinB);
    Serial.println(" Low.");
    _pcf->digitalWrite(_rightPinB, LOW);
  } else {
    Serial.print("rightPinB ");
    Serial.print(_rightPinB);
    Serial.println(" Low.");
    digitalWrite(_rightPinB, LOW);
  }
  if (_enLeft >= 0) {
    if (_enLeftPCF) {
    
    } else {
      digitalWrite(_enRight, HIGH);
    }
  }
  
  if (_enRight >= 0) {
    if (_enRightPCF) {
      
    } else {
      digitalWrite(_enRight, HIGH);
      //analogWrite(_enRight, speed);
    }
  }
}
void DriverControl::turnRight(int speed) {
  if (_leftPinAPCF) {
    Serial.print("leftPinAPCF ");
    Serial.print(_leftPinA);
    Serial.println(" High.");
    _pcf->digitalWrite(_leftPinA, HIGH);
  } else {
    Serial.print("leftPinA ");
    Serial.print(_leftPinA);
    Serial.println(" High.");
    digitalWrite(_leftPinA, HIGH);
  }
  if (_leftPinBPCF) {
    Serial.print("leftPinBPCF ");
    Serial.print(_leftPinB);
    Serial.println(" Low.");
    _pcf->digitalWrite(_leftPinB, LOW);
  } else {
    Serial.print("leftPinB ");
    Serial.print(_leftPinB);
    Serial.println(" Low.");
    digitalWrite(_leftPinB, LOW);
  }
  if (_rightPinAPCF) {
    Serial.print("rightPinAPCF ");
    Serial.print(_rightPinA);
    Serial.println(" High.");
    _pcf->digitalWrite(_rightPinA, LOW);
  } else {
    Serial.print("rightPinA ");
    Serial.print(_rightPinA);
    Serial.println(" High.");
    digitalWrite(_rightPinA, LOW);
  }
  if (_rightPinBPCF) {
    Serial.print("rightPinBPCF ");
    Serial.print(_rightPinB);
    Serial.println(" Low.");
    _pcf->digitalWrite(_rightPinB, LOW);
  } else {
    Serial.print("rightPinB ");
    Serial.print(_rightPinB);
    Serial.println(" Low.");
    digitalWrite(_rightPinB, LOW);
  }
  if (_enLeft >= 0) {
    if (_enLeftPCF) {
    
    } else {
      digitalWrite(_enRight, HIGH);
    }
  }
  
  if (_enRight >= 0) {
    if (_enRightPCF) {
      
    } else {
      digitalWrite(_enRight, HIGH);
      //analogWrite(_enRight, speed);
    }
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
