/******************************************************************************
 *                     (C) LandaLabs 2011                                      *
 *******************************************************************************
 *
 *  PROJECT     : BSP_LIB
 *
 *  FILE        : opto_out.c
 *
 *  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\Libraries\BSP
 *
 *  PURPOSE     : LEDs control related thinks.
 *
 *  TOOLCHAIN:  : IAR ARM Embedded Workbench 6.303
 *
 *  TARGET      :
 *
 *
 *  DATE        NAME              VER.    COMMENTS
 * -----------------------------------------------------------------------------
 *  2012.11     Igor Natachanny   1.0.0   Initial version
 *
 *******************************************************************************
 Copyright and Proprietary Statment.

 *******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
#define OPTO_OUTS_CTRL_IMPL_HERE
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
#include <stdio.h>
#include "ll_typedef.h"

#if defined STM32F4XX
#include <stm32f4xx.h>
#elif defined WIN32
#include <Win32MissingDefines.h>
#else
#include <stm32f2xx.h>
#endif

//#include "free_rtos_exp.h"
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
#include "..\BSP\bsp_srvcs.h"
#include  "opto_out_cnfg.h"
#include  "opto_out.h"
//  #include "vcp_term.h"

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

/**
 * @brief  Turns selected 'led' On.
 * @param  'led' going turn On.
 * @retval None
 */
void dig_out_ctrl(OPTO_OUT_ID_ET id, OPTO_OUT_CTRL_VAL_ET val)
{

    switch (val)
    {
    case TOGGLE:
        DO_PortOf[id]->ODR ^= DO_PinOf[id];
        break;
    case DEACTIVE:
        //   if(DO_InvrsOf[id] == 0)
#ifdef PHANTOM_BOARD
        DO_PortOf[id]->BSRRL = DO_PinOf[id];
#else        
        DO_PortOf[id]->BSRRH = DO_PinOf[id];
#endif          
        //    else
        //     DO_PortOf[id]->BSRRL = DO_PinOf[id];
        break;
    case ACTIVE:
        //    if(DO_InvrsOf[id] == 0)
#ifdef PHANTOM_BOARD
        DO_PortOf[id]->BSRRH = DO_PinOf[id];
#else        
        DO_PortOf[id]->BSRRL = DO_PinOf[id];
#endif          
        //    else
        //      DO_PortOf[id]->BSRRH = DO_PinOf[id];
        break;
        //   break;
    }/* switch(val) */

}/* dig_out_ctrl */

void Safety_out_ctrl(OPTO_OUT_CTRL_VAL_ET val)
{

    switch (val)
    {
    case DEACTIVE:
        //   if(DO_InvrsOf[id] == 0)
#ifdef SAFETY_REVERSE_LOGIC
        SAFETY_PortOf->BSRRL = SAFETY_PinOf;
#else        
        SAFETY_PortOf->BSRRH = SAFETY_PinOf;
#endif          
        //    else
        //     DO_PortOf[id]->BSRRL = DO_PinOf[id];
        break;
    case ACTIVE:
        //    if(DO_InvrsOf[id] == 0)
#ifdef SAFETY_REVERSE_LOGIC
        SAFETY_PortOf->BSRRH = SAFETY_PinOf;
#else        
        SAFETY_PortOf->BSRRL = SAFETY_PinOf;
#endif          
        //    else
        //      DO_PortOf[id]->BSRRH = DO_PinOf[id];
        break;
        //   break;
    }/* switch(val) */

}/* dig_out_ctrl */

/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
#if 0    
if( flars & USB_MCG_READY)
{
    flars &= ~USB_MCG_READY;
    SendResponse (msg_buff, 10);
}
#endif
