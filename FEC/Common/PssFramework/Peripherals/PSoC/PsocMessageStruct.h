/*
 * PsocMessageStruct.h
 *
 *  Created on: 20 бреб 2013
 *      Author: maora
 */

#ifndef PSOCMESSAGESTRUCT_H_
#define PSOCMESSAGESTRUCT_H_

#include <stdint.h>

#pragma pack(1)

#define M_PSOC_SOH 1

// TODO: Consider consolidating the SPI Error and the Ack Status enums.
enum E_PsocSpiError
{
    E_PsocSpiError_Ok = 0,
    E_PsocSpiError_InvalidState = 0x1,
    E_PsocSpiError_InvalidCommand = 0x2,
    E_PsocSpiError_InvalidSOH = 0x4,
    E_PsocSpiError_CrcError = 0x8,
    E_PsocSpiError_InvalidDeviceIndex = 0x10,
    E_PsocSpiError_InvalidDevice = 0x20,
    E_PsocSpiError_InvalidType = 0x40,
    E_PsocSpiError_InvalidValue = 0x80,
    E_PsocSpiError_NoSlaveResponse = 0x100,
    E_PsocSpiError_UnexpectedError = 0x200,
    E_PsocSpiError_BadSerialNumber = 0x400,
    E_PsocSpiError_BadCableId = 0x800,
    E_PsocSpiError_NoSpiResponse = 0x1000,
    E_PsocSpiError_NoAvailableSemaphore = 0x2000,
    E_PsocSpiError_HandlerFrameOverwritten = 0x4000,
    E_PsocSpiError_NotImplemented = 0x8000,
    E_PsocSpiError_UnknownError = 0xffff,
};

/*
 ////////////////////////////////////////////////////////////////
 //////////////////////////
 // Status Soft Register //
 //////////////////////////
 ////////////////////////////////////////////////////////////////
 #define INIT_STATE_STS              0x0001
 #define PSOC_OFF_LINE_STS           0x0002
 #define SYS_CONFIG_MOD_STS          0x0004
 #define SYS_OP_MOD_STS              0x0008
 //#define XXXXXXXXXX_STATUS         0x0010
 #define SYSTEM_MDL_WARN_STS         0x0020
 #define TEMP_MDL_WARN_STS           0x0040
 #define ANALOG_MDL_WARN_STS         0x0080
 #define MONITOR_MDL_WARN_STS        0x0100
 #define SAFETY_MDL_WARN_STS         0x0200
 #define SYSTEM_MDL_ERR_STS          0x0400
 #define TEMP_MDL_ERR_STS            0x0800
 #define ANALOG_MDL_ERR_STS          0x1000
 #define MONITOR_MDL_ERR_STS         0x2000
 #define SAFETY_MDL_ERR_STS          0x4000
 //#define XXXXXXXXXX_STATUS         0x8000
 *
 */
enum E_PsocStatusBits
{
    E_PsocStatusBits_PsocOffline = 0x1,
    E_PsocStatusBits_SystemNotInitialized = 0x2,
    E_PsocStatusBits_BITFailed = 0x4,
    E_PsocStatusBits_SystemNotConfigured = 0x8,
    E_PsocStatusBits_SystemModuleWarning = 0x20,
    E_PsocStatusBits_TemperatureModuleWarning = 0x40,
    E_PsocStatusBits_AnalogModuleWarning = 0x80,
    E_PsocStatusBits_MonitorModuleWarning = 0x100,
    E_PsocStatusBits_SafetyModuleWarning = 0x200,
    E_PsocStatusBits_SystemModuleError = 0x400,
    E_PsocStatusBits_TemperatureModuleError = 0x800,
    E_PsocStatusBits_AnalogModuleError = 0x1000,
    E_PsocStatusBits_MonitorModuleError = 0x2000,
    E_PsocStatusBits_SafetyModuleError = 0x4000,
};

#define M_STATUS_BITS_WARNING_MASK ((uint16_t)E_PsocStatusBits_SystemModuleWarning | \
        E_PsocStatusBits_TemperatureModuleWarning | \
        E_PsocStatusBits_AnalogModuleWarning | \
        E_PsocStatusBits_MonitorModuleWarning | \
        E_PsocStatusBits_SafetyModuleWarning)

#define M_STATUS_BITS_ERROR_MASK ((uint16_t)E_PsocStatusBits_SystemModuleError | \
        E_PsocStatusBits_TemperatureModuleError | \
        E_PsocStatusBits_AnalogModuleError | \
        E_PsocStatusBits_MonitorModuleError | \
        E_PsocStatusBits_SafetyModuleError)

#define M_STATUS_BITS_OPERATION_STATE ((uint16_t)E_PsocStatusBits_PsocOffline | \
        E_PsocStatusBits_SystemNotInitialized | \
        E_PsocStatusBits_BITFailed | \
        E_PsocStatusBits_SystemNotConfigured)

