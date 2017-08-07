//filename: LCD_lcdinit.c
//func:lcdinit() ...(lcd api)


#include "LCD_lcdinit.h"
#include "DataType.h"
#include "stdlib.h"
#include "font.h"
#include "MyMisc.h"
#include "LCD_Core.h"
#include "spi.h"



u16 POINT_COLOR = 0x0000;
u16 BACK_COLOR = 0xffff;

_lcd_dev lcddev;

//func:lcd初始化
void initLcd(void)
{	 	
        SPILCD_RST_RESET ;	//LCD_RST=0	 //SPI接口复位
        Delay_ms(20);       // delay 20 ms 
        SPILCD_RST_SET ;	//LCD_RST=1		
        Delay_ms(20);

        lcddev.width=320;
        lcddev.height=240;
        lcddev.wramcmd=0X2C;
        lcddev.setxcmd=0x2a;
        lcddev.setycmd=0X2b; 	

        LCD_WR_REG(0xCB);  
        LCD_WR_DATA8(0x39); 
        LCD_WR_DATA8(0x2C); 
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x34); 
        LCD_WR_DATA8(0x02); 

        LCD_WR_REG(0xCF);  
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0XC1); 
        LCD_WR_DATA8(0X30); 
 
        LCD_WR_REG(0xE8);  
        LCD_WR_DATA8(0x85); 
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x78); 
 
        LCD_WR_REG(0xEA);  
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x00); 
 
        LCD_WR_REG(0xED);  
        LCD_WR_DATA8(0x64); 
        LCD_WR_DATA8(0x03); 
        LCD_WR_DATA8(0X12); 
        LCD_WR_DATA8(0X81); 

        LCD_WR_REG(0xF7);  
        LCD_WR_DATA8(0x20); 
  
        LCD_WR_REG(0xC0);    //Power control 
        LCD_WR_DATA8(0x23);   //VRH[5:0] 
 
        LCD_WR_REG(0xC1);    //Power control 
        LCD_WR_DATA8(0x10);   //SAP[2:0];BT[3:0] 
 
        LCD_WR_REG(0xC5);    //VCM control 
        LCD_WR_DATA8(0x3e); //对比度调节
        LCD_WR_DATA8(0x28); 
 
        LCD_WR_REG(0xC7);    //VCM control2 
        LCD_WR_DATA8(0x86);  //--
 
        LCD_WR_REG(0x36);    // Memory Access Control 
        LCD_WR_DATA8(0x48); //C8	   //48 68竖屏//28 E8 横屏

        LCD_WR_REG(0x3A);    
        LCD_WR_DATA8(0x55); 

        LCD_WR_REG(0xB1);    
        LCD_WR_DATA8(0x00);  
        LCD_WR_DATA8(0x18); 
 
        LCD_WR_REG(0xB6);    // Display Function Control 
        LCD_WR_DATA8(0x08); 
        LCD_WR_DATA8(0x82);
        LCD_WR_DATA8(0x27);  
 
        LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
        LCD_WR_DATA8(0x00); 
 
        LCD_WR_REG(0x26);    //Gamma curve selected 
        LCD_WR_DATA8(0x01); 
 
        LCD_WR_REG(0xE0);    //Set Gamma 
        LCD_WR_DATA8(0x0F); 
        LCD_WR_DATA8(0x31); 
        LCD_WR_DATA8(0x2B); 
        LCD_WR_DATA8(0x0C); 
        LCD_WR_DATA8(0x0E); 
        LCD_WR_DATA8(0x08); 
        LCD_WR_DATA8(0x4E); 
        LCD_WR_DATA8(0xF1); 
        LCD_WR_DATA8(0x37); 
        LCD_WR_DATA8(0x07); 
        LCD_WR_DATA8(0x10); 
        LCD_WR_DATA8(0x03); 
        LCD_WR_DATA8(0x0E); 
        LCD_WR_DATA8(0x09); 
        LCD_WR_DATA8(0x00); 

        LCD_WR_REG(0XE1);    //Set Gamma 
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x0E); 
        LCD_WR_DATA8(0x14); 
        LCD_WR_DATA8(0x03); 
        LCD_WR_DATA8(0x11); 
        LCD_WR_DATA8(0x07); 
        LCD_WR_DATA8(0x31); 
        LCD_WR_DATA8(0xC1); 
        LCD_WR_DATA8(0x48); 
        LCD_WR_DATA8(0x08); 
        LCD_WR_DATA8(0x0F); 
        LCD_WR_DATA8(0x0C); 
        LCD_WR_DATA8(0x31); 
        LCD_WR_DATA8(0x36); 
        LCD_WR_DATA8(0x0F); 
 
        LCD_WR_REG(0x11);    //Exit Sleep 
        Delay_ms(120);
				
        LCD_WR_REG(0x29);    //Display on 
        LCD_WR_REG(0x2c); 
		LCD_Clear(WHITE);
	
}


