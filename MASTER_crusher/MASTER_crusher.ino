#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <Pushbutton.h>
#include <ZumoMotors.h>
#include <PLabBTSerial.h>


ZumoReflectanceSensorArray reflectanceSensors(QTR_NO_EMITTER_PIN);
ZumoMotors motors;

double lastDistance = 100;

unsigned int reflectanceValues[6];
Pushbutton button(12);

#define trigPin 6
#define echoPin 5

#define rxPin 3
#define txPin 4

#define yellowLedPin 2

PLabBTSerial btSerial(txPin, rxPin);

#define treshold 1800
#define REVERSE_SPEED     200 // 0 is stopped, 400 is full speed
#define TURN_SPEED        400
#define FORWARD_SPEED     100
#define REVERSE_DURATION  300 // ms
#define TURN_DURATION     400 // ms

bool useBt = false;
bool useRightProgram = false;

void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);
  reflectanceSensors.init();
  randomSeed(analogRead(0));
  Serial.println("Starting master crusher");
  setBluetoothProgram();
  button.waitForButton();

  if (useRightProgram) {
    superStartRight();
  } else {
    superStartLeft();
  }

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(yellowLedPin, OUTPUT);
}

void loop() {
  Serial.println("Using right program:");
  Serial.println(useRightProgram);


  int distance = getDistance();
  detectLine();
  detectOpponent(distance);
  killOpponent(distance);
  detectLine();
  runRandom();
  motors.setSpeeds(0, 0);
}

void setBluetoothProgram() {
  if (!useBt) {
    Serial.println("We are not using bluetooth");
    random(2, 500);
    random(1, 4000);
    int nr = random(1, 399);
    Serial.println(nr);
    if (nr < 200) {
      useRightProgram = true;
      Serial.println("Using right prog, setting led high");
      digitalWrite(yellowLedPin, HIGH);
    } else {
      Serial.println("Using left prog, setting led low");
      useRightProgram = false;
      digitalWrite(yellowLedPin, LOW);
    }
    return;
  }
  Serial.println("We ARE using bluetooth");
  while (true) {
    Serial.println("Venter BT");
    while (btSerial.available()) {
      char c = btSerial.read();
      Serial.print(c);
      if (c == 'M') {
          useRightProgram = true;
          btSerial.write("1");
          return;
      }
      if (c == 'F') {
          useRightProgram = false;
          btSerial.write("2");
          return;
      }
    }
    delay(500);
  }

}

void runRandom() {
  int dis = getDistance();
  if (dis < 40) {
    return;
  }
  motors.setSpeeds(random(-400, 400), random(-400, 400));
  delay(300);
  motors.setSpeeds(0, 0);
}

void superStartRight() {
    motors.setSpeeds(-200, -200);
    delay(1000);
    motors.setSpeeds(200, -200);
    delay(300);
    motors.setSpeeds(400, 400);
    delay(500);
    motors.setSpeeds(0, 0);
}

void superStartLeft() {
    motors.setSpeeds(-200, -200);
    delay(1000);
    motors.setSpeeds(-200, 200);
    delay(300);
    motors.setSpeeds(400, 400);
    delay(500);
    motors.setSpeeds(0, 0);
}

void detectLine() {
  reflectanceSensors.read(reflectanceValues);
  if (isLineLeft(reflectanceValues)) {
    // if leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
  }
  else if (isLineRight(reflectanceValues))
  {
    // if rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
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

void killOpponent(double distance) {
  int maxAngle = 120;
  int minAngle = 60;
  int maxDistance = 60;
  int superDistance = 10;

  int dis = getDistance();

  if (dis < maxDistance) {
    // ATTACK!
    motors.setSpeeds(400, 400);
    delay(300);
  }
}

void detectOpponent(double distance) {
  int i = 0;
  int dir = random(1, 101);
  while (i < 5) {
    int dis = getDistance();
    if (dis < 20) {
        motors.setSpeeds(0, 0);
        return;
    }
    delay(100);
    if (dir < 50) {
      motors.setSpeeds(-200, 200);

    } else {
      motors.setSpeeds(200, -200);
    }
    i++;
  }
    motors.setSpeeds(0, 0);
}

