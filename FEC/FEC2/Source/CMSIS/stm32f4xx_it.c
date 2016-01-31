/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "main.h"
#include "stm32f4x7_eth.h"

/* Scheduler includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* lwip includes */
#include "lwip/sys.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern xSemaphoreHandle s_xSemaphore;
extern xSemaphoreHandle ETH_link_xSemaphore;
extern __IO uint32_t LsiFreq;
extern __IO uint32_t CaptureNumber, PeriodValue;
uint16_t tmpCC4[2] = {0, 0};
/* Private function prototypes -----------------------------------------------*/
extern void xPortSysTickHandler(void);
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

#pragma optimize=none
__no_init uint32_t register_r0;
__no_init uint32_t register_r1;
__no_init uint32_t register_r2;
__no_init uint32_t register_r3;
__no_init uint32_t register_r12;
__no_init uint32_t register_lr; /* Link register. */
__no_init uint32_t register_pc; /* Program counter. */
__no_init uint32_t register_psr;/* Program status register. */
void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress)
{
    /* These are volatile to try and prevent the compiler/linker optimising them
     away as the variables never actually get used.  If the debugger won't show the
     values of the variables, make them global my moving their declaration outside
     of this function. */
    uint32_t mmfar;
    uint32_t hfsr;
    uint32_t cfsr;

    register_r0 = pulFaultStackAddress[0];
    register_r1 = pulFaultStackAddress[1];
    register_r2 = pulFaultStackAddress[2];
    register_r3 = pulFaultStackAddress[3];

    register_r12 = pulFaultStackAddress[4];
    register_lr = pulFaultStackAddress[5];
    register_pc = pulFaultStackAddress[6];
    register_psr = pulFaultStackAddress[7];

    mmfar = SCB->MMFAR;
    hfsr = SCB->HFSR;
    cfsr = SCB->CFSR;

    /* When the following line is hit, the variables contain the register values. */
    for (;;)
        ;
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
#pragma optimize=none
void HardFault_Handler(void)
{
    __asm volatile
        (
                //" handler2_address_const: .word prvGetRegistersFromStack    \n"
                " tst lr, #4                                                \n"
                " ite eq                                                    \n"
                " mrseq r0, msp                                             \n"
                " mrsne r0, psp                                             \n"
                " ldr r1, [r0, #24]                                         \n"
                " bl prvGetRegistersFromStack \n"
    //            " mov r2, #prvGetRegistersFromStack                            \n"
    //            " bx r2                                                     \n"
        );
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
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  xPortSysTickHandler(); 
}

/**
  * @brief  This function handles External line 10 interrupt request.
  * @param  None
  * @retval None
  */
//void EXTI15_10_IRQHandler(void)
void EXTI9_5_IRQHandler(void)
{
  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

  if(EXTI_GetITStatus(ETH_LINK_EXTI_LINE) != RESET)
  {
    /* Give the semaphore to wakeup LwIP task */
    xSemaphoreGiveFromISR(ETH_link_xSemaphore, &xHigherPriorityTaskWoken ); 
  }
  /* Clear interrupt pending bit */
  EXTI_ClearITPendingBit(ETH_LINK_EXTI_LINE);

  /* Switch tasks if necessary. */	
  if( xHigherPriorityTaskWoken != pdFALSE )
  {
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
  }
}

/**
  * @brief  This function handles ethernet DMA interrupt request.
  * @param  None
  * @retval None
  */
void ETH_IRQHandler(void)
{
  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

  /* Frame received */
  if ( ETH_GetDMAFlagStatus(ETH_DMA_FLAG_R) == SET) 
  {
    /* Give the semaphore to wakeup LwIP task */
    xSemaphoreGiveFromISR( s_xSemaphore, &xHigherPriorityTaskWoken );   
  }

  /* Clear the interrupt flags. */
  /* Clear the Eth DMA Rx IT pending bits */
  ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
  ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);

  /* Switch tasks if necessary. */	
  if( xHigherPriorityTaskWoken != pdFALSE )
  {
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
  }
}

/**
  * @brief  This function handles TIM5 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM5, TIM_IT_CC4) != RESET)
  {
    /* Get the Input Capture value */
    tmpCC4[CaptureNumber++] = TIM_GetCapture4(TIM5);

    /* Clear CC4 Interrupt pending bit */
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);

    if (CaptureNumber >= 2)
    {
      /* Compute the period length */
      PeriodValue = (uint16_t)(0xFFFF - tmpCC4[0] + tmpCC4[1] + 1);
    }
  }
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
