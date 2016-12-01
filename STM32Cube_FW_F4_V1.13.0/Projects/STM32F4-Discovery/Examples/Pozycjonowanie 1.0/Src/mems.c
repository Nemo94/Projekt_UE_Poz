//COPYRIGHT 2016 MICHAL POTEMSKI

/* Includes ------------------------------------------------------------------*/
#include "mems.h"

//PODLACZENIE AKCELEROMETRU:

//PD3 - CS
//PC2 - MISO
//PB15 - MOSI
//PB13 - SCK

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


int16_t ThresholdHigh = 200;
int16_t ThresholdLow = -200;

uint32_t Spi2Timeout = SPI2_TIMEOUT_MAX;

static SPI_HandleTypeDef 	AkceleroSpiHandle;

/* Private function prototypes -----------------------------------------------*/

static void     SPI2_Init(void);
static void     SPI2_MspInit(void);
static uint8_t  SPI2_WriteRead(uint8_t Byte);
static  void    SPI2_Error(void);

void            LIS3DH_SPI_Write(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void            LIS3DH_SPI_Read(uint8_t *pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);

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

static uint8_t SPI2_WriteRead(uint8_t Byte)
{
  uint8_t receivedbyte = 0;
  if(HAL_SPI_TransmitReceive(&AkceleroSpiHandle, (uint8_t*) &Byte, (uint8_t*) &receivedbyte, 1, Spi2Timeout) != HAL_OK)
  {
    SPI2_Error();
  }
  
  return receivedbyte;
}

static void SPI2_Error(void)
{
  HAL_SPI_DeInit(&AkceleroSpiHandle);
	SPI2_Init();
}

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

void LIS3DH_Line_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  AKCELERO_CS_GPIO_CLK_ENABLE();
  GPIO_InitStructure.Pin = AKCELERO_CS_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull  = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
  HAL_GPIO_Init(AKCELERO_CS_GPIO_PORT, &GPIO_InitStructure);
 
  AKCELERO_CS_HIGH();
  
  SPI2_Init();
}

void LIS3DH_Config(void)
{
	LIS3DH_SetODR(LIS3DH_ODR_100Hz);
	LIS3DH_SetMode(LIS3DH_NORMAL);
	LIS3DH_SetFullScale(LIS3DH_FULLSCALE_2);
	LIS3DH_SetAxis(LIS3DH_X_ENABLE | LIS3DH_Y_ENABLE | LIS3DH_Z_ENABLE);
}

void LIS3DH_SPI_Write(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  /* Configure the MS bit: 
     - When 0, the address will remain unchanged in multiple read/write commands.
     - When 1, the address will be auto incremented in multiple read/write commands.
  */
  if(NumByteToWrite > 0x01)
  {
    WriteAddr |= (uint8_t)MULTIPLEBYTE_CMD;
  }
  AKCELERO_CS_LOW();
  
  SPI2_WriteRead(WriteAddr);
  
  /* Send the data (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
    SPI2_WriteRead(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }
  
  AKCELERO_CS_HIGH();
}

void LIS3DH_SPI_Read(uint8_t *pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  
  if(NumByteToRead > 0x01)
  {
    ReadAddr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
  }
  else
  {
    ReadAddr |= (uint8_t)READWRITE_CMD;
  }
  AKCELERO_CS_LOW();
  
  SPI2_WriteRead(ReadAddr);
  
  /* Receive the data (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock (Slave device) */
    *pBuffer = SPI2_WriteRead(DUMMY_BYTE);
    NumByteToRead--;
    pBuffer++;
  }
    AKCELERO_CS_HIGH();
}

void ReadAccelero(void)
{
	AxesRaw_t akcelero_data;
  
	LIS3DH_Config();
  LIS3DH_GetAccAxesRaw(&akcelero_data);
  
  akcelero_x = akcelero_data.AXIS_X;
  akcelero_y = akcelero_data.AXIS_Y;
	akcelero_z = akcelero_data.AXIS_Z;
	
}

