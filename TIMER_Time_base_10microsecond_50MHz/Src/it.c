//file for writing all interrupt handlers
/*
 * Note: all the interrupt or exception handlers are already defined in start up code, but they are attributed as weak
so, we only override these handlers in our application, according to our neeed.
*
*/

#include "main.h"

extern TIM_HandleTypeDef htim_6;


void SysTick_Handler ()
{
	HAL_IncTick ();					//incrementing global tick variable
	HAL_SYSTICK_IRQHandler ();	   //process the systick interrupt
}

void TIM6_DAC_IRQHandler ()
{
	HAL_TIM_IRQHandler( &htim_6 );
}

/* Note: Main code which is to be executed when interrupt occurs is written in Application Callback function,
	   which is present in Application layer. So note: first ISR is called whenever interrupt occurs, then interrupt
	   processing API is called inside ISR which finds out the cause of event, ie which event causes interrupt and then
	   that interrupt processing API ie IRQHandler just calls application call back for that event to do further main thing
	   while interrupt like writing main send api code, receiving code etc.
	    */
