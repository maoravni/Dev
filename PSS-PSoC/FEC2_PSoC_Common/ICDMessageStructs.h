/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

//////////////
// Includes //
//////////////
#include <stdio.h>
#include "device.h"
//#include <Temperature_Module_Handler.h>

/////////////
// Defines //
/////////////

#define DATA_BUFFER_SIZE 	28
///////////////////////////////////////////////////////////////////////////////////////////
// ICD //
//////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(1)

////////////////////////////////////////////////////////////////////////////////////////////////
// Enumerates //
////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////
// Message Status List //
/////////////////////////
typedef enum 
{
	E_Success				= 0x00,
	E_InvalidState			= 0x01,
	E_InvalidCommand		= 0x02,
	E_InvalidSOH			= 0x04,
	E_InvalidCRC			= 0x08,
	E_InvalidDeviceIndex 	= 0x10,
	E_InvalidDevice		 	= 0x20,
	E_InvalidType		 	= 0x40,
	E_InvalidValue		 	= 0x80,
} E_MessageStatus;
///////////////////
// Commands List //
///////////////////

typedef enum 
{
	/* System Messages */
	E_EmptyCommand					= 0x00,
	E_GetPSoCversion				= 0x01,
	E_KeepAlive						= 0x02,
	E_GetCableID					= 0x03,
	E_GetStatus						= 0x04,
	E_StartBoardConfig				= 0x05,
	E_EndBoardConfig				= 0x06,
	E_SetErrorsMask					= 0x07,
	E_SetWarningsMask				= 0x08,
	E_GetErrors						= 0x09,
	E_GetWarnings					= 0x0A,
	/* Configuration Messages */
	E_ConfigTemperatureSensor 		= 0x21,
	E_ConfigCurrentSensor			= 0x22,
	E_ConfigDigitalIO		 		= 0x23,
	E_ConfigAnalogIO		 		= 0x24,
	E_ConfigChannelType				= 0x25,
	
	/* I/O Operation messages*/
	E_ReadSingleDevice				= 0x40,
	E_WriteSingleDevice				= 0x41,
	E_WriteMultipleDevice			= 0x42,
	E_GetDeviceErrors				= 0x43,
	E_GetDeviceWarnings				= 0x44,
	
	/* Group reading */
	E_ReadTemperatureSensors		= 0x45,
	E_ReadPWMs						= 0x46,
	E_ReadAnalogInputs				= 0x48,
	E_ReadBridgeCurrents			= 0x49,
	E_ReadCurrentSensor		  	= 0x50,
	E_ReadDigitalInputs				= 0x52,
    E_ReadTemperaturePwmDISensors	= 0x53,
	E_ReadTemperaturePacket			= 0x54,
	E_ReadAnalogsFeedbacksErrors	= 0x55,
	/* Operational Messages */
	E_ResetBoardToON				= 0x0C,
	E_StartPSoCErrorRecovery		= 0x0D,
	E_ResetPSoC						= 0xF0,
	E_StartBootloader				= 0xF1

} E_Commands;

/////////////////////////////////////////////////////////////////////////////

/* PWM Group number */
typedef enum 
{
	E_Group1		= 0x01,
	E_Group2		= 0x02
} E_PWMGroup;

///////////////////////////////////////
// CONFIG_TEMPERATURE_SENSOR Enum's //
//////////////////////////////////////
/* Temperature Sensor Type */
typedef enum 
{
	E_PT100 			= 0x00,
	E_PT500 			= 0x01,
	E_PT1000			= 0x02,
	E_NTC				= 0x03,
	E_Liquid_Detector 	= 0x04
} E_TemperatureSensorType;

/* Temperature Filter Type */
typedef enum 
{
	E_SinglePoleIIR = 0x00,
	E_MovingAverage	= 0x01,
	E_GroupAverage	= 0x02
} E_TemperatureFilterType;

//////////////////////////////
// CONFIG DIGITAL IO Enum's //
//////////////////////////////
typedef enum 
{
	E_PrISM		= 0x00,
	E_SPWM		= 0x01,
	E_PWM		= 0x02,
	E_DIGI_OUT	= 0x03
} E_ChannelDriveMode;

//////////////////////////////
// READ SINGLE DEVICEEnum's //
//////////////////////////////
typedef enum 
{
	E_TemperatureSensor = 0x00,
	E_AnalogInput 		= 0x01,
	E_ThermistorInput 	= 0x03,
	E_CurrentSensor 	= 0x04,
	E_CableIDs 			= 0x05,
	E_DigitalInputs 	= 0x06,
	E_AnalogOutput 		= 0x09,
	E_DigitalOutputs	= 0x0A
} E_Device;


///////////////////////////////////
// WRITE MULTIPLE DEVICES Enum's //
///////////////////////////////////
typedef enum 
{
	E_AnalogInput1		= 0x01,
	E_AnalogInput2		= 0x02,
	E_SlowPWM_Output1	= 0x04,
	E_SlowPWM_Output2	= 0x08,
	E_PWM_Output1 		= 0x10,
	E_PWM_Output2 		= 0x20,
	E_HB_PWM_Output1 	= 0x40,
	E_HB_PWM_Output2 	= 0x80
} E_OutputToWrite;

