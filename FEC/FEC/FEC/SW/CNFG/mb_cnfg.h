/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     :  PHANTOM
*
*  FILE        : mb_cnfg.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Phantom\SW\CNFG  
*
*  PURPOSE     : This project specific Modbus Library configuration Data.
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : Mosquito Board  
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.08     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
      Copyright and Proprietary Statment.

 Igor Natachanny 
 Independent Embedded System Professional.
 igornatachanny@hotmail.com 054-7656857 
******************************************************************************/

#ifndef _MB_CNFG_H_
#define _MB_CNFG_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */

/* --- EXPORTED TYPES ------------------------------------------------------ */
  typedef enum {
    NODE_ID0,
    NODE_ID1,
    NODE_ID2,
    NUMBER_OF_NODEIDs /* Keep this last    */
  }NOD_ID_ET;
  
  typedef enum _modbus_node_name_ {
    MB_1 = 0,
    MB_2,
/* next modbus node number should added here */
    NUM_OF_MB_NODES
  }MB_NODE_NAME_ET;
/* --- EXPORTED CONSTANTS -------------------------------------------------- */
//  #define   MB_SLAVE_NODE
  #define   MB_SLAVE_ADDR_TEMPLATE   0x80
  
  #define   DE_PORT   GPIOA
  #define   DE_PIN    GPIO_Pin_10
  #define   RE_PORT   GPIOA
  #define   RE_PIN    GPIO_Pin_10
  
  #define   DE2_PORT   GPIOA
  #define   DE2_PIN    GPIO_Pin_10    
  #define   RE2_PORT   GPIOA
  #define   RE2_PIN    GPIO_Pin_10      

 
  #define MODBUS_QUEUE_LEN       20
  #define MODBUS_QUEUE_ITEM_SZ   ( sizeof(MB_QUEUE_ITM_ST) )
  
  #define MB_BUF_SZ  256  /* as maximal lenght(in bytes) of Modbus  Application Data Unit */
/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/

/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/
#ifdef  GET_NODE_ID_IMPL_HERE  
  /*                                  NODE_ID0   NODE_ID1    NODE_ID2           */
  static GPIO_TypeDef*   PortOf[] = { GPIOD,     GPIOD,      GPIOE      };
                         
  const uint16_t static  PinOf[] =  {GPIO_Pin_2, GPIO_Pin_6, GPIO_Pin_12 };
#endif
  

  
  /* Modbus channels to phisicals connectivites assignment look-up table */
//#ifdef MB_INIT
/*                                             MB_1    MB_2   */
  static USART_TypeDef*   mb_media_asign[] = {USART3};//, USART3};
//#else
//  extern USART_TypeDef*   mb_media_asign[];
//#endif  
#ifdef __cplusplus
}
#endif
#endif /* _MB_CNFG_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
