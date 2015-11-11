/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     :  
*
*  FILE        : iwdg_cnfg.h
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
******************************************************************************/

#ifndef _IWDG_CNFG_H_
#define _IWDG_CNFG_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */

/* --- EXPORTED TYPES ------------------------------------------------------ */
   
  typedef enum {
    MONITOR_ALV_E = 0,        
    PWM_ALV_E,            
    MB_1_MASTER_ALV_E,    
    AUX_TERM_ALV_E,       
    TCP_IP_ALV_E,         
    TCP_MB_ALV_E,         
    ETH_IF_ALV_E,
    IDLE_ALV_E,
    MAX_ALIVE_FLAG_E   /* Keep it last */
  }TASK_ALIVE_FLAGS_ET;

  
  typedef struct tasks_alive_flags {
    uint32_t   monitor         :1;
    uint32_t   pwm             :1;
    uint32_t   mb_1_master     :1;
    uint32_t   aux_term        :1;
    uint32_t   tcp_ip          :1;
    uint32_t   tcp_mb          :1;
    uint32_t   eth_if          :1;
    uint32_t   idle            :1;
    uint32_t   reserved       :23;
    uint32_t   force_iwdg      :1;  // true if exteranl wd reset is requsedted (used for device HW reset)
  } TASK_ALIVE_FLAGS_ST, *TASK_ALIVE_FLAGS_PST;
   
  #define FORCE_IWDG_BIT   0x80000000
  #define MASK_WD_WORD (((2^(MAX_ALIVE_FLAG_E-1))-1) | FORCE_IWDG_BIT)
   
   typedef union _tasks_alive_flags_ {
     TASK_ALIVE_FLAGS_ST tasks;
     uint32_t word_32;
   } TASK_ALIVE_FLAGS_UT, *TASK_ALIVE_FLAGS_PUT;
/* --- EXPORTED CONSTANTS -------------------------------------------------- */
//#undef  USE_IWDG   Igor 20120227 Moved to project option \compiler \preprocessor
//#define  USE_IWDG   
/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/

/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/







#ifdef __cplusplus
}
#endif
#endif /* _IWDG_CNFG_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
