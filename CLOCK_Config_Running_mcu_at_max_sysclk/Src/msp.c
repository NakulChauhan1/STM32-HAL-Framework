
#include "main.h"

void HAL_MspInit(void)				//low level inits of processor
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

void HAL_UART_MspInit ( UART_HandleTypeDef *huart )
{
	//will do later
}
