/*
 * WAP to send characters over UART to MCU. MCU should convert all the the lower case letters in to uppercase and send it back to the user
 * Using below parameter settings:
 * BaudRate = 115200
 * No of data bits = 8
 * Stop Bits = 1
 * Parity = None
 * NO UART Hardware flow control
 * Use UART2 peripheral of the MCU in Asynchronous mode (ie UART mode)
 *
 * */



#include "main.h"
#include<string.h>

void SystemClockConfig();
void UART2_Init ();
void Error_Handler ();

UART_HandleTypeDef uart2;

char * data = "UART testing\n";

int main()
{
	//first API to be called in order to use STMCube HAL LAyer
	//it does HL initializations and LL initializations of processor
	HAL_Init();

	//not present in Cube HAL, this  is application specific, by default system clock will be running internal RC oscillator, but to change it this function is used
	SystemClockConfig();

	//HLL initializations and LL initializations of peripheral
	UART2_Init ();


	//since we do not know how much byte we will receive, so we will receive it byte by byte
	uint8_t rcvd_data;
	uint8_t rcv_buffer[100];
	uint8_t c = 0;

	//note: Code will be blocked at HAL_UART_Transmit() or HAL_UART_Receive() until byte is received
	uint32_t status;
	status = HAL_UART_Transmit ( &uart2, (uint8_t *) data, strlen(data), HAL_MAX_DELAY );			//HAL_MAX_DELAY is standard macro of HAL which tells time at which something is declared as time out
	if ( status != HAL_OK )
	{
		//means data transmission failed
		Error_Handler();
	}

	while (1)
	{
		HAL_UART_Receive ( &uart2, &rcvd_data, 1,  HAL_MAX_DELAY );					//reading one byte of data

		if ( rcvd_data == '\r' )					//when user enters carriage return then it means its end of data
			break;

		rcv_buffer [ c++ ] = rcvd_data;
	}

	rcv_buffer[c++] = '\r';

	//converting lower case to capital and capital to lower
	for ( uint8_t i = 0; i < c; i++ )
	{
		if ( rcv_buffer[i] > 'a' && rcv_buffer[i] < 'z' )
			rcv_buffer [i] -= 32;
		else if ( rcv_buffer[i] > 'A' && rcv_buffer[i] < 'Z' )
			rcv_buffer [i] += 32;
	}

	HAL_UART_Transmit ( &uart2, rcv_buffer, c, HAL_MAX_DELAY );			//HAL_MAX_DELAY is standard macro of HAL which tells time at which something is declared as time out



	/*Note: In STM32 Cube Framework project for every 1 ms, systick timer will be ticking, so there will be systick timer interrupt in every 1 ms,
	  entire STM32 frame functionality depends upon it like timer related functionality and other timing related functionality of cube frame work
	 so we have to implement systick handler in our code and we have to increment the global tick variable  inside the handler because cube layer is dependent on this variable value.
	 So we have to implement systick handler and we know we implement or do interrupt related things in in it.c
	 */

	while(1);

	return 0;
}

void SystemClockConfig()
{
	//this application does not require system clock configurations
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
