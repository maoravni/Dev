/*
 * PscSubsystem.c
 *
 *  Created on: 4 Jul 2013
 *      Author: maora
 */

#include "PscSubsystem.h"
#ifndef FEC_BOOTLOADER
#include <logger.h>
#endif

uint16_t Psc_CableId = 0;
uint16_t Psc_ControllerId = 0;
uint8_t Psc_NodeId = 0xff;
uint8_t Psc_BoardType = 0xff;

#define M_ADC_33V_COUNTS_TO_VOLTS (3.3/4096.0)
#define M_ADC_33V_REFERENCE_DIVIDER ((10000.0+10000.0)/10000.0)
#define M_ADC_33V_FEC3_DIVIDER ((10000.0 + 38300.0) / 38300.0)
#define M_ADC_33V_FEC2_DIVIDER ((49900.0 + 10000.0) / 10000.0)

#define M_ADC_COUNTS_TO_VOLTS (3.0/4096.0)
#define M_ADC_FEC2_24V_DIVIDER ((110000.0 + 10000.0) / 10000.0)
#define M_ADC_FEC3_24V_DIVIDER ((80600.0 + 10000.0) / 10000.0)
#define M_ADC_5V_DIVIDER ((10000.0 + 10000.0) / 10000.0)

#define M_ADC_FEC2_COUNTS_TO_24V (M_ADC_COUNTS_TO_VOLTS * M_ADC_FEC2_24V_DIVIDER)
#define M_ADC_FEC3_COUNTS_TO_24V (M_ADC_COUNTS_TO_VOLTS * M_ADC_FEC3_24V_DIVIDER)
#define M_ADC_COUNTS_TO_5V (M_ADC_COUNTS_TO_VOLTS * M_ADC_5V_DIVIDER)
#define M_ADC_COUNTS_TO_33V (M_ADC_COUNTS_TO_VOLTS * M_ADC_5V_DIVIDER)

extern int optionCount;
extern char **optionArray;


#define NEW_TABLE
static const int C_CableIdAdcValues[] = {
#ifdef TABLE_1
        0,
        220,
        426,
        621,
        801,
        977,
        1128,
        1285,
        1441,
        1592,
        1733,
        1863,
        1980,
        2098,
        2216,
        2326,
        2431,
        2547,
        4096
#endif
#ifdef TABLE_2
        0,
        333,
        645,
        940,
        1214,
        1481,
        1709,
        1947,
        2184,
        2412,
        2626,
        2822,
        3000,
        3179,
        3357,
        3524,
        3684,
        3859,
#endif
#ifdef NEW_TABLE
        0,
        226,
        614,
        924,
        1206,
        1467,
        1700,
        1913,
        2127,
        2332,
        2522,
        2695,
        2850,
        2996,
        3139,
        3275,
        3402,
        3530
#endif
};

uint8_t Psc_SetIpAddress()
{
    return 0;
}

uint8_t Psc_GetLSBIpAddress()
{
    return 100+Psc_CableId;
}

#define M_DELAY_LOOP_CONSTANT 0x100000

uint16_t Psc_GetCableId()
{
    return Psc_CableId;
}

uint8_t Psc_GetNodeId()
{
    return 0;
}

uint8_t Psc_GetControllerId()
{
    return 0;
}

uint8_t Psc_GetBoardType()
{
        Psc_BoardType = E_BoardType_Fec3;
    //else
    //    Psc_BoardType = E_BoardType_Fec2;

    return Psc_BoardType;
}

uint8_t Psc_GetNumberOfSlaves()
{
    if (Psc_GetBoardType() == E_BoardType_Fec3)
        return 10;
    else
        return 8;
}

float Psc_GetBoardVoltage24V()
{
	return 24;
}

float Psc_GetBoardVoltage33V()
{
	return 3.3;
}

void Psc_SubsystemCheckInit()
{
	
}
