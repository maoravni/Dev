/******************************************************************************
 *                     (C) LandaLabs 2011                                      *
 *******************************************************************************
 *
 *  PROJECT     : FEC
 *
 *  FILE        : usart.c
 *
 *  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Phantom\SW\BSP
 *
 *  PURPOSE     : Usart extentions for StdPeriph_Driver.
 *
 *  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
 *
 *  TARGET      : PHANTOM
 *
 *
 *  DATE        NAME              VER.    COMMENTS
 * -----------------------------------------------------------------------------
 *  2012.01     Igor Natachanny   1.0.0   Initial version
 *
 *******************************************************************************
 Copyright and Proprietary Statment.
 *******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "stdarg.h"
#include <assert.h>
#include <stm32f4xx.h>
#include "ll_typedef.h"
#include "free_rtos_exp.h"
#include "mb_cnfg.h"
//  #include "..\..\..\Common\MODBUS\Rtu\rtu_slave.h"
//  #include "..\..\..\Common\MODBUS\Rtu\rtu_master.h"
//  #include "..\..\..\Common\MODBUS\modbus_exp.h"
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
#include "usart.h"
#include <leds.h>

/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
//extern xSemaphoreHandle xSmphrTx, xSmphrRx;
T_UsartRespBuffer usartResponseBuffer;
xSemaphoreHandle usartResponseCompleteSemaphore;
//xSemaphoreHandle usartForcedDelaySemaphore;

/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */

/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */
/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/

/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */

//  #define BUF_SIZE 10
/* --- PRIVATE MACROS ------------------------------------------------------- */
//#define IS_MOSQUITO_MB_USART(USART) (((USART) == USART6) || \
//                                     ((USART) == USART3))
/* --- PRIVATE TYPES -------------------------------------------------------- */

struct buf_st
{
    int in;                            // Next In Index
    int out;                           // Next Out Index
    char buf[SERRXBUFSIZE];                       // Buffer
};

/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */

void init_buf(struct buf_st *p);
int32_t get_char(struct buf_st *p, uint8_t* c_ptr);
int32_t put_char(struct buf_st *p, uint8_t c);

/* --- PRIVATE_DATA --------------------------------------------------------- */

//  static struct buf_st   rbuf_1 = { 0, 0, };
//  static struct buf_st   tbuf_1 = { 0, 0, };
static struct buf_st rbuf_3 =
{ 0, 0, };
static struct buf_st tbuf_3 =
{ 0, 0, };

static struct buf_st rbuf_6 =
{ 0, 0, };
static struct buf_st tbuf_6 =
{ 0, 0, };

//  static uint8_t mb2_req_buf[SERRXBUFSIZE];
//  static uint8_t mb2_rsp_buf[SERRXBUFSIZE];

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/*----------------------------------------------------------------------------*
 Reads "size" bytes from Tx buffer of "USARTx" to specified by "data_ptr" memo-
 ry area. A user will responsible to this pointer validity.
 Returns: -1 (fault) if this USART is not supported,
 -2 (fault) if "data_ptr" = 0,
 -3 (fault) if "size" = 0,
 0 (success) - else.
 *----------------------------------------------------------------------------*/

uint32_t rcv_buf(uint8_t *data_ptr, uint32_t size, USART_TypeDef *USARTx)
{
    uint32_t res;

    while (size != 0)
    {

        do
        {
            res = rcv_char(USARTx, data_ptr);
        } while (res);

        size--;
        data_ptr++;
    }

    return res;
}

/*----------------------------------------------------------------------------*
 Writes "size" bytes specified by "data_ptr"  to a Tx buffer of "USARTx".
 Returns: -1 (fault) if this USART is not supported,
 -2 (fault) if "data_ptr" = 0,
 -3 (fault) if "size" = 0,
 0 (success) - else.
 *----------------------------------------------------------------------------*/

