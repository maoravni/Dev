/******************************************************************************
 *                     (C) LandaLabs 2012                                      *
 *******************************************************************************
 *
 *  PROJECT     :  BSP_LIB
 *
 *  FILE        : iwdg,c
 *
 *  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\Libraries\BSP
 *
 *  PURPOSE     :
 *
 *  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
 *
 *  TARGET      :
 *
 *
 *  DATE        NAME              VER.    COMMENTS
 * -----------------------------------------------------------------------------
 *  2012.09     Igor Natachanny   1.1.0   check iwdg reset occur added
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
#include "iwdg_cnfg.h"
#include "ll_typedef.h"
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */__IO uint32_t TimingDelay = 0;
__IO uint32_t LsiFreq = 0;
__IO uint32_t CaptureNumber = 0, PeriodValue = 0;
/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */
/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */

/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
uint32_t GetLSIFrequency(void);

/* --- PRIVATE_DATA --------------------------------------------------------- */
static TASK_ALIVE_FLAGS_UT task_alive;
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/**
 * @brief:  Check if the system has resumed from IWDG reset.
 *
 * @param: none.
 *
 * @retval: TRUE - if IWDG reset was occured, FALSE - otherwise.
 **/
uint32_t was_iwdg_reset_occured()
{

    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST ) != RESET)
    {
        return TRUE; /* IWDGRST flag set */
    }
    else
    {
        return FALSE; /* IWDGRST flag is not set */
    }
}/******************** was_iwdg_reset_occured ******************************/

/**
 * @brief: Checks FORCE_IWDG_BIT into task_alive.word_32.
 *
 * @param: none.
 *
 * @retval: TRUE - if it is set, FALSE - otherwise.
 **/

uint32_t is_force_iwdg_requested()
{

    if (task_alive.tasks.force_iwdg)
        return FALSE; //  HW (WD) reset is requested
    else
        return TRUE; // no HW (WD)  reset is requested

}/********************** is_force_iwdg_requested() **************************/

/**
 * @brief: Forces IWDG by setting FORCE_IWDG_BIT into task_alive.word_32.
 *
 * @param: none.
 *
 * @retval: none
 **/
void iwdg_force()
{

    task_alive.word_32 &= ~FORCE_IWDG_BIT;

}/************************** void iwdg_force() ******************************/

/**
 * @brief: Checks the values of task alive flags.
 *
 * @param: none
 *
 * @retval: true - if all flags are sets, false - if at least one flag is reset.
 **/
uint32_t is_all_task_alive()
{
//    uint32_t tmp = 0,tmp2 = MASK_WD_WORD;
//    tmp = task_alive.word_32 & MASK_WD_WORD;
//#if 0    
    if ((task_alive.word_32 & MASK_WD_WORD)==MASK_WD_WORD)
        return TRUE;
    else
        return FALSE;
//#endif    
#if 0
    uint32_t tmp = 1, res = FALSE;

    while(tmp < 2^IDLE_ALV_E)
    {
        if(!(task_alive.word_32 & tmp))
        break;
        else
        tmp <<= 1;
    }

    if(tmp == (2^IDLE_ALV_E))
    res = TRUE;

    return res;
#endif     
}/********************** reset_task_alive_bits ******************************/

/**
 * @brief: Resets all task alive flags.
 *
 * @param: none.
 *
 * @retval:
 **/
void reset_task_alive_bits()
{

    task_alive.word_32 &= FORCE_IWDG_BIT;

}/********************** reset_task_alive_bits ******************************/

/**
 * @brief: Sets the appropriate task alive flag.
 *
 * @param: position of a desiried alive flag.
 *
 * @retval: none.
 **/
void set_task_alive_bit(TASK_ALIVE_FLAGS_ET bit_pos)
{
    uint32_t tmp = 1;

    if (bit_pos < MAX_ALIVE_FLAG_E)
    {
        tmp <<= bit_pos;
        task_alive.word_32 |= tmp;
    }

}/********************** set_task_alive_bit *********************************/

/**
 * @brief: Itialize the IWDG.
 *
 * @param: none.
 *
 * @retval: none.
 **/
void iwdg_init()
{

    task_alive.word_32 |= FORCE_IWDG_BIT;
#ifdef USE_IWDG    

    LsiFreq = GetLSIFrequency();

    /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency dispersion) */

    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /* IWDG counter clock: LSI/32 */
    IWDG_SetPrescaler(IWDG_Prescaler_256);

    /* Set counter reload value to obtain 250ms IWDG TimeOut.
     Counter Reload Value = 250ms/IWDG counter clock period
     = 250ms / (LSI/32)
     = 0.25s / (LsiFreq/32)
     = LsiFreq/(32 * 4)
     = LsiFreq/128
     */
    IWDG_SetReload(LsiFreq/16); //128);

    /* Reload IWDG counter */
    IWDG_ReloadCounter();

    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();
#endif 

    reset_task_alive_bits();
}/***************************** iwdg_init ********************************/
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/**
 * @brief  Configures TIM5 to measure the LSI oscillator frequency.
 * @param  None
 * @retval LSI Frequency
 */
uint32_t GetLSIFrequency(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    RCC_ClocksTypeDef RCC_ClockFreq;

    /* Enable the LSI oscillator ************************************************/
    RCC_LSICmd(ENABLE);

    /* Wait till LSI is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY ) == RESET)
    {
    }

    /* TIM5 configuration *******************************************************/
    /* Enable TIM5 clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    /* Connect internally the TIM5_CH4 Input Capture to the LSI clock output */
    TIM_RemapConfig(TIM5, TIM5_LSI );

    /* Configure TIM5 presclaer */
    TIM_PrescalerConfig(TIM5, 0, TIM_PSCReloadMode_Immediate );

    /* TIM5 configuration: Input Capture mode ---------------------
     The LSI oscillator is connected to TIM5 CH4
     The Rising edge is used as active edge,
     The TIM5 CCR4 is used to compute the frequency value 
     ------------------------------------------------------------ */
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
    TIM_ICInitStructure.TIM_ICFilter = 0;
    TIM_ICInit(TIM5, &TIM_ICInitStructure);

    /* Enable TIM5 Interrupt channel */
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable TIM5 counter */
    TIM_Cmd(TIM5, ENABLE);

    /* Reset the flags */TIM5 ->SR = 0;

    /* Enable the CC4 Interrupt Request */
    TIM_ITConfig(TIM5, TIM_IT_CC4, ENABLE);

    /* Wait until the TIM5 get 2 LSI edges (refer to TIM5_IRQHandler() in
     stm32f2xx_it.c file) ******************************************************/
    while (CaptureNumber != 2)
    {
    }
    /* Deinitialize the TIM5 peripheral registers to their default reset values */
    TIM_DeInit(TIM5 );

    /* Compute the LSI frequency, depending on TIM5 input clock frequency (PCLK1)*/
    /* Get SYSCLK, HCLK and PCLKx frequency */
    RCC_GetClocksFreq(&RCC_ClockFreq);

    /* Get PCLK1 prescaler */
    if ((RCC ->CFGR & RCC_CFGR_PPRE1 )== 0){
        /* PCLK1 prescaler equal to 1 => TIMCLK = PCLK1 */
        return ((RCC_ClockFreq.PCLK1_Frequency / PeriodValue) * 8);
    }
    else
    { /* PCLK1 prescaler different from 1 => TIMCLK = 2 * PCLK1 */
        return (((2 * RCC_ClockFreq.PCLK1_Frequency) / PeriodValue) * 8);
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
