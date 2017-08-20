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

int
main(int argc, char* argv[])
{
  // At this stage the system clock should have already been configured
  // at high speed.

	RCC->AHB1ENR = 0x00000002; //supply clock to port'B'
	GPIOB->MODER = 0x40000280; // pin 15 in port'B'
	GPIOB->OTYPER = 0;
	GPIOB->PUPDR = 0;
	GPIOB->OSPEEDR = 0;

	unsigned int period,buf;
  // Infinite loop
  while (1)
    {
       // Add your code here.
		for (period = 0x1000; period >= 1; period--) { //frequency is incresing
			GPIOB->ODR = 0x8000;
			buf = period;
			while (buf--); //High value time

			GPIOB->ODR = 0x0000;
			buf = period;
			while (buf--); //Low value time -> duty rate is same
		}
    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
