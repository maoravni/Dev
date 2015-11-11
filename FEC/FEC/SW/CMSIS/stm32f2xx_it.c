/**
 ******************************************************************************
 * @file    stm32f2xx_it.c
 * @author  MCD Application Team
 * @version V1.1.0
 * @date    07-October-2011
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
#include "stm32f2xx_it.h"
#include "ll_typedef.h"
#include "main.h"
#include "stm32f2x7_eth.h"

/* Scheduler includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* lwip includes */
#include "lwip/sys.h"

//#include "usb_core.h"
//#include "usbd_core.h"
//#include "usbd_cdc_core.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t tmpCC4[2] =
{ 0, 0 };
extern __IO uint32_t TimingDelay;
extern __IO uint32_t PeriodValue;
extern __IO uint32_t CaptureNumber;
extern xSemaphoreHandle s_xSemaphore;

//extern USB_OTG_CORE_HANDLE USB_OTG_dev;
//extern uint32_t USBD_OTG_ISR_Handler(USB_OTG_CORE_HANDLE *pdev);
/* Private function prototypes -----------------------------------------------*/
extern void xPortSysTickHandler(void);
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

//void OTG_FS_WKUP_IRQHandler(void)
//{
//    if (USB_OTG_dev.cfg.low_power)
//    {
//        *(uint32_t *) (0xE000ED10) &= 0xFFFFFFF9;
//        SystemInit();
//        USB_OTG_UngateClock(&USB_OTG_dev);
//    }
//    EXTI_ClearITPendingBit(EXTI_Line18 );
//}
/**
 * @brief  This function handles OTG_HS Handler.
 * @param  None
 * @retval None
 */

//void OTG_FS_IRQHandler(void)
//{
//    USBD_OTG_ISR_Handler(&USB_OTG_dev); //Igor 29/10/2012
//}

/**
 * @brief  This function handles TIM5 global interrupt request.
 * @param  None
 * @retval None
 */
void TIM5_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM5, TIM_IT_CC4 ) != RESET)
    {
        /* Get the Input Capture value */
        tmpCC4[CaptureNumber++] = TIM_GetCapture4(TIM5 );

        /* Clear CC4 Interrupt pending bit */
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC4 );

        if (CaptureNumber >= 2)
        {
            /* Compute the period length */
            PeriodValue = (uint16_t)(0xFFFF - tmpCC4[0] + tmpCC4[1] + 1);
        }
    }
}

/**
 * @brief   This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

void prvGetRegistersFromStack(uint32_t *pulFaultStackAddress)
{
    /* These are volatile to try and prevent the compiler/linker optimising them
     away as the variables never actually get used.  If the debugger won't show the
     values of the variables, make them global my moving their declaration outside
     of this function. */
    volatile uint32_t r0;
    volatile uint32_t r1;
    volatile uint32_t r2;
    volatile uint32_t r3;
    volatile uint32_t r12;
    volatile uint32_t lr; /* Link register. */
    volatile uint32_t pc; /* Program counter. */
    volatile uint32_t psr;/* Program status register. */

    r0 = pulFaultStackAddress[0];
    r1 = pulFaultStackAddress[1];
    r2 = pulFaultStackAddress[2];
    r3 = pulFaultStackAddress[3];

    r12 = pulFaultStackAddress[4];
    lr = pulFaultStackAddress[5];
    pc = pulFaultStackAddress[6];
    psr = pulFaultStackAddress[7];

    /* When the following line is hit, the variables contain the register values. */
    for (;;)
        ;
}
/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
//static void HardFault_Handler( void ) __attribute__( ( naked ) );
void HardFault_Handler(void)
{
    //  TBD should be added only wgen the syslog will be ready
    // the TERMINAL use malloc function which not allowd during interrupt
    ///M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_ERROR,"ERROR: HardFault_Handler");
    /* Go to infinite loop when Hard Fault exception occurs */
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
    //  TBD should be added only wgen the syslog will be ready
    // the TERMINAL use malloc function which not allowd during interrupt
    //M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_ERROR,"ERROR: MemManage_Handler");
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
    //  TBD should be added only wgen the syslog will be ready
    // the TERMINAL use malloc function which not allowd during interrupt
    //M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_ERROR,"ERROR: BusFault_Handler");
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
    //  TBD should be added only wgen the syslog will be ready
    // the TERMINAL use malloc function which not allowd during interrupt
    //M_LOGGER_LOGF_UNSORTED(M_LOGGER_LEVEL_ERROR,"ERROR: UsageFault_Handler");
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
//void SVC_Handler(void)
//{
//}
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
//void PendSV_Handler(void)
//{
//}
/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
    xPortSysTickHandler();
}
//#if 0
/**
 * @brief  This function handles External line 10 interrupt request.
 * @param  None
 * @retval None
 */
void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(ETH_LINK_EXTI_LINE ) != RESET)
    {
        Eth_Link_ITHandler(DP83848_PHY_ADDRESS);
        /* Clear interrupt pending bit */
        EXTI_ClearITPendingBit(ETH_LINK_EXTI_LINE );
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
    if (ETH_GetDMAFlagStatus(ETH_DMA_FLAG_R ) == SET)
    {
        /* Give the semaphore to wakeup LwIP task */
        xSemaphoreGiveFromISR( s_xSemaphore, &xHigherPriorityTaskWoken);
    }

    /* Clear the interrupt flags. */
    /* Clear the Eth DMA Rx IT pending bits */
    ETH_DMAClearITPendingBit(ETH_DMA_IT_R );
    ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS );

    /* Switch tasks if necessary. */
    if (xHigherPriorityTaskWoken != pdFALSE)
    {
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken);
    }
}
//#endif
/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f2xx.s).                                               */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
 {
 }*/

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
