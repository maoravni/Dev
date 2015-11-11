/******************************************************************************
 *                     (C) LandaLabs 2012                                      *
 *******************************************************************************
 *
 *  PROJECT     : MODBUS_LIB
 *
 *  FILE        : rtu_master.c
 *
 *  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\MODBUS\Rtu
 *
 *  PURPOSE     :
 *
 *  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
 *
 *  TARGET      :
 *
 *
 *  DATE        NAME              VER.    COMMENTS
 * -----------------------------------------------------------------------------
 *  2012.08     Igor Natachanny   1.1.0   Total Retries Counter Support added.
 *  2012.11     Yossi Sarusi      1.2     Add retry for Modbus device error (0x80).
 *                                        FEC board porting
 *******************************************************************************
 Copyright and Proprietary Statment.

 *******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
#define  GET_NODE_ID_IMPL_HERE
#define  MB_INIT
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
#include <stdio.h>
#include <string.h>
#include <stm32f2xx.h>
#include "free_rtos_exp.h"
#include "..\..\AUX_TERM\aux_term.h"
#include "usart.h"
#include "ll_typedef.h"
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
#include "rtu_msgs.h"
#include "mb_cnfg.h"
#include "..\Common\mb_infra.h"
#include "rtu_master.h"
#include "..\..\HMI\hmi.h"
#include "iwdg.h"
#ifdef CPP_FRAMEWORK
#include <logger.h>
//#include "StatusLed.h"
#endif
/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
uint16_t crc16_update(uint16_t crc, uint8_t a);
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */
MB_MASTER_NODE_PST mb_node_1;
/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */

/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
#define   HIGHT_BYTE(uint16_t)   ((uint8_t) ((uint16_t) >> 8))
#define     LOW_BYTE(uint16_t)   ((uint8_t) ((uint16_t) & 0xff))

/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
void default_callback_func(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t token_val);
static LL_STATUS send_mb_master_queue(MB_MASTER_NODE_PST node_ptr, MB_MASTER_IN_QUE_ITM_PST new_req_ptr, bool urgent,
        bool from_interrupt);
typedef uint32_t (*mb_func_hndl)(void*);
void rtu_master_comm_task(MB_MASTER_NODE_PST mb_node_ptr);
uint16_t mb_pack_u16(uint16_t data);
void mb_get_slave_resp(RTU_FUNC_ERR_ET func_status, uint8_t *resp_buf, int32_t resp_sz, uint16_t **resp_data_ptr,
        int32_t *data_amount);

/* --- PRIVATE_DATA --------------------------------------------------------- */
//static   int8_t     msg_buf[255];
static uint32_t total_retries = 0;
uint32_t rtu_master_stack_wm;
static uint32_t total_no_resp = 0;
//long static StartSysTickCounter=0;
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/**
 * @brief  : Returns a total number of Modbus Slave Responce absents.
 *
 * @param  : none
 *
 * @retval : total number of responce absents.
 **/
uint32_t get_total_no_resp()
{
    return total_no_resp;
}/********************** get_total_retries_val ******************************/

/**
 * @brief  : Returns a total number of Modbus communication faults.
 *
 * @param  : none
 *
 * @retval : total number of faults.
 **/
uint32_t get_total_retries()
{
    return total_retries;
}/********************** get_total_retries_val ******************************/

/**
 * @brief    Builds a INPUT_REGS_READ master request message and sends it to
 * @brief    the input queue of deseried Modbus Master Node.
 *
 * @param    node_ptr:       The pointer to the appropriate Master Node Descriptor.
 * @param    slave_id:       The Modbus slave device identificator.
 * @param    start_addr:
 * @param    regs_quant:
 * @param    urgent:         Is this message must be sent as soon as possible?
 * @param    from_interrupt: Is now this function is called from a ISR?
 * @param    cb_func :       The pointer to call-back function.
 *
 * @retval   res: the result of sending to queue.
 */

