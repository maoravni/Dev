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
#include "ll_typedef.h"
#include "free_rtos_exp.h"
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
    return (0);
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
}  /* void USART6_IRQHandler() */

    /*----------------------------------------------------------------------------*
     Sets the 'wrk_mode'('TX'|'RX') on the 'mb_node'('MB_1'|'MB_2').
     *----------------------------------------------------------------------------*/

uint32_t usart3_set_dir(bool isTransmit)
{
    return 0;

}/******************************* mb_set_dir ********************************/

/*-----------------------------------------------------------------------------
 This function handles USART3 global interrupt request. Modbus Master port.
 USART1 is used as Modbus port#1 there.
 *----------------------------------------------------------------------------*/
void USART3_IRQHandler()
{
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