//func:写寄存器函数 
void LCD_WR_REG(u16 regval)
{ 
	SPILCD_CS_RESET;  //LCD_CS=0
    SPILCD_RS_RESET;
	SPI_WriteByte(SPI1,regval&0x00FF);
	SPILCD_CS_SET;  //LCD_CS=1	   		 
}


//func:写LCD数据
void LCD_WR_DATA(u16 data)
{
 	SPILCD_CS_RESET;  //LCD_CS=0
	SPILCD_RS_SET;	
	SPI_WriteByte(SPI1,data>>8);
	SPI_WriteByte(SPI1,data);
	SPILCD_CS_SET;  //LCD_CS=1		
}


//func:写8位数据
void LCD_WR_DATA8(u8 da)  
{
	SPILCD_CS_RESET;  //LCD_CS=0
	SPILCD_RS_SET;				    	   
	SPI_WriteByte(SPI1,da);	
	SPILCD_CS_SET;  //LCD_CS=1   			 
}


//func:写寄存器
void LCD_WR_REG_DATA(u8 LCD_Reg, u16 LCD_RegValue)
{
	LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}


//func：写GRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);  
}

//func:延时i
void opt_delay(u8 i)
{
	while(i--);
}


//func:设置光标位置
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
    LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA8(Xpos>>8); 
	LCD_WR_DATA8(Xpos&0XFF);	 
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA8(Ypos>>8); 
	LCD_WR_DATA8(Ypos&0XFF);
}


//func:画点
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_WR_DATA(POINT_COLOR); 
}


//func:清屏填充色
void LCD_Clear(u16 color)
{
	u32 index=0;      
	LCD_SetCursor(0x00,0x0000);	//设置光标位置 
	LCD_WriteRAM_Prepare();     //开始写入GRAM	 	  
	for(index=0;index<76800;index++)
	{
		LCD_WR_DATA(color);
	}

}


//func：指定区域填充色
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	xlen=ex-sx+1;	   
	for(i=sy;i<=ey;i++)
	{									   
	 	LCD_SetCursor(sx,i);      				//设置光标位置 
		LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
		for(j=0;j<xlen;j++)LCD_WR_DATA(color);	//设置光标位置 	    
	}
}


//func：指定区域填充颜色块（多个颜色）
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 		//得到填充的宽度
	height=ey-sy+1;		//高度
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//设置光标位置 
		LCD_WriteRAM_Prepare();     //开始写入GRAM
		for(j=0;j<width;j++)LCD->LCD_RAM=color[i*height+j];//写入数据 
	}	  
}

//func：画线
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

//func:画矩形
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}

//func:画圆
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
}


//func：16*16汉字
void showhanzi16(unsigned int x,unsigned int y,u16 index)	
{  
	unsigned char i,j,k;
	const unsigned char *temp=hanzi16;    
	temp+=index*32;	
	for(j=0;j<16;j++)
	{
		LCD_SetCursor(y,x+j);
		LCD_WriteRAM_Prepare();	//开始写入GRAM
		for(k=0;k<2;k++)
		{
			for(i=0;i<8;i++)
			{ 		     
			 	if((*temp&(1<<i))!=0)
				{
					LCD_WR_DATA(POINT_COLOR);
				} 
				else
				{
					LCD_WR_DATA(BACK_COLOR);
				}   
			}
			temp++;
		}
	 }
}


