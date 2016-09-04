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

#define MSG_Disconnect 0x4001
#define MSG_Connect 0x4002
#define MSG_BSSBlanketCleaningState 0x31
#define MSG_BSSSendCCSEngageState 0x30
#define MSG_PolisherMove 0x23
#define MSG_PolisherRecovery 0x25
#define MSG_PolisherSetParams 0x26
#define MSG_PolisherStatusNotification 0x28
#define MSG_PolisherStop 0x24
#define MSG_ActivateActivationWithFeedbackControl 0x314
#define MSG_ActivateConcentrationControlMsg 0x319
#define MSG_ActivateLeakageDetectionControl 0x315
#define MSG_ActivateObserveAndNotifyControlMsg 0x318
#define MSG_ActivateWaterTankControl 0x312
#define MSG_ActivateWaterTankLevelControl 0x313
#define MSG_AddActivationInputDevice 0x229
#define MSG_AddShutdownOperation 0x222
#define MSG_ConfigControlStopConditions 0x223
#define MSG_Define3SensorWaterTankLevel 0x216
#define MSG_Define3SensorWaterTank 0x214
#define MSG_ActivateHysteresisTemperatureControl 0x311
#define MSG_DefineAnalogSensorWaterTank 0x215
#define MSG_DefineCalculateOnTwoDevicesControl 0x227
#define MSG_DefineConcentrationCalculatorControl 0x230
#define MSG_DefineProtectionAggregatorControl 0x231
#define MSG_DefineConcentrationControl 0x228
#define MSG_DefineConstantDeltaProtection 0x219
#define MSG_DefineCurrentLimitsProtection 0x225
#define MSG_DefineModbusPeriph 0x113
#define MSG_DefineHysteresisTemperatureControl 0x213
#define MSG_ActivateInverterControl 0x310
#define MSG_ActivatePIDControl 0x309
#define MSG_AddDependentDeviceToControl 0x212
#define MSG_AddFeedbackDeviceToControl 0x211
#define MSG_BoardRecovery 0x15
#define MSG_Define6RTDPeriph 0x101
#define MSG_Define8TCPeriph 0x102
#define MSG_DefineActivationWithFeedbackControl 0x210
#define MSG_DefineAnalogOutInverterControl 0x207
#define MSG_DefineDataCardPeriph 0x104
#define MSG_DefineDeviceProtection 0x208
#define MSG_DefineGenericModbusPeriph 0x100
#define MSG_DefineInverterPeriph 0x105
#define MSG_DefineLeakageDetectionControl 0x221
#define MSG_DefineModbusInverterControl 0x209
#define MSG_DefineOnboardPeriph 0x107
#define MSG_DefineOnboardI2cMi3IrPeriph 0x111
#define MSG_DefineOnboardPT100Periph 0x106
#define MSG_DefineProportionalProtection 0x220
#define MSG_DefinePumaPeriph 0x103
#define MSG_DefineObserveAndNotifyControl 0x224
#define MSG_DefineSubtractTwoDevicesControl 0x226
#define MSG_DefinePIDControl 0x204
#define MSG_DefineProtectionControl 0x205
#define MSG_DefineVirtualPeriph 0x114
#define MSG_DeviceStatusNotification 0x401
#define MSG_EndApplicationUpload 0x1002
#define MSG_EndBoardConfig 0x10
#define MSG_GetBoardStatus 0x6
#define MSG_BoardStatusNotification 0x6
#define MSG_GetBoardType 0x18
#define MSG_GetBoardTypReply 0x18
#define MSG_GetCableId 0x16
#define MSG_GetCableIdReplay 0x16
#define MSG_GetDefinedPeripheals 0x1
#define MSG_GetDefinedPeriphealsReply 0x1
#define MSG_GetHashAndModifiedDate 0x19
#define MSG_GetHashAndModifiedDateReply 0x19
#define MSG_GetLeakageDetectionParametersReply 0x317
#define MSG_GetPID 0x304
#define MSG_GetPIDReply 0x305
#define MSG_GetStatus 0x400
#define MSG_InitControl 0x300
#define MSG_LastModifiedDate 0x21
#define MSG_NextAppPacket 0x1001
#define MSG_PSSReadyForRecovery 0x31
#define MSG_ResetBoard 0x50
#define MSG_ResetToOnControl 0x302
#define MSG_SetLeakageDetectionParameters 0x316
#define MSG_SetPsocAllowedCableMask 0x20
#define MSG_SetPsocSlaveConfig 0x17
#define MSG_SetBoardConfiguration 0x8
#define MSG_ActivateMonitoring 0x306
#define MSG_AutoTune 0x308
#define MSG_ErrorNotification 0x13
#define MSG_ErrorNotificationWithSecondary 0x23
#define MSG_ErrorNotificationWithInfo 0x25
#define MSG_GetErrors 0x13
#define MSG_GetVersion 0x5
#define MSG_GetVersionReply 0x5
#define MSG_GetWarnings 0x14
#define MSG_SetDeviceConfig 0x200
#define MSG_SetDeviceOutputValue 0x307
#define MSG_DefineLogging 0x7
#define MSG_Ack 0x3
#define MSG_KeepAlive 0x2
#define MSG_KeepAliveReply 0x2
#define MSG_SeqEnded 0x4
#define MSG_SetErrorsMask 0x11
#define MSG_SetPID 0x303
#define MSG_SetAnalogDeviceConfig 0x217
#define MSG_SetSwPWMDeviceConfig 0x202
#define MSG_SetPIDControlParameters 0x206
#define MSG_SetMi3IrDeviceConfig 0x218
#define MSG_SetTemperatureDeviceConfig 0x201
#define MSG_SetWarningsMask 0x12
#define MSG_StartApplicationUpload 0x1000
#define MSG_StartBoardConfig 0x9
#define MSG_StopControl 0x301
#define MSG_ControlStatusNotification 0x402
#define MSG_WarningNotification 0x14
#define MSG_WarningNotificationWithSecondary 0x24
#define MSG_WriteModbusRegister 0x112
#define MSG_TMCAck 0x1
#define MSG_RevolverBrushForward 0x10
#define MSG_RevolverBrushReverse 0x9
#define MSG_RevolverCleancycle 0x12
#define MSG_TMCErrorNotification 0x5
#define MSG_RevolverGetStatus 0x3
#define MSG_RevolverGotoNextBlade 0x8
#define MSG_TMCKeepAlive 0x17
#define MSG_TMCKeepAliveReply 0x17
#define MSG_RevolverHardReset 0x20
#define MSG_RevolverMoveToOn 0x19
#define MSG_RevolverMoveToReady 0x16
#define MSG_RevolverMoveToStandby 0x18
#define MSG_RevolverSetHomePosition 0x14
#define MSG_RevolverInit 0x6
#define MSG_RevolverRecovery 0x13
#define MSG_TMCSeqEnded 0x2
#define MSG_RevolverSetMotorsOff 0x21
#define MSG_RevolverSetParams 0x15
#define MSG_RevolverSetZeroPosition 0x7
#define MSG_RevolverStatusNotification 0x3
#define MSG_RevolverStopbrush 0x11
#define MSG_TMCWarningNotification 0x4
#define MSG_TMCGetVersion 0x22
#define MSG_TMCGetVersionReply 0x22
#define MSG_TMCSetControls 0x32
#define MSG_WaitDisengageTubApprovle 0x34
#define MSG_WaitStopPumpsApprovle 0x33