/*
 ////////////////////////////////////////////////////////////////
 /////////////////////////
 // Error Soft Register //
 /////////////////////////
 ////////////////////////////////////////////////////////////////


 #define RELAY_OPEN_ERR              0x0001
 //#define XXXXXXXXXXXXXX_ERR        0x0002
 #define PSoC_RTD_OVER_TEMP_ERR      0x0004
 #define CABLES_ID_CONFLICT_ERR      0x0008
 //#define XXXXXXXXXXXXXX_ERR        0x0010
 #define MISSING_RTD_ERR             0x0020
 #define RTD_CAL_RES_FAULT_ERR       0x0040
 #define MMCU_RTD_OVER_TEMP_ERR      0x0080
 #define SSR_OVER_TEMP_ERR           0x0100
 //#define XXXXXXXXXXXXXX_ERR        0x0200
 #define SSR_OVER_CURRENT_ERR        0x0400
 #define SSR_ALWAYS_ON_ERR           0x0800
 //#define XXXXXXXXXXXXXX_ERR        0x1000
 //#define XXXXXXXXXXXXXX_ERR        0x2000
 //#define XXXXXXXXXXXXXX_ERR        0x4000
 //#define XXXXXXXXXXXXXX_ERR        0x8000
 */

enum E_PsocErrorBits
{
    E_PsocErrorBits_SafetyRelayLow = 0x1,
    E_PsocErrorBits_HardTemperatureLimit = 0x4,
    E_PsocErrorBits_CableIdNotMatching = 0x8,
    E_PsocErrorBits_Pt100MissingSensor = 0x20,
    E_PsocErrorBits_Pt100CalResistorError = 0x40,
    E_PsocErrorBits_SoftTemperatureLimit = 0x80,
    E_PsocErrorBits_SsrOverheating = 0x100,
    E_PsocErrorBits_MissingCurrentSensor = 0x200,
    E_PsocErrorBits_OverCurrentLimit = 0x400,
    E_PsocErrorBits_SsrAlwaysOn = 0x800,
    E_PsocErrorBits_PrintBarLifted = 0x2000,
    E_PsocErrorBits_LiquidDetectorError = 0x4000
};

/*
 ////////////////////////////////////////////////////////////////
 ///////////////////////////
 // Warning Soft Register //
 ////////////////////////////////////////////////////////////////
 //#define XXXXXXXXXXXXXX_WARN       0x0001
 //#define XXXXXXXXXXXXXX_WARN       0x0002
 //#define XXXXXXXXXXXXXX_WARN       0x0004
 //#define XXXXXXXXXXXXXX_WARN       0x0008
 //#define XXXXXXXXXXXXXX_WARN       0x0010
 //#define XXXXXXXXXXXXXX_WARN       0x0020
 //#define XXXXXXXXXXXXXX_WARN       0x0040
 //#define XXXXXXXXXXXXXX_WARN       0x0080
 #define SSR_HIGH_TEMP_WARN          0x0100
 //#define XXXXXXXXXXXXXX_WARN       0x0200
 //#define XXXXXXXXXXXXXX_WARN       0x0400
 #define SSR_ALWAYS_OFF_WARN         0x0800
 //#define XXXXXXXXXXXXXX_WARN       0x1000
 //#define XXXXXXXXXXXXXX_WARN       0x2000
 //#define XXXXXXXXXXXXXX_WARN       0x4000
 //#define XXXXXXXXXXXXXX_WARN       0x8000

 */
enum E_PsocWarningBits
{
    E_PsocWarningBits_SsrOverheating = 0x100,
    //E_PsocWarningBits_MissingCurrentSensor = 0x400,
    E_PsocWarningBits_SsrAlwaysOff = 0x800,
//E_PsocWarningBits_MissingTemperatureSensor = 0x2,
};

enum E_PsocPwmType
{
    E_PsocPwmType_Size
};

enum E_PsocTemperatureSensorType
{
    E_PsocTemperatureSensorType_PT100,
    E_PsocTemperatureSensorType_PT500,
    E_PsocTemperatureSensorType_PT1000,
    E_PsocTemperatureSensorType_NTC,
    E_PsocTemperatureSensorType_LiquidDetector
};

enum E_PsocTemperatureSensorFilterType
{
    E_PsocTemperatureSensorFilterType_SinglePoleIIR,
};

enum E_PsocDigitalIoDriveMode
{
    E_PsocDigitalIoDriveMode_Size,
};

