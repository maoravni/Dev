/*
 * AirBleedingControl.h
 *
 *  Created on: 9 Jul 2013
 *      Author: maora
 */

#ifndef AIRBLEEDINGCONTROL_H_
#define AIRBLEEDINGCONTROL_H_

#include "ControlBase.h"
#include <Elements/Element.h>

class AirBleedingControl: public ControlBase
{
    ElementU8* m_outputs[2];

public:
    AirBleedingControl();
    virtual ~AirBleedingControl();

    void setOutputValue(int value);

    void setOutputElements(ElementU8* first, ElementU8* second);

    virtual void execute();
    virtual void updateNotification(ElementBase* element);

    virtual bool sendNotification();

    virtual E_ControlType getControlType() {return E_ControlType_AirBleeding;}

//    virtual void addProtectionElement(ValidationElementBase* element);

    virtual bool onInitControl();
    virtual bool onMove2Standby(uint32_t delay);
    virtual bool onReset2On();
};

inline bool AirBleedingControl::onInitControl()
{
    endInitControl();
    return true;
}

inline bool AirBleedingControl::onMove2Standby(uint32_t delay)
{
    endMove2Standby();
    return true;
}

inline bool AirBleedingControl::onReset2On()
{
    endReset2On();
    return true;
}

#endif /* AIRBLEEDINGCONTROL_H_ */
