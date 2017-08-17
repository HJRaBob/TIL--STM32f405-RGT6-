// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#define RCC_AHB1ENR (*(volatile unsigned long*)0x40023830)
#define GPIOA_MODER (*(volatile unsigned long*)0x40020000)
#define GPIOA_OTYPER (*(volatile unsigned long*)0x40020004)
#define GPIOA_OSPEEDR (*(volatile unsigned long*)0x40020008)
#define GPIOA_PUPDR (*(volatile unsigned long*)0x4002000C)
#define GPIOA_ODR (*(volatile unsigned long*)0x40020014)

#define GPIOC_MODER (*(volatile unsigned long*)0x40020800)
#define GPIOC_OTYPER (*(volatile unsigned long*)0x40020804)
#define GPIOC_OSPEEDR (*(volatile unsigned long*)0x40020808)
#define GPIOC_PUPDR (*(volatile unsigned long*)0x4002080C)
#define GPIOC_ODR (*(volatile unsigned long*)0x40020814)

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
	RCC_AHB1ENR = 0x00000005; //'A','C' port에 clock 공급
	GPIOA_MODER = 0xA8000050; //'A' port의 2,3 핀 출력모드로 선언
	GPIOA_OTYPER = 0; //port'A' output type register
	GPIOA_PUPDR = 0; //port'A' pull up pull dawn retister
	GPIOA_OSPEEDR = 0; //port'A'

	GPIOC_MODER = 0x00000550; //'C' port의 2,3,4,5 핀 출력모드로 선언
	GPIOC_OTYPER = 0;
	GPIOC_PUPDR = 0;
	GPIOC_OSPEEDR = 0;
  // At this stage the system clock should have already been configured
  // at high speed.

  // Infinite loop
  while (1)
    {
       // Add your code here.
	  GPIOA_ODR = 0x000C; //LED_ON
	  GPIOC_ODR = 0X003C; //LD_OFF
	  ms_delay_int_count(100);
	  GPIOA_ODR = 0x0000; //LED_OFF
	  GPIOC_ODR = 0X0000; //LD_ON
	  ms_delay_int_count(100);
    }
}

#pragma GCC diagnostic pop
