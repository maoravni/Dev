/******************************************************************************
 *                     (C) LandaLabs 2012                                      *
 *******************************************************************************
 *
 *  PROJECT     :
 *
 *  FILE        :
 *
 *  PATH        :
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
 *  2012.01     Igor Natachanny   1.0.0   Initial version
 *
 *******************************************************************************
 *      Copyright and Proprietary Statment.
 *
 *******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
#ifdef __cplusplus
 extern "C" {
#endif
#define OPTO_OUTS_CTRL_IMPL_HERE
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
#include "ll_typedef.h"
#include "opto_out_cnfg.h"
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
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
/* --- PRIVATE_DATA --------------------------------------------------------- */
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/**
 * @brief: Configures a deseried PWM channel as HW PWM Channel with deseried duty cycle value.
 *
 * @param:
 *
 * @retval:
 **/
STATUS hw_pwm_channel_init(OPTO_OUT_ID_ET hwpwm_ch_id)
{
    uint8_t AltFunNum;
    uint8_t pin_src;

    if ((hwpwm_ch_id == OPTO_OUT1) || (hwpwm_ch_id == OPTO_OUT3) || (hwpwm_ch_id == OPTO_OUT5))
        return ERROR;

    switch (hwpwm_ch_id)
    {

    case OPTO_OUT2:
        AltFunNum = GPIO_AF_TIM1;
        pin_src = GPIO_PinSource9;
        break;
    case OPTO_OUT4:
        AltFunNum = GPIO_AF_TIM1;
        pin_src = GPIO_PinSource11;
        break;
    case OPTO_OUT6:
        AltFunNum = GPIO_AF_TIM1;
        pin_src = GPIO_PinSource13;
        break;
    case OPTO_OUT7:
        AltFunNum = GPIO_AF_TIM9;
        pin_src = GPIO_PinSource5;
        break;
    case OPTO_OUT8:
        AltFunNum = GPIO_AF_TIM9;
        pin_src = GPIO_PinSource6;
        break;
#ifndef FEC2_BOARD
    case OPTO_OUT9:
        AltFunNum = GPIO_AF_TIM8;
        pin_src = GPIO_PinSource6;
        break;
    case OPTO_OUT10:
        AltFunNum = GPIO_AF_TIM8;
        pin_src = GPIO_PinSource7;
        break;
    case OPTO_OUT11:
        AltFunNum = GPIO_AF_TIM8;
        pin_src = GPIO_PinSource8;
        break;
    case OPTO_OUT12:
        AltFunNum = GPIO_AF_TIM8;
        pin_src = GPIO_PinSource9;
        break;
#endif
    default:
        return ERROR;
        break;

    }/* switch(hwpwm_ch_id) */

    DO_PortOf[hwpwm_ch_id]->MODER = GPIO_Mode_AF;

    GPIO_PinAFConfig(DO_PortOf[hwpwm_ch_id], pin_src, AltFunNum);

    return OK;
}/* STATUS hw_pwm_channel_init(OPTO_OUT_ID_ET hwpwm_ch_id, uint32_t freq_Hz, uint16_t duty_cycle) */

void hw_pwm_init(u32 frequency_2_4_6, u32 frequency_2_8, u32 frequency_9_10_11_12)
{ //frequency  in Hz  maximum 600000  Hz  minimum  10 Hz
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    uint16_t TimerPeriod = 200 - 1;
    uint16_t TimerPrescalar = 0;
    uint16_t ChannelPulse = 0;


    GPIO_InitTypeDef GPIO_InitStructure;

    /* TIM1 TIM8 TIM9 clock   60Mhz */
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_TIM8 | RCC_APB2Periph_TIM9, ENABLE);

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM8,  ENABLE);

//#if 0
    /* GPIOC  GPIOE clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE);

    /* GPIOC Configuration: TIM8 CH1 (PC6), TIM8 CH2 (PC7), TIM8 CH3 (PC8) and TIM8 CH4 (PC9) */
    //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Connect TIM8 pins to AF3 */
    //GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
    //GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
    //GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM8);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM8);
#if 0
    /* GPIOE Configuration: TIM9 CH1 (PE5), TIM9 CH2 (PE6), TIM1 CH1 (PE9) , TIM1 CH2 (PE11), TIM1 CH3 (PE13) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_9| GPIO_Pin_11| GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* Connect TIM9 pins to AF3 */
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_TIM9);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_TIM9);

    /* Connect TIM1 pins to AF1 */
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);

