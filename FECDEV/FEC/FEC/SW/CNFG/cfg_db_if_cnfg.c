/*
 * cfg_db_if_cnfg.c
 *
 *  Created on: 9 Dec 2012
 *      Author: maora
 */

#include "ll_typedef.h"
#include "GnrlCfgDB.h"
#include "CmbrCfgDB.h"
#include "cfg_db_if_cnfg.h"

#ifdef __cplusplus
 extern "C" {
#endif

#if 0
 //- maora 12 Dec 2012: Changed "cfg_db_if_data" to be static const - that way the array will be stored in ROM and not RAM.
const CFG_DB_IF_ENTRY_ST cfg_db_if_data[]= {

/* 00 */{NR, ID_GNRL_CFG_INT_CONTROL_TYPE,                         NR,  1000}, /* "0: Internal controller, 1: External controller (PUMA), 2: LABView" */

/* 01 */{NR, ID_GNRL_CMND_INT_ZONE_ENABLE_TEMPERATURE_WALL_n,        0,  2001}, /* Wall Zone enable temperature 1   */
/* 02 */{NR, ID_GNRL_CMND_INT_ZONE_ENABLE_TEMPERATURE_WALL_n,        1,  2002}, /* Wall Zone enable temperature 2   */
/* 03 */{NR, ID_GNRL_CMND_INT_ZONE_ENABLE_TEMPERATURE_WALL_n,        2, 2003}, /* Wall Zone enable temperature 3   */
/* 04 */{NR, ID_GNRL_CMND_INT_ZONE_ENABLE_TEMPERATURE_WALL_n,        3, 2004}, /* Wall Zone enable temperature 4   */
/* 05 */{NR, ID_GNRL_CMND_INT_ZONE_ENABLE_TEMPERATURE_WALL_n,        4, 2005}, /* Wall Zone enable temperature 5   */

/* 06 */{NR, ID_GNRL_MNTR_INT_ZONE_PV_TEMPERATURE_WALL_n,           0,  2101}, /* Wall Zone measured temperature 1 */
/* 07 */{NR, ID_GNRL_MNTR_INT_ZONE_PV_TEMPERATURE_WALL_n,           1,  2102}, /* Wall Zone measured temperature 2 */
/* 08 */{NR, ID_GNRL_MNTR_INT_ZONE_PV_TEMPERATURE_WALL_n,           2,  2103}, /* Wall Zone measured temperature 3 */
/* 09 */{NR, ID_GNRL_MNTR_INT_ZONE_PV_TEMPERATURE_WALL_n,           3,  2104}, /* Wall Zone measured temperature 4 */
/* 10 */{NR, ID_GNRL_MNTR_INT_ZONE_PV_TEMPERATURE_WALL_n,           4,  2105}, /* Wall Zone measured temperature 5 */

/* 11 */{NR, ID_GNRL_CFG_INT_ZONE_SP_TEMPERATURE_WALL_n,            0,  2201}, /* Wall Zone SP temperature 1 */
/* 12 */{NR, ID_GNRL_CFG_INT_ZONE_SP_TEMPERATURE_WALL_n,            1,  2202}, /* Wall Zone SP temperature 2 */
/* 13 */{NR, ID_GNRL_CFG_INT_ZONE_SP_TEMPERATURE_WALL_n,            2,  2203}, /* Wall Zone SP temperature 3 */
/* 14 */{NR, ID_GNRL_CFG_INT_ZONE_SP_TEMPERATURE_WALL_n,            3,  2204}, /* Wall Zone SP temperature 4 */
/* 15 */{NR, ID_GNRL_CFG_INT_ZONE_SP_TEMPERATURE_WALL_n,            4,  2205}, /* Wall Zone SP temperature 5 */

/* 16 */{NR, ID_GNRL_MNTR_INT_ZONE_MV_WALL_n,                       0,  2301}, /* Wall Zone MV 1 */
/* 17 */{NR, ID_GNRL_MNTR_INT_ZONE_MV_WALL_n,                       1,  2302}, /* Wall Zone MV 2 */
/* 18 */{NR, ID_GNRL_MNTR_INT_ZONE_MV_WALL_n,                       2,  2303}, /* Wall Zone MV 3 */
/* 19 */{NR, ID_GNRL_MNTR_INT_ZONE_MV_WALL_n,                       3,  2304}, /* Wall Zone MV 4 */
/* 20 */{NR, ID_GNRL_MNTR_INT_ZONE_MV_WALL_n,                       4,  2305}, /* Wall Zone MV 5 */



/* 21 */{NR, ID_GNRL_CFG_INT_ZONE_PID_P_TEMPERATURE_WALL_n,                     0,  2401}, /* Wall Zone PID_P 1 */
/* 22 */{NR, ID_GNRL_CFG_INT_ZONE_PID_P_TEMPERATURE_WALL_n,                     1,  2402}, /* Wall Zone PID_P 2 */
/* 23 */{NR, ID_GNRL_CFG_INT_ZONE_PID_P_TEMPERATURE_WALL_n,                     2,  2403}, /* Wall Zone PID_P 3 */
/* 24 */{NR, ID_GNRL_CFG_INT_ZONE_PID_P_TEMPERATURE_WALL_n,                     3,  2404}, /* Wall Zone PID_P 4 */
/* 25 */{NR, ID_GNRL_CFG_INT_ZONE_PID_P_TEMPERATURE_WALL_n,                     4,  2405}, /* Wall Zone PID_P 5 */

/* 26 */{NR, ID_GNRL_CFG_INT_ZONE_PID_I_TEMPERATURE_WALL_n,                     0,  2501}, /* Wall Zone PID_I 1 */
/* 27 */{NR, ID_GNRL_CFG_INT_ZONE_PID_I_TEMPERATURE_WALL_n,                     1,  2502}, /* Wall Zone PID_I 2 */
/* 28 */{NR, ID_GNRL_CFG_INT_ZONE_PID_I_TEMPERATURE_WALL_n,                     2,  2503}, /* Wall Zone PID_I  3 */
/* 29 */{NR, ID_GNRL_CFG_INT_ZONE_PID_I_TEMPERATURE_WALL_n,                     3,  2504}, /* Wall Zone PID_I  4 */
/* 30 */{NR, ID_GNRL_CFG_INT_ZONE_PID_I_TEMPERATURE_WALL_n,                     4,  2505}, /* Wall Zone PID_I  5 */

/* 31 */{NR, ID_GNRL_CFG_INT_ZONE_PID_D_TEMPERATURE_WALL_n,                     0,  2601}, /* Wall Zone PID_D 1 */
/* 32 */{NR, ID_GNRL_CFG_INT_ZONE_PID_D_TEMPERATURE_WALL_n,                     1,  2602}, /* Wall Zone PID_D 2 */
/* 33 */{NR, ID_GNRL_CFG_INT_ZONE_PID_D_TEMPERATURE_WALL_n,                     2,  2603}, /* Wall Zone PID_D 3 */
/* 34 */{NR, ID_GNRL_CFG_INT_ZONE_PID_D_TEMPERATURE_WALL_n,                     3,  2604}, /* Wall Zone PID_D 4 */
/* 35 */{NR, ID_GNRL_CFG_INT_ZONE_PID_D_TEMPERATURE_WALL_n,                     4,  2605}, /* Wall Zone PID_D 5 */

/* 36 */{NR, ID_GNRL_CFG_INT_ZONE_POWER_LIMIT_TEMPERATURE_WALL_n,               0,  2701}, /* Wall Zone POWER LIMIT 1 */
/* 37 */{NR, ID_GNRL_CFG_INT_ZONE_POWER_LIMIT_TEMPERATURE_WALL_n,               1,  2702}, /* Wall Zone POWER LIMIT 2 */
/* 38 */{NR, ID_GNRL_CFG_INT_ZONE_POWER_LIMIT_TEMPERATURE_WALL_n,               2,  2703}, /* Wall Zone POWER LIMIT 3 */
/* 39 */{NR, ID_GNRL_CFG_INT_ZONE_POWER_LIMIT_TEMPERATURE_WALL_n,               3,  2704}, /* Wall Zone POWER LIMIT 4 */
/* 40 */{NR, ID_GNRL_CFG_INT_ZONE_POWER_LIMIT_TEMPERATURE_WALL_n,               4,  2705}, /* Wall Zone POWER LIMIT 5 */

/* 41 */{NR, ID_GNRL_CMND_INT_ZONE_AUTO_TUNE_TEMPERATURE_WALL_n,                0,  2801}, /* Wall Zone AUTO_TUNE 1 */
/* 42 */{NR, ID_GNRL_CMND_INT_ZONE_AUTO_TUNE_TEMPERATURE_WALL_n,                1,  2802}, /* Wall Zone AUTO_TUNE 2 */
/* 43 */{NR, ID_GNRL_CMND_INT_ZONE_AUTO_TUNE_TEMPERATURE_WALL_n,                2,  2803}, /* Wall Zone AUTO_TUNE 3 */
/* 44 */{NR, ID_GNRL_CMND_INT_ZONE_AUTO_TUNE_TEMPERATURE_WALL_n,                3,  2804}, /* Wall Zone AUTO_TUNE 4 */
/* 45 */{NR, ID_GNRL_CMND_INT_ZONE_AUTO_TUNE_TEMPERATURE_WALL_n,                4,  2805}, /* Wall Zone AUTO_TUNE 5 */


/*  */  {NR, ID_GNRL_CMND_INT_TIME_SS_HSS,                                      NR,  990}, /* Bits 15 - 9: Sec;   Bits 9 - 0: mSec    */
/*  */  {NR, ID_GNRL_CMND_INT_TIME_HH_MM,                                       NR,  991}, /* Bits 15 - 8: Hours; Bits 7 - 0: Minutes */
/*  */  {NR, ID_GNRL_CMND_INT_DATE_MM_DD,                                       NR,  992}, /* Bits 15 - 8: Month; Bits 7 - 0: Day     */
/*  */  {NR, ID_GNRL_CMND_INT_DATE_YEAR,                                        NR,  993}, /* Bits 15 - 0: Year;                      */

/* 46 */{ 0, ID_CMBR_CMND_INT_ZONE_ENABLE_TEMPERATURE_FRONT,  NR,   2006}, /* Chamber#1 Zone enable temperature Front  */
/* 47 */{ 0, ID_CMBR_CMND_INT_ZONE_ENABLE_TEMPERATURE_MEDIUM, NR,   2007}, /* Chamber#1 Zone enable temperature Medium */
/* 48 */{ 0, ID_CMBR_CMND_INT_ZONE_ENABLE_TEMPERATURE_REAR,   NR,   2008}, /* Chamber#1 Zone enable temperature Rear   */
/* 49 */{ 1, ID_CMBR_CMND_INT_ZONE_ENABLE_TEMPERATURE_FRONT,  NR,   2009}, /* Chamber#2 Zone enable temperature Front  */
/* 50 */{ 1, ID_CMBR_CMND_INT_ZONE_ENABLE_TEMPERATURE_MEDIUM, NR,   2010}, /* Chamber#2 Zone enable temperature Medium */
/* 51 */{ 1, ID_CMBR_CMND_INT_ZONE_ENABLE_TEMPERATURE_REAR,   NR,   2011}, /* Chamber#2 Zone enable temperature Rear   */

/* 52 */{ 0, ID_CMBR_MNTR_INT_HTEX_ZONE_PV_TEMPERATURE_FRONT,      NR,  2106}, /* Chamber#1 Zone measured temperature Front  */
/* 53 */{ 0, ID_CMBR_MNTR_INT_HTEX_ZONE_PV_TEMPERATURE_MEDIUM,     NR,  2107}, /* Chamber#1 Zone measured temperature Medium */
/* 54 */{ 0, ID_CMBR_MNTR_INT_HTEX_ZONE_PV_TEMPERATURE_REAR,       NR,  2108}, /* Chamber#1 Zone measured temperature Rear   */
/* 55 */{ 1, ID_CMBR_MNTR_INT_HTEX_ZONE_PV_TEMPERATURE_FRONT,      NR,  2109}, /* Chamber#2 Zone measured temperature Front  */
/* 56 */{ 1, ID_CMBR_MNTR_INT_HTEX_ZONE_PV_TEMPERATURE_MEDIUM,     NR,  2110}, /* Chamber#2 Zone measured temperature Medium */
/* 57 */{ 1, ID_CMBR_MNTR_INT_HTEX_ZONE_PV_TEMPERATURE_REAR,       NR,  2111}, /* Chamber#2 Zone measured temperature Rear   */

/* 58 */{ 0, ID_CMBR_CFG_INT_ZONE_SP_TEMPERATURE_FRONT,       NR,   2206}, /* Chamber#1 Zone SP temperature Front  */
/* 59 */{ 0, ID_CMBR_CFG_INT_ZONE_SP_TEMPERATURE_MEDIUM,      NR,   2207}, /* Chamber#1 Zone SP temperature Medium */
/* 60 */{ 0, ID_CMBR_CFG_INT_ZONE_SP_TEMPERATURE_REAR,         NR,  2208}, /* Chamber#1 Zone SP temperature Rear   */
/* 61 */{ 1, ID_CMBR_CFG_INT_ZONE_SP_TEMPERATURE_FRONT,    NR,  2209}, /* Chamber#2 Zone SP temperature Front  */
/* 62 */{ 1, ID_CMBR_CFG_INT_ZONE_SP_TEMPERATURE_MEDIUM,       NR,  2210}, /* Chamber#2 Zone SP temperature Medium */
/* 63 */{ 1, ID_CMBR_CFG_INT_ZONE_SP_TEMPERATURE_REAR,         NR,  2211}, /* Chamber#2 Zone SP temperature Rear   */

/* 64 */{ 0, ID_CMBR_MNTR_INT_HTEX_ZONE_MV_FRONT,                  NR,  2306}, /* Chamber#1 Zone MV Front  */
/* 65 */{ 0, ID_CMBR_MNTR_INT_HTEX_ZONE_MV_MEDIUM,                 NR,  2307}, /* Chamber#1 Zone MV Medium */
/* 66 */{ 0, ID_CMBR_MNTR_INT_HTEX_ZONE_MV_REAR,                   NR,  2308}, /* Chamber#1 Zone MV Rear   */
/* 67 */{ 1, ID_CMBR_MNTR_INT_HTEX_ZONE_MV_FRONT,                  NR,  2309}, /* Chamber#2 Zone MV Front  */
/* 68 */{ 1, ID_CMBR_MNTR_INT_HTEX_ZONE_MV_MEDIUM,                 NR,  2310}, /* Chamber#2 Zone MV Medium */
/* 69 */{ 1, ID_CMBR_MNTR_INT_HTEX_ZONE_MV_REAR,                   NR,  2311}, /* Chamber#2 Zone MV Rear   */

/* 70 */{ 0, ID_CMBR_CFG_INT_ZONE_PID_P_TEMPERATURE_FRONT,                NR,  2406}, /* Chamber#1 Zone PID_P Front  */
/* 71 */{ 0, ID_CMBR_CFG_INT_ZONE_PID_P_TEMPERATURE_MEDIUM,               NR,   2407}, /* Chamber#1 Zone PID_P Medium */
/* 72 */{ 0, ID_CMBR_CFG_INT_ZONE_PID_P_TEMPERATURE_REAR,                 NR,   2408}, /* Chamber#1 Zone PID_P Rear   */
/* 73 */{ 1, ID_CMBR_CFG_INT_ZONE_PID_P_TEMPERATURE_FRONT,                NR,   2409}, /* Chamber#2 Zone PID_P Front  */
/* 74 */{ 1, ID_CMBR_CFG_INT_ZONE_PID_P_TEMPERATURE_MEDIUM,               NR,   2410}, /* Chamber#2 Zone PID_P Medium */
/* 75 */{ 1, ID_CMBR_CFG_INT_ZONE_PID_P_TEMPERATURE_REAR,                 NR,   2411}, /* Chamber#2 Zone PID_P Rear   */

/* 76 */{ 0, ID_CMBR_CFG_INT_ZONE_PID_I_TEMPERATURE_FRONT,                NR,   2506}, /* Chamber#1 Zone PID_I Front  */
/* 77 */{ 0, ID_CMBR_CFG_INT_ZONE_PID_I_TEMPERATURE_MEDIUM,               NR,   2507}, /* Chamber#1 Zone PID_I Medium */
/* 78 */{ 0, ID_CMBR_CFG_INT_ZONE_PID_I_TEMPERATURE_REAR,                 NR,   2508}, /* Chamber#1 Zone PID_I Rear   */
/* 79 */{ 1, ID_CMBR_CFG_INT_ZONE_PID_I_TEMPERATURE_FRONT,                NR,   2509}, /* Chamber#2 Zone PID_I Front  */
/* 80 */{ 1, ID_CMBR_CFG_INT_ZONE_PID_I_TEMPERATURE_MEDIUM,               NR,   2510}, /* Chamber#2 Zone PID_I Medium */
/* 81 */{ 1, ID_CMBR_CFG_INT_ZONE_PID_I_TEMPERATURE_REAR,                 NR,   2511}, /* Chamber#2 Zone PID_I Rear   */

/* 82 */{ 0, ID_CMBR_CFG_INT_ZONE_PID_D_TEMPERATURE_FRONT,                NR,   2606}, /* Chamber#1 Zone PID_D Front  */
/* 83 */{ 0, ID_CMBR_CFG_INT_ZONE_PID_D_TEMPERATURE_MEDIUM,               NR,   2607}, /* Chamber#1 Zone PID_D Medium */
/* 84 */{ 0, ID_CMBR_CFG_INT_ZONE_PID_D_TEMPERATURE_REAR,                 NR,   2608}, /* Chamber#1 Zone PID_D Rear   */
/* 85 */{ 1, ID_CMBR_CFG_INT_ZONE_PID_D_TEMPERATURE_FRONT,                NR,   2609}, /* Chamber#2 Zone PID_D Front  */
/* 86 */{ 1, ID_CMBR_CFG_INT_ZONE_PID_D_TEMPERATURE_MEDIUM,               NR,   2610}, /* Chamber#2 Zone PID_D Medium */
/* 87 */{ 1, ID_CMBR_CFG_INT_ZONE_PID_D_TEMPERATURE_REAR,                 NR,   2611}, /* Chamber#2 Zone PID_D Rear   */

/* 88 */{ 0, ID_CMBR_CFG_INT_ZONE_POWER_LIMIT_TEMPERATURE_FRONT,          NR,   2706}, /* Chamber#1 Zone POWER LIMIT Front  */
/* 89 */{ 0, ID_CMBR_CFG_INT_ZONE_POWER_LIMIT_TEMPERATURE_MEDIUM,         NR,   2707}, /* Chamber#1 Zone POWER LIMIT Medium */
/* 90 */{ 0, ID_CMBR_CFG_INT_ZONE_POWER_LIMIT_TEMPERATURE_REAR,           NR,   2708}, /* Chamber#1 Zone POWER LIMIT Rear   */
/* 91 */{ 1, ID_CMBR_CFG_INT_ZONE_POWER_LIMIT_TEMPERATURE_FRONT,          NR,   2709}, /* Chamber#2 Zone POWER LIMIT Front  */
/* 92 */{ 1, ID_CMBR_CFG_INT_ZONE_POWER_LIMIT_TEMPERATURE_MEDIUM,         NR,   2710}, /* Chamber#2 Zone POWER LIMIT Medium */
/* 93 */{ 1, ID_CMBR_CFG_INT_ZONE_POWER_LIMIT_TEMPERATURE_REAR,           NR,   2711}, /* Chamber#2 Zone POWER LIMIT Rear   */

/* 94 */{ 0, ID_CMBR_CMND_INT_ZONE_AUTO_TUNE_TEMPERATURE_FRONT,            NR,  2806}, /* Chamber#1 Zone AUTO_TUNE Front    */
/* 95 */{ 0, ID_CMBR_CMND_INT_ZONE_AUTO_TUNE_TEMPERATURE_MEDIUM,           NR,  2807}, /* Chamber#1 Zone AUTO_TUNE Medium   */
/* 96 */{ 0, ID_CMBR_CMND_INT_ZONE_AUTO_TUNE_TEMPERATURE_REAR,             NR,  2808}, /* Chamber#1 Zone AUTO_TUNE Rear     */
/* 97 */{ 1, ID_CMBR_CMND_INT_ZONE_AUTO_TUNE_TEMPERATURE_FRONT,            NR,  2809}, /* Chamber#2 Zone AUTO_TUNE Front    */
/* 98 */{ 1, ID_CMBR_CMND_INT_ZONE_AUTO_TUNE_TEMPERATURE_MEDIUM,           NR,  2810}, /* Chamber#2 Zone AUTO_TUNE Medium   */
/* 99 */{ 1, ID_CMBR_CMND_INT_ZONE_AUTO_TUNE_TEMPERATURE_REAR,             NR,  2811}, /* Chamber#2 Zone AUTO_TUNE Rear     */

/* 100 */{ 0, ID_CMBR_MNTR_INT_TEMPERATURE_BLOWER_FRONT_IN,         NR, 1001}, /* Chamber#1 Blower Front In Temperature        */
/* 101 */{ 0, ID_CMBR_MNTR_INT_TEMPERATURE_BLOWER_FRONT_OUT,        NR, 1002}, /* Chamber#1 Blower Front Out Temperature       */
/* 102 */{ 0, ID_CMBR_MNTR_INT_TEMPERATURE_HTEX_FRONT,              NR, 1003}, /* Chamber#1  Heat-Exchanger  Front Temperature */
/* 103 */{ 0, ID_CMBR_MNTR_INT_TEMPERATURE_REF_FRONT,               NR, 1004}, /* Chamber#1  Reference  Front Temperature      */
/* 104 */{ 0, ID_CMBR_MNTR_INT_TEMPERATURE_OUT_FRONT,               NR, 1005}, /* Chamber#1  Out Front Temperature             */

/* 105 */{ 0, ID_CMBR_MNTR_INT_TEMPERATURE_BLOWER_MEDIUM_IN,        NR, 1006}, /* Chamber#1 Blower Medium In Temperature        */
/* 106 */{ 0, ID_CMBR_MNTR_INT_TEMPERATURE_BLOWER_MEDIUM_OUT,       NR, 1007}, /* Chamber#1 Blower Medium Out Temperature       */
/* 107 */{ 0, ID_CMBR_MNTR_INT_TEMPERATURE_HTEX_MEDIUM,             NR, 1008}, /* Chamber#1  Heat-Exchanger  Medium Temperature */
/* 108 */{ 0, ID_CMBR_MNTR_INT_TEMPERATURE_REF_MEDIUM,              NR, 1009}, /* Chamber#1  Reference  Medium Temperature      */
/* 109 */{ 0, ID_CMBR_MNTR_INT_TEMPERATURE_OUT_MEDIUM,              NR, 1010}, /* Chamber#1  Out Medium Temperature             */

/* 110 */{ 0, ID_CMBR_MNTR_INT_TEMPERATURE_BLOWER_REAR_IN,          NR, 1011}, /* Chamber#1 Blower Rear In Temperature        */
/* 111 */{ 0, ID_CMBR_MNTR_INT_TEMPERATURE_BLOWER_REAR_OUT,         NR, 1012}, /* Chamber#1 Blower Rear Out Temperature       */
/* 112 */{ 0, ID_CMBR_MNTR_INT_TEMPERATURE_HTEX_REAR,               NR, 1013}, /* Chamber#1  Heat-Exchanger  Rear Temperature */
/* 113 */{ 0, ID_CMBR_MNTR_INT_TEMPERATURE_REF_REAR,                NR, 1014}, /* Chamber#1  Reference  Rear Temperature      */
/* 114 */{ 0, ID_CMBR_MNTR_INT_TEMPERATURE_OUT_REAR,                NR, 1015}, /* Chamber#1  Out Rear Temperature             */

/* 115 */{ 0, ID_CMBR_MNTR_INT_HUMIDITY,                            NR, 1101}, /* Chamber#1  humidity value           */
/* 116 */{ 0, ID_CMBR_CFG_INT_BLOWER_SPEED,                         NR, 1201}, /* Chamber#1  Blower speed value       */
/* 117 */{ 0, ID_CMBR_CFG_INT_AIR_VALVE_IN_OPEN,                    NR, 1301}, /* Chamber#1  Air valve input is open  */
/* 118 */{ 0, ID_CMBR_CFG_INT_AIR_VALVE_OUT_OPEN,                   NR, 1302}, /* Chamber#1  Air valve output is open */

/* 119 */{ 1, ID_CMBR_MNTR_INT_TEMPERATURE_BLOWER_FRONT_IN,         NR, 1501}, /* Chamber#2 Blower Front In Temperature        */
/* 120 */{ 1, ID_CMBR_MNTR_INT_TEMPERATURE_BLOWER_FRONT_OUT,        NR, 1502}, /* Chamber#2 Blower Front Out Temperature       */
/* 121 */{ 1, ID_CMBR_MNTR_INT_TEMPERATURE_HTEX_FRONT,              NR, 1503}, /* Chamber#2  Heat-Exchanger  Front Temperature */
/* 122 */{ 1, ID_CMBR_MNTR_INT_TEMPERATURE_REF_FRONT,               NR, 1504}, /* Chamber#2  Reference  Front Temperature      */
/* 123 */{ 1, ID_CMBR_MNTR_INT_TEMPERATURE_OUT_FRONT,               NR, 1505}, /* Chamber#2  Out Front Temperature             */

/* 124 */{ 1, ID_CMBR_MNTR_INT_TEMPERATURE_BLOWER_MEDIUM_IN,        NR, 1506}, /* Chamber#2 Blower Medium In Temperature        */
/* 125 */{ 1, ID_CMBR_MNTR_INT_TEMPERATURE_BLOWER_MEDIUM_OUT,       NR, 1507}, /* Chamber#2 Blower Medium Out Temperature       */
/* 126 */{ 1, ID_CMBR_MNTR_INT_TEMPERATURE_HTEX_MEDIUM,             NR, 1508}, /* Chamber#2  Heat-Exchanger  Medium Temperature */
/* 127 */{ 1, ID_CMBR_MNTR_INT_TEMPERATURE_REF_MEDIUM,              NR, 1509}, /* Chamber#2  Reference  Medium Temperature      */
/* 128 */{ 1, ID_CMBR_MNTR_INT_TEMPERATURE_OUT_MEDIUM,              NR, 1510}, /* Chamber#2  Out Medium Temperature             */

/* 129 */{ 1, ID_CMBR_MNTR_INT_TEMPERATURE_BLOWER_REAR_IN,          NR, 1511}, /* Chamber#2 Blower Rear In Temperature        */
/* 130 */{ 1, ID_CMBR_MNTR_INT_TEMPERATURE_BLOWER_REAR_OUT,         NR, 1512}, /* Chamber#2 Blower Rear Out Temperature       */
/* 131 */{ 1, ID_CMBR_MNTR_INT_TEMPERATURE_HTEX_REAR,               NR, 1513}, /* Chamber#2  Heat-Exchanger  Rear Temperature */
/* 132 */{ 1, ID_CMBR_MNTR_INT_TEMPERATURE_REF_REAR,                NR, 1514}, /* Chamber#2  Reference  Rear Temperature      */
/* 133 */{ 1, ID_CMBR_MNTR_INT_TEMPERATURE_OUT_REAR,                NR, 1515}, /* Chamber#2  Out Rear Temperature             */

/* 134 */{ 1, ID_CMBR_MNTR_INT_HUMIDITY,                            NR, 1102}, /* Chamber#2  humidity value           */
/* 135 */{ 1, ID_CMBR_CFG_INT_BLOWER_SPEED,                         NR, 1202}, /* Chamber#2  Blower speed value       */
/* 136 */{ 1, ID_CMBR_CFG_INT_AIR_VALVE_IN_OPEN,                    NR, 1303}, /* Chamber#2  Air valve input is open  */
/* 137 */{ 1, ID_CMBR_CFG_INT_AIR_VALVE_OUT_OPEN,                   NR, 1304}, /* Chamber#2  Air valve output is open */

/* 138 */{ NR, ID_GNRL_MNTR_INT_CRITICAL_ALARM,                     NR, 998}, /* Critical alarm status: 1: rise, 0: cleared */
/* 139 */{ NR, ID_GNRL_CMND_INT_CLEAR_ALARM,                        NR, 999}, /* Initiates Clear alarm command */
/* 140 */{ NR, ID_GNRL_CMND_INT_DEBUG_VALUE,                        NR, 7}, /* Debug value set*/
/* 141 */{ NR, ID_GNRL_CMND_INT_DEBUG_CTRL,                         NR, 8}, /* Debug Cntrl set*/
/* 142 */{ NR, ID_GNRL_MNTR_INT_APPLIC_VERSION,                     NR, 997}, /* Apllication verstion*/
/* 143 */{ 0, ID_CMBR_MNTR_INT_NOT_USED,                            NR, 1120}, /* TBD Chamber#1  AIR valve in ON TH*/
/* 144 */{ 0, ID_CMBR_MNTR_INT_NOT_USED,                            NR, 1121}, /* TBD Chamber#1 AIR valve in OFF TH*/
/* 145 */{ 0, ID_CMBR_MNTR_INT_NOT_USED,                            NR, 1122}, /* TBD Chamber#1 AIR valve Out ON TH*/
/* 146 */{ 0, ID_CMBR_MNTR_INT_NOT_USED,                            NR, 1123}, /* TBD Chamber#1 AIR valve Out OFF TH*/
/* 147 */{ 1, ID_CMBR_MNTR_INT_NOT_USED,                            NR, 1124}, /* TBD Chamber#2 AIR valve in ON TH*/
/* 148 */{ 1, ID_CMBR_MNTR_INT_NOT_USED,                            NR, 1125}, /* TBD Chamber#2 AIR valve in OFF TH*/
/* 149 */{ 1, ID_CMBR_MNTR_INT_NOT_USED,                            NR, 1126}, /* TBD Chamber#2 AIR valve Out ON TH*/
/* 150 */{ 1, ID_CMBR_MNTR_INT_NOT_USED,                            NR, 1127}, /* TBD Chamber#2 AIR valve Out OFF TH*/

  };

#endif

#ifdef __cplusplus
 }
#endif
