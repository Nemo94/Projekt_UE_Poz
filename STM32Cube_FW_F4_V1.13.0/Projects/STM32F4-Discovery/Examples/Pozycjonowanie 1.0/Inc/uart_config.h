/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_CONFIG_H
#define __UART_CONFIG_H



#include "main.h"



void USART3_Init(void);
void send_char(char c);
UART_HandleTypeDef Usart3Handle;


#endif
