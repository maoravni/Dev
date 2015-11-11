/*
 * PscSubsystem.h
 *
 *  Created on: 4 Jul 2013
 *      Author: maora
 */

#ifndef PSCSUBSYSTEM_H_
#define PSCSUBSYSTEM_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

typedef enum
{
    E_BoardType_Fec,
    E_BoardType_Fec2,
    E_BoardType_Fec3,
    //E_BoardType_TemperatureControlPSoC,
}E_BoardType;

typedef enum {
  E_PscSubsystem_Error,  //000
  E_PscSubsystem_Dryer, //001
  E_PscSubsystem_Ibt,  //010
  E_PscSubsystem_Ith,  //011
  E_PscSubsystem_Btc,  //100
  E_PscSubsystem_Conditioner,  //101
  E_PscSubsystem_Not_Used,  //  110
  E_PscSubsystem_Atp,  //111

  E_PscSubsystem_Size
} E_PscSubsystem;

extern uint16_t Psc_CableId;
extern uint16_t Psc_ControllerId;
extern uint8_t Psc_NodeId;
extern uint8_t Psc_BoardType;

//uint8_t Psc_SetIpAddress(E_PscSubsystem subsystem, uint8_t dipSwitchValue);
uint8_t Psc_SetIpAddress();
uint8_t Psc_GetLSBIpAddress();
uint16_t Psc_GetCableId();
uint8_t Psc_GetNodeId();
//uint8_t Psc_GetControllerId();
uint8_t Psc_GetBoardType();
uint8_t Psc_GetNumberOfSlaves();
float Psc_GetBoardVoltage24V();
float Psc_GetBoardVoltage33V();

#ifdef __cplusplus
}
#endif

#endif /* PSCSUBSYSTEM_H_ */
