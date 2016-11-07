#ifndef __pwm_H
#define __pwm_H

#include "main.h"


extern void Error_Handler(void);

void TIM2_Init(void);
void ServoSetAngle(uint8_t servo_id, float angle);                   
#endif
