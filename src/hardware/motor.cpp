#include "motor.h"
#include <Arduino.h>
#include "freertos/FreeRTOS.h"

stepper::stepper(uint8_t CP, uint8_t DIR, uint8_t Limit, uint8_t channel, int32_t frequency, uint8_t ID_):\
freq(frequency),DIRPin(DIR),limitPin(Limit),ledChannel(channel),ID(ID_){
    ledcSetup(channel, frequency, 4);
    ledcAttachPin(CP, channel);
    ledcWrite(channel, 0);
    pinMode(Limit, INPUT_PULLUP);
    pinMode(DIR, OUTPUT);
}

stepper::~stepper()
{
}

void stepper::calibrate_start(bool direction){
    digitalWrite(DIRPin, direction);
    ledcWrite(ledChannel, 8);
}
/**
 * @brief this function should run at least 
 *        every 5 ms
 * 
 * @return int 
 */
int stepper::handle(){
    if (calibDone == false){
        if (digitalRead(limitPin))
            readSensorCount = 0;
        else
            readSensorCount++;
        if (readSensorCount > 10){
            calibDone = true;
            actionDone = true;
            now_ms = 0;
            ledcWrite(ledChannel, 0);
        }
    }else{
        if (!actionDone){
            if (millis() > stop_at_ms){
                ledcWrite(ledChannel, 0);
                if (actionDoneHook != nullptr){
                    actionDoneHook(this);
                }
                actionDone = true;
            }
        }
    }

}

bool stepper::isCalibDone(){
    return calibDone;
}

int stepper::setAngle(float pos){
    if (length_ms <= 0)
        throw;
    if (!calibDone || pos > Anglemax || pos < Anglemin)
        return -1;
    target_ms = ((length_ms + 0.0) / (Anglemax - Anglemin)) * (pos - Anglemin);
    if (target_ms > now_ms)
        digitalWrite(DIRPin, 1);
    else
        digitalWrite(DIRPin, 0);
    stop_at_ms = abs(target_ms - now_ms) + millis();
    actionDone = false;
    ledcWrite(ledChannel, 8);
    return 0;
}

void stepper::setTotalMS(int32_t ms){
    length_ms = ms;
}

void stepper::setAngleLimit(float min, float max){
    if (min > max){
        float tmp = max;
        max = min;
        min = tmp;
    }
    Anglemin = min;
    Anglemax = max;
}
uint8_t stepper::getID(){
    return ID;
}
