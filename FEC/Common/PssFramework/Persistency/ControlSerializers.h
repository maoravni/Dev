/*
 * ControlSerializers.h
 *
 *  Created on: 10 May 2016
 *      Author: maora
 */

#ifndef CONTROLSERIALIZERS_H_
#define CONTROLSERIALIZERS_H_

#include "Serializer.h"

enum E_ControlSerializationType
{
    E_ControlSerializationType_ControlBase,
    E_ControlSerializationType_LiquidLevel3Sensors,
    E_ControlSerializationType_PidControl,
    E_ControlSerializationType_ActivationWithFeedbackControl,
    E_ControlSerializationType_AddTwoDevicesControl,
    E_ControlSerializationType_AnalogLiquidLevelControl,
    E_ControlSerializationType_AnalogOutInverterControl,
    E_ControlSerializationType_ConcentrationControl,
    E_ControlSerializationType_DivideTwoDevicesControl,
    E_ControlSerializationType_EmergencyInputControl,
    E_ControlSerializationType_HysteresisControl,
    E_ControlSerializationType_LiftPbOnError,
    E_ControlSerializationType_LiftPbOnErrorCcsGen2,
    E_ControlSerializationType_LiftPbOnErrorCcsGen3,
    E_ControlSerializationType_LiquidLevelPumpControl,
    E_ControlSerializationType_MaxTwoDevicesControl,
    E_ControlSerializationType_MinTwoDevicesControl,
    E_ControlSerializationType_ModbusInverterControl,
    E_ControlSerializationType_MultiplyTwoDevicesControl,
    E_ControlSerializationType_ObserveAndNotifyControl,
    E_ControlSerializationType_OrderedShutdownControl,
    E_ControlSerializationType_ProtectionControl,
    E_ControlSerializationType_SubtractTwoDevicesControl,
    E_ControlSerializationType_DeviceProtectionChecker,
    E_ControlSerializationType_ProtectionConstantDeltaChecker,
    E_ControlSerializationType_ProtectionProportionalChecker,
    E_ControlSerializationType_ProtectionCurrentLimitsChecker,
    E_ControlSerializationType_DeviceThresholdChecker,
    E_ControlSerializationType_CalculateOnTwoDevicesControl,
    E_ControlSerializationType_ProtectionCheckerBase,
    E_ControlSerializationType_T_OperationNode,
};

class ControlBase;
template<> class Serializer<ControlBase> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_ControlBase;}
    void serialize(F_FILE* f, ControlBase &c);
    void deserialize(F_FILE* f, ControlBase &c);
};

class LiquidLevel3Sensors;
template<> class Serializer<LiquidLevel3Sensors> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_LiquidLevel3Sensors;}
    void serialize(F_FILE* f, LiquidLevel3Sensors &c);
    void deserialize(F_FILE* f, LiquidLevel3Sensors &c);
};

class PidControl;
template<> class Serializer<PidControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_PidControl;}
    void serialize(F_FILE* f, PidControl &c);
    void deserialize(F_FILE* f, PidControl &c);
};

class ActivationWithFeedbackControl;
template<> class Serializer<ActivationWithFeedbackControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_ActivationWithFeedbackControl;}
    void serialize(F_FILE* f, ActivationWithFeedbackControl &c);
    void deserialize(F_FILE* f, ActivationWithFeedbackControl &c);
};

class ConcentrationControl;
template<> class Serializer<ConcentrationControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_ConcentrationControl;}
    void serialize(F_FILE* f, ConcentrationControl &c);
    void deserialize(F_FILE* f, ConcentrationControl &c);
};

class ObserveAndNotifyControl;
template<> class Serializer<ObserveAndNotifyControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_ObserveAndNotifyControl;}
    void serialize(F_FILE* f, ObserveAndNotifyControl &c);
    void deserialize(F_FILE* f, ObserveAndNotifyControl &c);
};

class LiftPbOnError;
template<> class Serializer<LiftPbOnError> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_LiftPbOnError;}
    void serialize(F_FILE* f, LiftPbOnError &c);
    void deserialize(F_FILE* f, LiftPbOnError &c);
};

class LiftPbOnErrorCcsGen3;
template<> class Serializer<LiftPbOnErrorCcsGen3> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_LiftPbOnErrorCcsGen3;}
    void serialize(F_FILE* f, LiftPbOnErrorCcsGen3 &c);
    void deserialize(F_FILE* f, LiftPbOnErrorCcsGen3 &c);
};

class OrderedShutdownControl;
template<> class Serializer<OrderedShutdownControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_OrderedShutdownControl;}
    void serialize(F_FILE* f, OrderedShutdownControl &c);
    void deserialize(F_FILE* f, OrderedShutdownControl &c);
};

class AnalogLiquidLevelControl;
template<> class Serializer<AnalogLiquidLevelControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_AnalogLiquidLevelControl;}
    void serialize(F_FILE* f, AnalogLiquidLevelControl &c);
    void deserialize(F_FILE* f, AnalogLiquidLevelControl &c);
};

class HysteresisControl;
template<> class Serializer<HysteresisControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_HysteresisControl;}
    void serialize(F_FILE* f, HysteresisControl &c);
    void deserialize(F_FILE* f, HysteresisControl &c);
};

class AnalogOutInverterControl;
template<> class Serializer<AnalogOutInverterControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_AnalogOutInverterControl;}
    void serialize(F_FILE* f, AnalogOutInverterControl &c);
    void deserialize(F_FILE* f, AnalogOutInverterControl &c);
};

