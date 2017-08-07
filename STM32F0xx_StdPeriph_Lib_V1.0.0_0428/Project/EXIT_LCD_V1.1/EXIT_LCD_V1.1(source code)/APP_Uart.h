//filename: APP_Uart.h

#ifndef _APP_UART_H
#define _APP_UART_H

#include "Common.h"
#include "UartService.h"


#define PC_UART			UART2
#define ENTRY_PC_UART       UART2
#define EXIT_PC_UART     UART2


//********************************************
//************UART数据包定义****************
//********************************************
#define	FROM_PC_HEAD0 0xAA
#define	FROM_PC_HEAD1 0xCC
#define FROM_PC_HEAD2 0x06

#define FROM_PC_TYPE 0x07   //数据类别

#define InSide_Close 0x00
#define InSide_Open  0x80

#define OutSide_Close 0x20
#define OutSide_Open  0x30




//
//用户设置数据
//
#define LSK_RX_LEN 11 
#define LSK_RX_HEAD0 0xAA

#define LSK_RX_TYPE 0x01

#define LSK_TX_LEN 11
#define LSK_TX_HEAD0 0xAA
#define LSK_TX_TYPE 0x07



typedef	struct LSK_TX
{
    BYTE	  packetHead0;			//帧头    		1字节     ；1
    BYTE      dataType;      	    //数据类型  	1字节    ；2
    BYTE      boardaddrL;              //出口板地址  2字节	   ； 3
    BYTE      boardaddrH;              // 							: 4
    BYTE      numbleL;         		//数量地址    2字节，; 5，6
    BYTE      numbleH;             //            		
    BYTE      weightL;             // 重量地址            // 7，8
    BYTE      weightH;      			//
    BYTE      state;				//	方向和运行状态			:9
    BYTE      status;				//信号状态				:10
    BYTE	  checkSum;				// 校验字节  	1字节       //11
}LSK_TX;

typedef	struct LSK_RX 
{
    BYTE	  packetHead0;			//帧头    		1字节     ；1
    BYTE      dataType;      	    //数据类型  	1字节    ；2
    BYTE      boardaddrL;              //出口板地址  2字节	   ； 3
    BYTE      boardaddrH;              // 							: 4
    BYTE      numbleL;         		//数量地址    2字节，; 5，6
    BYTE      numbleH;             //            		
    BYTE      weightL;             // 重量地址            // 7，8
    BYTE      weightH;      			//
    BYTE      state;				//	方向和运行状态			:9
    BYTE      status;				//信号状态				:10
    BYTE	  checkSum;				// 校验字节  	1字节       //11
}LSK_RX;

extern LSK_RX gCarRx,gPlatRx;

extern LSK_TX gUartToPC;


void UartFromPC(INT8U UartId,UART_SOURCE *UartSource);
void uartToPC(void);
void UartSendData(void);
void UartReadDada(void);
void UartTimeout(void);
void DirectionInit(void);

#endif