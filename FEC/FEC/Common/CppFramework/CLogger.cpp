/*
 * CLogger.cpp
 *
 *  Created on: 12 бреб 2012
 *      Author: maora
 */

#include "CLogger.h"
#include <string.h>
#include <stdarg.h>
//#include "StatusLed.h"
#include <stdio.h>
#include <stdlib.h>
#include "board_setup.h"
#include "usart.h"
#include "rtc.h"
#include "logger.h"
#include "CBinarySemaphore.h"
//#ifdef NEW_FEC
//#include <PscServer/MessageStructs.h>
//#else
//#include <TmcServer/TmcMasterServer.h>
//#endif
#include <iostream>
#include <lwip/netif.h>
#include <dg_in_ctrl.h>
#include <lwip/stats.h>

CLogger CLogger::m_singletonInstance;

#define M_LOGGER_DEFAULT_TCP_PORT 22
#define M_LOGGER_DEFAULT_UDP_BASE_PORT 1111

#define M_LOGGER_DEFAULT_SUBTASK_STACK_SIZE 600
#define M_LOGGER_DEFAULT_SUBTASK_PRIORITY 0

#define M_LOGGER_PERIODIC_HEAP_UPDATE_DELAY 1000
#define M_LOGGER_PERIODIC_TASK_LIST_UPDATE_DELAY 10000

//#define M_LOGGER_SEMAPHORE_DEQUEUE_PERIOD 100
//#define M_LOGGER_SEMAPHORE_ENQUEUE_PERIOD 100
#define M_LOGGER_MESSAGE_ENQUEUE_PERIOD 50
uint32_t clogger_stack_wm;

static const char CLoggerLevelString[10][10] =
{ "VERBOSE", "TRACE", "DEBUG", "INFO", "WARNING", "ERROR", "FATAL", "DUMMY", "OFF" };

CLogger::CLogger() :
        m_tcpConnector(M_LOGGER_DEFAULT_TCP_PORT), m_udpConnector(M_LOGGER_DEFAULT_UDP_BASE_PORT)
{
    m_outputTcpEnable = false;
    m_outputUdpEnable = false;
    m_outputPrintfEnable = false;
    m_outputUartEnable = false;
    m_outputUsbVcpEnable = false;
    m_outputSumLog = false;
    m_outputLongFormat = true;
    //m_currentQueueItem = 0;

    m_logStack = false;
    m_logHeap = false;
    m_logTasks = false;
    m_lastFreeHeap = 0;

    // TODO: Implement task masking based on level.
    setAllTaskMask(M_LOGGER_LEVEL_INFO);

}

CLogger::~CLogger()
{
}

CLogger& CLogger::getInstance()
{
    if (!m_singletonInstance.isValid())
    {
        // Create the task.
        m_singletonInstance.create("CLogger", M_LOGGER_DEFAULT_SUBTASK_STACK_SIZE, 1);
    }

    return m_singletonInstance;
}

