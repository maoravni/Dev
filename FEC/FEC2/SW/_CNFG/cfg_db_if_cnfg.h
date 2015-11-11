/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : PHANTOM 
*
*  FILE        : cfg_db_if_cnfg.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Phantom\SW\CNFG 
*
*  PURPOSE     : 	Date base interface map.
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      : Phantom board. 
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.08     Igor Natachanny   1.0.0   Initial version
   20/10/12    Yossi Sarusi      1.01    Add Debug value and applic version
   23/12/12    YOSSI Sarusi              Re-arrange it to support general middelware interface 
*
*******************************************************************************
      Copyright and Proprietary Statment.


******************************************************************************/

#ifndef _DB_IF_CNFG_H_
#define _DB_IF_CNFG_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* --- INCLUDES ------------------------------------------------------------ */

/* --- EXPORTED TYPES ------------------------------------------------------ */

/* --- EXPORTED CONSTANTS -------------------------------------------------- */


/*===========================================================================*/
/*      ***** P A C K A G E   C O N F I G U R A T I O N   D A T A *****      */
/*===========================================================================*/
  #ifdef CFG_DB_IF_CNFG
  
  const CFG_DB_IF_ENTRY_ST cfg_db_if_data[] = {
		
{NR, ID_GNRL_CFG_INT_CONTROL_TYPE,                         NR,  1000}, /* "0: Internal controller, 1: External controller (PUMA), 2: LABView" */
{NR, ID_GNRL_CMND_INT_SET_OPERATION_MODE,                 NR,  4},  /* turn on simulation mode*/
{NR, ID_GNRL_CMND_INT_SET_MODBUS_MASTER_BAUD,             NR,  5},  /* Set modbus master baudrate*/
{NR, ID_OTHERS_MNTR_INT_NOT_USED,             NR,  6},  /* Set modbus master baudrate*/
{NR, ID_OTHERS_MNTR_INT_NOT_USED,             NR,  7},  /* Set modbus master baudrate*/

{ NR, ID_GNRL_CMND_INT_DEBUG_CTRL,                         NR,	8}, /* Debug Cntrl set*/



{ NR, ID_GNRL_MNTR_INT_SW_VERSION_REVISION,              NR,	40}, /* Version Revision  */
{ NR, ID_GNRL_MNTR_INT_SW_VERSION_BUILD,                 NR,	41}, /* Version Bulid  */
{ NR, ID_GNRL_MNTR_INT_SW_VERSION_MINOR,                 NR,	42}, /* Version Minor  */
{ NR, ID_GNRL_MNTR_INT_SW_VERSION_MAJOR,                 NR,	43}, /* Version Major  */

{ NR, ID_GNRL_MNTR_INT_SUBSYS_ID,                 NR,	50}, /* Board SSID input */
{ NR, ID_GNRL_MNTR_INT_BOARD_HW_ID,               NR,	51}, /* Board HW manufacture  ID */
{ NR, ID_GNRL_MNTR_INT_BOARD_DIP_SWITCH,          NR,	52}, /* Board Dip switch select ID */
{ NR, ID_GNRL_MNTR_INT_BOARD_TEMPERATURE,         NR,	53}, /* Board measured temperature */
{ NR, ID_GNRL_CMND_INT_FW_UPGRD,                  NR,	54}, /* For the FEC firmware upgrade */
{ NR, ID_GNRL_CMND_SET_HWPWM_DTCYL,               NR,	55}, /* For the HW PWM Tesing */
{ NR, ID_GNRL_CMND_INT_FEC_RESET,                 NR,	56}, /* For the FEC Reset */


{ NR, ID_GNRL_CMND_INT_START_CAN_TEST,            NR,	60}, /*FEC ATP - Start CAN test  */
{ NR, ID_GNRL_CMND_INT_START_MODBUS_TEST,         NR,	61}, /*FEC ATP - Start Modbus test  */
{ NR, ID_GNRL_CMND_INT_START_EEPROM_TEST,         NR,	62}, /*FEC ATP - Start EEPROM  test */

{ NR, ID_GNRL_MNTR_INT_CAN_TEST_RESULT,         NR,	65}, /*FEC ATP - CAN test result: 0- Not Ready, 1-Pass, 2-Fail */
{ NR, ID_GNRL_MNTR_INT_MODBUS_TEST_RESULT,         NR,	66}, /*FEC ATP - Modbus test result: 0- Not Ready, 1-Pass, 2-Fail */
{ NR, ID_GNRL_MNTR_INT_EEPROM_TEST_RESULT,         NR,	67}, /*FEC ATP - EEPROM test result: 0- Not Ready, 1-Pass, 2-Fail */

//Set time and date
{NR, ID_GNRL_CMND_INT_TIME_SS_HSS,                          NR,  990}, /* Bits 15 - 9: Sec;   Bits 9 - 0: mSec    */
{NR, ID_GNRL_CMND_INT_TIME_HH_MM,                           NR,  991}, /* Bits 15 - 8: Hours; Bits 7 - 0: Minutes */
{NR, ID_GNRL_CMND_INT_DATE_MM_DD,                           NR,  992}, /* Bits 15 - 8: Month; Bits 7 - 0: Day     */
{NR, ID_GNRL_CMND_INT_DATE_YEAR,                            NR,  993}, /* Bits 15 - 0: Year;                      */

//Get time and date
{NR, ID_GNRL_MNTR_INT_TIME_SS_HSS,                          NR,  994}, /* Bits 15 - 9: Sec;   Bits 9 - 0: mSec    */
{NR, ID_GNRL_MNTR_INT_TIME_HH_MM,                           NR,  995}, /* Bits 15 - 8: Hours; Bits 7 - 0: Minutes */
{NR, ID_GNRL_MNTR_INT_DATE_MM_DD,                           NR,  996}, /* Bits 15 - 8: Month; Bits 7 - 0: Day     */
{NR, ID_GNRL_MNTR_INT_DATE_YEAR,                            NR,  997}, /* Bits 15 - 0: Year; */    

{ NR, ID_GNRL_MNTR_INT_CRITICAL_ALARM,                     NR,	998}, /* Critical alarm status: 1: rise, 0: cleared */
{ NR, ID_GNRL_CMND_INT_CLEAR_ALARM,                        NR,	999}, /* Initiates Clear alarm command */


{NR, ID_OTHERS_MNTR_INT_DIGITAL_INP_n,        0, 1001},
{NR, ID_OTHERS_MNTR_INT_DIGITAL_INP_n,        1, 1002}, 
{NR, ID_OTHERS_MNTR_INT_DIGITAL_INP_n,        2,	1003},
{NR, ID_OTHERS_MNTR_INT_DIGITAL_INP_n,        3,	1004}, 
{NR, ID_OTHERS_MNTR_INT_DIGITAL_INP_n,        4,	1005}, 
{NR, ID_OTHERS_MNTR_INT_DIGITAL_INP_n,        5,	1006}, 
{NR, ID_OTHERS_MNTR_INT_DIGITAL_INP_n,        6, 1007}, 
{NR, ID_OTHERS_MNTR_INT_DIGITAL_INP_n,        7, 1008}, 
{NR, ID_OTHERS_MNTR_INT_DIGITAL_INP_n,        8,	1009}, 
{NR, ID_OTHERS_MNTR_INT_DIGITAL_INP_n,        9,	1010}, 
{NR, ID_OTHERS_MNTR_INT_DIGITAL_INP_n,        10, 1011}, 
{NR, ID_OTHERS_MNTR_INT_DIGITAL_INP_n,        11, 1012}, 

//{ NR, ID_GNRL_MNTR_INT_APPLIC_VERSION,                     NR,	997}, /* Apllication verstion*/
{NR, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,        0,  2001}, /* Zone enable temperature 1  */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,        1,  2002}, /* Zone enable temperature 2 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,        2,	2003},/* Zone enable temperature 3  */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,        3,	2004}, /* Zone enable temperature 4  */


