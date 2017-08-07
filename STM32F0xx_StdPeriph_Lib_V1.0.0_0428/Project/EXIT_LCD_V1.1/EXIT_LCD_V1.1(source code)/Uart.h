//filename: Uart.h

#ifndef _UART_H
#define _UART_H

#include "Common.h"



#define EVAL_COM1                        USART1
#define EVAL_COM1_CLK                    RCC_APB2Periph_USART1

#define EVAL_COM1_TX_PIN                 GPIO_Pin_9
#define EVAL_COM1_TX_GPIO_PORT           GPIOA
#define EVAL_COM1_TX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define EVAL_COM1_TX_SOURCE              GPIO_PinSource9
#define EVAL_COM1_TX_AF                  GPIO_AF_1

#define EVAL_COM1_RX_PIN                 GPIO_Pin_10
#define EVAL_COM1_RX_GPIO_PORT           GPIOA
#define EVAL_COM1_RX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define EVAL_COM1_RX_SOURCE              GPIO_PinSource10
#define EVAL_COM1_RX_AF                  GPIO_AF_1

#define EVAL_COM1_CTS_PIN                GPIO_Pin11
#define EVAL_COM1_CTS_GPIO_PORT          GPIOA
#define EVAL_COM1_CTS_GPIO_CLK           RCC_AHBPeriph_GPIOA
#define EVAL_COM1_CTS_SOURCE             GPIO_PinSource11
#define EVAL_COM1_CTS_AF                 GPIO_AF_1

#define EVAL_COM1_RTS_PIN                GPIO_Pin_12
#define EVAL_COM1_RTS_GPIO_PORT          GPIOA
#define EVAL_COM1_RTS_GPIO_CLK           RCC_AHBPeriph_GPIOA
#define EVAL_COM1_RTS_SOURCE             GPIO_PinSource12
#define EVAL_COM1_RTS_AF                 GPIO_AF_1
   
#define EVAL_COM1_IRQn                   USART1_IRQn


#define EVAL_COM2                        USART2
#define EVAL_COM2_CLK                    RCC_APB1Periph_USART2

#define EVAL_COM2_TX_PIN                 GPIO_Pin_2
#define EVAL_COM2_TX_GPIO_PORT           GPIOA
#define EVAL_COM2_TX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define EVAL_COM2_TX_SOURCE              GPIO_PinSource2
#define EVAL_COM2_TX_AF                  GPIO_AF_1

#define EVAL_COM2_RX_PIN                 GPIO_Pin_3
#define EVAL_COM2_RX_GPIO_PORT           GPIOA
#define EVAL_COM2_RX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define EVAL_COM2_RX_SOURCE              GPIO_PinSource3
#define EVAL_COM2_RX_AF                  GPIO_AF_1

#define EVAL_COM2_CTS_PIN                GPIO_Pin0
#define EVAL_COM2_CTS_GPIO_PORT          GPIOA
#define EVAL_COM2_CTS_GPIO_CLK           RCC_AHBPeriph_GPIOA
#define EVAL_COM2_CTS_SOURCE             GPIO_PinSource0
#define EVAL_COM2_CTS_AF                 GPIO_AF_1

#define EVAL_COM2_RTS_PIN                GPIO_Pin_1
#define EVAL_COM2_RTS_GPIO_PORT          GPIOA
#define EVAL_COM2_RTS_GPIO_CLK           RCC_AHBPeriph_GPIOA
#define EVAL_COM2_RTS_SOURCE             GPIO_PinSource1
#define EVAL_COM2_RTS_AF                 GPIO_AF_1
   
#define EVAL_COM2_IRQn                   USART2_IRQn


void initUart(void);
#endif