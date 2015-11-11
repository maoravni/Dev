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
//    m_pBitContainer = NULL;
//    setSecondaryPssId(0);
//    m_errorBits = 0;
//    m_warningBits = 0;
}

ErrorBitManager::~ErrorBitManager()
{
}

//uint32_t ErrorBitManager::raiseError(E_PSSErrors error, bool errorState, int dummy)
//{
//    return raiseError(m_secondaryPssId, error, errorState, dummy);
//}
//
//uint32_t ErrorBitManager::raiseWarning(E_PSSWarnings warning, bool warningState, int dummy)
//{
//    return raiseWarning(m_secondaryPssId, warning, warningState, dummy);
//}

bool ErrorBitManager::isErrorRaised(E_PSSErrors error)
{
    uint32_t errorMask = 0;
    for (T_BitContainerMapIterator it = m_bitContainerMap.begin(); it != m_bitContainerMap.end(); ++it)
    {
        uint32_t temp = it->second.m_errorBits;
        errorMask |= temp;
    }
    return ((errorMask & error) != 0);
//    return ((m_bitContainer.m_errorBits & error) != 0);
}

uint32_t ErrorBitManager::raiseError(uint16_t secondaryPssId, E_PSSErrors error, bool errorState)
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

uint32_t ErrorBitManager::raiseWarning(uint16_t secondaryPssId, E_PSSWarnings warning, bool warningState)
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

bool ErrorBitManager::isWarningRaised(E_PSSWarnings warning)
{
    uint32_t errorMask = 0;
    for (T_BitContainerMapIterator it = m_bitContainerMap.begin(); it != m_bitContainerMap.end(); ++it)
    {
        uint32_t temp = it->second.m_warningBits;
        errorMask |= temp;
    }
    return ((errorMask & warning) != 0);
//    return ((m_bitContainer.m_warningBits & warning) != 0);
}

//void ErrorBitManager::setSecondaryPssId(uint16_t secondaryPssId)
//{
//    m_secondaryPssId = secondaryPssId;
////    m_bitContainerMap.clear();
////    m_pBitContainer = &(m_bitContainerMap[m_secondaryPssId]);
//}

T_BitContainer* ErrorBitManager::getBitContainer(uint16_t secondary)
{
    return &m_bitContainerMap[secondary];
//    T_BitContainer* bitContainer;
//    return &m_bitContainer;
//     if (secondary == m_secondaryPssId)
//         bitContainer = m_pBitContainer;
//     else
//         bitContainer = &(m_bitContainerMap[secondary]);
//     if (bitContainer == NULL)
//         printf("bitContainer is NULL!!!!");
//     return bitContainer;
}

void ErrorBitManager::sendCurrentErrors()
{
    for (T_BitContainerMapIterator it = m_bitContainerMap.begin(); it != m_bitContainerMap.end(); ++it)
    {
        PscMasterServer::getInstance().sendError(Psc_ControllerId, getPssId(), (*it).first, (*it).second.m_errorBits);
    }
}

void ErrorBitManager::sendCurrentWarnings()
{
    for (T_BitContainerMapIterator it = m_bitContainerMap.begin(); it != m_bitContainerMap.end(); ++it)
    {
        PscMasterServer::getInstance().sendWarning(Psc_ControllerId, getPssId(), (*it).first, (*it).second.m_warningBits);
    }
}
