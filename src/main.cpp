#include <Arduino.h>
#include "driver/ledc.h"
#include "hardware/motor.h"
#include "hardware/move.h"


#ifdef CORE_DEBUG_LEVEL
#undef CORE_DEBUG_LEVEL
#endif

#define CORE_DEBUG_LEVEL 3
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
static const char* TAG = "main.cpp";


// stepper asix6(18,5,19,2,10000);
static move* nowWhatMoves = nullptr;
static int8_t nowTotalNumOfMoves = 0;
static int8_t nowMoveNum = 0;
int8_t calib_map = 0;
int8_t action_map = 0;
int8_t status = 0;
stepper axis1(4,16,17,3,40000,1);
stepper axis3(25,26,27,5,10000,3);
stepper axis4(21,22,23,7,10000,4);
stepper axis5(5,18,19,1,100000,5);

int8_t run_move(move* whatmove, int8_t numOfMoves){
  if (nowWhatMoves != nullptr){
    //handle the conflict here
  }
  else{
    action_map = 0b00111010;
    nowWhatMoves = whatmove;
    nowTotalNumOfMoves = numOfMoves;
    nowMoveNum = 0;
    return 0;
  }
}

void move_action_cb(stepper* self){
  action_map |= 1 << (self->getID());
  return;
  // Serial.print("ID ");
  // Serial.print(self->getID());
  // self->getID();
  // Serial.println(" Finished its work");
  // self->setAngle(35);
}

int8_t move_handle();

void setup() {
  Serial.begin(115200);
  ESP_LOGI(TAG, "Booting");
  axis3.actionDoneHook = move_action_cb;
  axis3.calibDoneHook = [](stepper* self){
    // Serial.print("Done!:");
    // Serial.println(millis());
    calib_map |= 1 << (self->getID());
    // self->setAngle(60);
  };
  axis3.setTotalMS(40000);
  axis3.setAngleLimit(0,90);
  axis3.calibrate_start(1);

  axis4.actionDoneHook = move_action_cb;
  axis4.calibDoneHook = axis3.calibDoneHook;
  axis4.setTotalMS(26000);
  axis4.setAngleLimit(0,180);
  axis4.calibrate_start(0);

  axis1.actionDoneHook = move_action_cb;
  axis1.calibDoneHook = axis3.calibDoneHook;
  axis1.setTotalMS(13000);
  axis1.setAngleLimit(0,180);
  axis1.calibrate_start(0);

  axis5.actionDoneHook = move_action_cb;
  axis5.calibDoneHook = axis3.calibDoneHook;
  axis5.setTotalMS(5300);
  axis5.setAngleLimit(0,90);
  axis5.calibrate_start(0);

  Serial.println("startClib: ");
  Serial.println(millis());

}

void loop() {
  static bool flag = 1;

  int incoming = Serial.available();
  if (incoming)
  {
    char *buffer = nullptr;
    if (incoming < 100)
      buffer = new char[incoming + 1];
    memset(buffer, 0, incoming + 1);
    Serial.readBytes(buffer, incoming);
    if (buffer[0] == '1' && status == 0 && (calib_map == 0b00111010)){
      status = 1;
      run_move(chuoshou,17);
      Serial.println("enter state 1");
      //FIXME 
      // run_move()
    }
    else if (buffer[1] == '2' && status == 1 && action_map == 0b00111010){
      status = 2;
      run_move(chuobei,24);
      Serial.println("enter state 2");
      //FIXME 
      // run_move()
    }
    else if (buffer[1] == '3' && status == 2 && action_map == 0b00111010){
      status = 3;
      Serial.println("enter state 3");
      //FIXME 
      // run_move()
    }
    
    delete buffer;
  }
  move_handle();
  vTaskDelay(1);
}

int8_t move_handle(){  
  axis5.handle();
  axis1.handle();
  axis3.handle();
  axis4.handle();
  if (nowWhatMoves != nullptr){
    if (calib_map == 0b00111010){//all 4 axis calibed
      if(action_map == 0b00111010){
        action_map = 0;
        nowMoveNum++;
        if (nowMoveNum > nowTotalNumOfMoves){
          nowWhatMoves = nullptr;
          nowTotalNumOfMoves = 0;
          nowMoveNum = 0;
          Serial.println("end of moves");
          return 1;
        }
        Serial.println("\n-------\nnow move is");
        Serial.println(nowMoveNum);
        axis1.setAngle(nowWhatMoves->a1.angle);
        axis3.setAngle(nowWhatMoves->a3.angle);

        axis4.setAngle(nowWhatMoves->a4.angle);
        axis5.setAngle(nowWhatMoves->a5.angle);
        nowWhatMoves++;
        return 0;
      }
      return -1;
      
    }
    return -2;
  }else{
    return 0;
  }

}
