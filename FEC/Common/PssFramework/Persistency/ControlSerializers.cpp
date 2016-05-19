/*
 * ControlSerializers.cpp
 *
 *  Created on: 10 May 2016
 *      Author: maora
 */

#include <Persistency/ControlSerializers.h>
#include <Persistency/RangeCheckerSerializer.h>
#include <Controls/ControlBase.h>
#include <Controls/PidControl.h>
#include <Controls/LiquidLevel3Sensors.h>
#include <Controls/CalculateOnTwoDevicesControl.h>
#include <controls/AnalogLiquidLevelControl.h>
#include <Controls/AnalogOutInverterControl.h>
#include <controls/ConcentrationControl.h>
#include <Controls/DeviceProtectionChecker.h>
#include <Controls/EmergencyInputControl.h>
#include <controls/HysteresisControl.h>
#include <controls/LiftPbOnError.h>
#include <Controls/LiftPbOnErrorCcsGen2.h>
#include <Controls/LiftPbOnErrorCcsGen3.h>
#include <controls/LiquidLevelPumpControl.h>
#include <Controls/ModbusInverterControl.h>
#include <Controls/ObserveAndNotifyControl.h>
#include <Controls/OrderedShutdownControl.h>
#include <Controls/ProtectionConstantDeltaChecker.h>
#include <Controls/ProtectionControl.h>
#include <Controls/ProtectionCurrentLimitsChecker.h>
#include <Controls/ProtectionProportionalChecker.h>

void Serializer<ControlBase>::serialize(F_FILE* f, ControlBase& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    M_FWRITE_VARIABLE(c.m_controlIndex, f);
    M_FWRITE_VARIABLE(c.m_pssId, f);
    M_FWRITE_VARIABLE(c.m_stopOnDisconnection, f);
    M_FWRITE_VARIABLE(c.m_stopOnEmrBehavior, f);
    M_FWRITE_VARIABLE(c.m_monitoringEnabled, f);
    M_FWRITE_VARIABLE(c.m_isEnabled, f);

    // write the number of protection classes:
    uint16_t numOfProtections = c.m_protectionCheckers.size();
    M_FWRITE_VARIABLE(numOfProtections, f);

    for (int i = 0; i < numOfProtections; ++i)
    {
        c.m_protectionCheckers[i]->serialize(f);
    }

    uint16_t numOfDependencies = c.m_dependentCheckers.size();
    M_FWRITE_VARIABLE(numOfDependencies, f);

    for (int i = 0; i < numOfDependencies; ++i)
    {
        c.m_dependentCheckers[i].serialize(f);
    }

    updateRecordSize(f);
}

void Serializer<LiquidLevel3Sensors>::serialize(F_FILE* f, LiquidLevel3Sensors& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    uint16_t temp;

    temp = c.m_lowSensor->m_pssId;
    temp = c.m_midSensor->m_pssId;
    temp = c.m_highSensor->m_pssId;
    temp = c.m_calculatedOutputLevel->m_pssId;

    updateRecordSize(f);
}

void Serializer<PidControl>::serialize(F_FILE* f, PidControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_VARIABLE(c.m_input->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_outputIsControl, f);
    M_FWRITE_VARIABLE(c.m_output->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_setpoint->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_feedForward, f);
    M_FWRITE_VARIABLE(c.m_maxTemperature, f);
    M_FWRITE_VARIABLE(c.m_pidCalc, f);

    updateRecordSize(f);
}

void Serializer<ActivationWithFeedbackControl>::serialize(F_FILE* f, ActivationWithFeedbackControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_VARIABLE(c.m_outputEnableDevice->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_outputDisableDevice->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_activationOutputValue, f);
    M_FWRITE_VARIABLE(c.m_ignoreProtectionsDelay, f);
    M_FWRITE_VARIABLE(c.m_behaviorOnInit, f);

    uint16_t numOfFeedbacks = c.m_feedbackCheckers.size();
    M_FWRITE_VARIABLE(numOfFeedbacks, f);

    for (int i = 0; i < numOfFeedbacks; ++i)
    {
        c.m_feedbackCheckers[i].serialize(f);
    }

    updateRecordSize(f);
}

void Serializer<AddTwoDevicesControl>::serialize(F_FILE* f, AddTwoDevicesControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    Serializer<CalculateOnTwoDevicesControl> baseCalculate;
    baseCalculate.serialize(f, c);

    updateRecordSize(f);
}

