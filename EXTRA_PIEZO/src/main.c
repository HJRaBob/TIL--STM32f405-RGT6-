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
 * piezo makes melody by matching button
 *
 * switch 1 : C
 * switch 2 : D
 * switch 3 : E
 -----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal.h"
#include "ledconfig.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

TIM_HandleTypeDef TimHandle;
TIM_OC_InitTypeDef TIM_OC_Init;
GPIO_InitTypeDef PIEZO;
GPIO_InitTypeDef BUTTON;

int pulse = 3822;
unsigned char flag = 0;

void TIMER_Config(void)
{
	__HAL_RCC_TIM2_CLK_ENABLE();

	TimHandle.Instance			= TIM2;
	TimHandle.Init.Period		= pulse-1;
	TimHandle.Init.Prescaler	= 84;
	TimHandle.Init.ClockDivision= TIM_CLOCKDIVISION_DIV1;
	TimHandle.Init.CounterMode	= TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&TimHandle);
	HAL_TIM_Base_Start_IT(&TimHandle);

	TIM_OC_Init.OCMode			= TIM_OCMODE_TIMING;
	TIM_OC_Init.Pulse			= pulse/2-1;
	TIM_OC_Init.OCPolarity		= TIM_OCPOLARITY_LOW;
	TIM_OC_Init.OCFastMode		= TIM_OCFAST_DISABLE;
	HAL_TIM_OC_Init(&TimHandle);

	//channel 을 2개 둘 필요가 없음... 단위를 다르게 하면 모를까
	HAL_TIM_OC_ConfigChannel(&TimHandle,&TIM_OC_Init, TIM_CHANNEL_1);
	HAL_TIM_OC_Start_IT(&TimHandle,TIM_CHANNEL_1);

	HAL_NVIC_SetPriority(TIM2_IRQn,5,0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void Piezo_Config(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

	PIEZO.Mode	= GPIO_MODE_OUTPUT_PP;
	PIEZO.Pin	= GPIO_PIN_15;
	HAL_GPIO_Init(GPIOB,&PIEZO);
}

void Button_Config(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();

	BUTTON.Pin	= GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_12;
	BUTTON.Mode	= GPIO_MODE_IT_RISING_FALLING;
	BUTTON.Pull	= GPIO_NOPULL;
	BUTTON.Speed	= GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOA,&BUTTON);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn,1,0);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,2,0);

	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void doraeme(int high)
{
	switch (high) {
	case 0:
		pulse = 3822;
		TimHandle.Init.Period = pulse - 1;
		TIM_OC_Init.Pulse = pulse / 2 - 1;

		break;
	case 1:
		pulse = 3405;
		TimHandle.Init.Period = pulse - 1;
		TIM_OC_Init.Pulse = pulse / 2 - 1;

		break;
	case 2:
		pulse = 3033;
		TimHandle.Init.Period = pulse - 1;
		TIM_OC_Init.Pulse = pulse / 2 - 1;

		break;
	}

	HAL_TIM_Base_Init(&TimHandle);
	HAL_TIM_OC_ConfigChannel(&TimHandle, &TIM_OC_Init, TIM_CHANNEL_1);

}

int main(int argc, char* argv[])
{
	Piezo_Config();
	TIMER_Config();
	Button_Config();
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,1);
	while (1)
    {
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_8)
		doraeme(0);
	else if(GPIO_Pin == GPIO_PIN_11)
		doraeme(1);
	else if(GPIO_Pin == GPIO_PIN_12)
		doraeme(2);

	flag = ~flag;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(flag)
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,1);
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,0);
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
