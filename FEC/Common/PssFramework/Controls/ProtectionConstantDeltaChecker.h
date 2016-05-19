/*
 * ProtectionConstantDeltaChecker.h
 *
 *  Created on: 3 Sep 2014
 *      Author: maora
 */

#ifndef PROTECTIONCONSTANTDELTACHECKER_H_
#define PROTECTIONCONSTANTDELTACHECKER_H_

#include <Controls/ProtectionCheckerBase.h>

class ProtectionConstantDeltaChecker: public ProtectionCheckerBase
{
private:
    ElementBase* m_referenceElement;
    RangeChecker<float> m_allowedRange;

public:
    ProtectionConstantDeltaChecker();
    virtual ~ProtectionConstantDeltaChecker();

    void setReferenceElement(ElementBase* element);
    void updateAllowedDelta(float low, float high);

    virtual void updateNotification(ElementBase* element);
    virtual bool requestValueChange(ElementBase* element) {return true;}
    virtual E_ProtectionCheckerType getProtectionType() {return E_ProtectionCheckerType_ConstantDelta;}
    virtual E_PSSErrors getErrorType(){return E_PSSErrors_DeviceExceedsDelta;}
    virtual E_PSSWarnings getWarningType(){return E_PSSWarnings_ControlExceedsLimits;}
    virtual E_DeviceProtectionState calcProtectionState(ElementBase* element);

    virtual void serialize(F_FILE* f);

    template <class T> friend class Serializer;
};

#endif /* PROTECTIONCONSTANTDELTACHECKER_H_ */
