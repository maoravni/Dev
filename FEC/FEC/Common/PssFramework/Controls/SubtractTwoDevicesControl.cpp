/*
 * SubtractTwoDevicesControl.cpp
 *
 *  Created on: 6 Sep 2015
 *      Author: maora
 */

#include <Controls/SubtractTwoDevicesControl.h>
#include <PscServer/PscMasterServer.h>

SubtractTwoDevicesControl::SubtractTwoDevicesControl()
{
}

SubtractTwoDevicesControl::~SubtractTwoDevicesControl()
{
}

void SubtractTwoDevicesControl::execute()
{
    float input1 = m_input1->getValueF();
    float input2 = m_input2->getValueF();
    float result = input1 - input2;
    m_calculatedOutput->setValue(result);
}

AddTwoDevicesControl::AddTwoDevicesControl()
{
}

AddTwoDevicesControl::~AddTwoDevicesControl()
{
}

void AddTwoDevicesControl::execute()
{
    float input1 = m_input1->getValueF();
    float input2 = m_input2->getValueF();
    float result = input1 + input2;
    m_calculatedOutput->setValue(result);
}

MultiplyTwoDevicesControl::MultiplyTwoDevicesControl()
{
}

MultiplyTwoDevicesControl::~MultiplyTwoDevicesControl()
{
}

void MultiplyTwoDevicesControl::execute()
{
    float input1 = m_input1->getValueF();
    float input2 = m_input2->getValueF();
    float result = input1 * input2;
    m_calculatedOutput->setValue(result);
}

DivideTwoDevicesControl::DivideTwoDevicesControl()
{
}

DivideTwoDevicesControl::~DivideTwoDevicesControl()
{
}

void DivideTwoDevicesControl::execute()
{
    float input1 = m_input1->getValueF();
    float input2 = m_input2->getValueF();
    float result = input1 / input2;
    m_calculatedOutput->setValue(result);
}

MaxTwoDevicesControl::MaxTwoDevicesControl()
{
}

MaxTwoDevicesControl::~MaxTwoDevicesControl()
{
}

void MaxTwoDevicesControl::execute()
{
    float input1 = m_input1->getValueF();
    float input2 = m_input2->getValueF();
    float result = (input1 > input2) ? input1 : input2;
    m_calculatedOutput->setValue(result);
}

MinTwoDevicesControl::MinTwoDevicesControl()
{
}

MinTwoDevicesControl::~MinTwoDevicesControl()
{
}

void MinTwoDevicesControl::execute()
{
    float input1 = m_input1->getValueF();
    float input2 = m_input2->getValueF();
    float result = (input1 < input2) ? input1 : input2;
    m_calculatedOutput->setValue(result);
}
