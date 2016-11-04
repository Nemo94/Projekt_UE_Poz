//PB6     ------> I2C1_SCL
//PB9     ------> I2C1_SDA 

/* Includes ------------------------------------------------------------------*/
#include "magnetometr.h"

I2C_HandleTypeDef MagnetoI2CHandle;

/* I2C1 init function */
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

void Magnetometr_Read(uint16_t addrSlave, uint8_t *data, int n_data)
{
		while(HAL_I2C_Master_Receive(&MagnetoI2CHandle, addrSlave, data, n_data , 1000) != HAL_OK)
			{}
}


void Magnetometr_WriteCommand(uint16_t addrSlave, uint8_t reg)
{
		uint8_t *data;
		data=&reg;
		while( HAL_I2C_Master_Transmit(&MagnetoI2CHandle, addrSlave, data, 1 , 1000) != HAL_OK)
		{}
}

void Magnetometr_WriteData(uint16_t addrSlave, uint8_t *data, int n_data)
{
		 while(HAL_I2C_Master_Transmit(&MagnetoI2CHandle, addrSlave, data, n_data , 1000) != HAL_OK)
		 {}
}