M_BEGIN_MESSAGE_MAP(CMessageTask, PscMessageHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_ActivateActivationWithFeedbackControl + APP_MSG, MessageActivateActivationWithFeedbackControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_ActivateConcentrationControlMsg + APP_MSG, MessageActivateConcentrationControlMsgHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_ActivateLeakageDetectionControl + APP_MSG, MessageActivateLeakageDetectionControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_ActivateObserveAndNotifyControlMsg + APP_MSG, MessageActivateObserveAndNotifyControlMsgHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_ActivateWaterTankControl + APP_MSG, MessageActivateWaterTankControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_ActivateWaterTankLevelControl + APP_MSG, MessageActivateWaterTankLevelControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_AddActivationInputDevice + APP_MSG, MessageAddActivationInputDeviceHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_AddShutdownOperation + APP_MSG, MessageAddShutdownOperationHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_ConfigControlStopConditions + APP_MSG, MessageConfigControlStopConditionsHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_Define3SensorWaterTankLevel + APP_MSG, MessageDefine3SensorWaterTankLevelHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_Define3SensorWaterTank + APP_MSG, MessageDefine3SensorWaterTankHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_ActivateHysteresisTemperatureControl + APP_MSG, MessageActivateHysteresisTemperatureControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineAnalogSensorWaterTank + APP_MSG, MessageDefineAnalogSensorWaterTankHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineCalculateOnTwoDevicesControl + APP_MSG, MessageDefineCalculateOnTwoDevicesControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineConcentrationCalculatorControl + APP_MSG, MessageDefineConcentrationCalculatorControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineProtectionAggregatorControl + APP_MSG, MessageDefineProtectionAggregatorControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineConcentrationControl + APP_MSG, MessageDefineConcentrationControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineConstantDeltaProtection + APP_MSG, MessageDefineConstantDeltaProtectionHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineCurrentLimitsProtection + APP_MSG, MessageDefineCurrentLimitsProtectionHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineModbusPeriph + APP_MSG, MessageDefineModbusPeriphHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineHysteresisTemperatureControl + APP_MSG, MessageDefineHysteresisTemperatureControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_ActivateInverterControl + APP_MSG, MessageActivateInverterControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_ActivatePIDControl + APP_MSG, MessageActivatePIDControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_AddDependentDeviceToControl + APP_MSG, MessageAddDependentDeviceToControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_AddFeedbackDeviceToControl + APP_MSG, MessageAddFeedbackDeviceToControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_BoardRecovery + APP_MSG, MessageBoardRecoveryHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_Define6RTDPeriph + APP_MSG, MessageDefine6RTDPeriphHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_Define8TCPeriph + APP_MSG, MessageDefine8TCPeriphHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineActivationWithFeedbackControl + APP_MSG, MessageDefineActivationWithFeedbackControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineAnalogOutInverterControl + APP_MSG, MessageDefineAnalogOutInverterControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineDataCardPeriph + APP_MSG, MessageDefineDataCardPeriphHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineDeviceProtection + APP_MSG, MessageDefineDeviceProtectionHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineGenericModbusPeriph + APP_MSG, MessageDefineGenericModbusPeriphHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineInverterPeriph + APP_MSG, MessageDefineInverterPeriphHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineLeakageDetectionControl + APP_MSG, MessageDefineLeakageDetectionControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineModbusInverterControl + APP_MSG, MessageDefineModbusInverterControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineOnboardPeriph + APP_MSG, MessageDefineOnboardPeriphHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineOnboardI2cMi3IrPeriph + APP_MSG, MessageDefineOnboardI2cMi3IrPeriphHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineOnboardPT100Periph + APP_MSG, MessageDefineOnboardPT100PeriphHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineProportionalProtection + APP_MSG, MessageDefineProportionalProtectionHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefinePumaPeriph + APP_MSG, MessageDefinePumaPeriphHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineObserveAndNotifyControl + APP_MSG, MessageDefineObserveAndNotifyControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineSubtractTwoDevicesControl + APP_MSG, MessageDefineSubtractTwoDevicesControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefinePIDControl + APP_MSG, MessageDefinePIDControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineProtectionControl + APP_MSG, MessageDefineProtectionControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineVirtualPeriph + APP_MSG, MessageDefineVirtualPeriphHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_EndApplicationUpload + APP_MSG, MessageEndApplicationUploadHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_EndBoardConfig + APP_MSG, MessageEndBoardConfigHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_GetBoardStatus + APP_MSG, MessageGetBoardStatusHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_GetBoardType + APP_MSG, MessageGetBoardTypeHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_GetCableId + APP_MSG, MessageGetCableIdHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_GetDefinedPeripheals + APP_MSG, MessageGetDefinedPeriphealsHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_GetHashAndModifiedDate + APP_MSG, MessageGetHashAndModifiedDateHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_GetPID + APP_MSG, MessageGetPIDHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_GetStatus + APP_MSG, MessageGetStatusHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_InitControl + APP_MSG, MessageInitControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_LastModifiedDate + APP_MSG, MessageLastModifiedDateHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_NextAppPacket + APP_MSG, MessageNextAppPacketHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_ResetBoard + APP_MSG, MessageResetBoardHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_ResetToOnControl + APP_MSG, MessageResetToOnControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_SetLeakageDetectionParameters + APP_MSG, MessageSetLeakageDetectionParametersHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_SetPsocAllowedCableMask + APP_MSG, MessageSetPsocAllowedCableMaskHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_SetPsocSlaveConfig + APP_MSG, MessageSetPsocSlaveConfigHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_SetBoardConfiguration + APP_MSG, MessageSetBoardConfigurationHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_ActivateMonitoring + APP_MSG, MessageActivateMonitoringHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_AutoTune + APP_MSG, MessageAutoTuneHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_GetErrors + APP_MSG, MessageGetErrorsHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_GetVersion + APP_MSG, MessageGetVersionHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_GetWarnings + APP_MSG, MessageGetWarningsHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_SetDeviceConfig + APP_MSG, MessageSetDeviceConfigHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_SetDeviceOutputValue + APP_MSG, MessageSetDeviceOutputValueHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_DefineLogging + APP_MSG, MessageDefineLoggingHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_KeepAlive + APP_MSG, MessageKeepAliveHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_SetErrorsMask + APP_MSG, MessageSetErrorsMaskHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_SetPID + APP_MSG, MessageSetPIDHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_SetAnalogDeviceConfig + APP_MSG, MessageSetAnalogDeviceConfigHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_SetSwPWMDeviceConfig + APP_MSG, MessageSetSwPWMDeviceConfigHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_SetPIDControlParameters + APP_MSG, MessageSetPIDControlParametersHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_SetMi3IrDeviceConfig + APP_MSG, MessageSetMi3IrDeviceConfigHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_SetTemperatureDeviceConfig + APP_MSG, MessageSetTemperatureDeviceConfigHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_SetWarningsMask + APP_MSG, MessageSetWarningsMaskHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_StartApplicationUpload + APP_MSG, MessageStartApplicationUploadHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_StartBoardConfig + APP_MSG, MessageStartBoardConfigHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_StopControl + APP_MSG, MessageStopControlHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_WriteModbusRegister + APP_MSG, MessageWriteModbusRegisterHandler)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_Disconnect + APP_MSG, MessageDisconnect)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, MSG_Connect + APP_MSG, MessageConnect)
	M_MESSAGE_MAP_ENTRY(PscMessageHandler, NULL_MSG, MessageNotHandled)
M_END_MESSAGE_MAP(PscMessageHandler)

