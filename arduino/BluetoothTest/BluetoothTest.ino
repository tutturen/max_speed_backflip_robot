#include <SoftwareSerial.h>
#include <PLabBTSerial.h>

#define txPin 3
#define rxPin 2
#define ledPin 13

PLabBTSerial btSerial(txPin, rxPin);

void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  while (btSerial.available()) {
    Serial.println("oo");
    char c = btSerial.read();
    Serial.print(c);
    btSerial.write("L");
    if (c == 'M') {
        digitalWrite(ledPin, HIGH);
        btSerial.write("1");
    }
    if (c == 'F') {
        digitalWrite(ledPin, LOW);
        btSerial.write("2");
    }
  }
  delay(1000);
  //Serial.println("yes");
}
