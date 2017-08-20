/*
 * This file is part of the µOS++ distribution.
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

// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via ITM).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the ITM output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void ms_delay_int_count(volatile unsigned int nTime) {
   nTime = (nTime * 14000);
   for (; nTime > 0; nTime--);
}

int main(int argc, char* argv[])
{
	RCC->AHB1ENR = 0x00000003; //supply clock in port'A','B'

	GPIOA->MODER = 0xA8001000; //PWM pin 6 in port'A'
	GPIOA->OTYPER = 0;
	GPIOA->PUPDR = 0;
	GPIOA->OSPEEDR = 0;

	GPIOB->MODER = 0x00005280; //pin 6 (DCMoter_P) pin 7 (DCMoter_N) in port'B'
	GPIOB->OTYPER = 0;
	GPIOB->PUPDR = 0;
	GPIOB->OSPEEDR = 0;

	GPIOA->ODR = 0x0040;

  while (1)
    {
	  GPIOB->ODR = 0x0040; //CW
	  ms_delay_int_count(3000);
	  GPIOB->ODR = 0x0000; //STOP
	  ms_delay_int_count(3000);
	  GPIOB->ODR = 0x0080; //CCW
	  ms_delay_int_count(3000);
	  GPIOB->ODR = 0x0000; //STOP
	  ms_delay_int_count(3000);
    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