#endif

    if (frequency_2_4_6 != 0)
    {
        /*Timer1*/
        TimerPrescalar = (uint16_t)((600000.0 / (float) frequency_2_4_6) + 0.5);
        if (TimerPrescalar)
            TimerPrescalar--;
        /* Time Base configuration */
        TIM_TimeBaseStructure.TIM_Prescaler = TimerPrescalar;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;
        TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

        TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

        /* Channel 1, 2,3  Configuration in PWM mode */
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
        TIM_OCInitStructure.TIM_Pulse = ChannelPulse;
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
        TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
        TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

        TIM_OC1Init(TIM1, &TIM_OCInitStructure);

//  TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
        TIM_OC2Init(TIM1, &TIM_OCInitStructure);

//  TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
        TIM_OC3Init(TIM1, &TIM_OCInitStructure);

//  TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;
//  TIM_OC4Init(TIM1, &TIM_OCInitStructure);

        /* TIM1 counter enable */
        TIM_Cmd(TIM1, ENABLE);

        /* TIM1 Main Output Enable */
        TIM_CtrlPWMOutputs(TIM1, ENABLE);
    }

    if (frequency_9_10_11_12 != 0)
    {
        /*Timer8*/

        //TimerPrescalar=200;   //120 M  /10K /600
        TimerPrescalar = (uint16_t)((600000.0 / (float) frequency_9_10_11_12) + 0.5);

        if (TimerPrescalar)
            TimerPrescalar--;
        /* Time Base configuration */
        TIM_TimeBaseStructure.TIM_Prescaler = TimerPrescalar;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;
        TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

        TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

        /* Channel 1, 2,3,4  Configuration in PWM mode */
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
        TIM_OCInitStructure.TIM_Pulse = ChannelPulse;
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
        TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
        TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
        TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

        TIM_OC1Init(TIM8, &TIM_OCInitStructure);

//  TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
        TIM_OC2Init(TIM8, &TIM_OCInitStructure);

//  TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
        TIM_OC3Init(TIM8, &TIM_OCInitStructure);

//  TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;
        TIM_OC4Init(TIM8, &TIM_OCInitStructure);

        /* TIM1 counter enable */
        TIM_Cmd(TIM8, ENABLE);

        /* TIM1 Main Output Enable */
        TIM_CtrlPWMOutputs(TIM8, ENABLE);
    }

    if (frequency_2_8 != 0)
    {

        /*TIM9*/
        TimerPrescalar = (uint16_t)((600000.0 / (float) frequency_2_8) + 0.5);
        if (TimerPrescalar)
            TimerPrescalar--;
        /*ch1 ch2*/
        TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
        TIM_TimeBaseStructure.TIM_Prescaler = TimerPrescalar;
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

        TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);

        /* PWM1 Mode configuration: */
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
        TIM_OCInitStructure.TIM_Pulse = ChannelPulse;
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

        TIM_OC1Init(TIM9, &TIM_OCInitStructure);
        TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable );

        TIM_OC2Init(TIM9, &TIM_OCInitStructure);
        TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable );

        TIM_ARRPreloadConfig(TIM9, ENABLE);

        /* TIM9 enable counter */
        TIM_Cmd(TIM9, ENABLE);
    }

}

//Optoutput_number 2 4 6-12
// dutycicle  from 0 to 1000   0% to 100%  0.5% resolusion
void hw_pwm_dutysicle_set(u8 Optoutput_number, u16 dutycicle)
{

    uint16_t TIM_Pulse = (uint16_t)((0.2 * (float) dutycicle) + 0.5);
    TIM_Pulse--;
    switch (Optoutput_number)
    {
    case OPTO_OUT2:
        TIM1 ->CCR1 = TIM_Pulse;
        break;
    case OPTO_OUT4:
        TIM1 ->CCR2 = TIM_Pulse;
        break;
    case OPTO_OUT6:
        TIM1 ->CCR3 = TIM_Pulse;
        break;
    case OPTO_OUT7:
        TIM9 ->CCR1 = TIM_Pulse;
        break;
    case OPTO_OUT8:
        TIM9 ->CCR2 = TIM_Pulse;
        break;
#ifndef FEC2_BOARD
    case OPTO_OUT9:
        TIM8 ->CCR1 = TIM_Pulse;
        break;
    case OPTO_OUT10:
        TIM8 ->CCR2 = TIM_Pulse;
        break;
    case OPTO_OUT11:
        TIM8 ->CCR3 = TIM_Pulse;
        break;
    case OPTO_OUT12:
        TIM8 ->CCR4 = TIM_Pulse;
        break;
#endif
    default:
        break;
    }

}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
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
#ifdef __cplusplus
 }
#endif
