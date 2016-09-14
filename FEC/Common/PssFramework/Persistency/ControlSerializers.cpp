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
#include <Controls/ControlRepository.h>
#include <Controls/ProtectionAggregatorControl.h>

#define M_FREAD_AND_REFERENCE_CONTROL(element, file) \
    { \
        uint16_t temp; \
        M_FREAD_VARIABLE(temp, file); \
        element = ControlRepository::getInstance().getControlByIndex(temp); \
        if (element == NULL) \
            throw "Element not found"; \
    }

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

    serializeProtection(f, c);

    uint16_t numOfDependencies = c.m_dependentCheckers.size();
    M_FWRITE_VARIABLE(numOfDependencies, f);

    for (int i = 0; i < numOfDependencies; ++i)
    {
        c.m_dependentCheckers[i].serialize(f);
    }

    updateRecordSize(f);
}

void Serializer<ControlBase>::serializeProtection(F_FILE* f, ControlBase& c)
{
    // write the number of protection classes:
    uint16_t numOfProtections = c.m_protectionCheckers.size();
    M_FWRITE_VARIABLE(numOfProtections, f);

    for (int i = 0; i < numOfProtections; ++i)
    {
        uint16_t protectionIndex = c.m_protectionCheckers[i]->getProtectionIndex();
        M_FWRITE_VARIABLE(protectionIndex, f);
        //c.m_protectionCheckers[i]->serialize(f);
    }
}

void Serializer<ControlBase>::deserializeProtection(F_FILE* f, ControlBase& c)
{
    uint16_t protectionIndex;
    M_FREAD_VARIABLE(protectionIndex, f);

    ProtectionCheckerBase* protection = ControlRepository::getInstance().getProtectionControl()->getProtectionByIndex(
            protectionIndex);
    c.addProtectionChecker(protection);
}

void Serializer<ControlBase>::deserialize(F_FILE* f, ControlBase& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    M_FREAD_VARIABLE(c.m_controlIndex, f);
    M_FREAD_VARIABLE(c.m_pssId, f);
    M_FREAD_VARIABLE(c.m_stopOnDisconnection, f);
    M_FREAD_VARIABLE(c.m_stopOnEmrBehavior, f);
    M_FREAD_VARIABLE(c.m_monitoringEnabled, f);
    M_FREAD_VARIABLE(c.m_isEnabled, f);

    // write the number of protection classes:
    uint16_t numOfProtections;
    M_FREAD_VARIABLE(numOfProtections, f);

    // Add protection deserialization
    for (int i = 0; i < numOfProtections; ++i)
    {
        deserializeProtection(f, c);
    }

    uint16_t numOfDependencies;
    M_FREAD_VARIABLE(numOfDependencies, f);

    for (int i = 0; i < numOfDependencies; ++i)
    {
        DeviceThresholdChecker dtc = DeviceThresholdChecker(f);
        c.addDependentElement(dtc);
    }

}

void Serializer<LiquidLevel3Sensors>::serialize(F_FILE* f, LiquidLevel3Sensors& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    uint16_t temp;

    temp = c.m_lowSensor->m_elementIndex;
    temp = c.m_midSensor->m_elementIndex;
    temp = c.m_highSensor->m_elementIndex;
    temp = c.m_calculatedOutputLevel->m_elementIndex;

    updateRecordSize(f);
}

void Serializer<PidControl>::serialize(F_FILE* f, PidControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_ELEMENT_REFERENCE(c.m_input, f);
    M_FWRITE_VARIABLE(c.m_outputIsControl, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_output, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_setpoint, f);
    M_FWRITE_VARIABLE(c.m_feedForward, f);
    M_FWRITE_VARIABLE(c.m_maxTemperature, f);
    M_FWRITE_VARIABLE(c.m_pidCalc, f);

    updateRecordSize(f);
}

