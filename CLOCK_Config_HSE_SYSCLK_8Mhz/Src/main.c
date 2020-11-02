/*
 * Using HSE Configure the SYSCLk as 8Mhz
 * ABH clock as 4Mhz (HCLK)
 * APB1 clock as 2Mhz (PCLK1)
 * APB2 clock as 2Mhz (PCLK2)
 */


#include "main.h"
#include<string.h>


UART_HandleTypeDef uart2;


int main()
{
	//first API to be called in order to use STMCube HAL LAyer
	//it does HL initializations and LL initializations of processor
	HAL_Init();

	//not present in Cube HAL, this  is application specific, by default system clock will be running internal RC oscillator, but to change it this function is used
	SystemClockConfig();

	//HLL initializations and LL initializations of peripheral
	UART2_Init ();

	print_clock_info ();
	while(1);

	return 0;
}

void SystemClockConfig()
{
	RCC_OscInitTypeDef osc_init;

	memset ( &osc_init, 0, sizeof(osc_init) );

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_BYPASS;

	if ( HAL_RCC_OscConfig ( &osc_init ) != HAL_OK )			//HAL_RCC_OscConfig() API will turn on HSE oscillator, if it is there and will wait until it will be turned on. Till now, system clock is not sourced or configured to HSE, it is just turned on with this API
	{
		//Note: This API just activates the selected oscillator and waits till its not on
		Error_Handler();
	}


	RCC_ClkInitTypeDef clk_init;
	clk_init.ClockType = ( RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 );
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV2;

	if ( HAL_RCC_ClockConfig ( &clk_init, FLASH_ACR_LATENCY_0WS ) != HAL_OK )				//from this point onwards sysclk will be sourced by HSE
	{
		Error_Handler ();
	}

	//as sysclk source is changed to HSE, so HSI can be turned off to save power
	__HAL_RCC_HSI_DISABLE();

	/****************After this line SYSCLK is sourced by HSE */

	/*Since system clock is changed so SYSTICK configurations needs to be changed as it depends upon SYSCLK
	  As systick is responsible for generating interrupt for every 1ms and this is the requirement for STM32 device HAL Layer */

	HAL_SYSTICK_Config ( HAL_RCC_GetHCLKFreq()/1000 );					//parameter is the value which is tick number which means number of ticks to generate 1 ms interrupt with HCLK as frequency, here ( 1 ms/0.25 )

	HAL_SYSTICK_CLKSourceConfig ( SYSTICK_CLKSOURCE_HCLK );				//prescalar is there before reaching cortex system timer

	//so reconfiguration of SYSTICK timer is done

	/* Note: When main clock of uC ie SYSCLK is changed then we have to also reconfigure those things which were configured by keeping previous SYSCLK in mind like SYSTICK timer inits, peripheral init etc. */

}

void UART2_Init ()
{
		//UART_HandleTypeDef uart2;
		uart2.Instance = USART2;
		uart2.Init.BaudRate = 115200;
		uart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		uart2.Init.Mode = UART_MODE_RX;
		uart2.Init.OverSampling = UART_OVERSAMPLING_8;
		uart2.Init.Parity = UART_PARITY_NONE;
		uart2.Init.StopBits = UART_STOPBITS_1;
		uart2.Init.WordLength = UART_WORDLENGTH_8B;

		if ( HAL_UART_Init ( &uart2 ) != HAL_OK )		//from this api, HAL_UART_MspInit() api will called and LL inits of peripheral will be done
		{
			//something went wrong
			Error_Handler ();
		}
}

void print_clock_info ()
{
	char msg[100];

	memset ( msg, 0, sizeof(msg) );
	sprintf ( msg, "SYSCLK: %ld \r\n", HAL_RCC_GetSysClockFreq() );
	HAL_UART_Transmit( &uart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY );

	memset ( msg, 0, sizeof(msg) );
	sprintf ( msg, "HCLK: %ld \r\n", HAL_RCC_GetHCLKFreq() );
	HAL_UART_Transmit( &uart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY );

	memset ( msg, 0, sizeof(msg) );
	sprintf ( msg, "PCLK1: %ld \r\n", HAL_RCC_GetPCLK1Freq() );
	HAL_UART_Transmit( &uart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY );

	memset ( msg, 0, sizeof(msg) );
	sprintf ( msg, "PCLK2: %ld \r\n", HAL_RCC_GetPCLK2Freq() );
	HAL_UART_Transmit( &uart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY );

}


void Error_Handler ()
{
	//we can give warning here like blinking red led
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
