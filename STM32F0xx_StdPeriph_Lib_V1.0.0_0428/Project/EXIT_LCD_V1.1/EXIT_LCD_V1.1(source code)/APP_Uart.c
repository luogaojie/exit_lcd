//filename :APP_Uart.c
//func:

#include "APP_Uart.h"
#include "DataType.h"
#include "LCD_Core.h"

volatile LSK_RX gCarRxFromExit,gCarRxFromEntry,gCarRxFromWaster,gExitRxFromCar,gExitRxPC,gExitRxKey,gWasterRxFromCar,gEntryRxFormCar,gEntryRxFormPc,gEntryRxFormCar,
gEntryRxFormPlc;
LSK_TX gUartToPC;
volatile u16 gUartr2RxCnt,weight_all,numble_all;
volatile u8 status_weightL,status_numbleL,status_weightH,status_numbleH,time_stateL,stateL,ostateL;
volatile u16 status_state;
volatile u16 weight_HL,numble_HL,oweight_HL,onumble_HL;


//func: 串口数据处理（从pc获取）
void UartFromPC(u8 UartId,UART_SOURCE *UartSource)
{
  
  u8 *pData;
  static INT8U i,checkSum;
  LSK_RX uartRx;
  u16 id;
  
  u16 boardaddr_HL;
  ENTER_CRITICAL;
  u16 a;
  
  
  if(UartSource->UartRxBuff[0] == LSK_RX_HEAD0 && UartSource->UartRxBuff[1] == LSK_RX_TYPE)//判断帧头和类型
  {
    
    pData = (BYTE*)&uartRx;
    
    for(i = 0; i < sizeof(LSK_RX);i++)
    {
      *(pData + i) = UartSource->UartRxBuff[i];
    }
    
    pData = (BYTE*)&uartRx;
    
    /*计算校验和*/
    
    checkSum = 0;
    checkSum = CalculateChekSum(pData,(sizeof(LSK_RX) - 1),ADD_SUM) & 0xff;
    
    if(uartRx.status != 0x00)
    {
      id = uartRx.boardaddrL << 8 + uartRx.boardaddrH;
    }
    
    if(uartRx.checkSum == checkSum)                       //校验和符合
    {
      boardaddr_HL = uartRx.boardaddrH*256+uartRx.boardaddrL;
      if(boardaddr_HL == BoardAddr)                   //板地址符合
      {
        
        
        
        //////////////////////////////////////////////////////////////
        /*数据处理*/
        
        stateL = uartRx.state&0x0f;            //new状态
        a=uartRx.weightH*256;
        weight_HL = a+ uartRx.weightL;         //重量           
        a=uartRx.numbleH*256;
        numble_HL=a+uartRx.numbleL;            //数量      
        
        ostateL = status_state&0x0f;            //old状态
        a=status_weightH*256;
        oweight_HL = a+ status_weightL;         //重量           
        a=status_numbleH*256;
        onumble_HL=a+status_numbleL;            //数量 
        
        if(stateL != ostateL)
        {
            datarefresh(stateL);//状态刷新   
        }
        
        if(weight_HL != oweight_HL)
        {
 
           sc_weight(weight_HL);//重量刷新

        }
        
        if(numble_HL != onumble_HL)
        {

           sc_numble(numble_HL);//数量刷新
        }
        /*迭代数据*/
       
        status_state = uartRx.state;
        status_weightL = uartRx.weightL;
        status_weightH = uartRx.weightH;
        status_numbleL = uartRx.numbleL;
        status_numbleH = uartRx.numbleH;
        
        gUartToPC.boardaddrH = uartRx.boardaddrH;
        gUartToPC.boardaddrL = uartRx.boardaddrL;
        gUartToPC.numbleH = uartRx.numbleH;
        gUartToPC.numbleL = uartRx.numbleL;
        gUartToPC.weightH = uartRx.weightH;
        gUartToPC.weightL = uartRx.weightL;
        /////////////////////////////////////////////////////////////
        PC_Check_Flag = 1;
        for(i = 0; i < sizeof(LSK_RX);i++)
        {
          UartSource->UartRxBuff[i] = 0;
        }
        
        
        
        
        
      }
    }
  }
}


//func：方向判断
void DirectionInit(void)
{
  
  if(Board_InOutSide_Flag == 0)//内侧
  {
    gUartToPC.state = stateL;
  }
  else if(Board_InOutSide_Flag == 1)//外侧
  {
    gUartToPC.state = stateL | 0x10;
  }
}


//func:数据处理（到PC）
void uartToPC(void)
{   
  
  //LSK_TX uartTx;
  u8 i = 0;
  LSK_TX uartTx;
  ENTER_CRITICAL;
  DirectionInit();
  
  uartTx.packetHead0    = LSK_TX_HEAD0;
  uartTx.dataType       = LSK_TX_TYPE;
  uartTx.boardaddrL     = gUartToPC.boardaddrL;
  uartTx.boardaddrH     = gUartToPC.boardaddrH;
  uartTx.numbleL        = gUartToPC.numbleL;
  uartTx.numbleH        = gUartToPC.numbleH;
  uartTx.weightL        = gUartToPC.weightL;
  uartTx.weightH        = gUartToPC.weightH;
  
  
  uartTx.state          = gUartToPC.state;
  uartTx.status         = gUartToPC.status;
  gUartToPC.status      = 0;
  
  /*计算校验和*/
  uartTx.checkSum = 0;
  for(i = 0;i < (LSK_TX_LEN - 1);i++)
  {
    uartTx.checkSum += *((BYTE*)&uartTx + i);
  }
  EXIT_CRITICAL;
  
  UartSend(UART2,(BYTE*)&uartTx.packetHead0,LSK_TX_LEN);//发送数据
  
  
}


//func:发生数据函数
void UartSendData(void)
{
  if(gUartr2RxCnt <= 3)
  {
    gUartr2RxCnt++;
    if(PC_Check_Flag == 1)
    {
      UART2_TX_EN;
      uartToPC();
      PC_Check_Flag = 0;
      
    }
  }
  else if((gUartr2RxCnt>3)&&(gUartr2RxCnt<=9))
  {
    gUartr2RxCnt++;
    UART2_RX_EN;
  }
  else gUartr2RxCnt = 0;
}

//func：获取串口数据
void UartReadDada(void)
{
  UART_SOURCE *pUartSource;
  pUartSource = &gUart2;
  if(gUart2.UartRxLen>= LSK_RX_LEN)
  {
    UartFromPC(EXIT_PC_UART,pUartSource);
  }
}





//func：串口超时处理函数
void UartTimeout(void)
{
#define UART2_TIMEOUT_CNT  (5)
  
  if(gUart2.UartRxLen) //已经收了1个字节
  {
    gUart2.gUartTimeOutCnt++;
    if(gUart2.gUartTimeOutCnt > UART2_TIMEOUT_CNT) 	//如果超过一秒认为超时，可设 
    {
      gUart2.UartRxLen = 0;			//UART接收缓冲计数器清零
      gUart2.gUartTimeOutCnt = 0;		//数据包超时计数器清零
    }	
  }
  
  
}
