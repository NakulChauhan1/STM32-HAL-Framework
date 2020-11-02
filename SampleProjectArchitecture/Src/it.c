//file for writing all interrupt handlers
/*
 * Note: all the interrupt or exception handlers are already defined in start up code, but they are attributed as weak
so, we only override these handlers in our application, according to our neeed.
*
*/

/* Whenever interrupt is called triggered, then one of these ISR is called according to the corresponding interrupt */

#include "main.h"

extern UART_HandleTypeDef uart2;         //note: we needed handle variable which consists of all configuration for irqhandler but it was declared in main.c (some other file), so we used concept of extern

void SysTick_Handler ()
{
	HAL_IncTick ();					//incrementing global tick variable
	HAL_SYSTICK_IRQHandler ();	   //process the systick interrupt
}


void USART2_IRQHandler ()			//whenever byte will be received by mcu, then this handler will be immediately called by processor
{
	//interrupt processing API
	HAL_UART_IRQHandler ( &uart2 );				//this api finds out what causes interrupt and does whatever is required when that interrupt occurs, ie call Application CallBack

	/* Note: Main code which is to be executed when interrupt occurs is written in Application Callback function,
	   which is present in Application layer. So note: first ISR is called whenever interrupt occurs, then interrupt
	   processing API is called inside ISR which finds out the cause of event, ie which event causes interrupt and then
	   that interrupt processing API ie IRQHandler just calls application call back for that event to do further main thing
	   while interrupt like writing main send api code, receiving code etc. */
}
