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
{NR, ID_GNRL_CMND_INT_SET_MODBUS_MASTER_BAUD,              NR,  5},  /* Set modbus master baudrate*/

{NR, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,        0,  2001}, /* Chamber#1 Zone enable temperature Front  */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,        1,  2002}, /* Chamber#1 Zone enable temperature Medium */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,        2,	2003},/* Chamber#1 Zone enable temperature Rear   */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,        3,	2004}, /* Chamber#2 Zone enable temperature Front  */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,        4,	2005}, /* Chamber#2 Zone enable temperature Medium  */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,        5,	2006}, /* Chamber#2 Zone enable temperature Rear  */

{NR, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,        6,  2007}, /* IBD Zone enable temperature 1   */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,        7,  2008}, /* IBD Zone enable temperature 2   */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,        8,	2009}, /* IBD Zone enable temperature 3   */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,        9,	2010}, /* IBD Zone enable temperature 4   */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,        10,	2011}, /* IBD Zone enable temperature 5   */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_ENABLE_n,        11,	2012}, /* IBD Zone enable temperature 6  */

{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,            0,  2101}, /* Chamber#1 Zone pv temperature Front  */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,            1,  2102}, /* Chamber#1 Zone pv temperature Medium */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,            2,  2103},/* Chamber#1 Zone pv temperature Rear   */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,            3,  2104}, /* Chamber#2 Zone pv temperature Front  */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,            4,  2105}, /* Chamber#2 Zone pv temperature Medium  */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,            5,  2106}, /* Chamber#2 Zone pv temperature Rear  */

								
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,           6,	2107}, /* IBD Zone measured temperature 1 */	
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,           7,	2108}, /* IBD Zone measured temperature 2 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,           8,	2109}, /* IBD Zone measured temperature 3 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,           9,	2110}, /* IBD Zone measured temperature 4 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,           10,	2111}, /* IBD Zone measured temperature 5 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,           11,	2112}, /* IBD Zone measured temperature 12 */


{NR, ID_TEMPERATURE_CFG_INT_ZONE_SP_n,            0,	2201}, //chambers
{NR, ID_TEMPERATURE_CFG_INT_ZONE_SP_n,            1,	2202}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_SP_n,            2,	2203}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_SP_n,            3,	2204}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_SP_n,            4,	2205}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_SP_n,            5,	2206}, 

{NR, ID_TEMPERATURE_CFG_INT_ZONE_SP_n,            6,	2207}, /* IBD Zone SP temperature 1 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_SP_n,            7,	2208}, /* IBD Zone SP temperature 2 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_SP_n,            8,	2209}, /* IBD Zone SP temperature 3 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_SP_n,            9,	2210}, /* IBD Zone SP temperature 4 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_SP_n,            10,	2211}, /* IBD Zone SP temperature 5 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_SP_n,            11,	2212}, /* IBD Zone SP temperature 6*/

{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      0,	2301}, //chambers
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      1,	2302}, 
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      2,	2303}, 
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      3,	2304}, 
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      4,	2305}, 
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      5,	2306}, 

{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      6,	2307}, /* IBD Zone MV 1 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      7,	2308}, /* IBD Zone MV 2 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      8,	2309}, /* IBD Zone MV 3 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      9,	2310}, /* IBD Zone MV 4 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      10,2311}, /* IBD Zone MV 5 */
{NR, ID_TEMPERATURE_MNTR_INT_ZONE_MV_n,      11,2312}, /* IBD Zone MV 6 */

{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         0,	2401},  //Chambers
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         1,	2402}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         2,	2403}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         3,	2404}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         4,	2405}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         5,	2406}, 

{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         6,	2407}, /* IBD Zone PID_P 1 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         7,	2408}, /* IBD Zone PID_P 2 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         8,	2409}, /* IBD Zone PID_P 3 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         9,	2410}, /* IBD Zone PID_P 4 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         10,	2411}, /* IBD Zone PID_P 5 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_P_n,         11,	2412}, /* IBD Zone PID_P 6 */

