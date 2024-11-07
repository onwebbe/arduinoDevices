#include <Arduino.h>
#include <Wire.h>
#include "AHT10_8266.h"
#include "BMP180_8266.h"
#include "Displayer.h"
#include <Adafruit_AHTX0.h>
#include "pitches.h"

bool isAHT10 = false;
bool isBMP180 = false;
long timeGap = 500;
long timeGapFace = 500;
long lastTime = 0;
long lastTimeFace = 0;
Displayer displayer;
Adafruit_AHTX0 aht;

uint beepPin = 13;
long highFrequency = 440;
long lowFrequency = 261.6; 

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Wire.begin(0x38);
  // if (! aht.begin()) {
  if (! AHT10_8266::begin()) {
    Serial.println(F("AHT10 not detected!"));
  } else {
    isAHT10 = true;
  }
  if (!BMP180_8266::begin()) {
    Serial.println(F("BMP180 not detected!"));
  } else {
    isBMP180 = true;
  }
  Serial.println("1");
  displayer.init();
  Serial.println("2");
  lastTime = millis();
  Serial.println("3");
}
float temperatureValue = 0;
float humidityValue = 0;
float pressure = 0;
void loop() {
  // put your main code here, to run repeatedly:
  if ((millis() - lastTime) >= timeGap || (millis() - lastTime) < 0) {
    Serial.println("5");
    if (isAHT10) {
      // sensors_event_t humidity, temp;
      // aht.getEvent(&humidity, &temp);
      // temperatureValue = temp.temperature;
      // humidityValue = humidity.relative_humidity;
      temperatureValue = AHT10_8266::getTemperature();
      humidityValue = AHT10_8266::getHumidity();
      Serial.print(temperatureValue);
      Serial.println(" C");
      Serial.println();
  
      Serial.print(humidityValue);
      Serial.println(" %");
      Serial.println();
    }
    if (isBMP180) {
      pressure = BMP180_8266::getPressure();
      Serial.print(pressure);
      Serial.println(" hPa");
      Serial.println();
    }
    lastTime = millis();
    checkAlarm(temperatureValue, humidityValue, pressure);
  }
  if ((millis() - lastTimeFace) >= timeGap || (millis() - lastTimeFace) < 0) {
    int isLaugh = random(0, 10);
    displayer.setLaugh(isLaugh>5);
    lastTimeFace = millis();
  }
  displayer.setWeatherData(temperatureValue, humidityValue, pressure);
  displayer.loop();
}

void checkAlarm(double temperature, double humidity, double pressure) {
  if (pressure <= 900) {
    alarm(0);
  }
  if (temperature <= 10) {
    alarm(4);
  } else if (temperature <= 15) {
    alarm(3);
  }
  if (temperature > 30) {
    alarm(1);
  }
  if (humidity >= 75) {
    alarm(2);
  }
}
void alarm(int type) {
  switch(type) {
    case 0:
      alarmType0();
      break;
    case 1:
      alarmType1();
      break;
    case 2:
      alarmType2();
      break;
    case 3:
      alarmType3();
      break;
    case 4:
      alarmType4();
      break;
  }
  alarmOut();
}
void alarmOut() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(beepPin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(beepPin);
  }
}
void alarmType0() {
  melody[0] = NOTE_C4;
  melody[1] = NOTE_G3;
  melody[2] = NOTE_G3;
  melody[3] = NOTE_A3;
  melody[4] = NOTE_G3;
  melody[5] = 0;
  melody[6] = NOTE_B3;
  melody[7] = NOTE_C4;

  noteDurations[0] = 4;
  noteDurations[1] = 8;
  noteDurations[2] = 8;
  noteDurations[3] = 4;
  noteDurations[4] = 4;
  noteDurations[5] = 4;
  noteDurations[6] = 4;
  noteDurations[7] = 4;
}
void alarmType1() {
  melody[0] = NOTE_C5;
  melody[1] = NOTE_D5;
  melody[2] = NOTE_C5;
  melody[3] = NOTE_D5;
  melody[4] = NOTE_C5;
  melody[5] = NOTE_G4;
  melody[6] = NOTE_C5;
  melody[7] = 0;

  noteDurations[0] = 8;
  noteDurations[1] = 8;
  noteDurations[2] = 8;
  noteDurations[3] = 8;
  noteDurations[4] = 4;
  noteDurations[5] = 4;
  noteDurations[6] = 4;
  noteDurations[7] = 4;
}
void alarmType2() {
  melody[0] = NOTE_E5;
  melody[1] = NOTE_D5;
  melody[2] = NOTE_E5;
  melody[3] = NOTE_E5;
  melody[4] = NOTE_D5;
  melody[5] = NOTE_C5;
  melody[6] = NOTE_B4;
  melody[7] = NOTE_A4;

  noteDurations[0] = 16;
  noteDurations[1] = 16;
  noteDurations[2] = 1;
  noteDurations[3] = 8;
  noteDurations[4] = 8;
  noteDurations[5] = 8;
  noteDurations[6] = 8;
  noteDurations[7] = 2;
}
void alarmType3() {
  melody[0] = NOTE_A3;
  melody[1] = NOTE_B3;
  melody[2] = NOTE_C4;
  melody[3] = NOTE_D4;
  melody[4] = NOTE_E4;
  melody[5] = 0;
  melody[6] = NOTE_E5;
  melody[7] = 0;

  noteDurations[0] = 8;
  noteDurations[1] = 8;
  noteDurations[2] = 8;
  noteDurations[3] = 8;
  noteDurations[4] = 2;
  noteDurations[5] = 8;
  noteDurations[6] = 8;
  noteDurations[7] = 8;
}
void alarmType4() {
  melody[0] = NOTE_A3;
  melody[1] = NOTE_A3;
  melody[2] = 0;
  melody[3] = NOTE_A3;
  melody[4] = NOTE_A3;
  melody[5] = 0;
  melody[6] = NOTE_A3;
  melody[7] = 0;

  noteDurations[0] = 8;
  noteDurations[1] = 8;
  noteDurations[2] = 2;
  noteDurations[3] = 8;
  noteDurations[4] = 8;
  noteDurations[5] = 2;
  noteDurations[6] = 8;
  noteDurations[7] = 8;
}