void CLogger::run()
{
    //vTaskSetTaskNumber(xTaskGetCurrentTaskHandle(), M_LOGGER_TASK_NUMBER_LOGGER);

    //u16_t queueItemIndex;
    std::string msgToSend;
    int msgLength;
    netbuf* pNetBuf;
    portTickType previousHeapTickCount = 0;
    portTickType previousTaskTickCount = 0;
    portTickType currentTickCount = 0;

    u32 freeHeap;

    // run "enable outputs" again, to create the sub-tasks when we first get here.
    enableOutputTcp(m_outputTcpEnable);
    enableOutputUdp(m_outputUdpEnable);
    enableOutputStandardPrintf(m_outputPrintfEnable);
    enableOutputUart(m_outputUartEnable);
    enableOutputUsbVcp(m_outputUsbVcpEnable);

    //setCurrentTaskMask(1);

    T_LoggerQueueItem *pQueueItem;

    for (;;)
    {
#ifdef USE_IWDG
        // reset the watchdog:
        IWDG_ReloadCounter();
#endif

#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )
//        clogger_stack_wm = uxTaskGetStackHighWaterMark(0);
#endif
        if (m_outputTcpEnable && m_tcpReceiveQueue.receive(&pNetBuf, 0) == pdPASS)
        {
            netbuf_delete(pNetBuf);
        }

        static char message[TCP_MSS];
        // wait for a specified delay until we get a message in the queue.
        while (m_loggerMessageQueue.receive(&pQueueItem, 100) == pdPASS)
        {
            //if (m_taskMask[pQueueItem->taskId] > 0)
            {
                msgToSend = pQueueItem->asString(m_outputLongFormat);
                msgLength = msgToSend.length();

                if (m_outputTcpEnable/* && m_tcpConnector.isConnected()*/)
                {
                    //m_tcpConnector.send(queueItem.message, queueItem.length);
                    //puts(msgToSend.c_str());
                    m_tcpConnector.send((void*) msgToSend.c_str(), msgLength);
                }
//                if (m_outputPrintfEnable)
//                {
//                    printf("%s", queueItem.message);
//                }
                if (m_outputUartEnable)
                {
                    send_buf((uint8_t*) msgToSend.c_str(), msgLength, USART6 );
                }
//                if (m_outputUsbVcpEnable)
//                {
//                    SendResponse((uint8_t*) msgToSend.c_str(), msgLength);
//                }
//                if (m_outputSumLog)
//                    //sendMessageToSum(queueItem.message);
//                    TmcMasterServer::getInstance().sendLogMessage(msgToSend);
                if (m_outputUdpEnable)
                {
                    msgLength = pQueueItem->asArray(message, TCP_MSS);
                    m_udpConnector.send(message, msgLength);
                }
                if (m_outputUdpStringEnable)
                {
                    m_udpConnector.send(msgToSend.c_str(), msgLength);
                    m_udpConnector.send("\n\r", 2);
                }
            }
            delete pQueueItem;
//            yield();
        }
        //StatusLed::getInstance().toggle(LED_UART_TX);
        // check if the update period have elapsed:
        currentTickCount = getTickCount();

        if (m_logHeap)
        {
            if ((currentTickCount - previousHeapTickCount) >= M_LOGGER_PERIODIC_HEAP_UPDATE_DELAY)
            {
                previousHeapTickCount = currentTickCount;
                freeHeap = xPortGetFreeHeapSize();
                if (freeHeap != m_lastFreeHeap)
                {
                    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Free Heap: %d", xPortGetFreeHeapSize());
                    m_lastFreeHeap = freeHeap;
                }
#if (LWIP_STATS == 1)
                M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "LWIP PBUFS: used:%d max:%d avail:%d",
                        lwip_stats.memp[MEMP_PBUF_POOL].used, lwip_stats.memp[MEMP_PBUF_POOL].max,
                        lwip_stats.memp[MEMP_PBUF_POOL].avail);
                M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "LWIP MEM: used:%d max:%d avail:%d", lwip_stats.mem.used,
                        lwip_stats.mem.max, lwip_stats.mem.avail);
#endif
            }
        }
        if (m_logTasks)
        {
            if ((currentTickCount - previousTaskTickCount) >= M_LOGGER_PERIODIC_TASK_LIST_UPDATE_DELAY)
            {
                previousTaskTickCount = currentTickCount;
                logTaskList();
//                previousTaskTickCount = currentTickCount;
//                static char taskListMessage[500];
//                int taskListMessageLength;
//                getTaskList(taskListMessage);
//                taskListMessageLength = strlen(taskListMessage);
//                int currentIndex = 0;
//                M_LOGGER_LOGF(M_LOGGER_LEVEL_VERBOSE, "\n%s", taskListMessage);
//                for (currentIndex = 0; currentIndex < taskListMessageLength; currentIndex += M_LOGGER_MESSAGE_SIZE)
//                {
//                    printMessage(taskListMessage + currentIndex,
//                            ((taskListMessageLength - currentIndex > M_LOGGER_MESSAGE_SIZE) ?
//                                    M_LOGGER_MESSAGE_SIZE : (taskListMessageLength - currentIndex)));
//                }
            }
        }
//        delay(10);
//        yield();
    }
}

void CLogger::enableOutputStandardPrintf(bool enable)
{
    m_outputPrintfEnable = enable;
}

void CLogger::enableOutputTcp(bool enable)
{
//    m_outputTcpEnable = enable;
//    if (isValid())
//    {
//        if (m_outputTcpEnable)
//        {
//            if (!m_tcpConnector.isValid())
//            {
//                if ((m_tcpConnector.create("Logger TCP", M_LOGGER_DEFAULT_SUBTASK_STACK_SIZE)) != pdPASS)
//                    m_outputTcpEnable = false;
//                // attch the TCP queue handle to the local copy of the queue.
//                m_tcpReceiveQueue.attach(m_tcpConnector.getReceiveQueueHandle());
//            }
//            else
//            {
//                m_tcpConnector.resume();
//            }
//        }
//        else
//        {
//            if (m_tcpConnector.isValid())
//                m_tcpConnector.suspend();
//        }
//    }
}

