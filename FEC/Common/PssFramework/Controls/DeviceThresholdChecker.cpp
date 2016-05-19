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
