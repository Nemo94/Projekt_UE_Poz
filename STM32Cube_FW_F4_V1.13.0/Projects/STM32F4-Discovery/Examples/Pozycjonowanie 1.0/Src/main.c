/**
  *COPYRIGHT(c) 2016 STMicroelectronics
	Modified 2016 Michal Potemski
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


//zmienne z aktualnymi danymi akcelerometru, do przetwarzania!
uint16_t akcelero_x=0;
uint16_t akcelero_y=0;
uint16_t akcelero_z=0;


//aktualny kat w stopniach od polnocy 0-359, do przetwarzania dalej!
uint16_t kat_od_polnocy=0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{ 
 /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /* Configure LED3, LED4, LED5 and LED6 */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4); 
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);
  
  /* Configure the system clock to 168 MHz */
  SystemClock_Config();
  
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);
  
	
	LIS3DH_Line_Init();
	LIS3DH_Config();
		
	I2C1_Init();
	
	uint8_t magnetometer_is_working = 0;
	
	magnetometer_is_working = HMC5883_Init();
	
	if(magnetometer_is_working)
	{
		//wartosci realne do ustawienia w typedefie w main.h
		SetMagnetometerScaleValue(HMC5883_8G1);
	}
	
	USART3_Init();
	TIM2_Init();
	
  BSP_LED_Off(LED3);
  BSP_LED_Off(LED4);
  BSP_LED_Off(LED5);
  BSP_LED_Off(LED6);
  
	AllSteppersInit(NUMBER_OF_MOTORS);
	
  while(1)
  {

			ReadAccelero();
			HMC5883_Read();
		//tu znajduja sie aktualizowane w powyzszych funkcjach dane z akcelerometru i kompasu
		//akcelero_x
		//akcelero_y
		//akcelero_z
		//kat_od_polnocy
		
		printf("akcelero_x=%u\n", akcelero_x);
		printf("akcelero_y=%u\n", akcelero_y);
		printf("akcelero_z=%u\n", akcelero_z);
		printf("kat_od_polnocy=%u\n", kat_od_polnocy);
		HAL_Delay(5000);
		
		//wlacz uklady sterujace silnikami
		StepperXEnable(MOTOR1);
		StepperXEnable(MOTOR2);
		
		//wykonuja odpowiednia ilosc krokow w pierwszym kierunku
		StepperXExecute(MOTOR1, 10, 0);
		StepperXExecute(MOTOR2, 20, 0);
		
		//wykonuja taka sama liczbe krokow w odwrotnym kierunku, powracajac do poczatkowego polozenia
		StepperXExecute(MOTOR1, 10, 1);
		StepperXExecute(MOTOR2, 20, 1);
		
		//wylacz uklady sterujace silnikami
		StepperXDisable(MOTOR1);
		StepperXDisable(MOTOR2);
		
		
		//wartosci dla serw moga byc przyjmowane w katach od 0.0 do 168.0 co 0.5 
		ServoSetAngle(SERVO1, 20.5);
		ServoSetAngle(SERVO2, 40.0);
		ServoSetAngle(SERVO3, 60.5);
		
		ServoSetAngle(SERVO1, 0.0);
		ServoSetAngle(SERVO2, 0.0);
		ServoSetAngle(SERVO3, 0.0);
		
		HAL_Delay(5000);

  }
}

int __io_putchar(int c)
{
    send_char(c);
    return c;
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (KEY_BUTTON_PIN == GPIO_Pin)
  {
    //while (BSP_PB_GetState(BUTTON_KEY) != RESET);
  }
  
}


void SetMagnetometerScaleValue(HMC5883_SCALE_t scale)
{	
	uint16_t value;
  if(scale==HMC5883_0G8) value=0x00;
	else if(scale==HMC5883_1G3) value=0x20;
  else if(scale==HMC5883_1G9) value=0x40;
  else if(scale==HMC5883_2G5) value=0x60;
  else if(scale==HMC5883_4G0) value=0x80;
  else if(scale==HMC5883_4G7) value=0xA0;
  else if(scale==HMC5883_5G6) value=0xC0;
  else if(scale==HMC5883_8G1) value=0xE0;
	HMC5883_SetScale(value);
}

/**
  * @brief  Toggle LEDs
  * @param  None
  * @retval None
  */
void Toggle_Leds(void)
{
  BSP_LED_Toggle(LED3);
  HAL_Delay(100);
  BSP_LED_Toggle(LED4);
  HAL_Delay(100);
  BSP_LED_Toggle(LED5);
  HAL_Delay(100);
  BSP_LED_Toggle(LED6);
  HAL_Delay(100);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED5 on */
  BSP_LED_On(LED5);

	HAL_Delay(5000);
  NVIC_SystemReset();
}




#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif



