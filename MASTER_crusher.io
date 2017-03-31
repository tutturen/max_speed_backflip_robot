#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <Pushbutton.h>
#include <ZumoMotors.h>

#include <Servo.h>


#define treshold 1800

ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;

unsigned int reflectanceValues[6];
int servoPosition = 0;
Pushbutton button(12);

Servo servo;
#define trigPin 6
#define echoPin 5

void setup() {
  reflectanceSensors.init();
  button.waitForButton();  
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo.attach(4);
  servo.write(0);
  delay(500);
  servo.write(180);
  delay(500);
  servo.write(servoPosition);
}

void loop() {
  reflectanceSensors.read(reflectanceValues);

  int randomNumber = random(0, 181);

  Serial.println("RANDOM NR:");
  Serial.println(randomNumber);

  rotateServo(randomNumber);

  int distance = getDistance();

  Serial.println(distance);

  Serial.println(servoPosition);

  detectOpponent(servoPosition, distance);

  //rotateServo(90);

  delay(1000);
  
}

boolean isLineLeft(int sensorValues[]) {
  return sensorValues[1] < treshold;
}

boolean isLineRight(int sensorValues[]){
  return sensorValues[4] < treshold;
}

int getDistance() {
  long duration;
  long distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return (duration/2) / 29.1;
}

void rotateServo(int sPosition) {
  Serial.println("WRITING SERVO TO:");
  Serial.println(sPosition);
  servo.write(sPosition);
  servoPosition = sPosition;
  delay(500);
}

void detectOpponent(int positionServo, int distance) {
   Serial.println("DISTANCE: ");
   Serial.println(distance);
  if (distance < 40) {
    if (positionServo > 90) {
      // turn left
      int turnDegrees = 180 - positionServo;
      Serial.println("TURN DEGREES: ");
      Serial.println(turnDegrees);
      
      motors.setSpeeds(-400, 400);
      delay(turnDegrees);
      motors.setSpeeds(0, 0);
    } else {
      // turn right
      int turnDegrees = 90 - positionServo;
      Serial.println("TURN DEGREES: ");
      Serial.println(turnDegrees);

      motors.setSpeeds(400, -400);
      delay(turnDegrees);
      motors.setSpeeds(0, 0);
    }
  }
}

