/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : PHANTOM 
*
*  FILE        : board_setup.c
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Phantom\src 
*
*  PURPOSE     : 
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : PHANTOM 
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.01     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
      Copyright and Proprietary Statment.

 Igor Natachanny 
 Independent Embedded System Professional.
 igornatachanny@hotmail.com 054-7656857 
*******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
  
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
  #include "stm32f2xx_rcc.h"
  #include "stm32f2xx_gpio.h"
  #include "misc.h"
  #include "stm32f2xx_exti.h"
//  #include "leds.h"
//  #include "pins_roles.h"
  #include "usart.h"

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
  typedef void (*rcc_prph_clk_cmd)(uint32_t prph_name, FunctionalState NewState);

/* --- PRIVATE TYPES -------------------------------------------------------- */
  typedef struct{
    uint8_t            AltFunNum;
    uint8_t            GPIO_PinSource;
    GPIO_TypeDef*      GPIO_Port;
    rcc_prph_clk_cmd   ClkCmdFun;
    uint32_t           GPIO_Clock;
    GPIO_InitTypeDef   pin;
    EXTI_InitTypeDef   exin;
    NVIC_InitTypeDef   nvic;   
  } PIN_CNFG_ST, *PIN_CNFG_PS;
/*----------------------------------------------------------------------------*/
  typedef struct{
    USART_TypeDef      *UsartName;
//    uint32_t            UsartRole;
    uint32_t            UsartClk;
    rcc_prph_clk_cmd    ClkCmdFun;
    USART_InitTypeDef   CommParams;
    NVIC_InitTypeDef    NvicParams;   
  } USART_CNFG_ST, *USART_CNFG_PS;
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
  void stm32f2xx_pins_cfg();

/* --- PRIVATE_DATA --------------------------------------------------------- */
  #define BOARD_CNFG
  #include "board_setup.h"

/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
  /*------------------------------------------------------------------------------
  Configures a USARTs.
  *----------------------------------------------------------------------------*/
  void stm32f2_usart_cnfg(){
   uint32_t i, cnfg_data_size = sizeof(usarts_cfng_data)/sizeof(USART_CNFG_ST);
   
    for(i = 0; i < cnfg_data_size; i++){
      usarts_cfng_data[i].ClkCmdFun(usarts_cfng_data[i].UsartClk, ENABLE);
      USART_Init(usarts_cfng_data[i].UsartName, &(usarts_cfng_data[i].CommParams));
      NVIC_Init(&(usarts_cfng_data[i].NvicParams));
      USART_Cmd(usarts_cfng_data[i].UsartName, ENABLE);
//      USART_ITConfig(usarts_cfng_data[i].UsartName, USART_IT_TC, ENABLE);
    }/* for */

  }/* stm32f2_usart_cnfg */
  
/*----------------------------------------------------------------------------
  Configures and initializes all used pins.
 *----------------------------------------------------------------------------*/
  void stm32f2xx_pins_cnfg(){    
   uint32_t i, cnfg_data_size = sizeof(pins_cfng_data)/sizeof(PIN_CNFG_ST);
    
    for(i = 0; i < cnfg_data_size; i++){
      
        pins_cfng_data[i].ClkCmdFun(pins_cfng_data[i].GPIO_Clock, ENABLE);
        GPIO_Init( (pins_cfng_data[i].GPIO_Port), (&(pins_cfng_data[i].pin)) );
        
        if(pins_cfng_data[i].pin.GPIO_Mode == GPIO_Mode_AF){
          GPIO_PinAFConfig((pins_cfng_data[i].GPIO_Port),(pins_cfng_data[i].GPIO_PinSource),(pins_cfng_data[i].AltFunNum));
        }
        
    }/* for */
                  
  }/* stm32f2xx_pins_cnfg */

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
  
  void phantom_board_setup(){
    stm32f2xx_pins_cnfg();
//    InitDriver();
//    InitUsart6();
    stm32f2_usart_cnfg();
    init_usart(USART6);

//    usart3_init();
  }/* phantom_board_setup */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
/*----------------------------------------------------------------------------
  synop
 *----------------------------------------------------------------------------*/