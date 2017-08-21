// ----------------------------------------------------------------------------

/*LED control using JOG switch*/

#include<stdio.h>
#include<stdlib.h>

#define RCC_AHB1ENR (*(volatile unsigned long*)0x40023830)

#define GPIOA_MODER (*(volatile unsigned long*)0x40020000)
#define GPIOA_ODR   (*(volatile unsigned long*)0x40020014)
#define GPIOA_BSRR  (*(volatile unsigned long*)0x40020018)

#define GPIOB_MODER (*(volatile unsigned long*)0x40020400)
#define GPIOB_IDR   (*(volatile unsigned long*)0x40020410)

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
}// delay function

int main(int argc, char* argv[])
{
	RCC_AHB1ENR = 0x00000003; //port'A','B' use clock
	GPIOA_MODER = 0xA8000050; //use pin 2,3 in port'A' to output (pin2 : LED1, pin3: LED2)
	GPIOB_MODER = 0x00000280; //use pin 0,1,2,12,13 in port'B' to input

	unsigned int last = 0; //state of last JOG
	unsigned int current = 0; //state of current JOG
	unsigned char flag = 0; //upper 4 bit control LED2, lower 4bit control LED1

	/*main routine*/
	while (1) {
		current = GPIOB_IDR;
		if (!last) { //for edge check
			if (current & 0x0001) { //UP : 0x*0(LED1_OFF) or 0x*1(LED1_ON)
				flag &= 0xFD;
				flag ^= 0x01;
			} else if (current & 0x0002) {  //DOWN : 0x0*(LED2_OFF) or 0x1*(LED2_ON)
				flag &= 0xDF;
				flag ^= 0x10;
			} else if (current & 0x0004) //CENTER : 0x00(LED1,LED2 OFF)
				flag &= 0x00;
			else if (current & 0x1000) //LEFT : 0x*3(LED1_BLINK)
				flag |= 0x03;
			else if (current & 0x2000) //RIGHT : 0x3*(LED2_BLINK)
				flag |= 0x30;
		}

		last = current & 0x3007; //check only pin0,1,2,12,13
		
/*----------------------LED1 Control------------------------------------------*/
		if (!(flag & 0x01)) //0x*0 : LED1_OFF
			GPIOA_BSRR = 0x00040000;
		else if (flag & 0x02){ //0x*3 : LED1_BLINK
			GPIOA_ODR ^= 0x0004;
			ms_delay_int_count(100);
		}
		else if (!(flag & 0x02)) //0x*2 :LED1_ON
			GPIOA_BSRR = 0x00000004;
/*-----------------------------------------------------------------------------*/

/*----------------------LED2 Control------------------------------------------*/
		if (!(flag & 0x10)) //0x0* : LED2_OFF
			GPIOA_BSRR = 0x00080000;

		else if (flag & 0x20){ //0x3* : LED2_BLINK
			GPIOA_ODR ^= 0x0008;
			ms_delay_int_count(100);
		}
		else if (!(flag & 0x20)) //0x2* :LED2_ON
			GPIOA_BSRR = 0x00000008;
/*-----------------------------------------------------------------------------*/

	}
}
#pragma GCC diagnostic pop
