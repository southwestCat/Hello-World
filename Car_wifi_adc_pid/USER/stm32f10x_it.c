/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "led.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
  
 
s32 rcnt2,rcnt3,rcnt4,rcnt5;
  
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
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
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
void SysTick_Handler(void)
{
}

/*TIM7的中断见timer.c*/
//void TIM7_IRQHandler(void)			//每5ms一次
//{
//  get_encoder_ONE();
//  get_encoder_TWO();
//  get_encoder_THR();
//  get_encoder_FOU();

//  User_PidSpeedControl(150);//PID速度环控制（脉冲每周期）

//  LED0=!LED0; //LED0提示进入中断

//  TIM_ClearITPendingBit(TIM7,TIM_FLAG_Update);

//}


//********************************************TIM2编码器接口计数中断
void TIM2_IRQHandler(void)
{
  if (TIM2 -> CR1 & 0X0010)		  //小心注意
   {
    rcnt2 -= 1;
   }
   else rcnt2 += 1;

   TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
}

//********************************************TIM3编码器接口计数中断
void TIM3_IRQHandler(void)
{
  if (TIM3 -> CR1 & 0X0010)		  //小心注意
   {
    rcnt3 -= 1;
   }
   else rcnt3 += 1;

   TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
}


//********************************************TIM4编码器接口计数中断
void TIM4_IRQHandler(void)
{
  if (TIM4 -> CR1 & 0X0010)		  //小心注意
   {
    rcnt4 -= 1;
   }
   else rcnt4 += 1;
   TIM_ClearITPendingBit(TIM4, TIM_IT_Update); 
}


//********************************************TIM5编码器接口计数中断
void TIM5_IRQHandler(void)
{
  if (TIM5 -> CR1 & 0X0010)		  //小心注意
   {
    rcnt5 -= 1;
   }
   else rcnt5 += 1;
   TIM_ClearITPendingBit(TIM5, TIM_IT_Update); 
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
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


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