class LiftPbOnErrorCcsGen2;
template<> class Serializer<LiftPbOnErrorCcsGen2> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_LiftPbOnErrorCcsGen2;}
    void serialize(F_FILE* f, LiftPbOnErrorCcsGen2 &c);
    void deserialize(F_FILE* f, LiftPbOnErrorCcsGen2 &c);
};

class ProtectionControl;
template<> class Serializer<ProtectionControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_ProtectionControl;}
    void serialize(F_FILE* f, ProtectionControl &c);
    void deserialize(F_FILE* f, ProtectionControl &c);
};

class EmergencyInputControl;
template<> class Serializer<EmergencyInputControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_EmergencyInputControl;}
    void serialize(F_FILE* f, EmergencyInputControl &c);
    void deserialize(F_FILE* f, EmergencyInputControl &c);
};

class ModbusInverterControl;
template<> class Serializer<ModbusInverterControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_ModbusInverterControl;}
    void serialize(F_FILE* f, ModbusInverterControl &c);
    void deserialize(F_FILE* f, ModbusInverterControl &c);
};

class DivideTwoDevicesControl;
template<> class Serializer<DivideTwoDevicesControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_DivideTwoDevicesControl;}
    void serialize(F_FILE* f, DivideTwoDevicesControl &c);
    void deserialize(F_FILE* f, DivideTwoDevicesControl &c);
};

class MinTwoDevicesControl;
template<> class Serializer<MinTwoDevicesControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_MinTwoDevicesControl;}
    void serialize(F_FILE* f, MinTwoDevicesControl &c);
    void deserialize(F_FILE* f, MinTwoDevicesControl &c);
};

class LiquidLevelPumpControl;
template<> class Serializer<LiquidLevelPumpControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_LiquidLevelPumpControl;}
    void serialize(F_FILE* f, LiquidLevelPumpControl &c);
    void deserialize(F_FILE* f, LiquidLevelPumpControl &c);
};

class AddTwoDevicesControl;
template<> class Serializer<AddTwoDevicesControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_AddTwoDevicesControl;}
    void serialize(F_FILE* f, AddTwoDevicesControl &c);
    void deserialize(F_FILE* f, AddTwoDevicesControl &c);
};

class MaxTwoDevicesControl;
template<> class Serializer<MaxTwoDevicesControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_MaxTwoDevicesControl;}
    void serialize(F_FILE* f, MaxTwoDevicesControl &c);
    void deserialize(F_FILE* f, MaxTwoDevicesControl &c);
};

class MultiplyTwoDevicesControl;
template<> class Serializer<MultiplyTwoDevicesControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_MultiplyTwoDevicesControl;}
    void serialize(F_FILE* f, MultiplyTwoDevicesControl &c);
    void deserialize(F_FILE* f, MultiplyTwoDevicesControl &c);
};

class SubtractTwoDevicesControl;
template<> class Serializer<SubtractTwoDevicesControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_SubtractTwoDevicesControl;}
    void serialize(F_FILE* f, SubtractTwoDevicesControl &c);
    void deserialize(F_FILE* f, SubtractTwoDevicesControl &c);
};

class DeviceProtectionChecker;
template<> class Serializer<DeviceProtectionChecker> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_DeviceProtectionChecker;}
    void serialize(F_FILE* f, DeviceProtectionChecker &c);
    void deserialize(F_FILE* f, DeviceProtectionChecker &c);
};

class ProtectionConstantDeltaChecker;
template<> class Serializer<ProtectionConstantDeltaChecker> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_ProtectionConstantDeltaChecker;}
    void serialize(F_FILE* f, ProtectionConstantDeltaChecker &c);
    void deserialize(F_FILE* f, ProtectionConstantDeltaChecker &c);
};

class ProtectionProportionalChecker;
template<> class Serializer<ProtectionProportionalChecker> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_ProtectionProportionalChecker;}
    void serialize(F_FILE* f, ProtectionProportionalChecker &c);
    void deserialize(F_FILE* f, ProtectionProportionalChecker &c);
};

class ProtectionCurrentLimitsChecker;
template<> class Serializer<ProtectionCurrentLimitsChecker> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_ProtectionCurrentLimitsChecker;}
    void serialize(F_FILE* f, ProtectionCurrentLimitsChecker &c);
    void deserialize(F_FILE* f, ProtectionCurrentLimitsChecker &c);
};

class DeviceThresholdChecker;
template<> class Serializer<DeviceThresholdChecker> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_DeviceThresholdChecker;}
    void serialize(F_FILE* f, DeviceThresholdChecker &c);
    void deserialize(F_FILE* f, DeviceThresholdChecker &c);
};

class CalculateOnTwoDevicesControl;
template<> class Serializer<CalculateOnTwoDevicesControl> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_CalculateOnTwoDevicesControl;}
    void serialize(F_FILE* f, CalculateOnTwoDevicesControl &c);
    void deserialize(F_FILE* f, CalculateOnTwoDevicesControl &c);
};

class ProtectionCheckerBase;
template<> class Serializer<ProtectionCheckerBase> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_ProtectionCheckerBase;}
    void serialize(F_FILE* f, ProtectionCheckerBase &c);
    void deserialize(F_FILE* f, ProtectionCheckerBase &c);
};

struct T_OperationNode;
template<> class Serializer<T_OperationNode> : public SerializerBase
{
public:
    uint8_t getSerializationVersion() {return 1;}
    uint8_t getClassType() {return E_ControlSerializationType_T_OperationNode;}
    void serialize(F_FILE* f, T_OperationNode &c);
    void deserialize(F_FILE* f, T_OperationNode &c);
};

#endif /* CONTROLSERIALIZERS_H_ */
