/*
 * AdditionInfoErrorBitManager.cpp
 *
 *  Created on: 9 Aug 2016
 *      Author: maora
 */

#include <AdditionInfoErrorBitManager.h>
#include <logger.h>
#include <PscServer/PscMasterServer.h>

bool T_AdditionalInfoBitManager::hasErrors()
{
    T_AdditionalInfoErrorMapIterator it;
    for (it = m_errorBitsMap.begin(); it != m_errorBitsMap.end(); ++it)
    {
        if (it->second != 0)
            return true;
    }
    return false;
}

bool T_AdditionalInfoBitManager::hasWarnings()
{
    T_AdditionalInfoErrorMapIterator it;
    for (it = m_warningBitsMap.begin(); it != m_warningBitsMap.end(); ++it)
    {
        if (it->second != 0)
            return true;
    }
    return false;
}

AdditionInfoErrorBitManager::AdditionInfoErrorBitManager()
{
}

AdditionInfoErrorBitManager::~AdditionInfoErrorBitManager()
{
}

//uint32_t AdditionInfoErrorBitManager::raiseError(uint16_t secondaryPssId, E_PSSErrors mainError, bool errorState)
//{
//    T_AdditionalInfoBitManager* bitManager = &m_additionalInfoMap[secondaryPssId];
//
//    uint32_t *pCurrentError = &bitManager->m_errorBitsMap[0];
//    uint32_t lastError = *pCurrentError;
//
//    if (errorState)
//    {
//        (*pCurrentError) |= additionalError;
//        if (*pCurrentError != lastError)
//        {
//            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
//                    "PSSID {[PSSID:%d]} issued error %x caused by PSSID {[PSSID:%d]} with error %x", getPssId(),
//                    mainError, secondaryPssId, *pCurrentError);
//            PscMasterServer::getInstance().sendError(Psc_ControllerId, getPssId(), secondaryPssId, *pCurrentError);
//        }
//    }
//    else
//    {
//        // TODO: Add log message
//        (*pCurrentError) &= ~additionalError;
//        if (*pCurrentError != lastError)
//        {
//            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
//                    "PSSID {[PSSID:%d]} cleared error %x caused by PSSID {[PSSID:%d]} with error %x", getPssId(),
//                    mainError, secondaryPssId, *pCurrentError);
//            PscMasterServer::getInstance().sendError(Psc_ControllerId, getPssId(), secondaryPssId, *pCurrentError);
//        }
//    }
//    return *pCurrentError;
//}
//
//uint32_t AdditionInfoErrorBitManager::raiseWarning(uint16_t secondaryPssId, E_PSSWarnings mainWarning,
//        bool warningState)
//{
//    T_AdditionalInfoBitManager* bitManager = &m_additionalInfoMap[secondaryPssId];
//
//    uint32_t *pCurrentWarning = &bitManager->m_errorBitsMap[0];
//    uint32_t lastWarning = *pCurrentWarning;
//
//    if (warningState)
//    {
//        (*pCurrentWarning) |= additionalWarning;
//        if (*pCurrentWarning != lastWarning)
//        {
//            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
//                    "PSSID {[PSSID:%d]} issued warning %x caused by PSSID {[PSSID:%d]} with warning %x", getPssId(),
//                    mainWarning, secondaryPssId, *pCurrentWarning);
//            PscMasterServer::getInstance().sendWarning(Psc_ControllerId, getPssId(), secondaryPssId, *pCurrentWarning);
//        }
//    }
//    else
//    {
//        // TODO: Add log message
//        (*pCurrentWarning) &= ~additionalWarning;
//        if (*pCurrentWarning != lastWarning)
//        {
//            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
//                    "PSSID {[PSSID:%d]} cleared warning %x caused by PSSID {[PSSID:%d]} with warning %x", getPssId(),
//                    mainWarning, secondaryPssId, *pCurrentWarning);
//            PscMasterServer::getInstance().sendWarning(Psc_ControllerId, getPssId(), secondaryPssId, *pCurrentWarning);
//        }
//    }
//    return *pCurrentWarning;
//}

uint32_t AdditionInfoErrorBitManager::raiseError(uint16_t secondaryPssId, E_PSSErrors mainError,
        uint32_t additionalError, E_ValueType valueType, void* value, bool errorState)
{
    T_AdditionalInfoBitManager* bitManager = &m_additionalInfoMap[secondaryPssId];

    uint32_t *pCurrentError = &bitManager->m_errorBitsMap[additionalError];
    uint32_t lastError = *pCurrentError;

    if (errorState)
    {
        (*pCurrentError) |= additionalError;
        if (*pCurrentError != lastError)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                    "PSSID {[PSSID:%d]} issued error %x caused by PSSID {[PSSID:%d]} with error %x", getPssId(),
                    mainError, secondaryPssId, *pCurrentError);
            PscMasterServer::getInstance().sendError(Psc_ControllerId, getPssId(), secondaryPssId, *pCurrentError);
        }
    }
    else
    {
        // TODO: Add log message
        (*pCurrentError) &= ~additionalError;
        if (*pCurrentError != lastError)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                    "PSSID {[PSSID:%d]} cleared error %x caused by PSSID {[PSSID:%d]} with error %x", getPssId(),
                    mainError, secondaryPssId, *pCurrentError);
            PscMasterServer::getInstance().sendError(Psc_ControllerId, getPssId(), secondaryPssId, *pCurrentError);
        }
    }
    return *pCurrentError;
}

