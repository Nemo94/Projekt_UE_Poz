// COPYRIGHT 2016 MICHAL POTEMSKI  



// PB11     ------> USART3_RX
  //  PD8     ------> USART3_TX 

#include "main.h"
#include "uart_config.h"
		
UART_HandleTypeDef Usart3Handle;

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

void send_char(char c)
{
	uint8_t aTxBuffer=(uint8_t)c;
	HAL_UART_Transmit(&Usart3Handle, (uint8_t*)&aTxBuffer, 1, 5000); 
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
		__HAL_UART_FLUSH_DRREGISTER(&Usart3Handle);
}
