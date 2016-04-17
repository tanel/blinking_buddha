
// servo
#include <Servo.h>
Servo myservo;
int pos = 0;
int servoPin = 9;
int maxServoDegrees = 180;
int minServoDegrees = 0;
int stepBack = 0;

// PIR
int PIR = 2;
int eyeState = LOW;
unsigned long lastMotionAt = 0;
unsigned long finishedAt = 0;

// Are we debugging? Dont leave it enabled
int debugging = 0;

void setup() {
  // PIR
  pinMode(PIR, INPUT);

  if (debugging) {
    Serial.begin(9600);
  }

  // Set up servo
  myservo.attach(servoPin);
  myservo.write(maxServoDegrees);
  delay(1000);
  myservo.write(maxServoDegrees - stepBack);
  delay(300);
  myservo.detach();
}

void debug(String text) {
  if (debugging) {
    Serial.println(text);
  }
}

void moveEyeDown() {
    myservo.attach(servoPin);

    for (pos = minServoDegrees; pos <= maxServoDegrees; pos += 1) {
      // in steps of 1 degree
      myservo.write(pos);
      delay(15);
    }
    myservo.write(maxServoDegrees - stepBack);
    delay(300);

    myservo.detach();

    eyeState = LOW;
}

void moveEyeUp() {
    myservo.attach(servoPin);

    for (pos = maxServoDegrees; pos >= minServoDegrees; pos -= 1) {
      myservo.write(pos);
      delay(15);
    }
    myservo.write(minServoDegrees);
    delay(300);

    myservo.detach();

    eyeState = HIGH;
}

void loop() {
  delay(100);

  int val = digitalRead(PIR);

  if (HIGH == val) {
    // If last show was less than N seconds ago,
    // Ignore the movement for now. We want the
    // visitor to be able to leave the premise,
    // before the show starts again.
    int secondsSince = (millis() - finishedAt) / 1000;
    if (finishedAt > 0 && secondsSince < 60) {
      return;
    }
    lastMotionAt = millis();
    if (LOW == eyeState) {
      char buf[30];
      sprintf(buf, "%d seconds since last finish", secondsSince);
      debug(buf);
      debug("Starting");
      moveEyeUp();
    }
    return;
  }

  if (LOW == val && HIGH == eyeState) {
    int secondsSince = (millis() - lastMotionAt) / 1000;
    if (secondsSince > 5) {
      debug("Stopping");
      moveEyeDown();

      finishedAt = millis();
      char buf[100];
      sprintf(buf, "finished at %lu", finishedAt);
      debug(buf);
    }
  }
}
