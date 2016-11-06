/**
  * COPYRIGHT(c) 2016 STMicroelectronics
  
  */

/* Includes ------------------------------------------------------------------*/
#include "mems.h"

//PODLACZENIE AKCELEROMETRU:
//PD0 - INT1
//PD1 - INT2
//PD3 - CS
//PC2 - MISO
//PB15 - MOSI
//PB13 - SCK

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Init af threshold to detect acceleration on MEMS */
/* Typical value: 
      - No  acceleration: X, Y inferior to 100 (positive or negative)
      - Max acceleration: X, Y around 2000 (positive or negative) */
int16_t ThresholdHigh = 200;
int16_t ThresholdLow = -200;

uint32_t Spi2Timeout = SPI2_TIMEOUT_MAX;    /*<! Value of Timeout when SPI communication fails */

static SPI_HandleTypeDef 	AkceleroSpiHandle;

/* Private function prototypes -----------------------------------------------*/

static void     SPI2_Init(void);
static void     SPI2_MspInit(void);
static uint8_t  SPI2_WriteRead(uint8_t Byte);
static  void    SPI2_Error(void);

/* Link functions for Accelerometer peripheral */
void            ACCELERO_IO_Init(void);
void            ACCELERO_IO_ITConfig(void);
void            ACCELERO_IO_Write(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void            ACCELERO_IO_Read(uint8_t *pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);

/* Private functions ---------------------------------------------------------*/



static void SPI2_Init(void)
{
  if(HAL_SPI_GetState(&AkceleroSpiHandle) == HAL_SPI_STATE_RESET)
  {
    AkceleroSpiHandle.Instance = SPI2;
    AkceleroSpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    AkceleroSpiHandle.Init.Direction = SPI_DIRECTION_2LINES;
    AkceleroSpiHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
    AkceleroSpiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
    AkceleroSpiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
    AkceleroSpiHandle.Init.CRCPolynomial = 7;
    AkceleroSpiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
    AkceleroSpiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    AkceleroSpiHandle.Init.NSS = SPI_NSS_SOFT;
    AkceleroSpiHandle.Init.TIMode = SPI_TIMODE_DISABLED;
    AkceleroSpiHandle.Init.Mode = SPI_MODE_MASTER;

    SPI2_MspInit();
    HAL_SPI_Init(&AkceleroSpiHandle);
  }
}

/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received 
  *         from the SPI bus.
  * @param  Byte: Byte send.
  * @retval The received byte value
  */
static uint8_t SPI2_WriteRead(uint8_t Byte)
{
  uint8_t receivedbyte = 0;
  
  /* Send a Byte through the SPI peripheral */
  /* Read byte from the SPI bus */
  if(HAL_SPI_TransmitReceive(&AkceleroSpiHandle, (uint8_t*) &Byte, (uint8_t*) &receivedbyte, 1, Spi2Timeout) != HAL_OK)
  {
    SPI2_Error();
  }
  
  return receivedbyte;
}

/**
  * @brief  SPIx error treatment function.
  */
static void SPI2_Error(void)
{
  /* De-initialize the SPI communication bus */
  HAL_SPI_DeInit(&AkceleroSpiHandle);
  
  /* Re-Initialize the SPI communication bus */
  SPI2_Init();
}

/**
  * @brief  SPI MSP Init.
  */
static void SPI2_MspInit(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

		__SPI2_CLK_ENABLE();
		__GPIOC_CLK_ENABLE();
		__GPIOB_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = (GPIO_PIN_13|GPIO_PIN_15);
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull  = GPIO_PULLDOWN;
  GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GPIO_PIN_2;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStructure.Pull  = GPIO_PULLDOWN;
  GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
  GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}




/**
  * @brief  Configures the Accelerometer SPI interface.
  */
void ACCELERO_IO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Configure the Accelerometer Control pins --------------------------------*/
  /* Enable CS GPIO clock and configure GPIO pin for Accelerometer Chip select */  
  AKCELERO_CS_GPIO_CLK_ENABLE();
  
  /* Configure GPIO PIN for LIS Chip select */
  GPIO_InitStructure.Pin = AKCELERO_CS_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull  = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
  HAL_GPIO_Init(AKCELERO_CS_GPIO_PORT, &GPIO_InitStructure);
  
  /* Deselect: Chip Select high */
  AKCELERO_CS_HIGH();
  
  SPI2_Init();
}

