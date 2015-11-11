/*
 * logger.h
 *
 *  Created on: 13 бреб 2012
 *      Author: maora
 */

#ifndef LOGGER_H_
#define LOGGER_H_


//#ifdef __cplusplus
#include "CLogger.h"
//#endif
#include "FreeRTOS.h"
#include "task.h"

#define M_LOGGER_TASK_NUMBER_LOGGER 1
#define M_LOGGER_TASK_NUMBER_TMC_MASTER_SERVER 2
#define M_LOGGER_TASK_NUMBER_TMC_MESSAGE_HANDLER 3

#define M_LOGGER_CONFIG_USE_LOGGER 1

#define M_LOGGER_LEVEL_OFF 0x8
#define M_LOGGER_LEVEL_FATAL 0x6
#define M_LOGGER_LEVEL_HALT_FLAG 0x80
#define M_LOGGER_LEVEL_ERROR 0x5
#define M_LOGGER_LEVEL_WARNING 0x4
#define M_LOGGER_LEVEL_INFO 0x3
#define M_LOGGER_LEVEL_DEBUG 0x2
#define M_LOGGER_LEVEL_TRACE 0x1
#define M_LOGGER_LEVEL_VERBOSE 0x0

#define M_LOGGER_DEBUG_MIN_LEVEL M_LOGGER_LEVEL_VERBOSE

//#define M_LOGGER_LEVEL_DEBUG   M_LOGGER_LEVEL_DEBUG
//#define M_LOGGER_LEVEL_INFO    M_LOGGER_LEVEL_INFO
//#define FIELD_ERROR_LEVEL   M_LOGGER_LEVEL_FIELD
//#define M_LOGGER_LEVEL_WARNING M_LOGGER_LEVEL_WARNING
//#define HIGH_ERROR_LEVEL    M_LOGGER_LEVEL_TRACE
//#define Rclog    M_LOGGER_LEVEL_FATAL
//#define M_LOGGER_LEVEL_ERROR HIGH_ERROR_LEVEL
//#define M_LOGGER_LEVEL_ERROR HIGH_ERROR_LEVEL
//#ifndef _OLD_LOGGER_
//#define AUTO_DEBUG_MSG M_LOGGER_LOGF
//#endif

//#ifdef __cplusplus
//extern "C"
//{
//#endif

#ifdef M_LOGGER_CONFIG_USE_LOGGER

//#ifdef NDEBUG
//#define pcTaskGetTaskName(a) ""
//#endif

/**
 * Macro to log using printf-like interface. Prints also the file name and line.
 */
#ifdef __cplusplus
#define M_LOGGER_PRINTF(level, ...) \
{ \
    CLogger::getInstance().logfMessage(level, __FILE__, __LINE__, (char*)pcTaskGetTaskName(xTaskGetCurrentTaskHandle()), __VA_ARGS__); \
}
#else
#define M_LOGGER_PRINTF(level, ...) \
{ \
    loggerClassLogfMessage(level, __FILE__, __LINE__, (char*)pcTaskGetTaskName(xTaskGetCurrentTaskHandle()), __VA_ARGS__); \
}
#endif


#define M_LOGGER_LOGF(level, ...) do { \
                               if ( \
                                   ((short)(level) >= M_LOGGER_DEBUG_MIN_LEVEL)) { \
                                 M_LOGGER_PRINTF(level, __VA_ARGS__); \
                                 if ((level) & M_LOGGER_LEVEL_HALT_FLAG) { \
                                   while(1); \
                                 } \
                               } \
                             } while(0)


#else // USE LOGGER
#define M_LOGGER_PRINTF(...)

#endif // USE LOGGER
//#ifdef __cplusplus
//}
//#endif

#endif /* LOGGER_H_ */
