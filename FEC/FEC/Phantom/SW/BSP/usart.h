/******************************************************************************
*                     (C) LandaLabs 2011                                      *
*******************************************************************************
*
*  PROJECT     : PHANTOM 
*
*  FILE        : usart.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Phantom\SW\BOARD
*
*  PURPOSE     : 
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

 Igor Natachanny 
 Independent Embedded System Professional.
 igornatachanny@hotmail.com 054-7656857 
******************************************************************************/

#ifndef _USART_TERM_H_
#define _USART_TERM_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Speed */
#define USART_SPEED 57600                            /* in bauds */
#define USART_TIC   ((10 * 1000000 / USART_SPEED))   /* Time Inter Character in µs */

/* COM buffers */
#define SERRXBUFSIZE    256         /* Size of serial receive buffer */
#define SERTXBUFSIZE    256         /* Size of serial transmit buffer */

/* Error codes */
#define ERR_LOW_LEVEL 0x99
#define ERR_OVERFLOW  0x01
#define ERR_FRAME     0x02
#define ERR_NOISE     0x04
#define ERR_OVERRUN   0x08



/* Exported variables --------------------------------------------------------*/


/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/
/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
   int32_t get_tx_complete_flag(USART_TypeDef *USARTx);
  void clear_tx_complete_flag(USART_TypeDef *USARTx);
  int32_t get_rx_idle_flag(USART_TypeDef *USARTx);
  int32_t get_rx_idle_flag(USART_TypeDef *USARTx); 
  void clear_rx_idle_flag(USART_TypeDef *USARTx);
  uint32_t rcv_buf(uint8_t *data_ptr, uint32_t size, USART_TypeDef *USARTx);
  uint32_t send_buf(uint8_t *data_ptr, uint32_t size, USART_TypeDef *USARTx);
  int32_t rcv_char (USART_TypeDef *USARTx, uint8_t *c_ptr);
  int32_t send_char(USART_TypeDef *USARTx, uint8_t c);
  uint32_t init_usart( USART_TypeDef *USARTx );  
//void InitUsart6( void );
//void Terminal_print (const char *fmt,...);
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/







#ifdef __cplusplus
}
#endif
#endif /* _USART_TERM_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2011                           */
/*=========================================================== end_of_file ====*/
