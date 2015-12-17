/*
 * AirBleedingControl.cpp
 *
 *  Created on: 9 Jul 2013
 *      Author: maora
 */

#include "AirBleedingControl.h"

AirBleedingControl::AirBleedingControl()
{

}

AirBleedingControl::~AirBleedingControl()
{
}

void AirBleedingControl::setOutputValue(int value)
{
    *m_outputs[0] = (uint8_t)((value & 0x1) != 0);
    *m_outputs[1] = (uint8_t)((value & 0x2) != 0);
}

void AirBleedingControl::setOutputElements(ElementU8* first, ElementU8* second)
{
    m_outputs[0] = first;
    m_outputs[1] = second;
}

void AirBleedingControl::execute()
{
}

void AirBleedingControl::updateNotification(ElementBase* element)
{
}

bool AirBleedingControl::sendNotification()
{
	return true;
}

//void AirBleedingControl::addProtectionElement(ValidationElementBase* element)
//{
//}