LL_STATUS rtu_input_regs_read(MB_MASTER_NODE_PST node_ptr, uint8_t slave_id, uint16_t start_addr, uint16_t regs_quant,
        bool urgent, bool from_interrupt, uint32_t token_val,
        void (*cb_func)(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t token_val))
{
    LL_STATUS res = LL_ERROR;
    MB_MASTER_IN_QUE_ITM_ST new_req;

    // Maor {1 Jan 2013}: Static allocation fix
    //new_req.data_ptr = pvPortMalloc(sizeof(INPUT_REGS_READ_REQ_DATA_ST));

//    if (new_req.data_ptr)
//    {
    new_req.token_val = token_val;
    new_req.func_code = INPUT_REGS_READ;
    new_req.slave_id = slave_id;
    // Maor {1 Jan 2013}: Static allocation fix
    new_req.data_arr.input_regs_read_req.start_addr = start_addr;
    new_req.data_arr.input_regs_read_req.regs_quant = regs_quant;
    new_req.data_size = sizeof(MB_RTU_REQ_ADU_ST);
    new_req.call_back_fun = (*cb_func);
    res = send_mb_master_queue(node_ptr, &new_req, urgent, from_interrupt);
    if (res != LL_OK)
    {
//            vPortFree(new_req.data_ptr);
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "rtu_input_regs_read: error in send_mb_master_queue");
        while (1)
        {
            ;
        }
    }
//    }/* if(new_req.data_ptr) */

    return res;
}/************************* rtu_input_regs_read ******************************/

/**
 * @brief    Builds a HOLD_REGS_READ master request message and sends it to
 * @brief    the input queue of deseried Modbus Master Node.
 *
 * @param    node_ptr:       The pointer to the appropriate Master Node Descriptor.
 * @param    slave_id:       The Modbus slave device identificator.
 * @param    start_addr:
 * @param    regs_quant:
 * @param    urgent:         Is this message must be sent as soon as possible?
 * @param    from_interrupt: Is now this function is called from a ISR?
 * @param    cb_func :       The pointer to call-back function.
 *
 * @retval   res: the result of sending to queue.
 */

LL_STATUS rtu_hold_regs_read(MB_MASTER_NODE_PST node_ptr, uint8_t slave_id, uint16_t start_addr, uint16_t regs_quant,
        bool urgent, bool from_interrupt, uint32_t token_val,
        void (*cb_func)(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t token_val))
{
    LL_STATUS res = LL_ERROR;
    MB_MASTER_IN_QUE_ITM_ST new_req;

    // Maor {1 Jan 2013}: Static allocation fix
//    new_req.data_ptr = pvPortMalloc(sizeof(HOLD_REGS_READ_REQ_DATA_ST));

//    if (new_req.data_ptr)
//    {
    new_req.func_code = HOLD_REGS_READ;
    new_req.token_val = token_val;
    new_req.slave_id = slave_id;
    new_req.data_arr.hold_regs_read_req.start_addr = start_addr;
    new_req.data_arr.hold_regs_read_req.regs_quant = regs_quant;
    new_req.call_back_fun = (*cb_func);
    new_req.data_size = sizeof(MB_RTU_REQ_ADU_ST);

    res = send_mb_master_queue(node_ptr, &new_req, urgent, from_interrupt);
    if (res != LL_OK)
    {
        //vPortFree(new_req.data_ptr);
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,
                "ERROR: rtu_hold_regs_read: error in send_mb_master_queue. Urgent=%d, From_interrupt=%d", urgent,
                from_interrupt);
//20120909            while (1)
//            {
//              ;
//            }

    }
//    }/* if(new_req.data_ptr) */

    return res;
}/************************* rtu_hold_regs_read ******************************/

/**
 * @brief    Builds a WRITE_SINGLE_REG master request message and sends it to
 * @brief    the input queue of deseried Modbus Master Node.
 *
 * @param    node_ptr:       The pointer to the appropriate Master Node Descriptor.
 * @param    slave_id:       The Modbus slave device identificator.
 * @param    reg_addr:       The register into the slave device to be re-writed.
 * @param    reg_val :       The new value to be placed into the register.
 * @param    urgent:         Is this message must be sent as soon as possible?
 * @param    from_interrupt: Is now this function is called from a ISR?
 * @param    cb_func :       The pointer to call-back function.
 *
 * @retval   res: the result of sending to queue.
 */

