/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : FEC  
*
*  FILE        : can_tst.c
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
  #include <string.h>
#ifdef STM32F4XX
#include <stm32f4xx.h>
#else
#include <stm32f2xx.h>
#endif

  #include "free_rtos_exp.h"
  #include "ll_typedef.h"
  #include "GnrlCfgDB.h"
  #include "CfgDB.h"
#ifdef CPP_FRAMEWORK
#include <logger.h>
#endif

/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
  #include "can.h"
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
uint8_t can_tx_tmplt[] = {'A','A','A','A','A','A','A',0};
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
  void tx_data_init(CanTxMsg *tx_data_ptr);
  void tx_data_inc(CanTxMsg *tx_data_ptr);
  void vCanTstTmrCbf( xTimerHandle pxTimer );
/* --- PRIVATE_DATA --------------------------------------------------------- */
  static u32 state = 0, tx_flag = 0;
  CanTxMsg TxMessage1;
  CanRxMsg RxMessage1;
  CanTxMsg TxMessage2;
  CanRxMsg RxMessage2;
  xTimerHandle CanTstTmrHndl;
  static u32 CanTstTmrExpire = 0;
  static U32 can_tst_res = TEST_FAIL;
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/*******************************************************************************
  * @brief : Controlled by Monitor CAN test function.       
  * @param   
  * @retval  
*******************************************************************************/
  void can_test_mntr()
  {
   STATUS res;
     
     switch(state){
        case 0:/* CAN1TX_CAN2RX */
          if(!tx_flag){
            CAN_Transmit(CAN1, &TxMessage1);
            xTimerStart( CanTstTmrHndl, 0 );
            CanTstTmrExpire = 0;
            tx_flag = 1;
          }
          if(!(rcv_can(CAN2))){            
            xTimerStop( CanTstTmrHndl, 0 );
            tx_flag = 0;
            state = 1;
            can_tst_res = TEST_PASS;
#if 0            
            if(!(strcmp((const char*)&(TxMessage1.Data[0]), (const char*)&(RxMessage2.Data[0]))))
              can_tst_res = TEST_PASS;
            else 
              can_tst_res = TEST_FAIL;
#endif            
            tx_data_inc(&TxMessage1);            
          }
        break;
        case 1:/* CAN1RX_CAN2TX */
          if(!tx_flag){
            CAN_Transmit(CAN2, &TxMessage2);
            tx_flag = 1;
            xTimerStart( CanTstTmrHndl, 0 );
            CanTstTmrExpire = 0;
          }
          if(!(rcv_can(CAN1))){           
            xTimerStop( CanTstTmrHndl, 0 );
            tx_flag = 0;
            state = 0;
            can_tst_res = TEST_PASS;
#if 0             
            if(!(strcmp((const char*)&(TxMessage2.Data[0]), (const char*)&(RxMessage1.Data[0]))))
              can_tst_res = TEST_PASS;
            else 
              can_tst_res = TEST_FAIL;
#endif            
            tx_data_inc(&TxMessage2);
          }
        break;
      }/* switch(state) */
     
      if(CanTstTmrExpire == 1){
        CanTstTmrExpire = 0;
        tx_flag = 0;
        can_tst_res = TEST_FAIL;
      }
     
      CfgPutParam(NULL, ID_GNRL_MNTR_INT_CAN_TEST_RESULT, &can_tst_res, CfgDB_SET_TO_DB);
           
              
//       M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"can test monitor: can_tst_res = %d", can_tst_res);
//       M_LOGGER_LOGF (M_LOGGER_LEVEL_TRACE, "can test monitor CALLED!",0,0,0,0,0);
//       M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"can test monitor: res = %d", res);       

     
  }/************** void can_test( void* pvParameters ) *********************/
  
  /**
  * @brief: CAN Test timer creation. Called once from Init Task 
  *
  * @param:      
  *
  * @retval: 
**/
  void CanTestTimerCreate()
  {

    CanTstTmrHndl = xTimerCreate( "CAN_TST_TMR",          // Just a text name, not used by the kernel.
                                          1000,           // The timer period in ticks.
                                          pdFALSE,        // The timers will no auto-reload themselves when they expire.
                                          NULL,   // Assign each timer a unique id equal to its array index.
                                          vCanTstTmrCbf   // Each timer calls the same callback when it expires.
                                      );
    if(!CanTstTmrHndl)
      while(1);    

  }
  
