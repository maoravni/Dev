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

struct PSSActivateActivationWithFeedbackControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	int outputValue;
	int timeout;
	};

struct PSSActivateConcentrationControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	float concentrationHighSetPoint;
	float concentrationLowSetPoint;
	float concentrationMaxWarning;
	float concentrationMaxWorking;
	float concentrationMinWarning;
	float concentrationMinWorking;
	float tankLevelHighSetpoint;
	float tankLevelLowSetpoint;
	float tankMaxWarning;
	float tankMaxWorking;
	float tankMinWarning;
	float tankMinWorking;
	unsigned int activationDelay;
	};

struct PSSActivateLeakageDetectionControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short calibrationStage;
	int stabilizationTimeout;
	int calculationWindow;
	float allowedDeviation;
	};

struct PSSActivateObserveAndNotifyControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	float setPoint;
	float maxWarningRange;
	float maxWorkingRange;
	float minWorkingRange;
	float minWarningRange;
	};

struct PSSActivateWaterTankControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	};

struct PSSActivateWaterTankLevelControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	float lowLevelSetPoint;
	float midLevelSetPoint;
	float highLevelSetPoint;
	};

struct PSSAddActivationInputDeviceMsg {
	unsigned short cableId;
	unsigned short controlPssId;
	unsigned short devicePssId;
	char activationType;
	char activeHigh;
	};

struct PSSAddShutdownOperation {
	unsigned short cableId;
	unsigned short controlPssId;
	unsigned int delay;
	char operation;
	float setpoint;
	};

struct PSSConfigControlStopConditions {
	unsigned short cableId;
	unsigned short pssId;
	char stopOnEmr;
	char stopOnDisconnection;
	};

struct PSSDefine3SensorWaterTankLevelMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short tankPssid;
	unsigned short drainPssid;
	unsigned short fillPssid;
	};

struct PSSDefine3SensorWaterTankMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short lowSensorPssid;
	unsigned short midSensorPssid;
	unsigned short highSensorPssid;
	unsigned short fillPssid;
	};

struct PSSActivateHysteresisTemperatureControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	float setPoint;
	float deactivateSetPoint;
	float maxWarningRange;
	float maxWorkingRange;
	float minWorkingRange;
	float minWarningRange;
	};

struct PSSDefineAnalogSensorWaterTankMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short sensorPssid;
	unsigned short fillPssid;
	float lowLevelThreshold;
	float midLevelThreshold;
	float highLevelThreshold;
	};

struct PSSDefineCalculateOnTwoDevicesControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short input1;
	unsigned short input2;
	unsigned short output;
	unsigned short calculationFunction;
	};

struct PSSDefineConcentrationCalculatorControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short viscosityInputPssId;
	unsigned short temperatureInputPssId;
	unsigned short concentrationOutputPssId;
	float concentration1;
	float intercept1;
	float slope1;
	float concentration2;
	float intercept2;
	float slope2;
	};

struct PSSDefineProtectionAggregatorControl {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short outputPssId;
	char negateResult;
	unsigned short bitwiseOperation;
	};

struct PSSDefineConcentrationControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short concentrationInput;
	unsigned short liquidLevelInput;
	unsigned short conditionerValve;
	unsigned short waterValve;
	char conditionerValveActivationValue;
	char waterValveActivationValue;
	};

struct PSSDefineConstantDeltaProtectionMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short connectedToControlPSSID;
	unsigned short referenceInputPSSID;
	float allowedUpperDelta;
	float allowedLowerDelta;
	unsigned short debounceTimer;
	};

struct PSSDefineCurrentLimitsProtectionMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short connectedToControlPSSID;
	float upperLimits;
	float lowerWarningLimits;
	float lowerErrorLimits;
	unsigned short debounceTimer;
	};

struct PSSDefineModbusPeriphMsg {
	unsigned short cableId;
	unsigned short slaveCableId;
	unsigned short periphPssId;
	unsigned short modbusId;
	int peripheralType;
	};

struct PSSDefineHysteresisTemperatureControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short inputPssId;
	unsigned short outputPssId;
	float outputValue;
	};

struct PSSActivateInverterControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	float setPoint;
	unsigned int activationDelay;
	};

struct PSSActivatePIDControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	float setPoint;
	float maxWarningRange;
	float maxWorkingRange;
	float minWorkingRange;
	float minWarningRange;
	float feedForward;
	unsigned int activationDelay;
	};

struct PSSAddDependentDeviceToControl {
	unsigned short cableId;
	unsigned short controlPssId;
	unsigned short dependentDevicePssId;
	unsigned short thresholdValueType;
	int thresholdValue;
	char greaterThan;
	char deactivateControlOnChange;
	char lockOnActivate;
	unsigned short dependencyCheckType;
	};

struct PSSAddFeedbackDeviceToControl {
	unsigned short cableId;
	unsigned short controlPssId;
	unsigned short devicePssId;
	unsigned short thresholdValueType;
	int thresholdValue;
	char greaterThan;
	char deactivateControlOnChange;
	};

struct PSSBoardRecoveryMsg {
	unsigned short cableId;
	};

struct PSSDefine6RTDPeriphMsg {
	unsigned short cableId;
	unsigned short slaveCableId;
	unsigned short periphPSSId;
	unsigned short modbusId;
	};

struct PSSDefine8TCPeriphMsg {
	unsigned short cableId;
	unsigned short slaveCableId;
	unsigned short periphPSSId;
	unsigned short modbusId;
	};

struct PSSDefineActivationWithFeedbackControlMsg {
	unsigned short cableId;
	unsigned short pssID;
	unsigned short activatePSSId;
	unsigned short deactivatePSSId;
	unsigned short activationTimeout;
	unsigned short deactivationTimeout;
	char activationWithFeedbackBehaviorOnInit;
	unsigned int ignoreProtectionsDelay;
	unsigned short activationFeedbackOutputPSSId;
	unsigned short deactivationFeedbackOutputPSSId;
	};

struct PSSDefineAnalogOutInverterControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short enableId;
	unsigned short speedId;
	};

struct PSSDefineDataCardPeriphMsg {
	unsigned short cableId;
	unsigned short slaveCableId;
	unsigned short periphPSSId;
	unsigned short modbusId;
	};

struct PSSDefineDeviceProtectionControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short dataType;
	float upperSoftLimit;
	float upperHardLimit;
	float lowerSoftLimit;
	float lowerHardLimit;
	char checkLowLimit;
	char checkHighLimit;
	unsigned short connectedControl;
	unsigned short debounceTimer;
	};

struct PSSDefineGenericModbusPeriphMsg {
	unsigned short cableId;
	unsigned short slaveCableId;
	unsigned short periphPssId;
	unsigned short modbusId;
	unsigned short startAddress;
	unsigned short numberOfRegisters;
	unsigned short scaling;
	unsigned short updateInterval;
	};

struct PSSDefineInverterPeriphMsg {
	unsigned short cableId;
	unsigned short slaveCableId;
	unsigned short periphPSSId;
	unsigned short modbusId;
	char inverterType;
	float minSpeed;
	float maxSpeed;
	float accelRate;
	float decelRate;
	float motorFrequency;
	float motorCurrent;
	float nominalRpm;
	};

struct PSSDefineLeakageDetectionControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short tankLevelPSSID;
	unsigned short tubLevelPSSID;
	float upperBoundForLeak;
	float lowerBoundForLeak;
	};

struct PSSDefineModbusInverterControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short enableId;
	unsigned short outputFrequencyPSSId;
	unsigned short outputCurrentPSSId;
	unsigned short outputSetPointPSSId;
	};

struct PSSDefineOnboardPeriphMsg {
	unsigned short cableId;
	unsigned short slaveCableId;
	unsigned short periphPSSId;
	unsigned short numberOfDevices;
	int periphType;
	unsigned short sampleInterval;
	unsigned int lowPassFilter;
	};

