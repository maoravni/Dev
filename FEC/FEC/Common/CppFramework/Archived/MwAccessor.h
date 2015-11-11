/*
 * MwAccessor.h
 *
 *  Created on: 11 Mar 2013
 *      Author: maora
 */

#ifndef MWACCESSOR_H_
#define MWACCESSOR_H_

#include <CfgDB.h>

class MwAccessor
{
    U32 m_id;
public:
    MwAccessor(U32 id);
    MwAccessor();
    virtual ~MwAccessor();

    void setId(U32 id);

    U32 cfgGetParam(U32& value)
    {
        U32 status;
        status = CfgGetParam(0, m_id, &value, sizeof(value), CfgDB_GET_FROM_DB);
        return status;
    }

    U32 cfgPutParam(U32 value)
    {
        U32 status = OK;
        status = CfgPutParam(0, m_id, &value, (CfgDB_SET_FLAG) CfgDB_SET_TO_DB);
        return status;
    }

    U32 cfgGetParam()
    {
        U32 value;
        CfgGetParam(0, m_id, &value, sizeof(value), CfgDB_GET_FROM_DB);
        return value;
    }

    U32 cfgGetFromRam()
    {
        U32 value;
        CfgGetParam(0, m_id, &value, sizeof(value), CfgDB_GET_DIRECTLY_FROM_RAM);
        return value;
    }

    U32 cfgGetFromSource()
    {
        U32 value;
        CfgGetParam(0, m_id, &value, sizeof(value), CfgDB_GET_DIRECTLY_FROM_SOURCE);
        return value;
    }

    //assignment operator
    U32 operator=(U32& value)
    {
        U32 status = OK;
        status = CfgPutParam(0, m_id, &value, (CfgDB_SET_FLAG) CfgDB_SET_TO_DB);
        return status;
    }

    //casting operators
    operator U32()
    {
        U32 value;
        CfgGetParam(0, m_id, &value, sizeof(value), CfgDB_GET_FROM_DB);
        return value;
    }

};

#endif /* MWACCESSOR_H_ */
