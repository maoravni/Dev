/*
 * ControlRepositorySerializer.cpp
 *
 *  Created on: 9 May 2016
 *      Author: maora
 */

#include <Persistency/ControlRepositorySerializer.h>
#include <Controls/ControlRepository.h>
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
#include <logger.h>

struct T_ControlRepositoryIndexes
{
    uint16_t m_protectionControl;
    uint16_t m_emergencyInputControl;
    uint16_t m_orderedShutdownControl;
};

void Serializer<ControlRepository>::serialize(F_FILE* f, ControlRepository& cr)
{
    int result;

    storeStartPosition(f);

    serializeVersion(f);

    // go over all elements and count the number of elements that have a PSSID:
    uint16_t numOfControls = 0;

    numOfControls = cr.m_controlList.size();

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Serializing %d/%d Controls", numOfControls, cr.m_controlList.size());

    M_FWRITE_VARIABLE(numOfControls, f);

    // write a placeholder for the map record:
    int mapPos = f_tell(f);
    EntityMapRecord entityMapRecord =
    { 0 };
    for (int i = 0; i < cr.m_controlList.size(); ++i)
        M_FWRITE_VARIABLE(entityMapRecord, f);

    updateRecordSize(f);

    T_EntityMapRecordVector entityMapVec;

    T_ControlRepositoryIndexes ici;
    if (cr.m_protectionControl == NULL)
        ici.m_protectionControl = -1;
    else
        ici.m_protectionControl = cr.m_protectionControl->getControlIndex();
    if (cr.m_emergencyInputControl == NULL)
        ici.m_emergencyInputControl = -1;
    else
        ici.m_emergencyInputControl = cr.m_emergencyInputControl->getControlIndex();
    if (cr.m_orderedShutdownControl == NULL)
        ici.m_orderedShutdownControl = -1;
    else
        ici.m_orderedShutdownControl = cr.m_orderedShutdownControl->getControlIndex();

    M_FWRITE_VARIABLE(ici, f);

    // serialize each of the peripherals:
    for (int i = 0; i < cr.m_controlList.size(); ++i)
    {
        // store the position of the elements in the map record.
        entityMapRecord.pssId = cr.m_controlList[i]->getPssId();
        entityMapRecord.filePos = f_tell(f);
        entityMapVec.push_back(entityMapRecord);

        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Serializing control %d @ %d", entityMapRecord.pssId,
                entityMapRecord.filePos);

        cr.m_controlList[i]->serialize(f);
    }

    assert(entityMapVec.size() == numOfControls);

    if (f_seek(f, mapPos, F_SEEK_SET) != F_NO_ERROR)
        throw "File operation Failed";
    for (int i = 0; i < numOfControls; ++i)
    {
        M_FWRITE_VARIABLE(entityMapVec[i], f);
    }

}

void Serializer<ControlRepository>::deserialize(F_FILE* f, ControlRepository& cr)
{
    int result;

    deserializeRecordSize(f);

    deserializeVersion(f);

    // go over all elements and count the number of elements that have a PSSID:
    uint16_t numOfControls = 0;

    M_FREAD_VARIABLE(numOfControls, f);

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Deserializing %d/%d Controls", numOfControls, cr.m_controlList.size());

    EntityMapRecord mapRecord;
    T_EntityMapRecordVector mapVec;

    for (int i = 0; i < numOfControls; ++i)
    {
        M_FREAD_VARIABLE(mapRecord, f);

        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Mapping control %d @ %d", mapRecord.pssId, mapRecord.filePos);

        mapVec.push_back(mapRecord);
    }

    EntityMapRecord entityMapRecord;
    T_EntityMapRecordVector entityMapVec;

    T_ControlRepositoryIndexes ici;
    M_FREAD_VARIABLE(ici, f);

    for (int i = 0; i < numOfControls; ++i)
    {
        ControlBase* c = deserializeControl(f, cr);

//        if (c->m_controlIndex == ici.m_protectionControl)
//            cr.m_protectionControl = static_cast<ProtectionControl*>(c);
        if (c->m_controlIndex == ici.m_emergencyInputControl)
            cr.m_emergencyInputControl = static_cast<EmergencyInputControl*>(c);
        if (c->m_controlIndex == ici.m_orderedShutdownControl)
            cr.m_orderedShutdownControl = static_cast<OrderedShutdownControl*>(c);
    }

}

