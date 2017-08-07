//filename: spi.c
//func:LCD_spiinit();...

#include "spi.h"

u8 SPI1_TX_Buff[buffersize];
u8 SPI1_RX_Buff[buffersize];

//func:spi1初始化
void initSpi1(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
  
  RCC_APB2PeriphClockCmd(SPI1_CLK, ENABLE);
  
  /* Enable SCK, MOSI, MISO and NSS GPIO clocks */
  RCC_AHBPeriphClockCmd(SPI1_SCK_GPIO_CLK | SPI1_MISO_GPIO_CLK | SPI1_MOSI_GPIO_CLK, ENABLE);
  
  GPIO_PinAFConfig(SPI1_SCK_GPIO_PORT, SPI1_SCK_SOURCE, SPI1_SCK_AF);
  GPIO_PinAFConfig(SPI1_MOSI_GPIO_PORT, SPI1_MOSI_SOURCE, SPI1_MOSI_AF);
  GPIO_PinAFConfig(SPI1_MISO_GPIO_PORT, SPI1_MISO_SOURCE, SPI1_MISO_AF);
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;

  /* SPI SCK--PA3 configuration */
  GPIO_InitStructure.GPIO_Pin = SPI1_SCK_PIN;
  GPIO_Init(SPI1_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI--PA4 configuration */
  GPIO_InitStructure.GPIO_Pin =  SPI1_MOSI_PIN;
  GPIO_Init(SPI1_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO--PA5 configuration */
  GPIO_InitStructure.GPIO_Pin = SPI1_MISO_PIN;
  GPIO_Init(SPI1_MISO_GPIO_PORT, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_5);
   /* -------------SPI_RxFIFOThresholdConfig---------------*/
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;


  
  SPI_Init(SPI1, &SPI_InitStructure);
  SPI_Cmd(SPI1,ENABLE);
  SPI_RxFIFOThresholdConfig(SPI1,SPI_RxFIFOThreshold_QF);
  
}


//spi1设置成成DMA传输
void SPI1_DMA_Configuration(void)
{
    DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel2);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)0x4001300C;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI1_RX_Buff;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = (u16)1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);

    
    DMA_DeInit(DMA1_Channel3);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)0x4001300C;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI1_TX_Buff;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = (u16)1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
    
    DMA_Cmd (DMA1_Channel2,ENABLE);

    DMA_Cmd (DMA1_Channel3,ENABLE);
}

//func:spi读写(8位)
u8 SPI_WriteByte(SPI_TypeDef* SPIxx,u8 Byte)
{
  u8 retry=0;
  while (SPI_I2S_GetFlagStatus(SPIxx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_SendData8(SPIxx,Byte); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPIxx, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_ReceiveData8(SPIxx); //返回通过SPIx最近接收的数据


}
