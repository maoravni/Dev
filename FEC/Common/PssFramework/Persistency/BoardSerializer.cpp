/*
 * BoardSerializer.cpp
 *
 *  Created on: 24 Jul 2016
 *      Author: maora
 */

#include "BoardSerializer.h"
#include <string.h>
#include <PscServer/PscMessageHandler.h>
#include <board_setup.h>

FecBoardConfiguration::FecBoardConfiguration() :
        m_boardType(E_BoardType_Fec), m_boardCableId(0), m_numOfPsocs(0), m_uartBaudRate(0), m_loggerIp(0), m_lastConfigurationHash(
                0), m_lastConfigurationTimestamp(0)
{
    memset(m_psocsCableId, 0, sizeof(m_psocsCableId));
}

FecBoardConfiguration::~FecBoardConfiguration()
{
}

void FecBoardConfiguration::getParametersFromBoard()
{
    m_boardType = (E_BoardType) Psc_GetBoardType();
    m_boardCableId = Psc_GetCableId();
    switch (m_boardType)
    {
    case E_BoardType_Fec2:
        m_numOfPsocs = 8;
        break;
    case E_BoardType_Fec3:
        m_numOfPsocs = 10;
        break;
    }
    for (int i = 0; i < m_numOfPsocs; ++i)
    {
        uint8_t cable1, cable2, cable3, errorBits;
        PscMessageHandler::getInstance()->getPsocManager()->getPsocCableID(i, cable1, cable2, cable3, errorBits);
        if (cable1 == cable2 && cable2 == cable3)
            m_psocsCableId[i] = cable1;
        else
            m_psocsCableId[i] = 0;
    }
    m_uartBaudRate = PscMessageHandler::getInstance()->m_usartBaudRate;
    m_loggerIp = CLogger::getInstance().getOutputUdpIpAddress();
    m_lastConfigurationHash = PscMessageHandler::getInstance()->m_lastConfigurationHash;
    m_lastConfigurationTimestamp = PscMessageHandler::getInstance()->m_lastConfigurationTimestamp;
}

void FecBoardConfiguration::setParametersToBoard()
{
    // verify that the board ID, type, and psoc Cable ID's are as expected.
    if (m_boardType != (E_BoardType) Psc_GetBoardType())
        throw("Wrong Board Type");

    if (m_boardCableId != Psc_GetCableId())
        throw("Wrong Cable ID");

    for (int i = 0; i < m_numOfPsocs; ++i)
    {
        if (m_psocsCableId[i] != 0)
        {
        int handlerIndex = PscMessageHandler::getInstance()->getPsocManager()->getPsocIndexByCableId(m_psocsCableId[i]);
        if (handlerIndex == -1)
            throw("Wrong PSoC Cable ID");
        }
    }
    ChangeUsartBaudrate(USART3, m_uartBaudRate);
    CLogger::getInstance().setOutputUdpIpAddress(m_loggerIp);
    PscMessageHandler::getInstance()->m_lastConfigurationHash = m_lastConfigurationHash;
    PscMessageHandler::getInstance()->m_lastConfigurationTimestamp = m_lastConfigurationTimestamp;
}

bool FecBoardConfiguration::compareToCurrentParameters(FecBoardConfiguration& f)
{
    // verify that the board ID, type, and psoc Cable ID's are as expected.
    if (m_boardType != f.m_boardType)
        throw("Wrong Board Type");

    if (m_boardCableId != f.m_boardCableId)
        throw("Wrong Cable ID");

    if (m_numOfPsocs != f.m_numOfPsocs)
        throw("Wrong Number of PSoCs");

    for (int i = 0; i < m_numOfPsocs; ++i)
    {
        if (m_psocsCableId[i] != 0)
        {
            int j;
            for (j = 0; j < f.m_numOfPsocs; ++j)
                if (m_psocsCableId[i] == f.m_psocsCableId[j])
                    break;
            if (j >= m_numOfPsocs)
                throw("Wrong PSoC Cable ID");
        }
    }

    return true;
}

void Serializer<FecBoardConfiguration>::serialize(F_FILE* f, FecBoardConfiguration& e)
{
    storeStartPosition(f);
    serializeClassType(f);
    serializeVersion(f);

    M_FWRITE_VARIABLE(e, f);

    updateRecordSize(f);
}

void Serializer<FecBoardConfiguration>::deserialize(F_FILE* f, FecBoardConfiguration& e)
{
    deserializeRecordSize(f);
    deserializeClassType(f);
    deserializeVersion(f);

    M_FREAD_VARIABLE(e, f);

}

