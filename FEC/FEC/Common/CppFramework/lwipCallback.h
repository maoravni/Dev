/*
 * lwipCallback.h
 *
 *  Created on: 21 Oct 2014
 *      Author: maora
 */

#ifndef LWIPCALLBACK_H_
#define LWIPCALLBACK_H_

#include "arch/cc.h"
#include "lwip/netbuf.h"
#include "lwip/mem.h"
#include "lwip/api.h"
#include <PscServer/PscMessageStructs.h>

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

enum E_QueueCommandCommand
{
    E_QueueCommandCommand_Send,
    E_QueueCommandCommand_Receive,
    E_QueueCommandCommand_Disconnect
};

struct T_TcpQueueItem
{
    char data[sizeof(struct PscMessageStruct)];
    u16_t length;
};

union T_QueueItemPayload
{
    struct netbuf* pNetBuf;
    struct T_TcpQueueItem tcpQueueItem;
};

struct T_CallbackQueueItem
{
    enum E_QueueCommandCommand command;
    struct netconn* pNetconn;
    union T_QueueItemPayload payload;
};

struct T_NetconnRegistryItem
{
    struct netconn* pNetconn;
    xQueueHandle queueHandle;
};

void lwipCallbackHandler(struct netconn * pNetConn, enum netconn_evt, u16_t len);
void registerNetconn(struct netconn* pNetConn, xQueueHandle queueHandle);
void unregisterNetconn(struct netconn* pNetConn);

#ifdef __cplusplus
}
#endif //__cplusplus


#endif /* LWIPCALLBACK_H_ */