LL_STATUS rtu_single_reg_write(MB_MASTER_NODE_PST node_ptr, uint8_t slave_id, uint16_t reg_addr, uint16_t reg_val,
        bool urgent, bool from_interrupt, uint32_t token_val,
        void (*cb_func)(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t token_val))
{
    LL_STATUS res = LL_ERROR;
    MB_MASTER_IN_QUE_ITM_ST new_req;

    // Maor {1 Jan 2013}: Static allocation fix
    //new_req.data_ptr = pvPortMalloc(sizeof(SINGLE_REG_WRITE_REQ_DATA_ST));

//    if (new_req.data_ptr)
//    {
    new_req.func_code = SINGLE_REG_WRITE;
    new_req.token_val = token_val;
    new_req.slave_id = slave_id;
    new_req.data_arr.single_reg_write_req.reg_addr = reg_addr;
    new_req.data_arr.single_reg_write_req.reg_val = reg_val;
    new_req.data_size = sizeof(MB_RTU_REQ_ADU_ST);
    new_req.call_back_fun = (*cb_func);
    res = send_mb_master_queue(node_ptr, &new_req, urgent, from_interrupt);
    if (res != LL_OK)
    {
        //vPortFree(new_req.data_ptr);
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "rtu_single_reg_write: error in send_mb_master_queue");
//20120909            while (1)
//            {
//              ;
//            }

    }
//    }/* if(new_req.data_ptr) */

    return res;
}/************************* rtu_single_reg_write ****************************/

/**
 * @brief    Creates a Modbus Master Node at a selected media.
 * @param    mb_node_name: Modbus node identificator.
 * @retval   node_ptr:     pointer to created node descriptor(NULL - if fault)
 */
MB_MASTER_NODE_PST mb_master_node_create(MB_NODE_NAME_ET mb_node_name)
{
    MB_MASTER_NODE_PST node_ptr = NULL;

    node_ptr = pvPortMalloc(sizeof(MB_MASTER_NODE_ST));

    if (node_ptr)
    {
        node_ptr->name = mb_node_name;
        /* assigment to real connectivites */
        node_ptr->media_ptr = mb_media_asign[mb_node_name];
    }

    else
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"mb_master_node_create: error in pvPortMalloc");
        while (1)
        {
            ;
        }
    }

    return node_ptr;
}/* mb_master_node_create */

/**
 * @brief   Creates a communication handler task for a previously created Modbus
 * @brief   Master Node "node_ptr".Once created a Master is switched in TX mode.
 * @param   mb_task_name:
 * @param   stack_depth:  deseried task stack size.
 * @param   priority:     deseried task priority;
 * @param   que_len:      deseried task input queue lenght;
 * @param   node_ptr:     pointer to previously created node descriptor;
 * @retval  res:          Master Node init result.
 */
signed portBASE_TYPE mb_master_node_init(const signed char * const mb_task_name, unsigned short stack_depth,
        unsigned portBASE_TYPE priority, unsigned portBASE_TYPE que_len, MB_MASTER_NODE_PST node_ptr)
{
    signed portBASE_TYPE res = ERROR;

    if (node_ptr)
    {

        vSemaphoreCreateBinary(node_ptr->resp_rx_end_sem);
        if (node_ptr->resp_rx_end_sem)
            xSemaphoreTake(node_ptr->resp_rx_end_sem, portMAX_DELAY);

        node_ptr->queue = xQueueCreate( que_len, sizeof(MB_MASTER_IN_QUE_ITM_ST));
        if (node_ptr->queue)
        {
            res =
                    xTaskCreate( (pdTASK_CODE)rtu_master_comm_task, mb_task_name, stack_depth, (void *)node_ptr, priority, node_ptr->mb_task_handle );
            if (res)
            {
                //         node_ptr->resp_buf = pvPortMalloc(sizeof(MB_RTU_RESP_ADU_PUT));
                node_ptr->retry_count = RETRIES_NUM;
                mb_set_dir(node_ptr->name, TX);
            }
            else
            {
                vQueueDelete(node_ptr->queue); /* Delete the node queue and release   */
                vPortFree(node_ptr); /* taken by a node descriptor memory   */
                /* if a node communication task isn't created */
            }
        }

    }/* if(node_ptr) */

    return res;
}/************************** mb_master_node_init **************************/

