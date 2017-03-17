#include <SoftwareSerial.h>
#include <PLabBTSerial.h>

#define txPin 2
#define rxPin 3
#define ledPin 13

PLabBTSerial btSerial(txPin, rxPin);

void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  while (btSerial.available()) {
    char c = btSerial.read();
    Serial.print(c);
    btSerial.write("Hallo");
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
