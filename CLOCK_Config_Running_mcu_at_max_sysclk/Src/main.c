/*
 * Write an Application which does PLL configurations to boost the HCLK to
 * maximum capacity ie 180Mhz for our mcu, by using HSE as PLL source.
 * */

#include "main.h"

UART_HandleTypeDef uart2;

int main(void)
{

  HAL_Init();

  SystemClockConfig();

  UART2_Init ();

  print_clock_info ();

  while (1);

  return 0;
}


void SystemClockConfig(void)
{
	RCC_OscInitTypeDef osc_init;

	memset( &osc_init, 0, sizeof(osc_init) );
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.HSICalibrationValue = 16;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	osc_init.PLL.PLLM = 8;
	osc_init.PLL.PLLN = 180;
	osc_init.PLL.PLLP = 2;
	osc_init.PLL.PLLQ = 2;							//default value
	osc_init.PLL.PLLR = 2;							//default value


	if ( HAL_RCC_OscConfig( &osc_init ) != HAL_OK )
	{
		Error_Handler();
	}

	uint8_t Flash_Latency = 0;

	RCC_ClkInitTypeDef clk_init;

	memset( &clk_init, 0, sizeof(clk_init) );
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 ;
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV4;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV2;
	Flash_Latency = FLASH_ACR_LATENCY_5WS;

	if ( HAL_RCC_ClockConfig ( &clk_init, Flash_Latency ) != 1 )
	{
		Error_Handler();
	}

	ALL THIS IS NOT SUFFICIENT TO DRIVE AT MAX FREQ, WE HAVE TO DO MORE SETTING.

	/* Since base of all clocks (ie SYSCLK) is changed, so we have to reconfigure all the things
	 * which we have done earlier by keeping old base value, among which SYTICK timer is one */

	HAL_SYSTICK_CLKSourceConfig ( SYSTICK_CLKSOURCE_HCLK );

	if ( HAL_SYSTICK_Config ( HAL_RCC_GetSysClockFreq()/1000 ) != HAL_OK )
	{
		Error_Handler();
	}
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


void Error_Handler ()
{
	//we can give warning here like blinking red led
	while(1);
}

void print_clock_info ()
{

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
