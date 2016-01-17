/*
 * ControlRepository.h
 *
 *  Created on: 9 Jul 2013
 *      Author: maora
 */

#ifndef CONTROLREPOSITORY_H_
#define CONTROLREPOSITORY_H_

#include "ControlBase.h"
#include <DynamicArray.h>
#include "PidControl.h"
#include "ProtectionControl.h"
#include "AnalogOutInverterControl.h"
#include "ModbusInverterControl.h"
#include "ActivationWithFeedbackControl.h"
#include "HysteresisControl.h"
#include "LiquidLevel3Sensors.h"
#include "LiquidLevelPumpControl.h"
#include "AnalogLiquidLevelControl.h"
#include "EmergencyInputControl.h"
#include "LiftPbOnError.h"
#include "LiftPbOnErrorCcsGen2.h"
#include "LiftPbOnErrorCcsGen3.h"
#include <list>
#include <vector>
#include "OrderedShutdownControl.h"
#include "LeakDetectionControl.h"
#include "ObserveAndNotifyControl.h"
#include "CalculateOnTwoDevicesControl.h"

//typedef DynamicArray<ControlBase*> T_ControlArray;
typedef std::vector<ControlBase*> T_ControlList;
typedef std::vector<ControlBase*>::iterator T_ControlListIterator;

class ControlRepository
{
private:
    static ControlRepository s_instance;

    T_ControlList m_controlList;
    ProtectionControl* m_protectionControl;
    EmergencyInputControl* m_emergencyInputControl;
    OrderedShutdownControl* m_orderedShutdownControl;

    ControlRepository();

public:
    virtual ~ControlRepository();

    static ControlRepository& getInstance()
    {
        return s_instance;
    }

    ControlBase* getControlByPssId(int pssId);
    ControlBase* getControlByIndex(int index);
    void addControl(ControlBase* control);

    void setAllMonitoringEnabled(bool monitoringEnabled);

    void destroyAllControls();

    void initProtectionControl();
    void initEmergencyInputControl();
    ProtectionControl* getProtectionControl();
    EmergencyInputControl* getEmergencyInputControl();

    void resetAllControlsToOn(bool exceptStopOnDisconnection);
    void initAllControls(bool exceptStopOnDisconnection);
    void stopAllControls(bool exceptStopOnDisconnection);
    void stopOnEmr();
    void recoverFromEmergency();
    bool isInEmergency();

    void sendUpdateNotificationForAllControls();

    bool addShutdownOperation(int delay, uint16_t control, E_ShutdownOperation oepration, float setpoint);
    void executeShutdownOperation(bool exceptStopOnDisconnection);

    void setBoardInReady(bool state);
    void startRecovery();
};

#endif /* CONTROLREPOSITORY_H_ */
