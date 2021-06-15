#ifndef MOTOR_H
#define MOTOR_H

#include "driver/gpio.h"
class stepper
{
private:
    bool calibDone = false;
    bool actionDone = false;
    bool direction = false;
    int32_t freq = -1;
    int32_t length_ms = -1;
    int32_t now_ms = 0; //position now
    int32_t target_ms = 0;
    int32_t stop_at_ms = 0;
    int16_t readSensorCount = 0;
    uint8_t DIRPin = -1;
    uint8_t limitPin = -1;
    uint8_t ledChannel = -1;
    uint8_t ID = 0;
    float Anglemin = 0;
    float Anglemax = 0;
    
    

    /* data */
public:
    void (*actionDoneHook)(stepper*) = nullptr;
    void (*calibDoneHook)(stepper*) = nullptr;
    static void setup();
    bool isCalibDone();
    stepper(uint8_t CP, uint8_t DIR, uint8_t Limit, uint8_t channel, int32_t frequency, uint8_t ID_);
    // void setFerq(int freq);
    void setTotalMS(int32_t ms);
    int setAngle(float pos);
    void calibrate_start(bool direction_);
    int handle();
    uint8_t getID();
    ~stepper();
    void setAngleLimit(float min, float max);
};



/*
 * LEDC Chan to Group/Channel/Timer Mapping
** ledc: 0  => Group: 0, Channel: 0, Timer: 0
** ledc: 1  => Group: 0, Channel: 1, Timer: 0
** ledc: 2  => Group: 0, Channel: 2, Timer: 1
** ledc: 3  => Group: 0, Channel: 3, Timer: 1
** ledc: 4  => Group: 0, Channel: 4, Timer: 2
** ledc: 5  => Group: 0, Channel: 5, Timer: 2
** ledc: 6  => Group: 0, Channel: 6, Timer: 3
** ledc: 7  => Group: 0, Channel: 7, Timer: 3
** ledc: 8  => Group: 1, Channel: 0, Timer: 0
** ledc: 9  => Group: 1, Channel: 1, Timer: 0
** ledc: 10 => Group: 1, Channel: 2, Timer: 1
** ledc: 11 => Group: 1, Channel: 3, Timer: 1
** ledc: 12 => Group: 1, Channel: 4, Timer: 2
** ledc: 13 => Group: 1, Channel: 5, Timer: 2
** ledc: 14 => Group: 1, Channel: 6, Timer: 3
** ledc: 15 => Group: 1, Channel: 7, Timer: 3
*/

#endif
