#ifndef MOVE_H
#define MOVE_H
#include <stdint.h>

typedef struct
{
	uint8_t ID;
	float angle;
} action;

typedef struct
{
	action a1;
	action a3;
    action a4;
	action a5;
} move;

extern move chuoshou[];
extern move yujin[];
extern move chuobei[];
extern move demo[];
#endif
