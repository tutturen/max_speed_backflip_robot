#include <SoftwareSerial.h>
#include <PLabBTSerial.h>

#define txPin 3
#define rxPin 2
#define ledPin 13

PLabBTSerial btSerial(txPin, rxPin);

void setup() {
  btSerial.begin(9600);
  pinMode(ledPinOne, OUTPUT);
}

void loop() {
  while (btSerial.available()) {
    char c = btSerial.read();
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
}
