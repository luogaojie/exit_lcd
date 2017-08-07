//filename:LCD_Core.c
//func:show_setboardaddr();show_setdirection;show_data();judge_screen1();judge_screen2();


#include "LCD_Core.h"
#include "LCD_lcdinit.h"
#include "LCD_touch.h"
#include "DataType.h"
#include "LCD_Flash.h"
#include "stdio.h"
#include "Common.h"
#include "SysCheck.h"
#include "Cap.h"

u32 state,kilobit,hundred,decade,unit,boardaddr;
u32 screen_flag = 0;
u32 boardaddr = 950;
u32 flashdata;
BOOL setflag;
u32 scshowonce = 0;
u8 input = 0;
u32 w,q,b,s,g;
extern u32 adjust_flag;
extern u16 status_state,weight_HL,numble_HL;
extern u8 en_allrefresh,en_refresh;

extern volatile u8 status_weightL,status_numbleL,status_weightH,status_numbleH,stateL;
u32 x1,x2,x3,x4,y1,y2,y3,y4; 
adjust adjust_xy;
u32 A,B,X_0,Y_0,adjust_flag;
//屏幕1
void show_setboardaddr(u32 data)
{
    SysSelfcheck();
    boardaddr = BoardAddr;
    kilobit = (u16)(((boardaddr-(boardaddr%1000))/1000));
    hundred =(u16)((((boardaddr%1000)-(boardaddr%1000%100))/100));
    decade = (u16)((((boardaddr%1000%100)-(boardaddr%1000%100%10))/10));
    unit =(u16)((boardaddr%1000%100%10));
    
  	BACK_COLOR=RED;
	POINT_COLOR=BLACK;

    LCD_Clear(RED);
    //LCD_Fill(120,0,240,360,RED);//背景框
    LCD_Fill(120,0,122,360,BLACK);
    LCD_Fill(120,106,240,108,BLACK);
    LCD_Fill(120,212,240,214,BLACK);
    
  showhanzi32(10,280,7);//地
  showhanzi32(10,240,8);//址
  show32string(10,240,1,16,":");//
  show32_16(160,10,kilobit);//0
  show32_16(140,10,hundred);//9
  show32_16(120,10,decade);//5
  show32_16(100,10,unit);//0
  showhanzi32(180,260,24);//+
  showhanzi32(180,40,23);//_
  showhanzi32(180,170,5);//确
  showhanzi32(180,130,6);//定
  scshowonce = 2;
}


//屏幕2
void show_setdirection(u8 data)
{
    u8 bo;
    SysSelfcheck();
    BACK_COLOR=RED;
    POINT_COLOR=BLACK;
    LCD_Clear(RED);
    //LCD_Fill(120,0,240,360,RED);//背景框
    LCD_Fill(120,0,122,360,BLACK);
    LCD_Fill(120,106,240,108,BLACK);
    LCD_Fill(120,212,240,214,BLACK);

  if(Board_InOutSide_Flag==1)
  {
    bo=10;
  }
  else if(Board_InOutSide_Flag==0)
  {
    bo=9;
  }
  showhanzi32(0,280,9);//板
  showhanzi32(0,240,10);//方
  showhanzi32(0,200,11);//向
  show32string(0,200,1,16,":");//：
  showhanzi32(0,120,bo);//（内or外）
  //LCD_Fill(120,0,240,360,RED);//背景框
  LCD_Fill(120,0,122,360,BLACK);
  LCD_Fill(120,106,240,108,BLACK);
  LCD_Fill(120,212,240,214,BLACK);
  showhanzi32(180,260,9);//内
  showhanzi32(180,40,10);//外
  showhanzi32(180,170,5);//确
  showhanzi32(180,130,6);//定
}



