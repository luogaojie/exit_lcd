//filename' LCD_lcdinit.h

#ifndef _LCD_LCDINIT_H
#define _LCD_LCDINIT_H

#include "DataType.h"
#include "stdlib.h"
#include "stm32f0xx.h"


#define SPILCD_RST_SET     GPIO_SetBits(GPIOA,GPIO_Pin_11);
#define SPILCD_RST_RESET   GPIO_ResetBits(GPIOA,GPIO_Pin_11);//PA11-lcd_rst

#define SPILCD_RS_SET      GPIO_SetBits(GPIOA,GPIO_Pin_10);
#define SPILCD_RS_RESET    GPIO_ResetBits(GPIOA,GPIO_Pin_10);//PA10-lcd_dc

#define SPILCD_CS_SET      GPIO_SetBits(GPIOA,GPIO_Pin_15);
#define SPILCD_CS_RESET    GPIO_ResetBits(GPIOA,GPIO_Pin_15);//PA15-lcd_cs



//LCD结构体
typedef struct
{
  u16 width;
  u16 height;
  u16 id;
  u8  wramcmd;
  u8  setxcmd;
  u8  setycmd;
}_lcd_dev;

//LCD地址结构体
typedef struct
{
  u16 LCD_REG;
  u16 LCD_RAM;
}LCD_TypeDef;


extern _lcd_dev lcddev;//lcd参数
extern u16 POINT_COLOR;//字体颜色，默认红色
extern u16 BACK_COLOR;//字体背景色，默认白色

/****************************颜色定义**************************_lcd_dev.setxcmd************/
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A10作为数据命令区分线 
//注意设置时STM32内部会右移一位对其! 111110=0X3E			    
#define LCD_BASE        ((u32)(0x60000000 | 0x0007FFFE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)


void show32string(u32 x,u32 y,u32 num,u8 interval,char *p);
void show16_8(unsigned int y,unsigned int x, u16 index);
void show32_16(unsigned int y,unsigned int x, u16 index);
//void show16hanzistring(u32 x, u32 y,u32 num,u8 interval,u8 *p);
void show16string(u32 x,u32 y,u32 num,u8 interval,u8 *p);
void LCD_DrawPoint(u16 x,u16 y);											//画点
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);								//快速画点
void Draw_Circle(u16 x0,u16 y0,u8 r);										//画圆
void LCD_Clear(u16 color);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   				//画矩形
//void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);		   				//填充单色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);				//填充指定颜色					//显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//显示一个数字
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//显示 数字
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);//显示一个字符串,12/16字体
void initLcd(void);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_WriteRAM_Prepare(void);
void LCD_SetCursor(u16 Xpos, u16 Ypos);

u32 LCD_Pow(u8 m,u8 n);
void LCD_WR_REG_DATA(u8 LCD_Reg, u16 LCD_RegValue);
void LCD_WR_DATA(u16 data);
void opt_delay(u8 i);
void LCD_WR_REG(u16 regval);
void showimage(u16 x,u16 y); //显示40*40图片
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);

void LCD_WR_DATA8(u8 da);   //写8位数据  
void showhanzi16(unsigned int x,unsigned int y,u16 index);//16*16汉字
void showhanzi32(unsigned int x,unsigned int y,u32 index);//32*32汉字
void showhanzi64(unsigned int y,unsigned int x, u32 index);
#endif 