struct PSSDefineOnboardI2cMi3IrPeriph {
	unsigned short cableId;
	unsigned short slaveCableId;
	unsigned short periphPSSId;
	unsigned short sampleInterval;
	unsigned short numberOfDevices;
	unsigned int lowPassWindow;
	};

struct PSSDefineOnboardPT100PeriphMsg {
	unsigned short cableId;
	unsigned short slaveCableId;
	unsigned short periphPSSId;
	unsigned short sampleInterval;
	unsigned short numberOfDevices;
	unsigned int lowPassWindow;
	};

struct PSSDefineProportionalProtectionMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short connectedToControlPSSID;
	unsigned short referenceInputPSSID;
	float inputGain;
	float inputOffset;
	float allowedUpperDelta;
	float allowedLowerDelta;
	unsigned short debounceTimer;
	};

struct PSSDefinePumaPeriphMsg {
	unsigned short cableId;
	unsigned short slaveCableId;
	unsigned short periphPSSId;
	unsigned short modbusId;
	};

struct PSSDefineObserveAndNotifyControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short input;
	};

struct PSSDefineSubtractTwoDevicesControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short input1;
	unsigned short input2;
	unsigned short output;
	};

struct PSSDefinePIDControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short input;
	unsigned short output;
	char cascade;
	};

struct PSSDefineProtectionControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	};

struct PSSDefineVirtualPeriphMsg {
	unsigned short cableId;
	unsigned short slaveCableId;
	unsigned short periphPssId;
	int peripheralType;
	};

struct PSSDeviceStatusNotificationMsg {
	unsigned short cableId;
	unsigned short pssId;
	char exceptions;
	char dataType;
	int value;
	};

struct PSSEndApplicationUpload {
	unsigned short cableId;
	};

struct PSSEndBoardConfigMsg {
	unsigned short cableId;
	char deleteOnDisconnection;
	__int64 configHash1;
	__int64 configHash2;
	};

struct PSSGetBoardStatus {
	unsigned short cableId;
	};

struct PSSBoardStatusNotificationMsg {
	unsigned short cableId;
	char exceptions;
	char boardState;
	};

struct PSSGetBoardTypeMsg {
	unsigned short cableId;
	char slaveIndex;
	};

struct PSSGetBoardTypeReplyMsg {
	unsigned short cableId;
	char slaveIndex;
	char boardType;
	char numberOfSlaves;
	};

struct PSSGetCableIdMsg {
	unsigned short cableId;
	char slaveIndex;
	};

struct PSSGetCableIdReplyMsg {
	unsigned short masterCableId;
	char slaveIndex;
	unsigned short cableId1;
	unsigned short cableId2;
	unsigned short cableId3;
	char errorBits;
	};

struct PSSGetDefinedPeripheralsMsg {
	unsigned short cableId;
	};

struct PSSGetDefinedPeripheralsReplyMsg {
	unsigned short cableId;
	unsigned short totalNumberOfPeripherals;
	unsigned short peripheralIndex;
	unsigned short peripheralType;
	unsigned short devicesNum;
	};

struct PSSGetHashAndModifiedDateMsg {
	unsigned short cableId;
	};

struct PSSGetHashAndModifiedDateReply {
	unsigned short cableId;
	__int64 lastModifiedDate;
	__int64 configHash1;
	__int64 configHash2;
	};

struct PSSGetLeakageDetectionParametersReplyMsg {
	unsigned short cableId;
	unsigned short pssId;
	float tankLevelInitial;
	float deltaHtx;
	float deltaTub;
	};

struct PSSGetPIDMsg {
	unsigned short cableId;
	unsigned short pssId;
	};

struct PSSGetPIDReplyMsg {
	unsigned short cableId;
	unsigned short pssId;
	float p;
	float i;
	float d;
	};

struct PSSGetStatusMsg {
	unsigned short cableId;
	unsigned short pssId;
	};

