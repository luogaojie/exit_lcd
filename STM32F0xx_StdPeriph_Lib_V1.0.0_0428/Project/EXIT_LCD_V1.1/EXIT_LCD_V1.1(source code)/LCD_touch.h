//filename: LCD_touch.h

#ifndef _LCD_TOUCH_H
#define _LCD_TOUCH_H

#include "DataType.h"
#include "stm32f0xx.h"
#include "stdlib.h"

#define TP_PRES_DOWN 0x80  //触屏被按下	  
#define TP_CATH_PRES 0x40  //有按键按下了


typedef struct
{
	u8 (*init)(void);			//初始化触摸屏控制器
	u8 (*scan)(u8);				//扫描触摸屏.0,屏幕扫描;1,物理坐标;	 
	void (*adjust)(void);		//触摸屏校准
	u16 x0;						//原始坐标(第一次按下时的坐标)
	u16 y0;
	u16 x; 						//当前坐标(此次扫描时,触屏的坐标)
	u16 y;						   	    
	u8  sta;					//笔的状态 
								//b7:按下1/松开0; 
	                            //b6:0,没有按键按下;1,有按键按下.         			  
////////////////////////触摸屏校准参数/////////////////////////								
	float xfac;					
	float yfac;
	short xoff;
	short yoff;	   
//新增的参数,当触摸屏的左右上下完全颠倒时需要用到.
//touchtype=0的时候,适合左右为X坐标,上下为Y坐标的TP.
//touchtype=1的时候,适合左右为Y坐标,上下为X坐标的TP.
	u8 touchtype;
}_m_tp_dev;

typedef	struct adjust
{
  u32 x[4];
  u32 y[4];
}adjust;
extern _m_tp_dev tp_dev;


#define PEN  	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)  	//PB11 INT
#define DOUT 	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)  	//PB14  MISO

#define TDIN1 	GPIO_SetBits(GPIOB,GPIO_Pin_15)  	//PB15  MOSI
#define TDIN0 	GPIO_ResetBits(GPIOB,GPIO_Pin_15)

#define TCLK1 	GPIO_SetBits(GPIOB,GPIO_Pin_13)  	//PB13  SCLK
#define TCLK0 	GPIO_ResetBits(GPIOB,GPIO_Pin_13)

#define TCS1  	GPIO_SetBits(GPIOB,GPIO_Pin_12)  	//PB12  CS  
#define TCS0  	GPIO_ResetBits(GPIOB,GPIO_Pin_12)

extern void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
u8 TP_adjust(void);
void TP_Write_Byte(u8 num);						//向控制芯片写入一个数据
u16 TP_Read_AD(u8 CMD);							//读取AD转换值
u16 TP_Read_XOY(u8 xy);							//带滤波的坐标读取(X/Y)
u8 TP_Read_XY(u16 *x,u16 *y);					//双方向读取(X+Y)
u8 TP_Read_XY2(u16 *x,u16 *y);					//带加强滤波的双方向坐标读取
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color);//画一个坐标校准点
void TP_Draw_Big_Point(u16 x,u16 y,u16 color);	//画一个大点
u8 TP_Scan(u8 tp);								//扫描
void TP_Save_Adjdata(void);						//保存校准参数
u8 TP_Get_Adjdata(void);						//读取校准参数
void TP_Adjust(void);							//触摸屏校准
u8 TP_Init(void);								//初始化
																 
void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac);//显示校准信息
void TP_init();
void delay_us(void);
//void SPI_Flash_Write(void);
//void SPI_Flash_Read(void);

 		  
#endif