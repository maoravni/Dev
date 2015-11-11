//*********************************************************
//                                                        *
//                  code generator V 1.0.1.0              *
//                                                        *
//   NOTE: this class is generated automatically          *
//         Do not add any code to this class, it will be  *
//         deleted on the next code generation.           *
//         Use part classes to implement additional logic *
//                                                        *
//*********************************************************

class BaseMsg
{
    virtual char* getFormatString() const {return ""}
};

class PSSActivateBTCControlMsg {
public:
	char boardId;
	char zoneId;
	short setPointTemperature;
	short minPower;
	short maxPower;
	short lowRangeLimit;
	short lowWarningLimit;
	short highRangeLimit;
	short highWarningLimit;
	};

class PSSActivateEngageSystemMsg {
public:
	char boardId;
	char deviceId;
	char position;
	};

class PSSBTCControlStatusNotificationMsg {
public:
	char boardId;
	char deviceId;
	short powerLimit;
	short temperature;
	char status;
	};

class PSSGetBTCControlStatusMsg {
public:
	char boardId;
	char deviceId;
	};

class PSSGetPIDMsg {
public:
	char boardId;
	char zoneId;
	};

class PSSPIDValuesNotificationMsg {
public:
	char boardId;
	char zoneId;
	Single p;
	Single i;
	Single d;
	};

class PSSResetBoardMsg {
public:
	char boardId;
	};

class PSSSetBTCControlConfigMsg {
public:
	char boardId;
	char zoneId;
	short maxTemp;
	char index;
	char fillPumpIndex;
	};

class PSSActivateMonitoringMsg {
public:
	unsigned short interval;
	char enable;
	char boardId;
	char zoneId;
	unsigned short dataDefinition;
	};

class PSSActivatePumpMsg {
public:
	char boardId;
	char deviceId;
	short powerLimit;
	unsigned int duration;
	};

class PSSActivateZoneMsg {
public:
	char boardId;
	char zoneId;
	char activate;
	};

class PSSAutoTuneMsg {
public:
	char boardId;
	char zoneId;
	short setPoint;
	short overShoot;
	short powerAtStart;
	short powerStep;
	short calculation;
	};

class PSSBootLoaderMsg {
public:
	char boardId;
	};

class PSSEnableMsg {
public:
	char boardId;
	char zoneId;
	};

class PSSEnableRegistersNotificationMsg {
public:
	char enable;
	unsigned short interval;
	};

class PSSEngageSystemStatusNotificationMsg {
public:
	char boardId;
	char deviceId;
	char position;
	short valveState;
	char sensor1State;
	char sensor2State;
	char sensor3State;
	char sensor4State;
	char status;
	};

class PSSErrorNotificationMsg {
public:
	char boardId;
	char zoneId;
	unsigned int errors;
	};

class PSSFlowMeterStatusNotificationMsg {
public:
	char boardId;
	char deviceId;
	short flow;
	char status;
	};

class PSSGetAirBleedingZoneConfigMsg {
public:
	char boardId;
	char zoneId;
	};

class PSSSetAirBleedingZoneConfigMsg {
public:
	char boardId;
	char zoneId;
	unsigned short pidCalcPeriod;
	char measurmentsAverage;
	short convertMultiplier;
	short convertDivider;
	short convertOffset;
	char alarmDelay;
	unsigned int functionId;
	unsigned int peripheralType;
	char index;
	};

class PSSGetAirBleedingZoneConfigReplyMsg {
public:
	};

class PSSGetBlowerZoneConfigMsg {
public:
	char boardId;
	char zoneId;
	};

class PSSSetBlowerZoneConfigMsg {
public:
	char boardId;
	char zoneId;
	unsigned short pidCalcPeriod;
	char measurmentsAverage;
	char priority;
	char setPhaseConnectionPosition;
	short convertMultiplier;
	short convertDivider;
	short convertOffset;
	char alarmDelay;
	short maxPower;
	short ratedPower;
	short setPointRatio;
	unsigned short powerConsumption;
	unsigned int functionId;
	unsigned int peripheralType;
	short maxSpeed;
	char index;
	};

class PSSGetBlowerZoneConfigReplyMsg {
public:
	};

class PSSGetControllerConfigMsg {
public:
	char boardId;
	};