enum E_PsocDeviceType
{
    E_PsocDeviceType_TemperatureSensor = 0x00,
    E_PsocDeviceType_TemperaturePWM = 0x01,
    E_PsocDeviceType_AnalogInput = 0x02,
    E_PsocDeviceType_AnalogOutput = 0x03,
    E_PsocDeviceType_BridgeFeedback = 0x04,
    E_PsocDeviceType_CurrentLoop = 0x05,
    E_PsocDeviceType_ConnectorIDs = 0x06,
    E_PsocDeviceType_DigitalPWM = 0x07,
    E_PsocDeviceType_H_BridgePWM = 0x08,
    E_PsocDeviceType_DigitalInputs = 0x09,
    E_PsocDeviceType_Size
};

enum E_PsocCommands
{
    /* System Messages */
    E_PsocCommands_EmptyCommand = 0x00,
    E_PsocCommands_GetPSoCversion = 0x01,
    E_PsocCommands_KeepAlive = 0x02,
    E_PsocCommands_GetCableID = 0x03,
    E_PsocCommands_GetStatus = 0x04,
    E_PsocCommands_StartBoardConfig = 0x05,
    E_PsocCommands_EndBoardConfig = 0x06,
    E_PsocCommands_SetErrorsMask = 0x07,
    E_PsocCommands_SetWarningsMask = 0x08,
    E_PsocCommands_GetErrors = 0x09,
    E_PsocCommands_GetWarnings = 0x0A,

    /* Configuration Messages */
    E_PsocCommands_ConfigPWMs = 0x20,
    E_PsocCommands_ConfigTemperatureSensor = 0x21,
    E_PsocCommands_ConfigCurrentLoop = 0x22,
    E_PsocCommands_ConfigDigitalIO = 0x23,
    E_PsocCommands_ConfigAnalogIO = 0x24,
    E_PsocCommands_ConfigChannelFunction = 0x25,

    /* I/O Operation messages*/
    E_PsocCommands_ReadSingleDevice = 0x40,
    E_PsocCommands_WriteSingleDevice = 0x41,
    E_PsocCommands_WriteMultipleDevice = 0x42,
    E_PsocCommands_GetDeviceErrors = 0x43,
    E_PsocCommands_GetDeviceWarnings = 0x44,

    /* Group reading */
    E_PsocCommands_ReadTemperatureSensors = 0x45,
    E_PsocCommands_ReadPWMs = 0x46,
    E_PsocCommands_ReadDigitalIO = 0x47,
    E_PsocCommands_ReadAnalogInputs = 0x48,
    E_PsocCommands_ReadBridgeCurrents = 0x49,
    E_PsocCommands_ReadCurrentLoops = 0x50,
    E_PsocCommands_ReadConnectorsID = 0x51,
    E_PsocCommands_ReadDigitalInputs = 0x52,
    E_PsocCommands_ReadTemperaturePwmDISensors = 0x53,
    E_PsocCommands_ReadAnalogsFeedbacksErrors = 0x55,
    /* Operational Messages */
    E_PsocCommands_ResetBoardToON = 0x0C,
    E_PsocCommands_StartErrorRecovery = 0x0D,
    E_PsocCommands_ResetPSoC = 0xF0,
    E_PsocCommands_StartBootloader = 0xF1
};

enum E_PsocPrimaryFunction
{

    /// <remarks/>
    E_PsocPrimaryFunction_Undefined,

    /// <remarks/>
    E_PsocPrimaryFunction_HwValidation,

    /// <remarks/>
    E_PsocPrimaryFunction_SoftDebug,

    /// <remarks/>
    E_PsocPrimaryFunction_Heater,

    /// <remarks/>
    E_PsocPrimaryFunction_LiftPbOnError,

    /// <remarks/>
    E_PsocPrimaryFunction_Applicator,
    /// <remarks/>
    E_PsocPrimaryFunction_LiftPbOnErrorGen2,

    E_PsocPrimaryFunction_LiftPbOnErrorGen3,
};

enum E_DigitalIOType
{
    E_DigitalIOType_RandomPWM, E_DigitalIOType_DistributedPWM,
//    E_DigitalIOType_SlowPWM,
    E_DigitalIOType_PWM,
    E_DigitalIOType_DigitalOut,
};

//enum E_TemperatureSensorType
//{
//    E_PT100 = 0x00
////E_PT500,
////E_PT1000,
////E_NTC
//};
//
//enum E_TemperatureFilterType
//{
//    E_SinglePoleIIR = 0x00
////E_MovingAverage,
////E_GroupAverage
//};

enum E_Module
{
    E_TemperatureModule = 0x00, E_AnalogModule = 0x01
};

struct T_MessageHeader
{
    uint8_t start;
    uint8_t commandStatus;
    uint8_t serialNumber;
};

struct T_ConfigTemperatureSensor
{
    uint8_t channelIndex;
    uint8_t sensorType;
    uint8_t filterType;
    float hardLimit;
    float aCoeff;
    float bCoeff;
    uint8_t missingSensorPriority;
};