{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,     0,  2101}, /* Zone measured temperature 1  */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,     1,  2102}, /* Zone measured temperature 2  */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,     2,  2103},/* Zone measured temperature 3   */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,     3,  2104}, /* Zone measured temperature 4  */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,     4,  2105}, /* Zone measured temperature 5  */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,     5,  2106}, /* Zone measured temperature 6  */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,     6,	2107}, /* Zone measured temperature 7  */	
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,     7,	2108}, /* Zone measured temperature 8  */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,     8,	2109}, /* Zone measured temperature 9  */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,     9,	2110}, /* Zone measured temperature 10 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,     10,	2111}, /* Zone measured temperature 11 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,     11,	2112}, /* Zone measured temperature 12 */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,    12,	2113}, /* Zone measured temperature 13 */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,    13,	2114}, /* Zone measured temperature 14 */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,    14,	2115}, /* Zone measured temperature 15 */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,    15,	2116}, /* Zone measured temperature 16 */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,    16,	2117}, /* Zone measured temperature 17 */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,    17, 2118}, /* Zone measured temperature 18 */								


{NR, ID_TEMPERATURE_CMND_INT_ZONE_SP_n,            0,	2201}, /* Zone SP temperature 1 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_SP_n,            1,	2202}, 
{NR, ID_TEMPERATURE_CMND_INT_ZONE_SP_n,            2,	2203}, 
{NR, ID_TEMPERATURE_CMND_INT_ZONE_SP_n,            3,	2204}, 
{NR, ID_TEMPERATURE_CMND_INT_ZONE_SP_n,            4,	2205}, 
{NR, ID_TEMPERATURE_CMND_INT_ZONE_SP_n,            5,	2206}, 
{NR, ID_TEMPERATURE_CMND_INT_ZONE_SP_n,            6,	2207}, /* Zone SP temperature 7 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_SP_n,            7,	2208}, /* Zone SP temperature 8 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_SP_n,            8,	2209}, /* Zone SP temperature 9 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_SP_n,            9,	2210}, /* Zone SP temperature 10 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_SP_n,            10,	2211}, /* Zone SP temperature 11 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_SP_n,            11,	2212}, /* Zone SP temperature 12*/

