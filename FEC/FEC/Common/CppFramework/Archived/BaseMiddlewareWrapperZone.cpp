/*
 * BaseMiddlewareWrapperZone.cpp
 *
 *  Created on: 17 Dec 2012
 *      Author: maora
 */

#include "BaseMiddlewareWrapperZone.h"
#include "CfgDB.h"
#include <TmcMasterServer.h>

BaseMiddlewareWrapperZone::BaseMiddlewareWrapperZone()
{
}

BaseMiddlewareWrapperZone::~BaseMiddlewareWrapperZone()
{
}

MiddlewareAccessor::MiddlewareAccessor()
{
//    m_mwInstance = 0;
//    m_mwIndex = 0;
}

//void MiddlewareAccessor::setMiddlewareIds(int instance, int index)
//{
//    m_mwInstance = instance;
//    m_mwIndex = index;
//}

U32 MiddlewareAccessor::cfgGetParam(U32 id, U32& value)
{
    U32 status;
    status = CfgGetParam(0, id, &value, sizeof(value), CfgDB_GET_FROM_DB);
    return status;
}

U32 MiddlewareAccessor::cfgPutParam(U32 id, U32 value)
{
    U32 status = OK;
    status = CfgPutParam(0, id, &value, (CfgDB_SET_FLAG)CfgDB_SET_TO_DB);
    return status;
}

U32 MiddlewareAccessor::cfgGetParam(U32 id)
{
    U32 value;
    CfgGetParam(0, id, &value, sizeof(value), CfgDB_GET_FROM_DB);
    return value;
}

U32 MiddlewareAccessor::cfgGetFromRam(U32 id)
{
    U32 value;
    CfgGetParam(0, id, &value, sizeof(value), CfgDB_GET_DIRECTLY_FROM_RAM);
    return value;
}

U32 MiddlewareAccessor::cfgGetFromSource(U32 id)
{
    U32 value;
    CfgGetParam(0, id, &value, sizeof(value), CfgDB_GET_DIRECTLY_FROM_SOURCE);
    return value;
}

