/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAGNETOMETR_H
#define __MAGNETOMETR_H



#include "main.h"
#include <math.h>

#define M_PI 3.1415926

#define   HMC5883_SLAVE_ADR   0x3C



#define   HMC5883_REG_CONFB   0x01  // conf register-b
#define   HMC5883_REG_MODE    0x02  // mode register
#define   HMC5883_REG_DATA    0x03  // first data register
#define   HMC5883_REG_STATUS  0x09  // status register
#define   HMC5883_REG_IDA     0x0A  // id register-1
#define   HMC5883_REG_IDB     0x0B  // id register-2
#define   HMC5883_REG_IDC     0x0C  // id register-3



#define   HMC5883_POWERON_DELAY    15 //0x50000
#define   HMC5883_READ_DELAY    	 2// 10000        // >1000
#define   HMC5883_TIMEOUT         1000
#define   HMC5883_IDA_VALUE       0x48
#define   HMC5883_IDB_VALUE       0x34
#define   HMC5883_IDC_VALUE       0x33


typedef struct {
  int16_t x_raw;     // 12bit Roh-Daten der X-Achse (in +/- 2048)
  int16_t y_raw;     // 12bit Roh-Daten der Y-Achse (in +/- 2048)
  int16_t z_raw;     // 12bit Roh-Daten der Z-Achse (in +/- 2048)
  uint8_t cal_mode;  // kalibration mode
  int16_t x_min;     // kablibrationswert
  int16_t x_max;     // kablibrationswert
  int16_t y_min;     // kablibrationswert
  int16_t y_max;     // kablibrationswert
  int16_t z_min;     // kablibrationswert
  int16_t z_max;     // kablibrationswert
}HMC5883_RAW_t;



uint8_t HMC5883_Init(void);
uint8_t HMC5883_Read(void);
uint8_t HMC5883_SetScale(uint16_t value);
void HMC5883_Start_Calibration(void);
void HMC5883_Stop_Calibration(void);

void I2C1_Init(void);

void p_HMC5883_calcData(void);


extern uint16_t kat_od_polnocy;


#endif
