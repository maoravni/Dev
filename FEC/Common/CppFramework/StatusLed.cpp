/*
 * StatusLed.cpp
 *
 *  Created on: 1 бреб 2012
 *      Author: maora
 */

#include "StatusLed.h"
#include <leds.h>
#include <stdio.h>
#include <PscSubsystem.h>
#include <glcd.h>

StatusLed StatusLed::m_singletonInstance;
bool StatusLed::m_idleWasExecuted;

#define M_RESET_RATE 100
#define M_CONNECTED_RATE 1000
#define M_DISCONNECTED_RATE 250

// TODO revert changes Yossi made to toggle the LEDs.

StatusLed::StatusLed()
{
    setSpeed(E_AliveLedSpeed_Reset);
    m_idleWasExecuted = false;
}

StatusLed::~StatusLed()
{
}

void StatusLed::run()
{
	portTickType xLastFlashTime;
	xLastFlashTime = getTickCount();

	int cableId = Psc_CableId;

	while (cableId >= 5)
	{
	    led_light(ALIVE_LED);
	    delay(2500);
	    led_dark(ALIVE_LED);
	    delay(500);
	    cableId -= 5;
	}

	while (cableId > 0)
	{
        led_light(ALIVE_LED);
        delay(500);
        led_dark(ALIVE_LED);
        delay(500);
        cableId -= 1;
	}

	for (;;)
	{
	    static char freeMem[25];
	    sprintf(freeMem, "Free: %06d", xPortGetFreeHeapSize());
	    glcd_draw_string_xy(0, 24, freeMem);
	    glcd_write();
#ifdef WIN32
		printf("Led Toggle\n");
#endif
		/* Delay for half the flash period then turn the LED on. */
		delayUntil(&xLastFlashTime, m_flashRate);
//		GPIO_WriteBit(m_pPort, m_pin, m_bState ? Bit_SET : Bit_RESET);
		led_toggle(ALIVE_LED);

		/* Delay for half the flash period then turn the LED off. */
//		delayUntil(&xLastFlashTime, M_FLASH_RATE);
		//GPIO_WriteBit(m_pPort, m_pin, m_bState ? Bit_SET : Bit_RESET);
//		off(ALIVE_LED);
	}

}

void StatusLed::setSpeed(E_AliveLedSpeed speed)
{
    switch (speed)
    {
    case E_AliveLedSpeed_Reset:
        m_flashRate = M_RESET_RATE;
        break;
    case E_AliveLedSpeed_Disconnected:
        m_flashRate = M_DISCONNECTED_RATE;
        break;
    case E_AliveLedSpeed_Connected:
        m_flashRate = M_CONNECTED_RATE;
        break;
    }
}

extern "C" void taskSwitchLedIndication()
{
//    if (StatusLed::m_idleWasExecuted)
//        led_light(CPU_ACTIVITY_LED8);
//    else
        led_dark(CPU_ACTIVITY_LED8);
    StatusLed::m_idleWasExecuted = false;
}

//bool StatusLed::hardwareInit()
//{
//#ifdef IAR_EVAL_BOARD
//	STM_EVAL_LEDInit((Led_TypeDef) 0);
//	STM_EVAL_LEDInit((Led_TypeDef) 1);
//	STM_EVAL_LEDInit((Led_TypeDef) 2);
//	STM_EVAL_LEDInit((Led_TypeDef) 3);
//
//	STM_EVAL_LEDOn((Led_TypeDef) 0);
//	STM_EVAL_LEDOn((Led_TypeDef) 1);
//	STM_EVAL_LEDOn((Led_TypeDef) 2);
//	STM_EVAL_LEDOn((Led_TypeDef) 3);
//
//	STM_EVAL_LEDOff((Led_TypeDef) 0);
//	STM_EVAL_LEDOff((Led_TypeDef) 1);
//	STM_EVAL_LEDOff((Led_TypeDef) 2);
//	STM_EVAL_LEDOff((Led_TypeDef) 3);
//#endif
//#ifdef PHANTOM_BOARD
//	//on(0);
//	//on(1);
//	//on(2);
//	//on(3);
//
//	//off(0);
//    //off(1);
//    //off(2);
//    //off(3);
//#endif
//	return true;
//}
//
//void StatusLed::on(LED_ID_ET led)
//{
//#ifdef IAR_EVAL_BOARD
//    STM_EVAL_LEDOn((Led_TypeDef) led);
//#endif
//#ifdef PHANTOM_BOARD
//    PortOf[led]->BSRRH = PinOf[led];
//#endif
//}
//
//void statusLedOn(LED_ID_ET led)
//{
//#ifdef IAR_EVAL_BOARD
//    STM_EVAL_LEDOn((Led_TypeDef) led);
//#endif
//#ifdef PHANTOM_BOARD
//    PortOf[led]->BSRRH = PinOf[led];
//#endif
//}
//
//void StatusLed::off(LED_ID_ET led)
//{
//#ifdef IAR_EVAL_BOARD
//    STM_EVAL_LEDOff((Led_TypeDef) led);
//#endif
//#ifdef PHANTOM_BOARD
//    PortOf[led]->BSRRL = PinOf[led];
//#endif
//}
//
//void statusLedOff(LED_ID_ET led)
//{
//#ifdef IAR_EVAL_BOARD
//    STM_EVAL_LEDOff((Led_TypeDef) led);
//#endif
//#ifdef PHANTOM_BOARD
//    PortOf[led]->BSRRL = PinOf[led];
//#endif
//}
//
//void StatusLed::toggle(LED_ID_ET led)
//{
//#ifdef IAR_EVAL_BOARD
//    STM_EVAL_LEDToggle((Led_TypeDef) led);
//#endif
//#ifdef PHANTOM_BOARD
//    PortOf[led]->ODR ^= PinOf[led];
//#endif
//}
//
//void statusLedToggle(LED_ID_ET led)
//{
//#ifdef IAR_EVAL_BOARD
//    STM_EVAL_LEDToggle((Led_TypeDef) led);
//#endif
//#ifdef PHANTOM_BOARD
//    PortOf[led]->ODR ^= PinOf[led];
//#endif
//}
//
//void StatusLed::blink(LED_ID_ET led)
//{
//    on(led);
//    off(led);
//}
//
//
////void StatusLed::on(LED_ID_ET led)
////{
////	STM_EVAL_LEDOn((LED_ID_ET) led);
////}
//
////void StatusLed::off(LED_ID_ET led)
////{
////	STM_EVAL_LEDOff((LED_ID_ET) led);
////}
//
////StatusLed& StatusLed::GetSharedInstance()
////{
////	if (!s_sharedInstance.IsValid())
////	{
////		// Create the task.
////		s_sharedInstance.Create("Status Led", configMINIMAL_STACK_SIZE, 0);
////	}
////
////	return s_sharedInstance;
////}
////
////void StatusLed::toggle(LED_ID_ET led)
////{
////	STM_EVAL_LEDToggle((LED_ID_ET) led);
////}

