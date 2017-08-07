//filename: HAL_Mcu.c
//func:(...)

#include "stm32f0xx_it.h"
#include "Common.h"
#include "HAL_Mcu.h"
#include "UartService.h"
#include "APP_Core.h"
#include "APP_Uart.h"
#include "LCD_lcdinit.h"
#include "LCD_touch.h"
#include "LCD_Core.h"

u8 test_flag,flag,touch,en_refresh;
u32 refresh_flag;
extern volatile u8 status_weightL,status_numbleL,status_weightH,status_numbleH,stateL,en_allrefresh,ostateL;
extern volatile u16 status_state;
extern volatile u16 weight_HL,numble_HL;

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
 {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */


/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */

void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
	INT8U i;
  if(TIM2->SR & TIM_IT_Update)
  {
    TIM2->SR &= ~TIM_IT_Update;
    TIM2->ARR = 100*500;//64 * 50000; 10ms
	//LED_Blink();

    
    /*****************100ms********************/
    if(test_flag >= 10)
    {
     // LED_Blink();
	for(i=0;i<1;i++)
	{
		if(Input_PhotoElec[i]==0) InputPhotoElec_ONCnt[i]++;
		else InputPhotoElec_ONCnt[i]=0;
	}
    test_flag = 0;
    }
    else test_flag++;
    /************************************/
    
	

    
  }
  
  
}


void SysTick_Handler(void)
{
  gSysFlg0.Bits.TimerIntFlg = 1;
  /*
      if(flag >= 10)
    {
      touch = 1;
      flag = 0;
    }
    
    else 
    {
      flag++;

    }
    */
}

 /* @brief  This function handles TIM4 global interrupt request.
  * @param  None
  * @retval None
  */

 /* @brief  This function handles TIM2 global interrupt request.
  * @param  None
  * @retval None
  */


//串口2中断
void USART2_IRQHandler (void)
{
u32 data;
#if(UART2_EN)
    if(gUart2.UartRxLen < UARTRXBUFFLEN )
    {
        data = USART2->RDR;
    	gUart2.UartRxBuff[gUart2.UartRxLen++] = data;
        
    }
    else
    {
    	USART2->RDR = USART2->RDR; //空读 
        USART2->ICR |= 0xffff;
    }
#else
    data = USART2->RDR; //空读
#endif

  
}


//触摸中断
void EXTI4_15_IRQHandler(void)
{

  if(EXTI_GetFlagStatus(EXTI_Line11)!=RESET)
  {
              //LED_Blink();
              TP_Scan(0);
              setshow();
      EXTI_ClearFlag(EXTI_Line11);
   }
  
}

/******************************************************************************/
/*                 STM32F37x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f37x.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/