{NR, ID_TEMPERATURE_CMND_INT_ZONE_SP0_n,            0,   3201}, /* Zone SP temperature 1 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_SP0_n,            1,   3202},

{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P0_n,         0,    3401}, /* Zone PID_P 1 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P0_n,         1,    3402}, /* Zone PID_P 2 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P0_n,         2,    3403}, /* Zone PID_P 1 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P0_n,         3,    3404}, /* Zone PID_P 2 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P0_n,         4,    3405}, /* Zone PID_P 1 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P0_n,         5,    3406}, /* Zone PID_P 2 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P0_n,         6,    3407}, /* Zone PID_P 2 */

{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n,       0,    3501}, /* MV value from PUMA zone 1 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n,       1,    3502}, /* MV value from PUMA zone 2 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n,       2,    3503}, /* MV value from PUMA zone 3 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n,       3,    3504}, /* MV value from PUMA zone 4 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n,       4,    3505}, /* MV value from PUMA zone 5 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n,       5,    3506}, /* MV value from PUMA zone 6 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n,       6,    3507}, /* MV value from PUMA zone 7 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n,       7,    3508}, /* MV value from PUMA zone 8 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n,       8,    3509}, /* MV value from PUMA zone 9 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n,       9,    3510}, /* MV value from PUMA zone 10 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n,       10,   3511}, /* MV value from PUMA zone 11 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PUMA_MV_n,       11,   3512}, /* MV value from PUMA zone 12 */


{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      0,	2301}, /* Zone MV 1 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      1,	2302}, 
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      2,	2303}, 
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      3,	2304}, 
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      4,	2305}, 
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      5,	2306}, 
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      6,	2307}, /* Zone MV 7 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      7,	2308}, /* Zone MV 8 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      8,	2309}, /* Zone MV 9 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      9,	2310}, /* Zone MV 10 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      10,2311}, /* Zone MV 11 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      11,2312}, /* Zone MV 12 */

