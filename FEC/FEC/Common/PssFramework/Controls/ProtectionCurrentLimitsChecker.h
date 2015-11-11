/*
 * ProtectionCurrentLimitsChecker.h
 *
 *  Created on: 6 Aug 2015
 *      Author: maora
 */

#ifndef PROTECTIONCURRENTLIMITSCHECKER_H_
#define PROTECTIONCURRENTLIMITSCHECKER_H_

#include <Controls/ProtectionCheckerBase.h>

class ProtectionCurrentLimitsChecker: public ProtectionCheckerBase
{
private:
    RangeChecker<float> m_allowedRangeWarning;
    RangeChecker<float> m_allowedRangeError;

public:
    ProtectionCurrentLimitsChecker();
    virtual ~ProtectionCurrentLimitsChecker();

    void updateLimits(float lowWarning, float lowError, float high);

    virtual void updateNotification(ElementBase* element);
    virtual bool requestValueChange(ElementBase* element) {return true;}
    virtual E_ProtectionCheckerType getProtectionType() {return E_ProtectionCheckerType_CurrentLimits;}
    virtual E_PSSErrors getErrorType(){return E_PSSErrors_CurrentLoopAboveLimit;}
    virtual E_PSSWarnings getWarningType(){return E_PSSWarnings_CurrentLoopBelowLimit;}
    virtual E_DeviceProtectionState calcProtectionState(ElementBase* element);

};

#endif /* PROTECTIONCURRENTLIMITSCHECKER_H_ */
