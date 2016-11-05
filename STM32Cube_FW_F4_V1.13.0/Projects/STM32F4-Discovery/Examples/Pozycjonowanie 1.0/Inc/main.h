/**
  ******************************************************************************
  * @file    BSP/Inc/main.h 
  * @author  MCD Application Team
  * @version V1.2.6
  * @date    06-May-2016
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_accelerometer.h"
#include "mems.h"
#include "magnetometr.h"
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



#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
