/*
 * PscSubsystem.c
 *
 *  Created on: 4 Jul 2013
 *      Author: maora
 */

#include "PscSubsystem.h"
#include <logger.h>

uint16_t Psc_CableId;
uint8_t Psc_NodeId;
uint16_t Psc_ControllerId;
uint8_t ipByte3;

uint8_t Psc_SetIpAddress()
{
//    assert_param(DipSwitch<=7);
//    assert_param(SubSysType<=7);

    int ipByte0, ipByte1, ipByte2;

    Psc_CableId = Psc_GetCableId();
    Psc_NodeId = Psc_GetNodeId();
    Psc_ControllerId = (Psc_CableId & 0xf) << 4 | Psc_NodeId;

    ipByte0 = 172;
    ipByte1 = 30;
    ipByte2 = 30;

//    if (IsInAtpMode())
//        ipByte3 = 59;  //in ATP mode -> 172.30.30.59
//    else
//    {
        switch (Psc_CableId)
        {
        case E_PscSubsystem_Dryer:
        {  //172.30.30.50 - 172.30.30.57
            ipByte3 = 50 + Psc_NodeId;
            break;
        }
        case E_PscSubsystem_Ibt:
        { //172.30.30.60 - 172.30.30.67
            ipByte3 = 60 + Psc_NodeId;
            break;
        }
        case E_PscSubsystem_Ith:
        { //172.30.30.70 - 172.30.30.77
            ipByte3 = 70 + Psc_NodeId;
            break;
        }
        case E_PscSubsystem_Btc:
        { //172.30.30.80 - 172.30.30.871
            ipByte3 = 80 + Psc_NodeId;
            break;
        }
        case E_PscSubsystem_Conditioner:
            ipByte3 = 90 + Psc_NodeId;
            break;
        default:
            //172.30.30.59  - NO SSID dongle is plugged - or unsupported
            ipByte3 = 59;
            break;
        } //switch
//    }

    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "IP ADDRESS=%d.%d.%d.%d", ipByte0, ipByte1, ipByte2, ipByte3);
    //change the IP Addr
    change_ip_addr(ipByte0, ipByte1, ipByte2, ipByte3);

    return ipByte3;
}

uint16_t Psc_GetCableId()
{
#ifdef DRYER_DEBUG
    return 1;
#else
    return (uint16_t)Get_SSID_dig();
#endif
}

uint8_t Psc_GetNodeId()
{
#ifdef DRYER_DEBUG
    return 1;
#else
    return nod_id_get();
#endif
}

uint8_t Psc_GetControllerId()
{
}

uint8_t Psc_GetBoardType()
{
    return E_BoardType_Fec;
}

uint8_t Psc_GetNumberOfSlaves()
{
    return 0;
}

uint8_t Psc_GetLSBIpAddress()
{
    return ipByte3;
}
