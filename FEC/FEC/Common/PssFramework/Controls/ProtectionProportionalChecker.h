/*
 * ProtectionProportionalChecker.h
 *
 *  Created on: 3 Sep 2014
 *      Author: maora
 */

#ifndef ProtectionProportionalChecker_H_
#define ProtectionProportionalChecker_H_

#include <Controls/ProtectionCheckerBase.h>

class ProtectionProportionalChecker: public ProtectionCheckerBase
{
private:
    ElementBase* m_referenceElement;
    RangeChecker<float> m_allowedRange;
    float m_gain;
    float m_offset;

public:
    ProtectionProportionalChecker();
    virtual ~ProtectionProportionalChecker();

    void setReferenceElement(ElementBase* element);
    void updateParameters(float gain, float offset, float low, float high);

    virtual void updateNotification(ElementBase* element);
    virtual bool requestValueChange(ElementBase* element) {return true;}
    virtual E_ProtectionCheckerType getProtectionType() {return E_ProtectionCheckerType_Proportional;}
    virtual E_PSSErrors getErrorType(){return E_PSSErrors_DeviceExceedsSoftLimits;}
    virtual E_PSSWarnings getWarningType(){return E_PSSWarnings_ControlExceedsLimits;}
    virtual E_DeviceProtectionState calcProtectionState(ElementBase* element);

};

#endif /* ProtectionProportionalChecker_H_ */