//屏幕一判断
void judge_screen1(void)
{
    flashdata = *((volatile int *)(flashaddr_board));
    boardaddr = flashdata&0x07ff;
     if(tp_dev.x>=120&&tp_dev.y<=100)
  {
    boardaddr++;
    kilobit = (u16)((boardaddr-(boardaddr%1000))/1000);
    hundred =(u16)(((boardaddr%1000)-(boardaddr%1000%100))/100);
    decade = (u16)(((boardaddr%1000%100)-(boardaddr%1000%100%10))/10);
    unit =(u16)(boardaddr%1000%100%10);
   show32_16(160,10,kilobit);//千
   show32_16(140,10,hundred);//白
   show32_16(120,10,decade);//十
   show32_16(100,10,unit);   //个;
   flashdata = boardaddr | 0x1000;
   flashwrite_addr400(flashaddr_board,flashdata);
   tp_dev.x = 0xffff;
    tp_dev.y = 0xffff;
  }
            
  else if(tp_dev.x>=120&&(tp_dev.y>=110)&&(tp_dev.y<=200))
 {
    //LCD_Clear(WHITE);
    show_setdirection(1);
    screen_flag = 3;
    tp_dev.x = 0xffff;
    tp_dev.y = 0xffff;
  }
            
   else if(tp_dev.x>=120&&tp_dev.y>=210&&tp_dev.y<320)
  {
    
   boardaddr--;
   kilobit = (u16)((boardaddr-(boardaddr%1000))/1000);
   hundred =(u16)(((boardaddr%1000)-(boardaddr%1000%100))/100);
   decade = (u16)(((boardaddr%1000%100)-(boardaddr%1000%100%10))/10);
   unit =(u16)(boardaddr%1000%100%10);
   show32_16(160,10,kilobit);//千
   show32_16(140,10,hundred);//白
   show32_16(120,10,decade);//十
   show32_16(100,10,unit);   //个;
   flashdata = boardaddr | 0x1000;
   flashwrite_addr400(flashaddr_board,flashdata);
   tp_dev.x = 0xffff;
    tp_dev.y = 0xffff;
              
   }
}



//屏幕2判断
void judge_screen2(void)
{

  if(tp_dev.x>=120&&tp_dev.y<=100)
  {
    state=1;
    showhanzi32(0,120,10);  //外
    flashdata = boardaddr| 0x800;
    flashdata = flashdata | 0x1000;
    flashwrite_addr400(flashaddr_board,flashdata);
    tp_dev.x = 0xffff;
    tp_dev.y = 0xffff;
  }
            
  else if(tp_dev.x>=120&&(tp_dev.y>=110)&&(tp_dev.y<=200))
 {
     screen_flag = 4;
    //LCD_Clear(WHITE);
    init_sc();
    setflag = 1;
    flashdata |= 0x1000; //flashdata 13位 置1；
    flashwrite_addr400(flashaddr_board,flashdata);
    initTimer();
    tp_dev.x = 0xffff;
    tp_dev.y = 0xffff;
    
  }
            
   else if(tp_dev.x>=120&&tp_dev.y>=210&&tp_dev.y<300)
  {
   state=0;
    flashdata = boardaddr;
   showhanzi32(0,120,9); //内
   flashdata = flashdata | 0x1000;
   flashwrite_addr400(flashaddr_board,flashdata);
   tp_dev.x = 0xffff;
    tp_dev.y = 0xffff;
   }
}


//初始屏幕判断
void judge_initsc(void)
{
  if((tp_dev.x>=200)&&(tp_dev.x<=240)&&(tp_dev.y>=240)&&(tp_dev.y<=320))
  {
    TIM_DeInit(TIM2);   //定时器deinit；
    en_allrefresh = 0;
    screen_flag = 2;
    show_setboardaddr(950);//设置板地址
    tp_dev.x = 0xffff;
    tp_dev.y = 0xffff;
    
  }
  /*
  else if((tp_dev.x>=200)&&(tp_dev.x<=240)&&(tp_dev.y>=120)&&(tp_dev.y<=200))
  {
    init_sc();//屏幕复位
  }
  */
  else if((tp_dev.x>=200)&&(tp_dev.x<=240)&&(tp_dev.y<=80))
  {
    LCD_Clear(WHITE);
    sc_adjust1();
    screen_flag = 5;//校准界面
    TIM_DeInit(TIM2);
    tp_dev.x = 0xffff;
    tp_dev.y = 0xffff;
    //TP_adjust();//屏幕校准
  }
}


void sc_adjust1(void)
{
   LCD_Clear(WHITE);
    LCD_Fill(0,19,40,21,BLACK);
    LCD_Fill(19,0,21,40,BLACK);
}

