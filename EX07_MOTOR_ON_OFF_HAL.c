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

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "stm32f4xx_hal.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

/*------------------------------------------------------------------------------------------------
 * JOG switch control LED1,2 and DC Motor
 *
 * LDE 1,2 has two modes 1. blink, 2. off
 * DC Motor has three modes 1.clock wise 2.count clock wise 3.short break
 *
 * JOG switch up controls LED1 blink or off
 * Jog switch down controls LED2 blink or off
 * Jog switch left controls DC motor Count clock wise or short break
 * Jog switch right controls DC motor clock wise or short break
 * Jog switch center makes DC motor to short break and LED 1,2 off
--------------------------------------------------------------------------------------------------*/

GPIO_InitTypeDef GpioA,GpioB;

void ms_delay_int_count(volatile unsigned int nTime) {
   nTime = (nTime * 14000);
   for (; nTime > 0; nTime--);
}// delay function

int main(int argc, char* argv[])
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*---------------Port 'A' initialize--------------*/
	GpioA.Pin 	= GPIO_PIN_6| GPIO_PIN_2 | GPIO_PIN_3; //6: MOTOR PWM, 2:LED1, 3:LED2
	GpioA.Mode	= GPIO_MODE_OUTPUT_PP;
	GpioA.Pull	= GPIO_NOPULL;
	GpioA.Speed	= GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOA,&GpioA); //put each value to proper memory address in port'A'
	/*-----------------------------------------------*/

	/*---------------Port 'B' initialize--------------*/
	GpioB.Pin	= GPIO_PIN_6 | GPIO_PIN_7; //6: DC_Motor P, 7:DC_Motor N
	GpioB.Mode	= GPIO_MODE_OUTPUT_PP;
	GpioB.Pull	= GPIO_NOPULL;
	GpioB.Speed	= GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOB,&GpioB); //put each value to proper memory address in port'B'
	/*-----------------------------------------------*/

	unsigned int last = 0;
	unsigned char flag1 = 0;
	unsigned char flag2 = 0;
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,1);
	while (1)
    {
		if(!last){
			if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)||flag1){ //UP : LED1 blink
				HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_2);
				flag1 = 1;
				ms_delay_int_count(100);
			}
			if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)||flag2){ //DOWN : LED2 blink
				HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_3);
				flag2 = 1;
				ms_delay_int_count(100);
			}
			if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)){ //CENTER : ALL OFF
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2|GPIO_PIN_3,0); //LED1,2 OFF
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6|GPIO_PIN_7,1); //Motor short break
				flag1 = 0; flag2 = 0;
			}
			if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)){ //LEFT : motor ccw, or short break controlled DC_Motor P
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,1);
				HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_6);
			}
			if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13)){ //RIGHT : motor cw, or short break controlled DC_Motor N
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,1);
				HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
			}
		}
		last = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_12|GPIO_PIN_13); //last check any active button
    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