void CLogger::enableOutputUart(bool enable)
{
    m_outputUartEnable = enable;
}

//void CLogger::vprintfMessage(char* format, va_list args)
//{
//    char tempMessage[512];
//    int tempLength;
//
//    // take the semaphore, so that updating the local queue is atomic.
//    tempLength = vsprintf(tempMessage, format, args);
//
//    postMessageToLog(tempMessage, tempLength, getCurrentTaskNumber());
//}

//void CLogger::logfMessage(int level, const char* file, int line, const char* taskName, char* format, va_list args)
//{
//    if (level < m_taskMask[getCurrentTaskNumber()])
//        return;
//
//    T_LoggerQueueItem queueItem;
//    char tempMessage[500];
//
//    // get the current time.
//    rtc_get_time(&queueItem.timestamp);
//    queueItem.ticks = xTaskGetTickCountFromISR();
//    queueItem.level = level;
//    queueItem.file = std::string(file);
//    queueItem.line = line;
//    queueItem.taskName = std::string(taskName);
//    queueItem.taskId = getCurrentTaskNumber();
//    queueItem.controlledId = m_controlledId;
//
//    vsprintf(tempMessage, format, args);
//    queueItem.message = std::string(tempMessage);
//
//    postMessageToLog(queueItem);
//}

int getTaskCounter = 0;
void CLogger::logfMessage(int level, const char* file, int line, const char* taskName, char* format, ...)
{
//    printf("Current Task Number %d: %x\n", getTaskCounter++, getCurrentTaskNumber());
    assert(getCurrentTaskNumber() < M_MAX_NUMBER_OF_TASK_MASKS);

//    if (level < m_taskMask[getCurrentTaskNumber()])
//        return;
    // until we can set log level for specific tasks, we always look at index 0:
    if (level < m_taskMask[0])
        return;

    T_LoggerQueueItem queueItem;
    va_list args;
    char tempMessage[500];

    // get the current time.
    rtc_get_time(&queueItem.timestamp);
    queueItem.ticks = xTaskGetTickCountFromISR();
    queueItem.level = level;
    queueItem.file = std::string(file);
    queueItem.line = line;
    queueItem.taskName = std::string(taskName);
    queueItem.taskId = getCurrentTaskNumber();
    queueItem.controllerId = m_controllerId;

    va_start(args, format);
    vsprintf(tempMessage, format, args);
    va_end(args);

    queueItem.message = std::string(tempMessage);

    postMessageToLog(queueItem);
}

//void CLogger::printfMessage(char* format, ...)
//{
//    va_list args;
//
//    char message[500];
//    int length;
//
//    va_start(args, format);
//    length = vsprintf(message, format, args);
//    va_end(args);
//
//    postMessageToLog(message, length, getCurrentTaskNumber());
//}

//void CLogger::printMessage(char* message)
//{
//    int length;
//    // copy the message to the queue
//    length = strlen(message);
//
//    postMessageToLog(message, length, getCurrentTaskNumber());
//}

//void CLogger::printMessage(char* message, u16_t length)
//{
//    postMessageToLog(message, length, getCurrentTaskNumber());
//}

void CLogger::enableLogFreeHeap(bool enable)
{
    m_logHeap = enable;
}

void CLogger::enableLogTasks(bool enable)
{
    m_logTasks = enable;
}

//void CLogger::addCurrentTimeStamp()
//{
//    RTC_TimeTypeDef RTC_Time;
//    rtc_get_time(&RTC_Time);
//    printfMessage("%d:%d:%d.%d ", RTC_Time.RTC_Hours, RTC_Time.RTC_Minutes, RTC_Time.RTC_Seconds,
//            xTaskGetTickCountFromISR());
//}

portBASE_TYPE CLogger::onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth,
        unsigned portBASE_TYPE uxPriority)
{
    // initialize the message queue:
    if (m_loggerMessageQueue.create(M_LOGGER_QUEUE_SIZE, sizeof(T_LoggerQueueItem*)) == NULL)
        return false;

    if (m_enqueueSemaphore.create() != true)
        return false;

    return true;
}