struct PSSInitControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	};

struct PSSLastModifiedDateMsg {
	unsigned short cableId;
	__int64 lastModifiedDate;
	};

struct PSSNextAppPacket {
	unsigned short cableId;
	unsigned int packetNumber;
	unsigned short packetLength;
	unsigned short packetCrc;
	char payload[128];
	};

struct PSSResetBoardMsg {
	unsigned short cableId;
	char startupApp;
	};

struct PSSResetToOnControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	};

struct PSSSetLeakageDetectionParametersMsg {
	unsigned short cableId;
	unsigned short pssId;
	float tankLevelInitial;
	float deltaHtx;
	float deltaTub;
	};

struct PSSSetPsocAllowedCableMaskMsg {
	unsigned short cableId;
	char slaveIndex;
	unsigned __int64 allowedCableIdMask;
	};

struct PSSSetPsocSlaveConfigMsg {
	unsigned short cableId;
	unsigned short slaveCableId;
	unsigned short psocPrimaryFunction;
	};

struct PSSSetBoardConfigMsg {
	unsigned short cableId;
	unsigned int ntpServerAddress;
	unsigned int baudRate;
	};

struct PSSActivateMonitoringMsg {
	unsigned short cableId;
	unsigned short pssId;
	char active;
	int minUpdateInterval;
	int maxUpdateInterval;
	};

struct PSSAutoTuneMsg {
	unsigned short cableId;
	unsigned short pssID;
	float setPoint;
	float overShoot;
	float powerAtStart;
	float powerStep;
	short calculation;
	};

struct PSSErrorNotificationMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned int errors;
	};

struct PSSErrorNotificationWithSecondaryMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned int errors;
	unsigned short secondaryPssId;
	};

struct PSSErrorNotificationWithInfoMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned int errors;
	unsigned short secondaryPssId;
	unsigned int additionalError;
	char dataType;
	int dataValue;
	};

struct PSSGetErrorsMsg {
	unsigned short cableId;
	unsigned short pssId;
	};

struct PSSGetVersionMsg {
	unsigned short cableId;
	char slaveIndex;
	};

struct PSSGetVersionReplyMsg {
	unsigned short cableId;
	char slaveIndex;
	unsigned __int64 firmwareVersion;
	unsigned __int64 protocolVersion;
	unsigned short cableId1;
	unsigned short cableId2;
	unsigned short cableId3;
	};

struct PSSGetWarningsMsg {
	unsigned short cableId;
	unsigned short pssId;
	};

struct PSSSetDeviceConfigMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short periphPSSId;
	unsigned short deviceIndex;
	};

struct PSSSetDeviceOutputValueMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short valueType;
	int value;
	};

struct PSSSetLoggingLevelMsg {
	unsigned short cableId;
	char logLevel;
	char taskId;
	char enableLogging;
	};

struct PSSAckMsg {
	unsigned int originalRequestMsgId;
	unsigned int originalRequestSN;
	unsigned short cableId;
	unsigned short pssId;
	unsigned int status;
	};

struct PSSKeepAliveMsg {
	int dummy;
	};

struct PSSKeepAliveReplyMsg {
	int dummy;
	};

struct PSSSeqEndedMsg {
	unsigned int originalRequestMsgId;
	unsigned int originalRequestSN;
	unsigned short cableId;
	unsigned short pssId;
	unsigned int status;
	};

struct PSSSetErrorsMaskMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned int errorMask;
	};

struct PSSSetPIDMsg {
	unsigned short cableId;
	unsigned short pssId;
	float p;
	float i;
	float d;
	float itermRange;
	};

struct PSSSetAnalogDeviceConfigMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short periphPssid;
	unsigned short deviceIndex;
	float aCoff;
	float bCoff;
	float scalingA;
	float scalingB;
	};

struct PSSSetSWPWMDeviceConfigMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short periphPSSId;
	unsigned short deviceIndex;
	unsigned short pwmCycleLength;
	unsigned short pwmGroupID;
	char rampRiseStep;
	char rampFallStep;
	unsigned short rampRiseTime;
	unsigned short rampFallTime;
	};

struct PSSSetTemperatureControlParametersMsg {
	unsigned short cableId;
	unsigned short pssId;
	float maxOutput;
	unsigned short outputFilter;
	float pidSetpointRange;
	};

struct PSSSetMi3IrDeviceConfigMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short periphPssid;
	unsigned short deviceIndex;
	float aCoff;
	float bCoff;
	float emissivity;
	float transmissivity;
	float bottomTemp;
	float topTemp;
	float ambientBackground;
	char ambientBackgroundCompensation;
	char i2cChannel;
	};

struct PSSSetTemperatureDeviceConfigMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned short periphPssid;
	unsigned short deviceIndex;
	float aCoff;
	float bCoff;
	char sensorType;
	char missingSensorPriority;
	};

struct PSSSetWarningsMaskMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned int warningMask;
	};

struct PSSStartApplicationUpload {
	unsigned short cableId;
	unsigned short uploadTarget;
	unsigned int numberOfPackets;
	};

struct PSSStartBoardConfigMsg {
	unsigned short cableId;
	};

struct PSSStopControlMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned int activationDelay;
	};

struct PSSControlStatusNotificationMsg {
	unsigned short cableId;
	unsigned short pssId;
	float setPoint;
	char exceptions;
	char dataType;
	char state;
	};

struct PSSWarningNotificationMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned int warnings;
	};

struct PSSWarningNotificationWithSecondaryMsg {
	unsigned short cableId;
	unsigned short pssId;
	unsigned int warnings;
	unsigned short secondaryPssId;
	};

struct PSSWriteModbusRegister {
	unsigned short cableId;
	unsigned short periphPSSId;
	unsigned short registerAddress;
	unsigned short registerValue;
	};

struct RevolverAckMsg {
	unsigned int originalRequestMsgId;
	unsigned int originalRequestSN;
	unsigned short cableId;
	unsigned int status;
	};

struct RevolverBrushForwardMsg {
	unsigned short cableId;
	};

struct RevolverBrushReverseMsg {
	unsigned short cableId;
	};

struct RevolverCleancycleMsg {
	unsigned short cableId;
	};

struct RevolverErrorNotificationMsg {
	unsigned short cableId;
	unsigned short errorListSize;

	};

struct RevolverGetStatusMsg {
	unsigned short cableId;
	};

struct RevolverGotoNextBladeMsg {
	unsigned short cableId;
	};

struct RevolverKeepAliveMsg {
	int dummy;
	};

struct RevolverKeepAliveReplyMsg {
	int dummy;
	};

struct RevolverHardResetMsg {
	unsigned short cableId;
	};

struct RevolverMoveToOnMsg {
	unsigned short cableId;
	};

struct RevolverMoveToReadyMsg {
	unsigned short cableId;
	};

struct RevolverMoveToStandbyMsg {
	unsigned short cableId;
	};

struct RevolverSetHomePositionMsg {
	unsigned short cableId;
	};

struct RevolverInitMsg {
	unsigned short cableId;
	unsigned short isUnprotectedMode;
	};

struct RevolverRecoveryMsg {
	unsigned short cableId;
	};

struct RevolverSeqEndedMsg {
	unsigned int originalRequestMsgId;
	unsigned int originalRequestSN;
	unsigned short cableId;
	unsigned int status;
	};

struct RevolverSetMotorsOffMsg {
	unsigned short cableId;
	};

struct RevolverSetParamsMsg {
	unsigned short cableId;
	unsigned short blanketRevolutionParams;
	unsigned short brushSpeed;
	unsigned short revolutionTimeout;
	unsigned short brushTimeout;
	unsigned short distanceFromNipsensorToRevolver;
	};

struct RevolverSetZeroPositionMsg {
	unsigned short cableId;
	};