{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         0,	2501},  //Chambers
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         1,	2502}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         2,	2503}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         3,	2504}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         4,	2505}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         5,	2506}, 

{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         6,	2507}, /* IBD Zone PID_I 1 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         7,	2508}, /* IBD Zone PID_I 2 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         8,	2509}, /* IBD Zone PID_I  3 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         9,	2510}, /* IBD Zone PID_I  4 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         10,	2511}, /* IBD Zone PID_I  5 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_I_n,         11,	2512}, /* IBD Zone PID_I  6 */

{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         0,	2601},  //Chambers
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         1,	2602}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         2,	2603}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         3,	2604}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         4,	2605}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         5,	2606}, 

{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         6,	2607}, /* IBD Zone PID_D 1 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         7,	2608}, /* IBD Zone PID_D 2 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         8,	2609}, /* IBD Zone PID_D 3 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         9,	2610}, /* IBD Zone PID_D 4 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         10,	2611}, /* IBD Zone PID_D 5 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_PID_D_n,         11,	2612}, /* IBD Zone PID_D 6 */


{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   0,	2701}, //Chambers
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   1,	2702}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   2,	2703}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   3,	2704}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   4,	2705}, 
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   5,	2706}, 
								
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   6,	2707}, /* IBD Zone POWER LIMIT 1 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   7,	2708}, /* IBD Zone POWER LIMIT 2 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   8,	2709}, /* IBD Zone POWER LIMIT 3 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   9,	2710}, /* IBD Zone POWER LIMIT 4 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   10,	2711}, /* IBD Zone POWER LIMIT 5 */
{NR, ID_TEMPERATURE_CFG_INT_ZONE_POWER_LIMIT_n,   11,	2712}, /* IBD Zone POWER LIMIT 6*/
								

{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,   0,	2801}, //Chambers
{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,   1,	2802}, 
{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,   2,	2803}, 
{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,   3,	2804}, 
{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,   4,	2805}, 
{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,   5,	2806}, 

{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,    6,	2807}, /* IBD Zone AUTO_TUNE 1 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,    7,	2808}, /* IBD Zone AUTO_TUNE 2 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,    8,	2809}, /* IBD Zone AUTO_TUNE 3 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,    9,	2810}, /* IBD Zone AUTO_TUNE 4 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,    10,	2811}, /* IBD Zone AUTO_TUNE 5 */
{NR, ID_TEMPERATURE_CMND_INT_ZONE_AUTO_TUNE_n,    11,	2812}, /* IBD Zone AUTO_TUNE 6 */


{NR, ID_GNRL_CMND_INT_TIME_SS_HSS,                          NR,  990}, /* Bits 15 - 9: Sec;   Bits 9 - 0: mSec    */
{NR, ID_GNRL_CMND_INT_TIME_HH_MM,                           NR,  991}, /* Bits 15 - 8: Hours; Bits 7 - 0: Minutes */
{NR, ID_GNRL_CMND_INT_DATE_MM_DD,                           NR,  992}, /* Bits 15 - 8: Month; Bits 7 - 0: Day     */
{NR, ID_GNRL_CMND_INT_DATE_YEAR,                            NR,  993}, /* Bits 15 - 0: Year;                      */

{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                    12,	2113}, /* PV#13 - Chamber#1 Blower Front In Temperature        */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                    13,	2114}, /* PV#14 - Chamber#1 Blower Front Out Temperature       */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                    14,	2115}, /* PV#15 - Chamber#1 Blower Medium In Temperature */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                    15,	2116}, /* PV#16 - Chamber#1 Blower Medium Out Temperature   */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                    16,	2117}, /* PV#17 - Chamber#1 Blower Rear In Temperature           */
								
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                    17,	2118}, /* PV#18 - Chamber#1 Blower Rear Out Temperature      */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                    18,	2119}, /* PV#19 - Chamber#1  Out Front Temperature     */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                    19,	2120}, /* PV#20 - Chamber#1  Out Medium Temperature*/
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                    20,	2121}, /* PV#21 - Chamber#1  Out Rear Temperature      */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                    21,	2122}, /* PV#22 - Chamber#1   Heat-Exchanger                                                                                                                                                                                                                                                                                                                                                                                                                            Front Temperature             */
								
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                    22,	2123}, /* PV#23 - Chamber#1  Heat-Exchanger  Medium Temperature        */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                    23,	2124}, /* PV#24 - Chamber#1  Heat-Exchanger  Rear Temperature     */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                    24,	2125}, /* PV#25 - Chamber#1  Reference  Front Temperature */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                    25,	2126}, /* PV#26 - Chamber#1  Reference  Medium Temperature  */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                    26,	2127}, /* PV#27 - Chamber#1  Reference  Rear Temperature            */
							
