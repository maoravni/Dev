#ifndef _GnrlCfgDB_h_
#define _GnrlCfgDB_h_

   
#include "cfgDBUtil.h"
#include "fec.h"




  
typedef enum {
  TEST_RESULT_NOT_READ,
  TEST_RESULT_PASS,
  TEST_RESULT_FAIL
}TEST_RESULT;  

typedef enum
{
  OPERATION_MODE_NORMAL, //default - normal mode
  OPERATION_MODE_SIMULATION, // simulation of the real hardware
  OPERATION_MODE_ATP // Board test mode
} OPERATION_MODE;




typedef enum
{
	CNTL_TYPE_PID_INTERNAL, //on board pid algorithm
	CNTL_TYPE_PID_DEVICE,  // PID done by puma device
	CNTL_TYPE_PID_HOST     //PID done by the host (Labview, etc).
}CNTL_TYPE_PID;
//extern CNTL_TYPE_PID ControlTypeHandle;




typedef enum
{
	ID_GNRL_CFG_INT_SET_CONSOLE_LOG_MESSAGE_TH=ID_GNRL_CFG_INT_FIRST, /* +1 is to force id different from zero*/
	ID_GNRL_CFG_INT_ETHER_IP_ADDRESS,
	ID_GNRL_CFG_INT_ETHER_IP_MASK,
        ID_GNRL_CFG_INT_CONTROL_TYPE,     //0: Internal controller, 1: External controller (PUMA), 2: LABView
  	ID_GNRL_CFG_INT_LAST, /* for debug purpose only*/
} GNRL_Int_ID_Enum;

typedef enum
{
        ID_GNRL_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES= ID_GNRL_CMND_INT_FIRST,
	ID_GNRL_CMND_INT_COLD_RESET,  /* soft reset */
	ID_GNRL_CMND_INT_SET_DEFAULT_CONFIGURATION,
	ID_GNRL_CMND_INT_HW_RESET,
	ID_GNRL_CMND_INT_WD_ENABLE,
        ID_GNRL_CMND_INT_SET_OPERATION_MODE,
        ID_GNRL_CMND_INT_SET_MODBUS_MASTER_BAUD,
	ID_GNRL_CMND_INT_EEPROM_DISPLAY_CONTENT,

        ID_GNRL_CMND_INT_TIME_SS_HSS, /* Bits 15 - 9: Sec;   Bits 9 - 0: mSec    */
        ID_GNRL_CMND_INT_TIME_HH_MM,  /* Bits 15 - 8: Hours; Bits 7 - 0: Minutes */
        ID_GNRL_CMND_INT_DATE_MM_DD,  /* Bits 15 - 8: Month; Bits 7 - 0: Day     */
        ID_GNRL_CMND_INT_DATE_YEAR,   /* Bits 15 - 0: Year;                      */
        
        ID_GNRL_CMND_INT_START_CAN_TEST,
        ID_GNRL_CMND_INT_START_MODBUS_TEST,
        ID_GNRL_CMND_INT_START_EEPROM_TEST,

        ID_GNRL_CMND_INT_CLEAR_ALARM,
        ID_GNRL_CMND_INT_DEBUG_VALUE,
        ID_GNRL_CMND_INT_DEBUG_CTRL,
       
        ID_GNRL_CMND_INT_FW_UPGRD,
        ID_GNRL_CMND_INT_FEC_RESET,
        
        ID_GNRL_CMND_SET_HWPWM_DTCYL,


	ID_GNRL_CMND_INT_LAST, /* for debug only*/
}GNRL_Cmnd_ID_Enum;


typedef enum
{
		
	ID_GNRL_MNTR_INT_CRITICAL_ALARM=ID_GNRL_MNTR_INT_FIRST,
        ID_GNRL_MNTR_INT_TIME_SS_HSS, /* Bits 15 - 9: Sec;   Bits 9 - 0: mSec    */
        ID_GNRL_MNTR_INT_TIME_HH_MM,  /* Bits 15 - 8: Hours; Bits 7 - 0: Minutes */
        ID_GNRL_MNTR_INT_DATE_MM_DD,  /* Bits 15 - 8: Month; Bits 7 - 0: Day     */
        ID_GNRL_MNTR_INT_DATE_YEAR,   /* Bits 15 - 0: Year;                      */
        
        ID_GNRL_MNTR_INT_SUBSYS_ID,
        ID_GNRL_MNTR_INT_BOARD_HW_ID,
        ID_GNRL_MNTR_INT_BOARD_DIP_SWITCH,
        ID_GNRL_MNTR_INT_BOARD_TEMPERATURE,
        
        ID_GNRL_MNTR_INT_CAN_TEST_RESULT,
        ID_GNRL_MNTR_INT_MODBUS_TEST_RESULT,
        ID_GNRL_MNTR_INT_EEPROM_TEST_RESULT,
        
        ID_GNRL_MNTR_INT_SW_VERSION_REVISION,
        ID_GNRL_MNTR_INT_SW_VERSION_BUILD,
        ID_GNRL_MNTR_INT_SW_VERSION_MINOR,
        ID_GNRL_MNTR_INT_SW_VERSION_MAJOR,

	ID_GNRL_MNTR_INT_LAST, /* for debug only*/
}GNRL_Monitor_ID_Enum;


typedef enum
{
	ID_GNRL_CFG_STR_TERM_SUPER_PASS=ID_GNRL_CFG_STR_FIRST,
	ID_GNRL_CFG_STR_ALARM_ENABLE,
	ID_GNRL_CFG_STR_ALARM_SEVERITY,

	ID_GNRL_CFG_STR_LAST /* for debug only*/

} GNRL_Str_ID_Enum;


typedef enum
{

    ID_GNRL_VOLATILE_STR_GET_APPLIC_DESCRIPTION=ID_GNRL_VOLATILE_STR_FIRST,
    ID_GNRL_VOLATILE_STR_DATE_AND_TIME,
   
    ID_GNRL_VOLATILE_STR_LAST
} GNRL_VolatileStr_ID_Enum;


#ifdef __cplusplus
extern "C"
{
#endif

/* the following routine should not be called directly. The serve the DB handle routines */
U8 SetIpAddr(int SubSysType, int NodeID);
OPERATION_MODE OperationMode();
BOOL IsInAtpMode();
STATUS GnrlCfgInit();
STATUS GnrlCfgUpdateFromSource( BOOL LoadConfigFromUnit);
STATUS GnrlCfgBuildDB(BOOL SkipDefaultFile);
STATUS GnrlCfgDelete();
STATUS GnrlCfgDB_GetElement (CfgDB_ID Id, CfgDB_ID_data * buff,int DataLen,CfgDB_GET_FLAG GetFlag);
STATUS GnrlCfgDB_SetElement ( CfgDB_ID Id, CfgDB_ID_data * buff,CfgDB_SET_FLAG SetFlag);
STATUS GnrlCfg_UpdateApplicExistanceFileDbElements();
CfgDB_ID_IntDescriptor * GnrlCfg_DbIntDescriptor(CfgDB_ACCESS_TYPE AccessType,int *NumOfElements);
CfgDB_ID_StrDescriptor * GnrlCfg_DbStrDescriptor(CfgDB_ACCESS_TYPE AccessType,int *NumOfElements);
int GnrlCfgDbReady();
int GnrlDbInInit();

#ifdef __cplusplus
}
#endif

#endif