void sc_adjust2(void)
{
  LCD_Clear(WHITE);
   LCD_Fill(0,299,40,301,BLACK);
    LCD_Fill(19,280,21,320,BLACK);
}

void sc_adjust3(void)
{
  LCD_Clear(WHITE);
  LCD_Fill(200,299,240,301,BLACK);
  LCD_Fill(219,280,221,320,BLACK); 
}

void sc_adjust4(void)
{
  LCD_Clear(WHITE);
  LCD_Fill(200,19,240,21,BLACK);
  LCD_Fill(219,0,221,40,BLACK); 
}


//校准判断1
void adjust1_judge(void)
{
  if(PEN==0)
  {
    Delay_ms(20);
  if(PEN ==0)
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
        sc_adjust2();
        screen_flag = 6;
  }
  }
}

//校准判断2
void adjust2_judge(void)
{
  if(PEN==0)
  {
    Delay_ms(20);
  if(PEN ==0)
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
        sc_adjust3();
        screen_flag = 7;
  }
  }
}



//校准判断3
void adjust3_judge(void)
{
  if(PEN==0)
  {
    Delay_ms(20);
  if(PEN ==0)
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
        sc_adjust4();
        screen_flag = 8;
  }
  }
}



//校准判断4
void adjust4_judge(void)
{ 
  if(PEN==0)
  {
    Delay_ms(20);
  if(PEN ==0)
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
        A=(adjust_xy.x[0]-adjust_xy.x[3])/200;
        B=(adjust_xy.y[2]-adjust_xy.y[3])/280;
        X_0=adjust_xy.x[3]-20*A;
        Y_0=adjust_xy.y[3]-20*B;
        
        flashwrite_addr400(flashaddr_A,A);
        flashwrite_addr400(flashaddr_B,B);
        flashwrite_addr400(flashaddr_X_0,X_0);
        Y_0 = Y_0 | 0x400;
        flashwrite_addr400(flashaddr_Y_0,Y_0);
        init_sc();
        initTimer();
        screen_flag = 1;
  }
  }
}

//测试硬件
void text(void)
{

  if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0)&&(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))&&(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)))
  {
    input = 0x01;
  }
  else if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5))&&(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0)&&(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)))
  {
    input = 0x03;
  }
  else if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5))&&(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))&&(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0))
  {
    input = 0x02;
  }
  else if((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5))&&(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))&&(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)))
  {
    input = 0x08;
  }
  
  
  
          if(input==0x01)
    {

      show32string(0,160,3,20,"ON ");
      show32string(40,160,3,20,"OFF"); 
      show32string(80,160,3,20,"OFF");
    }
    else if(input==0x02)
      {

      show32string(0,160,3,20,"OFF");
      show32string(40,160,3,20,"ON "); 
      show32string(80,160,3,20,"OFF");
    }
      
      else if(input==0x03)
        {

      show32string(0,160,3,20,"OFF");
      show32string(40,160,3,20,"OFF"); 
      show32string(80,160,3,20,"ON ");
      }
      
        else if(input==0x08)
      {

      show32string(0,160,3,20,"OFF");
      show32string(40,160,3,20,"OFF"); 
      show32string(80,160,3,20,"OFF");
      }
}

