#include <SoftwareSerial.h>
#include <PLabBTSerial.h>

#define txPin 3
#define rxPin 2

char BTName[] = "MAX_SPEED";
char ATCommand[] = "AT+NAMEBACKFLIP_";
PLabBTSerial btSerial(txPin, rxPin);

void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);
}

void loop() {
  Serial.print("Setting new name for device to: Plab_");
  Serial.println(BTName);

  btSerial.write(ATCommand);
  btSerial.write(BTName);
  btSerial.write(0x0D);
  btSerial.write(0x0A);
  delay(1000);
}
