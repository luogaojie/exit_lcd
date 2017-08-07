//filename:LCD_SysInit.h

#ifndef _LCD_SYSINIT_H
#define _LCD_SYSINIT_H
#include "stm32f0xx.h"
#include "DataType.h"

void initSpi1(void);
void SPI1_DMA_Configuration(void);
u8 SPI_WriteByte(SPI_TypeDef* SPIxx,u8 Byte);


#define buffersize 1

#define SPI1_DR_Addr ( (u32)0x4001300C )





//#define SPI1                             SPI1
#define SPI1_CLK                         RCC_APB2Periph_SPI1
#define SPI1_IRQn                        SPI1_IRQn
#define SPI1_IRQHandler                  SPI1_IRQHandler

#define SPI1_SCK_PIN                     GPIO_Pin_3
#define SPI1_SCK_GPIO_PORT               GPIOB
#define SPI1_SCK_GPIO_CLK                RCC_AHBPeriph_GPIOB
#define SPI1_SCK_SOURCE                  GPIO_PinSource3
#define SPI1_SCK_AF                      GPIO_AF_0

#define SPI1_MISO_PIN                    GPIO_Pin_4
#define SPI1_MISO_GPIO_PORT              GPIOB
#define SPI1_MISO_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define SPI1_MISO_SOURCE                 GPIO_PinSource4
#define SPI1_MISO_AF                     GPIO_AF_0

#define SPI1_MOSI_PIN                    GPIO_Pin_5
#define SPI1_MOSI_GPIO_PORT              GPIOB
#define SPI1_MOSI_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define SPI1_MOSI_SOURCE                 GPIO_PinSource5
#define SPI1_MOSI_AF                     GPIO_AF_0



#define SPI2_CLK                         RCC_APB1Periph_SPI2
#define SPI2_IRQn                        SPI2_IRQn
#define SPI2_IRQHandler                  SPI2_IRQHandler

#define SPI2_SCK_PIN                     GPIO_Pin_13
#define SPI2_SCK_GPIO_PORT               GPIOB
#define SPI2_SCK_GPIO_CLK                RCC_AHBPeriph_GPIOB
#define SPI2_SCK_SOURCE                  GPIO_PinSource13
#define SPI2_SCK_AF                      GPIO_AF_0

#define SPI2_MISO_PIN                    GPIO_Pin_14
#define SPI2_MISO_GPIO_PORT              GPIOB
#define SPI2_MISO_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define SPI2_MISO_SOURCE                 GPIO_PinSource14
#define SPI2_MISO_AF                     GPIO_AF_0

#define SPI2_MOSI_PIN                    GPIO_Pin_15
#define SPI2_MOSI_GPIO_PORT              GPIOB
#define SPI2_MOSI_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define SPI2_MOSI_SOURCE                 GPIO_PinSource15
#define SPI2_MOSI_AF                     GPIO_AF_0

#endif