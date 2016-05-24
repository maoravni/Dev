/*
 * DeviceThresholdChecker.cpp
 *
 *  Created on: 10 May 2015
 *      Author: maora
 */

#include <Controls/DeviceThresholdChecker.h>
#include <Persistency/ControlSerializers.h>

//DeviceThresholdChecker::DeviceThresholdChecker()
//{
//    // TODO Auto-generated constructor stub
//
//}

//DeviceThresholdChecker::~DeviceThresholdChecker()
//{
//    // TODO Auto-generated destructor stub
//}

void DeviceThresholdChecker::serialize(F_FILE* f)
{
    Serializer<DeviceThresholdChecker> s;
    s.serialize(f, *this);
}

DeviceThresholdChecker::DeviceThresholdChecker(F_FILE* f) :
        m_element(NULL), m_thresholdValue(0), m_greaterThan(false), m_dependencyCheckType(
                E_DependencyCheckType_DeactivateAndActivate), m_previousCheckResult(
                E_DependencyCheckResult_Unchecked), m_deactivateOnChange(false), m_lockChange(false)
{
    Serializer<DeviceThresholdChecker> s;
    s.deserialize(f, *this);
}
