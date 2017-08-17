// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "stm32f4xx.h"

#define SHIFT_VAL 0x00040000

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
} //delay 함수 (측정을 통해 계산된값)

int main(int argc, char* argv[])
{
	RCC->AHB1ENR = 0x00000004; //supply clock in port'C'
	GPIOC->MODER = 0x00000550; //Use pin 2,3,4,5 in port 'C'
	GPIOC->OTYPER = 0;
	GPIOC->PUPDR = 0;
	GPIOC->OSPEEDR = 0;

	int i;
  // Infinite loop
  while (1)
    {
       // Add your code here.
	  GPIOC->ODR = 0x003C;
	  GPIOC->BSRR = SHIFT_VAL;
	  ms_delay_int_count(100);
	  for(i = 0 ;i<4;i++){
		  GPIOC->BSRR = SHIFT_VAL << i; //write 'reset' ->LED ON (Active Low)
		  ms_delay_int_count(500);
      }
	  for(i = 3 ;i>=0;i--){
	  	  GPIOC->BSRR = (SHIFT_VAL >> 16) << i; //write 'set' ->LED OFF (Active Low)
	  	  ms_delay_int_count(500);
	  }
    }
}

#pragma GCC diagnostic pop
