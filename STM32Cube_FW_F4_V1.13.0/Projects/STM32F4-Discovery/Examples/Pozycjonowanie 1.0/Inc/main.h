/**
  *COPYRIGHT(c) 2016 STMicroelectronics
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_accelerometer.h"
#include "mems.h"
#include "magnetometr.h"
#include "stepper_driver.h"
#include "pwm.h"
#include <stdio.h>

typedef enum HMC5883_SCALE_t HMC5883_SCALE_t;
	
enum HMC5883_SCALE_t{
  HMC5883_0G8 = 0,  // +/- 0,88 Gauss
  HMC5883_1G3,      // +/- 1,3 Gauss (default)
  HMC5883_1G9,      // +/- 1,9 Gauss
  HMC5883_2G5,      // +/- 2,5 Gauss
  HMC5883_4G0,      // +/- 4,0 Gauss
  HMC5883_4G7,      // +/- 4,7 Gauss
  HMC5883_5G6,      // +/- 5,6 Gauss
  HMC5883_8G1       // +/- 8,1 Gauss
};


/* Exported types ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void Toggle_Leds(void);
void Error_Handler(void);
extern void I2C1_Init(void);
extern void ReadAccelero(void);
extern uint8_t HMC5883_Init(void);
extern uint8_t HMC5883_Read(void);
extern void HMC5883_Start_Calibration(void);
extern void HMC5883_Stop_Calibration(void);
void SetMagnetometerScaleValue(HMC5883_SCALE_t scale);
extern void USART3_Init(void);
extern void send_char(char c);

extern void AllSteppersInit(uint8_t number_of_steppers);
extern void StepperXEnable(uint8_t stepper_id);
extern void StepperXDisable(uint8_t stepper_id);
extern void StepperXExecute(uint8_t stepper_id, uint16_t steps, uint8_t direction);
extern void TIM2_Init(void);
extern void ServoSetAngle(uint8_t servo_id, float angle);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