uint32_t AdditionInfoErrorBitManager::raiseWarning(uint16_t secondaryPssId, E_PSSWarnings mainWarning,
        uint32_t additionalWarning, E_ValueType valueType, void* value, bool warningState)
{
    T_AdditionalInfoBitManager* bitManager = &m_additionalInfoMap[secondaryPssId];

    uint32_t *pCurrentWarning = &bitManager->m_errorBitsMap[additionalWarning];
    uint32_t lastWarning = *pCurrentWarning;

    if (warningState)
    {
        (*pCurrentWarning) |= additionalWarning;
        if (*pCurrentWarning != lastWarning)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                    "PSSID {[PSSID:%d]} issued warning %x caused by PSSID {[PSSID:%d]} with warning %x", getPssId(),
                    mainWarning, secondaryPssId, *pCurrentWarning);
            PscMasterServer::getInstance().sendWarning(Psc_ControllerId, getPssId(), secondaryPssId, *pCurrentWarning);
        }
    }
    else
    {
        // TODO: Add log message
        (*pCurrentWarning) &= ~additionalWarning;
        if (*pCurrentWarning != lastWarning)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
                    "PSSID {[PSSID:%d]} cleared warning %x caused by PSSID {[PSSID:%d]} with warning %x", getPssId(),
                    mainWarning, secondaryPssId, *pCurrentWarning);
            PscMasterServer::getInstance().sendWarning(Psc_ControllerId, getPssId(), secondaryPssId, *pCurrentWarning);
        }
    }
    return *pCurrentWarning;
}

//uint32_t AdditionInfoErrorBitManager::setError(uint16_t secondaryPssId, E_PSSErrors mainError, uint32_t additionalError,
//        E_ValueType valueType, uint32_t value)
//{
//    T_AdditionalInfoBitManager* bitManager = &m_additionalInfoMap[secondaryPssId];
//
//    uint32_t *pCurrentError = &bitManager->m_errorBitsMap[additionalError];
//    uint32_t lastError = *pCurrentError;
//
//    (*pCurrentError) = additionalError;
//    if (*pCurrentError != 0)
//    {
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
//                "PSSID {[PSSID:%d]} issued error %x caused by PSSID {[PSSID:%d]} with error %x", getPssId(), mainError,
//                secondaryPssId, *pCurrentError);
//    }
//    else
//    {
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
//                "PSSID {[PSSID:%d]} cleared errors caused by PSSID {[PSSID:%d]} with error %x", getPssId(),
//                secondaryPssId, *pCurrentError);
//    }
//    PscMasterServer::getInstance().sendErrorWithInfo(Psc_ControllerId, getPssId(), secondaryPssId, *pCurrentError, additionalError, valueType, &value);
//    return *pCurrentError;
//}

//uint32_t AdditionInfoErrorBitManager::setWarning(uint16_t secondaryPssId, E_PSSWarnings mainWarning,
//        uint32_t additionalWarning, E_ValueType valueType, uint32_t value)
//{
//    T_AdditionalInfoBitManager* bitManager = &m_additionalInfoMap[secondaryPssId];
//
//    uint32_t *pCurrentWarning = &bitManager->m_errorBitsMap[additionalWarning];
//    uint32_t lastWarning = *pCurrentWarning;
//
//    (*pCurrentWarning) = additionalWarning;
//    if (*pCurrentWarning != 0)
//    {
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
//                "PSSID {[PSSID:%d]} issued warning %x caused by PSSID {[PSSID:%d]} with warning %x", getPssId(),
//                mainWarning, secondaryPssId, *pCurrentWarning);
//    }
//    else
//    {
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG,
//                "PSSID {[PSSID:%d]} cleared warnings caused by PSSID {[PSSID:%d]} with warning %x", getPssId(),
//                secondaryPssId, *pCurrentWarning);
//    }
//    PscMasterServer::getInstance().sendWarningWithInfo(Psc_ControllerId, getPssId(), secondaryPssId, *pCurrentWarning, additionalWarning, valueType, &dataValue);
//    return *pCurrentWarning;
//}

void AdditionInfoErrorBitManager::sendCurrentWarnings()
{
}

void AdditionInfoErrorBitManager::sendCurrentErrors()
{
}

