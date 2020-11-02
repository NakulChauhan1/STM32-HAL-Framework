
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"

void SystemClockConfig ();
void UART2_Init ();
void Error_Handler ();
void print_clock_info ();

#endif /* __MAIN_H */

