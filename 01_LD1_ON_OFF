#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#define RCC_AHB1ENR (*(volatile unsigned long*)0x40023830) //supply CLOCK to AHB1ENR
#define GPIOC_MODER (*(volatile unsigned long*)0x40020800) //select pin mode among Input,Output, Alternate function or Analog mode
#define GPIOC_OTYPER (*(volatile unsigned long*)0x40020804) //select output type between push-pull or open-drain
#define GPIOC_OSPEEDR (*(volatile unsigned long*)0x40020808) //output speed
#define GPIOC_PUPDR (*(volatile unsigned long*)0x4002080C) //pull-up or pull-down
#define GPIOC_ODR (*(volatile unsigned long*)0x40020814) //output data

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

void ms_delay_int_count(volatile unsigned int nTime)
{
	nTime = (nTime*14000);
	for(;nTime > 0 ; nTime--);
} //delay calculated by test

int main(int argc, char* argv[])
{
	RCC_AHB1ENR = 0x00000004; //supply clock in port'C'
	GPIOC_MODER = 0x00000550; //use pin 2,3,4,5 in port'C'
	GPIOC_OTYPER = 0; //push-pull
	GPIOC_PUPDR = 0; //No pull-up, No pull_down
	GPIOC_OSPEEDR = 0; //Low speed

  // Infinite loop
  while (1)
    {
       // Add your code here.
	  GPIOC_ODR = 0x003C; //LD all off (Active Low)
	  ms_delay_int_count(100);
	  GPIOC_ODR = 0x0000; //LD all on (Active Low)
	  ms_delay_int_count(100);
    }
}

#pragma GCC diagnostic pop
