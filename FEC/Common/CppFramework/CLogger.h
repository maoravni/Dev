/*
 * CLogger.h
 *
 *  Created on: 12 бреб 2012
 *      Author: maora
 */

#ifndef CLOGGER_H_
#define CLOGGER_H_

#ifdef __cplusplus

#include <AManagedTask.h>
#include <CTcpConnectorRR.h>
#include <CUdpConnector.h>
#include <stdarg.h>
#include <CCountingSemaphore.h>
//#include <vcp_srvcs.h>
#ifdef STM32F4XX
#include <stm32f4xx.h>
#else
#include <stm32f2xx.h>
#endif
#include <string>

#define M_LOGGER_QUEUE_SIZE 0x20
#define M_LOGGER_QUEUE_MASK (M_LOGGER_QUEUE_SIZE-1)

#define M_LOGGER_MESSAGE_SIZE 50
#define M_MAX_NUMBER_OF_TASK_MASKS 50

struct T_LoggerQueueItem
{
    //char message[M_LOGGER_MESSAGE_SIZE + 1];
    RTC_TimeTypeDef timestamp;
    portTickType ticks;
    std::string message;
    std::string file;
    std::string taskName;
    int line;
    int level;
    //s16_t length;
    portBASE_TYPE taskId;
    char controllerId;
    std::string asString(bool longFormat = true);
    int asArray(char* data, int maxLength);
};

/**
 * Class for a global system logger.
 */
class CLogger: public AManagedTask
{
private:
    /**
     * Specify the shared instance of the class.
     */
    static CLogger m_singletonInstance;

    /**
     * TCP Connector
     */
    CTcpConnectorRR m_tcpConnector;

    /**
     * UDP Connector;
     */
    CUdpConnector m_udpConnector;

    /**
     * The receive queue of the tcp connector, for configuring the logger.
     */
    CQueue m_tcpReceiveQueue;

    /**
     * Queue for incoming messages, before they're sent.
     */
    CQueue m_loggerMessageQueue;

    // TODO: Remove all comments containing references to the counting semaphore.
    CBinarySemaphore m_enqueueSemaphore; //!< Semaphore for thread-safe enqueuing.

    /**
     * Queue of the messages contents.
     * This queue is a cyclic buffer, which is used to eliminate the CQueue copying the contents to its
     * internals. The data is always allocated here, and we simply pass the current queue item pointer,
     * and then advance the current item index modulo 16 (& 0x10).
     */
    //T_LoggerQueueItem m_loggerQueueItems[M_LOGGER_QUEUE_SIZE];
    //char tempMessage[1024];
    //u16_t m_currentQueueItem;
    bool m_outputTcpEnable;    //!< enable output to TCP connections
    bool m_outputUdpEnable;
    bool m_outputUdpStringEnable;
    bool m_outputPrintfEnable;    //!< enable output to standard printf
    bool m_outputUartEnable;    //!< enable output to UART terminal
    bool m_outputUsbVcpEnable; //!< enable output to USB VCP
    bool m_outputFreeHeap; //!< enable periodic output of the free heap.
    bool m_outputSumLog; //!< enable output of logs to the SUM.
    bool m_outputLongFormat; //!< Output log in long or short format.

    bool m_logStack;
    bool m_logHeap;
    bool m_logTasks;
    u32_t m_lastFreeHeap;

    char m_controllerId;

    /**
     * Log Mask for each of the tasks: each task can have a different log level.
     */
    char m_taskMask[M_MAX_NUMBER_OF_TASK_MASKS];

public:
    CLogger();
    virtual ~CLogger();

    /**
     * Start the AuxTerm task and get a reference to the shared instance.
     *
     * @return a reference to the shared instance.
     */
    static CLogger &getInstance();

    /**
     * Logger main task
     */
    void run();

    /**
     * Enable logger output to standard printf.
     * On the board this translates to semihosting/SWO.
     * @param enable
     */
    void enableOutputStandardPrintf(bool enable);

    /**
     * Enable logger output to TCP/IP port 22
     * @param enable
     */
    void enableOutputTcp(bool enable);

