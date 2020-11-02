/*
 * Measure the frequency  of a signal using Timer.
 * */


#include "main.h"
#include<string.h>


void SystemClockConfig ();
void TIM2_Init ();
void Error_Handler();
void find_Frequency_of_signal ();

uint32_t first = -1;
uint32_t second = -1;

TIM_HandleTypeDef htim_6;
TIM_IC_InitTypeDef sConfig;

int main ()
{
	HAL_Init ();

	SystemClockConfig ();

	//now SYSCLK = 50MHz

	TIM2_Init ();

	find_Frequency_of_signal ();

	while ( !(first >= 0 && second >= 0) );

	uint32_t freq_of_signal = 1/(second - first);


	return 0;
}

void find_Frequency_of_signal ()
{
	if ( HAL_TIM_IC_Start_IT ( &htim_6, TIM_CHANNEL_1 ) != HAL_OK )
	{
		Error_Handler ();
	}
}

void SystemClockConfig ()
{

}

void HAL_TIM_PeriodElapsedCallback ( TIM_HandleTypeDef * htim )
{
	//do nothing or we can stop Timer
}

void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
{
	if ( first < 0 )
	{
		first = htim->Instance->CNT;
	}
	if ( second < 0 && first > 0 )
	{
		second = htim->Instance->CNT;
	}
	if ( first >=0 && second >= 0 )			//stop the timer, period is calculated
	{
		HAL_TIM_IC_Stop_IT ( htim, TIM_CHANNEL_1 );
	}
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

}

void TIM2_Init ()
{
	htim_6.Instance = TIM2;
	htim_6.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim_6.Init.Prescaler = 1;				//clock will be divided by 2
	htim_6.Init.Period = 0xFFFFFFFF;		//counting upto maximum value

	if ( HAL_TIM_IC_Init ( &htim_6 ) != HAL_OK )			//this API is used to configure the Time base unit of Input Capture Timer
	{
		Error_Handler();
	}

	htim_6.Channel = HAL_TIM_ACTIVE_CHANNEL_1;

	sConfig.ICPolarity = TIM_ICPOLARITY_RISING;				//ICPolarity asks for at which edge we want event to be detected ie timer to capture the value of Counter register
	sConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;			//tells which input to select in MUX which is present in diagram, 19.3.4 section in RTM
	sConfig.ICPrescaler = TIM_ICPSC_DIV1;									//to slow down the input signal in input channel of timer
	sConfig.ICFilter = 0;									//to configure the digital filter present in input stage of timer

	if ( HAL_TIM_IC_ConfigChannel ( &htim_6, &sConfig, TIM_CHANNEL_1 ) != HAL_OK )		//this API is used to configure the input channel of the Input Capture Timer
	{
		Error_Handler();
	}
}



void Error_Handler()
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