{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         0,	2401}, /* Zone PID_P 1 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         1,	2402}, /* Zone PID_P 2 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         2,	2403}, /* Zone PID_P 3 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         3,	2404}, /* Zone PID_P 4 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         4,	2405}, /* Zone PID_P 5 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         5,	2406}, /* Zone PID_P 6 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         6,	2407}, /* Zone PID_P 7 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         7,	2408}, /* Zone PID_P 8 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         8,	2409}, /* Zone PID_P 9 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         9,	2410}, /* Zone PID_P 10 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         10,	2411}, /* Zone PID_P 11 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         11,	2412}, /* Zone PID_P 12 */

{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         0,	2501},  /* IBD Zone PID_I 1 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         1,	2502},  /* IBD Zone PID_I 2 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         2,	2503},  /* IBD Zone PID_I 3 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         3,	2504},  /* IBD Zone PID_I 4 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         4,	2505},  /* IBD Zone PID_I 5 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         5,	2506},  /* IBD Zone PID_I 6 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         6,	2507}, /* IBD Zone PID_I 7 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         7,	2508}, /* IBD Zone PID_I 8 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         8,	2509}, /* IBD Zone PID_I 9 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         9,	2510}, /* IBD Zone PID_I 10 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         10,	2511}, /* IBD Zone PID_I 11 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         11,	2512}, /* IBD Zone PID_I 12 */

{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         0,	2601},  //Zone PID_D 1 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         1,	2602}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         2,	2603}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         3,	2604}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         4,	2605}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         5,	2606}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         6,	2607}, /* Zone PID_D 7 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         7,	2608}, /* Zone PID_D 8 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         8,	2609}, /* Zone PID_D 9 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         9,	2610}, /* Zone PID_D 10 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         10,	2611}, /* Zone PID_D 11 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         11,	2612}, /* Zone PID_D 12 */


{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   0,	2701},/* POWER LIMIT 1 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   1,	2702},/* POWER LIMIT 2 */ 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   2,	2703},/* POWER LIMIT 3 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   3,	2704},/* POWER LIMIT 4 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   4,	2705},/* POWER LIMIT 5 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   5,	2706},/* POWER LIMIT 6 */ 
								
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   6,	2707}, /* POWER LIMIT 7 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   7,	2708}, /* POWER LIMIT 8 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   8,	2709}, /* POWER LIMIT 9 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   9,	2710}, /* POWER LIMIT 10 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   10,	2711}, /* POWER LIMIT 11 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   11,	2712}, /* POWER LIMIT 12*/
								

{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,   0,	2801},/* Zone AUTO_TUNE  1 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,   1,	2802},/* Zone AUTO_TUNE  2 */ 
{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,   2,	2803},/* Zone AUTO_TUNE  3 */ 
{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,   3,	2804},/* Zone AUTO_TUNE  4 */ 

{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,   6,	2807},/* Zone AUTO_TUNE  7 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,   7,	2808},/* Zone AUTO_TUNE  8 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,   8,	2809},/* Zone AUTO_TUNE  9 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,   9,	2810},/* Zone AUTO_TUNE 10 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,  10,	2811},/* Zone AUTO_TUNE 11 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,  11,	2812},/* Zone AUTO_TUNE 12 */



{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,        12,	2113},/* PV#13 - Chamber#1 Blower Front In Temperature   */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,        13,	2114},/* PV#14 - Chamber#1 Blower Front Out Temperature  */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,        14,	2115},/* PV#15 - Chamber#1 Blower Medium In Temperature  */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,        15,	2116},/* PV#16 - Chamber#1 Blower Medium Out Temperature */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,        16,	2117},/* PV#17 - Chamber#1 Blower Rear In Temperature    */
								


{ NR, ID_OTHERS_MNTR_INT_ANALOG_INP_HM_MA_n,                0,	1101}, /* Chamber#1  humidity value           */								
{ NR, ID_OTHERS_MNTR_INT_ANALOG_INP_HM_MA_n,                1,	1102}, /* Chamber#2  humidity value           */

