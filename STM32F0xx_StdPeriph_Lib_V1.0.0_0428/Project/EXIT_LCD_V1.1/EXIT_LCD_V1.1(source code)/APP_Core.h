//filename: APP_Core.h

#ifndef _APP_CORE_H
#define _APP_CORE_H

#include "Common.h"
#include "APP_Uart.h"

#define CHECK_GOODS_TIME  40

/////////////////光电信号输入////////////////////////////////////
#define PhotoElec_INPUT1  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)

///////////////////////信号输入///////////////////////////////////////
#define Signal_INPUT1  ExitGPIO(GPIOA,GPIO_Pin_7,GPIOA,GPIO_Pin_6,GPIOA,GPIO_Pin_4)


void taskSchdule(void);
void LED_Blink(void);
void Input_Scan(void);
void PhotoElecCheck(void);

#endif