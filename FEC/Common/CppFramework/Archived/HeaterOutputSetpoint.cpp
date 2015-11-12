/*
 *
 * HeaterOutputSetpoint.cpp
 *  Created on: 7 May 2013
 *      Author: maora
 */

#include "HeaterOutputSetpoint.h"

HeaterOutputSetpoint::HeaterOutputSetpoint()
{

}

HeaterOutputSetpoint::~HeaterOutputSetpoint()
{
}

void HeaterOutputSetpoint::sendZoneStatus()
{
    sendMonitorReply(E_DataType_Power, m_lastPower);
    sendMonitorReply(E_DataType_SetPoint, cfgGetParam(m_mwSetPoint));

    sendMonitorReply(E_DataType_Temperature, m_lastTemperatureRead);

    sendMonitorReply(E_DataType_Status, getStatusStruct());
    //sendMonitorReply(E_DataType_Status | E_DataType_Power | E_DataType_Temperature, (char*)&monitorReply, sizeof(T_HeaterMonitorReplyStruct));
}

void HeaterOutputSetpoint::setOutputPower(int power)
{
    m_lastPower = power;
    m_outputDevice->configValues(power, -10, -5, +5, +10, 0);
}
