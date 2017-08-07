//filename: Uartservice.c
//func: UartSend()

#include	"UartService.h"

#if	UART1_EN
UART_SOURCE gUart1;
#endif

#if	UART2_EN
UART_SOURCE gUart2;
#endif

#if	UART3_EN
UART_SOURCE gUart3;
#endif


//func:串口发生服务函数
void UartSend(INT8U UartId,BYTE* pData,INT8U Lenth)
{
	volatile INT16U* UartTxBuff;
	volatile INT16U* UartTxFull;

		
	switch(UartId)
	{
		case UART1:
		{
#if UART1_EN
			UartTxBuff = UART_TX_BUFF1;
			UartTxFull = UART_TX_FULL1;
#endif
			break;
		}
                case UART2:
		{
#if UART2_EN
			UartTxBuff = UART_TX_BUFF2;
			UartTxFull = UART_TX_FULL2;
#endif
			break;
		}


		default: break;
	}

	while(Lenth)  //数据是否发完
	{
        while(!((*UartTxFull) & 0x80))	//发送BUFF或FIFO是否满
		{
			;
		}

		*UartTxBuff = *pData++;
		Lenth--;
	}
}


