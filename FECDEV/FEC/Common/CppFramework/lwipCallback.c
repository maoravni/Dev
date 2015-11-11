/*
 * lwipCallback.c
 *
 *  Created on: 21 Oct 2014
 *      Author: maora
 */

#include "lwipCallback.h"

int g_numUsedNetconnsInRegistry = 0;
struct T_NetconnRegistryItem g_netconnRegistry[MEMP_NUM_NETCONN];

void lwipCallbackHandler(struct netconn* pNetConn, enum netconn_evt event, u16_t len)
{
    int i;
    int yieldRequired;
    switch (event)
    {
    case NETCONN_EVT_RCVPLUS:
//        if (len == 0)
//            break;
        for (i = 0; i < g_numUsedNetconnsInRegistry; ++i)
        {
            if (g_netconnRegistry[i].pNetconn == pNetConn)
            {
                struct T_CallbackQueueItem queueItem;
                if (len == 0)
                {
                    if (pNetConn->state == NETCONN_NONE)
                        queueItem.command = E_QueueCommandCommand_Disconnect;
                    else
                        break;
                }
                else
                {
                    queueItem.command = E_QueueCommandCommand_Receive;
                }
                queueItem.pNetconn = pNetConn;
//                printf("lwipcallback insert to callback queue. %d\n", uxQueueMessagesWaiting(g_netconnRegistry[i].queueHandle)+1);
//                while (xQueueSendFromISR(g_netconnRegistry[i].queueHandle, &queueItem, &yieldRequired) == pdPASS);
//                if (yieldRequired)
//                    taskYIELD();
                while (xQueueSendToBack(g_netconnRegistry[i].queueHandle, &queueItem, 5) != pdPASS)
                {
                    vTaskDelay(5);
//                    printf("Callback Send Queue Size %d\n", uxQueueMessagesWaiting(g_netconnRegistry[i].queueHandle));
//                    int j = 0;
                }
            }
        }
        break;
//    case NETCONN_EVT_SENDPLUS:
//        if (len == 0)
//        {
//            for (i = 0; i < g_numUsedNetconnsInRegistry; ++i)
//            {
//                if (g_netconnRegistry[i].pNetconn == pNetConn)
//                {
//                    struct T_CallbackQueueItem queueItem;
//                    queueItem.command = E_QueueCommandCommand_Disconnect;
//                    queueItem.pNetconn = pNetConn;
//                    xQueueSendToBack(g_netconnRegistry[i].queueHandle, &queueItem, 200);
//                }
//            }
//        }
//        break;
//    }
    }
}

void registerNetconn(struct netconn* pNetConn, xQueueHandle queueHandle)
{
    g_netconnRegistry[g_numUsedNetconnsInRegistry].pNetconn = pNetConn;
    g_netconnRegistry[g_numUsedNetconnsInRegistry].queueHandle = queueHandle;
    ++g_numUsedNetconnsInRegistry;

}

void unregisterNetconn(struct netconn* pNetConn)
{
    int i = 0;

    // find the correct netconn
    for (i = 0; i < g_numUsedNetconnsInRegistry; ++i)
    {
        if (pNetConn == g_netconnRegistry[i].pNetconn)
        {
            break;
        }
    }

    // found the netconn. Now copy all data back one place:
    for (; i < g_numUsedNetconnsInRegistry; ++i)
    {
        g_netconnRegistry[i].pNetconn = g_netconnRegistry[i+1].pNetconn;
        g_netconnRegistry[i].queueHandle = g_netconnRegistry[i+1].queueHandle;
    }

    --g_numUsedNetconnsInRegistry;
}
