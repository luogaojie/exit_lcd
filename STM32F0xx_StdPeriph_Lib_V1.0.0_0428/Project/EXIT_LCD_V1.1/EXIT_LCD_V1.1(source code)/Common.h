//filename:Common.h

#ifndef _COMMON_H
#define _COMMON_H

#include "stm32f0xx.h"
#include "DataType.h"
#include "math.h"
#include "MyMisc.h"


//Function Operation Return 
#define		SUCCEED		0
#define		FAILED		1
//BOOL 
#define		TRUE		1
#define		FALSE		0	

#define  LSI_CLK (42000) 

#define UART_EN         1
#define PWM_EN          0
#define AD7328_EN       0
#define ADG_EN          0
#define ADG1414_EN      0
#define LS_CODE_EN      0
#define WATCHDOG_EN     1

#define TEST_URAT 0

//mak code define
#define BIT0    0x0001
#define BIT1    0x0002
#define BIT2    0x0004
#define BIT3    0x0008
#define BIT4    0x0010
#define BIT5    0x0020
#define BIT6    0x0040
#define BIT7    0x0080
#define BIT8    0x0100
#define BIT9    0x0200
#define BIT10   0x0400
#define BIT11   0x0800
#define BIT12   0x1000
#define BIT13   0x2000
#define BIT14   0x4000
#define BIT15   0x8000


#define  SYS_CLK (64)
//Define I/O process
#define SETBIT(X,MASK)		(X |= MASK)		
#define CLRBIT(X,MASK)      (X &= ~MASK)
#define CHKBIT(X,MASK)		(X & MASK) 
//Define memory process
#define SETBITONE(X,MASK)		(X |= MASK)
#define SETBITZERO(X,MASK)     (X &= ~MASK)
#define CHECKVALUE(X,MASK)	(X & MASK)

//*********************WatchDog************************//
#define		CLRWD	{IWDG_ReloadCounter();}
#define  ENTER_CRITICAL  { __ASM volatile ("cpsid i"); }
#define  EXIT_CRITICAL   { __ASM volatile ("cpsie i"); }

#define EALLOW  ;
#define EDIS    ;
//*********************UART***************************//
#if(UART_EN)

  //#define UART1_TX_EN    GPIO_SetBits(GPIOA, GPIO_Pin_1); 
  //#define UART1_RX_EN    GPIO_ResetBits(GPIOA, GPIO_Pin_1); 

  #define UART_TX_BUFF1 (INT16U*)&USART1->TDR
  #define UART_TX_FULL1	(INT16U*)&USART1->ISR
  #define UART_RX_BUFF1	(INT16U*)&USART1->RDR
  #define UART_RX_EMPT1	(INT16U*)&USART1->ISR

  #define UART2_TX_EN    GPIO_SetBits(GPIOA, GPIO_Pin_1); 
  #define UART2_RX_EN    GPIO_ResetBits(GPIOA, GPIO_Pin_1); 
  #define UART_TX_BUFF2 (INT16U*)&USART2->TDR
  #define UART_TX_FULL2	(INT16U*)&USART2->ISR
  #define UART_RX_BUFF2	(INT16U*)&USART2->RDR
  #define UART_RX_EMPT2	(INT16U*)&USART2->ISR

//  #define UART3_TX_EN    GPIO_SetBits(GPIOE, GPIO_Pin_15);
 // #define UART2_RX_EN    GPIO_ResetBits(GPIOE, GPIO_Pin_15); 
  #define UART_TX_BUFF3 (INT16U*)&USART3->TDR
  #define UART_TX_FULL3	(INT16U*)&USART3->ISR
  #define UART_RX_BUFF3	(INT16U*)&USART3->RDR
  #define UART_RX_EMPT3	(INT16U*)&USART3->ISR

