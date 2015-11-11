#ifndef DATA_H
#define DATA_H
#include "usbd_cdc_vcp.h"
extern uint16_t flars;
extern uint8_t msg_buff[];


#ifdef __cplusplus
extern "C"
{
#endif
uint16_t SendResponse(uint8_t* Buf, uint32_t Len);
#ifdef __cplusplus
}
#endif

#define MAX_MSG_SIZE   10
#define USB_MCG_READY  0x0001


#endif