//屏幕初始显示
void init_sc(void)
{
    SysSelfcheck();
    BoardAddr = 0;
    Board_InOutSide_Flag = 1;
    close_exitPB11();
    
    
    status_state = 0xff;
    kilobit = (u16)((BoardAddr-(BoardAddr%1000))/1000);
    hundred =(u16)(((BoardAddr%1000)-(BoardAddr%1000%100))/100);
    decade = (u16)(((BoardAddr%1000%100)-(BoardAddr%1000%100%10))/10);
    unit =(u16)(BoardAddr%1000%100%10);
    LCD_Fill(0,0,82,320,BLACK);
    LCD_Fill(200,0,240,320,BLACK);
    BACK_COLOR=BLACK;//字体背景颜色黑
    POINT_COLOR=WHITE;//字体颜色白
    
    showhanzi16(192,0,0); //杭
    showhanzi16(176,0,1); //州
    showhanzi16(160,0,2);//贝
    showhanzi16(144,0,3);//分
    showhanzi16(128,0,4);//科
    showhanzi16(112,0,5);//技
    
    show32_16(16,16,unit);
    show32_16(32,16,decade);
    show32_16(48,16,hundred);
    show32_16(64,16,kilobit);//地址
    
    if(Board_InOutSide_Flag==1)
    {
      showhanzi32(48,20,11); //
      showhanzi32(48,50,10); //外侧
    }
    
    else if(Board_InOutSide_Flag==0)
    {
      showhanzi32(48,20,11);  //内侧
      showhanzi32(48,50,9); //
    }
    
    LCD_Fill(82,0,200,320,LIGHTBLUE);//填充浅蓝色初始屏
    sc_weight(0);//重量刷新
    sc_numble(0);//数量刷新

    showhanzi32(16,288,0);//重
    showhanzi32(16,256,2);//量
    show32string(16,256,1,16,":");//：
    
    showhanzi32(48,288,1);//数
    showhanzi32(48,256,2);//量
    show32string(48,256,1,16,":");//：
    
    
    /*按钮框1*/
    showhanzi32(204,282,3);//设
    showhanzi32(204,250,4);//置
    LCD_DrawRectangle(200,240,240,320);
    LCD_DrawRectangle(204,244,236,316);   
    LCD_DrawLine(200,240,204,244);
    LCD_DrawLine(240,320,236,316);
    LCD_DrawLine(200,320,204,316);
    LCD_DrawLine(240,240,236,244);


    /*按钮框2*/
    /*
    showhanzi32(204,162,18);//复
    showhanzi32(204,130,19);//位
    LCD_DrawRectangle(200,120,240,200);
    LCD_DrawRectangle(204,196,236,124);  
    LCD_DrawLine(200,120,204,124);
    LCD_DrawLine(240,200,236,196);
    LCD_DrawLine(200,200,204,196);
    LCD_DrawLine(240,120,236,124);
    */
    
    /*按钮框3*/
    showhanzi32(204,42,20);//校
    showhanzi32(204,10,21);//准
    LCD_DrawRectangle(200,0,240,80);
    LCD_DrawRectangle(204,4,236,76);  
    LCD_DrawLine(200,0,204,4);
    LCD_DrawLine(240,80,236,76);
    LCD_DrawLine(200,80,204,76);
    LCD_DrawLine(240,0,236,4);
    screen_flag = 1;
    
    
    
    LCD_DrawLine(16,90,80,90);
    LCD_DrawLine(15,0,15,320);//画线
    //datarefresh(status_state);
    //sc_weight(weight_HL);
    //sc_numble(numble_HL);
}


//all数据刷新
void data_allrefresh(void)
{

    datarefresh(stateL);//状态刷新   
    sc_weight(weight_HL);//重量刷新
    sc_numble(numble_HL);//数量刷新
}
//func:刷新状态信息函数
void datarefresh(u8 input)
{

  if(input == Signal_Lock)
  {
    sc_lock();
  }
  
  else if(input == Signal_Print)
  {
    sc_print();
  }
  else if(input == signal_Nomal)
  {
    sc_nomal();
  }
}


//重量变化
void sc_weight(u16 weight)
{
  
  BACK_COLOR=BLACK;
  POINT_COLOR=WHITE;
  w = (weight-weight%10000)/10000;
  q = (weight%10000-weight%10000%1000)/1000;
  b = (weight%10000%1000-weight%10000%1000%100)/100;
  s = (weight%10000%1000%100-weight%10000%1000%100%10)/10;
  g = weight%10000%1000%100%10;
  POINT_COLOR=RED;
  if((w != 0)&&(q != 0)&&(b != 0)&&(s != 0)&&(g != 0))
{
  show32_16(220,16,w);
  show32_16(204,16,q);
  show32_16(188,16,b);
  show32string(16,188,1,16,".");
  show32_16(156,16,s);
  show32_16(140,16,g);
}
    if((w == 0)&&(q != 0))
  {
    LCD_Fill(16,220,48,236,BLACK);
  show32_16(204,16,q);
  show32_16(188,16,b);
  show32string(16,188,1,16,".");
  show32_16(156,16,s);
  show32_16(140,16,g);
  }
  
  if((w == 0)&&(q == 0))
  {
  LCD_Fill(16,220,48,236,BLACK);
  LCD_Fill(16,204,48,220,BLACK);
  show32_16(188,16,b);
  show32string(16,188,1,16,".");
  show32_16(156,16,s);
  show32_16(140,16,g);
  }
  
  
  POINT_COLOR=WHITE;
  show32string(16,124,2,16,"Kg");
  
}

