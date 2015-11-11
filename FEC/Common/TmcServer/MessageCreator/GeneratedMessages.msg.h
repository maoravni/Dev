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

#define MSG_ActivateBTCControl 0x63
#define MSG_ActivateEngageSystem 0x60
#define MSG_BtcControlStatusNotification 0x162
#define MSG_GetBtcControlStatus 0x62
#define MSG_GetPID 0x25
#define MSG_PIDValuesNotification 0x25
#define MSG_ResetBoard 0x21
#define MSG_SetBtcControlConfiguration 0x61
#define MSG_ActivateMonitoring 0x15
#define MSG_ActivatePump 0x11
#define MSG_ActivateZone 0x34
#define MSG_AutoTune 0x32
#define MSG_BootLoader 0x22
#define MSG_Enable 0x23
#define MSG_EnableRegistersNotification 0x52
#define MSG_EngageSystemStatusNotification 0x159
#define MSG_ErrorNotification 0x116
#define MSG_FlowMeterNotification 0x150
#define MSG_GetAirBleedingZoneConfig 0x42
#define MSG_SetAirBleedingZoneConfig 0x41
#define MSG_GetAirBleedingZoneConfigReply 0x42
#define MSG_GetBlowerZoneConfig 0x36
#define MSG_SetBlowerZoneConfig 0x35
#define MSG_GetBlowerZoneConfigReply 0x36
#define MSG_GetControllerConfiguration 0x29
#define MSG_SetControllerConfiguration 0x28
#define MSG_GetControllerConfigurationReply 0x29
#define MSG_GetEngageSystemStatus 0x59
#define MSG_GetError 0x16
#define MSG_SetFanZoneConfig 0x39
#define MSG_GetFanZoneConfigReply 0x40
#define MSG_GetFanZoneConfig 0x40
#define MSG_GetFlowMeterStatus 0x50
#define MSG_GetDigitalSensorStatus 0x6
#define MSG_GetPressureSensorStatus 0x48
#define MSG_GetPumpStatus 0x10
#define MSG_GetTankLevelStatus 0x55
#define MSG_SetTemperatureSensorZoneConfig 0x37
#define MSG_GetTemperatureSensorZoneConfigReply 0x38
#define MSG_GetTemperatureSensorZoneConfig 0x38
#define MSG_GetValveStatus 0x65
#define MSG_GetVersion 0x53
#define MSG_GetVersionReply 0x53
#define MSG_GetWarnings 0x17
#define MSG_GetTemperatureZoneConfiguration 0x31
#define MSG_SetTemperatureZoneConfiguration 0x30
#define MSG_GetTemperatureZoneConfigurationReply 0x31
#define MSG_GetWaterContaminationStatus 0x57
#define MSG_DigitalSensorStatusNotification 0x106
#define MSG_SetLoggingLevel 0x27
#define MSG_MonitorDataNotification 0x118
#define MSG_ActivateValve 0x33
#define MSG_PressureSensorStatusNotification 0x148
#define MSG_PumpStatusNotification 0x110
#define MSG_ReadControllerDataReply 0x7
#define MSG_AckMessage 0x3
#define MSG_KeepAlive 0x2
#define MSG_KeepAliveReply 0x2
#define MSG_ReadControllerData 0x7
#define MSG_ReadControllerSpecificData 0x8
#define MSG_ReadControllerSpecificDataReply 0x8
#define MSG_ReadSystemData 0x1
#define MSG_ReadSystemDataReply 0x1
#define MSG_RegistersNotification 0x52
#define MSG_SeqEnded 0x4
#define MSG_SetBlowerSpeed 0x43
#define MSG_SetEngageSystemDeviceConfiguration 0x58
#define MSG_SetErrorMask 0x19
#define MSG_SetFanSpeed 0x44
#define MSG_SetFlowMeterDeviceConfiguration 0x49
#define MSG_SetLimitSwitchDeviceConfiguration 0x12
#define MSG_SetPID 0x26
#define MSG_SetPower 0x14
#define MSG_SetPressureSensorDeviceConfiguration 0x47
#define MSG_SetPressureSwitchDeviceConfiguration 0x45
#define MSG_SetPumpDeviceConfiguration 0x9
#define MSG_SetRegister 0x51
#define MSG_SetTankLevelDeviceConfiguration 0x54
#define MSG_SetTemperature 0x13
#define MSG_SetValveDeviceConfiguration 0x64
#define MSG_SetWarningMask 0x20
#define MSG_SetWaterContaminationDeviceConfiguration 0x56
#define MSG_Stop 0x5
#define MSG_TankLevelStatusNotification 0x155
#define MSG_ValveStatusNotification 0x165
#define MSG_WarningNotification 0x117
#define MSG_WaterContaminationStatusNotification 0x157