uint32_t send_buf(uint8_t *data_ptr, uint32_t size, USART_TypeDef *USARTx)
{
    uint32_t res;

    while (size != 0)
    {

        do
        {
            res = send_char(USARTx, *data_ptr);
        } while (res);

        size--;
        data_ptr++;
    }

//    xSemaphoreTake(usartForcedDelaySemaphore, portMAX_DELAY);
//    vTaskDelay(3);
//    xSemaphoreGive(usartForcedDelaySemaphore);

    return res;
}

/*----------------------------------------------------------------------------
 Clears the rx/tx buffers and all errors of the selected USART.
 Returns: -1 (fault) if this USART is not supported, 0 (success) - else.
 *----------------------------------------------------------------------------*/

uint32_t init_usart(USART_TypeDef *USARTx)
{
    struct buf_st *rx_buf_ptr = 0, *tx_buf_ptr = 0;

    if (USARTx == USART1 )
    {
//      rx_buf_ptr = &rbuf_1;
//      tx_buf_ptr = &tbuf_1;
    }
    else
    {
        if (USARTx == USART3 )
        {
            rx_buf_ptr = &rbuf_3;
            tx_buf_ptr = &tbuf_3;

            // init the semaphore for receiving the data:
            vSemaphoreCreateBinary(usartResponseCompleteSemaphore);
//            vSemaphoreCreateBinary(usartForcedDelaySemaphore);
        }
        else
        {
            if (USARTx == USART6 )
            {
                rx_buf_ptr = &rbuf_6;
                tx_buf_ptr = &tbuf_6;
            }
            else
                return (-1);/* USART is not supported */
        }
    }

    rx_buf_ptr->in = rx_buf_ptr->out = 0;
    tx_buf_ptr->in = tx_buf_ptr->out = 0;

    // Init variables
//    Usart6SerRxClr();
//    Usart6SerTxClr();

//    Usart6SerErrors = 0;

    return (0);/* success */

}/* init_usart */

/*------------------------------------------------------------------------------
 Retrive first received character from apropriate receive buffer.
 Returns: -1 (fault) if a buffer is empty, -2(fault) if wrong port number, 0 (success) - else.
 *----------------------------------------------------------------------------*/
int32_t rcv_char(USART_TypeDef *USARTx, uint8_t *c_ptr)
{
    struct buf_st *p = 0;
    volatile int32_t res;

//    if(USARTx == USART1)    
//      p = &rbuf_1;
//        else{
    if (USARTx == USART3 )
        p = &rbuf_3;
    else
    {
        if (USARTx == USART6 )
            p = &rbuf_6;
        else
            return (-2);/* wrong port number */
    }
//     }

    res = get_char(p, c_ptr);

    return (res);
}

/*------------------------------------------------------------------------------
 Puts a character to be sent into apropriate transmit buffer.
 Returns: -1 (fault) if a buffer is full, -2(fault) if wrong port number, 0 (success) - else.
 *------------------------------------------------------------------------------*/
int32_t send_char(USART_TypeDef *USARTx, uint8_t c)
{
    struct buf_st *p = 0;
    volatile int32_t res;

//    if(USARTx == USART1)    
//      p = &tbuf_1;
//        else{
    if (USARTx == USART3 )
        p = &tbuf_3;
    else
    {
        if (USARTx == USART6 )
            p = &tbuf_6;
        else
            return (-2);/* wrong port number */
    }
//     }		          

    res = put_char(p, c);

    //USARTx->CR1 |= USART_FLAG_TXE;
    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);

    return (res);
}

/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/

/*-----------------------------------------------------------------------------
 This function handles USART6 global interrupt request.
 USART6 is used as AUX_TERM here.
 *----------------------------------------------------------------------------*/
