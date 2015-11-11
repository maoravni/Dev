/*
 * crc16.h
 *
 *  Created on: 19 Jun 2013
 *      Author: maora
 */

#ifndef CRC16_H_
#define CRC16_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"
#endif
uint16_t crc16_ccitt(unsigned short sum, uint8_t *p, uint32_t len);

#endif /* CRC16_H_ */
