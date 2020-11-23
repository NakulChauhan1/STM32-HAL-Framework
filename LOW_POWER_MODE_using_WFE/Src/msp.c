#include "stm32f4xx_hal.h"

void HAL_MspInit(void)
{
	//here we will do processor specific low level initializations required for the application

	//1. Set up the priority grouping for the processor
	HAL_NVIC_SetPriorityGrouping ( NVIC_PRIORITYGROUP_4 );

	//2. Enable required system exceptions of the processor
	SCB->SHCSR |= ( 0x7 << 16 );							//enabling MemoryManagement, BusFault, UsageFault system  exception
	/*HAL_NVIC_EnableIRQ ( MemoryManagement_IRQn  );
	HAL_NVIC_EnableIRQ( BusFault_IRQn );
	HAL_NVIC_EnableIRQ( UsageFault_IRQn );*/

	//3. Configure the priority for the system exceptions
	HAL_NVIC_SetPriority( MemoryManagement_IRQn, 0, 0 );
	HAL_NVIC_SetPriority( BusFault_IRQn, 0, 0 );
	HAL_NVIC_SetPriority( UsageFault_IRQn, 0, 0 );

	//Note: All the APIs which deals with processor specific details are in "stm32f4xx_hal_cortex.c" header file

	// There can be many other processor specific low level intialisations like low power mode etc, all depends upon the requirement of application

}

void HAL_TIM_Base_MspInit ( TIM_HandleTypeDef * htimer_6 )
{

	//enabling the clock of peripheral
	__HAL_RCC_TIM6_CLK_ENABLE();

	//enabling the IRQ of peripheral
	HAL_NVIC_EnableIRQ( TIM6_DAC_IRQn );

	//setting priority grouping
	HAL_NVIC_SetPriorityGrouping ( NVIC_PRIORITYGROUP_4 );

	//set priority
	HAL_NVIC_SetPriority ( TIM6_DAC_IRQn, 15, 0 );


}


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	 GPIO_InitTypeDef gpio_uart;
	 //here we are going to do the low level inits. of the USART2 peripheral

	 //1. enable the clock for the USART2 peripheral as well as for GPIOA peripheral

	 __HAL_RCC_USART2_CLK_ENABLE();
	 __HAL_RCC_USART2_CLK_SLEEP_DISABLE();				/*clock gating for USART2 peripheral, ie clock will be
	 	 	 	 	 	 	 	 	 	 	 	 	 	  automatically disabled when processor will go into sleep,
	 	 	 	 	 	 	 	 	 	 	 	 	 	  this is done in order to save power*/
	 __HAL_RCC_GPIOA_CLK_ENABLE();


	 //2 . Do the pin muxing configurations
	 gpio_uart.Pin = GPIO_PIN_2;
	 gpio_uart.Mode =GPIO_MODE_AF_PP;
	 gpio_uart.Pull = GPIO_PULLUP;
	 gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	 gpio_uart.Alternate =  GPIO_AF7_USART2; //UART2_TX
	 HAL_GPIO_Init(GPIOA,&gpio_uart);

	 gpio_uart.Pin = GPIO_PIN_3; //UART2_RX
	 HAL_GPIO_Init(GPIOA,&gpio_uart);


	 //3 . Enable the IRQ and set up the priority (NVIC settings )
	 //since we are using polling mode of USART, therefore enabling IRQ ie enabling interrupt request is not required
	 /*HAL_NVIC_EnableIRQ(USART2_IRQn);
	 HAL_NVIC_SetPriority(USART2_IRQn,15,0);*/

}
