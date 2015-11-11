/*
 * MessageStructs.h
 *
 *  Created on: 18 бреб 2012
 *      Author: maora
 */

#ifndef PSC_MESSAGESTRUCTS_H_
#define PSC_MESSAGESTRUCTS_H_

#include <CLogger.h>

// set member packing to 1.
#define M_PSS_ID_ALL 0xffff
#define M_PSS_ID_NONE 0
#define M_BOARD_ID_ALL 0xff

enum E_BoardState
{
    E_BoardState_Off = 0,
    E_BoardState_Startup = 1,
    E_BoardState_On = 2,
    E_BoardState_Initializing = 3,
    E_BoardState_Ready = 6,
    E_BoardState_Stopping = 11,
    E_BoardState_Error = 13,
    E_BoardState_StoppingOnEMR = 16,
    E_BoardState_EMR = 17,
    E_BoardState_Recovery = 23,
    E_BoardState_ShuttingDown = 24,
    E_BoardState_ApplicationUpload = 30,
    E_BoardState_Size,
};

enum E_BoardMode
{
    E_BoardMode_Normal = 0,
    E_BoardMode_HwValidation = 1,
    E_BoardMode_Size
};

enum E_AckStatus
{
    E_AckStatus_Success = 0x0,
    E_AckStatus_InvalidState = 0x1,
    E_AckStatus_InvalidMessage = 0x2,
    E_AckStatus_InvalidDevice = 0x4,
    E_AckStatus_Canceled = 0x8,
    E_AckStatus_InvalidCRC = 0x10,
    E_AckStatus_Timeout = 0x20,
    E_AckStatus_NotImplemented = 0x8000
//E_AckStatus_Size
};

enum E_SeqEndedStatus
{
    E_SeqEndedStatus_Success = 0x0,
    E_SeqEndedStatus_Timeout = 0x1,
    E_SeqEndedStatus_Stopped = 0x2,
    E_SeqEndedStatus_Overriden = 0x4,
    E_SeqEndedStatus_Error = 0x8,
};

enum E_PSSDeviceType
{
    E_PSSDeviceType_None = 0,
    E_PSSDeviceType_Dryer = 1,
    E_PSSDeviceType_IBD = 2,
    E_PSSDeviceType_ITH = 4,
    E_PSSDeviceType_Conditioning = 8,
    E_PSSDeviceType_Cleaning = 16,
    E_PSSDeviceType_All = 255,
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

enum E_PSSErrors
{
    E_PSSErrors_NoErrors = 0x00000000,
    E_PSSErrors_SensorMalfunction = 0x00000001,
    E_PSSErrors_ControlExceedsLimits = 0x00000002,
    E_PSSErrors_EngageTimeout = 0x00000004,
    E_PSSErrors_TankOverflow = 0x00000008,
    E_PSSErrors_BlanketStop = 0x00000010,
    E_PSSErrors_Emergency = 0x00000020,
    E_PSSErrors_PsocSystemModuleError = 0x00000040,
    E_PSSErrors_PsocTemperatureModuleError = 0x00000080,
    E_PSSErrors_PsocAnalogModuleError = 0x00000100,
    E_PSSErrors_PsocMonitorModuleError = 0x00000200,
    E_PSSErrors_PsocSafetyError = 0x00000400,
    E_PSSErrors_CurrentLoopAboveLimit = 0x00000800,
    E_PSSErrors_SsrAlwaysOn = 0x00001000,
    E_PSSErrors_SsrOverheating = 0x00002000,
    E_PSSErrors_CableIdNotMatching = 0x00004000,
    E_PSSErrors_DeviceExceedsSoftLimits = 0x00008000,
    E_PSSErrors_DeviceExceedsHardLimits = 0x00010000,
    E_PSSErrors_ActivationNotReady = 0x00020000,
    E_PSSErrors_ActivationFailed = 0x00040000,
    E_PSSErrors_PrintBarLifted = 0x00080000,
    E_PSSErrors_PsocOutputsNotOpen = 0x00100000,
    E_PSSErrors_PsocNotInOperationalState = 0x00200000,
    E_PSSErrors_PsocLiquidDetectorError   = 0x00400000,
    E_PSSErrors_InverterHwEnableDown      = 0x00800000,
    E_PSSErrors_InverterForwardNotEnabled = 0x01000000,
    E_PSSErrors_InverterFault             = 0x02000000,
    E_PSSErrors_InverterInLocalMode       = 0x04000000,
    E_PSSErrors_DeviceExceedsDelta        = 0x08000000,
};

enum E_PSSWarnings
{
    E_PSSWarnings_NoWarnings = 0x00000000,
    E_PSSWarnings_ControlExceedsLimits = 0x00000001,
    E_PSSWarnings_PsocSystemModuleWarning = 0x00000002,
    E_PSSWarnings_PsocTemperatureModuleWarning = 0x00000004,
    E_PSSWarnings_PsocAnalogModuleWarning = 0x00000008,
    E_PSSWarnings_PsocMonitorModuleWarning = 0x00000010,
    E_PSSWarnings_PsocLargeTemperatureVariation = 0x00000020,
    E_PSSWarnings_PsocMissingTemperatureSensor = 0x00000040,
    E_PSSWarnings_PsocSsrAlwaysOff = 0x00000080,
    E_PSSWarnings_PsocSsrHighTemperature = 0x00000100,
    E_PSSWarnings_PsocSafetyModuleWarning = 0x00000200,
    E_PSSWarnings_CurrentLoopBelowLimit = 0x00000400
};

enum E_TemperatureErrorMessages
{
    E_TemperatureZone_ValueHighWarning = 0x10000,
    E_TemperatureZone_ValueHighError,
    E_TemperatureZone_ValueLowWarning,
    E_TemperatureZone_ValueLowError
};

enum E_ValueType
{
    E_ValueType_Bool = 8,
    E_ValueType_S8 = 1,
    E_ValueType_S16 = 2,
    E_ValueType_S32 = 3,
    E_ValueType_U8 = 4,
    E_ValueType_U16 = 5,
    E_ValueType_U32 = 6,
    E_ValueType_Float = 7,
};

union T_ValueTypeUnion
{
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    int8_t s8;
    int16_t s16;
    int32_t s32;
    float f;
};

#pragma pack(1)

#define __int64 long long
#define Boolean bool

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

struct PscMessageIdSplit
{
    unsigned short id;
    char dst;
    char src;
};

union PscMessageId
{
    struct PscMessageIdSplit split;
    unsigned long full;
};

struct PscMessageHeader
{
    union PscMessageId id;
    unsigned short length;
    unsigned int sn;
    unsigned int crc;
};

struct PscMessageStruct
{
    struct PscMessageHeader header;
    union PSSMsgType payload;
};

// restore default member packing.
#pragma pack()

#endif /* PSC_MESSAGESTRUCTS_H_ */
