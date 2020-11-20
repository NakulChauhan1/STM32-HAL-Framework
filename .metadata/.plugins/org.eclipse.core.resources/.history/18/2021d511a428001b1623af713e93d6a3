/*
 *  Toggle LED connected to PA5 with 1 second delay. Donot use software to toggle LED.
 *
 *  Output Channel of Timer can be used to toggle LED without software.
 *
 * */


#include "main.h"
#include<string.h>

void SytemClockConfig ();
void GPIO_Inits ();
void TIM2_Inits ();
void Error_Handler();

TIM_HandleTypeDef htim;
uint32_t pulse_value = 12500000;				//pulse value to generate delay of 1s

int main ()
{
	HAL_Init ();

	SytemClockConfig ();

	GPIO_Inits ();										//making PA5 as output for toggling LED

	TIM2_Inits ();

	if ( HAL_TIM_OC_Start_IT ( &htim, TIM_CHANNEL_1) != HAL_OK  )				//starting OC channel 1, with interrupt capability
	{
		Error_Handler ();
	}

	while(1);

	return 0;
}


void TIM2_Inits ()
{

	htim.Instance = TIM2;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = 0xFFFFFFFF;
	if ( HAL_TIM_OC_Init ( &htim ) )
	{
		Error_Handler ();
	}

	TIM_OC_InitTypeDef sConfig;
	sConfig.OCMode = TIM_OCMODE_TOGGLE;
	sConfig.Pulse = pulse_value;							//first value of CCR
	sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;

	if ( HAL_TIM_OC_ConfigChannel( &htim, &sConfig, TIM_CHANNEL_1 ) != HAL_OK )
	{
		Error_Handler();
	}
}

void SytemClockConfig ()
{

	RCC_OscInitTypeDef OscInitStruct;
	memset ( &OscInitStruct, 0, sizeof(OscInitStruct) );
	OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	OscInitStruct.HSEState = RCC_HSE_ON;
	OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	OscInitStruct.PLL.PLLM = 8;
	OscInitStruct.PLL.PLLN = 50;
	OscInitStruct.PLL.PLLP = 2;
	OscInitStruct.PLL.PLLQ = 2;								//default state
	OscInitStruct.PLL.PLLR = 2;								//default state

	if ( HAL_RCC_OscConfig ( &OscInitStruct ) != HAL_OK )					//it activates HSE ie crystal oscillator and waits for turning it on
	{
		Error_Handler ();
	}

	RCC_ClkInitTypeDef ClkInitStruct;
	uint32_t FLatency;

	ClkInitStruct.ClockType = ( RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 );
	ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	FLatency = FLASH_ACR_LATENCY_1WS;

	if ( HAL_RCC_ClockConfig ( &ClkInitStruct, FLatency ) != HAL_OK )
	{
		Error_Handler ();
	}

	/* After this SYSCLK will be 50 MHz */

}

void GPIO_Inits ()
{

	__HAL_RCC_GPIOA_CLK_ENABLE ();

	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin  = GPIO_PIN_5;
	GPIO_Init.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_Init.Pull = GPIO_PULLUP;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_MEDIUM;

	HAL_GPIO_Init ( GPIOA, &GPIO_Init );
}

void HAL_TIM_OC_DelayElapsedCallback( TIM_HandleTypeDef *htim )
{
	uint32_t ccr_value;
	ccr_value = HAL_TIM_ReadCapturedValue ( htim, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE ( htim, TIM_CHANNEL_1, ccr_value + pulse_value );				//updates the value of CCR so that next toggling of led can happen, Note: Toggling happens at every instance when counter value reaches CCR value
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