ControlBase* Serializer<ControlRepository>::deserializeControl(F_FILE* f, ControlRepository& cr)
{
    int elementStartPosition;
    elementStartPosition = f_tell(f);

    deserializeRecordSize(f);

    uint8_t classType;
    M_FREAD_VARIABLE(classType, f);

    if (f_seek(f, elementStartPosition, F_SEEK_SET) != F_NO_ERROR)
        throw "File operation Failed";

    ControlBase* c;
    switch (classType)
    {
    case E_ControlSerializationType_ControlBase:
        throw "Bad Deserialisation";
    case E_ControlSerializationType_LiquidLevel3Sensors:
    {
        c = new LiquidLevel3Sensors;
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_PidControl:
    {
        c = new PidControl(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_ActivationWithFeedbackControl:
    {
        c = new ActivationWithFeedbackControl(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_AddTwoDevicesControl:
    {
        c = new AddTwoDevicesControl(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_AnalogLiquidLevelControl:
    {
        c = new AnalogLiquidLevelControl(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_AnalogOutInverterControl:
    {
        c = new AnalogOutInverterControl(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_ConcentrationControl:
    {
        c = new ConcentrationControl(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_DivideTwoDevicesControl:
    {
        c = new DivideTwoDevicesControl(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_EmergencyInputControl:
    {
        c = new EmergencyInputControl(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_HysteresisControl:
    {
        c = new HysteresisControl(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_LiftPbOnError:
    {
        c = new LiftPbOnError(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_LiftPbOnErrorCcsGen2:
    {
        c = new LiftPbOnErrorCcsGen2(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_LiftPbOnErrorCcsGen3:
    {
        c = new LiftPbOnErrorCcsGen3(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_LiquidLevelPumpControl:
    {
        c = new LiquidLevelPumpControl(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_MaxTwoDevicesControl:
    {
        c = new MaxTwoDevicesControl(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_MinTwoDevicesControl:
    {
        c = new MinTwoDevicesControl(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_ModbusInverterControl:
    {
        c = new ModbusInverterControl(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_MultiplyTwoDevicesControl:
    {
        c = new MultiplyTwoDevicesControl(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_ObserveAndNotifyControl:
    {
        c = new ObserveAndNotifyControl(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_OrderedShutdownControl:
    {
        c = new OrderedShutdownControl(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_ProtectionControl:
    {
        c = new ProtectionControl(f);
        if (cr.m_protectionControl == NULL)
        {
            cr.m_protectionControl = static_cast<ProtectionControl*>(c);
        }
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_SubtractTwoDevicesControl:
    {
        c = new SubtractTwoDevicesControl(f);
        cr.addControl(c);
        break;
    }
    case E_ControlSerializationType_DeviceProtectionChecker:
        throw "Bad Deserialisation";
    case E_ControlSerializationType_ProtectionConstantDeltaChecker:
        throw "Bad Deserialisation";
    case E_ControlSerializationType_ProtectionProportionalChecker:
        throw "Bad Deserialisation";
    case E_ControlSerializationType_ProtectionCurrentLimitsChecker:
        throw "Bad Deserialisation";
    case E_ControlSerializationType_DeviceThresholdChecker:
        throw "Bad Deserialisation";
    case E_ControlSerializationType_CalculateOnTwoDevicesControl:
        throw "Bad Deserialisation";
    case E_ControlSerializationType_ProtectionCheckerBase:
        throw "Bad Deserialisation";
    case E_ControlSerializationType_T_OperationNode:
        throw "Bad Deserialisation";
    default:
        throw "Bad Deserialisation";
    }

    return c;
}

