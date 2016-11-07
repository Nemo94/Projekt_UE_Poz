// COPYRIGHT 2016 MICHAL POTEMSKI  
#include "stepper_driver.h"


		

void StepperDriverInit(uint8_t stepper_id)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_TypeDef* STEPPERX_DIR_PORT;
	uint16_t STEPPERX_DIR_PIN;
	GPIO_TypeDef* STEPPERX_STEP_PORT;
	uint16_t STEPPERX_STEP_PIN;
	GPIO_TypeDef* STEPPERX_EN_PORT;
	uint16_t STEPPERX_EN_PIN;
	
	switch(stepper_id)
	{
		case 1:
		STEPPERX_DIR_PORT = STEPPER1_DIR_PORT;
		STEPPERX_DIR_PIN = STEPPER1_DIR_PIN;
		STEPPERX_STEP_PORT = STEPPER1_STEP_PORT;
		STEPPERX_STEP_PIN = STEPPER1_STEP_PIN;
		STEPPERX_EN_PORT = STEPPER1_EN_PORT;
		STEPPERX_EN_PIN = STEPPER1_EN_PIN;
		break;
		
		case 2:
		STEPPERX_DIR_PORT = STEPPER2_DIR_PORT;
		STEPPERX_DIR_PIN = STEPPER2_DIR_PIN;
		STEPPERX_STEP_PORT = STEPPER2_STEP_PORT;
		STEPPERX_STEP_PIN = STEPPER2_STEP_PIN;
		STEPPERX_EN_PORT = STEPPER2_EN_PORT;
		STEPPERX_EN_PIN = STEPPER2_EN_PIN;
		break;
		
		
		default:
		return;
		//break;
	}
	
		MOTOR_CLOCK_PINS_MACRO();
	

	GPIO_InitStruct.Pin = STEPPERX_EN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(STEPPERX_EN_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(STEPPERX_EN_PORT, STEPPERX_EN_PIN, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = STEPPERX_DIR_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(STEPPERX_DIR_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(STEPPERX_DIR_PORT, STEPPERX_DIR_PIN, GPIO_PIN_RESET);
	
	GPIO_InitStruct.Pin = STEPPERX_STEP_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(STEPPERX_STEP_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(STEPPERX_STEP_PORT, STEPPERX_STEP_PIN, GPIO_PIN_RESET);
	
	
	
}

void AllSteppersInit(uint8_t number_of_steppers)
{
	for(uint8_t i=1; i<=number_of_steppers; i++)
	{
		StepperDriverInit(i);
	}
}

void StepperXEnable(uint8_t stepper_id)
{
	GPIO_TypeDef* STEPPERX_EN_PORT;
	uint16_t STEPPERX_EN_PIN;
	
	switch(stepper_id)
	{
		case 1:
		STEPPERX_EN_PORT = STEPPER1_EN_PORT;
		STEPPERX_EN_PIN = STEPPER1_EN_PIN;
		break;
		
		case 2:
		STEPPERX_EN_PORT = STEPPER2_EN_PORT;
		STEPPERX_EN_PIN = STEPPER2_EN_PIN;
		break;
				
		default:
		return;
		//break;
	}
	
	HAL_GPIO_WritePin(STEPPERX_EN_PORT, STEPPERX_EN_PIN, GPIO_PIN_RESET);
	HAL_Delay(1);
}

void StepperXDisable(uint8_t stepper_id)
{
	GPIO_TypeDef* STEPPERX_EN_PORT;
	uint16_t STEPPERX_EN_PIN;
	
	switch(stepper_id)
	{
		case 1:
		STEPPERX_EN_PORT = STEPPER1_EN_PORT;
		STEPPERX_EN_PIN = STEPPER1_EN_PIN;
		break;
		
		case 2:
		STEPPERX_EN_PORT = STEPPER2_EN_PORT;
		STEPPERX_EN_PIN = STEPPER2_EN_PIN;
		break;
	
		
		default:
		return;
		//break;
	}
	
	HAL_GPIO_WritePin(STEPPERX_EN_PORT, STEPPERX_EN_PIN, GPIO_PIN_SET);
	HAL_Delay(1);
}


void StepperXExecute(uint8_t stepper_id, uint16_t steps, uint8_t direction)
{
	GPIO_TypeDef* STEPPERX_DIR_PORT;
	uint16_t STEPPERX_DIR_PIN;
	GPIO_TypeDef* STEPPERX_STEP_PORT;
	uint16_t STEPPERX_STEP_PIN;
	
	switch(stepper_id)
	{
		case 1:
		STEPPERX_DIR_PORT = STEPPER1_DIR_PORT;
		STEPPERX_DIR_PIN = STEPPER1_DIR_PIN;
		STEPPERX_STEP_PORT = STEPPER1_STEP_PORT;
		STEPPERX_STEP_PIN = STEPPER1_STEP_PIN;
		break;
		
		case 2:
		STEPPERX_DIR_PORT = STEPPER2_DIR_PORT;
		STEPPERX_DIR_PIN = STEPPER2_DIR_PIN;
		STEPPERX_STEP_PORT = STEPPER2_STEP_PORT;
		STEPPERX_STEP_PIN = STEPPER2_STEP_PIN;
		break;
		
		default:
		return;
	 //break;
	}
	
		if(steps !=0)
		{
			if(direction ==0)
			{
				HAL_GPIO_WritePin(STEPPERX_DIR_PORT, STEPPERX_DIR_PIN, GPIO_PIN_RESET);
				HAL_Delay(1);

					for(uint16_t i=1; i<=steps; i++)
					{
						HAL_GPIO_WritePin(STEPPERX_STEP_PORT, STEPPERX_STEP_PIN, GPIO_PIN_SET);
						HAL_Delay(5);
						HAL_GPIO_WritePin(STEPPERX_STEP_PORT, STEPPERX_STEP_PIN, GPIO_PIN_RESET);
						HAL_Delay(5);
					}
			}
			else
			{
				HAL_GPIO_WritePin(STEPPERX_DIR_PORT, STEPPERX_DIR_PIN, GPIO_PIN_SET);
				HAL_Delay(1);

					for(uint16_t i=1; i<=steps; i++)
					{
						HAL_GPIO_WritePin(STEPPERX_STEP_PORT, STEPPERX_STEP_PIN, GPIO_PIN_SET);
						HAL_Delay(5);
						HAL_GPIO_WritePin(STEPPERX_STEP_PORT, STEPPERX_STEP_PIN, GPIO_PIN_RESET);
						HAL_Delay(5);
					}
			}
		}
		
	
	
}