#else
  #define UART_TX_BUFF1 &hardWireReg
  #define UART_TX_FULL1	&hardWireReg
  #define UART_RX_BUFF1 &hardWireReg
  #define UART_RX_EMPT1 &hardWireReg

  #define UART_TX_BUFF2 &hardWireReg
  #define UART_TX_FULL2	&hardWireReg
  #define UART_RX_BUFF2 &hardWireReg
  #define UART_RX_EMPT2 &hardWireReg

  #define UART_TX_BUFF3 &hardWireReg
  #define UART_TX_FULL3	&hardWireReg
  #define UART_RX_BUFF3 &hardWireReg
  #define UART_RX_EMPT3 &hardWireReg
#endif

#define LED_ON   GPIO_ResetBits(GPIOB, GPIO_Pin_7); 
#define LED_OFF  GPIO_SetBits(GPIOB, GPIO_Pin_7); 
#define LED(sta)  sta? GPIO_SetBits(GPIOB,GPIO_Pin_7):GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define LED_FLASH {LED_ON;Delay(10000);LED_OFF;}


//输出宏定义
#define Output0_ON  GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define Output0_OFF GPIO_SetBits(GPIOB,GPIO_Pin_8)

//扫描外部输入信号类型
#define Input_Lock 0x04    //锁口输入
#define Input_Reset 0x02   //复位输入
#define Input_Print 0x01   //打印输入



//信号状态
#define Signal_PhotoElec 0x01  //光电信号
#define Signal_Reset     0x02  //复位信号
#define Signal_Print     0x04  //打印信号
#define Signal_Lock      0x08  //锁口信号
#define Signal_Error     0x40  //错误信号
#define signal_Nomal     0x00  //正常信号

#define ReadGpio(io,pin)  GPIO_ReadInputDataBit(io,pin)
#define ExitGPIO(gpio1,pin1,gpio2,pin2,gpio3,pin3)   ~(ReadGpio(gpio1,pin1)<<2|ReadGpio(gpio2,pin2)<<1|ReadGpio(gpio3,pin3))&0x07

#define Export_Num 1  //出口的数量

#define ExitAddrBase 950  //出口的基地址

#define PC_TO_PRINT_ON 0x08//PC发送打印信号
#define PC_TO_PRINT_OFF 0x04//PC发送取消打印信号


//flash地址宏定义
#define flashaddr_board 0x08007c00     //板地址，方向，是否已初始设置950标志位
#define flashaddr_A     0x08006c00     //触摸校准数据A
#define flashaddr_B     0x08007000     //触摸校准数据B
#define flashaddr_X_0   0x08007400     //触摸校准数据X_0
#define flashaddr_Y_0   0x08007800     //触摸校准数据Y_0
#define flashaddr_flag   0x08006800     //触摸校准数据Y_0

//定义系统标志
typedef union
{
	struct
	{
		INT16U TimerIntFlg:1;	//定时器中断
		INT16U SysStart:1;
		INT16U  BsEn:1;         //允许接收LS
		INT16U  sysDebugEn : 1;  //测试允许
	}Bits;
	INT16U All;
}SYS_FLAG0;

//时钟节拍定义
typedef struct
{
	INT32U	carSleepTicks;
    INT16U  led;
}TICKS;



extern volatile SYS_FLAG0 gSysFlg0;	//系统标志0
extern volatile TICKS gTicks;	//软件时钟节拍


extern volatile  INT8U gEntryTxEn;

extern BOOL LED_Status;//标记LED 状态
extern INT16U ExitAddr ;//出口板实际地址
extern INT16U ExitId ;//出口相对目标板地址

extern BOOL InputScan_F;//输入扫描状态标志位
extern INT8U Export[1];//定义一个数组储存13个端口的状态值
extern INT16U BoardAddr;//出口板的地址

extern BOOL Board_InOutSide_Flag;//板子的内外侧标志  0:内侧  1:外侧
extern BOOL InputScan_Flag ;//输入扫描

extern INT8U Input_PhotoElec[1];//存储13个光电信号输入
extern INT8U InputPhotoElec_ONCnt[1];//存储13个光电信号持续的时间
extern INT8U InputPhotoElec_Flag[1];//光电输入信号标志位
extern INT8U InputScanCnt ;//输入扫描计数
extern INT8U ExportL[1],ExportH[1];//寄存输入信号


extern BOOL PC_Check_Flag;  //PC查询标志位


#endif

