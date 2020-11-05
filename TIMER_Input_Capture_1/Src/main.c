/*
 * Measure the frequency  of a signal using Timer. We will be using LSE (32.768 MHz) as signal, ie we will be
 * measuring frequency of LSE signal.
 *
 * In this application, we have to generate LSE ie we have to first turn on LSE oscillator which is present
 * in our board and then have to bring it into one of the mcu pins (MC01 or MCO2) (for that have to do some settings)
	and then through jumper wires we will giving to timer channel so that we can calculate it.

	LSE is there in mcu for RTC related applications.
 */


#include "main.h"
#include<string.h>


void SystemClockConfig ();
void TIM2_Init ();
void Error_Handler();
void find_Frequency_of_signal ();
void LSE_Configurations ();
void UART2_Init ();

uint32_t first = -1;
uint32_t second = -1;

TIM_HandleTypeDef htim_6;
TIM_IC_InitTypeDef sConfig;
UART_HandleTypeDef huart;

volatile uint8_t is_capture_done = 0;

int main ()
{
	HAL_Init ();

	LSE_Configurations ();

	SystemClockConfig ();


	TIM2_Init ();

	UART2_Init ();										//because we want to send freq of signal value over uart to print it in terminal

	find_Frequency_of_signal ();



	return 0;
}

void find_Frequency_of_signal ()
{
	if ( HAL_TIM_IC_Start_IT ( &htim_6, TIM_CHANNEL_1 ) != HAL_OK )
	{
		Error_Handler ();
	}

		while ( 1 )					//wait until capture is done ie till both the values are not there, or until two Falling edges has not occurred
		{
			if ( is_capture_done == 1 )
			{
				uint32_t capture_difference;
				double time_period_of_signal;
				double timer2_count_resolution;
				double timer2_count_freq;
				double freq_of_signal;

				if ( second > first )
				{
					capture_difference = second - first;
				}
				else
				{
					capture_difference = first - second;
				}

				timer2_count_freq = ( HAL_RCC_GetPCLK1Freq () * 2) * ( htim_6.Init.Prescaler + 1 );
				timer2_count_resolution = 1/ timer2_count_freq;
				time_period_of_signal = capture_difference * timer2_count_resolution;
				freq_of_signal = 1/time_period_of_signal;

				char usr_msg[100];
				sprintf ( usr_msg, "Frequency of the signal applied = %.2f Hz\r\n", freq_of_signal );
				HAL_UART_Transmit ( &huart, (uint8_t*)usr_msg, strlen(usr_msg), HAL_MAX_DELAY );

				is_capture_done = 0;				//means next capture can take place
			}
		}
}


void LSE_Configurations ()
{
	//activating LSE crystal oscillator is done in SystemClockConfig

	uint32_t RCC_MCODiv = RCC_MCODIV_1;							//no division
	uint32_t RCC_MCOSource = RCC_MCO1SOURCE_LSE;				//oscillator frequency has to come from LSE
	uint32_t RCC_MCOx = RCC_MCO1;								//generate oscillator frequency /clock at MCO1 pin



	 HAL_RCC_MCOConfig( RCC_MCOx, RCC_MCOSource, RCC_MCODiv );			//it brings LSE oscillator frequency into gpio pin ie to MCO1

	//Note: Pin Muxing ie assigning pin alternate functionality (ie assigning pin capability to produce clock/frequency) is done in this API only, so no need to do it seperatly

}

void SystemClockConfig ()
{
		RCC_OscInitTypeDef osc_init;

		memset ( &osc_init, 0, sizeof(osc_init) );
		osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE;
		osc_init.HSIState = RCC_HSI_ON;
		osc_init.LSEState = RCC_LSE_ON;						//LSE Configurations
		osc_init.HSICalibrationValue = 16;
		osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;
		osc_init.PLL.PLLState = RCC_PLL_ON;
		osc_init.PLL.PLLM = 8;
		osc_init.PLL.PLLN = 50;
		osc_init.PLL.PLLP = 2;
		osc_init.PLL.PLLQ = 2;
		osc_init.PLL.PLLR = 2;

		if ( HAL_RCC_OscConfig ( &osc_init ) != HAL_OK )			//it activates HSI, LSE crystal oscillator and waits for turning it on
		{
			Error_Handler();
		}

		RCC_ClkInitTypeDef clk_init;
		uint32_t FLatency;

		memset ( &clk_init, 0, sizeof(clk_init) );
		clk_init.ClockType = ( RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 );
		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		clk_init.APB2CLKDivider = RCC_HCLK_DIV1;
		FLatency = FLASH_ACR_LATENCY_1WS;

		if ( HAL_RCC_ClockConfig ( &clk_init, FLatency ) != HAL_OK )
		{
			Error_Handler();
		}

		/* After this SYSCLK will be 50 MHz */

}

void HAL_TIM_PeriodElapsedCallback ( TIM_HandleTypeDef * htim )
{
	//do nothing or we can stop Timer
}

void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
{

}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)					//whenever capture event occurs then this callback will be called
{
	/* whenever capture interrupt happens then value of counter register is copied to CCR register and therefore we have to save
	  CCR's value in order to calculate the period of the signal applied */

	if ( is_capture_done == 0 )					//protecting the critical section ie shared variable first and second
	{
		if ( first < 0 )							//means first capture event has happened
		{
			first = __HAL_TIM_GET_COMPARE( htim, TIM_CHANNEL_1 );
		}

		if ( second < 0 && first > 0 )				//means second capture event has happened
		{
			second = __HAL_TIM_GET_COMPARE( htim, TIM_CHANNEL_1 );
			is_capture_done = 1;
		}
	}

}

void TIM2_Init ()
{
	//HLL configurations of timer2
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

void UART2_Init ()
{
	huart.Instance = USART2;
	huart.Init.BaudRate = 11520;
	huart.Init.WordLength = UART_WORDLENGTH_8B;
	huart.Init.StopBits = UART_STOPBITS_1;
	huart.Init.Parity = UART_PARITY_NONE;
	huart.Init.Mode = UART_MODE_TX;
	huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart.Init.OverSampling = UART_OVERSAMPLING_8;

	if ( HAL_UART_Init( &huart ) != HAL_OK )						//for low levels inits of uart, msp api will be called from inside of this API
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
