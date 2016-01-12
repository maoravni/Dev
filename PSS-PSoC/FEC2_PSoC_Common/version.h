/*
 * version.h
 *
 *  Created on: 27 Oct 2013
 */

#ifndef VERSION_H_
#define VERSION_H_

#define M_FEC_FIRMWARE_VERSION_MAJOR 1
#define M_FEC_FIRMWARE_VERSION_MINOR 0
#define M_FEC_FIRMWARE_VERSION_BUILD 0
#define M_FEC_FIRMWARE_VERSION_REVISION 201

#define M_ICD_VERSION_MAJOR 				2
#define M_ICD_VERSION_MINOR 				1
#define M_ICD_VERSION_BUILD 				0
#define M_ICD_VERSION_REVISION 				9

#define M_FEC2 1
#define M_FEC3 2
#if defined FEC2
#define M_FIRMWARE_BOARD_TYPE M_FEC2
#elif defined FEC3
#define M_FIRMWARE_BOARD_TYPE M_FEC3
#else
#error Please define board type: FEC2 or FEC3
#endif

#endif /* VERSION_H_ */
