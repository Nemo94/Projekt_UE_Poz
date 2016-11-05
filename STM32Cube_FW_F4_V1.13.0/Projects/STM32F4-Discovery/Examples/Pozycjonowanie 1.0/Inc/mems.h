/**
  ******************************************************************************
  * @file    BSP/Inc/mems.h 
  * @author  MCD Application Team
  * @version V1.2.6
  * @date    06-May-2016
  * @brief   Header for mems.c module
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
#ifndef __MEMS_H
#define __MEMS_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define ABS(x)         (x < 0) ? (-x) : x





 
#define SPI2_TIMEOUT_MAX                            0x1000 /*<! The value of the maximal timeout for BUS waiting loops */

/* Chip Select macro definition */
#define AKCELERO_CS_LOW()       HAL_GPIO_WritePin(AKCELERO_CS_GPIO_PORT, AKCELERO_CS_PIN, GPIO_PIN_RESET)
#define AKCELERO_CS_HIGH()      HAL_GPIO_WritePin(AKCELERO_CS_GPIO_PORT, AKCELERO_CS_PIN, GPIO_PIN_SET)

/**
  * @brief  ACCELEROMETER Interface pins
  */
#define AKCELERO_CS_PIN                        GPIO_PIN_3                 /* PD.03 */
#define AKCELERO_CS_GPIO_PORT                  GPIOD                      /* GPIOD */
#define AKCELERO_CS_GPIO_CLK_ENABLE()          __GPIOD_CLK_ENABLE()
#define AKCELERO_CS_GPIO_CLK_DISABLE()         __GPIOD_CLK_DISABLE()
#define AKCELERO_INT_GPIO_PORT                 GPIOD                      /* GPIOD */
#define AKCELERO_INT_GPIO_CLK_ENABLE()         __GPIOD_CLK_ENABLE()
#define AKCELERO_INT_GPIO_CLK_DISABLE()        __GPIOD_CLK_DISABLE()
#define AKCELERO_INT1_PIN                      GPIO_PIN_0                 /* PD.00 */
#define AKCELERO_INT1_EXTI_IRQn                EXTI0_IRQn 
#define AKCELERO_INT2_PIN                      GPIO_PIN_1                 /* PD.01 */
#define AKCELERO_INT2_EXTI_IRQn                EXTI1_IRQn 

extern uint16_t akcelero_x;
extern uint16_t akcelero_y;
extern uint16_t akcelero_z;

/* Exported functions ------------------------------------------------------- */
void ReadAccelero(void);

#endif /* __MEMS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
