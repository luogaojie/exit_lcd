//filename: LCD_touch.c
//func：(...)

#include "LCD_touch.h"
#include "LCD_lcdinit.h"
#include "stdlib.h"
#include "math.h"
#include "DataType.h"
#include "MyMisc.h"
#include "spi.h"
#include "LCD_flash.h"
#include "LCD_Core.h"
#include "watchDog.h"

_m_tp_dev tp_dev;

//默认为touchtype=0的数据.
u8 CMD_RDX=0XD0;
u8 CMD_RDY=0X90;
u16 SAVE_ADDR_BASE=0;
extern u32 A,B,X_0,Y_0,adjust_flag;


//SPI写数据
//向触摸屏IC写入1byte数据    
//num:要写入的数据
void TP_Write_Byte(u8 num)    
{  
  
	u8 count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN1;  
		else TDIN0;
		num<<=1;    
		TCLK0; 	 
		TCLK1;		//上升沿有效	        
	}
  
  //SPI_SendData8(SPI2,num);
} 		 
//SPI读数据 
//从触摸屏IC读取adc值
//CMD:指令
//返回值:读到的数据	   
u16 TP_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	TCLK0;		//先拉低时钟 	 
	TDIN0; 	//拉低数据线
	TCS0; 		//选中触摸屏IC
	TP_Write_Byte(CMD);//发送命令字
	Delay_us(6);//ADS7846的转换时间最长为6us
	TCLK0; 	     	    
	Delay_us(1);    	   
	TCLK1;		//给1个时钟，清除BUSY	    	    
	TCLK0;
    
	for(count=0;count<16;count++)//读出16位数据,只有高12位有效 
	{ 				  
		Num<<=1; 	 
		TCLK0;	//下降沿有效  	    	   
		TCLK1;
		if(DOUT)Num++; 		 
	}
   
    //Num=SPI_I2S_ReceiveData16(SPI2); 	
	Num>>=4;   	//只有高12位有效.
    //Num &= 0xfff0;
	TCS1;		//释放片选	 
	return(Num);   
}
//读取一个坐标值(x或者y)
//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值 
//xy:指令（CMD_RDX/CMD_RDY）
//返回值:读到的数据
#define READ_TIMES 10 	//读取次数
#define LOST_VAL 2	  	//丢弃值
u16 TP_Read_XOY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
    
	for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
    
	return TP_Read_AD(xy); 
} 
//读取x,y坐标
//最小值不能少于100.
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);	  												   
	//if(xtemp<100||ytemp<100)return 0;//读数失败
	*x=xtemp;
	*y=ytemp;
	return 1;//读数成功
}
//连续2次读取触摸屏IC,且这两次的偏差不能超过
//ERR_RANGE,满足条件,则认为读数正确,否则读数错误.	   
//该函数能大大提高准确度
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
#define ERR_RANGE 250 //误差范围 (x/11,y/15转换成实际误差值)
u8 TP_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=TP_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-ERR_RANGE内
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
}

//////////////////////////////////////////////////////////////////////////////////		  
//与LCD部分有关的函数  
//画一个触摸点
//用来校准用的
//x,y:坐标
//color:颜色
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color)
{
	POINT_COLOR=color;
	LCD_DrawLine(x-12,y,x+13,y);//横线
	LCD_DrawLine(x,y-12,x,y+13);//竖线
	LCD_DrawPoint(x+1,y+1);
	LCD_DrawPoint(x-1,y+1);
	LCD_DrawPoint(x+1,y-1);
	LCD_DrawPoint(x-1,y-1);
	Draw_Circle(x,y,6);//画中心圈
}	  
//画一个大点(2*2的点)		   
//x,y:坐标
//color:颜色
void TP_Draw_Big_Point(u16 x,u16 y,u16 color)
{	    
	POINT_COLOR=color;
	LCD_DrawPoint(x,y);//中心点 
	LCD_DrawPoint(x+1,y);
	LCD_DrawPoint(x,y+1);
	LCD_DrawPoint(x+1,y+1);	 	  	
}						  
//////////////////////////////////////////////////////////////////////////////////		  
//触摸按键扫描
//tp:0,屏幕坐标;1,物理坐标(校准等特殊场合用)
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
u8 TP_Scan(u8 tp)
{			   
	if(PEN==0)//有按键按下
	{

		if(tp)TP_Read_XY2(&tp_dev.x,&tp_dev.y);//读取物理坐标
		else if(TP_Read_XY2(&tp_dev.x,&tp_dev.y))//读取屏幕坐标
		{
             A = *((volatile int *)(flashaddr_A));
             B = *((volatile int *)(flashaddr_B));
             X_0 = *((volatile int *)(flashaddr_X_0));
             Y_0 = *((volatile int *)(flashaddr_Y_0));
             Y_0 = Y_0&0x3ff;
             //tp_dev.y -= B*8;                    //校准手动校准（！！！）
             if(tp_dev.x >=X_0)
             {
                 tp_dev.x = (tp_dev.x-X_0)/A;
                 if(tp_dev.x>240)
                 {
                   tp_dev.x = 0;
                 }
                 else tp_dev.x = 240-tp_dev.x;
             }
             else tp_dev.x = 0;
             
            if(tp_dev.y >=Y_0)
                 {
                 tp_dev.y = (tp_dev.y-Y_0)/B;
                 }
                 
            else tp_dev.y = 0;
             
	 	 } 
		if((tp_dev.sta&TP_PRES_DOWN)==0)//之前没有被按下
		{		 
			tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES;//按键按下  
			tp_dev.x0=tp_dev.x;//记录第一次按下时的坐标
			tp_dev.y0=tp_dev.y;  	   			 
		}			   
	}else
	{
		 if(tp_dev.sta&TP_PRES_DOWN)//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);//标记按键松开	
		}
        else//之前就没有被按下
		{
			tp_dev.x0=0;
			tp_dev.y0=0;
			tp_dev.x=0xffff;
			tp_dev.y=0xffff;
		}	    
	}
	return tp_dev.sta&TP_PRES_DOWN;//返回当前的触屏状态
}



