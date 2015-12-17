/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     :  BSP_LIB
*
*  FILE        :  rtc.c
*
*  PATH        :  C:\Landalabs\Spitfire\Embedded\Projects\Common\Libraries\BSP
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
*  2012.09     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
*      Copyright and Proprietary Statment.
*
*******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
//#include "stm32f2xx.h"
#include <string.h>

//#include "stm322xg_eval.h"
#include <stdio.h>
#include <stdint.h>
//#include "ll_typedef.h"
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
//#include <stm32f2xx.h>
#include "rtc.h"
#ifdef CPP_FRAMEWORK
#include <logger.h>
#endif
/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */
/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */
/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
uint32_t rtc_date_time_update();
void rtc_config();
uint32_t rtc_set_time();
uint32_t rtc_set_date();
void rtc_get_date(RTC_DateTypeDef *cur_date_ptr);
void rtc_get_time(RTC_TimeTypeDef *cur_time_ptr);
/* --- PRIVATE_DATA --------------------------------------------------------- */
//RTC_InitTypeDef RTC_InitStructure;
//RTC_TimeTypeDef RTC_TimeStructure;
//RTC_DateTypeDef RTC_DateStructure;
//RTC_TimeTypeDef  RTC_TimeStampStructure;
//RTC_DateTypeDef  RTC_TimeStampDateStructure;
//__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;
//static uint32_t sec_set     = 0;
//static uint32_t minutes_set = 0;
//static uint32_t hours_set   = 0;
//static uint32_t day_set     = 0;
//static uint32_t month_set   = 0;
//static uint32_t year_set    = 0;
#define M_RTC_CONFIG_TIMEOUT 100000

/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
/* Uncomment the corresponding line to select the RTC Clock source */
#define RTC_CLOCK_SOURCE_LSE   /* LSE used as RTC source clock */
/* #define RTC_CLOCK_SOURCE_LSI */ /* LSI used as RTC source clock. The RTC Clock may varies due to LSI frequency dispersion. */
/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
/**
  * @brief: builds a current time and/or date string to be displayed on terminal.
  *
  * @param:
  *
  * @retval:
  **/
char* rtc_show_time_date(char *tmp_buf, TIME_DATE_SHOW_CTRL_ET show_ctrl){
	//int32_t   date_len = 0, len;
	//
	//memset(tmp_buf, 0x5A, 50);//DBG

	//switch(show_ctrl){
	//  case DATE_AND_TIME:
	//    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);/* Get the current Date */
	//    date_len = sprintf(tmp_buf, "%0.2d/%0.2d/%0.2d  ", RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Year);        
	//  case TIME_ONLY:
	//    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);/* Get the current Time  */
	//    len = sprintf(tmp_buf + date_len,"%0.2d:%0.2d:%0.2d", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
	//    break;        

	//}/* switch(show_ctrl) */

	//return tmp_buf;
	return "";
}/* rtc_show_time_date */

/**
* @brief: Sets a year values.
*
* @param: the data received from a remoute host.
*
* @retval: 0 - if success;
**/
uint32_t rtc_set_year_val(uint32_t data){
	//uint32_t res = ~OK;

	//year_set = 0;

	//if (data < 100){
	//	RTC_DateStructure.RTC_Year = (uint8_t)data;
	//	year_set = 1;
	//	res = OK;
	//}

	//res = rtc_date_time_update();
	//if (res == OK)
	//	sec_set = minutes_set = hours_set = day_set = month_set = year_set = 0;

	//return res;
	return 0;
}/***********************   rtc_set_year_vals   *****************************/