void Serializer<PidControl>::deserialize(F_FILE* f, PidControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    M_FREAD_AND_REFERENCE_ELEMENT_WITH_CAST(c.m_input, ValidationElementFloat, f);
    M_FREAD_VARIABLE(c.m_outputIsControl, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_output, f);
    M_FREAD_AND_REFERENCE_ELEMENT_WITH_CAST(c.m_setpoint, ValidationElementFloat, f);
    M_FREAD_VARIABLE(c.m_feedForward, f);
    M_FREAD_VARIABLE(c.m_maxTemperature, f);
    M_FREAD_VARIABLE(c.m_pidCalc, f);
}

void Serializer<ActivationWithFeedbackControl>::serialize(F_FILE* f, ActivationWithFeedbackControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_ELEMENT_REFERENCE(c.m_outputEnableDevice, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_outputDisableDevice, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_feedbackEnabledDevice, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_feedbackDisabledDevice, f);
    M_FWRITE_VARIABLE(c.m_activationOutputValue, f);
    M_FWRITE_VARIABLE(c.m_ignoreProtectionsDelay, f);
    M_FWRITE_VARIABLE(c.m_behaviorOnInit, f);

    uint16_t numOfFeedbacks = c.m_feedbackCheckers.size();
    M_FWRITE_VARIABLE(numOfFeedbacks, f);

    for (int i = 0; i < numOfFeedbacks; ++i)
    {
        c.m_feedbackCheckers[i].serialize(f);
    }

    uint16_t numOfActivationIos;
    if (c.m_pActivateControlDeviceList == NULL)
    {
        numOfActivationIos = 0;
        M_FWRITE_VARIABLE(numOfActivationIos, f);
    }
    else
    {
        numOfActivationIos = c.m_pActivateControlDeviceList->size();
        M_FWRITE_VARIABLE(numOfActivationIos, f);

        for (int i = 0; i < numOfActivationIos; ++i)
        {
            (*c.m_pActivateControlDeviceList)[i].serialize(f);
        }
    }

    uint16_t numOfDeactivationIos;
    if (c.m_pDeactivateControlDeviceList == NULL)
    {
        numOfDeactivationIos = 0;
        M_FWRITE_VARIABLE(numOfDeactivationIos, f);
    }
    else
    {
        numOfDeactivationIos = c.m_pDeactivateControlDeviceList->size();
        M_FWRITE_VARIABLE(numOfDeactivationIos, f);

        for (int i = 0; i < numOfDeactivationIos; ++i)
        {
            (*c.m_pDeactivateControlDeviceList)[i].serialize(f);
        }
    }

    updateRecordSize(f);
}

