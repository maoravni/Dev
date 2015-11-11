/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : MOSQUITO 
*
*  FILE        : board_setup.c
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Mosquito\SW\BSP 
*
*  PURPOSE     : 
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : Mosquito at it's board. 
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

  #include "usb_bsp.h"
  #include "usb_conf.h"
  #include "usb_core.h"
  #include "usbd_desc.h"
  #include "usbd_core.h"
  #include "usbd_cdc_core.h"
  #include "usbd_usr.h"
  #include "usart.h"

/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */
  __ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;
/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */
/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
  typedef void (*rcc_prph_clk_cmd)(uint32_t prph_name, FunctionalState NewState);

/* --- PRIVATE TYPES -------------------------------------------------------- */
  typedef struct{
    uint32_t           pre_out_val;
    uint8_t            AltFunNum;
    uint8_t            pin_src;
    uint8_t            port_src;
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
/*----------------------------------------------------------------------------*/
    typedef struct{
      SPI_TypeDef         *spi_name;
      rcc_prph_clk_cmd     clk_cmd_fun;
      uint32_t             spi_clk;
      SPI_InitTypeDef      spi_init_params;

  } SPI_CNFG_ST, *SPI_CNFG_PS;
  
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
  void stm32f2xx_pins_cnfg();
  void stm32f2_usart_cnfg();
  void stm32f2_spi_cnfg();
/* --- PRIVATE_DATA --------------------------------------------------------- */
  #define BOARD_CNFG
  #include "board_setup.h"

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/*----------------------------------------------------------------------------
  Configures and initializes the Mosquito Board.
 *----------------------------------------------------------------------------*/  
  void prvSetupHardware()
  {
    
    /* 4 bit for pre-emption priority, 0 bits for subpriority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
    
    stm32f2xx_pins_cnfg();/* Keep this first! */
    stm32f2_usart_cnfg();
    stm32f2_spi_cnfg();
    
/* USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);       */
    
  }/* prvSetupHardware */

/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/**
 * @brief:  Init a SPI interface as desiried.   
 *
 * @param:  A pointer to a SPI descriptor structure(SPI1 or SPI2).     
 *
 * @retval: zero if success, 1 - if the parameter has incorrect value.  
**/
  void stm32f2_spi_cnfg(){
   uint32_t i, cnfg_data_size = sizeof(spi_cfng_data)/sizeof(SPI_CNFG_ST);

    for(i = 0; i < cnfg_data_size; i++){
      SPI_I2S_DeInit(spi_cfng_data[i].spi_name);
      spi_cfng_data[i].clk_cmd_fun(spi_cfng_data[i].spi_clk, ENABLE);
      SPI_Init(spi_cfng_data[i].spi_name, &(spi_cfng_data[i].spi_init_params));
/* SPI interrupts init shoul be added here */
      SPI_Cmd(spi_cfng_data[i].spi_name, ENABLE); /* Keep this last! */
    }
  

  
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; 
//  SPI_Init(SPI2, &SPI_InitStructure);
 //------------------------------------------------------------------------------  
 
  }/*************** uint32_t SPIx__Init(SPI_TypeDef *SPIx) ********************/
/*------------------------------------------------------------------------------
  Configures a USARTs.
*-----------------------------------------------------------------------------*/
  void stm32f2_usart_cnfg(){
   uint32_t i, cnfg_data_size = sizeof(usarts_cfng_data)/sizeof(USART_CNFG_ST);
   
    for(i = 0; i < cnfg_data_size; i++){
      init_usart(  usarts_cfng_data[i].UsartName );
      usarts_cfng_data[i].ClkCmdFun(usarts_cfng_data[i].UsartClk, ENABLE);
      USART_Init(usarts_cfng_data[i].UsartName, &(usarts_cfng_data[i].CommParams));
      NVIC_Init(&(usarts_cfng_data[i].NvicParams));
      USART_Cmd(usarts_cfng_data[i].UsartName, ENABLE);
      USART_ITConfig(usarts_cfng_data[i].UsartName, USART_IT_RXNE, ENABLE);
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
      
      if(pins_cfng_data[i].pin.GPIO_Mode == GPIO_Mode_OUT){       
        if(pins_cfng_data[i].pre_out_val == 0)
//          pins_cfng_data[i].GPIO_Port->BSRRL = pins_cfng_data[i].pin.GPIO_Pin;
          pins_cfng_data[i].GPIO_Port->ODR &= ~(pins_cfng_data[i].pin.GPIO_Pin);
        else
//          pins_cfng_data[i].GPIO_Port->BSRRH = pins_cfng_data[i].pin.GPIO_Pin;
          pins_cfng_data[i].GPIO_Port->ODR |= pins_cfng_data[i].pin.GPIO_Pin;
      }/* if the pin in OUTPUT mode */
      
      if(pins_cfng_data[i].pin.GPIO_Mode == GPIO_Mode_IN){
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);/* Enable the Input Clock */
         if (pins_cfng_data[i].exin.EXTI_LineCmd == ENABLE){    
           SYSCFG_EXTILineConfig(pins_cfng_data[i].port_src, pins_cfng_data[i].pin_src);/* Connect the GPIO EXTI Line to the GPIO Pin */
           EXTI_Init(&(pins_cfng_data[i].exin));
            NVIC_Init(&(pins_cfng_data[i].nvic));
         }

      }/* if the pin in INPUT mode */
      

        GPIO_Init( (pins_cfng_data[i].GPIO_Port), (&(pins_cfng_data[i].pin)) );
        
        if(pins_cfng_data[i].pin.GPIO_Mode == GPIO_Mode_AF){
          GPIO_PinAFConfig((pins_cfng_data[i].GPIO_Port),(pins_cfng_data[i].pin_src),(pins_cfng_data[i].AltFunNum));
        }
        
    }/* for */
                  
  }/* stm32f2xx_pins_cnfg */


/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
