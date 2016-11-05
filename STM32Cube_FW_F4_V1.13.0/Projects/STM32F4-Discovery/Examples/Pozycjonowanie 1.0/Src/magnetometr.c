//PB6     ------> I2C1_SCL
//PB9     ------> I2C1_SDA 

/* Includes ------------------------------------------------------------------*/
#include "magnetometr.h"

//cholerne circular reference

typedef enum {
  HMC5883_OK =0,  // dane ok
  HMC5883_OVF,    // overflow
  HMC5883_ERR,    // blad I2C
  HMC5883_NOINIT  // brak inicjalizacji
}HMC5883_STATUS_t;



typedef struct {
  HMC5883_STATUS_t status;   // status sensora
  uint16_t winkel;           // kat w stopniach [0 bis 359]
}HMC5883_t;
HMC5883_t HMC5883;



I2C_HandleTypeDef MagnetoI2CHandle;
HMC5883_RAW_t HMC5883_RAW;


void I2C1_Init(void)
{

  MagnetoI2CHandle.Instance = I2C1;
  MagnetoI2CHandle.Init.ClockSpeed = 100000;
  MagnetoI2CHandle.Init.DutyCycle = I2C_DUTYCYCLE_2;
  MagnetoI2CHandle.Init.OwnAddress1 = 0;
  MagnetoI2CHandle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  MagnetoI2CHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  MagnetoI2CHandle.Init.OwnAddress2 = 0;
  MagnetoI2CHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  MagnetoI2CHandle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&MagnetoI2CHandle) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{
	
			__GPIOB_CLK_ENABLE();
	    __HAL_RCC_I2C1_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct;
  if(i2cHandle->Instance==I2C1)
  {

    GPIO_InitStruct.Pin = (GPIO_PIN_6|GPIO_PIN_9);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
    __HAL_RCC_I2C1_CLK_DISABLE();
		__GPIOB_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_9);

  }
} 

uint8_t Magnetometr_Read(uint16_t addrSlave, uint8_t *data, int n_data)
{
		while(HAL_I2C_Master_Receive(&MagnetoI2CHandle, addrSlave, data, n_data , 1000) != HAL_OK)
			{}
				
				uint8_t powrot=1;
				return powrot;
}


uint8_t Magnetometr_WriteCommand(uint16_t addrSlave, uint8_t reg)
{
		uint8_t *data;
		data=&reg;
		while( HAL_I2C_Master_Transmit(&MagnetoI2CHandle, addrSlave, data, 1 , 1000) != HAL_OK)
		{}
			
				uint8_t powrot=1;
				return powrot;
}

uint8_t Magnetometr_WriteData(uint16_t addrSlave, uint8_t *data, int n_data)
{
		 while(HAL_I2C_Master_Transmit(&MagnetoI2CHandle, addrSlave, data, n_data , 1000) != HAL_OK)
		 {}
			 
			 	uint8_t powrot=1;
				return powrot;
}

//inicjalizacja kompasu
uint8_t HMC5883_Init(void)
{
  uint8_t powrot=0;
  uint8_t ok=0;
	uint8_t sukces=0;
	uint8_t data[2];

  HMC5883_RAW.x_raw=0;
  HMC5883_RAW.y_raw=0;
  HMC5883_RAW.z_raw=0;
  HMC5883_RAW.cal_mode=0;
  HMC5883_RAW.x_min=0;
  HMC5883_RAW.x_max=0;
  HMC5883_RAW.y_min=0;
  HMC5883_RAW.y_max=0;

  HMC5883.status=HMC5883_NOINIT;
  HMC5883.winkel=0;


	HAL_Delay(HMC5883_POWERON_DELAY);
	
		sukces = Magnetometr_WriteCommand(HMC5883_WRITE_ADDR, HMC5883_REG_IDA );
		if (sukces != 0 )
		{
			sukces = Magnetometr_Read(HMC5883_READ_ADDR, data, 1 );
		}
		
		if(sukces!=0)
		{	
			if(data[0]==HMC5883_IDA_VALUE) ok++;
		}
		
				sukces = Magnetometr_WriteCommand(HMC5883_WRITE_ADDR, HMC5883_REG_IDB );
		if (sukces != 0 )
		{
			sukces = Magnetometr_Read(HMC5883_READ_ADDR, data, 1 );
		}
		
		if(sukces!=0)
		{	
			if(data[0]==HMC5883_IDB_VALUE) ok++;
		}
		
				sukces = Magnetometr_WriteCommand(HMC5883_WRITE_ADDR, HMC5883_REG_IDC );
		if (sukces != 0 )
		{
			sukces = Magnetometr_Read(HMC5883_READ_ADDR, data, 1 );
		}
		
		if(sukces!=0)
		{	
			if(data[0]==HMC5883_IDC_VALUE) ok++;
		}

  if(ok==3) 
		{
    powrot=1;
  }
  else {
    HMC5883.status=HMC5883_ERR;
  }

  return powrot;
}

