
//COPYRIGHT 2016 MICHAL POTEMSKI

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

#define AKCELERO_CS_LOW()       HAL_GPIO_WritePin(AKCELERO_CS_GPIO_PORT, AKCELERO_CS_PIN, GPIO_PIN_RESET)
#define AKCELERO_CS_HIGH()      HAL_GPIO_WritePin(AKCELERO_CS_GPIO_PORT, AKCELERO_CS_PIN, GPIO_PIN_SET)


#define AKCELERO_CS_PIN                        GPIO_PIN_3                 /* PD.03 */
#define AKCELERO_CS_GPIO_PORT                  GPIOD                      /* GPIOD */
#define AKCELERO_CS_GPIO_CLK_ENABLE()          __GPIOD_CLK_ENABLE()
#define AKCELERO_CS_GPIO_CLK_DISABLE()         __GPIOD_CLK_DISABLE()


extern uint16_t akcelero_x;
extern uint16_t akcelero_y;
extern uint16_t akcelero_z;

/* Exported functions ------------------------------------------------------- */
void ReadAccelero(void);
void LIS3DH_Line_Init(void);
void LIS3DH_Config(void);
#endif /* __MEMS_H */
