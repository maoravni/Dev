/*
 * ModbusTcp.cpp
 *
 *  Created on: 23 Jan 2013
 *      Author: maora
 */

#include "MbTcp/ModbusTcp.h"
#include <iwdg_cnfg.h>
#include <iwdg.h>
#include <string.h>
#include <rtu_master.h>
#include <cfg_db_if.h>
#include "mb_infra.h"
#include <os_cnfg.h>

ModbusTcp ModbusTcp::s_instance;

ModbusTcp::ModbusTcp() :
        m_tcpConnector(502)
{
    token_val = 0xFFFFFFFF;
    m_mbap_hdr_ptr = (MBAP_HEADER_PST) &(m_mb_tcp_resp[0]);
}

ModbusTcp::~ModbusTcp()
{
}

ModbusTcp& ModbusTcp::getInstance()
{
    if (!s_instance.isValid())
    {
        // Create the task.
        s_instance.create("ModbusTcp", MODBUS_TASK_STACK_SZ, MODBUS_TASK_PRIORITY);
    }

    return s_instance;
}

portBASE_TYPE ModbusTcp::onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
        unsigned portBASE_TYPE uxPriority)
{
    // TODO: Issue an error when task creation isn't successfull.
    portBASE_TYPE res;

    // create the TCP connector
    if ((res = m_tcpConnector.create("ModbusTcp", usStackDepth)) != pdPASS)
        return res;

    return res;
}

void ModbusTcp::run()
{
    err_t err;
    LL_STATUS res;

    //LWIP_UNUSED_ARG(arg);

    while (1)
    {

        set_task_alive_bit(TCP_MB_ALV_E);

#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
        tcp_mb_stack_wm = uxTaskGetStackHighWaterMark(0);
#endif
        if (m_tcpConnector.receive(&m_dataBuffer, m_len, 50) == pdPASS)
        {
            m_adu_ptr = (MB_TCP_ADU_PST) &m_dataBuffer;

            memcpy(m_mb_tcp_resp, m_dataBuffer, 4);

            if (m_adu_ptr->unit_id == THIS_UNIT)
            {
                res = db_req_hndr(m_adu_ptr->func_code, m_adu_ptr->data, m_mb_tcp_resp, NULL);
                //netconn_write(newconn, mb_tcp_resp, resp_sz + 2 , NETCONN_COPY);
                m_tcpConnector.send(m_mb_tcp_resp, res);
            }
            else
            {/* external slave */
                switch (m_adu_ptr->func_code)
                {
                case HOLD_REGS_READ:
                    do
                    {
                        res = rtu_hold_regs_read(mb_node_1, m_adu_ptr->unit_id,
                                SWAP_16(m_adu_ptr->data.single_reg_write_req.reg_addr),
                                SWAP_16(m_adu_ptr->data.single_reg_write_req.reg_val), 0, 0, token_val,
                                modbusTcpCallback);
                    } while (res != LL_OK );
                    break;
                case INPUT_REGS_READ:
                    do
                    {
                        res = rtu_input_regs_read(mb_node_1, m_adu_ptr->unit_id,
                                SWAP_16(m_adu_ptr->data.single_reg_write_req.reg_addr),
                                SWAP_16(m_adu_ptr->data.single_reg_write_req.reg_val), 0, 0, token_val,
                                modbusTcpCallback);
                    } while (res != LL_OK );
                    break;
                case SINGLE_REG_WRITE:
                    do
                    {
                        res = rtu_single_reg_write(mb_node_1, m_adu_ptr->unit_id,
                                SWAP_16(m_adu_ptr->data.single_reg_write_req.reg_addr),
                                SWAP_16(m_adu_ptr->data.single_reg_write_req.reg_val), 0, 0, token_val,
                                modbusTcpCallback);
                    } while (res != LL_OK );
                    break;
                default:
                    res = 10;
                    break;
                }/* switch(m_adu_ptr->func_code) */
            }/* external slave */
        }
    }
}

void ModbusTcp::callback(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t token_val)
{
    switch (func_status)
    {
    case 2:
        resp_buf[0] = m_adu_ptr->func_code | 0x80;
        break;
    default:
        memcpy(&(m_mb_tcp_resp[6]), resp_buf, resp_sz - 2);
        m_mbap_hdr_ptr->len = SWAP_16(resp_sz-2);
        m_tcpConnector.send(m_mb_tcp_resp, (resp_sz - 2) + 6);
        break;

    }/* switch(func_status) */
}

void modbusTcpCallback(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t token_val)
{
    ModbusTcp::getInstance().callback(func_status, resp_buf, resp_sz, token_val);
}
