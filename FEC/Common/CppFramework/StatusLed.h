/*
 * StatusLed.h
 *
 *  Created on: 1 бреб 2012
 *      Author: maora
 */

#ifndef STATUSLED_H_
#define STATUSLED_H_

#ifdef __cplusplus

#include "AManagedTask.h"

#if defined STM32F4XX
#include <stm32f4xx.h>
#elif defined WIN32
#include <Win32MissingDefines.h>
#else
#include <stm32f2xx.h>
#endif

#ifdef IAR_EVAL_BOARD
#include "iar_stm32f207zg_sk.h"
#endif
//#ifdef PHANTOM_BOARD
#define LEDS_CTRL_IMPL_HERE
#include "leds.h"
//#endif

enum E_AliveLedSpeed
{
    E_AliveLedSpeed_Reset,
    E_AliveLedSpeed_Connected,
    E_AliveLedSpeed_Disconnected,
};

class StatusLed: public AManagedTask
{
public:
    static bool m_idleWasExecuted;

private:
    /**
     * Specify the shared instance of the class.
     */
    static StatusLed m_singletonInstance;
    StatusLed();

    int m_flashRate;

public:
    virtual ~StatusLed();

    /**
     * Start the LCD task and get a reference to the shared instance.
     *
     * @return a reference to the shared instance.
     */
    static StatusLed &getInstance();

    void run();

    void setSpeed(E_AliveLedSpeed speed);

//    bool hardwareInit();
//
//    static void on(LED_ID_ET led);
//    static void off(LED_ID_ET led);
//    static void toggle(LED_ID_ET led);
//    static void blink(LED_ID_ET led);
};

inline StatusLed& StatusLed::getInstance()
{
    if (!m_singletonInstance.isValid())
    {
        // Create the task.
        m_singletonInstance.create("Status Led", configMINIMAL_STACK_SIZE, 0);
    }

    return m_singletonInstance;
}

#endif //__cplusplus

#ifdef __cplusplus
extern "C"
#endif
void taskSwitchLedIndication();

//extern "C"
//{
//
//#define led_toggle(a) statusLedToggle(a)
//#define led_dark(a) statusLedOff(a)
//
//void statusLedOn(LED_ID_ET led);
//
//void statusLedOff(LED_ID_ET led);
//
//void statusLedToggle(LED_ID_ET led);
//
//#ifdef __cplusplus
//}
//#endif // cplusplus

#endif /* STATUSLED_H_ */