/**
* @brief: Sets a day and month values.
*
* @param: the data received from a remoute host.
*
* @retval: 0 - if success;
**/
uint32_t rtc_set_day_month_vals(uint32_t data){
	//uint32_t res = ~OK;
	//uint32_t tmp;

	//day_set = month_set = 0;

	///* Bits 15 - 8: Month; Bits 7 - 0: Day */

	//tmp = data & 0x000000FF;/* Get day value */
	//if (tmp < 32){/* very simple input check */
	//	day_set = 1;
	//	RTC_DateStructure.RTC_Date = (uint8_t)tmp;
	//}
	//tmp = data >> 8;/* Get Month value */
	//if (tmp < 13){
	//	month_set = 1;
	//	RTC_DateStructure.RTC_Month = (uint8_t)tmp;
	//}
	//if ((day_set == 1) && (month_set == 1))
	//	res = OK;

	//res = rtc_date_time_update();

	//if (res == OK){
	//	sec_set = minutes_set = hours_set = day_set = month_set = year_set = 0;
	//}
	//return res;
	return 0;
}/***************   rtc_set_day_month_vals   ****************************/

/**
* @brief: Sets a minutes and hours values.
*
* @param: the data received from a remoute host.
*
* @retval: 0 - if success;
**/
uint32_t rtc_set_hours_minutes_vals(uint32_t data){
	//uint32_t res = ~OK;
	//uint32_t tmp;

	///* Bits 15 - 8: Hours; Bits 7 - 0: Minutes */

	//tmp = data & 0x000000FF;/* Get minutes value */
	//if (tmp < 60){
	//	minutes_set = 1;
	//	RTC_TimeStructure.RTC_Minutes = (uint8_t)tmp;
	//}
	//tmp = data >> 8;/* Get hours value */
	//if (tmp < 24){
	//	hours_set = 1;
	//	RTC_TimeStructure.RTC_Hours = (uint8_t)tmp;
	//}
	//if ((minutes_set == 1) && (hours_set == 1))
	//	res = OK;

	//res = rtc_date_time_update();
	//if (res == OK)
	//	sec_set = minutes_set = hours_set = day_set = month_set = year_set = 0;

	//return res;
	return 0;
}/***************   rtc_set_hours_minutes_vals   ****************************/

/**
* @brief: Sets a seconds value.
*
* @param: the data received from a remoute host.
*
* @retval: 0 - if success;
**/
uint32_t rtc_set_seconds_val(uint32_t data){
	//uint32_t res = ~OK;
	//sec_set = 0;

	//data = data >> 10;/* Bits 15 - 9: Sec;   Bits 9 - 0: mSec    */
	//if (data < 60){
	//	sec_set = 1;
	//	res = OK;
	//	RTC_TimeStructure.RTC_Seconds = (uint8_t)data;
	//}

	//res = rtc_date_time_update();
	//if (res == OK)
	//	sec_set = minutes_set = hours_set = day_set = month_set = year_set = 0;

	//return res;
	return 0;
}/***************   rtc_set_seconds_val   ***********************************/

/**
  * @brief  Display the current time on a Terminal.
  * @param  if NULL current date value will displaed on terminal.
  * @retval: 0 - if success;
  */
void rtc_get_time(RTC_TimeTypeDef *cur_time_ptr){

	//if (cur_time_ptr == 0){
	//	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);/* Get the current Time  */
	//	//M_LOGGER_LOGF(IGOR_ERROR_LEVEL,">>Time :  %0.2d:%0.2d:%0.2d<<", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
	//}
	//else{
	//	RTC_GetTime(RTC_Format_BIN, cur_time_ptr);/* Get the current Time  */
	//}

	//(void)RTC->DR;/* Unfreeze the RTC DR Register */

}/***************************   rtc_get_time   ******************************/

/**
  * @brief  Display the current date on a Terminal.
  * @param  if NULL current date value will displaed on terminal.
  * @retval None
  */
void rtc_get_date(RTC_DateTypeDef *cur_date_ptr){

	//if (cur_date_ptr == 0){
	//	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);/* Get the current Date */
	//	//M_LOGGER_LOGF(IGOR_ERROR_LEVEL,">>Date: %0.2d/%0.2d/%0.2d <<", RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Year);
	//}
	//else{
	//	RTC_GetDate(RTC_Format_BIN, cur_date_ptr);/* Get the current Time  */
	//}
}/***************************   rtc_get_date   ******************************/

