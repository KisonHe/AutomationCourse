#include <Arduino.h>
#include "driver/ledc.h"
#include "hardware/motor.h"

// stepper asix6(18,5,19,2,10000);
stepper axis5(21,22,23,1,100000,5);
int cnt = 0;

void mycb(stepper* self){
  Serial.print("ID ");
  Serial.print(self->getID());
  Serial.println(" Finished its work");
}

void setup() {
  Serial.begin(115200);
  // pinMode(19, INPUT_PULLUP);
  // asix6.calibrate_start(1);
  axis5.actionDoneHook = mycb;
  axis5.setTotalMS(5300);
  axis5.setAngleLimit(0,90);
  axis5.calibrate_start(0);
  Serial.println("startClib: ");
  Serial.println(millis());
  // vTaskDelay(100);
}

void loop() {
  // Serial.println(cnt);
  // asix6.handle();
  axis5.handle();
  static bool flag = true;
  if (flag && axis5.isCalibDone()){
    flag = false;
    Serial.print("Done!:");
    Serial.println(millis());
    axis5.setAngle(45);
  }
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