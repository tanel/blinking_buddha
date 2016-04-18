
// servo
#include <Servo.h>
Servo myservo;
int pos = 0;
const int servoPin = 9;
const int maxServoDegrees = 90;
const int minServoDegrees = 0;

#define OPEN HIGH
#define CLOSED LOW

// PIR
const int PIR = 2;
int eyeState = OPEN;
unsigned long lastMotionAt = 0;
unsigned long finishedAt = 0;
const int servoDelay = 5;
const int servoLongDelay = 100;

void setup() {
  // PIR
  pinMode(PIR, INPUT);

  Serial.begin(9600);

  // Set up servo
  myservo.attach(servoPin);
  myservo.write(minServoDegrees);
  delay(1000);
  myservo.write(minServoDegrees);
  delay(servoLongDelay);
  myservo.detach();
}

void closeEye() {
  myservo.attach(servoPin);

  for (pos = minServoDegrees; pos <= maxServoDegrees; pos += 1) {
    // in steps of 1 degree
    myservo.write(pos);
    delay(servoDelay);
  }
  myservo.write(maxServoDegrees);
  delay(servoLongDelay);

  myservo.detach();

  eyeState = CLOSED;
}

void openEye() {
  myservo.attach(servoPin);

  for (pos = maxServoDegrees; pos >= minServoDegrees; pos -= 1) {
    myservo.write(pos);
    delay(servoDelay);
  }
  myservo.write(minServoDegrees);
  delay(servoLongDelay);

  myservo.detach();

  eyeState = OPEN;
}

void loop() {
  delay(1000);

  int movement = digitalRead(PIR);
  if (movement && OPEN == eyeState) {
    Serial.println("Closing eye");
    closeEye();
  } else if (CLOSED == eyeState) {
    Serial.println("Opening eye");
    openEye();
  }

  
}
