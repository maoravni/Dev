#ifndef _TemperatureCfgDB_h_
#define _TemperatureCfgDB_h_

#ifdef __cplusplus
extern "C"
{
#endif

#include "cfgDBUtil.h"

#define  NUMBER_OF_TEMPERATURE_ZONES  12
#define SENSOR_NOT_CONNECTED_MARK 9999 // Value for marking bot connected sensor
  

typedef enum
{
    ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n = ID_TEMPERATURE_CFG_INT_FIRST,
    ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,
    ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,
    ID_TEMPERATURE_CFG_INT_ZONE_SP_n,
    ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,
    ID_TEMPERATURE_CFG_INT_LAST, /* for debug purpose only*/
} TEMPERATURE_Int_ID_Enum;

typedef enum
{
    ID_TEMPERATURE_CMND_INT_UPDATE_DB_PERIPHERALS_VALUES = ID_TEMPERATURE_CMND_INT_FIRST,
    ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,
    ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,

    ID_TEMPERATURE_CMND_INT_LAST, /* for debug only*/
} TEMPERATURE_Cmnd_ID_Enum;

typedef enum
{

    ID_TEMPERATURE_MNTR_INT_ZONE_PV_n = ID_TEMPERATURE_MNTR_INT_FIRST,
    ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,
    ID_TEMPERATURE_MNTR_INT_CRITICAL_ALARM,

    ID_TEMPERATURE_MNTR_INT_LAST, /* for debug only*/
} TEMPERATURE_Monitor_ID_Enum;

typedef enum
{
    ID_TEMPERATURE_CFG_STR_TBD = ID_TEMPERATURE_CFG_STR_FIRST, ID_TEMPERATURE_CFG_STR_LAST /* for debug only*/

} TEMPERATURE_Str_ID_Enum;

typedef enum
{

    ID_TEMPERATURE_VOLATILE_STR_TBD = ID_TEMPERATURE_VOLATILE_STR_FIRST,

    ID_TEMPERATURE_VOLATILE_STR_LAST
} TEMPERATURE_VolatileStr_ID_Enum;

/* the following routine should not be called directly. The serve the DB handle routines */
STATUS TemperatureCfgInit();
STATUS TemperatureCfgUpdateFromSource(BOOL LoadConfigFromUnit);
STATUS TemperatureCfgBuildDB(BOOL SkipDefaultFile);
STATUS TemperatureCfgDelete();
STATUS TemperatureCfgDB_GetElement(CfgDB_ID Id, CfgDB_ID_data * buff, int DataLen, CfgDB_GET_FLAG GetFlag);
STATUS TemperatureCfgDB_SetElement(CfgDB_ID Id, CfgDB_ID_data * buff, CfgDB_SET_FLAG SetFlag);
STATUS TemperatureCfg_UpdateApplicExistanceFileDbElements();
CfgDB_ID_IntDescriptor * TemperatureCfg_DbIntDescriptor(CfgDB_ACCESS_TYPE AccessType, int *NumOfElements);
CfgDB_ID_StrDescriptor * TemperatureCfg_DbStrDescriptor(CfgDB_ACCESS_TYPE AccessType, int *NumOfElements);
int TemperatureCfgDbReady();
int TemperatureDbInInit();

#ifdef __cplusplus
}
#endif

#endif