/**
  * @brief  Initialize the real time clock at system starting.
  * @param  None
  * @retval None
  */
void rtc_init(){

	//if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2){
	//	rtc_config();/* RTC configuration  */
	//	/* Configure the RTC data register and RTC prescaler */
	//	RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
	//	RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
	//	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	//	/* Check on RTC init */
	//	if (RTC_Init(&RTC_InitStructure) == ERROR){
	//		M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, ">> !! RTC Prescaler Config failed. !! <<");
	//	}
	//}
	//else
	//{
	//	/* Check if the Power On Reset flag is set */
	//	if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET){
	//		M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, ">> !! Power On Reset occurred.... !! <<");
	//	}
	//	/* Check if the Pin Reset flag is set */
	//	else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET){
	//		M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, ">> !! External Reset occurred.... !! <<");
	//	}

	//	M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, ">> !! No need to configure RTC.... !! <<");




	//	/* Display the RTC Time/Date and TimeStamp Time/Date */
	//	rtc_get_time(0);
	//	rtc_get_date(0);
	//}
}/*****************************   rtc_init   ********************************/
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
/**
* @brief: init & config the Wakeup unit
*
* @param:
*
* @retval:
**/
void rtc_wakeup_init(){
	//NVIC_InitTypeDef NVIC_InitStructure;
	//EXTI_InitTypeDef EXTI_InitStructure;

	///* Enable the PWR clock */
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	///* Allow access to RTC */
	//PWR_BackupAccessCmd(ENABLE);

	///* Wait for RTC APB registers synchronisation */
	//RTC_WaitForSynchro();

	///* EXTI configuration *******************************************************/
	//EXTI_ClearITPendingBit(EXTI_Line22);
	//EXTI_InitStructure.EXTI_Line = EXTI_Line22;
	//EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	//EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	//EXTI_Init(&EXTI_InitStructure);

	///* RTC Wake Up Interrupt cnfg */
	///* Enable the RTC Wakeup Interrupt */
	//EXTI_ClearITPendingBit(EXTI_Line22);
	//NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (uint8_t)(configMAX_SYSCALL_INTERRUPT_PRIORITY >> 4) + 3;  //mast be above configMAX_SYSCALL_INTERRUPT_PRIORITY (11)
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);

	//RTC_WakeUpCmd(DISABLE);
	//RTC_ITConfig(RTC_IT_WUT, DISABLE);

	///* Configure the RTC WakeUp Clock source: CK_SPRE (1Hz) */
	//RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);/* RTC_WakeUpClock_RTCCLK_Div2 */
	//RTC_SetWakeUpCounter(0xfafd);

	///* Enable the RTC Wakeup Interrupt */
	//RTC_ITConfig(RTC_IT_WUT, ENABLE);

	///* Enable Wakeup Counter */
	//RTC_WakeUpCmd(ENABLE);


	//RTC_ClearFlag(RTC_FLAG_WUTF);
}/************************** rtc_wakeup_init ********************************/

/**
  * @brief  This function handles RTC Wakeup global interrupt request.
  * @param  None
  * @retval None
  */
void RTC_WKUP_IRQHandler(void){

	//if (RTC_GetITStatus(RTC_IT_WUT) != RESET){
	//	RTC_ClearITPendingBit(RTC_IT_WUT);
	//	EXTI_ClearITPendingBit(EXTI_Line22);
	//}
}/**************************** RTC_WKUP_IRQHandler **************************/