struct RevolverStatusNotificationMsg {
	unsigned short cableId;
	char revolverState;
	unsigned short revolverStatus;
	char brushState;
	unsigned short brushStatus;
	unsigned short activeBlade;
	};

struct RevolverStopbrushMsg {
	unsigned short cableId;
	};

struct RevolverWarningNotificationMsg {
	unsigned short cableId;
	unsigned int warnings;
	};

union PSSMsgType{
	struct PSSActivateActivationWithFeedbackControlMsg pSSActivateActivationWithFeedbackControlMsg;
	struct PSSActivateConcentrationControlMsg pSSActivateConcentrationControlMsg;
	struct PSSActivateLeakageDetectionControlMsg pSSActivateLeakageDetectionControlMsg;
	struct PSSActivateObserveAndNotifyControlMsg pSSActivateObserveAndNotifyControlMsg;
	struct PSSActivateWaterTankControlMsg pSSActivateWaterTankControlMsg;
	struct PSSActivateWaterTankLevelControlMsg pSSActivateWaterTankLevelControlMsg;
	struct PSSAddActivationInputDeviceMsg pSSAddActivationInputDeviceMsg;
	struct PSSAddShutdownOperation pSSAddShutdownOperation;
	struct PSSConfigControlStopConditions pSSConfigControlStopConditions;
	struct PSSDefine3SensorWaterTankLevelMsg pSSDefine3SensorWaterTankLevelMsg;
	struct PSSDefine3SensorWaterTankMsg pSSDefine3SensorWaterTankMsg;
	struct PSSActivateHysteresisTemperatureControlMsg pSSActivateHysteresisTemperatureControlMsg;
	struct PSSDefineAnalogSensorWaterTankMsg pSSDefineAnalogSensorWaterTankMsg;
	struct PSSDefineCalculateOnTwoDevicesControlMsg pSSDefineCalculateOnTwoDevicesControlMsg;
	struct PSSDefineConcentrationCalculatorControlMsg pSSDefineConcentrationCalculatorControlMsg;
	struct PSSDefineProtectionAggregatorControl pSSDefineProtectionAggregatorControl;
	struct PSSDefineConcentrationControlMsg pSSDefineConcentrationControlMsg;
	struct PSSDefineConstantDeltaProtectionMsg pSSDefineConstantDeltaProtectionMsg;
	struct PSSDefineCurrentLimitsProtectionMsg pSSDefineCurrentLimitsProtectionMsg;
	struct PSSDefineModbusPeriphMsg pSSDefineModbusPeriphMsg;
	struct PSSDefineHysteresisTemperatureControlMsg pSSDefineHysteresisTemperatureControlMsg;
	struct PSSActivateInverterControlMsg pSSActivateInverterControlMsg;
	struct PSSActivatePIDControlMsg pSSActivatePIDControlMsg;
	struct PSSAddDependentDeviceToControl pSSAddDependentDeviceToControl;
	struct PSSAddFeedbackDeviceToControl pSSAddFeedbackDeviceToControl;
	struct PSSBoardRecoveryMsg pSSBoardRecoveryMsg;
	struct PSSDefine6RTDPeriphMsg pSSDefine6RTDPeriphMsg;
	struct PSSDefine8TCPeriphMsg pSSDefine8TCPeriphMsg;
	struct PSSDefineActivationWithFeedbackControlMsg pSSDefineActivationWithFeedbackControlMsg;
	struct PSSDefineAnalogOutInverterControlMsg pSSDefineAnalogOutInverterControlMsg;
	struct PSSDefineDataCardPeriphMsg pSSDefineDataCardPeriphMsg;
	struct PSSDefineDeviceProtectionControlMsg pSSDefineDeviceProtectionControlMsg;
	struct PSSDefineGenericModbusPeriphMsg pSSDefineGenericModbusPeriphMsg;
	struct PSSDefineInverterPeriphMsg pSSDefineInverterPeriphMsg;
	struct PSSDefineLeakageDetectionControlMsg pSSDefineLeakageDetectionControlMsg;
	struct PSSDefineModbusInverterControlMsg pSSDefineModbusInverterControlMsg;
	struct PSSDefineOnboardPeriphMsg pSSDefineOnboardPeriphMsg;
	struct PSSDefineOnboardI2cMi3IrPeriph pSSDefineOnboardI2cMi3IrPeriph;
	struct PSSDefineOnboardPT100PeriphMsg pSSDefineOnboardPT100PeriphMsg;
	struct PSSDefineProportionalProtectionMsg pSSDefineProportionalProtectionMsg;
	struct PSSDefinePumaPeriphMsg pSSDefinePumaPeriphMsg;
	struct PSSDefineObserveAndNotifyControlMsg pSSDefineObserveAndNotifyControlMsg;
	struct PSSDefineSubtractTwoDevicesControlMsg pSSDefineSubtractTwoDevicesControlMsg;
	struct PSSDefinePIDControlMsg pSSDefinePIDControlMsg;
	struct PSSDefineProtectionControlMsg pSSDefineProtectionControlMsg;
	struct PSSDefineVirtualPeriphMsg pSSDefineVirtualPeriphMsg;
	struct PSSDeviceStatusNotificationMsg pSSDeviceStatusNotificationMsg;
	struct PSSEndApplicationUpload pSSEndApplicationUpload;
	struct PSSEndBoardConfigMsg pSSEndBoardConfigMsg;
	struct PSSGetBoardStatus pSSGetBoardStatus;
	struct PSSBoardStatusNotificationMsg pSSBoardStatusNotificationMsg;
	struct PSSGetBoardTypeMsg pSSGetBoardTypeMsg;
	struct PSSGetBoardTypeReplyMsg pSSGetBoardTypeReplyMsg;
	struct PSSGetCableIdMsg pSSGetCableIdMsg;
	struct PSSGetCableIdReplyMsg pSSGetCableIdReplyMsg;
	struct PSSGetDefinedPeripheralsMsg pSSGetDefinedPeripheralsMsg;
	struct PSSGetDefinedPeripheralsReplyMsg pSSGetDefinedPeripheralsReplyMsg;
	struct PSSGetHashAndModifiedDateMsg pSSGetHashAndModifiedDateMsg;
	struct PSSGetHashAndModifiedDateReply pSSGetHashAndModifiedDateReply;
	struct PSSGetLeakageDetectionParametersReplyMsg pSSGetLeakageDetectionParametersReplyMsg;
	struct PSSGetPIDMsg pSSGetPIDMsg;
	struct PSSGetPIDReplyMsg pSSGetPIDReplyMsg;
	struct PSSGetStatusMsg pSSGetStatusMsg;
	struct PSSInitControlMsg pSSInitControlMsg;
	struct PSSLastModifiedDateMsg pSSLastModifiedDateMsg;
	struct PSSNextAppPacket pSSNextAppPacket;
	struct PSSResetBoardMsg pSSResetBoardMsg;
	struct PSSResetToOnControlMsg pSSResetToOnControlMsg;
	struct PSSSetLeakageDetectionParametersMsg pSSSetLeakageDetectionParametersMsg;
	struct PSSSetPsocAllowedCableMaskMsg pSSSetPsocAllowedCableMaskMsg;
	struct PSSSetPsocSlaveConfigMsg pSSSetPsocSlaveConfigMsg;
	struct PSSSetBoardConfigMsg pSSSetBoardConfigMsg;
	struct PSSActivateMonitoringMsg pSSActivateMonitoringMsg;
	struct PSSAutoTuneMsg pSSAutoTuneMsg;
	struct PSSErrorNotificationMsg pSSErrorNotificationMsg;
	struct PSSErrorNotificationWithSecondaryMsg pSSErrorNotificationWithSecondaryMsg;
	struct PSSErrorNotificationWithInfoMsg pSSErrorNotificationWithInfoMsg;
	struct PSSGetErrorsMsg pSSGetErrorsMsg;
	struct PSSGetVersionMsg pSSGetVersionMsg;
	struct PSSGetVersionReplyMsg pSSGetVersionReplyMsg;
	struct PSSGetWarningsMsg pSSGetWarningsMsg;
	struct PSSSetDeviceConfigMsg pSSSetDeviceConfigMsg;
	struct PSSSetDeviceOutputValueMsg pSSSetDeviceOutputValueMsg;
	struct PSSSetLoggingLevelMsg pSSSetLoggingLevelMsg;
	struct PSSAckMsg pSSAckMsg;
	struct PSSKeepAliveMsg pSSKeepAliveMsg;
	struct PSSKeepAliveReplyMsg pSSKeepAliveReplyMsg;
	struct PSSSeqEndedMsg pSSSeqEndedMsg;
	struct PSSSetErrorsMaskMsg pSSSetErrorsMaskMsg;
	struct PSSSetPIDMsg pSSSetPIDMsg;
	struct PSSSetAnalogDeviceConfigMsg pSSSetAnalogDeviceConfigMsg;
	struct PSSSetSWPWMDeviceConfigMsg pSSSetSWPWMDeviceConfigMsg;
	struct PSSSetTemperatureControlParametersMsg pSSSetTemperatureControlParametersMsg;
	struct PSSSetMi3IrDeviceConfigMsg pSSSetMi3IrDeviceConfigMsg;
	struct PSSSetTemperatureDeviceConfigMsg pSSSetTemperatureDeviceConfigMsg;
	struct PSSSetWarningsMaskMsg pSSSetWarningsMaskMsg;
	struct PSSStartApplicationUpload pSSStartApplicationUpload;
	struct PSSStartBoardConfigMsg pSSStartBoardConfigMsg;
	struct PSSStopControlMsg pSSStopControlMsg;
	struct PSSControlStatusNotificationMsg pSSControlStatusNotificationMsg;
	struct PSSWarningNotificationMsg pSSWarningNotificationMsg;
	struct PSSWarningNotificationWithSecondaryMsg pSSWarningNotificationWithSecondaryMsg;
	struct PSSWriteModbusRegister pSSWriteModbusRegister;
	struct RevolverAckMsg revolverAckMsg;
	struct RevolverBrushForwardMsg revolverBrushForwardMsg;
	struct RevolverBrushReverseMsg revolverBrushReverseMsg;
	struct RevolverCleancycleMsg revolverCleancycleMsg;
	struct RevolverErrorNotificationMsg revolverErrorNotificationMsg;
	struct RevolverGetStatusMsg revolverGetStatusMsg;
	struct RevolverGotoNextBladeMsg revolverGotoNextBladeMsg;
	struct RevolverKeepAliveMsg revolverKeepAliveMsg;
	struct RevolverKeepAliveReplyMsg revolverKeepAliveReplyMsg;
	struct RevolverHardResetMsg revolverHardResetMsg;
	struct RevolverMoveToOnMsg revolverMoveToOnMsg;
	struct RevolverMoveToReadyMsg revolverMoveToReadyMsg;
	struct RevolverMoveToStandbyMsg revolverMoveToStandbyMsg;
	struct RevolverSetHomePositionMsg revolverSetHomePositionMsg;
	struct RevolverInitMsg revolverInitMsg;
	struct RevolverRecoveryMsg revolverRecoveryMsg;
	struct RevolverSeqEndedMsg revolverSeqEndedMsg;
	struct RevolverSetMotorsOffMsg revolverSetMotorsOffMsg;
	struct RevolverSetParamsMsg revolverSetParamsMsg;
	struct RevolverSetZeroPositionMsg revolverSetZeroPositionMsg;
	struct RevolverStatusNotificationMsg revolverStatusNotificationMsg;
	struct RevolverStopbrushMsg revolverStopbrushMsg;
	struct RevolverWarningNotificationMsg revolverWarningNotificationMsg;
};
