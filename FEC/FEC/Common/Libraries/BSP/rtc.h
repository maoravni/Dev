/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     :  
*
*  FILE        : *.h
*   
*  PATH        :  
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
*  2012.01     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
*      Copyright and Proprietary Statment.
*
******************************************************************************/

#ifndef _RTC_H_
#define _RTC_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */

/* --- EXPORTED TYPES ------------------------------------------------------ */
   typedef enum _time_date_show_ctrl_{
     DATE_ONLY = 0,
     TIME_ONLY = 1,
     DATE_AND_TIME = 2
   } TIME_DATE_SHOW_CTRL_ET;
/* --- EXPORTED CONSTANTS -------------------------------------------------- */

/*===========================================================================*/
/*                ***** P A C K A G E   S E R V I C E S *****                */
/*===========================================================================*/

/* --- EXPORTED MACRO -------------------------------------------------------*/

/* --- EXPORTED FUNCTIONS ---------------------------------------------------*/
  void rtc_get_time(RTC_TimeTypeDef *cur_time_ptr);
  uint32_t rtc_set_year_val(uint32_t data);
  uint32_t rtc_set_day_month_vals(uint32_t data);
  uint32_t rtc_set_hours_minutes_vals(uint32_t data);
  uint32_t rtc_set_seconds_val(uint32_t data);
  void rtc_init();
  char * rtc_show_time_date(char *tmp_buf, TIME_DATE_SHOW_CTRL_ET show_ctrl);
/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/







#ifdef __cplusplus
}
#endif
#endif /* _RTC_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