{ NR, ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n,                   0,	1201}, /* AOma#1 -      */
{ NR, ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n,                   1,	1202}, /* AOma#2-      */

{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     0,	1301}, /* DO#1  */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     1,	1302}, /* DO#2  */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     2,	1303}, /* DO#3  */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     3,	1304}, /* DO#4  */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     4,	1305}, /* DO#5  */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     5,	1306}, /* DO#6  */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     6,	1307}, /* DO#7  */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     7,	1308}, /* DO#8  */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     8,	1309}, /* DO#9  */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     9,	1310}, /* DO#11  */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     10,	1311}, /* DO#11  */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     11,	1312}, /* DO#12  */

//tbd should be remove in the FEC
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     12,	1313}, /* DO#13  */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     13,	1314}, /* DO#14  */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     14,	1315}, /* DO#15 */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     15,	1316}, /* DO#16  */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     16,	1317}, /* DO#17 */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     17,	1318}, /* DO#18 */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     18,	1319}, /* DO#19 */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     19,	1320}, /* DO#20 */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     20,	1321}, /* DO#21 */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     21,	1322}, /* DO#22 */

{ NR, ID_OTHERS_CMND_INT_ITH_VALVES_STATE,                  0, 1380}, /* ITH valves state (0-3)*/


{ NR, ID_OTHERS_CMND_INT_SAFTY_RELAY,                       NR,	1399}, /* for ATP uses Set Safety relay */

{ NR, ID_OTHERS_CMND_INT_LED_OUT_n,                         0,	1401}, /* for ATP uses Turn LED 1 */
{ NR, ID_OTHERS_CMND_INT_LED_OUT_n,                         1,	1402}, /* for ATP uses Turn LED 2 */
{ NR, ID_OTHERS_CMND_INT_LED_OUT_n,                         2,	1403}, /* for ATP uses Turn LED 3 */
{ NR, ID_OTHERS_CMND_INT_LED_OUT_n,                         3,	1404}, /* for ATP uses Turn LED 4 */
{ NR, ID_OTHERS_CMND_INT_LED_OUT_n,                         4,	1405}, /* for ATP uses Turn LED 5 */
{ NR, ID_OTHERS_CMND_INT_LED_OUT_n,                         5,	1406}, /* for ATP uses Turn LED 6 */
{ NR, ID_OTHERS_CMND_INT_LED_OUT_n,                         6,	1407}, /* for ATP uses Turn LED 7 */
{ NR, ID_OTHERS_CMND_INT_LED_OUT_n,                         7,	1408}, /* for ATP uses Turn LED 8 */
{ NR, ID_OTHERS_CMND_INT_LED_OUT_n,                         8,	1409}, /* for ATP uses Turn LED 9 */
{ NR, ID_OTHERS_CMND_INT_LED_OUT_n,                         9,	1410}, /* for ATP uses Turn LED 10 */
{ NR, ID_OTHERS_CMND_INT_LED_OUT_n,                         10,	1411}, /* for ATP uses Turn LED 11 */
{ NR, ID_OTHERS_CMND_INT_LED_OUT_n,                         11,	1412}, /* for ATP uses Turn LED 12 */
{ NR, ID_OTHERS_CMND_INT_LED_OUT_n,                         12,	1413}, /* for ATP uses Turn LED 13 */
{ NR, ID_OTHERS_CMND_INT_LED_OUT_n,                         13,	1414}, /* for ATP uses Turn LED 14 */
{ NR, ID_OTHERS_CMND_INT_LED_OUT_n,                         14,	1415}, /* for ATP uses Turn LED 15 */



{ NR, ID_OTHERS_MNTR_INT_ANALOG_INP_MA_n,                    0,	1601}, /* AI ma 1 -      */
{ NR, ID_OTHERS_MNTR_INT_ANALOG_INP_MA_n,                    1,	1602}, /* AI ma 2 -      */
{ NR, ID_OTHERS_MNTR_INT_ANALOG_INP_MA_n,                    2,	1603}, /* AI ma 3 -      */