/**
* @brief  Configure the RTC peripheral by selecting the clock source.
* @param  None
* @retval None
*/
void rtc_config(){

//	uint32_t RtcConfigTimeout = 0;
//
//	/* Enable the PWR clock */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
//
//	/* Allow access to RTC */
//	PWR_BackupAccessCmd(ENABLE);
//
//#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
//	/* The RTC Clock may varies due to LSI frequency dispersion. */
//	/* Enable the LSI OSC */ 
//	RCC_LSICmd(ENABLE);
//
//	/* Wait till LSI is ready */  
//	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET){}
//
//	/* Select the RTC Clock Source */
//	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
//
//#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
//	/* Enable the LSE OSC */
//	RCC_LSEConfig(RCC_LSE_ON);
//
//	/* Wait till LSE is ready */
//	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && (RtcConfigTimeout < M_RTC_CONFIG_TIMEOUT))
//	{
//		++RtcConfigTimeout;
//	}
//	if (RtcConfigTimeout >= M_RTC_CONFIG_TIMEOUT)
//	{
//		puts("rtc config timeout");
//		return;
//	}
//
//	/* Select the RTC Clock Source */
//	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
//#else
//#error Please select the RTC Clock source inside the main.c file
//#endif /* RTC_CLOCK_SOURCE_LSI */
//
//	SynchPrediv = 0xFF;
//	AsynchPrediv = 0x7F;
//
//	/* Enable the RTC Clock */
//	RCC_RTCCLKCmd(ENABLE);
//
//	/* Wait for RTC APB registers synchronisation */
//	RTC_WaitForSynchro();
//
//	/* Enable The TimeStamp */
//	//  RTC_TimeStampCmd(RTC_TimeStampEdge_Falling, ENABLE);    
}/**********************   void rtc_config(void)   ****************************/

/**
  * @brief: Update time and date values.
  *
  * @param:
  *
  * @retval:
  **/
uint32_t rtc_date_time_update(){
	//uint32_t res = ~OK;

	//if (sec_set && minutes_set && hours_set && day_set && month_set && year_set){
	//	res = rtc_set_time();
	//	res = rtc_set_date();
	//	if (res == OK)
	//		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);/* Indicator for the RTC configuration */
	//	rtc_get_time(0);
	//	rtc_get_date(0);
	//}

	//return res;
	return 0;
}/************************ rtc_date_time_update *****************************/

/**
  * @brief  Sets the time value.
  * @param  None
  * @retval None
  */
uint32_t rtc_set_time(){
	//uint32_t res = ~OK;

	//RTC_TimeStructure.RTC_H12 = RTC_H12_AM;/* 24-hours only! */

	//if (RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)/* Configure the RTC time register */
	//	M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, ">> !! RTC Set Time failed. !! <<");
	//else{
	//	M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, ">> !! RTC Set Time success. !! <<");
	//	res = OK;
	//}
	//return res;
	return 0;
}/******************************   rtc_set_time   ***************************/

/**
  * @brief  Sets the date value.
  * @param  None
  * @retval None
  */
uint32_t rtc_set_date(){
	//uint32_t res = ~OK;

	//RTC_DateStructure.RTC_WeekDay = 1; //WeekDay (01-07)
	///* Configure the RTC date register */
	//if (RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR){
	//	M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, ">> !! RTC Set Date failed. !! <<");
	//}
	//else{
	//	M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, ">> !! RTC Set Date success. !! <<");
	//	res = OK;
	//}
	//return res;
	return 0;
}/******************************   rtc_set_date   ***************************/

/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
/**
  * @brief:
  *
  * @param:
  *
  * @retval:
  **/
#if 0
NVIC_InitTypeDef NVIC_InitStructure; 
EXTI_InitTypeDef EXTI_InitStructure;

/* Enable the PWR clock */
RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

/* Allow access to RTC */
PWR_BackupAccessCmd(ENABLE);

/* LSI used as RTC source clock */
/* The RTC Clock may varies due to LSI frequency dispersion. */   
/* Enable the LSI OSC */ 
RCC_LSICmd(ENABLE);

/* Wait till LSI is ready */  
while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
{
}