void USART6_IRQHandler()
{
    if (((USART6 ->SR & USART_FLAG_TXE )!= 0)&&((USART6 ->CR1 & USART_FLAG_TXE) != 0)){
    if (tbuf_6.out == tbuf_6.in)
    USART6 ->CR1 &= ~USART_FLAG_TXE;		      // disable TX interrupt if nothing to send
    else
    {
        USART6 ->DR = tbuf_6.buf[tbuf_6.out];
        tbuf_6.out = (tbuf_6.out + 1) & SERBUFMASK;
//            if (tbuf_6.out == SERRXBUFSIZE)
//                tbuf_6.out = 0;
    }
}

if ((USART6 ->SR & USART_FLAG_RXNE) != 0)
{   // Rx interrupt
    rbuf_6.buf[rbuf_6.in] = USART6 ->DR & 0x1FF;
    rbuf_6.in = (rbuf_6.in + 1) & SERBUFMASK;
//        if (rbuf_6.in == SERRXBUFSIZE)
//            rbuf_6.in = 0;
}

if ((USART6 ->SR & USART_FLAG_IDLE) != 0)
{   // Idle frame is detected

}

}  /* void USART6_IRQHandler() */

    /*----------------------------------------------------------------------------*
     Sets the 'wrk_mode'('TX'|'RX') on the 'mb_node'('MB_1'|'MB_2').
     *----------------------------------------------------------------------------*/

uint32_t usart3_set_dir(bool isTransmit)
{

    if (isTransmit)
    {
        DE_PORT ->BSRRL = DE_PIN; /* DE2 Hight */
        RE_PORT ->BSRRL = RE_PIN; /* RE2 Hight */

//        USART3 ->CR1 &= ~USART_FLAG_RXNE; // disable receive interrupt
//        USART3 ->CR1 &= ~USART_FLAG_IDLE; // disable idle interrupt
        USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
        USART_ITConfig(USART3, USART_IT_IDLE, DISABLE);
//        led_light(MBS_TX_LED);
//        led_dark(MBS_RX_LED);
        led_toggle(MODBUS_LED);
    }
    else
    {
//        led_dark(MBS_TX_LED);
//        led_light(MBS_RX_LED);
        led_toggle(MODBUS_LED);
        DE_PORT ->BSRRH = DE_PIN; /* DE2 Low */
        RE_PORT ->BSRRH = RE_PIN; /* RE2 Low */

//        USART3 ->CR1 |= USART_FLAG_RXNE; // enable receive interrupt
        USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//        USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
    }

    return 0;

}/******************************* mb_set_dir ********************************/

/*-----------------------------------------------------------------------------
 This function handles USART3 global interrupt request. Modbus Master port.
 USART1 is used as Modbus port#1 there.
 *----------------------------------------------------------------------------*/
