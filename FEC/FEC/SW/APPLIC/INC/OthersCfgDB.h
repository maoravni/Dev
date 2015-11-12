#ifndef _OthersCfgDB_h_
#define _OthersCfgDB_h_

#ifdef __cplusplus
extern "C"
{
#endif

#include "cfgDBUtil.h"
  
#define NUMBER_OF_ANALOG_INP_MA   3
#define NUM_OF_ANALOG_OUT_MA      2
#define NUM_OF_ANALOG_OUT_V       1
#define NUMBER_OF_ANALOG_INP_HM   2
  

typedef enum
{
    ID_OTHERS_CFG_INT_TBD = ID_OTHERS_CFG_INT_FIRST,

    ID_OTHERS_CFG_INT_LAST, /* for debug purpose only*/
} OTHERS_Int_ID_Enum;

typedef enum
{
    ID_OTHERS_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES = ID_OTHERS_CMND_INT_FIRST,
    ID_OTHERS_CMND_INT_DIGITAL_OUT_n,
    ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n,
    ID_OTHERS_CMND_INT_ANALOG_OUT_V,

    ID_OTHERS_CMND_INT_SAFTY_RELAY,  //for ATP uses activate safety relay
    ID_OTHERS_CMND_INT_LED_OUT_n, //for ATP uses- turn on/off the LEDs
    ID_OTHERS_CMND_INT_ANALOG_IN_MA_CALIB_n,
    ID_OTHERS_CMND_INT_ITH_VALVES_STATE,  //turn on/off two valves of ITH input air pressure
    
    ID_OTHERS_CMND_INT_LAST, /* for debug only*/
} OTHERS_Cmnd_ID_Enum;

typedef enum
{

    ID_OTHERS_MNTR_INT_DIGITAL_INP_n = ID_OTHERS_MNTR_INT_FIRST,
    ID_OTHERS_MNTR_INT_ANALOG_INP_MA_n,
    ID_OTHERS_MNTR_INT_ANALOG_INP_MA_DIG_n,
    ID_OTHERS_MNTR_INT_ANALOG_INP_HM_DIG_n,
    ID_OTHERS_MNTR_INT_ANALOG_INP_HM_MA_n,
    ID_OTHERS_MNTR_INT_NOT_USED,

    ID_OTHERS_MNTR_INT_LAST, /* for debug only*/
} OTHERS_Monitor_ID_Enum;

typedef enum
{
    ID_OTHERS_CFG_STR_TBD = ID_OTHERS_CFG_STR_FIRST, ID_OTHERS_CFG_STR_LAST /* for debug only*/

} OTHERS_Str_ID_Enum;

typedef enum
{

    ID_OTHERS_VOLATILE_STR_TBD = ID_OTHERS_VOLATILE_STR_FIRST,

    ID_OTHERS_VOLATILE_STR_LAST
} OTHERS_VolatileStr_ID_Enum;

/* the following routine should not be called directly. The serve the DB handle routines */
STATUS OthersCfgInit();
STATUS OthersCfgUpdateFromSource(BOOL LoadConfigFromUnit);
STATUS OthersCfgBuildDB(BOOL SkipDefaultFile);
STATUS OthersCfgDelete();
STATUS OthersCfgDB_GetElement(CfgDB_ID Id, CfgDB_ID_data * buff, int DataLen, CfgDB_GET_FLAG GetFlag);
STATUS OthersCfgDB_SetElement(CfgDB_ID Id, CfgDB_ID_data * buff, CfgDB_SET_FLAG SetFlag);
STATUS OthersCfg_UpdateApplicExistanceFileDbElements();
CfgDB_ID_IntDescriptor * OthersCfg_DbIntDescriptor(CfgDB_ACCESS_TYPE AccessType, int *NumOfElements);
CfgDB_ID_StrDescriptor * OthersCfg_DbStrDescriptor(CfgDB_ACCESS_TYPE AccessType, int *NumOfElements);
int OthersCfgDbReady();
int OthersDbInInit();

#ifdef __cplusplus
}
#endif

#endif