/**
 * @brief   A main function of Modbus Master node communication Task.
 * @param   node_ptr: a pointer to a master modbus node descriptor;
 * @retval  res     : none.
 ***/

void rtu_master_comm_task(MB_MASTER_NODE_PST node_ptr)
{
    MB_MASTER_IN_QUE_ITM_ST cur_req;
    MB_RTU_REQ_ADU_UT mb_req;
    signed portBASE_TYPE res;
    uint16_t crc16_val, tmp_16;
    RTU_FUNC_ERR_ET func_status = RTU_FUNC_ERR_UNKNOWN;
    uint16_t *resp_data_ptr;
    int32_t data_amount;
    uint16_t Register, Data;

    while (1)
    {

        // reset the watchdog:
        IWDG_ReloadCounter();

        xQueueReceive(node_ptr->queue, (MB_MASTER_IN_QUE_ITM_PST)(&cur_req), portMAX_DELAY);

        led_toggle(MODBUS_LED);

        set_task_alive_bit(MB_1_MASTER_ALV_E);

#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
        rtu_master_stack_wm = uxTaskGetStackHighWaterMark(node_ptr->mb_task_handle);
#endif       
        mb_req.adu.slave_id = cur_req.slave_id;
        mb_req.adu.func_code = cur_req.func_code;
        func_status = RTU_FUNC_ERR_UNKNOWN;

        switch (cur_req.func_code)
        {
        case HOLD_REGS_READ:
            Register = cur_req.data_arr.hold_regs_read_req.start_addr;
            Data = cur_req.data_arr.hold_regs_read_req.regs_quant;
            mb_req.adu.req_data.hold_regs_read_req.start_addr = SWAP_16(cur_req.data_arr.hold_regs_read_req.start_addr);
            mb_req.adu.req_data.hold_regs_read_req.regs_quant = SWAP_16(cur_req.data_arr.hold_regs_read_req.regs_quant);
            break;
        case INPUT_REGS_READ:
            Register = cur_req.data_arr.input_regs_read_req.start_addr;
            Data = cur_req.data_arr.input_regs_read_req.regs_quant;
            mb_req.adu.req_data.input_regs_read_req.start_addr =
                    SWAP_16(cur_req.data_arr.input_regs_read_req.start_addr);
            mb_req.adu.req_data.input_regs_read_req.regs_quant =
                    SWAP_16(cur_req.data_arr.input_regs_read_req.regs_quant);
            break;
        case SINGLE_REG_WRITE:
            Register = cur_req.data_arr.single_reg_write_req.reg_addr;
            Data = cur_req.data_arr.single_reg_write_req.reg_val;
            mb_req.adu.req_data.single_reg_write_req.reg_addr = SWAP_16(cur_req.data_arr.single_reg_write_req.reg_addr);
            mb_req.adu.req_data.single_reg_write_req.reg_val = SWAP_16(cur_req.data_arr.single_reg_write_req.reg_val);
            break;

        default:
            continue;
            break;
        }/* switch(cur_req.func_code) */

        mb_req.adu.crc16_val = crc16_calc(&(mb_req.adu.slave_id), cur_req.data_size - 2);

        node_ptr->retry_count = RETRIES_NUM;

        while (node_ptr->retry_count)
        {
            node_ptr->resp_len = 0;
            mb_set_dir(node_ptr->name, TX);
            send_buf(mb_req.buf, cur_req.data_size, node_ptr->media_ptr);

            //wait for response completed
            res = xSemaphoreTake(node_ptr->resp_rx_end_sem, RTU_MASTER_SEM_RECV_TICK_TO_WAIT);
            if (res == pdPASS)
            {
                led_toggle(MODBUS_LED);
                crc16_val = crc16_calc(node_ptr->resp_buf, node_ptr->resp_len - 2);
                memcpy(&tmp_16, &(node_ptr->resp_buf[node_ptr->resp_len - 2]), sizeof(uint16_t));
                if (tmp_16 == crc16_val)
                {
                    if (node_ptr->resp_buf[1] & 0x80)
                        func_status = RTU_FUNC_ERR_EXEP;
                    else
                    {
                        func_status = RTU_FUNC_ERR_OK;
                        break;
                    }
                }
                else
                    func_status = RTU_FUNC_ERR_CRC16;

            }
            else
            {
                led_dark(MODBUS_LED);

                total_retries++;
                if (total_retries == UINT32_MAX)
                    total_retries = 0;
            }
            node_ptr->retry_count--;
        }/* while(node_ptr->retry_count) */

        if ((node_ptr->retry_count == 0) && (func_status == RTU_FUNC_ERR_UNKNOWN))
        {
            func_status = RTU_FUNC_ERR_NO_SLAVE_RESP;
            total_no_resp++;
            if (total_no_resp == UINT32_MAX)
                total_no_resp = 0;
        }

        if (func_status != RTU_FUNC_ERR_OK)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE,"ERROR: rtu_master_comm_task. func_code=%d, func_status=%d, SlaveDevice=%d, Register=%d, Data=%d",cur_req.func_code,func_status,cur_req.slave_id,Register,Data);
        }
        if (cur_req.token_val != 0xFFFFFFFF)/* if not mb_tcp request */
            mb_get_slave_resp(func_status, node_ptr->resp_buf, node_ptr->resp_len, &resp_data_ptr, &data_amount);

        if (cur_req.call_back_fun == NULL)
        {
            default_callback_func(func_status, resp_data_ptr, data_amount, cur_req.token_val);
        }
        else
        {
            if (cur_req.token_val != 0xFFFFFFFF)/* if not mb_tcp request */
                cur_req.call_back_fun(func_status, resp_data_ptr, data_amount, cur_req.token_val);
            else
                cur_req.call_back_fun(func_status, (uint16_t*) node_ptr->resp_buf, node_ptr->resp_len,
                        cur_req.token_val);
        }
        //vPortFree(cur_req.data_ptr);

        mb_set_dir(node_ptr->name, TX);

    }/* while(1) */

}/* rtu_master_comm_task */

