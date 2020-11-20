/*
 * Write a application, which uses basic timer to generate interrupt for every 10  microseconds
 * but with timer clock frequency, TIMx_CLK of 50Mhz. Toggle the GPIO or LED inside, the Timer IRQ handler and verify using logic analyzer.
 *
 * */


#include "main.h"
#include<string.h>


void SystemClockConfig ();
void TIM6_Init ();
void Error_Handler();
void GPIO_Inits ();
void ToggleWithDelay ();


TIM_HandleTypeDef htim_6;

int main ()
{
	HAL_Init ();

	SystemClockConfig ();

	//now SYSCLK = 50MHz

	TIM6_Init ();

	GPIO_Inits ();

	//note: till here timer is only initialized not started, we just have to start the timer (in order to generate required delay), as we have done the initializations.
	ToggleWithDelay ();


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

void SystemClockConfig ()
{
	RCC_OscInitTypeDef osc_init;

	memset ( &osc_init, 0, sizeof(osc_init) );
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.HSICalibrationValue = 16;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLM = 8;
	osc_init.PLL.PLLN = 200;
	osc_init.PLL.PLLP = 4;
	osc_init.PLL.PLLQ = 2;
	osc_init.PLL.PLLR = 2;

	if ( HAL_RCC_OscConfig ( &osc_init ) != HAL_OK )
	{
		Error_Handler();
	}

	RCC_ClkInitTypeDef clk_init;
	uint32_t FLatency;

	memset ( &clk_init, 0, sizeof(clk_init) );
	clk_init.ClockType = ( RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 );
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV2;
	FLatency = FLASH_ACR_LATENCY_1WS;

	if ( HAL_RCC_ClockConfig ( &clk_init, FLatency ) != HAL_OK )
	{
		Error_Handler();
	}

	/* After this SYSCLK will be 100 MHz */

}

void TIM6_Init ()
{
	//note: SYSCLK = 50MHz
	//High Level inits of timer peripheral and then for LL corresponding msp function will be called from this API

	TIM_HandleTypeDef htim;
	htim.Instance = TIM6;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Prescaler = 0;				//( prescalar + 1 ) is the exact value which the clock
	htim.Init.Period = ( 500 - 1 );

	if ( HAL_TIM_Base_Init( &htim ) != HAL_OK )						//Initializing time base unit of Basic Timer
	{
		Error_Handler();
	}
}


void Error_Handler()
{
	while(1);
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