{ NR, ID_OTHERS_CMND_INT_ANALOG_OUT_V,                      0,	1701}, /* AO V 1 -      */

{ NR, ID_TEMPERATURE_CMND_INT_ZONE_PV_CALIB_n,               0,	3001}, /* Calib temperature 1    */
{ NR, ID_TEMPERATURE_CMND_INT_ZONE_PV_CALIB_n,               1,	3001}, /* Calib temperature 2    */
{ NR, ID_TEMPERATURE_CMND_INT_ZONE_PV_CALIB_n,               2,	3001}, /* Calib temperature 3    */
{ NR, ID_TEMPERATURE_CMND_INT_ZONE_PV_CALIB_n,               3,	3001}, /* Calib temperature 4    */
{ NR, ID_TEMPERATURE_CMND_INT_ZONE_PV_CALIB_n,               4,	3001}, /* Calib temperature 5    */
{ NR, ID_TEMPERATURE_CMND_INT_ZONE_PV_CALIB_n,               5,	3001}, /* Calib temperature 6    */
{ NR, ID_TEMPERATURE_CMND_INT_ZONE_PV_CALIB_n,               6,	3001}, /* Calib temperature 7    */
{ NR, ID_TEMPERATURE_CMND_INT_ZONE_PV_CALIB_n,               7,	3001}, /* Calib temperature 8    */
{ NR, ID_TEMPERATURE_CMND_INT_ZONE_PV_CALIB_n,               8,	3001}, /* Calib temperature 9    */
{ NR, ID_TEMPERATURE_CMND_INT_ZONE_PV_CALIB_n,               9,	3001}, /* Calib temperature 10    */
{ NR, ID_TEMPERATURE_CMND_INT_ZONE_PV_CALIB_n,               10,	3001}, /* Calib temperature 11    */
{ NR, ID_TEMPERATURE_CMND_INT_ZONE_PV_CALIB_n,               11,	3001}, /* Calib temperature 12    */

{ NR, ID_OTHERS_CMND_INT_ANALOG_IN_MA_CALIB_n,               0,	3101}, /* Calib ma 1    */
{ NR, ID_OTHERS_CMND_INT_ANALOG_IN_MA_CALIB_n,               1,	3102}, /* Calib ma 2    */
{ NR, ID_OTHERS_CMND_INT_ANALOG_IN_MA_CALIB_n,               2,	3103}, /* Calib ma 3    */

//for BTC
{ NR, ID_BTC_CMND_INT_COMMAND,                                 NR, 1110},
{ NR, ID_BTC_CMND_INT_FILL_PUMP_ENABLE,                      NR, 1111},
{ NR, ID_BTC_CMND_INT_DRAIN_PUMP_ENABLE,                     NR, 1112},
{ NR, ID_BTC_CMND_INT_ENGAGE_ENABLE,                         NR, 1113},
{ NR, ID_BTC_CMND_AIR_PRESSURE_KIFE_CMND_ENABLE,             NR, 1114},
{ NR, ID_BTC_CMND_TANK_DRAIN_WATER_ENABLE,                   NR, 1115},


{ NR, ID_BTC_CFG_INT_BLANKET_SP,                             NR, 1130},
{ NR, ID_BTC_CFG_INT_BLANKET_PID_P,                          NR, 1131},
{ NR, ID_BTC_CFG_INT_BLANKET_PID_I,                          NR, 1132},
{ NR, ID_BTC_CFG_INT_BLANKET_PID_D,                          NR, 1133},
{ NR, ID_BTC_CFG_INT_FILL_PUMP_HIGH_POWER_LIMIT,             NR, 1134},
{ NR, ID_BTC_CFG_INT_FILL_PUMP_LOW_POWER_LIMIT,              NR, 1135},
{ NR, ID_BTC_CFG_INT_DRAIN_PUMP_POWER,                       NR, 1136},
{ NR, ID_BTC_CFG_INT_WATER_TANK_FILL_TIME_OUT,               NR, 1137},
{ NR, ID_BTC_CFG_INT_AIR_KNIFE_PRESSURE_TIME_OUT,            NR, 1138},
{ NR, ID_BTC_CFG_INT_ENGAGE_TIME_OUT,                        NR, 1139},
{ NR, ID_BTC_CFG_INT_WATER_TANK_HIGH_TEMPERATURE_LIMIT,      NR, 1140},
{ NR, ID_BTC_CFG_INT_WATER_LINE_HIGH_TEMPERATURE_LIMIT,      NR, 1141},
{ NR, ID_BTC_CFG_INT_WATER_CONTAMINATION_HIGH_LIMIT,         NR, 1142},

