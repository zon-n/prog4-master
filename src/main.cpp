#include <Arduino.h>
#include <Servo.h>

int readChannel(int channelINput, int minLimit, int maxLimit, int defaultValue)
{
  int ch = pulseIn(channelINput, HIGH, 30000);
  if (ch < 100)
    return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

bool readSwitch(int channelInput, bool defaultValue) {
  int intDefaultValue = (defaultValue)? 100 : 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

Servo servo;

void setup()
{
  Serial.begin(115200);
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);


  servo.attach(7);
}

void loop()
{
  int ch1Value = readChannel(1, -100, 100, 0);
  Serial.println(ch1Value);
  int ch2Value = readChannel(2, -100, 100, 0);
  Serial.println(ch2Value);
  int ch3Value = readChannel(3, 0, 100, 0);
  Serial.println(ch3Value);
  int ch4Value = readChannel(4, -100, 100, 0);
  Serial.println(ch4Value);
  int ch5Value = readChannel(5, 0, 100, 0);
  Serial.println(ch5Value);
  int ch6Value = readChannel(6, 0, 100, 0);
  Serial.println(ch6Value);
  Serial.println("__________________________________");
  delay(1000);

  // int angle = map(ch1Value, -100, 100, 180, 0);
  // servo.write(angle);
  // Serial.println(angle);
}