/**
  * @brief: CAN Test Initialization. 
  *
  * @param:      
  *
  * @retval: 
**/
  void CAN_TestInit(){
      
    /* Transmit Structure preparation */
    TxMessage1.StdId = 0x321;
    TxMessage1.ExtId = 0x01;
    TxMessage1.RTR = CAN_RTR_DATA;
    TxMessage1.IDE = CAN_ID_STD;
    TxMessage1.DLC = 8;
    memcpy(&(TxMessage1.Data[0]),&(can_tx_tmplt[0]), 8);
  
  
    TxMessage2.StdId = 0x321;
    TxMessage2.ExtId = 0x01;
    TxMessage2.RTR = CAN_RTR_DATA;
    TxMessage2.IDE = CAN_ID_STD;
    TxMessage2.DLC = 8;
    memcpy(&(TxMessage2.Data[0]),&(can_tx_tmplt[0]), 8);
    
    state   = 0;
    tx_flag = 0;
  }

/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
  /**
  * @brief: CAN test timer call-back function.
  *
  * @param: ptr to a CAN Tx structure.
  *
  * @retval: none.
**/
  void vCanTstTmrCbf( xTimerHandle pxTimer )
  {
    CanTstTmrExpire = 1;
  }
  /**
  * @brief: Fills a tx-string with 'A'.
  *
  * @param: ptr to a CAN Tx structure.
  *
  * @retval: none.
**/
  void tx_data_init(CanTxMsg *tx_data_ptr)
  {
    if(tx_data_ptr)
      memcpy(&(tx_data_ptr->Data[0]),&(can_tx_tmplt[0]), 8);    
  }
  
  /**
  * @brief: Increments a tx-string value;
  *
  * @param: ptr to a CAN Tx structure.     
  *
  * @retval: 
**/
  void tx_data_inc(CanTxMsg *tx_data_ptr)
  {
   int i;
    
    if(!tx_data_ptr)  return;
   
    for(i = 0; i < 7; i++ ){     
      if(tx_data_ptr->Data[i] != 'Z')
        break;
    }/*** while(int i = 0; i < 8; i++) ***/
    
    if( i == 7)
      tx_data_init(tx_data_ptr);
    else 
      (tx_data_ptr->Data[i])++;
    
  }/****************** void tx_data_inc(CanTxMsg *tx_data_ptr) ****************/
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
/*******************************************************************************
  * @brief : CAN test Task function.       
  * @param   
  * @retval  
*******************************************************************************/
  void can_test( void* pvParameters )
  {
   U32 can_tst_res = 0;
   static u32 state = 0, tx_flag = 0;
   
    while (1){
    
      switch(state){
        case 0:
          if(!tx_flag){
            CAN_Transmit(CAN1, &TxMessage1);
            tx_flag = 1;
          }
          if(!(rcv_can(CAN2))){
            tx_flag = 0;
            state = 1;
            if(!(strcmp((const char*)&(TxMessage1.Data[0]), (const char*)&(RxMessage2.Data[0])))){
                   M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"CAN_1_TX to CAN_2_RX TEST OK");
                   can_tst_res = 1;
            }else{   
               M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"CAN_1_TX to CAN_2_RX TEST FAULT");
               can_tst_res = 0;
             }
            CfgPutParam(NULL, ID_GNRL_MNTR_INT_CAN_TEST_RESULT, &can_tst_res ,CfgDB_SET_TO_DB);
            tx_data_inc(&TxMessage1);
            vTaskDelay(1000);
          }
        break;
        case 1:
          if(!tx_flag){
            CAN_Transmit(CAN2, &TxMessage2);
            tx_flag = 1;
          }
          if(!(rcv_can(CAN1))){
            tx_flag = 0;
            state = 0;
            if(!(strcmp((const char*)&(TxMessage2.Data[0]), (const char*)&(RxMessage1.Data[0])))){
              M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"CAN_2_TX to CAN_1_RX TEST OK");
              can_tst_res = 1;
            }else{
               M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"CAN_2_TX to CAN_1_RX TEST FAULT");
               can_tst_res = 0;
             }
            CfgPutParam(NULL, ID_GNRL_MNTR_INT_CAN_TEST_RESULT, &can_tst_res ,CfgDB_SET_TO_DB);
            tx_data_inc(&TxMessage2);
            vTaskDelay(1000);
          }
        break;
      }/* switch(state) */        
    }    
  }/************** void can_test( void* pvParameters ) *********************/
