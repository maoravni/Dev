/*
 * MessageStructs.h
 *
 *  Created on: 18 бреб 2012
 *      Author: maora
 */

#ifndef MESSAGESTRUCTS_H_
#define MESSAGESTRUCTS_H_

#include <CLogger.h>

// set member packing to 1.
#define M_DEVICE_ID_ALL 0xff
#define M_BOARD_ID_ALL 0xff

enum E_AckStatus
{
    E_AckStatus_Success = 0x0,
    E_AckStatus_InvalidState = 0x1,
    E_AckStatus_InvalidMessage = 0x2,
    E_AckStatus_InvalidDevice = 0x4,
    E_AckStatus_Canceled = 0x8,
    E_AckStatus_NotImplemented = 0x8000
//E_AckStatus_Size
};

enum E_SeqEndedStatus
{
    E_SeqEndedStatus_Success = 0x0,
    E_SeqEndedStatus_Timeout = 0x1,
    E_SeqEndedStatus_Stopped = 0x2,
    E_SeqEndedStatus_Overriden = 0x4,
    E_SeqEndedStatus_Error
};

enum E_TMSDeviceType
{
    E_TMSDeviceType_None = 0,
    E_TMSDeviceType_Dryer = 1,
    E_TMSDeviceType_IBD = 2,
    E_TMSDeviceType_ITH = 4,
    E_TMSDeviceType_Conditioning = 8,
    E_TMSDeviceType_Cleaning = 16,
    E_TMSDeviceType_All = 255,
};

enum E_ZoneType
{
    E_ZoneType_Heater,
    E_ZoneType_TemperatureSensor,
    E_ZoneType_Humidity,
    E_ZoneType_AirBleeding,
    E_ZoneType_Fan,
    E_ZoneType_Blower,
    E_ZoneType_IBD,
    E_ZoneType_IBDBlower,
    E_ZoneType_IBDFan,
    E_ZoneType_AnalogInput,
    E_ZoneType_Cooling,
    E_ZoneType_PressureSensor,
    E_ZoneType_AirInletValve,
    E_ZoneType_Valve,

    E_ZoneType_BtcEngageSystem,

    E_ZoneType_FlowMeter,
    E_ZoneType_Pump,

    E_ZoneType_DigitalInput,
    E_ZoneType_TankLevelSens,

    E_ZoneType_BtcBlanketTemperature,
    E_ZoneType_BtcWaterContamination,

    E_ZoneType_Size
};

#define M_BASE_ZONE_UNASSIGNED_ID 0xfe

enum E_DataTypes
{
    E_DataType_Temperature = 0x1,
    E_DataType_SetPoint = 0x2,
    E_DataType_Power = 0x4,
    E_DataType_ActivatedSetPoint = 0x8,
    E_DataType_IO = 0x10,
    E_DataType_PowerConsumption = 0x20,
    E_DataType_MaxPower = 0x40,
    E_DataType_RatedPower = 0x80,
    E_DataType_Status = 0x100,
    E_DataType_Humidity = 0x200,
    E_DataType_BlowerSpeed = 0x400,
    E_DataType_FanSpeed = 0x800,
    E_DataType_InValveState = 0x1000,
    E_DataType_OutValveState = 0x2000
};

enum E_TmsErrors
{
    E_TmsErrors_None = 0,
    E_TmsErrors_TempExceedsLimit = 0x1,
    E_TmsErrors_EngageTimeOut = 0x2,
    E_TmsErrors_WaterTankTimeOut = 0x4,
    E_TmsErrors_AirKnifeTimeOut = 0x8,
    E_TmsErrors_WaterContamination = 0x10,
    E_TmsErrors_WaterTankOverflow = 0x20,
    E_TmsErrors_TubOverflow = 0x40,
    E_TmsErrors_BlanketStop = 0x80,
    E_TmsErrors_Emergency = 0x100,
    E_TmsErrors_Engage = 0x200,
    E_TmsErrors_SensorMalfunction = 0x400,
    E_TmsErrors_Drawer = 0x800,
    E_TmsErrors_HeaterTempExceedsLimit = 0x1000,
};

enum E_TmsWarnings
{
    E_TmsWarnings_None = 0, E_TmsWarnings_TempExceedsLimit = 0x1, E_TmsWarnings_Size
};

enum E_TemperatureErrorMessages
{
    E_TemperatureZone_ValueHighWarning = 0x10000,
    E_TemperatureZone_ValueHighError,
    E_TemperatureZone_ValueLowWarning,
    E_TemperatureZone_ValueLowError
};

#pragma pack(1)

#define __int64 long long

#include "GeneratedMessages.def.h"

struct TemperatureZoneSetPoint
{
    short lowWarningRange; //!< Below this value an error should be issued.
    short lowRange; //!< Below this value a warning should be issued.
    short highRange; //!< Above this range a warning shuld be issued.
    short highWarningRange; //!< Above this value an error should be issued.
    short temperature; //!< Temperature of the set point.
};

#define M_TEMPERATURE_ZONE_NUMBER_OF_SET_POINTS 3

struct T_TemperatureZoneMonitorReply
{
    unsigned short temperature;
    unsigned short status;
};

struct TmcMessageIdSplit
{
    unsigned short id;
    char dst;
    char src;
};

union TmcMessageId
{
    TmcMessageIdSplit split;
    unsigned long full;
};

struct TmcMessageHeader
{
    TmcMessageId id;
    unsigned short length;
    unsigned int sn;
    unsigned int crc;
};

struct TmcMessageStruct
{
    TmcMessageHeader header;
    PSSMsgType payload;
};

// restore default member packing.
#pragma pack()

#endif /* MESSAGESTRUCTS_H_ */
