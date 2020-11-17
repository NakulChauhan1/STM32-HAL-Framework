//file for writing all interrupt handlers
/*
 * Note: all the interrupt or exception handlers are already defined in start up code, but they are attributed as weak
so, we only override these handlers in our application, according to our neeed.
*
*/

#include "main.h"
extern TIM_HandleTypeDef htim_2;

void SysTick_Handler ()
{
	HAL_IncTick ();					//incrementing global tick variable
	HAL_SYSTICK_IRQHandler ();	   //process the systick interrupt
}

void TIM2_IRQHandler ()
{
	HAL_TIM_IRQHandler( &htim_2 );
}
