/******************************************************************************
 *                     (C) LandaLabs 2012                                      *
 *******************************************************************************
 *
 *  PROJECT     : FEC_BOOT
 *
 *  FILE        : main_boot.c
 *
 *  PATH        : C:\Landalabs\Spitfire\Branches_Embedded\FirstPrintFEC\FEC_BOOT\SW\APPLIC\src
 *
 *  PURPOSE     :
 *
 *  TOOLCHAIN:  : IAR ARM Embedded Workbench 6.40.1.3812
 *
 *  TARGET      : FEC at it's board.
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
/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_iwdg.h"
#include "stm32f2x7_eth.h"
#include "bsp_srvcs.h"
#include "iwdg.h"
#include "dg_in_ctrl.h"
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
#include "netconf.h"
#include "boot_cnfg.h"
#include "tftpserver.h"
#include "httpserver.h"
#include "fec.h"
#include "netconf.h"

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
/* Private define ------------------------------------------------------------*/
#define SYSTEMTICK_PERIOD_MS  10

/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */
/* Private typedef -----------------------------------------------------------*/
typedef void (*pFunction)(void);
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
uint32_t Get_SSID_dig();
BOOL SetIpAddr(int SubSysType, int NodeID);
/* --- PRIVATE_DATA --------------------------------------------------------- */__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
uint32_t timingdelay;
pFunction Jump_To_Application;
uint32_t JumpAddress;
int IP_BYTE_H, IP_BYTE_M1, IP_BYTE_M2, IP_BYTE_L;
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*       * M A I N   F U N C T I O N S   I M P L E M E N T A T I O N *        */
/*============================================================================*/

__no_init uint32_t StayInBootLoader @
0x20000000;

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
    /*!< At this stage the microcontroller clock setting is already configured to 
     120 MHz, this is done through SystemInit() function which is called from
     startup file (startup_stm32f2xx.s) before to branch to application main.
     To reconfigure the default setting of SystemInit() function, refer to
     system_stm32f2xx.c file
     */

    /* Test if the FEC Board already has an application image downloaded */
    __IO uint32_t *app_pres_key_addr = (__IO uint32_t *) (APPL_PRESENT_KEY_ADDR);
    uint32_t app_pres_key_val = *app_pres_key_addr;

    prvSetupHardware();
    uint32_t ssid = Get_SSID_dig();

    bool jumpToApp = (StayInBootLoader != 0x12345678);
    StayInBootLoader = 0;

//       DBGMCU->APB1FZ &= DBGMCU_APB1_FZ_DBG_IWDEG_STOP;
//Set the IP address according to the SubSystem type and node ID
    SetIpAddr(Get_SSID_dig(), nod_id_get());

    if (1)
    {
        /* Jump to user application */
        JumpAddress = *(__IO uint32_t*) (USER_FLASH_FIRST_PAGE_ADDRESS + 4);
        Jump_To_Application = (pFunction) JumpAddress;
        /* Initialize user application's Stack Pointer */
        __set_MSP(*(__IO uint32_t*) USER_FLASH_FIRST_PAGE_ADDRESS);
        Jump_To_Application();
    }
    else
    { /* Launch FEC Bootloader */

        /* configure ethernet (GPIOs, clocks, MAC, DMA) */
        ETH_BSP_Config();

        /* Initialize the LwIP stack */
        LwIP_Init();

        /* Initialize the TFTP server */
        IAP_tftpd_init();

        iwdg_init();

        /* Infinite loop */
        while (1)
        {
            /* check if any packet received */
            if (ETH_CheckFrameReceived())
            {
                /* process received ethernet packet */
                LwIP_Pkt_Handle();
            }
            /* handle periodic timers for LwIP */
            LwIP_Periodic_Handle(LocalTime);
            IWDG_ReloadCounter();
        }

    }/* else - enter in IAP mode */

}/***************************** int main(void) ********************************/

/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
//Set the IP address according to SSID and the Dip switch (NodeID)
BOOL SetIpAddr(int SubSysType, int NodeID)
{
    assert_param(DipSwitch<=7);
    assert_param(SubSysType<=7);

    BOOL RetVal = OK;

    IP_BYTE_H = 172;
    IP_BYTE_M1 = 30;
    IP_BYTE_M2 = 30;

    switch (SubSysType)
    {
    case SUBSYS_DRYER: //001
        //172.30.30.50 - 172.30.30.57
        IP_BYTE_L = 50 + NodeID;
        break;

    case SUBSYS_IBD:  //010
        //172.30.30.60 - 172.30.30.67
        IP_BYTE_L = 60 + NodeID;
        break;

    case SUBSYS_ITH:  //011
        //172.30.30.70 - 172.30.30.77
        IP_BYTE_L = 70 + NodeID;
        break;

    case SUBSYS_BTC:  //100
        //172.30.30.80 - 172.30.30.871
        IP_BYTE_L = 80 + NodeID;
        break;

    default:
        //172.30.30.59  - NO SSID dongle is plugged - or unsupported
        IP_BYTE_L = 59;
        RetVal = ERROR;
        break;
    } //switch

    return RetVal;
}

void jump_to_applic()
{
    /* Jump to user application */
    JumpAddress = *(__IO uint32_t*) (USER_FLASH_FIRST_PAGE_ADDRESS + 4);
    Jump_To_Application = (pFunction) JumpAddress;
    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t*) USER_FLASH_FIRST_PAGE_ADDRESS);
    Jump_To_Application();
}

/**
 * @brief  Inserts a delay time.
 * @param  nCount: number of 10ms periods to wait for.
 * @retval None
 */
void Delay(uint32_t nCount)
{
    /* Capture the current local time */
    timingdelay = LocalTime + nCount;

    /* wait until the desired delay finish */
    while (timingdelay > LocalTime)
    {
    }
}/* void Delay(uint32_t nCount) */

/**
 * @brief  Updates the system local time
 * @param  None
 * @retval None
 */
void Time_Update(void)
{
    LocalTime += SYSTEMTICK_PERIOD_MS;
}/* void Time_Update(void) */
/******************************************************************************/
#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {}
}
#endif
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2013                           */
/*=========================================================== end_of_file ====*/

/*----------------------------------------------------------------------------
 synop
 *----------------------------------------------------------------------------*/
