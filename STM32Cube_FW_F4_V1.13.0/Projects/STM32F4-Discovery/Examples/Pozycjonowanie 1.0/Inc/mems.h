/**
  * COPYRIGHT(c) 2016 STMicroelectronics
 
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
