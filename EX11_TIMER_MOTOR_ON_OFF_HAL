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

/*=============================================================================
 * DC motor control using Timer3 (up count)
 *
 * timer toggles DC motor CW and Short break
 *
 * --------pin number-----------
 * DCMotor PWM 	: PA6
 * DCMotor_p	: PB6
 * DCMotor_N	: PB7
 * -----------------------------
 *
 * -------DCMoter operate---------
 * ON(CW) : DCMotor_P->HIGH
 * 			DCMOTOR_N->LOW
 * 			DCMOTOP_PWN->HIGH
 *
 * OFF 	  : DCMotor_P->HIGH
 * 			DCMOTOR_N->HIGH
 * 			DCMOTOP_PWN->HIGH
 *--------------------------------
=============================================================================*/


#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

TIM_HandleTypeDef TimHandle;
GPIO_InitTypeDef DCMotor;

void Timer_Config(void)
{
	__HAL_RCC_TIM3_CLK_ENABLE();

	TimHandle.Instance				= TIM3;
	TimHandle.Init.Period			= 10000-1;
	TimHandle.Init.Prescaler		= 8400-1;
	TimHandle.Init.ClockDivision	= TIM_CLOCKDIVISION_DIV1;
	TimHandle.Init.CounterMode		= TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&TimHandle);

	HAL_TIM_Base_Start_IT(&TimHandle);

	HAL_NVIC_SetPriority(TIM3_IRQn,0,0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

void DCMotor_Config(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	DCMotor.Pin 	= GPIO_PIN_6|GPIO_PIN_7; //DCMotor_P,N setting
	DCMotor.Mode	= GPIO_MODE_OUTPUT_PP;
	DCMotor.Pull	= GPIO_NOPULL;
	DCMotor.Speed	= GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOB,&DCMotor);

	DCMotor.Pin		= GPIO_PIN_6; //PWM setting
	HAL_GPIO_Init(GPIOA,&DCMotor);
}

int main(int argc, char* argv[])
{
	DCMotor_Config();
	Timer_Config();

	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,1);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,1);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,0);
	while (1)
    {
    }
}

void  HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