/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/**
 * @brief: Transforms a slave response data to a clear form for application.
 *
 * @param: func_status
 *         resp_buf
 *         resp_sz
 *
 * @retval: none
 **/
void mb_get_slave_resp(RTU_FUNC_ERR_ET func_status, uint8_t *resp_buf, int32_t resp_sz, uint16_t **resp_data_ptr,
        int32_t *data_amount)
{
    MB_RTU_RESP_ADU_PUT slave_resp_ptr = (MB_RTU_RESP_ADU_PUT) resp_buf;
    int32_t vals_amount, i;
    uint16_t *vals_ptr, tmp;

    switch (func_status)
    {

    case RTU_FUNC_ERR_OK:

        switch (slave_resp_ptr->buf[1])
        {
        case HOLD_REGS_READ:
        case INPUT_REGS_READ:
        case SINGLE_REG_WRITE:
            vals_amount = slave_resp_ptr->buf[2] / 2;
            *data_amount = vals_amount;
            vals_ptr = (uint16_t*) &(slave_resp_ptr->buf[3]);
            *resp_data_ptr = vals_ptr;
            for (i = 0; i < vals_amount; i++)
            {
                tmp = *(vals_ptr + i);
                *(vals_ptr + i) = SWAP_16(tmp);
            }
            break;
        }

        break;
    case RTU_FUNC_ERR_EXEP:
        break;
    default:
        break;
    };/* switch(func_status) */

}/*************************** mb_get_slave_resp *****************************/
/**
 * @brief: Default callback function for
 *
 * @param
 *
 * @retval
 **/
