#include "stm32f4xx_hal.h"

void HAL_MspInit()
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



void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	/* Low level inits of peripheral */

	//enabling the clock of peripheral
	__HAL_RCC_TIM2_CLK_ENABLE();

	//enabling the IRQ of peripheral
	HAL_NVIC_EnableIRQ( TIM2_IRQn );

	//setting priority grouping
	HAL_NVIC_SetPriorityGrouping ( NVIC_PRIORITYGROUP_4 );

	//set priority
	HAL_NVIC_SetPriority ( TIM2_IRQn, 15, 0 );

	/*Pin Muxing
		as we are using using pins to give inputs to timer, therefore we have to give alternate functionality to those gpios
	  	  ie configure gpio behave as TIMER2 channel 1
			Note: by this we are only configuring pin as channel, not as input channel or output channel */



	GPIO_InitTypeDef GPIO_Init;

	GPIO_Init.Mode = GPIO_MODE_AF_PP;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_Init.Pin = GPIO_PIN_0;
	GPIO_Init.Alternate = GPIO_AF1_TIM2;

	HAL_GPIO_Init ( GPIOA, &GPIO_Init );

}
