//filename:APP_Main.c
//func:main()

#include "APP_Main.h"
#include "uart.h"
#include "LCD_touch.h"
#include "watchDog.h"
#include "LCD_Core.h"
#include "SysCheck.h"
#include "LCD_flash.h"
#include "APP_Uart.h"
#include "APP_Core.h"
#include "LCD_lcdinit.h"





void main(void)
{
  
	InitSys();      //系统初始化
	initUart();     //再次串口初始化
    //flashwrite_addr400(flashaddr_board,950);
    //flashwrite_addr400(flashaddr_Y_0,0);
    TP_adjust();    //触摸屏校准,第一次校准一次
    init_sc();//状态初始化
    
    
	LED_ON;
	Delay_ms(200);
	LED_OFF;
    Delay_ms(200);
    LED_ON;

	
#if(WATCHDOG_EN)
        enWatchdog();
#endif

while(1)
  {

      //text();  //测试用

      EALLOW;
      
      if(gSysFlg0.Bits.TimerIntFlg)
      {
  #if(WATCHDOG_EN)
      CLRWD;
  #endif
      
      ENTER_CRITICAL;
      UartReadDada();
      EXIT_CRITICAL;
      taskSchdule();
      
      gSysFlg0.Bits.TimerIntFlg = 0;
      
      }
      EDIS;
      

  }
}
