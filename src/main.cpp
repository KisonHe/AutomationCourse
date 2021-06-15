#include <Arduino.h>
#include "driver/ledc.h"
#include "hardware/motor.h"

// stepper asix6(18,5,19,2,10000);

stepper axis1(4,16,17,3,20000,1);
stepper axis3(25,26,27,5,10000,3);
stepper axis4(21,22,23,7,10000,4);
stepper axis5(5,18,19,1,100000,5);
int cnt = 0;

void mycb(stepper* self){
  Serial.print("ID ");
  Serial.print(self->getID());
  Serial.println(" Finished its work");
  self->setAngle(35);
}

void setup() {
  Serial.begin(115200);
  axis3.calibDoneHook = [](stepper* self){
    // Serial.print("Done!:");
    // Serial.println(millis());
    self->setAngle(60);
  };
  axis3.setTotalMS(40000);
  axis3.setAngleLimit(0,90);
  axis3.calibrate_start(1);

  axis4.calibDoneHook = axis3.calibDoneHook;
  axis4.setTotalMS(26000);
  axis4.setAngleLimit(0,180);
  axis4.calibrate_start(0);

  axis1.calibDoneHook = axis3.calibDoneHook;
  axis1.setTotalMS(26000);
  axis1.setAngleLimit(0,180);
  axis1.calibrate_start(0);

  axis5.calibDoneHook = axis3.calibDoneHook;
  axis5.setTotalMS(5300);
  axis5.setAngleLimit(0,90);
  axis5.calibrate_start(0);

  Serial.println("startClib: ");
  Serial.println(millis());

}

void loop() {
  // Serial.println(cnt);
  // asix6.handle();
  axis5.handle();
  axis1.handle();
  axis3.handle();
  axis4.handle();
  // static bool flag = true;
  // if (flag && axis3.isCalibDone()){
  //   flag = false;
  //   Serial.print("Done!:");
  //   Serial.println(millis());
  //   // axis5.setAngle(45);
  // }
  // if (digitalRead(19))
  //   cnt = 0;
  // else
  //   cnt++;
  // if (cnt >= 500){
  //   Serial.println("------real low!------");
  //   cnt = 0;
  // }
  vTaskDelay(1);
}