////////////////////////////////////////////////////////////////////////////////////////////////
// Structures //
////////////////////////////////////////////////////////////////////////////////////////////////

/* Header Structure */
typedef struct 
{
	uint8 Start;
	uint8 Command_Status;
} T_MessageHeader;

/* GET VERSION REPLAY */
typedef struct 
{
	uint8 	FirmwareVersion;
	uint8 	ProtocolVersion;
} T_GetVersionReply;

/* GET CABLE ID REPLAY */
typedef struct 
{
	uint8 	CableIDs[3];
	uint8   StatusBits;
} T_GetCableIDReply;

/* CONFIG TEMPERATURE SENSOR */
typedef struct 
{
	uint8 						ChannelIndex;
	E_TemperatureSensorType 	SensorType;
	E_TemperatureFilterType 	FilterType;
	float 						TempUpLimit;
	float						Gain_LiquidDetectTimer;
	float						Offset;
} T_ConfigTemperatureSensor;

/* CONFIG CURRENT SENSOR */
typedef struct 
{
	uint8 						ChannelIndex;
	uint16 						CurrentLowerLimit;
	uint16						CurrentUpperLimit;
} T_ConfigCurrentSensor;

/* CONFIG DIGITAL IO */
typedef struct 
{
	uint8 						ChannelIndex;
	uint8						ChannelType;
	uint8                       RiseStepSize;
	uint8                       FallStepSize;
	uint8                       RiseStepTickCount;
	uint8                       FallStepTickCount;
} T_ConfigDigitalIO;

/* CONFIG ANALOG IO */
typedef struct 
{
	uint8 						ChannelIndex;
	uint16						CurrentLowerLimit;
	uint16						CurrentUpperLimit;
} T_ConfigAnalogIO;

/* CONFIG PRIMARY FUNCTION */
typedef struct 
{
	uint8						PrimaryFunction;	
} T_ConfigPrimaryFunction;

/* READ SINGLE DEVICE */
typedef struct
{
	E_Device					Device;
	uint8						ChannelIndex;
} T_ReadSingleDevice;

typedef union
{
	/* digital inputs */
	float  TemperatureSensor;
	uint16 AnalogInput[2];
	uint16 ThermistorInput[2];
	uint16 CurrentSensor[2];
	uint8  CableID[4];
	uint8  DigitalInputs[4];
	uint8  AnalogOutput[4];
	uint8  DigitalOutputS[4];	
}U_SingleDeviceValue;

/* READ SINGLE DEVICE REPLY */
typedef struct
{
	U_SingleDeviceValue	Value; 						
	uint8				SensorsState;
} T_ReadSingleDeviceReply;

/* WRITE SINGLE DEVICE */
typedef struct
{
	E_Device	Device;
	uint8		ChannelIndex;
	uint8		Value;
} T_WriteSingleDevice;

/* WRITE MULTIPLE DEVICE */
typedef struct
{
	uint8 WriteBitfield;
	uint8 AnalogOutputs[2];
	uint8 DigitalOutput[6];
} T_WriteMultipleDevice;

/* READ TEMPERATURE SENSORS */
typedef struct
{
    float temperatureSensor[4];
    uint8 StatusBits;
} T_ReadTemperatureSensors;

/* READ PWMs */
typedef struct
{
    uint8 OutputChannel[6];
	uint8 StatusBits;
} T_ReadPWMs;

/* READ ANALOG INPUTS */
typedef struct
{
    uint16 AnalogInputs[2];
	uint8  StatusBits;
	uint16 ThermistorInput;
	
} T_ReadAnalogInputs;

/* READ CURRENT LOOPS */
typedef struct
{
    uint16 PeakCurrent[2];
	uint8  HeatCntrlPulseCount;
	uint8  StatusBits;
} T_ReadCurrentSensor;

/* READ DIGITAL INPUTS */
typedef struct
{
    uint16 DI1:1;
	uint16 DI2:1;
	uint16 DI3:1;
	uint16 DI4:1;
	uint16 DI5:1;
	uint16 DI6:1;
	
	uint16 GI1:1;
	uint16 GI2:1;
	uint16 GI3:1;
	uint16 GI4:1;
	uint16 GI5:1;
	uint16 GI6:1;
	
	uint16 Spare:4;
} T_DigitalInputsBits;

typedef union
{
    uint16 				DigitalInputsWord;
	T_DigitalInputsBits DigitalInputsBits;
} U_DigitalInputsData;

typedef struct
{
    U_DigitalInputsData DigitalInputsData;
	uint8 				StatusBits;
} T_ReadDigitalInputs;

/* READ TEMPERATURE SENSORS, PWMs, DIGITAL INPUTS */
typedef struct
{
    float  temperatureSensors[4];
    uint8  temperatureStatusBits;
    uint8  pwmOutputs[6];
    uint8  pwmStatusBits;
    uint16 DigitalInputBits;
    uint16 systemStatus;
} T_ReadTemperaturePwmDISensors;

