/*
 * MwAccessor.cpp
 *
 *  Created on: 11 Mar 2013
 *      Author: maora
 */

#include "MwAccessor.h"

MwAccessor::MwAccessor(U32 id)
{
    m_id = id;
}

MwAccessor::MwAccessor()
{
    m_id = 0;
}

MwAccessor::~MwAccessor()
{
}

/*
U32 MwAccessor::cfgGetParam(U32 id, U32& value)
{
    U32 status;
    status = CfgGetParam(m_mwInstance, id, &value, sizeof(value), CfgDB_GET_FROM_DB);
    return status;
}

U32 MwAccessor::cfgPutParam(U32 id, U32 value)
{
    U32 status = OK;
    status = CfgPutParam(m_mwInstance, id, &value, (CfgDB_SET_FLAG)CfgDB_SET_TO_DB);
    return status;
}

U32 MwAccessor::cfgGetParam(U32 id)
{
    U32 value;
    CfgGetParam(m_mwInstance, id, &value, sizeof(value), CfgDB_GET_FROM_DB);
    return value;
}

U32 MwAccessor::cfgGetFromRam(U32 id)
{
    U32 value;
    CfgGetParam(0, id, &value, sizeof(value), CfgDB_GET_DIRECTLY_FROM_RAM);
    return value;
}

U32 MwAccessor::cfgGetFromSource(U32 id)
{
    U32 value;
    CfgGetParam(0, id, &value, sizeof(value), CfgDB_GET_DIRECTLY_FROM_SOURCE);
    return value;
}
 */

void MwAccessor::setId(U32 id)
{
    m_id = id;
}
