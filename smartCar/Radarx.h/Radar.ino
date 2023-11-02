#pragma once
class Radar {
  private:
    float _radarData[10];
    int _trigPin;
    int _echoPin;
    int _motorPin;
  public:
    Radar(int trigPin, int echoPin, int motorPin);
}
