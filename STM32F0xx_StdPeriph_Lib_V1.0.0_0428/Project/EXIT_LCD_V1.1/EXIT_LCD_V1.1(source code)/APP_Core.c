//filename: APP_Core.c
//func:
//！

#include "APP_Core.h"
#include "APP_Uart.h"
#include "LCD_lcdinit.h"
#include "LCD_Core.h"

BOOL LED_Status;


void taskSchdule(void)
{
  PhotoElecCheck();
  Input_Scan();
  UartSendData();
  UartTimeout();
}


void LED_Blink(void)
{
  LED_Status  = !LED_Status;
  LED(LED_Status);
}


//光电信号检测
void PhotoElecCheck(void)
{
  INT8U i = 0;
  Input_PhotoElec[0] = PhotoElec_INPUT1;
  
  for(i=0;i<Export_Num;i++)
  {
    //定时器每100ms查询一次,大于3s确定有光电信号输入
    
    if(InputPhotoElec_ONCnt[i] > 30)
    {
      InputPhotoElec_Flag[i] = 1;
    }
    else
    {
      InputPhotoElec_Flag[i] = 0;
    }
  }
}


//func:按键检测
void Input_Scan(void)
{
  INT8U i;
  
  //if(InputScan_Flag == 0)
  {
    //输入扫描
    ExportL[0] = Signal_INPUT1;
    
    //InputScan_Flag = 1;
  }
  
  InputScan_Flag = 1;
  for(i=0;i<Export_Num;i++){
    if (ExportL[i] != ExportH[i]){
      InputScan_Flag = 0;
      break;
    }
  }
  
  for(i=0;i<Export_Num;i++){
    ExportH[i] = ExportL[i];
  }
  
  if(InputScan_Flag == 1)
  {
    InputScanCnt++;
  }
  else{
    InputScanCnt = 0;
  }
  

  
    if(InputScanCnt >= 10)
  {
    for(i=0;i<Export_Num;i++)
    {
      Export[i]=ExportL[i];
      if(Export[i] != 0)
      {
        
        //ExitAddr = (INT16U)(BoardAddr * Export_Num + i + ExitAddrBase);
        //gUartToPC.exitIdL = (INT8U)(ExitAddr & 0x00ff);
        //gUartToPC.exitIdH = (INT8U)((ExitAddr >> 8) & 0xff); //(ExitAddr & 0xff00)>>8;
        
        switch(Export[i])
        {
        case Input_Lock: gUartToPC.status = Signal_Lock;  break;          //锁口
        case Input_Reset: gUartToPC.status = Signal_Reset;break;        //复位
        case Input_Print: gUartToPC.status = Signal_Print;break;        //打印
        default:break;
        }
        break;
      }
      
      if(InputPhotoElec_Flag[i] == 1)//没有按键输入,则判断有无光电信号
      {
        
        //ExitAddr = (INT16U)(BoardAddr * Export_Num + i + ExitAddrBase);
        //gUartToPC.exitIdL = (INT8U)(ExitAddr & 0x00ff);
        //gUartToPC.exitIdH = (INT8U)((ExitAddr >> 8) & 0xff);
        gUartToPC.status = Signal_Lock;                              //锁口
        break;
      }
      
      if(gUartToPC.status == Signal_Lock || Signal_Lock == Signal_Print)
      {
        Output0_ON;
      }
      else Output0_OFF;
    }
  }
}