class PSSSetControllerConfigMsg {
public:
	char boardId;
	char masterControllerId;
	unsigned __int64 powerConsumption;
	};

class PSSGetControllerConfigReplyMsg {
public:
	};

class PSSGetEngageSystemStatusMsg {
public:
	char boardId;
	char deviceId;
	};

class PSSGetErrorsMsg {
public:
	char boardId;
	char zoneId;
	};

class PSSSetFanZoneConfigMsg {
public:
	char boardId;
	char zoneId;
	char index;
	};

class PSSGetFanZoneConfigReplyMsg {
public:
	};

class PSSGetFanZoneConfigMsg {
public:
	char boardId;
	char zoneId;
	};

class PSSGetFlowMeterStatusMsg {
public:
	char boardId;
	char deviceId;
	};

class PSSGetDigitalSensorStatusMsg {
public:
	char boardId;
	char deviceId;
	};

class PSSGetPressureSensorStatusMsg {
public:
	char boardId;
	char deviceId;
	};

class PSSGetPumpStatusMsg {
public:
	char boardId;
	char deviceId;
	};

class PSSGetTankLevelStatusMsg {
public:
	char boardId;
	char deviceId;
	};

class PSSSetTemperatureSensorZoneConfigMsg {
public:
	char boardId;
	char zoneId;
	unsigned short pidCalcPeriod;
	char measurmentsAverage;
	short convertMultiplier;
	short convertDivider;
	short convertOffset;
	char alarmDelay;
	unsigned int functionId;
	unsigned int peripheralType;
	char index;
	};

class PSSGetTemperatureSensorZoneConfigReplyMsg {
public:
	};

class PSSGetTemperatureSensorZoneConfigMsg {
public:
	char boardId;
	char zoneId;
	};

class PSSGetValveStatusMsg {
public:
	char boardId;
	char deviceId;
	};

class PSSGetVersionMsg {
public:
	char boardId;
    virtual char* getFormatString() const {return "GetVersion %d"}
	};

class PSSGetVersionReplyMsg {
public:
	char boardId;
	unsigned __int64 firmwareVersion;
	unsigned __int64 protocolVersion;
	};

class PSSGetWarningsMsg {
public:
	char boardId;
	char zoneId;
	};

class PSSGetTemperatureZoneConfigMsg {
public:
	char boardId;
	char zoneId;
	};

class PSSSetTemperatureZoneConfigMsg {
public:
	char boardId;
	char zoneId;
	unsigned short pidCalcPeriod;
	char measurmentsAverage;
	char priority;
	char setPhaseConnectionPosition;
	short convertMultiplier;
	short convertDivider;
	short convertOffset;
	char alarmDelay;
	short maxPower;
	short ratedPower;
	short setPointRatio;
	unsigned short powerConsumption;
	unsigned int functionId;
	unsigned int peripheralType;
	short maxTemperature;
	char index;
	};

class PSSGetTemperatureZoneConfigReplyMsg {
public:
	};

class PSSGetWaterContaminationStatusMsg {
public:
	char boardId;
	char deviceId;
	};

class PSSDigitalSensorStatusNotificationMsg {
public:
	char boardId;
	char deviceId;
	char state;
	char status;
	};

class PSSSetLoggingLevelMsg {
public:
	char boardId;
	char logLevel;
	char taskId;
	char enableLogging;
	};

class PSSMonitoringDataNotificationMsg {
public:
	char boardId;
	char zoneId;
	unsigned int dataSpecification;
	unsigned short dataLength;
	Byte data[25];
	};

class PSSActivateValveMsg {
public:
	char boardId;
	char zoneId;
	short valveState;
	};

class PSSPressureSensorStatusNotificationMsg {
public:
	char boardId;
	char deviceId;
	short pressure;
	char status;
	};

class PSSPumpStatusNotificationMsg {
public:
	char boardId;
	char deviceId;
	short powerLimit;
	char status;
	};

class PSSReadControllerDataReplyMsg {
public:
	char boardId;
	char zoneId;
	short temperature;
	short power;
	char status;
	};

class PSSAckMsg {
public:
	unsigned int originalRequestMsgId;
	unsigned int originalRequestSN;
	char boardId;
	char zoneId;
	unsigned int status;
	};

class PSSKeepAliveMsg {
public:
	};

