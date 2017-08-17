//;******************************************************************************************
//;* 02_LED_SHIFT_ONLY_ODR
//;*
//;*이 프로그램은 포트C의 핀 2,3,4,5
//;* Target: stm32f4
//;*****************************************************************************************

/*관계 레지스터의 주소*/

#include "stm32f4xx.h"
#define VAL 0x0004 //constant for shift operate

void ms_delay_int_count(volatile unsigned int nTime) {
   nTime = (nTime * 14000);
   for (; nTime > 0; nTime--);
}

/*메읶루틴*/
int main(int argc, char* argv[]) {
   /*GPIO초기화(각 레지스터의 값)*/
   RCC->AHB1ENR = 0x00000004; //supply clock in port'C'
   GPIOC->MODER = 0x00000550; //Use pin 2,3,4,5 in port 'C'
   GPIOC->OTYPER = 0;
   GPIOC->PUPDR = 0;
   GPIOC->OSPEEDR = 0;

   int i=0;
   /*char flag = 0x0;*/ //for문 사용 안할시 사용
   /*메읶루틴에서 하는 일*/
   //light change LD1~4
   while (1) {

      for(i = 0;i<4;i++)
      {
         GPIOC ->ODR = ~(VAL << i); //Active Low : use '~'
         ms_delay_int_count(500);
      }
      for(i = 3; i>=0; i--)
      {
         GPIOC ->ODR = ~(VAL << i);
         ms_delay_int_count(500);
      }

//--------------------------------------------------------------------------------------
      //not use 'for'
      /*
      GPIOC ->ODR = ~(VAL << i);
      ms_delay_int_count(500);
      if(!flag)
    	  i++;
      else
    	  i--;
      if(i==-1||i==4)
    	  flag = ~flag;*/
   }
}
