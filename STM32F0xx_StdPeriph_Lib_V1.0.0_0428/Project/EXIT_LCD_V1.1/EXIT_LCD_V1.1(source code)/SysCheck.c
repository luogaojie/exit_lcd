//filename: SysCheck.c
//func:SysSelfcheck();


#include "SysCheck.h"
#include "LCD_Core.h"
#include "stm32f0xx.h"
#include "Common.h"
#include "LCD_flash.h"

extern u32 screen_flag;
extern u32 boardaddr;
extern u32 flashdata;
extern BOOL setflag;
extern u32 scshowonce;
u32 data;

void SysSelfcheck(void)
{
     
     data = *((volatile u32 *)(flashaddr_flag));  //读数据
     setflag = (data&0x01);
    
    
   if(setflag != 0)
   {
     flashwrite_addr400(flashaddr_board,950);       //0x13b6 设置地址为950，方向内侧，已设置标志位为1
     flashwrite_addr400(flashaddr_flag,0);
   }
   
    flashdata = *((volatile u32 *)(flashaddr_board));  //读数据
    BoardAddr = flashdata&0x07ff;                 //地址
    boardaddr = BoardAddr;
    Board_InOutSide_Flag = (flashdata&0x800)>>11; //内外侧
    
}