void Serializer<ActivationWithFeedbackControl>::deserialize(F_FILE* f, ActivationWithFeedbackControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    M_FREAD_AND_REFERENCE_ELEMENT(c.m_outputEnableDevice, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_outputDisableDevice, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_feedbackEnabledDevice, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_feedbackDisabledDevice, f);
    M_FREAD_VARIABLE(c.m_activationOutputValue, f);
    M_FREAD_VARIABLE(c.m_ignoreProtectionsDelay, f);
    M_FREAD_VARIABLE(c.m_behaviorOnInit, f);

    uint16_t numOfFeedbacks;
    M_FREAD_VARIABLE(numOfFeedbacks, f);

    for (int i = 0; i < numOfFeedbacks; ++i)
    {
        DeviceThresholdChecker dtc = DeviceThresholdChecker(f);
        c.m_feedbackCheckers.push_back(f);
    }

    uint16_t numOfActivationIos;
    M_FREAD_VARIABLE(numOfActivationIos, f);
    if (numOfActivationIos != 0)
    {
        for (int i = 0; i < numOfActivationIos; ++i)
        {
            IODeviceChecker iod = IODeviceChecker(f);
            c.addActivateDevice(iod.m_element, iod.m_activateHigh);
        }
    }

    uint16_t numOfDeactivationIos;
    M_FREAD_VARIABLE(numOfDeactivationIos, f);
    if (numOfDeactivationIos != 0)
    {
        for (int i = 0; i < numOfDeactivationIos; ++i)
        {
            IODeviceChecker iod = IODeviceChecker(f);
            c.addDeactivateDevice(iod.m_element, iod.m_activateHigh);
        }
    }
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

void Serializer<AddTwoDevicesControl>::deserialize(F_FILE* f, AddTwoDevicesControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    Serializer<CalculateOnTwoDevicesControl> baseCalculate;
    baseCalculate.deserialize(f, c);

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

void Serializer<SubtractTwoDevicesControl>::deserialize(F_FILE* f, SubtractTwoDevicesControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    Serializer<CalculateOnTwoDevicesControl> baseCalculate;
    baseCalculate.deserialize(f, c);

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

void Serializer<MinTwoDevicesControl>::deserialize(F_FILE* f, MinTwoDevicesControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    Serializer<CalculateOnTwoDevicesControl> baseCalculate;
    baseCalculate.deserialize(f, c);

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

void Serializer<MaxTwoDevicesControl>::deserialize(F_FILE* f, MaxTwoDevicesControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    Serializer<CalculateOnTwoDevicesControl> baseCalculate;
    baseCalculate.deserialize(f, c);

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

void Serializer<MultiplyTwoDevicesControl>::deserialize(F_FILE* f, MultiplyTwoDevicesControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    Serializer<CalculateOnTwoDevicesControl> baseCalculate;
    baseCalculate.deserialize(f, c);

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

void Serializer<DivideTwoDevicesControl>::deserialize(F_FILE* f, DivideTwoDevicesControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    Serializer<CalculateOnTwoDevicesControl> baseCalculate;
    baseCalculate.deserialize(f, c);

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
    M_FWRITE_ELEMENT_REFERENCE(c.m_levelSensorElement, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_calculatedOutputLevel, f);

    updateRecordSize(f);
}

void Serializer<AnalogLiquidLevelControl>::deserialize(F_FILE* f, AnalogLiquidLevelControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    M_FREAD_VARIABLE(c.m_lowLevelValue, f);
    M_FREAD_VARIABLE(c.m_midLevelValue, f);
    M_FREAD_VARIABLE(c.m_highLevelValue, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_levelSensorElement, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_calculatedOutputLevel, f);

}

void Serializer<AnalogOutInverterControl>::serialize(F_FILE* f, AnalogOutInverterControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_ELEMENT_REFERENCE(c.m_enableOutput, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_deviceSetpoint, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_requestedSetpointElement, f);

    updateRecordSize(f);
}

void Serializer<AnalogOutInverterControl>::deserialize(F_FILE* f, AnalogOutInverterControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    M_FREAD_AND_REFERENCE_ELEMENT_WITH_CAST(c.m_enableOutput, ElementU8, f);
    M_FREAD_AND_REFERENCE_ELEMENT_WITH_CAST(c.m_deviceSetpoint, ValidationElementFloat, f);
    M_FREAD_AND_REFERENCE_ELEMENT_WITH_CAST(c.m_requestedSetpointElement, ValidationElementFloat, f);

}

void Serializer<CalculateOnTwoDevicesControl>::serialize(F_FILE* f, CalculateOnTwoDevicesControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_ELEMENT_REFERENCE(c.m_input1, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_input2, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_calculatedOutput, f);

    updateRecordSize(f);
}

void Serializer<CalculateOnTwoDevicesControl>::deserialize(F_FILE* f, CalculateOnTwoDevicesControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    M_FREAD_AND_REFERENCE_ELEMENT(c.m_input1, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_input2, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_calculatedOutput, f);

}

void Serializer<ConcentrationControl>::serialize(F_FILE* f, ConcentrationControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_ELEMENT_REFERENCE(c.m_concentration, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_tankLevel, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_conditionerValve, f);
    M_FWRITE_VARIABLE(c.m_conditionerValveActivationValue, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_waterValve, f);
    M_FWRITE_VARIABLE(c.m_waterValveActivationValue, f);

    M_FWRITE_ELEMENT_REFERENCE(c.m_concentrationLowSetpoint, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_liquidLevelLowSetpoint, f);
    M_FWRITE_VARIABLE(c.m_concentrationHighSetpoint, f);
    M_FWRITE_VARIABLE(c.m_liquidLevelHighSetpoint, f);

    updateRecordSize(f);
}

void Serializer<ConcentrationControl>::deserialize(F_FILE* f, ConcentrationControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    M_FREAD_AND_REFERENCE_ELEMENT(c.m_concentration, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_tankLevel, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_conditionerValve, f);
    M_FREAD_VARIABLE(c.m_conditionerValveActivationValue, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_waterValve, f);
    M_FREAD_VARIABLE(c.m_waterValveActivationValue, f);

    M_FREAD_AND_REFERENCE_ELEMENT_WITH_CAST(c.m_concentrationLowSetpoint, ValidationElementFloat, f);
    M_FREAD_AND_REFERENCE_ELEMENT_WITH_CAST(c.m_liquidLevelLowSetpoint, ValidationElementFloat, f);
    M_FREAD_VARIABLE(c.m_concentrationHighSetpoint, f);
    M_FREAD_VARIABLE(c.m_liquidLevelHighSetpoint, f);

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

void Serializer<DeviceProtectionChecker>::deserialize(F_FILE* f, DeviceProtectionChecker& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ProtectionCheckerBase> baseC;
    baseC.deserialize(f, c);

    Serializer<RangeChecker<float> > rs;
    rs.deserialize(f, c.m_softProtectionRange);
    rs.deserialize(f, c.m_hardProtectionRange);
}

void Serializer<DeviceThresholdChecker>::serialize(F_FILE* f, DeviceThresholdChecker& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    M_FWRITE_ELEMENT_REFERENCE(c.m_element, f);
    M_FWRITE_VARIABLE(c.m_pssId, f);

    updateRecordSize(f);
}

void Serializer<DeviceThresholdChecker>::deserialize(F_FILE* f, DeviceThresholdChecker& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    M_FREAD_AND_REFERENCE_ELEMENT(c.m_element, f);
    M_FREAD_VARIABLE(c.m_pssId, f);

}

void Serializer<EmergencyInputControl>::serialize(F_FILE* f, EmergencyInputControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_ELEMENT_REFERENCE(c.m_emergencyInputElement, f);

    updateRecordSize(f);
}

void Serializer<EmergencyInputControl>::deserialize(F_FILE* f, EmergencyInputControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    M_FREAD_AND_REFERENCE_ELEMENT(c.m_emergencyInputElement, f);

}

void Serializer<HysteresisControl>::serialize(F_FILE* f, HysteresisControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_ELEMENT_REFERENCE(c.m_input, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_output, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_activateSetpointElement, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_deactivateSetpointElement, f);
    M_FWRITE_VARIABLE(c.m_activateSetpoint, f);
    M_FWRITE_VARIABLE(c.m_deactivateSetpoint, f);
    M_FWRITE_VARIABLE(c.m_outputValue, f);
    M_FWRITE_VARIABLE(c.m_maxTemperature, f);
    M_FWRITE_VARIABLE(c.m_coolingMode, f);

    updateRecordSize(f);
}

void Serializer<HysteresisControl>::deserialize(F_FILE* f, HysteresisControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    M_FREAD_AND_REFERENCE_ELEMENT_WITH_CAST(c.m_input, ValidationElementFloat, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_output, f);
    M_FREAD_AND_REFERENCE_ELEMENT_WITH_CAST(c.m_activateSetpointElement, ValidationElementFloat, f);
    M_FREAD_AND_REFERENCE_ELEMENT_WITH_CAST(c.m_deactivateSetpointElement, ValidationElementFloat, f);
    M_FREAD_VARIABLE(c.m_activateSetpoint, f);
    M_FREAD_VARIABLE(c.m_deactivateSetpoint, f);
    M_FREAD_VARIABLE(c.m_outputValue, f);
    M_FREAD_VARIABLE(c.m_maxTemperature, f);
    M_FREAD_VARIABLE(c.m_coolingMode, f);

}

void Serializer<LiftPbOnError>::serialize(F_FILE* f, LiftPbOnError& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_ELEMENT_REFERENCE(c.m_airPressure, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_tubEngage[0], f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_tubEngage[1], f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_tubEngage[2], f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_tubEngage[3], f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_squeegeeEngage[0], f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_squeegeeEngage[1], f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_pbOk, f);
    M_FWRITE_VARIABLE(c.m_cableId, f);

    updateRecordSize(f);
}

void Serializer<LiftPbOnError>::deserialize(F_FILE* f, LiftPbOnError& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    M_FREAD_AND_REFERENCE_ELEMENT(c.m_airPressure, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_tubEngage[0], f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_tubEngage[1], f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_tubEngage[2], f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_tubEngage[3], f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_squeegeeEngage[0], f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_squeegeeEngage[1], f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_pbOk, f);
    M_FREAD_VARIABLE(c.m_cableId, f);

}

void Serializer<LiftPbOnErrorCcsGen2>::serialize(F_FILE* f, LiftPbOnErrorCcsGen2& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_ELEMENT_REFERENCE(c.m_tubEngage[0], f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_tubEngage[1], f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_tubDisengage[0], f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_tubDisengage[1], f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_blanketMoving, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_airPressureOk, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_pbOk, f);
    M_FWRITE_VARIABLE(c.m_cableId, f);

    updateRecordSize(f);
}

void Serializer<LiftPbOnErrorCcsGen2>::deserialize(F_FILE* f, LiftPbOnErrorCcsGen2& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    M_FREAD_AND_REFERENCE_ELEMENT(c.m_tubEngage[0], f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_tubEngage[1], f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_tubDisengage[0], f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_tubDisengage[1], f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_blanketMoving, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_airPressureOk, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_pbOk, f);
    M_FREAD_VARIABLE(c.m_cableId, f);

}

void Serializer<LiftPbOnErrorCcsGen3>::serialize(F_FILE* f, LiftPbOnErrorCcsGen3& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_ELEMENT_REFERENCE(c.m_airPressureUp, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_airPressureDown, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_blanketMoving, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_airPressureOk, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_pbOk, f);
    M_FWRITE_VARIABLE(c.m_cableId, f);

    updateRecordSize(f);
}

void Serializer<LiftPbOnErrorCcsGen3>::deserialize(F_FILE* f, LiftPbOnErrorCcsGen3& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    M_FREAD_AND_REFERENCE_ELEMENT(c.m_airPressureUp, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_airPressureDown, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_blanketMoving, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_airPressureOk, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_pbOk, f);
    M_FREAD_VARIABLE(c.m_cableId, f);

}

void Serializer<LiquidLevelPumpControl>::serialize(F_FILE* f, LiquidLevelPumpControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_ELEMENT_REFERENCE(c.m_tankLevelInput, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_drainPump, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_fillPump, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_lowSetpoint, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_midSetpoint, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_highSetpoint, f);

    M_FWRITE_VARIABLE(c.m_currentSetpoint, f);
    M_FWRITE_VARIABLE(c.m_fillPumpValue, f);
    M_FWRITE_VARIABLE(c.m_currentTankLevel, f);

    updateRecordSize(f);
}

void Serializer<LiquidLevelPumpControl>::deserialize(F_FILE* f, LiquidLevelPumpControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    M_FREAD_AND_REFERENCE_ELEMENT(c.m_tankLevelInput, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_drainPump, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_fillPump, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_lowSetpoint, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_midSetpoint, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_highSetpoint, f);

    M_FREAD_VARIABLE(c.m_currentSetpoint, f);
    M_FREAD_VARIABLE(c.m_fillPumpValue, f);
    M_FREAD_VARIABLE(c.m_currentTankLevel, f);
}

void Serializer<ModbusInverterControl>::serialize(F_FILE* f, ModbusInverterControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_ELEMENT_REFERENCE(c.m_requestedSetpoint, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_enableOutput, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_setpoint, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_outputCurrent, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_outputFrequency, f);
    //M_FWRITE_ELEMENT_REFERENCE(c.m_inverterPeripheral, f);

    updateRecordSize(f);
}

void Serializer<ModbusInverterControl>::deserialize(F_FILE* f, ModbusInverterControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    M_FREAD_AND_REFERENCE_ELEMENT_WITH_CAST(c.m_requestedSetpoint, ValidationElementFloat, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_enableOutput, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_setpoint, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_outputCurrent, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_outputFrequency, f);
    //M_FREAD_AND_REFERENCE_ELEMENT(c.m_driveStatus, f);

}

void Serializer<ObserveAndNotifyControl>::serialize(F_FILE* f, ObserveAndNotifyControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_ELEMENT_REFERENCE(c.m_input, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_setpoint, f);

    updateRecordSize(f);
}

void Serializer<ObserveAndNotifyControl>::deserialize(F_FILE* f, ObserveAndNotifyControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    M_FREAD_AND_REFERENCE_ELEMENT_WITH_CAST(c.m_input, ValidationElementFloat, f);
    M_FREAD_AND_REFERENCE_ELEMENT_WITH_CAST(c.m_setpoint, ValidationElementFloat, f);
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

void Serializer<OrderedShutdownControl>::deserialize(F_FILE* f, OrderedShutdownControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    M_FREAD_VARIABLE(c.m_exceptStopOnDisconnection, f);

    uint16_t numOfOperations;
    M_FREAD_VARIABLE(numOfOperations, f);

    Serializer<T_OperationNode> on;
    T_OperationNode n;
    for (int i = 0; i < numOfOperations; ++i)
    {
        on.deserialize(f, n);
        c.m_shutdownOperationList.push_back(n);
    }
}

void Serializer<ProtectionCheckerBase>::serialize(F_FILE* f, ProtectionCheckerBase& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    M_FWRITE_ELEMENT_REFERENCE(c.m_observedElement, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_protectionStatus, f);
    M_FWRITE_VARIABLE(c.m_debounceTimeout, f);
    M_FWRITE_VARIABLE(c.m_protectionIndex, f);

    updateRecordSize(f);
}

void Serializer<ProtectionCheckerBase>::deserialize(F_FILE* f, ProtectionCheckerBase& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    M_FREAD_AND_REFERENCE_ELEMENT(c.m_observedElement, f);
    if (c.m_observedElement != NULL)
        c.m_observedElement->addObserver(&c);
    M_FREAD_AND_REFERENCE_ELEMENT_WITH_CAST(c.m_protectionStatus, ElementU8, f);
    M_FREAD_VARIABLE(c.m_debounceTimeout, f);
    M_FREAD_VARIABLE(c.m_protectionIndex, f);

}
void Serializer<ProtectionConstantDeltaChecker>::serialize(F_FILE* f, ProtectionConstantDeltaChecker& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ProtectionCheckerBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_ELEMENT_REFERENCE(c.m_referenceElement, f);

    Serializer<RangeChecker<float> > rs;
    rs.serialize(f, c.m_allowedRange);

    updateRecordSize(f);
}

void Serializer<ProtectionConstantDeltaChecker>::deserialize(F_FILE* f, ProtectionConstantDeltaChecker& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ProtectionCheckerBase> baseC;
    baseC.deserialize(f, c);

    M_FREAD_AND_REFERENCE_ELEMENT(c.m_referenceElement, f);

    Serializer<RangeChecker<float> > rs;
    rs.deserialize(f, c.m_allowedRange);
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

void Serializer<ProtectionCurrentLimitsChecker>::deserialize(F_FILE* f, ProtectionCurrentLimitsChecker& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ProtectionCheckerBase> baseC;
    baseC.deserialize(f, c);

    Serializer<RangeChecker<float> > rs;
    rs.deserialize(f, c.m_allowedRangeWarning);
    rs.deserialize(f, c.m_allowedRangeError);
}

void Serializer<ProtectionProportionalChecker>::serialize(F_FILE* f, ProtectionProportionalChecker& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ProtectionCheckerBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_ELEMENT_REFERENCE(c.m_referenceElement, f);

    Serializer<RangeChecker<float> > rs;
    rs.serialize(f, c.m_allowedRange);

    M_FWRITE_VARIABLE(c.m_gain, f);
    M_FWRITE_VARIABLE(c.m_offset, f);

    updateRecordSize(f);
}

void Serializer<ProtectionProportionalChecker>::deserialize(F_FILE* f, ProtectionProportionalChecker& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ProtectionCheckerBase> baseC;
    baseC.deserialize(f, c);

    M_FREAD_AND_REFERENCE_ELEMENT(c.m_referenceElement, f);

    Serializer<RangeChecker<float> > rs;
    rs.deserialize(f, c.m_allowedRange);

    M_FREAD_VARIABLE(c.m_gain, f);
    M_FREAD_VARIABLE(c.m_offset, f);

}

void Serializer<ProtectionControl>::serialize(F_FILE* f, ProtectionControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

//    Serializer<ControlBase> baseC;
//    baseC.serialize(f, c);
    Serializer<ControlBase>::serialize(f, c);

    M_FWRITE_ELEMENT_REFERENCE(c.m_dryContactElement, f);

    updateRecordSize(f);
}

void Serializer<ProtectionControl>::deserialize(F_FILE* f, ProtectionControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

//    Serializer<ControlBase> baseS;
//    baseS.deserialize(f, c);
    Serializer<ControlBase>::deserialize(f, c);

    M_FREAD_AND_REFERENCE_ELEMENT(c.m_dryContactElement, f);
}

void Serializer<ProtectionControl>::serializeProtection(F_FILE* f, ControlBase& c)
{
    // write the number of protection classes:
    uint16_t numOfProtections = c.m_protectionCheckers.size();
    M_FWRITE_VARIABLE(numOfProtections, f);

    for (int i = 0; i < numOfProtections; ++i)
    {
        c.m_protectionCheckers[i]->serialize(f);
    }
}

void Serializer<ProtectionControl>::deserializeProtection(F_FILE* f, ControlBase& c)
{
    int elementStartPosition;
    elementStartPosition = f_tell(f);

    deserializeRecordSize(f);

    uint8_t classType;
    M_FREAD_VARIABLE(classType, f);

    if (f_seek(f, elementStartPosition, F_SEEK_SET) != F_NO_ERROR)
        throw "File operation Failed";

    ProtectionControl *pc = static_cast<ProtectionControl*>(&c);

    switch (classType)
    {
    case E_ControlSerializationType_DeviceProtectionChecker:
    {
        DeviceProtectionChecker *p = new DeviceProtectionChecker(f);
        pc->addProtectionChecker(p);
        break;
    }
    case E_ControlSerializationType_ProtectionConstantDeltaChecker:
    {
        ProtectionConstantDeltaChecker *p = new ProtectionConstantDeltaChecker(f);
        pc->addProtectionCheckerButDontSubsribe(p);
        break;
    }
    case E_ControlSerializationType_ProtectionProportionalChecker:
    {
        ProtectionProportionalChecker *p = new ProtectionProportionalChecker(f);
        pc->addProtectionCheckerButDontSubsribe(p);
        break;
    }
    case E_ControlSerializationType_ProtectionCurrentLimitsChecker:
    {
        ProtectionCurrentLimitsChecker *p = new ProtectionCurrentLimitsChecker(f);
        pc->addProtectionCheckerButDontSubsribe(p);
        break;
    }
    default:
        throw "Bad Deserialisation";
    }
}

void Serializer<T_OperationNode>::serialize(F_FILE* f, T_OperationNode& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    M_FWRITE_VARIABLE(c.delay, f);
    M_FWRITE_VARIABLE(c.control->m_controlIndex, f);
    M_FWRITE_VARIABLE(c.operation, f);
    M_FWRITE_VARIABLE(c.setpoint, f);

    updateRecordSize(f);
}

void Serializer<T_OperationNode>::deserialize(F_FILE* f, T_OperationNode& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    M_FREAD_VARIABLE(c.delay, f);
    M_FREAD_AND_REFERENCE_CONTROL(c.control, f);
    M_FREAD_VARIABLE(c.operation, f);
    M_FREAD_VARIABLE(c.setpoint, f);
}

void Serializer<ConcentrationCalculatorControl>::serialize(F_FILE* f, ConcentrationCalculatorControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_ELEMENT_REFERENCE(c.m_viscosity, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_temperature, f);
    M_FWRITE_ELEMENT_REFERENCE(c.m_calculatedOutput, f);

    M_FWRITE_VARIABLE(c.m_concentration1, f);
    M_FWRITE_VARIABLE(c.m_intercept1, f);
    M_FWRITE_VARIABLE(c.m_slope1, f);
    M_FWRITE_VARIABLE(c.m_concentration2, f);
    M_FWRITE_VARIABLE(c.m_intercept2, f);
    M_FWRITE_VARIABLE(c.m_slope2, f);

    updateRecordSize(f);
}

void Serializer<ConcentrationCalculatorControl>::deserialize(F_FILE* f, ConcentrationCalculatorControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    M_FREAD_AND_REFERENCE_ELEMENT(c.m_viscosity, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_temperature, f);
    M_FREAD_AND_REFERENCE_ELEMENT(c.m_calculatedOutput, f);

    M_FREAD_VARIABLE(c.m_concentration1, f);
    M_FREAD_VARIABLE(c.m_intercept1, f);
    M_FREAD_VARIABLE(c.m_slope1, f);
    M_FREAD_VARIABLE(c.m_concentration2, f);
    M_FREAD_VARIABLE(c.m_intercept2, f);
    M_FREAD_VARIABLE(c.m_slope2, f);

}

void Serializer<ProtectionAggregatorControl>::serialize(F_FILE* f, ProtectionAggregatorControl& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    Serializer<ControlBase> baseC;
    baseC.serialize(f, c);

    M_FWRITE_ELEMENT_REFERENCE(c.m_calculatedOutput, f);

    M_FWRITE_VARIABLE(c.m_negateResult, f);
    M_FWRITE_VARIABLE(c.m_bitwiseOperation, f);

    updateRecordSize(f);
}

void Serializer<ProtectionAggregatorControl>::deserialize(F_FILE* f, ProtectionAggregatorControl& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    Serializer<ControlBase> baseS;
    baseS.deserialize(f, c);

    M_FREAD_AND_REFERENCE_ELEMENT(c.m_calculatedOutput, f);

    M_FREAD_VARIABLE(c.m_negateResult, f);
    M_FREAD_VARIABLE(c.m_bitwiseOperation, f);

}

void Serializer<IODeviceChecker>::serialize(F_FILE* f, IODeviceChecker& c)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    M_FWRITE_ELEMENT_REFERENCE(c.m_element, f);
    M_FWRITE_VARIABLE(c.m_activateHigh, f);

    updateRecordSize(f);
}

void Serializer<IODeviceChecker>::deserialize(F_FILE* f, IODeviceChecker& c)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    M_FREAD_AND_REFERENCE_ELEMENT(c.m_element, f);
    M_FREAD_VARIABLE(c.m_activateHigh, f);

}
