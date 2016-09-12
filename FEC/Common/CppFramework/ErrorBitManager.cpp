/*
 * ErrorBitManager.cpp
 *
 *  Created on: 15 Feb 2015
 *      Author: maora
 */

#include <ErrorBitManager.h>
#include <PscSubsystem.h>
#include <PscServer/PscMasterServer.h>

ErrorBitManager::ErrorBitManager()
{
    m_pssId = 0;
}

ErrorBitManager::~ErrorBitManager()
{
}

uint32_t ErrorBitManager::raiseErrorWithInfo(uint16_t secondaryPssId, E_PSSErrors error, bool errorState,
        uint8_t dataType, const void *dataValue, uint32_t additionalError)
{
    T_BitContainer* bitContainer;

    bitContainer = getBitContainer(secondaryPssId);
    uint32_t lastError = bitContainer->m_errorBits;

    assert(m_pssId != 0);
//    assert(m_secondaryPssId != 0);

    if (errorState)
    {
        // TODO: Add log message
        bitContainer->m_errorBits |= error;
        if (bitContainer->m_errorBits != lastError)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSID {[PSSID:%d]} issued error %x caused by PSSID {[PSSID:%d]}",
                    getPssId(), error, secondaryPssId);
            PscMasterServer::getInstance().sendError(Psc_ControllerId, getPssId(), secondaryPssId,
                    bitContainer->m_errorBits);
        }
    }
    else
    {
        // TODO: Add log message
        bitContainer->m_errorBits &= ~error;
        if (bitContainer->m_errorBits != lastError)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSID {[PSSID:%d]} cleared error %x caused by PSSID {[PSSID:%d]}",
                    getPssId(), error, secondaryPssId);
            PscMasterServer::getInstance().sendError(Psc_ControllerId, getPssId(), secondaryPssId,
                    bitContainer->m_errorBits);
        }
    }
    return bitContainer->m_errorBits;
}

uint32_t ErrorBitManager::raiseErrorSimple(uint16_t secondaryPssId, E_PSSErrors error, bool errorState)
{
    T_BitContainer* bitContainer;

    bitContainer = getBitContainer(secondaryPssId);
    uint32_t lastError = bitContainer->m_errorBits;

    assert(m_pssId != 0);
//    assert(m_secondaryPssId != 0);

    if (errorState)
    {
        // TODO: Add log message
        bitContainer->m_errorBits |= error;
        if (bitContainer->m_errorBits != lastError)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSID {[PSSID:%d]} issued error %x caused by PSSID {[PSSID:%d]}",
                    getPssId(), error, secondaryPssId);
            PscMasterServer::getInstance().sendError(Psc_ControllerId, getPssId(), secondaryPssId,
                    bitContainer->m_errorBits);
        }
    }
    else
    {
        // TODO: Add log message
        bitContainer->m_errorBits &= ~error;
        if (bitContainer->m_errorBits != lastError)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSID {[PSSID:%d]} cleared error %x caused by PSSID {[PSSID:%d]}",
                    getPssId(), error, secondaryPssId);
            PscMasterServer::getInstance().sendError(Psc_ControllerId, getPssId(), secondaryPssId,
                    bitContainer->m_errorBits);
        }
    }
    return bitContainer->m_errorBits;
}

uint32_t ErrorBitManager::raiseWarningSimple(uint16_t secondaryPssId, E_PSSWarnings warning, bool warningState)
{
    T_BitContainer* bitContainer;

    bitContainer = getBitContainer(secondaryPssId);
    uint32_t lastWarning = bitContainer->m_warningBits;

    assert(m_pssId != 0xffff);
//    assert(m_secondaryPssId != 0xffff);

    if (warningState)
    {
        // TODO: Add log message
        bitContainer->m_warningBits |= warning;
        if (bitContainer->m_warningBits != lastWarning)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSID {[PSSID:%d]} issued warning %x caused by PSSID {[PSSID:%d]}",
                    getPssId(), warning, secondaryPssId);
            PscMasterServer::getInstance().sendWarning(Psc_ControllerId, getPssId(), secondaryPssId,
                    bitContainer->m_warningBits);
        }
    }
    else
    {
        // TODO: Add log message
        bitContainer->m_warningBits &= ~warning;
        if (bitContainer->m_warningBits != lastWarning)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSID {[PSSID:%d]} cleared warning %x caused by PSSID {[PSSID:%d]}",
                    getPssId(), warning, secondaryPssId);
            PscMasterServer::getInstance().sendWarning(Psc_ControllerId, getPssId(), secondaryPssId,
                    bitContainer->m_warningBits);
        }
    }
    return bitContainer->m_warningBits;
}

T_BitContainer* ErrorBitManager::getBitContainer(uint16_t secondary)
{
    return &m_bitContainerMap[secondary];
}

void ErrorBitManager::sendCurrentErrors()
{
    for (T_BitContainerMapIterator it = m_bitContainerMap.begin(); it != m_bitContainerMap.end(); ++it)
    {
        PscMasterServer::getInstance().sendError(Psc_ControllerId, getPssId(), (*it).first, (*it).second.m_errorBits);
    }
}

uint32_t ErrorBitManager::raiseWarningWithInfo(uint16_t secondaryPssId, E_PSSWarnings warning, bool warningState,
        uint8_t dataType, const void *dataValue, uint32_t additionalError)
{
    T_BitContainer* bitContainer;

    bitContainer = getBitContainer(secondaryPssId);
    uint32_t lastWarning = bitContainer->m_warningBits;

    assert(m_pssId != 0xffff);
//    assert(m_secondaryPssId != 0xffff);

    if (warningState)
    {
        // TODO: Add log message
        bitContainer->m_warningBits |= warning;
        if (bitContainer->m_warningBits != lastWarning)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSID {[PSSID:%d]} issued warning %x caused by PSSID {[PSSID:%d]}",
                    getPssId(), warning, secondaryPssId);
            PscMasterServer::getInstance().sendWarning(Psc_ControllerId, getPssId(), secondaryPssId,
                    bitContainer->m_warningBits);
        }
    }
    else
    {
        // TODO: Add log message
        bitContainer->m_warningBits &= ~warning;
        if (bitContainer->m_warningBits != lastWarning)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "PSSID {[PSSID:%d]} cleared warning %x caused by PSSID {[PSSID:%d]}",
                    getPssId(), warning, secondaryPssId);
            PscMasterServer::getInstance().sendWarning(Psc_ControllerId, getPssId(), secondaryPssId,
                    bitContainer->m_warningBits);
        }
    }
    return bitContainer->m_warningBits;
}

void ErrorBitManager::sendCurrentWarnings()
{
    for (T_BitContainerMapIterator it = m_bitContainerMap.begin(); it != m_bitContainerMap.end(); ++it)
    {
        PscMasterServer::getInstance().sendWarning(Psc_ControllerId, getPssId(), (*it).first,
                (*it).second.m_warningBits);
    }
}
