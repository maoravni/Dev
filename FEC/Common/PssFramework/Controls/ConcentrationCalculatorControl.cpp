/*
 * ConcentrationCalculatorControl.cpp
 *
 *  Created on: 12 Jul 2016
 *      Author: maora
 */

#include <Controls/ConcentrationCalculatorControl.h>
#include <PscServer/PscMasterServer.h>
#include <Persistency/ControlSerializers.h>

ConcentrationCalculatorControl::ConcentrationCalculatorControl()
{
    m_viscosity = NULL;
    m_temperature = NULL;
    m_calculatedOutput = NULL;
    m_concentration1 = 0;
    m_intercept1 = 0;
    m_slope1 = 0;
    m_concentration2 = 0;
    m_intercept2 = 0;
    m_slope2 = 0;

}

ConcentrationCalculatorControl::~ConcentrationCalculatorControl()
{
}

bool ConcentrationCalculatorControl::onInitControl()
{
    endInitControl();
    return true;
}

bool ConcentrationCalculatorControl::onMove2Standby(uint32_t delay)
{
    endMove2Standby();
    return true;
}

bool ConcentrationCalculatorControl::onReset2On()
{
    endReset2On();
    return true;
}

bool ConcentrationCalculatorControl::onMove2Error()
{
    endMove2Error();
    return true;
}

bool ConcentrationCalculatorControl::onStopOnEmr()
{
    endStopOnEmr();
    return true;
}

bool ConcentrationCalculatorControl::onRecoverFromEmr()
{
    endRecoverFromEmr();
    return true;
}

void ConcentrationCalculatorControl::updateNotification(ElementBase* element)
{
    if (element == m_viscosity || element == m_temperature)
    {
        float viscosity, temperature, result, vt1, vt2;

        viscosity = m_viscosity->getValueF();
        temperature = m_temperature->getValueF();

        vt1 = m_intercept1 + temperature*m_slope1;
        vt2 = m_intercept2 + temperature*m_slope2;

        result = (viscosity - vt2)/(vt1-vt2)*(m_concentration1 - m_concentration2) + m_concentration2;

        m_calculatedOutput->setValueValid(m_viscosity->isValid() && m_temperature->isValid());
        m_calculatedOutput->setValue(result);
    }
}

void ConcentrationCalculatorControl::execute()
{
}

bool ConcentrationCalculatorControl::sendNotification()
{
    // static instance for the master server.
    PscMessageStruct replyMessage;

    replyMessage.header.id.split.id = MSG_ControlStatusNotification;
    replyMessage.payload.pSSControlStatusNotificationMsg.pssId = getPssId();
    replyMessage.payload.pSSControlStatusNotificationMsg.dataType = E_ValueType_Float;
    replyMessage.payload.pSSControlStatusNotificationMsg.setPoint = m_calculatedOutput->getValueU32();
    replyMessage.payload.pSSControlStatusNotificationMsg.state = m_controlState;
    replyMessage.payload.pSSControlStatusNotificationMsg.exceptions = getControlExceptions();
    replyMessage.payload.pSSControlStatusNotificationMsg.cableId = Psc_ControllerId;
    replyMessage.header.length = sizeof(replyMessage.header)
            + sizeof(replyMessage.payload.pSSControlStatusNotificationMsg);

    PscMasterServer::getInstance().sendMessage(replyMessage);

    return true;
}

void ConcentrationCalculatorControl::setCoefficients(float c1, float i1, float s1, float c2, float i2, float s2)
{
    m_concentration1 = c1;
    m_concentration2 = c2;
    m_intercept1 = i1;
    m_intercept2 = i2;
    m_slope1 = s1;
    m_slope2 = s2;
}

void ConcentrationCalculatorControl::serialize(F_FILE* f)
{
    Serializer<ConcentrationCalculatorControl> s;
    s.serialize(f, *this);
}

ConcentrationCalculatorControl::ConcentrationCalculatorControl(F_FILE* f)
{
    Serializer<ConcentrationCalculatorControl> s;
    s.deserialize(f, *this);
    setViscosityElement(m_viscosity);
    setTemperatureElement(m_temperature);
}
