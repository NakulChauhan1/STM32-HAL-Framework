/*
 * Write a application, which uses basic timer to generate interrupt for every 100 ms. Toggle the GPIO or LED inside
 * the Timer IRQ handler and verify using logic analyzer.
 *
 * */


#include "main.h"
#include<string.h>

void SystemClockConfig();
void TIM6_Inits();
void Error_Handler();
void GPIO_Inits();
void ToggleWithDelay ();

TIM_HandleTypeDef htim_6;

int main ()
{

	//first API to be called in order to use STMCube HAL LAyer
	//it does HL initializations and LL initializations of processor
	HAL_Init();

	//clock configurations for the Application
	SystemClockConfig();

	//HLL initializations and LL initializations of peripheral
	TIM6_Inits();

	GPIO_Inits();


	//note: till here timer is only initialized not started, we just have to start the timer (in order to generate required delay), as we have done the initializations.
	ToggleWithDelay ();

	while(1);

	return 0;
}

void ToggleWithDelay ()
{
	//start timer in interrupt mode
	HAL_TIM_Base_Start_IT ( &htim_6 );
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)				//after every specified amount of time, this function is called via ISR->IRQHandler then finally this function
{
	/* If code has reached here, it means the required time delay has been elapsed, in our case 100 ms */
	HAL_GPIO_TogglePin ( GPIOA, GPIO_PIN_5 );
	//TIM6->SR = 0;		//clearing the Update event*/
}


void SystemClockConfig()
{
	//we donot need clk config, in this application, so default clock is sufficient
}

void TIM6_Inits()
{
	htim_6.Instance = TIM6;
	htim_6.Init.CounterMode = TIM_COUNTERMODE_UP;				//Basic timer can only do up counting
	htim_6.Init.Prescaler = 24;
	htim_6.Init.Period = 64000 - 1;

	if ( HAL_TIM_Base_Init ( &htim_6 ) != HAL_OK )
	{
		Error_Handler ();
	}

}


void GPIO_Inits()
{
	GPIO_InitTypeDef gpio;

	memset (&gpio, 0, sizeof(gpio));
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	//no need of Alternate functionality
	gpio.Pin = GPIO_PIN_5;

	HAL_GPIO_Init( GPIOA, &gpio );					//GPIOA is pointer which is type casted to GPIOA_BASE in stm32f446xx.h

	//Enabling clock of GPIOA
	__HAL_RCC_GPIOA_CLK_ENABLE();
}

void Error_Handler ()
{
	while(1);
}














/*
 * 1. Processor Initializations
 * 2. System Clock Configurations
 * 3. Peripheral Initializations
 * 	  High Level Inits (done in UART2_Init() ) + Low Level Inits ( done in msp.c)
 *
 *
 * Processor Initializations: ( done in HAL_Inits() )
 * - High Level Inits
 * - Low Level Inits (call is made by HAL_Init() to HAL_MspInit() in msp.c to do this task)
 *
 *
 * Peripheral Initializations:
 * - High Level Inits   ( done in UART2_Init () )
 * - Low Level Inits	(call is made by HAL_UART_Init() to HAL_UART_MspInit() in msp.c to do this task)
 *
 * */