{ NR, ID_OTHERS_MNTR_INT_ANALOG_INP_HM_MA_n,                0,	1101}, /* Chamber#1  humidity value           */

{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     12,	1313}, /* DO#13 - Chamber#1  Air valve input is open  */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     13,	1314}, /* DO#14 -Chamber#1  Air valve output is open */
							
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   27,	2128}, /* PV#28 - Chamber#2 Blower Front In Temperature        */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   28,	2129}, /* PV#29 - Chamber#2 Blower Front Out Temperature       */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   29,	2130}, /* PV#30 -   Chamber#2 Blower Medium In Temperature */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   30,	2131}, /* PV#31 -   Chamber#2 Blower Medium Out Temperature      */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   31,	2132}, /* PV#32 -   Chamber#2 Blower Rear In Temperature            */

{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   32,	2133}, /* PV#33 -  Chamber#2 Blower Rear Out Temperature        */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   33,	2134}, /* PV#34 -  Chamber#2  Out Front Temperature       */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   34,	2135}, /* PV#35 -   Chamber#2  Out Medium Temperature */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   35,	2136}, /* PV#36 -   Chamber#2  Out Rear Temperature     */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   36,	2137}, /* PV#37 -   Chamber#2  Heat-Exchanger  Front Temperature            */
							
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   37,	2138}, /* PV#38 -  Chamber#2  Heat-Exchanger  Medium Temperature       */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   38,	2139}, /* PV#39 -  Chamber#2  Heat-Exchanger  Rear Temperature       */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   39,	2140}, /* PV#40 -   Chamber#2  Reference  Front Temperature */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   40,	2141}, /* PV#41 -   Chamber#2  Reference  Medium Temperature    */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   41,	2142}, /* PV#42 -   Chamber#2  Reference  Rear Temperature           */

{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   42,	2143}, /* PV#43 -  IBD_B_1            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   43,	2144}, /* PV#44-   IBD_O_1            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   44,	2145}, /* PV#45 -  IBD_B_2            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   45,	2146}, /* PV#46-   IBD_O_2            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   46,	2147}, /* PV#47 -  IBD_B_3            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   47,	2148}, /* PV#48-   IBD_O_3            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   48,	2149}, /* PV#49 -  IBD_B_4            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   49,	2150}, /* PV#50-   IBD_O_4            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   50,	2151}, /* PV#51 -  IBD_B_5            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   51,	2152}, /* PV#52-   IBD_O_5            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   52,	2153}, /* PV#53 -  IBD_B_6            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   53,	2154 },/* PV#54-   IBD_O_6            */