void default_callback_func(uint8_t func_status, uint16_t *resp_buf, int32_t resp_sz, uint32_t token_val)
{
    uint16_t reg_addr, reg_val, tmp_16;

    //sprintf( (char*)msg_buf,"\r default_cb_func called!");
    M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE,"Default_cb_func called!");

    switch (func_status)
    {
    case 1:
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,"ERROR: Slave Responce CRC_16 FAULT!");
        break;
    case 2:
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,"ERROR: Slave didn't answer!");
        break;
    case 3:
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,"ERROR: Function Exeption: incorrect function params or Function unsupported by slave!");
        break;
    case 0:
        switch (resp_buf[1])
        {
        case HOLD_REGS_READ:
            M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE,"HOLD_REGS_READ: - OK!");
            //            byte_count = resp_buf[2];
            //            sprintf( (char*)msg_buf,"\r HOLD_REGS_READ: slave_id = 0x%x, reg_addr = 0x%x, reg_val = %d - OK!", resp_buf[0], reg_addr, reg_val);
            break;
        case SINGLE_REG_WRITE:
            memcpy(&tmp_16, &(resp_buf[2]), sizeof(uint16_t));
            reg_addr = SWAP_16(tmp_16);
            memcpy(&tmp_16, &(resp_buf[4]), sizeof(uint16_t));
            reg_val = SWAP_16(tmp_16);
            M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE,"SINGLE_REG_WRITE: slave_id = 0x%x, reg_addr = 0x%x, reg_val = %d - OK!", resp_buf[0], reg_addr, reg_val);
            break;
        }/* switch(func_code) */
        break;
    default:
        M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE,"UNKNOWN ERROR!");
        break;
    }/* switch(func_status) */

}/******************* void default_callback_func() *************************/

/**
 * @brief
 * @brief
 *
 * @param   node_ptr       :  pointer to Modbus Master node descriptor;
 * @param   new_req_ptr    :  pointer to previously filed mb_master queue item structure;
 * @param   urgent         :  TRUE -  if if the message should be sended ASAP;
 * @param   from_interrupt :  TRUE -  if this function is called from a ISR;
 *
 * @retval  res:          result.
 */

static LL_STATUS send_mb_master_queue(MB_MASTER_NODE_PST node_ptr, MB_MASTER_IN_QUE_ITM_PST new_req_ptr, bool urgent,
        bool from_interrupt)
{
    portBASE_TYPE RetValue;
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    if (from_interrupt)
    {
        if (urgent)
            RetValue = xQueueSendToFrontFromISR( node_ptr->queue, new_req_ptr, &xHigherPriorityTaskWoken );
        else
            RetValue = xQueueSendToBackFromISR( node_ptr->queue, new_req_ptr, &xHigherPriorityTaskWoken );
    }
    else
    {
        if (urgent)
            RetValue = xQueueSendToFront( node_ptr->queue, new_req_ptr, RTU_MASTER_SEND_QUEU_TICK_TO_WAIT );
        else
            RetValue = xQueueSendToBack( node_ptr->queue, new_req_ptr, RTU_MASTER_SEND_QUEU_TICK_TO_WAIT );
    }

    /* Switch tasks if necessary. */
    if (xHigherPriorityTaskWoken != pdFALSE)
    {
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken);
    }

    if (RetValue != pdPASS)
        return LL_ERROR;

    return LL_OK ;
}
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
/**
 * @brief
 *
 * @param
 *
 * @retval
 **/
//       sprintf( (char*)msg_buf,"\r status = %x, slave_id = %x, func_code = %x", node_ptr->resp.buf[1], node_ptr->resp.buf[0], node_ptr->resp.buf[1]);
/**
 * @brief   Modbus Slave Node rerspoce papser.
 *
 * @param   node_ptr:     pointer to Modbus Master node descriptor;
 *
 * @retval  res:          Master Node init result.
 **/

