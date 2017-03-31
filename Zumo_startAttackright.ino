//rygger og svinger og roterer mot venstre. 


#include <ZumoMotors.h>
#include <Pushbutton.h>


ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);

const int MAX_SPEED = 400;

void setup() {
  button.waitForButton();
  // put your setup code here, to run once:
  if(100 > 0){
    motors.setSpeeds(-200, -200);
    delay(1000);
    motors.setSpeeds(200, -200);
    delay(300);
    motors.setSpeeds(400, 400);
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
