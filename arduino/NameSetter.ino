#include <SoftwareSerial.h>
#include <PLabBTSerial.h>

#define txPin 3
#define rxPin 2

char BTName[] = "BACK_FLIP";
char ATCommand[] = "AT+NAMEMAX_SPEED";
PLabBTSerial btSerial(txPin, rxPin);

void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);
}

void loop() {
  Serial.print("Setting new name for device to: MAX_SPEED");
  Serial.println(BTName);

  btSerial.write(ATCommand);
  btSerial.write(BTName);
  btSerial.write(0x0D);
  btSerial.write(0x0A);
  delay(5000);
}