//func：32*32汉字
void showhanzi32(unsigned int y,unsigned int x, u32 index)	
{  
	unsigned char i,j,k;
	const unsigned char *temp=hanzi32;    
	temp+=index*128;	
	for(j=0;j<32;j++)
	{
		LCD_SetCursor(y,x+j);
		LCD_WriteRAM_Prepare();	//开始写入GRAM
		for(k=0;k<4;k++)
		{
			for(i=0;i<8;i++)
			{ 		     
			 	if((*temp&(1<<i))!=0)
				{
					LCD_WR_DATA(POINT_COLOR);
				} 
				else
				{
					LCD_WR_DATA(BACK_COLOR);
				}   
			}
			temp++;
		}
	 }
}


//func：64*64汉字
void showhanzi64(unsigned int y,unsigned int x, u32 index)	
{  
	unsigned char i,j,k;
	const unsigned char *temp=hanzi64;    
	temp+=index*512;	
	for(j=0;j<64;j++)
	{
		LCD_SetCursor(y,x+j);
		LCD_WriteRAM_Prepare();	//开始写入GRAM
		for(k=0;k<8;k++)
		{
			for(i=0;i<8;i++)
			{ 		     
			 	if((*temp&(1<<i))!=0)
				{
					LCD_WR_DATA(POINT_COLOR);
				} 
				else
				{
					LCD_WR_DATA(BACK_COLOR);
				}   
			}
			temp++;
		}
	 }
}


//func：m^n函数
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}



/*显示16*8字符*/
void show16_8(unsigned int y,unsigned int x, u16 index)	
{  
	unsigned char i,j,k;
	const unsigned char *temp=cha16_8;    
	temp+=index*16;	
	for(j=0;j<8;j++)
	{
		LCD_SetCursor(x,y+j);
		LCD_WriteRAM_Prepare();	//开始写入GRAM
		for(k=0;k<2;k++)
		{
			for(i=0;i<8;i++)
			{ 		     
			 	if((*temp&(1<<i))!=0)
				{
					LCD_WR_DATA(POINT_COLOR);
				} 
				else
				{
					LCD_WR_DATA(BACK_COLOR);
				}   
			}
			temp++;
		}
	 }
}

/*显示32*16字符*/
void show32_16(unsigned int y,unsigned int x, u16 index)	
{  
	unsigned char i,j,k;
	const unsigned char *temp=cha32_16;    
	temp+=index*64;	
	for(j=0;j<16;j++)
	{
		LCD_SetCursor(x,y+j);
		LCD_WriteRAM_Prepare();	//开始写入GRAM
		for(k=0;k<4;k++)
		{
			for(i=0;i<8;i++)
			{ 		     
			 	if((*temp&(1<<i))!=0)
				{
					LCD_WR_DATA(POINT_COLOR);
				} 
				else
				{
					LCD_WR_DATA(BACK_COLOR);
				}   
			}
			temp++;
		}
	 }
}

/*显示字符串16*8*/
void show16string(u32 x,u32 y,u32 num,u8 interval,u8 *p)
{
  u8 i,j;
  y = y-8;
  for(i = 0;i < num;i++)
  {
    for(j = 0;j < 96;j++)
    {
      if(*(p+i) == *(asc_string+j))
      {
        show16_8(y,x,j);
        x = x;
        y -= interval;
        break;
      }
    }
  }
  
}

/*显示字符串32*16*/
void show32string(u32 x,u32 y,u32 num,u8 interval,char *p)
{
  u8 i,j;
  y = y-16;
  for(i = 0;i < num;i++)
  {
    for(j = 0;j < 96;j++)
    {
      if(*(p+i) == *(asc_string+j))
      {
        show32_16(y,x,j);
        x = x;
        y -= interval;
        break;
      }
    }
  }
  
}
/*显示字符串32*16*//*
void show32string(u32 x,u32 y,u32 num,u8 interval, u8 *p)
{
  u8 i,j;
  y=y-32
}
*/

/*

void show16hanzistring(u32 x, u32 y,u32 num,u8 interval,u8 *p)
{
  u8 i,j;
  y = y-20;
  num *= 2;
  for(i = 0;i < num;i += 2)
  {
    for(j = 0;j < 28;j += 2)
    {
      if((*(p+i) == *(hanzi16_string+j))&&(*(p+1+i) == *(hanzi16_string+1+j)))
      {

        showhanzi16(y,x,(j/2));
        x = x;
        y -= interval;
        break;
      }
    }
  }
}
*/