{ NR, ID_BTC_MNTR_INT_STATE,                                 NR, 1170},
{ NR, ID_BTC_MNTR_INT_BLANKET_TEMPERATURE,                   NR, 1171},
{ NR, ID_BTC_MNTR_INT_FILL_PUMP_POWER,                       NR, 1172},
{ NR, ID_BTC_MNTR_INT_WATER_CONTAMINATION,                   NR, 1173},
{ NR, ID_BTC_MNTR_INT_ENGAGE,                                NR, 1174},

{ NR, ID_BTC_MNTR_INT_ALARM_GLOBAL,                          NR, 1180},
#if 0
{ NR, ID_BTC_MNTR_INT_ALARM_ENGAGE_TIME_OUT,                 NR, 1181},
{ NR, ID_BTC_MNTR_INT_ALARM_FILL_WATER_TIME_OUT,             NR, 1182},
{ NR, ID_BTC_MNTR_INT_ALARM_AIR_KNIFE_TIME_OUT,              NR, 1183},
{ NR, ID_BTC_MNTR_INT_ALARM_WATER_TANK_EXCEED_TEMPREATURE,   NR, 1184},
{ NR, ID_BTC_MNTR_INT_ALARM_WATER_LINE_EXCEED_TEMPREATURE,   NR, 1185},
{ NR, ID_BTC_MNTR_INT_ALARM_WATER_CONTAMINATION,             NR, 1186},
{ NR, ID_BTC_MNTR_INT_ALARM_WATER_TANK_OVERFLOW,             NR, 1187},
{ NR, ID_BTC_MNTR_INT_ALARM_TUB_OVERFLOW,                    NR, 1188},
{ NR, ID_BTC_MNTR_INT_ALARM_DRAWER,                          NR, 1189},
{ NR, ID_BTC_MNTR_INT_ALARM_BLANKET,                         NR, 1190},

#endif

{ NR, ID_OTHERS_MNTR_INT_NOT_USED,   NR,	1120}, /* TBD Chamber#1  AIR valve in ON TH*/
{ NR, ID_OTHERS_MNTR_INT_NOT_USED,   NR,	1121}, /* TBD Chamber#1 AIR valve in OFF TH*/
{ NR, ID_OTHERS_MNTR_INT_NOT_USED,   NR,	1122}, /* TBD Chamber#1 AIR valve Out ON TH*/
{ NR, ID_OTHERS_MNTR_INT_NOT_USED,   NR,	1123}, /* TBD Chamber#1 AIR valve Out OFF TH*/
{ NR, ID_OTHERS_MNTR_INT_NOT_USED,   NR,	1124}, /* TBD Chamber#2 AIR valve in ON TH*/
{ NR, ID_OTHERS_MNTR_INT_NOT_USED,   NR,	1125}, /* TBD Chamber#2 AIR valve in OFF TH*/
{ NR, ID_OTHERS_MNTR_INT_NOT_USED,   NR,	1126}, /* TBD Chamber#2 AIR valve Out ON TH*/
{ NR, ID_OTHERS_MNTR_INT_NOT_USED,   NR,	1127}, /* TBD Chamber#2 AIR valve Out OFF TH*/


  };
   
  #endif /* CFG_DB_IF_CNFG */
#ifdef __cplusplus
}
#endif
#endif /* _DB_IF_CNFG_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