bool CLogger::hardwareInit()
{
    portBASE_TYPE res;

// initialize UART
//    stm32f2_usart_cnfg();

    return true;
}

//void loggerClassPrintfMessage(char* format, ...)
//{
//    va_list args;
//
//// take the semaphore, so that updating the local queue is atomic.
//    va_start(args, format);
//    CLogger::getInstance().vprintfMessage(format, args);
//    va_end(args);
//}

//void loggerClassPrintMessage(char* message)
//{
//    CLogger::getInstance().printMessage(message);
//}

//void loggerClassPrintMessageLength(char* message, u16_t length)
//{
//    CLogger::getInstance().printMessage(message, length);
//}

//void loggerClassAddCurrentTimeStamp()
//{
//    CLogger::getInstance().addCurrentTimeStamp();
//}

void CLogger::setCurrentTaskMask(char level)
{
    // until we can set log level for specific tasks, we always look at index 0:
    setAllTaskMask(level);
//#ifndef NDEBUG
////#if (configUSE_TRACE_FACILITY == 1)
////    m_taskMask[uxTaskGetTaskNumber(xTaskGetCurrentTaskHandle())] = level;
////#else
//    m_taskMask[0] = level;
////#endif
//#endif
}

void CLogger::setAllTaskMask(char level)
{
    for (int i = 0; i < M_MAX_NUMBER_OF_TASK_MASKS; ++i)
        m_taskMask[i] = level;
}

void loggerClassLogfMessage(int level, const char* file, int line, const char* taskName, char* format, ...)
{
    va_list args;
    char tempMessage[500];

    va_start(args, format);
    vsprintf(tempMessage, format, args);
    va_end(args);

    CLogger::getInstance().logMessage(level, file, line, taskName, tempMessage);
}

void CLogger::logMessage(int level, const char* file, int line, const char* taskName, char* message)
{
    assert(getCurrentTaskNumber() < M_MAX_NUMBER_OF_TASK_MASKS);
    // until we can set log level for specific tasks, we always look at index 0:
//    if (level < m_taskMask[getCurrentTaskNumber()])
//        return;
    if (level < m_taskMask[0])
        return;

    T_LoggerQueueItem queueItem;

    // get the current time.
    rtc_get_time(&queueItem.timestamp);
    queueItem.ticks = xTaskGetTickCountFromISR();
    queueItem.level = level;
    queueItem.file = std::string(file);
    queueItem.line = line;
    queueItem.taskName = std::string(taskName);
    queueItem.taskId = getCurrentTaskNumber();
    queueItem.message = std::string(message);
    queueItem.controllerId = m_controllerId;

    postMessageToLog(queueItem);
}

void CLogger::enableOutputUdpString(bool enable)
{
    enableOutputUdp(enable);
    m_outputUdpEnable = false;
    m_outputUdpStringEnable = enable;
}

void CLogger::enableOutputUdp(bool enable)
{
    m_outputUdpEnable = enable;
    m_outputUdpStringEnable = false;
    if (isValid())
    {
        if (m_outputUdpEnable)
        {
            if (!m_udpConnector.isValid())
            {
                if ((m_udpConnector.create("Logger UDP", M_LOGGER_DEFAULT_SUBTASK_STACK_SIZE,
                        M_LOGGER_DEFAULT_SUBTASK_PRIORITY)) != pdPASS)
                    m_outputUdpEnable = false;
                //m_udpConnector.setPort(M_LOGGER_DEFAULT_UDP_BASE_PORT+(Get_SSID_dig()<<4)+nod_id_get());

                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Logger UDP Output Enabled");
            }
            else
            {
                m_udpConnector.resume();
            }
//            m_udpConnector.setIpAddress(ipAddress);
        }
        else
        {
            if (m_udpConnector.isValid())
                m_udpConnector.suspend();
        }
    }
}

void CLogger::logTaskList()
{
    enterCritical();
    static char taskListMessage[500];
    int taskListMessageLength;
    getTaskList(taskListMessage);
    taskListMessageLength = strlen(taskListMessage);
    int currentIndex = 0;
    exitCritical();
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "\n%s", taskListMessage);
}

void CLogger::updateOutputUdpIpAddress(ip_addr& ipAddress)
{
    m_udpConnector.setIpAddress(ipAddress);
}