{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   54,	2155 },/* PV#55-   IBD_H1_1            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   55,	2156 },/* PV#56-   IBD_H2_1            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   56,	2157 },/* PV#57-   IBD_H3_1            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   57,	2158 },/* PV#55-   IBD_H1_2            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   58,	2159 },/* PV#56-   IBD_H2_2            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   59,	2160 },/* PV#57-   IBD_H3_2            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   60,	2161 },/* PV#55-   IBD_H1_3            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   61,	2162 },/* PV#56-   IBD_H2_3            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   62,	2163 },/* PV#57-   IBD_H3_3            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   63,	2164 },/* PV#55-   IBD_H1_4            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   64,	2165 },/* PV#56-   IBD_H2_4            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   65,	2166 },/* PV#57-   IBD_H3_4            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   66,	2167 },/* PV#55-   IBD_H1_5            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   67,	2168 },/* PV#56-   IBD_H2_5            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   68,	2169 },/* PV#57-   IBD_H3_5            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   69,	2170 },/* PV#55-   IBD_H1_6            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   70,	2171 },/* PV#56-   IBD_H2_6            */
{ NR, ID_TEMPERATURE_MNTR_INT_ZONE_PV_n,                   71,	2172 },/* PV#57-   IBD_H3_6            */

								
{ NR, ID_OTHERS_MNTR_INT_ANALOG_INP_HM_MA_n,                1,	1102}, /* Chamber#2  humidity value           */

{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     14,	1315}, /* DO#15- Chamber#2  Air valve input is open  */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     15,	1316}, /* DO#16 - Chamber#2  Air valve output is open */

{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     16,	1317}, /* DO#17 - FAN IBD 1 */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     17,	1318}, /* DO#17 - FAN IBD 2 */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     18,	1319}, /* DO#17 - FAN IBD 3 */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     19,	1320}, /* DO#17 - FAN IBD 4 */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     20,	1321}, /* DO#17 - FAN IBD 5 */
{ NR, ID_OTHERS_CMND_INT_DIGITAL_OUT_n,                     21,	1322}, /* DO#17 - FAN IBD 6 */


{ NR, ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n,                   0,	1200}, /* AOma#1 - IBD1-3  Blower speed value       */
{ NR, ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n,                   1,	1201}, /* AOma#2- IBD4-4  Blower speed value       */
{ NR, ID_OTHERS_CMND_INT_ANALOG_OUT_MA_n,                   2,	1202}, /* AOma#3- Dryer Blower speed value       */

{ NR, ID_GNRL_MNTR_INT_CRITICAL_ALARM,                     NR,	998}, /* Critical alarm status: 1: rise, 0: cleared */
{ NR, ID_GNRL_CMND_INT_CLEAR_ALARM,                        NR,	999}, /* Initiates Clear alarm command */
{ NR, ID_GNRL_CMND_INT_DEBUG_VALUE,                        NR,	7}, /* Debug value set*/
{ NR, ID_GNRL_CMND_INT_DEBUG_CTRL,                         NR,	8}, /* Debug Cntrl set*/
{ NR, ID_GNRL_MNTR_INT_APPLIC_VERSION,                     NR,	997}, /* Apllication verstion*/
{ NR, ID_OTHERS_MNTR_INT_NOT_USED,                         NR,	1120}, /* TBD Chamber#1  AIR valve in ON TH*/
{ NR, ID_OTHERS_MNTR_INT_NOT_USED,                           NR,	1121}, /* TBD Chamber#1 AIR valve in OFF TH*/
{ NR, ID_OTHERS_MNTR_INT_NOT_USED,                           NR,	1122}, /* TBD Chamber#1 AIR valve Out ON TH*/
{ NR, ID_OTHERS_MNTR_INT_NOT_USED,                           NR,	1123}, /* TBD Chamber#1 AIR valve Out OFF TH*/
{ NR, ID_OTHERS_MNTR_INT_NOT_USED,                           NR,	1124}, /* TBD Chamber#2 AIR valve in ON TH*/
{ NR, ID_OTHERS_MNTR_INT_NOT_USED,                           NR,	1125}, /* TBD Chamber#2 AIR valve in OFF TH*/
{ NR, ID_OTHERS_MNTR_INT_NOT_USED,                           NR,	1126}, /* TBD Chamber#2 AIR valve Out ON TH*/
{ NR, ID_OTHERS_MNTR_INT_NOT_USED,                           NR,	1127}, /* TBD Chamber#2 AIR valve Out OFF TH*/


  };
   
  #endif /* CFG_DB_IF_CNFG */
#ifdef __cplusplus
}
#endif
#endif /* _DB_IF_CNFG_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
