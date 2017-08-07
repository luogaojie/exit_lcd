//filename:LCD_flash.c
//func:flashwrite(u32 data) 


#include "stm32f0xx.h"
#include "LCD_flash.h"
#include "DataType.h"


__IO FLASH_Status FLASHStatus = FLASH_COMPLETE;


//写数据(erase a page ,write 4 bytes)
u8 flashwrite_addr400(u32 addr,u32 data)
{
  u32 flashcheck;
  flashcheck = *((volatile u32 *)addr);
  if(flashcheck == data)  return 0;
  FLASH_Unlock();
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
  FLASH_ErasePage(addr);

  FLASH_ProgramWord(addr, data);

  FLASH_Lock(); 
  return 0;
}