void Serializer<SubtractTwoDevicesControl>::serialize(F_FILE* f, SubtractTwoDevicesControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    Serializer<CalculateOnTwoDevicesControl> baseCalculate;
    baseCalculate.serialize(f, c);

    updateRecordSize(f);
}

void Serializer<MinTwoDevicesControl>::serialize(F_FILE* f, MinTwoDevicesControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    Serializer<CalculateOnTwoDevicesControl> baseCalculate;
    baseCalculate.serialize(f, c);

    updateRecordSize(f);
}

void Serializer<MaxTwoDevicesControl>::serialize(F_FILE* f, MaxTwoDevicesControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    Serializer<CalculateOnTwoDevicesControl> baseCalculate;
    baseCalculate.serialize(f, c);

    updateRecordSize(f);
}

void Serializer<MultiplyTwoDevicesControl>::serialize(F_FILE* f, MultiplyTwoDevicesControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    Serializer<CalculateOnTwoDevicesControl> baseCalculate;
    baseCalculate.serialize(f, c);

    updateRecordSize(f);
}

void Serializer<DivideTwoDevicesControl>::serialize(F_FILE* f, DivideTwoDevicesControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    Serializer<CalculateOnTwoDevicesControl> baseCalculate;
    baseCalculate.serialize(f, c);

    updateRecordSize(f);
}

void Serializer<AnalogLiquidLevelControl>::serialize(F_FILE* f, AnalogLiquidLevelControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_VARIABLE(c.m_lowLevelValue, f);
    M_FWRITE_VARIABLE(c.m_midLevelValue, f);
    M_FWRITE_VARIABLE(c.m_highLevelValue, f);
    M_FWRITE_VARIABLE(c.m_levelSensorElement->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_calculatedOutputLevel->m_pssId, f);

    updateRecordSize(f);
}

void Serializer<AnalogOutInverterControl>::serialize(F_FILE* f, AnalogOutInverterControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_VARIABLE(c.m_enableOutput->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_deviceSetpoint->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_requestedSetpointElement->m_pssId, f);

    updateRecordSize(f);
}

void Serializer<CalculateOnTwoDevicesControl>::serialize(F_FILE* f, CalculateOnTwoDevicesControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_VARIABLE(c.m_input1->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_input2->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_calculatedOutput->m_pssId, f);

    updateRecordSize(f);
}

void Serializer<ConcentrationControl>::serialize(F_FILE* f, ConcentrationControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_VARIABLE(c.m_concentration->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_tankLevel->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_conditionerValve->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_conditionerValveActivationValue, f);
    M_FWRITE_VARIABLE(c.m_waterValve->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_waterValveActivationValue, f);

    M_FWRITE_VARIABLE(c.m_concentrationLowSetpoint->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_liquidLevelLowSetpoint->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_concentrationHighSetpoint, f);
    M_FWRITE_VARIABLE(c.m_liquidLevelHighSetpoint, f);

    updateRecordSize(f);
}

void Serializer<DeviceProtectionChecker>::serialize(F_FILE* f, DeviceProtectionChecker& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ProtectionCheckerBase> baseC;
    baseC.serialize(f, c);

    Serializer<RangeChecker<float> > rs;
    rs.serialize(f, c.m_softProtectionRange);
    rs.serialize(f, c.m_hardProtectionRange);

    updateRecordSize(f);
}

void Serializer<DeviceThresholdChecker>::serialize(F_FILE* f, DeviceThresholdChecker& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    M_FWRITE_VARIABLE(c.m_element->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_pssId, f);

    updateRecordSize(f);
}

void Serializer<EmergencyInputControl>::serialize(F_FILE* f, EmergencyInputControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_VARIABLE(c.m_emergencyInputElement->m_pssId, f);

    updateRecordSize(f);
}

void Serializer<HysteresisControl>::serialize(F_FILE* f, HysteresisControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_VARIABLE(c.m_input->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_output->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_activateSetpointElement->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_deactivateSetpointElement->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_activateSetpoint, f);
    M_FWRITE_VARIABLE(c.m_deactivateSetpoint, f);
    M_FWRITE_VARIABLE(c.m_outputValue, f);
    M_FWRITE_VARIABLE(c.m_maxTemperature, f);
    M_FWRITE_VARIABLE(c.m_coolingMode, f);

    updateRecordSize(f);
}