//func:校准程序，校准数据保存在地址为0x08006c00~0x08007800;
u8 TP_adjust(void)
{ 
  
   u32 x1,x2,x3,x4,y1,y2,y3,y4; 
    adjust_flag = *((volatile u32 *)(flashaddr_Y_0));
    adjust_flag = adjust_flag>>10;
    if(adjust_flag == 1)
    {
      return 0;
    }
   close_exitPB11();//关闭中断
    adjust adjust_xy;
    
    LCD_Fill(0,19,40,21,BLACK);
    LCD_Fill(19,0,21,40,BLACK);
    while(1)
    {
        if(PEN == 0)
        {
        Delay_ms(20);
        if(PEN == 0)
        {
        TP_Read_XY2(&tp_dev.x,&tp_dev.y);
        x1 = tp_dev.x;
        y1 = tp_dev.y;
        
        TP_Read_XY2(&tp_dev.x,&tp_dev.y);
        x2 = tp_dev.x;
        y2 = tp_dev.y;
        
        TP_Read_XY2(&tp_dev.x,&tp_dev.y);
        x3 = tp_dev.x;
        y3 = tp_dev.y;
        
        TP_Read_XY2(&tp_dev.x,&tp_dev.y);
        x4 = tp_dev.x;
        y4 = tp_dev.y;
        
        adjust_xy.x[0] = (x1+x2+x3+x4)/4;
        adjust_xy.y[0] = (y1+y2+y3+y4)/4;
        
        break;
        }
        }
      
    }
    
    Delay_ms(1000);    
    LCD_Fill(0,299,40,301,BLACK);
    LCD_Fill(19,280,21,320,BLACK);


    while(1)
    {
        if(PEN == 0)
        {
        Delay_ms(20);
        if(PEN == 0)
        {    
        TP_Read_XY2(&tp_dev.x,&tp_dev.y);
        x1 = tp_dev.x;
        y1 = tp_dev.y;
        
        TP_Read_XY2(&tp_dev.x,&tp_dev.y);
        x2 = tp_dev.x;
        y2 = tp_dev.y;
        
        TP_Read_XY2(&tp_dev.x,&tp_dev.y);
        x3 = tp_dev.x;
        y3 = tp_dev.y;
        
        TP_Read_XY2(&tp_dev.x,&tp_dev.y);
        x4 = tp_dev.x;
        y4 = tp_dev.y;
        
        adjust_xy.x[1] = (x1+x2+x3+x4)/4;
        adjust_xy.y[1] = (y1+y2+y3+y4)/4;
        
        break;
        }
        }
      
    }
        Delay_ms(1000);    
        LCD_Fill(200,299,240,301,BLACK);
        LCD_Fill(219,280,221,320,BLACK);        

    while(1)
    {
        if(PEN == 0)
        {
        Delay_ms(20);
        if(PEN == 0)
        {    
        TP_Read_XY2(&tp_dev.x,&tp_dev.y);
        x1 = tp_dev.x;
        y1 = tp_dev.y;
        
        TP_Read_XY2(&tp_dev.x,&tp_dev.y);
        x2 = tp_dev.x;
        y2 = tp_dev.y;
        
        TP_Read_XY2(&tp_dev.x,&tp_dev.y);
        x3 = tp_dev.x;
        y3 = tp_dev.y;
        
        TP_Read_XY2(&tp_dev.x,&tp_dev.y);
        x4 = tp_dev.x;
        y4 = tp_dev.y;
        
        adjust_xy.x[2] = (x1+x2+x3+x4)/4;
        adjust_xy.y[2] = (y1+y2+y3+y4)/4;
        
        break;
        }
        }
      
    }
        Delay_ms(1000);    
        LCD_Fill(200,19,240,21,BLACK);
        LCD_Fill(219,0,221,40,BLACK); 

    while(1)
    {
        if(PEN == 0)
        {
        Delay_ms(20);
        if(PEN == 0)
        {    
        TP_Read_XY2(&tp_dev.x,&tp_dev.y);
        x1 = tp_dev.x;
        y1 = tp_dev.y;
        
        TP_Read_XY2(&tp_dev.x,&tp_dev.y);
        x2 = tp_dev.x;
        y2 = tp_dev.y;
        
        TP_Read_XY2(&tp_dev.x,&tp_dev.y);
        x3 = tp_dev.x;
        y3 = tp_dev.y;
        
        TP_Read_XY2(&tp_dev.x,&tp_dev.y);
        x4 = tp_dev.x;
        y4 = tp_dev.y;
        
        adjust_xy.x[3] = (x1+x2+x3+x4)/4;
        adjust_xy.y[3] = (y1+y2+y3+y4)/4;
        
        break;
        }
        }
    }      
    
    adjust_flag = 1;
    //校准
    A=(adjust_xy.x[0]-adjust_xy.x[3])/200;
    B=(adjust_xy.y[2]-adjust_xy.y[3])/280;
    X_0=adjust_xy.x[3]-20*A;
    Y_0=adjust_xy.y[3]-20*B;
    
    flashwrite_addr400(flashaddr_A,A);
    flashwrite_addr400(flashaddr_B,B);
    flashwrite_addr400(flashaddr_X_0,X_0);
    Y_0 = Y_0 | 0x400;
    flashwrite_addr400(flashaddr_Y_0,Y_0);
    open_exitPB11();//打开中断
    return 0;
}