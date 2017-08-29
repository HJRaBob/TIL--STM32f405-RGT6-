/*
 * This file is part of the 쨉OS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
 * Jog_up -> LED1 bright, Motor slow
 * JOG_dwon -> LED1 dark , Motor fast
 ------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "stm32f4xx_it.h"
#include "ledconfig.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

TIM_HandleTypeDef TimHandle;
TIM_OC_InitTypeDef TIM_OC_Init;
GPIO_InitTypeDef JOG;

int pulse = 4200-1;

void TIMER_Config(void)
{
	__HAL_RCC_TIM2_CLK_ENABLE();

	TimHandle.Instance			= TIM2;
	TimHandle.Init.Period		= 8400-1;
	TimHandle.Init.Prescaler	= 0;
	TimHandle.Init.ClockDivision= TIM_CLOCKDIVISION_DIV1;
	TimHandle.Init.CounterMode	= TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&TimHandle);
	HAL_TIM_Base_Start_IT(&TimHandle);

	TIM_OC_Init.OCMode			= TIM_OCMODE_TIMING;
	TIM_OC_Init.Pulse			= pulse;
	TIM_OC_Init.OCPolarity		= TIM_OCPOLARITY_LOW;
	TIM_OC_Init.OCFastMode		= TIM_OCFAST_DISABLE;
	HAL_TIM_OC_Init(&TimHandle);

	//channel 을 2개 둘 필요가 없음... 단위를 다르게 하면 모를까
	HAL_TIM_OC_ConfigChannel(&TimHandle,&TIM_OC_Init, TIM_CHANNEL_1);
	HAL_TIM_OC_Start_IT(&TimHandle,TIM_CHANNEL_1);

	HAL_NVIC_SetPriority(TIM2_IRQn,0,0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void JOG_Config(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	JOG.Pin		= GPIO_PIN_0|GPIO_PIN_1;
	JOG.Mode	= GPIO_MODE_IT_RISING;
	JOG.Pull	= GPIO_NOPULL;
	JOG.Speed	= GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOB,&JOG);

	HAL_NVIC_SetPriority(EXTI0_IRQn,1,0);
	HAL_NVIC_SetPriority(EXTI1_IRQn,2,0);

	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}

int main(int argc, char* argv[])
{
	TIMER_Config();
	JOG_Config();
	LED_Config(GPIO_PIN_2|GPIO_PIN_3);
	DCMotor_Config();
	DCMotor_Control(1);
	while (1)
    {

    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_0) {
		if (pulse > 8400 - 1)
			pulse = 7200 - 1;
		else
			pulse+=1000;
		TIM_OC_Init.Pulse = pulse;
		HAL_TIM_OC_ConfigChannel(&TimHandle, &TIM_OC_Init, TIM_CHANNEL_1);
	}

	if (GPIO_Pin == GPIO_PIN_1) {
		if (pulse < 0)
			pulse = 200-1;
		else
			pulse -=1000;
		TIM_OC_Init.Pulse = pulse;
		HAL_TIM_OC_ConfigChannel(&TimHandle, &TIM_OC_Init, TIM_CHANNEL_1);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,1);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,0);
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,0);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,1);
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