    /**
     * Enable logger output to UDP, port 1111
     */
    void enableOutputUdp(bool enable);

    /**
     * Enable logger output to UDP as string, port 1111
     */
    void enableOutputUdpString(bool enable);

    /**
     * Update the logger UDP IP Address:
     */
    void updateOutputUdpIpAddress(ip_addr& ipAddress);
    /**
     * Enable logger output to UART.
     * @param enable
     */
    void enableOutputUart(bool enable);

    /**
     * Enable logger output to USB VCP
     */
    void enableOutputUsbVcp(bool enable);

    /**
     * Log message to the SUM
     * @param enable
     */
    void enableOutputSum(bool enable);

    /**
     * Enable periodic output of the free heap
     * @param enable
     */
    void enableLogFreeHeap(bool enable);

    void enableLongFormat(bool l)
    {
        m_outputLongFormat = l;
    }

    /**
     * Enable periodic logging of tasks status
     * @param enable
     */
    void enableLogTasks(bool enable);

    /**
     * printf-like function for logging.
     */
//void printfMessage(char* format, ...);
    /**
     * Standard log format printer. Prints time stamp, task name, etc.
     * @param format
     */
    void logfMessage(int level, const char* file, int line, const char* taskName, char* format, ...);
    //void logfMessage(int level, const char* file, int line, const char* taskName, char* format, va_list args);
    void logMessage(int level, const char* file, int line, const char* taskName, char* message);

    /**
     * vprintf-like function for logging.
     * @param format
     * @param args
     */
//void vprintfMessage(char* format, va_list args);
    /**
     * simple string message logging.
     * @param message
     */
//void printMessage(char* message);
    /**
     * simple string message logging.
     * @param message
     * @param length
     */
//void printMessage(char* message, u16_t length);
    /**
     * Print the current time stamp to the log.
     */
//void addCurrentTimeStamp();
    /**
     * Perform hardware and sub-task initializations.
     * @return true if successful.
     */
    bool hardwareInit();

    /**
     * set the debug level to display for the current task.
     * @param level
     */
    void setCurrentTaskMask(char level);

    /**
     * set the debug level to display for all tasks.
     * @param level
     */
    void setAllTaskMask(char level);
//    /**
//     * The framework calls this function before the task is created to give a chance to a managed task to perform its
//     * own initialization code and stop the task creation process if needed.
//     *
//     * @param pcName specifies the task name.
//     * @param usStackDepth specifies the task stack depth.
//     * @param uxPriority specifies the task priority.
//     *
//     * @return pdTRUE if success, pdFALSE otherwise. If the method return pdFALSE the task creation process
//     * is stopped and no FreeRTOS resource are allocated.
//     */
    portBASE_TYPE onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
            unsigned portBASE_TYPE uxPriority);

    void setControllerId(char controllerId)
    {
        m_controllerId = controllerId;
    }

    void logTaskList();
    bool checkLevel(int level)
    {
        // until we can set log level for specific tasks, we always look at index 0:
//        return (level >= m_taskMask[getCurrentTaskNumber()]);
        return (level >= m_taskMask[0]);
    }

private:
    /**
     * Post the message to the log queue for sending to the various destinations
     * @param message
     * @param length
     * @param taskId
     */
//void postMessageToLog(char* message, int length, int taskId);
    void postMessageToLog(T_LoggerQueueItem &item);

    /**
     * send a queued message to the SUM
     * @param message
     * @param length
     */
//void sendMessageToSum(std::string &message);
    int getCurrentTaskNumber()
    {
#if (configUSE_TRACE_FACILITY == 1)
        return uxTaskGetTaskNumber(xTaskGetCurrentTaskHandle());
#else
        return 0;
#endif
    }
};

#endif

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
//    void loggerClassPrintfMessage(char* format, ...);
//    void loggerClassPrintMessage(char* message);
//    void loggerClassPrintMessageLength(char* message, u16_t length);
//    void loggerClassAddCurrentTimeStamp();
void loggerClassLogfMessage(int level, const char* file, int line, const char* taskName, char* format, ...);

#ifdef __cplusplus
}
#endif // cplusplus
#endif /* CLOGGER_H_ */
