#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <Pushbutton.h>
#include <ZumoMotors.h>

#include <NewServo.h>

#define treshold 1800

ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;

double lastDistance = 100;

bool flipServo = false;

unsigned int reflectanceValues[6];
int servoPosition = 90;
Pushbutton button(12);

NewServo servo;
#define trigPin 6
#define echoPin 5
#define servoPin 4

#define REVERSE_SPEED     200 // 0 is stopped, 400 is full speed
#define TURN_SPEED        400
#define FORWARD_SPEED     100
#define REVERSE_DURATION  300 // ms
#define TURN_DURATION     400 // ms

void setup() {
  reflectanceSensors.init();
  button.waitForButton();  
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.println("Attaching servo");
  servo.attach(servoPin);
  servo.write(servoPosition);
}

void loop() {
  int distance = getDistance();

  Serial.println(servoPin);
  detectLine(reflectanceValues);
  rotateServoNext();
  killOpponent(servoPosition, distance);
  detectLine(reflectanceValues);
  rotateServoNext();
  detectOpponent(servoPosition, distance);
  detectLine(reflectanceValues);
  motors.setSpeeds(0, 0);
}

void detectLine(int reflectanceValues[]) {
  reflectanceSensors.read(reflectanceValues);
  Serial.println(reflectanceValues[1]);
  Serial.println(reflectanceValues[4]);
  if (isLineLeft(reflectanceValues)) {
    // if leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
  else if (isLineRight(reflectanceValues))
  {
    // if rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }
}

boolean isLineLeft(int sensorValues[]) {
  return sensorValues[1] < treshold;
}

boolean isLineRight(int sensorValues[]){
  return sensorValues[4] < treshold;
}

double getDistance() {
  long duration;
  long distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  double newDistance = (duration/2) / 29.1;
  if (newDistance < (lastDistance + 10) && newDistance > (lastDistance - 10)) {
      lastDistance = newDistance;
      return newDistance;
  }
  lastDistance = newDistance;
  return 100;
}

void rotateServoNext() {
  Serial.println(servoPosition);
  if (servoPosition > 160 || servoPosition < 20) {
    flipServo = !flipServo;
  }
  if (flipServo) {
     rotateServo((servoPosition + 20) % 181);
  } else {
      rotateServo((servoPosition - 20) % 0);
  }
}

void rotateServo(int sPosition) {
  servo.write(sPosition);
  servoPosition = sPosition;
  delay(100);
}

void killOpponent(int positionServo, double distance) {
  int maxAngle = 120;
  int minAngle = 60;
  int maxDistance = 40;
  int superDistance = 10;


  if (positionServo > minAngle && positionServo < maxAngle) {
    if (distance < maxDistance) {
      // ATTACK!
      motors.setSpeeds(400, 400);
      delay(300);
    }
  }
}

void detectOpponent(int positionServo, double distance) {
   Serial.println(distance);
  if (distance < 20) {
    if (positionServo > 90) {
      // turn left
      int turnDegrees = 180 - positionServo;
      
      motors.setSpeeds(-400, 400);
      delay(100);
      motors.setSpeeds(0, 0);
    } else {
      // turn right
      int turnDegrees = 90 - positionServo;

      motors.setSpeeds(400, -400);
      delay(100);
      motors.setSpeeds(0, 0);
    }
    rotateServo(90);
    delay(200);
  }
}