//odczyt z kompasu, w winkel znajduje sie kat z przedzialu od 0 do 359
uint8_t HMC5883_Read(void)
{
  uint8_t powrot = 0;
	uint8_t sukces=0;
	uint8_t data[6];
	
	data[0]=HMC5883_REG_MODE;
	data[1]=0x01;


  HAL_Delay(HMC5883_READ_DELAY);

  // tryb Single-Measurement-Mode
	sukces = Magnetometr_WriteData(HMC5883_WRITE_ADDR, data, 2 );

  if(sukces!=0) 
	{			
		//czekaj na koniec pomiaru, 6 ms (pdf) +1 ms na zapas
		HAL_Delay(7);		
		sukces = Magnetometr_WriteCommand(HMC5883_WRITE_ADDR, HMC5883_REG_DATA );
		if (sukces != 0 )
		{
			sukces = Magnetometr_Read(HMC5883_READ_ADDR, data, 6 );
		}
		
		if(sukces != 0)
		{
		
    HMC5883_RAW.x_raw=(((uint16_t)data[0]<<8) + data[1]);
		HMC5883_RAW.z_raw=(((uint16_t)data[2]<<8) + data[3]);
    HMC5883_RAW.y_raw=(((uint16_t)data[4]<<8) + data[5]);

    p_HMC5883_calcData();

		powrot=1;
		}
  }
  else 
	{
    HMC5883.status=HMC5883_ERR;
    HMC5883.winkel=0;
  }

  return powrot;
}


//ustaw skale
uint8_t HMC5883_SetScale(uint16_t value)
{
  uint8_t powrot=0;
  uint8_t data[2];
	uint8_t sukces=0;
	
	data[0] = HMC5883_REG_CONFB;

	
		data[1] = value;


		sukces = Magnetometr_WriteData(HMC5883_WRITE_ADDR, data, 2 );

  if(sukces !=0) powrot=1;

  return powrot;
}


//--------------------------------------------------------------
// startet Kalibration :
//
// Sensor in alle Richungen bewegen,
// damit min und max Werte gespeichert werden
// dann Kalibration beenden
//--------------------------------------------------------------
void HMC5883_Start_Calibration(void)
{
  if(HMC5883_RAW.cal_mode==0) 
	{
    HMC5883_RAW.cal_mode=1;
    HMC5883_RAW.x_min=3000;
    HMC5883_RAW.x_max=-3000;
    HMC5883_RAW.y_min=3000;
    HMC5883_RAW.y_max=-3000;
  }
}

void HMC5883_Stop_Calibration(void)
{
  HMC5883_RAW.cal_mode=0;
}


void p_HMC5883_calcData(void)
{
  int16_t int_val;
  int16_t x_offset, y_offset;

  HMC5883.status=HMC5883_OK;
  // check auf OVF
  if(HMC5883_RAW.x_raw<(-2048))  HMC5883.status=HMC5883_OVF;
  if(HMC5883_RAW.y_raw<(-2048))  HMC5883.status=HMC5883_OVF;
  if(HMC5883_RAW.z_raw<(-2048))  HMC5883.status=HMC5883_OVF;

  // check ob Kalibration läuft
  if((HMC5883_RAW.cal_mode==1) && (HMC5883.status==HMC5883_OK)) {
    if(HMC5883_RAW.x_raw<HMC5883_RAW.x_min) HMC5883_RAW.x_min=HMC5883_RAW.x_raw;
    if(HMC5883_RAW.x_raw>HMC5883_RAW.x_max) HMC5883_RAW.x_max=HMC5883_RAW.x_raw;
    if(HMC5883_RAW.y_raw<HMC5883_RAW.y_min) HMC5883_RAW.y_min=HMC5883_RAW.y_raw;
    if(HMC5883_RAW.y_raw>HMC5883_RAW.y_max) HMC5883_RAW.y_max=HMC5883_RAW.y_raw;
  }

  // umrechnung für den Winkel
  if(HMC5883.status==HMC5883_OK) {
    x_offset=HMC5883_RAW.x_min+HMC5883_RAW.x_max;
    y_offset=HMC5883_RAW.y_min+HMC5883_RAW.y_max;
    int_val=atan2((double)HMC5883_RAW.y_raw - y_offset,(double)HMC5883_RAW.x_raw - x_offset)* (180 / M_PI) +180;
    if(int_val<0) int_val=0;
    if(int_val>359) int_val=359;
    HMC5883.winkel=int_val;
		
		kat_od_polnocy = HMC5883.winkel;
  }

}