/**
  * @brief  Configures the Accelerometer INT2.
  *         EXTI0 is already used by user button so INT1 is not configured here.
  */
void ACCELERO_IO_ITConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Enable INT2 GPIO clock and configure GPIO PINs to detect Interrupts */
  AKCELERO_INT_GPIO_CLK_ENABLE();
  
  /* Configure GPIO PINs to detect Interrupts */
  GPIO_InitStructure.Pin = AKCELERO_INT2_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(AKCELERO_INT_GPIO_PORT, &GPIO_InitStructure);
  
  /* Enable and set Accelerometer INT2 to the lowest priority */
  HAL_NVIC_SetPriority((IRQn_Type)AKCELERO_INT2_EXTI_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ((IRQn_Type)AKCELERO_INT2_EXTI_IRQn);
}

/**
  * @brief  Writes one byte to the Accelerometer.
  * @param  pBuffer: pointer to the buffer containing the data to be written to the Accelerometer.
  * @param  WriteAddr: Accelerometer's internal address to write to.
  * @param  NumByteToWrite: Number of bytes to write.
  */
void ACCELERO_IO_Write(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  /* Configure the MS bit: 
     - When 0, the address will remain unchanged in multiple read/write commands.
     - When 1, the address will be auto incremented in multiple read/write commands.
  */
  if(NumByteToWrite > 0x01)
  {
    WriteAddr |= (uint8_t)MULTIPLEBYTE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  AKCELERO_CS_LOW();
  
  /* Send the Address of the indexed register */
  SPI2_WriteRead(WriteAddr);
  
  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
    SPI2_WriteRead(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  AKCELERO_CS_HIGH();
}

/**
  * @brief  Reads a block of data from the Accelerometer.
  * @param  pBuffer: pointer to the buffer that receives the data read from the Accelerometer.
  * @param  ReadAddr: Accelerometer's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the Accelerometer.
  */
void ACCELERO_IO_Read(uint8_t *pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
  if(NumByteToRead > 0x01)
  {
    ReadAddr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
  }
  else
  {
    ReadAddr |= (uint8_t)READWRITE_CMD;
  }
  /* Set chip select Low at the start of the transmission */
  AKCELERO_CS_LOW();
  
  /* Send the Address of the indexed register */
  SPI2_WriteRead(ReadAddr);
  
  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to ACCELEROMETER (Slave device) */
    *pBuffer = SPI2_WriteRead(DUMMY_BYTE);
    NumByteToRead--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  AKCELERO_CS_HIGH();
}

void ReadAccelero(void)
{

  int16_t buffer[3] = {0};
  int16_t xval, yval, zval = 0x00;
  
  /* pobieramy dane i zapisujemy w buforach X, Y, Z */
  BSP_ACCELERO_GetXYZ(buffer);
  
  xval = buffer[0];
  yval = buffer[1];
	zval = buffer[2];
  
	akcelero_x = xval;
	akcelero_y = yval;
	akcelero_z = zval;
	
	//zapalenie diod w zaleznosci od przyspieszenia osi - zrobione w przykladzie producenta
  if((ABS(xval))>(ABS(yval)))
  {
    if(xval > ThresholdHigh)
    { 
      /* LED5 On */
      BSP_LED_On(LED5);
      HAL_Delay(10);
    }
    else if(xval < ThresholdLow)
    { 
      /* LED4 On */
      BSP_LED_On(LED4);      
      HAL_Delay(10);
    }
    else
    { 
      HAL_Delay(10);
    }
  }
  else
  {
    if(yval < ThresholdLow)
    {
      /* LED6 On */
      BSP_LED_On(LED6);
      HAL_Delay(10);
    }
    else if(yval > ThresholdHigh)
    {
      /* LED3 On */
      BSP_LED_On(LED3);
      HAL_Delay(10);
    } 
    else
    { 
      HAL_Delay(10);
    }
  } 
	
	//
  
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED6);
}

/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
