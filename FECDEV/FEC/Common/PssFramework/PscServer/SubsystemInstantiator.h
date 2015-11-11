/*
 * SubsystemInstantiator.h
 *
 *  Created on: 7 Jul 2013
 *      Author: maora
 */

#ifndef SUBSYSTEMINSTANTIATOR_H_
#define SUBSYSTEMINSTANTIATOR_H_

#include <DynamicArray.h>
#include <Controls/ControlBase.h>
#include <Controls/PidControl.h>
#include <Controls/AirBleedingControl.h>

class SubsystemInstantiator
{
    DynamicArray<PidControl*> m_pidControlArray;
    DynamicArray<ValidationElementFloat*> m_temperatureSensorArray;
    DynamicArray<AirBleedingControl*> m_airBleedingControlArray;

public:
    SubsystemInstantiator();
    virtual ~SubsystemInstantiator();

    void ith();
    void ibt();
    void dryer();
    void btc();

    PidControl* getPid(int index);
    ValidationElementFloat* getTemperatureSensor(int index);
    AirBleedingControl* getAirBleedingControl(int index);
};

#endif /* SUBSYSTEMINSTANTIATOR_H_ */
