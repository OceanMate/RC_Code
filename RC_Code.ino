#include <IBusBM.h>

IBusBM ibus;

int channelSize;
int rc_values[channelSize];
const int motor1_forward = 49;
const int motor1_backwards = 47;
const int motor1_control = 9;

int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue) {
  uint16_t ch = ibus.readChannel(channelInput);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

bool readSwitch(byte channelInput, bool defaultValue) {
  int intDefaultValue = (defaultValue) ? 100 : 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

void setup() {
  Serial.begin(115200);
  ibus.begin(Serial1);
}

void loop() {

  // Put jumper on motor controllers to use on board 5V regulator
  // Remove jumper from sides of inputs pins to controller speed of motors

  for (byte i = 0; i < channelSize; i++) {
    if (i < 4 | i == 6 | i == 7 | i == 8) {
      rc_values[i] = readChannel(i, -255, 255, 0);
    } else {
      rc_values[i] = readSwitch(i, false);
    }

    Serial.print("Ch");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(rc_values[i]);
    Serial.print(" | ");
  }


  if (rc_values[2] < 0) {
    digitalWrite(motor1_forward, LOW);
    digitalWrite(motor1_backwards, HIGH);
  } else {
    digitalWrite(motor1_forward, HIGH);
    digitalWrite(motor1_backwards, LOW);
  }
  analogWrite(motor1_control, abs(rc_values[2]));

  if (rc_values[4] == 1) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  Serial.print("\n");

  delay(10);
}