/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : PHANTOM 
*
*  FILE        : 
*   
*  PATH        :  
*
*  PURPOSE     : 
*
*  TOOLCHAIN:  : IAR ARM Embedded Workbench 6.303 Evaluation
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
#ifdef __cplusplus
 extern "C" {
#endif
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
#include "ll_typedef.h"
#include <logger.h>
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
#include "a2d.h"
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
/*******************************************************************************
  * @brief : A2D test Task function.       
  * @param   
  * @retval  
*******************************************************************************/
  void a2d_test( void* pvParameters ){
    int   param_id;
    uint16_t      val;
    A2D_RES_ET    res;
    
    while (1){
      
//      change_ip_addr(IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR4);//11/12/2012 Igor

      res = a2d_read_dig(RTD_1, &val);
      if(res == A2D_COMPLETE)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"RTD_1  = %d", val);           /* 1 */

      
      res = a2d_read_dig(RTD_2, &val);
      if(res == A2D_COMPLETE)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"RTD_2  = %d", val);           /* 2 */

      
      res = a2d_read_dig(RTD_3, &val);
      if(res == A2D_COMPLETE)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"RTD_3  = %d", val);           /* 3 */

      res = a2d_read_dig(RTD_4, &val);
      if(res == A2D_COMPLETE)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"RTD_4  = %d", val);           /* 4 */
      
      res = a2d_read_dig(RTD_5, &val);
      if(res == A2D_COMPLETE)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"RTD_5  = %d", val);           /* 5 */
      
      res = a2d_read_dig(RTD_6, &val);
      if(res == A2D_COMPLETE)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"RTD_6  = %d", val);           /* 6 */

      res = a2d_read_dig(RTD_7, &val);
      if(res == A2D_COMPLETE)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"RTD_7  = %d", val);           /* 7 */
      
      res = a2d_read_dig(RTD_8, &val);
      if(res == A2D_COMPLETE)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"RTD_8  = %d", val);           /* 8 */
      
      res = a2d_read_dig(RTD_9, &val);
      if(res == A2D_COMPLETE)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"RTD_9  = %d", val);           /* 9 */
      
      res = a2d_read_dig(RTD_10, &val);
      if(res == A2D_COMPLETE)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"RTD_10 = %d", val);          /* 10 */
      
      res = a2d_read_dig(RTD_11, &val);
      if(res == A2D_COMPLETE)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"RTD_11 = %d", val);          /* 11 */

      res = a2d_read_dig(RTD_12, &val);
      if(res == A2D_COMPLETE)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"RTD_12 = %d", val);          /* 12 */
      
      res = a2d_read_dig(in_4_20mA_1, &val);
      if(res == A2D_COMPLETE)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"in_4_20_mA_1 = %d", val);    /* 13 */
      
      res = a2d_read_dig(in_4_20mA_2, &val);
      if(res == A2D_COMPLETE)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"in_4_20_mA_2 = %d", val);    /* 14 */
      
      res = a2d_read_dig(in_4_20mA_3, &val);
      if(res == A2D_COMPLETE)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"in_4_20_mA_3 = %d", val);    /* 15 */
      else
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"A2D_BUSY");
      
      res = a2d_read_dig(HUM_SENS_1, &val);
      if(res == A2D_COMPLETE)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"HUM_SENS_1 = %d", val);      /* 16 */
      else
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"A2D_BUSY");
      
      res = a2d_read_dig(HUM_SENS_2, &val);
      if(res == A2D_COMPLETE)
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"HUM_SENS_2 = %d", val);      /* 17 */
      else
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"A2D_BUSY");
      
      vTaskDelay(1000);
    }    
  }/*************** void a2d_test( void* pvParameters ) ***********************/
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
/*----------------------------------------------------------------------------
  synop
 *----------------------------------------------------------------------------*/
#ifdef __cplusplus
 }
#endif
