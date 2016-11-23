// COPYRIGHT 2016 MICHAL POTEMSKI  



// PB11     ------> USART3_RX
//  PD8     ------> USART3_TX 

#include "main.h"
#include "uart_config.h"
		
		
UART_HandleTypeDef Usart3Handle;
uint8_t RxBuffer[2];
uint8_t RxData;
volatile uint8_t bufor=0;
uint8_t command=0;
__IO ITStatus UartReady = RESET;
uint8_t aTxBuffer[35];

void USART3_Init(void)
{

  Usart3Handle.Instance = USART3;
  Usart3Handle.Init.BaudRate = 9600;
  Usart3Handle.Init.WordLength = UART_WORDLENGTH_8B;
  Usart3Handle.Init.StopBits = UART_STOPBITS_1;
  Usart3Handle.Init.Parity = UART_PARITY_NONE;
  Usart3Handle.Init.Mode = UART_MODE_TX_RX;
  Usart3Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  Usart3Handle.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&Usart3Handle) != HAL_OK)
  {
    Error_Handler();
  }
	
	HAL_UART_Receive_IT(&Usart3Handle, (uint8_t*)&RxData, 1); 
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  
	if(uartHandle->Instance==USART3)
  {
    __HAL_RCC_USART3_CLK_ENABLE();
		__GPIOB_CLK_ENABLE();
		__GPIOD_CLK_ENABLE();


    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

	if(uartHandle->Instance==USART3)
  {
    __HAL_RCC_USART3_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8);

    HAL_NVIC_DisableIRQ(USART3_IRQn);
  }
} 

void send_string(char* string)
{
	for(uint8_t i=0; i<30; i++)
	{
		uint8_t offset=0;
		
		if((uint8_t)string[i]=='\n') 
		{
				aTxBuffer[i+offset]='\r';
				offset++;
				aTxBuffer[i+offset]='\n';
				i++;
		}
	aTxBuffer[i+offset]=(uint8_t)string[i];
	}
	HAL_UART_Transmit_IT(&Usart3Handle, (uint8_t*)&aTxBuffer, 35);
	while (UartReady != SET)
  {
		;
  }
  UartReady = RESET;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	UartReady = SET;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{

		if(RxData!='\r' && RxData!='\n')
		{
			RxBuffer[bufor] = RxData;
			bufor++;
		}
			for(uint8_t i=0; i<bufor; i++)
			{
				command=RxBuffer[0]*10+RxBuffer[1];
			}
	if(bufor>2) 
	{
		bufor=0;
		RxBuffer[1]=0;
		RxBuffer[0]=0;
	}	
		HAL_UART_Receive_IT(&Usart3Handle, (uint8_t*)&RxData, 1); 
		__HAL_UART_FLUSH_DRREGISTER(&Usart3Handle);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
	bufor=0;
	RxBuffer[1]=0;
	RxBuffer[0]=0;
	HAL_UART_Receive_IT(&Usart3Handle, (uint8_t*) &RxData, 1);
}