/* READ TEMPERATURE PACKET */
typedef struct
{
    float  temperatureSensors[3];
    int16  PeakCurrent[2];
	uint8  CS_Counter;
    uint16 SystemStatus;
    uint16 SystemWarnings;
    uint16 SystemErrors;
	uint8  SlowPWM1;
	uint8  InputCableID1;
} T_ReadTemperaturePacket;

/* READ ANALOG FEEDBACKS, ERRORS */
// Take note that the feedbacks are integers, in 0.1 resolution (meaning float * 10)
typedef struct
{
    uint16 analogIn[2];
    uint8  analogInStatusBits;
    int16  PeakCurrent[2];
	uint8  HeatCntrlPulseCount;
    uint8  CurrentSensorStatusBits;
    uint16 SystemWarnings;
    uint16 SystemStatus;
    uint16 SystemErrors;
	uint16 analogOut[2];
} T_ReadAnalogsFeedbacksErrors;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Final massage structures and Data types union
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Data Types Union */
typedef union 
{
	T_GetVersionReply				GetVersionReply;
	T_GetCableIDReply				GetCableIDReply;
	T_ConfigTemperatureSensor		ConfigTemperatureSensor;
	T_ConfigCurrentSensor				ConfigCurrentSensor;
	T_ConfigDigitalIO				ConfigDigitalIO;
	T_ConfigAnalogIO				ConfigAnalogIO;
	T_ConfigPrimaryFunction			ConfigPrimaryFunction;
	T_ReadSingleDevice				ReadSingleDevice;
	T_ReadSingleDeviceReply			ReadSingleDeviceReply;
	T_WriteSingleDevice				WriteSingleDevice;
	T_WriteMultipleDevice			WriteMultipleDevice;
	T_ReadTemperatureSensors		ReadTemperatureSensors;
	T_ReadPWMs						ReadPWMs;
	T_ReadAnalogInputs				ReadAnalogInputs;
	T_ReadCurrentSensor				ReadCurrentSensor;
	T_ReadDigitalInputs				ReadDigitalInputs;
	T_ReadTemperaturePwmDISensors   ReadTemperaturePwmDISensors;
	T_ReadTemperaturePacket			ReadTemperaturePacket;
	T_ReadAnalogsFeedbacksErrors    ReadAnalogsFeedbacksErrors;
	
	uint8 							DataBytes_uint8[DATA_BUFFER_SIZE];
	uint16 							DataBytes_uint16[DATA_BUFFER_SIZE/2];
	int16 							DataBytes_int16[DATA_BUFFER_SIZE/2];
	float 							DataBytes_float[DATA_BUFFER_SIZE/4];
} T_MessageData;

/* Rx Massage Structure */
typedef struct 
{
	uint8 			Start;
	uint8 			Command;
	uint8 			SerialNumber;
	T_MessageData	Data;
	uint8 			CRC;
} T_Rx_Buffer;


/* Tx Massage Structure */
typedef struct 
{
	uint8 			Status;
    uint8 			SerialNumber;
	T_MessageData 	Data;
	uint8 			crc;
} T_Tx_Buffer;

#pragma pack()

/***************************************
*    Function Prototypes
***************************************/

/*******************************************************************************
* Function Name: 
********************************************************************************
* Summary:
*   
*
* Parameters:
*   none
*
* Return:
*   none
*
*******************************************************************************/
/* General */
void Ex_EmptyCommand(void);
void Ex_GetPSoCversion(void);
void Ex_KeepAlive(void);
void Ex_GetCableID(void);
void Ex_GetStatus(void);
void Ex_SetErrorsMask(void);
void Ex_SetWarningsMask(void);
void Ex_GetErrors(void);
void Ex_GetWarnings(void);
void Ex_StartBoardConfig(void);
void Ex_EndBoardConfig(void);
/* configuration */
void Ex_ConfigTemperatureSensor(void);  
void Ex_ConfigCurrentSensor(void);
void Ex_ConfigDigitalIO(void);
void Ex_ConfigAnalogIO(void);
void Ex_ConfigChannelType(void);

/* Single Read/Write */
void Ex_ReadSingleDevice(void);
void Ex_WriteSingleDevice(void);
/* Write to multiple devices */
void Ex_WriteMultipleDevice(void);
/* Device State */
void Ex_GetDeviceErrors(void);
void Ex_GetDeviceWarnings(void);
/* Read multiple devices */
void Ex_ReadTemperatureSensors(void);
void Ex_ReadPWMs(void);
void Ex_ReadAnalogInputs(void);
void Ex_ReadBridgeCurrents(void);
void Ex_ReadCurrentSensor(void);
void Ex_ReadDigitalInputs(void);
void Ex_ReadTemperaturePwmDISensors(void);
void Ex_ReadAnalogsFeedbacksErrors(void);
void Ex_ReadTemperaturePacket(void);
/* Operation */
void Ex_ResetBoardToON(void);
void Ex_StartPSoCErrorRecovery(void);
void Ex_ResetPSoC(void);
void Ex_StartBootloader(void);

