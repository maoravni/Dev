/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     :  dg_in_ctrl.c
*
*  FILE        : 
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\FEC\SW\BSP 
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
*  2012.11     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
*      Copyright and Proprietary Statment.
* 
*******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
#define  DG_IN_CTRL_IMPL_HERE
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
  #include "ll_typedef.h"

#if defined STM32F4XX
#include <stm32f4xx.h>
#elif defined WIN32
#include <Win32MissingDefines.h>
#else
#include <stm32f2xx.h>
#endif

  #include "dg_in_ctrl.h"
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
  * @brief: Build and return the value of Sub - System ID (SSID) pins. 
  *
  * @param:      
  *
  * @retval: 
**/
  uint32_t Get_SSID_dig(){
#ifdef FEC2_BOARD
      return 0;
#else
   uint32_t  res = 0;
   uint8_t   tmp;
   
    res = 0;
    
     tmp = GPIO_ReadInputDataBit(PortOf[SSID_1], PinOf[SSID_1]);    
    if(tmp == 1)
      res += 4;
    
    tmp = GPIO_ReadInputDataBit(PortOf[SSID_2], PinOf[SSID_2]);
    if(tmp == 1)
      res += 2;
    
    tmp = GPIO_ReadInputDataBit(PortOf[SSID_3], PinOf[SSID_3]);
    if(tmp == 1)
      res += 1;
    
#if 0
    tmp = GPIO_ReadInputDataBit(PortOf[SSID_4], PinOf[SSID_4]);
    if(tmp == 1)
      res += XXX;
#endif    
//    M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE, "SSID = %d",res,0,0,0,0,0);    
    return res;
#endif
  }/*************************** Get_SSID_dig **********************************/

/**
  * @brief: returnes the value of specified digital input bit into user provided place 
  *
  * @param:      
  *
  * @retval: 
**/
  uint32_t dig_in_get(DIG_IN_ID_ET dig_in, uint8_t *res_ptr){
    
    if((dig_in < 0)||(dig_in > NUMBER_OF_DG_INs))
      return LL_ERROR;

#ifndef WIN32
    *res_ptr = GPIO_ReadInputDataBit(PortOf[dig_in], PinOf[dig_in]);
#endif
 
//    M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE, "DIG_IN_%d = %d",(dig_in - NODE_ID_2), *res_ptr,0,0,0,0,0);
    
    return LL_OK;
  }/****************************** dig_in_get *********************************/

/**
  * @brief: 
  *
  * @param:      
  *
  * @retval: 
**/
  uint32_t nod_id_get(){
#ifdef FEC2_BOARD
      return 0;
#else
   uint32_t   res = 0;
   uint16_t  tmp = 0;
   
   tmp = GPIO_ReadInputDataBit(PortOf[NODE_ID_0], PinOf[NODE_ID_0]);    
    if(tmp == 0)
      res += 4;
    
    tmp = GPIO_ReadInputDataBit(PortOf[NODE_ID_1], PinOf[NODE_ID_1]);
    if(tmp == 0)
      res += 2;
    
    tmp = GPIO_ReadInputDataBit(PortOf[NODE_ID_2], PinOf[NODE_ID_2]);
    if(tmp == 0)
      res += 1;

//    M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE, "NODE_ID = %d",res,0,0,0,0,0);
    return res;
#endif
  }/****************************** nod_id_get *********************************/

/**
  * @brief: 
  *
  * @param:      
  *
  * @retval: 
**/

  uint32_t board_id_get(){
#ifdef FEC2_BOARD
      return 0;
#else
   uint32_t  res = 0;
   uint8_t   tmp;
   
    res = 0;

    tmp = GPIO_ReadInputDataBit(PortOf[BOARD_ID_0], PinOf[BOARD_ID_0]);    
    if(tmp == 1)
      res += 1;
    
    tmp = GPIO_ReadInputDataBit(PortOf[BOARD_ID_1], PinOf[BOARD_ID_1]);
    if(tmp == 1)
      res += 2;
    
    tmp = GPIO_ReadInputDataBit(PortOf[BOARD_ID_2], PinOf[BOARD_ID_2]);
    
    if(tmp == 1)
      res += 4;
//    M_LOGGER_LOGF_UNSORTED (M_LOGGER_LEVEL_TRACE, "BOARD_ID = %d",res,0,0,0,0,0);    
    return res;
#endif
  }/*************************** board_id_get **********************************/
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
