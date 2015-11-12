/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : MOSQUITO
*
*  FILE        : ll_types.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\
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
*  2012.07     Yossi Sarusi   1.0.0   Initial version
*
*******************************************************************************
*/
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
  #include "stm32f2xx_rcc.h"
  #include "stm32f2xx_gpio.h"
  #include "misc.h"
  #include "stm32f2xx_exti.h"

  
  #include "..\BSP\timers.h"

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
#define IS_TIMER_CH(CH) (((CH) == 1) || \
                        ((CH) == 2) || \
                        ((CH) == 3) || \
                        ((CH) == 4))

/* --- PRIVATE_DATA --------------------------------------------------------- */
  
static TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
static TIM_OCInitTypeDef  TIM_OCInitStructure;
static uint16_t TimerPeriod = 0;
static uint32_t TimerCounterClock=0;

/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */

/**
  * @brief  Config timer as PWM timer with specific cycle 
  * @param  PwmCycleFreq: PWM cycle in Hz
  * @retval None
  */
static void  Timer1_Config(uint8_t PwmCycleFreq)
{
  /* TIM1 Configuration ---------------------------------------------------
   Generate 7 PWM signals with 4 different duty cycles:
   TIM1 input clock (TIM1CLK) is set to 2 * APB2 clock (PCLK2), since APB2 
    prescaler is different from 1.   
    TIM1CLK = 2 * PCLK2  
    PCLK2 = HCLK / 2 
    => TIM1CLK = 2 * (HCLK / 2) = HCLK = SystemCoreClock
   TIM1CLK = SystemCoreClock, Prescaler = 1000, TIM1 counter clock = SystemCoreClock/prescaler
   SystemCoreClock is set to 120 MHz for STM32F2xx devices. TIM1 counter clock=120Khz
   
    
   Note: 
    SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
    Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
    function to update SystemCoreClock variable value. Otherwise, any configuration
    based on this variable will be incorrect. 
  ----------------------------------------------------------------------- */
  /* Compute the value to be set in ARR regiter to generate signal frequency at 10Hz */

  TimerCounterClock=SystemCoreClock/1000;//120Mhz/1000=120Khz
  TimerPeriod = (TimerCounterClock / PwmCycleFreq ) - 1; //set the cycle to 10Hz=100msec
 
  /* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
  
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 1000;  //  120Mhz/1000=120Khz
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Channel 1, 2,3 and 4 Configuration in PWM mode */
  //prepare general purpose structure that will be used later on for every channel setting: Timer1_SetPwmPulse
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

}

/**
  * @brief  Sets the Timer1 pulse value for specific channel
  * @param  ChNum: channel number 1-4
  * @param  PulseInPersent: Precentage of the positive pulse in the PWM. 10-90 value range.
  * @retval None
  */
static void Timer1_SetPwmPulse(uint8_t ChNum, uint8_t PulseInPersent)
{
  assert_param(IS_TIMER_CH(ChNum));
  assert_param((PulseInPersent>9) || (PulseInPersent<90));
  
  uint16_t ChPulse= (uint16_t) (((uint32_t) PulseInPersent * (TimerPeriod - 1)) / 100);
  
  TIM_OCInitStructure.TIM_Pulse = ChPulse; //set tha value in the pre-setted structure
  
  switch (ChNum)
  {
    case 1:
      TIM_OC1Init(TIM1, &TIM_OCInitStructure);
      break;
    case 2:
      TIM_OC2Init(TIM1, &TIM_OCInitStructure);
      break;
    case 3:
      TIM_OC3Init(TIM1, &TIM_OCInitStructure);
      break;  
    case 4:
      TIM_OC4Init(TIM1, &TIM_OCInitStructure);
      break;
   }
}


/**
  * @brief  Enable timer 1.
  * @retval None
  */
static void Timer1_Enable()
{
   /* TIM1 counter enable */
  TIM_Cmd(TIM1, ENABLE);

  /* TIM1 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

/**
  * @brief  Disable timer 1.
  * @retval None
  */
static void Timer1_Disable()
{
   /* TIM1 counter Disable */
  TIM_Cmd(TIM1, DISABLE);

  /* TIM1 Main Output Disable */
  TIM_CtrlPWMOutputs(TIM1, DISABLE);
}
/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/*----------------------------------------------------------------------------
  Configures and initializes the Mosquito Board.
 *----------------------------------------------------------------------------*/  
  void Timer_Init(){
    /* Igor 22/7/2012 Changed acc. to Tal request for blower test at 100Hz.*/
   Timer1_Config(100); //10Hz=100msec pwm cycle
   
   
   
   //for test purpose only
   Timer1_SetPwmPulse(3, 20);  //Ch#3 20% PWM pulse  j9 pin 6
   Timer1_SetPwmPulse(1, 40);  //Ch#1 20% PWM pulse  j9 pin 7
   Timer1_SetPwmPulse(4, 60);  //Ch#4 60% PWM pulse  j9 pin 8
   Timer1_SetPwmPulse(2, 80);  //Ch#2 80% PWM pulse  j9 pin 9
   
   Timer1_Enable();
    
  }
