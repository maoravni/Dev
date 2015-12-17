/******************************************************************************
 *                     (C) LandaLabs 2012                                      *
 *******************************************************************************
 *
 *  PROJECT     :
 *
 *  FILE        : ll_types.h
 *
 *  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\
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
 *  2012.07     Yossi Sarusi   1.0.0   Initial version
 *
 *******************************************************************************
 */
#ifndef _LL_TYPEDEF_H_
#define _LL_TYPEDEF_H_

#ifdef __cplusplus
extern "C"
{
#endif

#if defined STM32F4XX
#include <stm32f4xx.h>
#elif defined WIN32
#include <Win32MissingDefines.h>
#else
#include <stm32f2xx.h>
#endif
	
//#include <stdint.h>

//#ifdef __cplusplus
// extern "C" {
//#endif

/****************************************************************************
 * Include Files       NOTE: minimize nesting of header files
 ****************************************************************************/

/****************************************************************************
 * Manifest Constants
 ****************************************************************************/
#define UINT16_TO_UINT8(a)	((a&0xff))
/****************************************************************************
 * Macros
 ****************************************************************************/
#define SWAP(valu16) (((UINT16)valu16<<8) | ((UINT16)valu16>>8))

#define  ll_MAX(a,b)                ((a>b) ? (a) : (b))
#define  ll_MIN(a,b)                ((a<b) ? (a) : (b))

#define  ll_HI_U8(u16)              ((U8)(u16 >> 8))
#define  ll_LO_U8(u16)              ((U8)(u16 & 0x00FF))

#define  ll_HI_U16(u32)             ((U16)(u32 >> 16))
#define  ll_LO_U16(u32)             ((U16)(u32 & 0x0000FFFF))
#define  ll_BYTE_xx(u32,xx)         ((U8)(u32 >> ((xx-1)*8) & 0x000000FF))

#define  ll_Abs(x)                   (((x) > 0)? (x) : -(x))
/****************************************************************************
 * Types
 ****************************************************************************/
typedef int8_t INT8;
typedef int16_t INT16;
typedef int32_t INT32;

typedef uint8_t UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;

typedef unsigned   char    u8_t;
typedef signed     char    s8_t;
typedef unsigned   short   u16_t;
typedef signed     short   s16_t;
typedef unsigned   long    u32_t;
typedef signed     long    s32_t;

#ifndef __cplusplus
typedef uint8_t bool;
#endif

#ifdef __cplusplus
#define ll_ok     false
#define ll_error  true
#endif

#define LL_STATUS UINT8
#define LL_OK (LL_STATUS)0
#define LL_ERROR (LL_STATUS)-1
#ifndef __cplusplus
#define true 1
#define false 0
#endif

typedef int BOOL;
typedef int STATUS;

#define OK		0
//#define ERROR		(-1)

typedef enum
{
    OK_E = 0, NO_ROOM_E = -2, UNKNOWN_ERROR_E
} STATUS_ET;


#define   TST_STOP  0
#define   TST_RUN   1

#define FALSE		0

#define TRUE		1

#ifndef NULL
#define NULL		0
#endif

typedef int (*FUNCPTR)(); /* ptr to function returning int */

// end //to do - yossi

//void M_LOGGER_LOGF_UNSORTED(int level,const char *format ,...);

//#define M_LOGGER_LEVEL_DEBUG   1
//#define M_LOGGER_LEVEL_INFO    2
//#define FIELD_ERROR_LEVEL   3
//#define M_LOGGER_LEVEL_WARNING 4
//#define HIGH_ERROR_LEVEL    5
//#define IGOR_ERROR_LEVEL    6
//#define M_LOGGER_LEVEL_ERROR HIGH_ERROR_LEVEL
//#define M_LOGGER_LEVEL_ERROR HIGH_ERROR_LEVEL

#define TEST_PASS        1
#define TEST_FAIL        2
#define TEST_NOT_READY   0

#ifdef __cplusplus
}
#endif

#endif /* _LL_TYPEDEF_H_ */
