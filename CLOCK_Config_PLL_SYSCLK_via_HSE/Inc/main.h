
#ifndef MAIN_H_
#define MAIN_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

void SystemClockConfig( uint8_t clk );
void UART2_Init ();
void Error_Handler ();
void print_clock_info ();

/** @defgroup SYS_CLK_FREQ
  * @{
  */

#define SYS_CLK_FREQ_50MHz		50
#define SYS_CLK_FREQ_84MHz		84
#define SYS_CLK_FREQ_120MHz		120

#endif /* MAIN_H_ */
