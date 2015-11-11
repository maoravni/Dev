/*
 * ModbusTcp.h
 *
 *  Created on: 23 Jan 2013
 *      Author: maora
 */

#ifndef MODBUSTCP_H_
#define MODBUSTCP_H_

#include <AManagedTask.h>
#include <CTcpConnector.h>
#include <ll_typedef.h>
#include <rtu_msgs.h>
#include <MbTcp/tcp_mb.h>

#define MB_TCP_RESP_SZ   300

//typedef struct mb_tcp_adu
//{
//    uint16_t transaction_id;
//    uint16_t protocol_id;
//    uint16_t len;
//    uint8_t unit_id;
//    uint8_t func_code;
//    REQ_DATA_UT data;
//} MB_TCP_ADU_ST, *MB_TCP_ADU_PST;
//
//typedef struct mbap_hdr_ {
//  uint16_t   transaction_id;
//  uint16_t   protocol_id;
//  uint16_t   len;
//  uint8_t    unit_id;
//}MBAP_HEADER_ST, *MBAP_HEADER_PST;

class ModbusTcp: public AManagedTask
{
    CTcpConnector m_tcpConnector; //!< TCP Connector for the server

    char m_dataBuffer[PBUF_POOL_BUFSIZE];
    u16_t m_len;
    u8_t m_mb_tcp_resp[MB_TCP_RESP_SZ];
    MB_TCP_ADU_PST m_adu_ptr;
    uint32_t  token_val;
    MBAP_HEADER_PST m_mbap_hdr_ptr;

    ModbusTcp();

    static ModbusTcp s_instance;

public:
    static ModbusTcp& getInstance();

    virtual ~ModbusTcp();

    /**
     * Task control function.
     */
    void run();

    /**
     * The framework calls this function before the task is created to give a chance to a managed task to perform its
     * own initialization code and stop the task creation process if needed.
     *
     * @param pcName specifies the task name.
     * @param usStackDepth specifies the task stack depth.
     * @param uxPriority specifies the task priority.
     *
     * @return pdTRUE if success, pdFALSE otherwise. If the method return pdFALSE the task creation process
     * is stopped and no FreeRTOS resource are allocated.
     */
    portBASE_TYPE onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
            unsigned portBASE_TYPE uxPriority);

    void callback(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t token_val);
};

void modbusTcpCallback(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t token_val);

#endif /* MODBUSTCP_H_ */