//数量变化
void sc_numble(u16 numble)
{
  BACK_COLOR=BLACK;
  POINT_COLOR=WHITE;
  w = (numble-numble%10000)/10000;
  q = (numble%10000-numble%10000%1000)/1000;
  b = (numble%10000%1000-numble%10000%1000%100)/100;
  s = (numble%10000%1000%100-numble%10000%1000%100%10)/10;
  g = numble%10000%1000%100%10;
  POINT_COLOR=RED;
  if((w != 0)&&(q != 0)&&(b != 0)&&(s != 0)&&(g != 0))
{
  show32_16(204,48,w);
    show32_16(188,48,q);
  show32_16(172,48,b);
  show32_16(156,48,s);
  show32_16(140,48,g);
}
  if((w == 0)&&(q != 0))
  {
    LCD_Fill(48,204,80,220,BLACK);
  show32_16(188,48,q);
  show32_16(172,48,b);
  show32_16(156,48,s);
  show32_16(140,48,g);
  }
  
  if((w == 0)&&(q == 0)&&(b != 0))
  {
    
   LCD_Fill(48,204,80,220,BLACK);
   LCD_Fill(48,188,80,204,BLACK);
  show32_16(172,48,b);
  show32_16(156,48,s);
  show32_16(140,48,g);
  }
  if((w ==0)&&(q == 0)&&(b == 0)&&(s != 0))
  {
   LCD_Fill(48,204,80,220,BLACK);
   LCD_Fill(48,188,80,204,BLACK);
   LCD_Fill(48,172,80,188,BLACK);
  show32_16(156,48,s);
  show32_16(140,48,g);   
  }
  if((w ==0)&&(q == 0)&&(b == 0)&&(s == 0))
  {
   LCD_Fill(48,204,80,220,BLACK);
   LCD_Fill(48,188,80,204,BLACK);
   LCD_Fill(48,172,80,188,BLACK);
   LCD_Fill(48,156,80,172,BLACK);

  show32_16(140,48,g);
  }
  POINT_COLOR=WHITE;
  showhanzi32(48,92,22);//件
}


//关闭变化
void sc_lock(void)
{
  LCD_Fill(82,0,200,320,RED);
  BACK_COLOR=RED;
  POINT_COLOR=BLACK;
  showhanzi64(110,60,6);//中
  showhanzi64(110,140,3);//闭
  showhanzi64(110,220,2);//关
  show32string(126,60,3,16,"...");
}


//打印变化
void sc_print(void)
{
  LCD_Fill(82,0,200,320,YELLOW);
  BACK_COLOR=YELLOW;
  POINT_COLOR=BLACK;
  showhanzi64(110,60,6);//中
  showhanzi64(110,140,1);//印
  showhanzi64(110,220,0);//打
  show32string(126,60,3,16,"...");
  
}


//使用变化
void sc_nomal(void)
{
  LCD_Fill(82,0,200,320,GREEN);
  BACK_COLOR=GREEN;
  POINT_COLOR=BLACK;
  showhanzi64(110,60,6);//中
  showhanzi64(110,140,5);//用
  showhanzi64(110,220,4);//使
  show32string(126,60,3,16,"...");
}


//退出中断
void close_exitPB11(void)
{ 
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0 ;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void open_exitPB11(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0 ;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


 //触摸判断
void setshow(void)
{
  switch(screen_flag)
  {
  case 1:judge_initsc(); break;     //初始判断
  
  case 2:judge_screen1(); break;    //屏幕1判断
  
  case 3:judge_screen2(); break;    //屏幕2判断
  
  case 5:adjust1_judge(); break;    //校准1
   
  case 6:adjust2_judge(); break;    //校准2
  
  case 7:adjust3_judge(); break;    //校准3
  
  case 8:adjust4_judge(); break;    //校准4
  
  default: break;
  }
}