void USART3_IRQHandler()
{
    static portBASE_TYPE xHigherPriorityTaskWoken;
    uint32_t status_reg_val = 0;
    int dummy = 0;

    /*** Read data register not empty ***/
    if ((USART3 ->CR1 & USART_FLAG_RXNE )!= 0){
        if ((USART3 ->SR & USART_FLAG_RXNE )!= 0){   // Rx interrupt
            USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
            rbuf_3.buf[rbuf_3.in] = USART3 ->DR & 0x1FF;
//            mb_node_1->resp_len++;
            usartResponseBuffer.resp_len++;
            rbuf_3.in = (rbuf_3.in + 1) & SERBUFMASK;
//            if (rbuf_3.in < SERRXBUFSIZE)
//                rbuf_3.in++;
//            else
//                rbuf_3.in = 0;
        }
    }/* if ((USART3->CR1 & USART_FLAG_RXNE) */

    /*** IDLE line detected ***/
    if ((USART3 ->CR1 & USART_FLAG_IDLE )!= 0){
        if ((USART3 ->SR & USART_FLAG_IDLE )!= 0){
            status_reg_val = USART3 ->SR;/* as first step in a sequence for TC and IDLE flags reset */
            /* Dummy Data Register reading for complete the sequence for IDLE bit Reset. */
            dummy = USART3 ->DR & 0x1FF;
            dummy++;/* this for compiler warning prevent */
            status_reg_val += dummy;
            led_dark(MODBUS_LED);
            if (usartResponseBuffer.resp_len > 3)
            {
                memcpy(usartResponseBuffer.resp_buf, rbuf_3.buf, rbuf_3.in);
//          mb_node_1->resp_len = rbuf_1.in;
//                puts("usart give");
                xSemaphoreGiveFromISR(usartResponseCompleteSemaphore, &xHigherPriorityTaskWoken);
//                if (xHigherPriorityTaskWoken)
//                    taskYIELD();
            }
            rbuf_3.out = rbuf_3.in = 0;
        }
    }/* if ((USART3->CR1 & USART_FLAG_IDLE) */

    /*** Transmit data register empty ***/
    if ((USART3 ->CR1 & USART_FLAG_TXE )!= 0){
        if ((USART3 ->SR & USART_FLAG_TXE )!= 0){
            if (tbuf_3.out != tbuf_3.in)
            {
                USART3 ->DR = tbuf_3.buf[tbuf_3.out];
                tbuf_3.out = (tbuf_3.out + 1) & SERBUFMASK;
//                tbuf_3.out++;
//                if (tbuf_3.out == SERRXBUFSIZE)
//                    tbuf_3.out = 0;
            }
            else
            {
                tbuf_3.out = tbuf_3.in = 0;
                USART3 ->CR1 &= ~USART_FLAG_TXE;/* disable TX interrupt if nothing to send */
                USART3 ->CR1 |= USART_FLAG_TC; // enable transmission complete interrupt.
//                USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
//                USART_ITConfig(USART3, USART_IT_TC, ENABLE);
            }
        }
    }/* if ((USART3->CR1 & USART_FLAG_TXE) */

    /*** Transmission complete ***/
    if ((USART3 ->CR1 & USART_FLAG_TC )!= 0){
        if ((USART3 ->SR & USART_FLAG_TC )!= 0){
            /* Dummy Data Register write for complete the sequence for TC bit Reset. */
//        USART3->DR = 0;
//            USART_ITConfig(USART3, USART_IT_TC, DISABLE);
            USART3 ->CR1 &= ~USART_FLAG_TC; // disable transmission complete interrupt.
//            mb_set_dir(mb_node_1->name, RX); //xSemaphoreGiveFromISR( mb_node_1->req_tx_end_sem, &xHigherPriorityTaskWoken );
            usart3_set_dir(false);
        }
    }/* if ((USART3->CR1 & USART_FLAG_TC) */

}/* void USART3_IRQHandler() */

/*------------------------------------------------------------------------------
 Clear com buffer indexes
 *------------------------------------------------------------------------------*/
void init_buf(struct buf_st *p)
{

    p->in = 0;
    p->out = 0;
}

/*----------------------------------------------------------------------------
 Params:  a pointer to a desiried buffer,
 Returns: 0 (success);  -1 (fault) if a buffer is empty.
 *----------------------------------------------------------------------------*/
int32_t get_char(struct buf_st *p, uint8_t* c_ptr)
{

    if (p->out == p->in)
        return (-1);
    else
    {
        *c_ptr = p->buf[p->out];
        p->out = (p->out + 1) & SERBUFMASK;
//        p->out++;
//        if (p->out == SERRXBUFSIZE)
//            p->out = 0;
        return (0);
    }
}/* get_char */
/*----------------------------------------------------------------------------
 Params:  a pointer to a desiried buffer,
 Returns: 0 (success);  -1 (fault) if a buffer is full.
 *----------------------------------------------------------------------------*/
int32_t put_char(struct buf_st *p, uint8_t c)
{
    unsigned int tmp_inx = p->in;

    tmp_inx = (tmp_inx + 1) & SERBUFMASK;
//    tmp_inx++;
//    if (tmp_inx == SERRXBUFSIZE)
//        tmp_inx = 0;

    if (tmp_inx == p->out)
        return (-1);
    else
    {
        p->buf[p->in] = c;
        p->in = tmp_inx;
        return (0);
    }
}/* put_char */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/