class PSSKeepAliveReplyMsg {
public:
	};

class PSSReadControllerDataMsg {
public:
	char boardId;
	char zoneId;
	};

class PSSReadControllerSpecificDataMsg {
public:
	unsigned short dataToRead;
	char boardId;
	char zoneId;
	};

class PSSReadControllerSpecificDataReplyMsg {
public:
	unsigned short dataToRead;
	char boardId;
	char zoneId;
	unsigned short arrayLength;
	char* data;
	};

class PSSReadSystemDataMsg {
public:
	};

class TMCSystemData {
public:
	char boardId;
	unsigned __int64 firmwareVersion;
	unsigned __int64 protocolVersion;
	char masterControllerId;
	unsigned short availableTemperatureZones;
	unsigned short availableTemperatureSensorZones;
	unsigned short availableBlowerZones;
	unsigned short availableFanZones;
	unsigned short availableAirBleedingZones;
	};

class PSSReadSystemDataReplyMsg {
public:
	unsigned short listLength;
	ISystemDataNotificationData systemData[25];
	};

class PSSRegistersNotificationMsg {
public:
	unsigned short registerID;
	unsigned int value;
	};

class PSSSeqEndedMsg {
public:
	unsigned int originalRequestMsgId;
	unsigned int originalRequestSN;
	char boardId;
	char zoneId;
	unsigned int status;
	};

class PSSSetBlowerSpeedMsg {
public:
	char deviceType;
	char boardId;
	char zoneId;
	short speed;
	short lowWarningLimit;
	short lowRangeLimit;
	short highRangeLimit;
	short highWarningLimit;
	};

class PSSSetEngageSystemConfigurationMsg {
public:
	char boardId;
	char deviceId;
	char ioIndex;
	char valveIoIndex;
	char engageSensor1Index;
	char engageSensor2Index;
	char engageSensor3Index;
	char engageSensor4Index;
	};

class PSSSetErrorsMaskMsg {
public:
	char boardId;
	char zoneId;
	unsigned int errorMask;
	};

class PSSSetFanSpeedMsg {
public:
	char boardId;
	char zoneId;
	short speed;
	};

class PSSSetFlowMeterDeviceConfigurationMsg {
public:
	char boardId;
	char deviceId;
	char ioIndex;
	short warningLevel;
	short errorLevel;
	};

class PSSSetLimitSwitchDeviceConfigurationMsg {
public:
	char boardId;
	char deviceId;
	char ioIndex;
	};

class PSSSetPIDMsg {
public:
	char boardId;
	char zoneId;
	Single p;
	Single i;
	Single d;
	short setPointSmoothing;
	Single iThreshold;
	unsigned short outputSmoothing;
	};

class PSSSetPowerMsg {
public:
	char boardId;
	char zoneId;
	short power;
	};

class PSSSetPressureSensorDeviceConfigurationMsg {
public:
	char boardId;
	char deviceId;
	char ioIndex;
	short warningLevel;
	short errorLevel;
	};

class PSSSetPressureSwitchDeviceConfigurationMsg {
public:
	char boardId;
	char deviceId;
	char ioIndex;
	};

class PSSSetPumpDeviceConfigurationMsg {
public:
	char boardId;
	char deviceId;
	char ioIndex;
	short powerLimit;
	};

class PSSSetRegisterMsg {
public:
	unsigned short registerID;
	unsigned int value;
	};

class PSSSetTankLevelDeviceConfigurationMsg {
public:
	char boardId;
	char deviceId;
	char ioIndex;
	char highIoIndex;
	char midIoIndex;
	char lowIoIndex;
	};

class PSSSetTemperatureMsg {
public:
	char boardId;
	char zoneId;
	short setPointTemperature;
	short lowWarningLimit;
	short lowRangeLimit;
	short highRangeLimit;
	short highWarningLimit;
	};

class PSSSetValveDeviceConfigurationMsg {
public:
	char boardId;
	char deviceId;
	char ioIndex;
	};

class PSSSetWarningsMaskMsg {
public:
	char boardId;
	char zoneId;
	unsigned int warningMask;
	};

class PSSSetWaterContaminationConfigurationMsg {
public:
	char boardId;
	char deviceId;
	char ioIndex;
	short warningLevel;
	short errorLevel;
	};

