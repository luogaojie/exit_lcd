//filename: HAL_Mcu.h
#ifndef _HAL_MCU_H
#define _HAL_MCU_H


#include "stm32f0xx_it.h"
#include "Common.h"
#include "MyMisc.h"

void TIM2_IRQHandler(void);
void USART2_IRQHandler (void);
void EXTI4_15_IRQHandler(void);
void TIM3_IRQHandler(void);

#endif