void CLogger::postMessageToLog(T_LoggerQueueItem &queueItem)
{
    int currentPosition = 0;
    //T_LoggerQueueItem queueItem;

    // wait until the sending is finished.
    if (m_outputPrintfEnable)
        printf("%s", queueItem.asString(m_outputLongFormat).c_str());
    if (m_enqueueSemaphore.take(0) != pdPASS)
        return;
    //queueItem.message = message;
    //queueItem.taskId = taskId;
    T_LoggerQueueItem *pqi = new T_LoggerQueueItem();
    *pqi = queueItem;
    portBASE_TYPE enqueueResult = m_loggerMessageQueue.sendToBack(&pqi, M_LOGGER_MESSAGE_ENQUEUE_PERIOD);

    if (enqueueResult == pdFALSE)
        delete pqi;
//    while (length >= 0)
//    {
//        queueItem.taskId = taskId;
//        queueItem.length = (length < M_LOGGER_MESSAGE_SIZE) ? length : M_LOGGER_MESSAGE_SIZE;
//        strncpy(queueItem.message, message + currentPosition, M_LOGGER_MESSAGE_SIZE);
//        // make sure a NULL is at the end of the string.
//        queueItem.message[queueItem.length] = 0;
//        // queue the message for sending:
//        if (m_loggerMessageQueue.sendToBack(&queueItem, M_LOGGER_MESSAGE_ENQUEUE_PERIOD) == pdPASS)
//        {
//            if (m_outputPrintfEnable)
//            {
//                printf("%s", queueItem.message);
//            }
//            // advance the queue pointer
//            //advanceCurrentQueueItem();
//        }
//        else
//        {
//            //printf("Enqueue failed!\n");
//        }
//        length -= M_LOGGER_MESSAGE_SIZE;
//        currentPosition += M_LOGGER_MESSAGE_SIZE;
//    }
    // give back the semaphore.
    m_enqueueSemaphore.give();
}

void CLogger::enableOutputUsbVcp(bool enable)
{
    m_outputUsbVcpEnable = enable;
}

void CLogger::enableOutputSum(bool enable)
{
    m_outputSumLog = enable;
}

std::string T_LoggerQueueItem::asString(bool longFormat)
{
    char tempMessage[500];
    int tempLength;

    if (longFormat)
    {
        tempLength = snprintf(tempMessage, 500, "%d:%d:%d.%d <%s> %s:%s:%d - %s\n", timestamp.RTC_Hours,
                timestamp.RTC_Minutes, timestamp.RTC_Seconds, ticks, CLoggerLevelString[level & 0xf], taskName.c_str(),
                file.c_str(), line, message.c_str());
    }
    else
    {
        tempLength = snprintf(tempMessage, 500, "%d:%d:%d.%d <%s> - %s\n", timestamp.RTC_Hours, timestamp.RTC_Minutes,
                timestamp.RTC_Seconds, ticks, CLoggerLevelString[level & 0xf], message.c_str());
    }
    tempMessage[tempLength] = 0;

    return std::string(tempMessage);
}

int T_LoggerQueueItem::asArray(char* data, int maxLength)
{
    U16 msgLength;
    U16 fileNameLength;
    U16 taskNameLength;
    int currentIndex;
    // add controller id to the log.
    data[0] = 0;
    data[1] = timestamp.RTC_Hours;
    data[2] = timestamp.RTC_Minutes;
    data[3] = timestamp.RTC_Seconds;
    *(U32*) (data + 4) = ticks;

    *(U32*) (data + 8) = line;
    *(U32*) (data + 12) = level;
    *(U32*) (data + 16) = taskId;

    fileNameLength = file.length();
    taskNameLength = taskName.length();
    msgLength = message.length();

    *(U16*) (data + 20) = fileNameLength;
    *(U16*) (data + 22) = taskNameLength;
    *(U16*) (data + 24) = msgLength;

    currentIndex = 26;
    memcpy(data + currentIndex, file.c_str(), fileNameLength);
    currentIndex += fileNameLength;
    memcpy(data + currentIndex, taskName.c_str(), taskNameLength);
    currentIndex += taskNameLength;

    // perform a check that the message size does not exceed the max length:
    msgLength = ((currentIndex + msgLength) > maxLength) ? (maxLength - currentIndex) : msgLength;
    memcpy(data + currentIndex, message.c_str(), msgLength);
    currentIndex += msgLength;
    return currentIndex;
}