void Serializer<LiftPbOnError>::serialize(F_FILE* f, LiftPbOnError& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_VARIABLE(c.m_airPressure->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_tubEngage[0]->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_tubEngage[1]->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_tubEngage[2]->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_tubEngage[3]->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_squeegeeEngage[0]->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_squeegeeEngage[1]->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_pbOk, f);
    M_FWRITE_VARIABLE(c.m_cableId, f);

    updateRecordSize(f);
}

void Serializer<LiftPbOnErrorCcsGen2>::serialize(F_FILE* f, LiftPbOnErrorCcsGen2& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_VARIABLE(c.m_tubEngage[0]->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_tubEngage[1]->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_tubDisengage[0]->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_tubDisengage[1]->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_blanketMoving->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_airPressureOk->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_pbOk->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_cableId, f);

    updateRecordSize(f);
}

void Serializer<LiftPbOnErrorCcsGen3>::serialize(F_FILE* f, LiftPbOnErrorCcsGen3& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_VARIABLE(c.m_airPressureUp->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_airPressureDown->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_blanketMoving->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_airPressureOk->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_pbOk->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_cableId, f);

    updateRecordSize(f);
}

void Serializer<LiquidLevelPumpControl>::serialize(F_FILE* f, LiquidLevelPumpControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    updateRecordSize(f);
}

void Serializer<ModbusInverterControl>::serialize(F_FILE* f, ModbusInverterControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_VARIABLE(c.m_requestedSetpoint->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_enableOutput->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_setpoint->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_outputCurrent->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_outputFrequency->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_driveStatus->m_pssId, f);

    updateRecordSize(f);
}

void Serializer<ObserveAndNotifyControl>::serialize(F_FILE* f, ObserveAndNotifyControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_VARIABLE(c.m_input->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_setpoint->m_pssId, f);

    updateRecordSize(f);
}

void Serializer<OrderedShutdownControl>::serialize(F_FILE* f, OrderedShutdownControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_VARIABLE(c.m_exceptStopOnDisconnection, f);

    uint16_t numOfOperations = c.m_shutdownOperationList.size();
    M_FWRITE_VARIABLE(numOfOperations, f);

    T_ShutdownOperationListIterator it;
    for (it = c.m_shutdownOperationList.begin(); it != c.m_shutdownOperationList.end(); ++it)
    {
        Serializer<T_OperationNode> on;
        on.serialize(f, (*it));
    }

    updateRecordSize(f);
}

void Serializer<ProtectionCheckerBase>::serialize(F_FILE* f, ProtectionCheckerBase& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    M_FWRITE_VARIABLE(c.m_observedElement->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_protectionStatus->m_pssId, f);
    M_FWRITE_VARIABLE(c.m_debounceTimeout, f);

    updateRecordSize(f);
}

void Serializer<ProtectionConstantDeltaChecker>::serialize(F_FILE* f, ProtectionConstantDeltaChecker& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ProtectionCheckerBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_VARIABLE(c.m_referenceElement->m_pssId, f);

    Serializer<RangeChecker<float> > rs;
    rs.serialize(f, c.m_allowedRange);

    updateRecordSize(f);
}

void Serializer<ProtectionCurrentLimitsChecker>::serialize(F_FILE* f, ProtectionCurrentLimitsChecker& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ProtectionCheckerBase> baseC;
    baseC.serialize(f, c);

    Serializer<RangeChecker<float> > rs;
    rs.serialize(f, c.m_allowedRangeWarning);
    rs.serialize(f, c.m_allowedRangeError);

    updateRecordSize(f);
}

void Serializer<ProtectionProportionalChecker>::serialize(F_FILE* f, ProtectionProportionalChecker& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ProtectionCheckerBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_VARIABLE(c.m_referenceElement->m_pssId, f);

    Serializer<RangeChecker<float> > rs;
    rs.serialize(f, c.m_allowedRange);

    M_FWRITE_VARIABLE(c.m_gain, f);
    M_FWRITE_VARIABLE(c.m_offset, f);

    updateRecordSize(f);
}

void Serializer<ProtectionControl>::serialize(F_FILE* f, ProtectionControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_VARIABLE(c.m_dryContactElement->m_pssId, f);

    updateRecordSize(f);
}

void Serializer<T_OperationNode>::serialize(F_FILE* f, T_OperationNode& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    M_FWRITE_VARIABLE(c.delay, f);
    M_FWRITE_VARIABLE(c.control->m_pssId, f);
    M_FWRITE_VARIABLE(c.operation, f);
    M_FWRITE_VARIABLE(c.setpoint, f);

    updateRecordSize(f);
}
