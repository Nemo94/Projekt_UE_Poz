/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STEPPER_DRIVER_H
#define __STEPPER_DRIVER_H



#include "main.h"

#define MOTOR1 1
#define MOTOR2 2
#define MOTOR3 3
#define MOTOR4 4
#define NUMBER_OF_MOTORS 4

#define MOTOR_CLOCK_PINS_MACRO() __GPIOE_CLK_ENABLE()

#define STEPPER1_EN_PORT 			GPIOE
#define STEPPER1_DIR_PORT 		GPIOE
#define STEPPER1_STEP_PORT		GPIOE
#define STEPPER1_EN_PIN				GPIO_PIN_4
#define STEPPER1_DIR_PIN 			GPIO_PIN_5
#define STEPPER1_STEP_PIN			GPIO_PIN_6


#define STEPPER2_EN_PORT			GPIOE
#define STEPPER2_DIR_PORT			GPIOE
#define STEPPER2_STEP_PORT		GPIOE
#define STEPPER2_EN_PIN				GPIO_PIN_7
#define STEPPER2_DIR_PIN			GPIO_PIN_8
#define STEPPER2_STEP_PIN			GPIO_PIN_9

#define STEPPER3_EN_PORT			GPIOE
#define STEPPER3_DIR_PORT			GPIOE
#define STEPPER3_STEP_PORT		GPIOE
#define STEPPER3_EN_PIN				GPIO_PIN_10
#define STEPPER3_DIR_PIN			GPIO_PIN_11
#define STEPPER3_STEP_PIN			GPIO_PIN_12

#define STEPPER4_EN_PORT			GPIOE
#define STEPPER4_DIR_PORT			GPIOE
#define STEPPER4_STEP_PORT		GPIOE
#define STEPPER4_EN_PIN				GPIO_PIN_13
#define STEPPER4_DIR_PIN			GPIO_PIN_14
#define STEPPER4_STEP_PIN			GPIO_PIN_15

void AllSteppersInit(uint8_t number_of_steppers);
void StepperXEnable(uint8_t stepper_id);
void StepperXDisable(uint8_t stepper_id);
void StepperXExecute(uint8_t stepper_id, uint16_t steps, uint8_t direction);

#endif
