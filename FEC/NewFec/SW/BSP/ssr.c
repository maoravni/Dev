/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : MOSQUITO 
*
*  FILE        : ssr.c
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
*  2012.07     Yossi Sarusi   1.0.0   Initial version
*
*******************************************************************************
      Copyright and Proprietary Statment.

*******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
  
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
#include <stm32f2xx.h>
  #include "misc.h"

  
  #include "fec_timers.h"

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
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/*----------------------------------------------------------------------------
  Configures and initializes the Mosquito Board.
 *----------------------------------------------------------------------------*/  
  void SSR_Init()
  {
    
   Timer_Init();
    
  }
