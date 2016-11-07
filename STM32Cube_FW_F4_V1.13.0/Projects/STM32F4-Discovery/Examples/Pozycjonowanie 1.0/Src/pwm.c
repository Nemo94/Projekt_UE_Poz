// COPYRIGHT 2016 MICHAL POTEMSKI  

#include "pwm.h"

  //  PA1     ------> TIM2_CH2
  //  PA2     ------> TIM2_CH3
 //   PA3     ------> TIM2_CH4 

TIM_HandleTypeDef PWMHandle;

void TIM2_Init(void)
{
	
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
	GPIO_InitTypeDef GPIO_InitStruct;

  PWMHandle.Instance = TIM2;
  PWMHandle.Init.Prescaler = 250 - 1;
  PWMHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  PWMHandle.Init.Period = 6720;
  PWMHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&PWMHandle) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&PWMHandle, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }


	__GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = (GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 336;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;


  if (HAL_TIM_PWM_ConfigChannel(&PWMHandle, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_TIM_PWM_ConfigChannel(&PWMHandle, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_TIM_PWM_ConfigChannel(&PWMHandle, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
	
	HAL_TIM_PWM_Start(&PWMHandle, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&PWMHandle, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&PWMHandle, TIM_CHANNEL_4);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* tim_pwmHandle)
{

  if(tim_pwmHandle->Instance==TIM2)
  {
    __HAL_RCC_TIM2_CLK_ENABLE();

    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  }
}


void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* tim_pwmHandle)
{

  if(tim_pwmHandle->Instance==TIM2)
  {
    __HAL_RCC_TIM2_CLK_DISABLE();
    HAL_NVIC_DisableIRQ(TIM2_IRQn);

  }
} 

void ServoSetAngle(uint8_t servo_id, float angle)
{
	if(((int32_t)angle <0 )|| ((int32_t)angle>=180))
	{
		return;
	}
	else if((uint32_t)angle>=168)
	{
		angle = 168;
	}
	
	TIM_OC_InitTypeDef ConfigStruct;
	ConfigStruct.OCMode = TIM_OCMODE_PWM1;
	ConfigStruct.Pulse = (uint32_t)(2*angle);
	ConfigStruct.OCPolarity = TIM_OCPOLARITY_HIGH;
  ConfigStruct.OCFastMode = TIM_OCFAST_DISABLE;
	
	switch(servo_id)
	{
		case 1:
			HAL_TIM_PWM_ConfigChannel(&PWMHandle, &ConfigStruct, TIM_CHANNEL_2);
			HAL_TIM_PWM_Start(&PWMHandle, TIM_CHANNEL_2);
			break;
		
		case 2:
			HAL_TIM_PWM_ConfigChannel(&PWMHandle, &ConfigStruct, TIM_CHANNEL_3);
			HAL_TIM_PWM_Start(&PWMHandle, TIM_CHANNEL_3);

			break;
		
		case 3:
			HAL_TIM_PWM_ConfigChannel(&PWMHandle, &ConfigStruct, TIM_CHANNEL_4);
			HAL_TIM_PWM_Start(&PWMHandle, TIM_CHANNEL_4);
			break;
		
		default:
			return;
	}
}
