#include "DriverControl.h"
#include <analogWrite.h>
#include <Adafruit_PCF8574.h>
#include "CarRadar.h"

// https://madhephaestus.github.io/ESP32Servo/annotated.html

/* Example for 1 output LED that is connected from power to the GPIO expander pin #7
 * Note the LEDs must be connected with the CATHODES to the expander, to SINK current!
 * The PCF8574 cannot SOURCE current!
 */

Adafruit_PCF8574 pcf8574;
DriverControl *driver;

long _lastTime;
bool _isGo = true;
int _timeGap = 300;
int _currentAngle = 0;
int _servoPin = 13;
Servo *_myservo;
EchoControl *_echo;
CarRadar *radar;

void setup() {
  // 初始化调试串口波特率
  Serial.begin(115200);
  Serial.println("----------Start----------");
  Serial.print("SDA:");
  Serial.println(SDA);
  Serial.print("SCL:");
  Serial.println(SCL);
  _lastTime = millis();
  // while (!pcf8574.begin(0x20, &Wire)) {
  //   Serial.println("Couldn't find PCF8574");
  //   delay(500);
  // }
  // Test Driver
  // driver = new DriverControl(&pcf8574, 14, 2, 15, 3, -1, -1);
  // Serial.println("found");
  
  // driver->setLeftAPCF(false);
  // driver->setLeftBPCF(true);
  // driver->setENLeftPCF(false);

  // driver->setRightAPCF(false);
  // driver->setRightBPCF(true);
  // driver->setENRightPCF(false);
  // driver->setup();
  // Serial.println("--------------------");


  // Test Radar
  // radar = new CarRadar(&pcf8574, 4, 16, 2);
  // _myservo = new Servo();
  // ESP32PWM::allocateTimer(0);
	// ESP32PWM::allocateTimer(1);
	// ESP32PWM::allocateTimer(2);
	// ESP32PWM::allocateTimer(3);
	// _myservo->setPeriodHertz(50);    // standard 50 hz servo
	// _myservo->attach(_servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
  // _echo = new EchoControl(&pcf8574, 14, 15);
  // _echo->setup();

  radar = new CarRadar(&pcf8574, 4, 16, 2);
  radar->setTrigPinAPCF(true);
  // Serial.println("radar start");
}

void loop() {
  // Test Driver
  // driver->goStaight(255);
  // delay(5000);
  // driver->goBack(255);
  // delay(5000);
  // driver->turnRight(255);
  // delay(5000);
  // driver->turnLeft(255);
  // delay(5000);

  // radar->loop();
  // long currentTime = millis();
  // if ((currentTime - _lastTime) > _timeGap || (currentTime - _lastTime) < 0) {
  //   Serial.print("start angle: ");
  //   Serial.println(_currentAngle);
  //   _myservo->write(_currentAngle);
  //   if (_isGo) {
  //     _currentAngle += 10;
  //     if (_currentAngle > 180) {
  //       _currentAngle -= 20;
  //       _isGo = false;
  //     }
  //   } else {
  //     _currentAngle -= 10;
  //     if (_currentAngle < 0) {
  //       _currentAngle += 20;
  //       _isGo = true;
  //     }
  //   }
  //   _lastTime = millis();
  // }

  // Serial.print("distance:");
  // Serial.println(_echo->getDistance());
  // Serial.println("----------------------------");

  
  radar->loop();
}