/******************************************************************************
*                     (C) Igor Natachanny 2012                                *
*******************************************************************************
*
*  PROJECT     : RON3K 
*
*  FILE        : tim.c
*   
*  PATH        : C:\Igor\MyProjects\Eilon\RON3K\SW\WRK\BSP\
*
*  PURPOSE     : 
*
*  TOOLCHAIN:  : RealView MDK-ARM Version 4.0
*
*  TARGET      : RON2K5 
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2011.05     Igor Natachanny   1.0.0   Initial version.
*
*******************************************************************************
      Copyright and Proprietary Statment.
        
	This source code may be used by my legal customers only. It may not be used 
   copied or transferred in whatever form in whole or in part, to a third 
   party without a written consent. Derivative programs created by my customers 
   using this software in object code form only are not restricted in any way.  

 Igor Natachanny 
 Independent Embedded System Professional.
 igornatachanny@hotmail.com 054-7656857 
*******************************************************************************/
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
  #include "stm32f2xx.h"
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
/* --- PRIVATE_DATA --------------------------------------------------------- */
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  uint32_t flag_tim3 = 0;
  uint32_t flag_tim4 = 0;
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
  static void NVIC_Configuration(uint32_t tim_irq_num);
  static void TIM_Configuration(TIM_TypeDef *TIMx);

/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
//#define DEBUG
/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/*----------------------------------------------------------------------------
  Returns a status of timer3 or timer4.
   1- if expired; 0 - if not expired;  -2 - wrong timer ID.
 *----------------------------------------------------------------------------*/
  int32_t get_tim_status(TIM_TypeDef *TIMx){

    if(TIMx == TIM3)
      return flag_tim3;
    else {
      if(TIMx == TIM4)
        return flag_tim4;
      else
        return(-2);
	}
  }/* get_tim_status */
/*----------------------------------------------------------------------------
  Starts a timer3 or timer4 for deseried time.
  Returns 0- if seccess; -1 - wrong time value; -2 - wrong timer ID.
  TIM3 or TIM4 ; mSec||uSec||Sec;
 *----------------------------------------------------------------------------*/
  int32_t tim_start(TIM_TypeDef *TIMx, uint32_t time, uint32_t units){
   RCC_ClocksTypeDef RCC_ClocksStatus;
   uint32_t cnt_clk = 0x00;
   uint32_t Period = 65536, Prescaler = 0;
   
    if(TIMx == TIM3)
      flag_tim3 = 0;
    else {
      if(TIMx == TIM4)
        flag_tim4 = 0;
      else
        return(-2);
    }  
      
//    time_val = time*units; /* Desiried timer value in Sec */  

    RCC_GetClocksFreq(&RCC_ClocksStatus);
    cnt_clk = 2 * RCC_ClocksStatus.PCLK1_Frequency; /* Timer Counter Clk Frequency (Hz) */
    cnt_clk = cnt_clk / units; 

    do{
       Prescaler++;
       if(Prescaler > 65535)
         return(-1);
       Period = time * cnt_clk / Prescaler;// / units;
    }while(Period > 65535);

  /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = Period;
    TIM_TimeBaseStructure.TIM_Prescaler = Prescaler - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_Configuration(TIMx);

	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);

	return 0;
  }/*** tim_uS_start ***/
/*----------------------------------------------------------------------------
  Returns 1 if wrong timer ID, 0 - if success/ TIM3 or TIM4
 *----------------------------------------------------------------------------*/
  int32_t tim_init(TIM_TypeDef *TIMx){
#if 0
//    DBGMCU->CR |= DBGMCU_TIM3_STOP;
    switch((uint32_t)TIMx){
      case (uint32_t)TIM3:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
        NVIC_Configuration(TIM3_IRQn);
      break;
      case (uint32_t)TIM4:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
        NVIC_Configuration(TIM4_IRQn);
      break;
      default:
        return(1);
    }/*switch(TIMx)*/
	
    TIM_Configuration(TIMx);   

#ifdef DEBUG
    make_gp_out(GPIO_Pin_12, GPIOC);   /* Debug GPIOC Pin_12 as the Test Output.*/
#endif
#endif
    return(0);
  }
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/*----------------------------------------------------------------------------
  TIMx Configuration
*----------------------------------------------------------------------------*/
  void TIM_Configuration(TIM_TypeDef *TIMx){

    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
 
    TIM_ARRPreloadConfig(TIMx, ENABLE);
    TIM_Cmd(TIMx, ENABLE);

#ifdef DEBUG
    TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
#else
    TIM_ITConfig(TIMx, TIM_IT_Update, DISABLE);
#endif

  }/* TIM_Configuration */
/*----------------------------------------------------------------------------
TIMx NVIC enable Interrupt
*----------------------------------------------------------------------------*/
  static void NVIC_Configuration(uint32_t tim_irq_num){
   NVIC_InitTypeDef NVIC_InitStructure;

   /* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

   /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    /* Enable the TIM3 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = tim_irq_num;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

  }/* NVIC_Configuration */

/*----------------------------------------------------------------------------
  TIM_3 Interrupt Service Routine
*----------------------------------------------------------------------------*/
  void TIM3_IRQHandler(void){
    
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update); /* Clear TIM3 update interrupt */
    flag_tim3 = 1;
#ifdef DEBUG
    togle_gpio(GPIO_Pin_12, GPIOC);/* Debug */
#else
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
#endif
  }
/*----------------------------------------------------------------------------
  TIM_4 Interrupt Service Routine
*----------------------------------------------------------------------------*/
  void TIM4_IRQHandler(void){

    TIM_ClearITPendingBit(TIM4, TIM_IT_Update); /* Clear TIM4 update interrupt */
    flag_tim4 = 1;
#ifdef DEBUG
    togle_gpio(GPIO_Pin_12, GPIOC);/* Debug */
#else
    TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
#endif
  } 
/*============================================================================*/
/*                     (C) Copyright Igor Natachanny 2012                     */
/*=========================================================== end_of_file ====*/
/*----------------------------------------------------------------------------
  synop
 *----------------------------------------------------------------------------*/
