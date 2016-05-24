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

    T_ControlRepositoryIndexes ici;
    ici.m_protectionControl = cr.m_protectionControl->getControlIndex();
    ici.m_emergencyInputControl = cr.m_emergencyInputControl->getControlIndex();
    ici.m_orderedShutdownControl = cr.m_orderedShutdownControl->getControlIndex();

    M_FWRITE_VARIABLE(ici, f);
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

    EntityMapRecord entityMapRecord;
    T_EntityMapRecordVector entityMapVec;

    // serialize each of the peripherals:
    for (int i = 0; i < cr.m_controlList.size(); ++i)
    {
        M_FREAD_VARIABLE(entityMapRecord, f);

        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Mapping control %d @ %d", entityMapRecord.pssId, entityMapRecord.filePos);

        entityMapVec.push_back(entityMapRecord);
    }

    for (int i = 0; i < numOfControls; ++i)
    {
        deserializeControl(f, cr);
    }

    // TODO: deserialize the hard referenced controls:
    T_ControlRepositoryIndexes ici;
    ici.m_protectionControl = cr.m_protectionControl->getControlIndex();
    ici.m_emergencyInputControl = cr.m_emergencyInputControl->getControlIndex();
    ici.m_orderedShutdownControl = cr.m_orderedShutdownControl->getControlIndex();

    M_FWRITE_VARIABLE(ici, f);
}

void Serializer<ControlRepository>::deserializeControl(F_FILE* f, ControlRepository& cr)
{
    int elementStartPosition;
    elementStartPosition = f_tell(f);

    deserializeRecordSize(f);

    uint8_t classType = deserializeClassType(f);

    if (f_seek(f, elementStartPosition, F_SEEK_SET) != F_NO_ERROR)
        throw "File operation Failed";

    switch (classType)
    {
    case E_ControlSerializationType_ControlBase:
        throw "Bad Deserialisation";
    case E_ControlSerializationType_LiquidLevel3Sensors:
    {
        ControlBase* c = new LiquidLevel3Sensors;
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_PidControl:
    {
        ControlBase* c = new PidControl(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_ActivationWithFeedbackControl:
    {
        ControlBase* c = new ActivationWithFeedbackControl(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_AddTwoDevicesControl:
    {
        ControlBase* c = new AddTwoDevicesControl(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_AnalogLiquidLevelControl:
    {
        ControlBase* c = new AnalogLiquidLevelControl(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_AnalogOutInverterControl:
    {
        ControlBase* c = new AnalogOutInverterControl(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_ConcentrationControl:
    {
        ControlBase* c = new ConcentrationControl(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_DivideTwoDevicesControl:
    {
        ControlBase* c = new DivideTwoDevicesControl(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_EmergencyInputControl:
    {
        ControlBase* c = new EmergencyInputControl(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_HysteresisControl:
    {
        ControlBase* c = new HysteresisControl(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_LiftPbOnError:
    {
        ControlBase* c = new LiftPbOnError(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_LiftPbOnErrorCcsGen2:
    {
        ControlBase* c = new LiftPbOnErrorCcsGen2(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_LiftPbOnErrorCcsGen3:
    {
        ControlBase* c = new LiftPbOnErrorCcsGen3(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_LiquidLevelPumpControl:
    {
        ControlBase* c = new LiquidLevelPumpControl(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_MaxTwoDevicesControl:
    {
        ControlBase* c = new MaxTwoDevicesControl(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_MinTwoDevicesControl:
    {
        ControlBase* c = new MinTwoDevicesControl(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_ModbusInverterControl:
    {
        ControlBase* c = new ModbusInverterControl(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_MultiplyTwoDevicesControl:
    {
        ControlBase* c = new MultiplyTwoDevicesControl(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_ObserveAndNotifyControl:
    {
        ControlBase* c = new ObserveAndNotifyControl(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_OrderedShutdownControl:
    {
        ControlBase* c = new OrderedShutdownControl(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_ProtectionControl:
    {
        ControlBase* c = new ProtectionControl(f);
        ControlRepository::getInstance().addControl(c);
        break;
    }
    case E_ControlSerializationType_SubtractTwoDevicesControl:
    {
        ControlBase* c = new SubtractTwoDevicesControl(f);
        ControlRepository::getInstance().addControl(c);
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
}

