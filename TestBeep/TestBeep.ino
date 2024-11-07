#include "pitches.h"
uint beepPin = 13;
int melody[8];
int noteDurations[8];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(beepPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  alarmType0();
  alarmOut();
  delay(2000);  
  alarmType1();
  alarmOut();
  delay(2000);
  alarmType2();
  alarmOut();
  delay(2000);
  alarmType3();
  alarmOut();
  delay(2000);
  alarmType4();
  alarmOut();
  delay(5000);
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