struct T_ConfigCurrentLoop
{
    uint8_t channelIndex;
    uint16_t minCurrentLimit;
    uint16_t maxCurrentLimit;
};

struct T_ConfigDigitalIO
{
    uint8_t channelIndex;
    E_DigitalIOType channelType;
    uint8_t RiseStepSize;
    uint8_t FallStepSize;
    uint8_t RiseStepTickCount;
    uint8_t FallStepTickCount;
};

struct T_ConfigChannelFunction
{
    uint8_t primaryFunction;
};

struct T_ReadSingleDevice
{
    E_Module module;
    uint8_t channelIndex;
};

#ifdef ReadMultipleDevices
struct T_ReadMultipleDevices
{
    uint8_t NumOfModules;
    E_Module Module;
};
#endif

struct T_WriteSingleDevice
{
    E_Module module;
    uint8_t channelIndex;
    uint16_t value;
};

struct T_VersionStruct
{
    uint16_t major;
    uint16_t minor;
    uint16_t build;
    uint16_t revision;
};

union T_VersionUnion
{
    T_VersionStruct split;
    unsigned long long full;
};

struct T_PsocVersion
{
//    T_Version firmwareVersion;
//    T_Version protocolVersion;
    T_VersionUnion firmwareVersion;
    T_VersionUnion protocolVersion;
    uint8_t boardType;
};

struct T_PsocRequestWithVoltage
{
    float voltage;
};

struct T_PsocCableIdReply
{
    uint8_t input1Id;
    uint8_t input2Id;
    uint8_t output1Id;
    uint8_t errorBits;
};

struct T_PsocStatusReply
{
    E_PsocStatusBits statusBits :16;
};

struct T_PsocGetErrorsReply
{
    E_PsocErrorBits errorBits :16;
};

struct T_PsocGetWarningsReply
{
    E_PsocWarningBits warningBits :16;
};

#define M_NUM_OF_PWM_OUTPUTS 6
#define M_NUM_OF_ANALOG_OUTPUTS 2

struct T_ReadTemperaturePwmDISensors
{
    float temperatureSensors[4];
    uint8_t temperatureErrorBits;
    uint8_t pwmOutputsReported[M_NUM_OF_PWM_OUTPUTS]; //!< This is the reported value of the PWM outputs
    uint8_t pwmErrorBits;
    uint16_t digitalInputBits;
    uint16_t systemStatus;
};

// Take note that the feedbacks are integers, in 0.1 resolution (meaning float * 10)
struct T_ReadAnalogsFeedbacksErrors
{
    uint16_t analogIn[2];
    uint8_t analogInErrorBits;
    uint16_t peakCurrent[2];
    uint8_t pwmCounts;
    uint8_t peakCurrentsErrorBits;
    uint16_t systemWarnings;
    uint16_t systemStatus;
    uint16_t systemErrors;
    uint16_t analogOut[2];
};

struct T_WriteMultipleDeviceValues
{
    uint8_t writeBitfield;
    uint8_t analogOutputs[2];
    uint8_t digitalOutputs[6];
};

struct T_WriteMultipleDevice
{
    uint8_t Device;
    T_WriteMultipleDeviceValues WriteMultipleDeviceValues;
};

union T_MessageData
{
    T_ConfigTemperatureSensor configTemperatureSensor;
//    T_ReadSingleDevice readSingleDevice;
//    T_WriteSingleDevice writeSingleDevice;
    T_PsocVersion psocVersion;
    T_PsocRequestWithVoltage psocRequestWithVoltage;
    T_PsocCableIdReply psocCableIdReply;
    T_PsocStatusReply psocStatus;
    T_PsocGetErrorsReply psocErrors;
    T_PsocGetWarningsReply psocWarnings;
    T_ReadTemperaturePwmDISensors readTemperaturePwmDISensors;
    T_ReadAnalogsFeedbacksErrors readAnalogsFeedbacksErrors;
    T_WriteMultipleDeviceValues writeMultipleDevices;
    T_ConfigCurrentLoop configCurrentLoop;
    T_ConfigDigitalIO configDigitalIO;
    T_ConfigChannelFunction configChannelFunction;
#ifdef ReadMultipleDevices
    T_ReadMultipleDevices ReadMultipleDevices;
#endif
    uint8_t uint8Arr[28];
    uint16_t uint16Arr[14];
    uint32_t uint32Arr[7];
    float floatArr[7];
};

struct T_MessagePacket
{
    T_MessageHeader header;
    T_MessageData data;
    //uint8_t spareByte;
    // TODO: Check if this is needed, since we use hardware CRC:
//    uint8_t crc;
};

#pragma pack()

#endif /* PSOCMESSAGESTRUCT_H_ */