/* Select the RTC Clock Source */
RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

/* Enable the RTC Clock */
RCC_RTCCLKCmd(ENABLE);

/* Wait for RTC APB registers synchronisation */
RTC_WaitForSynchro();

/* Calendar Configuration with LSI supposed at 32KHz */
RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
RTC_InitStructure.RTC_SynchPrediv	=  0xFF; /* (32KHz / 128) - 1 = 0xFF*/
RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
RTC_Init(&RTC_InitStructure);  

/* EXTI configuration *******************************************************/
EXTI_ClearITPendingBit(EXTI_Line22);
EXTI_InitStructure.EXTI_Line = EXTI_Line22;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);

/* Enable the RTC Wakeup Interrupt */
NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (uint8_t)(configMAX_SYSCALL_INTERRUPT_PRIORITY>>4)+3;  //mast be above configMAX_SYSCALL_INTERRUPT_PRIORITY (11)
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);  

/* Configure the RTC WakeUp Clock source: CK_SPRE (1Hz) */
RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
RTC_SetWakeUpCounter(0x0);

/* Enable the RTC Wakeup Interrupt */
RTC_ITConfig(RTC_IT_WUT, ENABLE);

/* Enable Wakeup Counter */
RTC_WakeUpCmd(ENABLE);
//#if 0
NVIC_InitTypeDef NVIC_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;

if(RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2){     
	//      rtc_config();/* RTC configuration  */
	RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;/* Configure the RTC data register and RTC prescaler */
	RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	if(RTC_Init(&RTC_InitStructure) == ERROR)/* Check on RTC init */
		M_LOGGER_LOGF(IGOR_ERROR_LEVEL,">> !! RTC Prescaler Config failed. !! <<");
	//     RTC_TimeRegulate();/* Configure the time register */ 
}else{
	if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET){/* Check if the Power On Reset flag is set */
		M_LOGGER_LOGF(IGOR_ERROR_LEVEL,">> !! Power On Reset occurred.... !! <<");
	}else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)/* Check if the Pin Reset flag is set */
		M_LOGGER_LOGF(IGOR_ERROR_LEVEL,">> !! External Reset occurred.... !! <<");
	M_LOGGER_LOGF(IGOR_ERROR_LEVEL,">> !! No need to configure RTC.... !! <<");
}
rtc_config();/* RTC configuration  */    
RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);/* Enable the PWR clock */
PWR_BackupAccessCmd(ENABLE);/* Allow access to RTC */
RTC_WaitForSynchro();/* Wait for RTC APB registers synchronisation */
//      RTC_ClearFlag(RTC_FLAG_ALRAF);/* Clear the RTC Alarm Flag */
//      EXTI_ClearITPendingBit(EXTI_Line17);/* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
/* EXTI configuration *******************************************************/
EXTI_ClearITPendingBit(EXTI_Line22);
EXTI_InitStructure.EXTI_Line = EXTI_Line22;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);

/* RTC Wake Up Interrupt cnfg */
/* Enable the RTC Wakeup Interrupt */
EXTI_ClearITPendingBit(EXTI_Line22);
NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = (uint8_t)(configMAX_SYSCALL_INTERRUPT_PRIORITY>>4)+3;  //mast be above configMAX_SYSCALL_INTERRUPT_PRIORITY (11)
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);  

/* Configure the RTC WakeUp Clock source: CK_SPRE (1Hz) */
RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div2);/* RTC_WakeUpClock_CK_SPRE_16bits */
RTC_SetWakeUpCounter(0x0);

/* Enable the RTC Wakeup Interrupt */
RTC_ITConfig(RTC_IT_WUT, ENABLE);

/* Enable Wakeup Counter */
RTC_WakeUpCmd(ENABLE);      

/* Display the RTC Time/Date and TimeStamp Time/Date */
rtc_get_time(0);      
rtc_get_date(0);

#endif
