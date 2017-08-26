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
#include "stm32f4xx.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

/*-----------------------------------------------------------------------------
 * Control LD 1,2 in MDP board using switch s1,2,3
 *
 * *************pin map****************
 * LD1 : PC2
 * LD2 : PC3
 * S1  : PA8
 * S2  : PA11
 * S3  : PA12
 * ************************************
 *
 * *************function***************
 * S1(pop up) -> LD1(toggle blink, off);
 * S2(pop up) -> LD2(toggle blink, off);
 * S3(pop up) -> LD1(off),LD2(off)
 * ************************************
 *----------------------------------------------------------------------------*/

int led1=0,led2=0;//led = (led == 0)? off:blink

void GPIOA_Config(void)
{
	RCC->AHB1ENR	|=	0x00000001;
	GPIOA->MODER	&=	0xA8000000;
	GPIOA->PUPDR	=	0x00000000;
	GPIOA->OSPEEDR	= 	0x00000000;
}

void GPIOC_Config(void)
{
	RCC->AHB1ENR	|=	0x00000004;
	GPIOC->MODER	|=	0x00000050;
	GPIOC->PUPDR	=	0x00000000;
	GPIOC->OSPEEDR	=	0x00000000;
}

void EXTI_Config(void)
{
	RCC->APB2ENR |= 0x00004000; //use system configuration

	SYSCFG->EXTICR[2] &= 0x0FF0; //Pin 11,Pin8 Port A(0000)
	SYSCFG->EXTICR[3] &= 0xFFF0; //Pin 12 Port A(0000)

	EXTI->FTSR 	|= 0x00001900; //pop up -> falling edge
	EXTI->IMR 	|= 0x00001900; //mask -> only use pin12,11,8

	//pin 8's priority is higher than pin11,12's
	NVIC_SetPriority(EXTI9_5_IRQn ,0);
	NVIC_SetPriority(EXTI15_10_IRQn ,1);

	//NVIC can handle Interrupt pin8,11,12
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void ms_delay_int_count(volatile unsigned int nTime) {
   nTime = (nTime * 14000);
   for (; nTime > 0; nTime--);
}// delay function

int main(int argc, char* argv[])
{
	GPIOA_Config();
	GPIOC_Config();

	EXTI_Config();
	while (1)
    {
		if(led1)
			GPIOC->ODR ^= 0x00000004;
		else
			GPIOC->ODR |= 0x00000004;

		if(led2)
			GPIOC->ODR ^= 0x00000008;
		else
			GPIOC->ODR |= 0x00000008;

		ms_delay_int_count(100);
    }
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI->PR == 0x00000100){//interrupt from pin 8
		EXTI->PR |= 0x00000100;//interrupt has handled
		led1 = ~led1;
	}
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI->PR == 0x00000800){//interrupt from pin 11
		EXTI->PR |= 0x00000800;//interrupt has handled
		led2 = ~led2;
	}else if(EXTI->PR == 0x00001000){//interrupt from pin 12
		EXTI->PR |= 0x00001000;//interrupt has handled
		led1=0; led2=0;//led all off
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
