// COPYRIGHT 2016 MICHAL POTEMSKI  
#ifndef __UART_CONFIG_H
#define __UART_CONFIG_H



#include "main.h"

#define ROZM_BUF 30

void USART3_Init(void);
void send_string(char* string);
UART_HandleTypeDef Usart3Handle;
uint8_t command;
uint8_t aTxBuffer[30];

#endif
