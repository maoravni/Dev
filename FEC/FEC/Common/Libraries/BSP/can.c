/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     :  
*
*  FILE        : can.c 
*   
*  PATH        :  
*
*  PURPOSE     : 
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.40.1.3812
*
*  TARGET      :  
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.01     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
*      Copyright and Proprietary Statment.
* 
*******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
#ifdef STM32F4XX
#include <stm32f4xx.h>
#else
#include <stm32f2xx.h>
#endif

#include "ll_typedef.h"
#include "FreeRtosConfig.h"
#include <logger.h>

/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */
/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */

/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
#define CAN1_MCG_READY  0x0002
#define CAN2_MCG_READY  0x0004
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
/* --- PRIVATE_DATA --------------------------------------------------------- */
  static uint16_t  can_flags=0;
//  extern CanTxMsg TxMessage1;
  CanRxMsg RxMessage1;
//  extern CanTxMsg TxMessage2;
  CanRxMsg RxMessage2;
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
  uint32_t rcv_can(CAN_TypeDef* CANx){
   uint32_t res = ERROR;
    
    switch((uint32_t)CANx){ 
      case (uint32_t)CAN1:
        if(can_flags & CAN1_MCG_READY){
           can_flags &=~CAN1_MCG_READY;
//           M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"CAN_1 receives  = %s", ((uint8_t*)&(RxMessage1.Data[0])));
           res = OK;
       }
      break;
      
      case (uint32_t)CAN2:
        if(can_flags & CAN2_MCG_READY){
          can_flags &=~CAN2_MCG_READY;
//          M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"CAN_2 receives  = %s", ((uint8_t*)&(RxMessage2.Data[0])));
          res = OK;
        }
      break;
    }   
    return res;
  }/* uint32_t rcv_can */
  
  void Rcv_CANS(){
    
     if(can_flags & CAN1_MCG_READY){
     can_flags &=~CAN1_MCG_READY;
     M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE,"CAN_1 received  = %s", ((uint8_t*)&(RxMessage1.Data[0])));
   }
   
   if(can_flags & CAN2_MCG_READY){
     can_flags &=~CAN2_MCG_READY;
     M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE,"CAN_2 received  = %s", ((uint8_t*)&(RxMessage2.Data[0])));
   }
   
  }   
  
void Init_CAN(void){


GPIO_InitTypeDef  GPIO_InitStructure;
CAN_InitTypeDef        CAN_InitStructure;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;

  NVIC_InitTypeDef NVIC_InitStructure; 
  
 // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); already done in board setup
  
     /* Enable the CAN1  Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (uint8_t)(configMAX_SYSCALL_INTERRUPT_PRIORITY>>4)+2;  //mast be above configMAX_SYSCALL_INTERRUPT_PRIORITY (11)
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
    /* Enable the CAN2  Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (uint8_t)(configMAX_SYSCALL_INTERRUPT_PRIORITY>>4)+2;  //mast be above configMAX_SYSCALL_INTERRUPT_PRIORITY (11)
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* CAN GPIOs configuration **************************************************/

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOB, ENABLE);
  
   
  /* Connect CAN1 pins to AF9 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1); 
  
  /* Connect CAN2 pins to AF9 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN2); 
  
  /* Configure CAN1 RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
   /* Configure CAN2 RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* CAN configuration ********************************************************/  
  /* Enable CAN clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);
  
  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_DeInit(CAN2);
  CAN_StructInit(&CAN_InitStructure);
  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    
  /* CAN Baudrate = 1MBps (CAN clocked at 30 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
  CAN_InitStructure.CAN_Prescaler = 2;
  CAN_Init(CAN1, &CAN_InitStructure);
  
  CAN_Init(CAN2, &CAN_InitStructure);
  
  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0;         
  CAN_FilterInitStructure.CAN_FilterIdLow = 0;           
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;      
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;       
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
   
  
  CAN_FilterInitStructure.CAN_FilterNumber = 14;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  

  
  /* Enable FIFO 0 message pending Interrupt */
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
 
  CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
}
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
  void CAN1_RX0_IRQHandler(void)
{
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage1);
  
 if ((RxMessage1.StdId == 0x321)&&(RxMessage1.IDE == CAN_ID_STD) && (RxMessage1.DLC == 8))
 {
    
      can_flags |= CAN1_MCG_READY;
   
 }
}



void CAN2_RX0_IRQHandler(void)
{
  CAN_Receive(CAN2, CAN_FIFO0, &RxMessage2);
  
 if ((RxMessage2.StdId == 0x321)&&(RxMessage2.IDE == CAN_ID_STD) && (RxMessage2.DLC == 8))
 {
    
      can_flags |=CAN2_MCG_READY;
   
 }
}
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
/**
  * @brief: 
  *
  * @param:      
  *
  * @retval: 
**/
