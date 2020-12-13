/*
 * Application for sending data over UART2, whenever button interrupt is received.

Measure current consumption with sleep mode and without sleep mode.

Enter to sleep mode using WFI instruction.
 *
 * */


/* Outcome of using sleep mode is consumption of current decreases by 1.5 mA - 2.0 mA */

#include "main.h"
#include<string.h>

void SystemClockConfig();
void Error_Handler();
void GPIO_Inits();
void UART2_Inits ();


TIM_HandleTypeDef htim_6;
UART_HandleTypeDef huart2;

char data[] = "SLEEPONEXIT feature\r\n";

int main ()
{

	//first API to be called in order to use STMCube HAL LAyer
	//it does HL initializations and LL initializations of processor


	HAL_Init();						//processor level initializations

	//clock configurations for the Application
	SystemClockConfig();

	//HLL initializations and LL initializations of peripherals



	//Initializations of GPIO
	GPIO_Inits();

	//Initializations of UART
	UART2_Inits ();


	//enabling Sleep Mode using Sleep on Exit feature
	HAL_PWR_EnableSleepOnExit ();				// enabled after all the initializations

	/* start with fresh Status register of Timer to avoid any spurious interrupts */
	  TIM6->SR = 0;



	while(1);

	return 0;
}






void SystemClockConfig()
{
	//we do not need clk config in this application, so default clock is sufficient
}




void GPIO_Inits()
{

	//these pins are used for debugging purpose
    __HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef ledgpio ;
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&ledgpio);

	ledgpio.Pin = GPIO_PIN_12;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&ledgpio);

	GPIO_AnalogConfig ();				//putting all unused pins into analog mode to save power

}


void UART2_Inits ()
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 921600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX;

	if ( HAL_UART_Init(&huart2) != HAL_OK )
	{
		//There is a problem
		Error_Handler();
	}

}

void GPIO_AnalogConfig(void)
{
	GPIO_InitTypeDef GpioA;

	uint32_t gpio_pins = GPIO_PIN_0 | GPIO_PIN_1 |GPIO_PIN_4 | \
						 GPIO_PIN_5 | GPIO_PIN_6 |GPIO_PIN_7 |\
						 GPIO_PIN_8 | GPIO_PIN_9 |GPIO_PIN_10 |\
						 GPIO_PIN_11 | GPIO_PIN_12 |GPIO_PIN_13 | \
						 GPIO_PIN_14 | GPIO_PIN_15;

	GpioA.Pin = gpio_pins;
	GpioA.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init( GPIOA, &GpioA );
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