M_BEGIN_MESSAGE_MAP(CMessageTask, TmcMessageHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_ActivateBTCControl + APP_MSG, MessageActivateBTCControlHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_ActivateEngageSystem + APP_MSG, MessageActivateEngageSystemHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetBtcControlStatus + APP_MSG, MessageGetBtcControlStatusHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetPID + APP_MSG, MessageGetPIDHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_ResetBoard + APP_MSG, MessageResetBoardHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetBtcControlConfiguration + APP_MSG, MessageSetBtcControlConfigurationHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_ActivateMonitoring + APP_MSG, MessageActivateMonitoringHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_ActivatePump + APP_MSG, MessageActivatePumpHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_ActivateZone + APP_MSG, MessageActivateZoneHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_AutoTune + APP_MSG, MessageAutoTuneHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_BootLoader + APP_MSG, MessageBootLoaderHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_Enable + APP_MSG, MessageEnableHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_EnableRegistersNotification + APP_MSG, MessageEnableRegistersNotificationHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetAirBleedingZoneConfig + APP_MSG, MessageGetAirBleedingZoneConfigHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetAirBleedingZoneConfig + APP_MSG, MessageSetAirBleedingZoneConfigHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetBlowerZoneConfig + APP_MSG, MessageGetBlowerZoneConfigHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetBlowerZoneConfig + APP_MSG, MessageSetBlowerZoneConfigHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetControllerConfiguration + APP_MSG, MessageGetControllerConfigurationHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetControllerConfiguration + APP_MSG, MessageSetControllerConfigurationHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetEngageSystemStatus + APP_MSG, MessageGetEngageSystemStatusHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetError + APP_MSG, MessageGetErrorHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetFanZoneConfig + APP_MSG, MessageSetFanZoneConfigHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetFanZoneConfig + APP_MSG, MessageGetFanZoneConfigHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetFlowMeterStatus + APP_MSG, MessageGetFlowMeterStatusHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetDigitalSensorStatus + APP_MSG, MessageGetDigitalSensorStatusHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetPressureSensorStatus + APP_MSG, MessageGetPressureSensorStatusHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetPumpStatus + APP_MSG, MessageGetPumpStatusHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetTankLevelStatus + APP_MSG, MessageGetTankLevelStatusHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetTemperatureSensorZoneConfig + APP_MSG, MessageSetTemperatureSensorZoneConfigHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetTemperatureSensorZoneConfig + APP_MSG, MessageGetTemperatureSensorZoneConfigHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetValveStatus + APP_MSG, MessageGetValveStatusHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetVersion + APP_MSG, MessageGetVersionHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetWarnings + APP_MSG, MessageGetWarningsHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetTemperatureZoneConfiguration + APP_MSG, MessageGetTemperatureZoneConfigurationHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetTemperatureZoneConfiguration + APP_MSG, MessageSetTemperatureZoneConfigurationHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_GetWaterContaminationStatus + APP_MSG, MessageGetWaterContaminationStatusHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetLoggingLevel + APP_MSG, MessageSetLoggingLevelHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_ActivateValve + APP_MSG, MessageActivateValveHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_KeepAlive + APP_MSG, MessageKeepAliveHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_ReadControllerData + APP_MSG, MessageReadControllerDataHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_ReadControllerSpecificData + APP_MSG, MessageReadControllerSpecificDataHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_ReadSystemData + APP_MSG, MessageReadSystemDataHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetBlowerSpeed + APP_MSG, MessageSetBlowerSpeedHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetEngageSystemDeviceConfiguration + APP_MSG, MessageSetEngageSystemDeviceConfigurationHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetErrorMask + APP_MSG, MessageSetErrorMaskHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetFanSpeed + APP_MSG, MessageSetFanSpeedHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetFlowMeterDeviceConfiguration + APP_MSG, MessageSetFlowMeterDeviceConfigurationHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetLimitSwitchDeviceConfiguration + APP_MSG, MessageSetLimitSwitchDeviceConfigurationHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetPID + APP_MSG, MessageSetPIDHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetPower + APP_MSG, MessageSetPowerHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetPressureSensorDeviceConfiguration + APP_MSG, MessageSetPressureSensorDeviceConfigurationHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetPressureSwitchDeviceConfiguration + APP_MSG, MessageSetPressureSwitchDeviceConfigurationHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetPumpDeviceConfiguration + APP_MSG, MessageSetPumpDeviceConfigurationHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetRegister + APP_MSG, MessageSetRegisterHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetTankLevelDeviceConfiguration + APP_MSG, MessageSetTankLevelDeviceConfigurationHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetTemperature + APP_MSG, MessageSetTemperatureHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetValveDeviceConfiguration + APP_MSG, MessageSetValveDeviceConfigurationHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetWarningMask + APP_MSG, MessageSetWarningMaskHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_SetWaterContaminationDeviceConfiguration + APP_MSG, MessageSetWaterContaminationDeviceConfigurationHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, MSG_Stop + APP_MSG, MessageStopHandler)
	M_MESSAGE_MAP_ENTRY(TmcMessageHandler, NULL_MSG, MessageNotHandled)
M_END_MESSAGE_MAP(TmcMessageHandler)

