/*
 * MiddlewareRegisterMonitor.cpp
 *
 *  Created on: 27 Dec 2012
 *      Author: maora
 */

#include "MiddlewareRegisterMonitor.h"
#include "cfg_db_if.h"
#include "CfgDB.h"
#include <TmcMasterServer.h>

extern CFG_DB_IF_ENTRY_ST cfg_db_if_data[];
extern uint32_t num_of_entries;

MiddlewareRegisterMonitor::MiddlewareRegisterMonitor()
{
    m_enabled = false;
}

MiddlewareRegisterMonitor::~MiddlewareRegisterMonitor()
{
}

void MiddlewareRegisterMonitor::sendAllRegisters()
{
    int i;

    U32 value;
    U32 status;
    for (i = 0; i < num_of_entries; ++i)
    {
      if (cfg_db_if_data[i].mb_reg == 2101)
        int j = 0;
        status = cfgGetParam(cfg_db_if_data[i].instance, cfg_db_if_data[i].db_id, cfg_db_if_data[i].db_id_inx, value);
        if (status == OK)
        {
            sendValue(cfg_db_if_data[i].mb_reg, value);
        }
    }
}

void MiddlewareRegisterMonitor::setRegister(unsigned short id, U32 value)
{
    int i;
    for (i = 0; i < num_of_entries; ++i)
    {
        if (cfg_db_if_data[i].mb_reg == id)
        {
            cfgPutParam(cfg_db_if_data[i].instance, cfg_db_if_data[i].db_id, cfg_db_if_data[i].db_id_inx, value);
            break;
        }
    }
}

U32 MiddlewareRegisterMonitor::cfgGetParam(U32 instance, U32 id, U32 index, U32& value)
{
    U32 status;
    status = CfgGetParam(instance, CfgDB_ID_AddIndex((U32)id, (U32)index), &value, sizeof(value), CfgDB_GET_FROM_DB);
    return status;
}

U32 MiddlewareRegisterMonitor::cfgPutParam(U32 instance, U32 id, U32 index, U32 value)
{
    U32 status;
    status = CfgPutParam(instance, CfgDB_ID_AddIndex((U32)id, (U32)index), &value, (CfgDB_SET_FLAG) CfgDB_SET_TO_DB);
    return status;
}

void MiddlewareRegisterMonitor::sendValue(unsigned short registerID, U32 value)
{
// static instance for the master server.
    static TmcMasterServer &masterServerInstance = TmcMasterServer::getInstance();
    TmcMessageStruct replyMessage;
    PSSRegistersNotificationMsg &payload = replyMessage.payload.pSSRegistersNotificationMsg;

    replyMessage.header.id.split.id = MSG_RegistersNotification;
    payload.registerID = registerID;
    payload.value = value;
    replyMessage.header.length = sizeof(replyMessage.header) + sizeof(payload);

    masterServerInstance.sendMessage(replyMessage);
}

void MiddlewareRegisterMonitor::reset()
{
    m_enabled = false;
}