class PSSStopMsg {
public:
	char boardId;
	char zoneId;
	};

class PSSTankLevelStatusNotificationMsg {
public:
	char boardId;
	char deviceId;
	char tankLevel;
	char highLevelSensor;
	char midLevelSensor;
	char lowLevelSensor;
	char status;
	};

class PSSValveStatusNotificationMsg {
public:
	char boardId;
	char deviceId;
	short state;
	char status;
	};

class PSSWarningNotificationMsg {
public:
	char boardId;
	char zoneId;
	unsigned int warnings;
	};

class PSSWaterContaminationStatusNotificationMsg {
public:
	char boardId;
	char deviceId;
	short contamination;
	char status;
	};

union PSSMsgType{
	PSSActivateBTCControlMsg pSSActivateBTCControlMsg;
	PSSActivateEngageSystemMsg pSSActivateEngageSystemMsg;
	PSSBTCControlStatusNotificationMsg pSSBTCControlStatusNotificationMsg;
	PSSGetBTCControlStatusMsg pSSGetBTCControlStatusMsg;
	PSSGetPIDMsg pSSGetPIDMsg;
	PSSPIDValuesNotificationMsg pSSPIDValuesNotificationMsg;
	PSSResetBoardMsg pSSResetBoardMsg;
	PSSSetBTCControlConfigMsg pSSSetBTCControlConfigMsg;
	PSSActivateMonitoringMsg pSSActivateMonitoringMsg;
	PSSActivatePumpMsg pSSActivatePumpMsg;
	PSSActivateZoneMsg pSSActivateZoneMsg;
	PSSAutoTuneMsg pSSAutoTuneMsg;
	PSSBootLoaderMsg pSSBootLoaderMsg;
	PSSEnableMsg pSSEnableMsg;
	PSSEnableRegistersNotificationMsg pSSEnableRegistersNotificationMsg;
	PSSEngageSystemStatusNotificationMsg pSSEngageSystemStatusNotificationMsg;
	PSSErrorNotificationMsg pSSErrorNotificationMsg;
	PSSFlowMeterStatusNotificationMsg pSSFlowMeterStatusNotificationMsg;
	PSSGetAirBleedingZoneConfigMsg pSSGetAirBleedingZoneConfigMsg;
	PSSSetAirBleedingZoneConfigMsg pSSSetAirBleedingZoneConfigMsg;
	PSSGetAirBleedingZoneConfigReplyMsg pSSGetAirBleedingZoneConfigReplyMsg;
	PSSGetBlowerZoneConfigMsg pSSGetBlowerZoneConfigMsg;
	PSSSetBlowerZoneConfigMsg pSSSetBlowerZoneConfigMsg;
	PSSGetBlowerZoneConfigReplyMsg pSSGetBlowerZoneConfigReplyMsg;
	PSSGetControllerConfigMsg pSSGetControllerConfigMsg;
	PSSSetControllerConfigMsg pSSSetControllerConfigMsg;
	PSSGetControllerConfigReplyMsg pSSGetControllerConfigReplyMsg;
	PSSGetEngageSystemStatusMsg pSSGetEngageSystemStatusMsg;
	PSSGetErrorsMsg pSSGetErrorsMsg;
	PSSSetFanZoneConfigMsg pSSSetFanZoneConfigMsg;
	PSSGetFanZoneConfigReplyMsg pSSGetFanZoneConfigReplyMsg;
	PSSGetFanZoneConfigMsg pSSGetFanZoneConfigMsg;
	PSSGetFlowMeterStatusMsg pSSGetFlowMeterStatusMsg;
	PSSGetDigitalSensorStatusMsg pSSGetDigitalSensorStatusMsg;
	PSSGetPressureSensorStatusMsg pSSGetPressureSensorStatusMsg;
	PSSGetPumpStatusMsg pSSGetPumpStatusMsg;
	PSSGetTankLevelStatusMsg pSSGetTankLevelStatusMsg;
	PSSSetTemperatureSensorZoneConfigMsg pSSSetTemperatureSensorZoneConfigMsg;
	PSSGetTemperatureSensorZoneConfigReplyMsg pSSGetTemperatureSensorZoneConfigReplyMsg;
	PSSGetTemperatureSensorZoneConfigMsg pSSGetTemperatureSensorZoneConfigMsg;
	PSSGetValveStatusMsg pSSGetValveStatusMsg;
	PSSGetVersionMsg pSSGetVersionMsg;
	PSSGetVersionReplyMsg pSSGetVersionReplyMsg;
	PSSGetWarningsMsg pSSGetWarningsMsg;
	PSSGetTemperatureZoneConfigMsg pSSGetTemperatureZoneConfigMsg;
	PSSSetTemperatureZoneConfigMsg pSSSetTemperatureZoneConfigMsg;
	PSSGetTemperatureZoneConfigReplyMsg pSSGetTemperatureZoneConfigReplyMsg;
	PSSGetWaterContaminationStatusMsg pSSGetWaterContaminationStatusMsg;
	PSSDigitalSensorStatusNotificationMsg pSSDigitalSensorStatusNotificationMsg;
	PSSSetLoggingLevelMsg pSSSetLoggingLevelMsg;
	PSSMonitoringDataNotificationMsg pSSMonitoringDataNotificationMsg;
	PSSActivateValveMsg pSSActivateValveMsg;
	PSSPressureSensorStatusNotificationMsg pSSPressureSensorStatusNotificationMsg;
	PSSPumpStatusNotificationMsg pSSPumpStatusNotificationMsg;
	PSSReadControllerDataReplyMsg pSSReadControllerDataReplyMsg;
	PSSAckMsg pSSAckMsg;
	PSSKeepAliveMsg pSSKeepAliveMsg;
	PSSKeepAliveReplyMsg pSSKeepAliveReplyMsg;
	PSSReadControllerDataMsg pSSReadControllerDataMsg;
	PSSReadControllerSpecificDataMsg pSSReadControllerSpecificDataMsg;
	PSSReadControllerSpecificDataReplyMsg pSSReadControllerSpecificDataReplyMsg;
	PSSReadSystemDataMsg pSSReadSystemDataMsg;
	TMCSystemData tMCSystemData;
	PSSReadSystemDataReplyMsg pSSReadSystemDataReplyMsg;
	PSSRegistersNotificationMsg pSSRegistersNotificationMsg;
	PSSSeqEndedMsg pSSSeqEndedMsg;
	PSSSetBlowerSpeedMsg pSSSetBlowerSpeedMsg;
	PSSSetEngageSystemConfigurationMsg pSSSetEngageSystemConfigurationMsg;
	PSSSetErrorsMaskMsg pSSSetErrorsMaskMsg;
	PSSSetFanSpeedMsg pSSSetFanSpeedMsg;
	PSSSetFlowMeterDeviceConfigurationMsg pSSSetFlowMeterDeviceConfigurationMsg;
	PSSSetLimitSwitchDeviceConfigurationMsg pSSSetLimitSwitchDeviceConfigurationMsg;
	PSSSetPIDMsg pSSSetPIDMsg;
	PSSSetPowerMsg pSSSetPowerMsg;
	PSSSetPressureSensorDeviceConfigurationMsg pSSSetPressureSensorDeviceConfigurationMsg;
	PSSSetPressureSwitchDeviceConfigurationMsg pSSSetPressureSwitchDeviceConfigurationMsg;
	PSSSetPumpDeviceConfigurationMsg pSSSetPumpDeviceConfigurationMsg;
	PSSSetRegisterMsg pSSSetRegisterMsg;
	PSSSetTankLevelDeviceConfigurationMsg pSSSetTankLevelDeviceConfigurationMsg;
	PSSSetTemperatureMsg pSSSetTemperatureMsg;
	PSSSetValveDeviceConfigurationMsg pSSSetValveDeviceConfigurationMsg;
	PSSSetWarningsMaskMsg pSSSetWarningsMaskMsg;
	PSSSetWaterContaminationConfigurationMsg pSSSetWaterContaminationConfigurationMsg;
	PSSStopMsg pSSStopMsg;
	PSSTankLevelStatusNotificationMsg pSSTankLevelStatusNotificationMsg;
	PSSValveStatusNotificationMsg pSSValveStatusNotificationMsg;
	PSSWarningNotificationMsg pSSWarningNotificationMsg;
	PSSWaterContaminationStatusNotificationMsg pSSWaterContaminationStatusNotificationMsg;
};
