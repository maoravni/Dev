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
#include <stm32f4xx_rcc.h>
#include <continousA2D.h>

uint16_t Psc_CableId = 0xffff;
uint16_t Psc_ControllerId = 0xffff;
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
//    assert_param(DipSwitch<=7);
//    assert_param(SubSysType<=7);

    int ipByte0, ipByte1, ipByte2, ipByte3;

    Psc_CableId = Psc_GetCableId();
    Psc_ControllerId = Psc_CableId + 0x100;

    ipByte0 = 172;
    ipByte1 = 30;
    ipByte2 = 30;

    ipByte3 = 100+Psc_CableId;

#ifdef FEC_BOOTLOADER
    printf("IP ADDRESS=%d.%d.%d.%d", ipByte0, ipByte1, ipByte2, ipByte3);
#else
    M_LOGGER_LOGF(M_LOGGER_LEVEL_INFO, "IP ADDRESS=%d.%d.%d.%d", ipByte0, ipByte1, ipByte2, ipByte3);
#endif

    //change the IP Addr
    change_ip_addr(ipByte0, ipByte1, ipByte2, ipByte3);

    return ipByte3;
}

uint8_t Psc_GetLSBIpAddress()
{
    return 100+Psc_CableId;
}

#define M_DELAY_LOOP_CONSTANT 0x100000

uint16_t Psc_GetCableId()
{
#ifdef DRYER_DEBUG
    return 4;
#else
    int adcValue;
    int i;
    float scale;

    vTaskDelay(1000);

    Psc_GetBoardType();

    scale = 24 / Psc_GetBoardVoltage24V();

    adcValue = (int)(a2d_filtered_arr[ADC1_in8] * scale);

    for (i = 0; (i < sizeof(C_CableIdAdcValues) && adcValue > C_CableIdAdcValues[i]); ++i);

    Psc_CableId = (i-1);


    return Psc_CableId;
#endif
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
    int adcValue;
    int i;
    float scale;

    //vTaskDelay(1000);

    scale = 3.3 / Psc_GetBoardVoltage33V();

    adcValue = (int)(a2d_filtered_arr[ADC3_in6] * scale);

    if (adcValue > 3000)
        Psc_BoardType = E_BoardType_Fec3;
    else
        Psc_BoardType = E_BoardType_Fec2;

    return Psc_BoardType;
}

uint8_t Psc_GetNumberOfSlaves()
{
    Psc_GetBoardType();
    if (Psc_BoardType == E_BoardType_Fec3)
        return 10;
    else
        return 8;
}

float Psc_GetBoardVoltage24V()
{
    if (Psc_BoardType == E_BoardType_Fec3)
        return a2d_filtered_arr[ADC3_in5] * M_ADC_FEC3_COUNTS_TO_24V;
    else
        return a2d_filtered_arr[ADC3_in5] * M_ADC_FEC2_COUNTS_TO_24V;
}

float Psc_GetBoardVoltage33V()
{
    return a2d_filtered_arr[ADC1_in13] * M_ADC_COUNTS_TO_33V;
}

void Psc_SubsystemCheckInit()
{
}
