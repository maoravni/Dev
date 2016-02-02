/*
 * PsocMessageHandlerBase.h
 *
 *  Created on: 22 בדצמ 2013
 *      Author: maora
 */

#ifndef PSOCMESSAGEHANDLERBASE_H_
#define PSOCMESSAGEHANDLERBASE_H_

#include <stdint.h>
#include <CBinarySemaphore.h>
#include "PsocMessageStruct.h"
#include <string.h>
#include <psocSpiWithDma.h>
#include <stdio.h>

extern "C" T_PsocSpiReceiveFrame psocSpiResponseBuffer;

class PsocHandler;

class PsocMessageHandlerBase
{
public:
    PsocMessageHandlerBase() {}
    virtual ~PsocMessageHandlerBase() {}
    virtual E_PsocSpiError handleReceiveBuffer(PsocHandler* psocHandler, T_MessagePacket* processedBuffer, uint8_t serialNumber);
    virtual void prepareTransmitBuffer(T_MessagePacket* transmitBuffer, uint8_t serialNumber);
    virtual E_PsocCommands getPsocCommand() = 0;

protected:
    inline E_PsocSpiError copyReceivedBuffer(T_MessagePacket* processedBuffer, uint8_t serialNumber)
    {
        memcpy(processedBuffer, psocSpiResponseBuffer.buffer, sizeof(T_MessagePacket));
        if (!psocSpiResponseBuffer.transferComplete)
            return E_PsocSpiError_NoSlaveResponse;
        if (!psocSpiResponseBuffer.crcOk)
            return E_PsocSpiError_CrcError;
        if (processedBuffer->header.serialNumber != serialNumber)
        {
            return E_PsocSpiError_BadSerialNumber;
        }
        return (E_PsocSpiError)processedBuffer->header.commandStatus;
    }
};

class PMHEmpty : public PsocMessageHandlerBase
{
public:
    virtual E_PsocCommands getPsocCommand(){ return E_PsocCommands_EmptyCommand; }
};

class PMHGetVersion : public PsocMessageHandlerBase
{
public:
    virtual E_PsocCommands getPsocCommand(){ return E_PsocCommands_GetPSoCversion; }
};

class PMHKeepAlive : public PsocMessageHandlerBase
{
public:
    virtual E_PsocCommands getPsocCommand() {return E_PsocCommands_KeepAlive;}
};

class PMHGetCableID : public PsocMessageHandlerBase
{
private:
    virtual void prepareTransmitBuffer(T_MessagePacket* transmitBuffer, uint8_t serialNumber){}

public:
    virtual void prepareTransmitBuffer(T_MessagePacket* transmitBuffer, uint8_t serialNumber, float boardVoltage);
    virtual E_PsocCommands getPsocCommand() {return E_PsocCommands_GetCableID;}
};

class PMHGetErrors : public PsocMessageHandlerBase
{
public:
};

class PMHGetWarnings : public PsocMessageHandlerBase
{
public:
};

class PMHWriteSingleDevice : public PsocMessageHandlerBase
{
public:
};

class PMHGetDeviceErrors : public PsocMessageHandlerBase
{
public:
};

class PMHGetDeviceWarnings : public PsocMessageHandlerBase
{
public:
};

class PMHReadTemperatureSensors : public PsocMessageHandlerBase
{
public:
};

class PMHReadPWMs : public PsocMessageHandlerBase
{
public:
};

class PMHReadDigitalIO : public PsocMessageHandlerBase
{
public:
};

class PMHReadAnalogInputs : public PsocMessageHandlerBase
{
public:
};

class PMHReadBridgeCurrents : public PsocMessageHandlerBase
{
public:
};

class PMHReadCurrentLoops : public PsocMessageHandlerBase
{
public:
};

class PMHReadDigitalInputs : public PsocMessageHandlerBase
{
public:
};

class PMHResetBoardToOn : public PsocMessageHandlerBase
{
public:
};

class PMHStartErrorRecovery : public PsocMessageHandlerBase
{
public:
    virtual E_PsocCommands getPsocCommand(){ return E_PsocCommands_StartErrorRecovery; }
};

class PMHStartBoardConfig : public PsocMessageHandlerBase
{
public:
    virtual E_PsocCommands getPsocCommand(){ return E_PsocCommands_StartBoardConfig; }
};

class PMHEndBoardConfig : public PsocMessageHandlerBase
{
public:
    virtual E_PsocCommands getPsocCommand(){ return E_PsocCommands_EndBoardConfig; }
};

class PMHResetPSoC : public PsocMessageHandlerBase
{
public:
};

class PMHWriteMultipleDevices : public PsocMessageHandlerBase
{
private:
    virtual void prepareTransmitBuffer(T_MessagePacket* transmitBuffer, uint8_t serialNumber){}

public:
    virtual void prepareTransmitBufferPwm(T_MessagePacket* transmitBuffer, uint8_t serialNumber, uint8_t* pwmValues);
    virtual void prepareTransmitBufferAnalogOuts(T_MessagePacket* transmitBuffer, uint8_t serialNumber, uint8_t* analogValues);
    virtual E_PsocCommands getPsocCommand() {return E_PsocCommands_WriteMultipleDevice; }
//    virtual E_PsocSpiError handleReceiveBuffer(PsocHandler* psocHandler, T_MessagePacket* processedBuffer, uint8_t serialNumber);
};

class PMHReadTemperaturePwmDISensors : public PsocMessageHandlerBase
{
private:
    virtual void prepareTransmitBuffer(T_MessagePacket* transmitBuffer, uint8_t serialNumber){}

public:
    virtual void prepareTransmitBufferOutputs(T_MessagePacket* transmitBuffer, uint8_t serialNumber, uint8_t* pwmValues, uint8_t* analogValues, float voltage);
    virtual E_PsocSpiError handleReceiveBuffer(PsocHandler* psocHandler, T_MessagePacket* processedBuffer, uint8_t serialNumber);
    virtual E_PsocCommands getPsocCommand(){ return E_PsocCommands_ReadTemperaturePwmDISensors; }
};

class PMHReadAnalogsFeedbacksErrors : public PsocMessageHandlerBase
{
private:
    virtual void prepareTransmitBuffer(T_MessagePacket* transmitBuffer, uint8_t serialNumber){}

public:
    virtual void prepareTransmitBufferOutputs(T_MessagePacket* transmitBuffer, uint8_t serialNumber, uint8_t* pwmValues, uint8_t* analogValues, float voltage);
    virtual E_PsocSpiError handleReceiveBuffer(PsocHandler* psocHandler, T_MessagePacket* processedBuffer, uint8_t serialNumber);
    virtual E_PsocCommands getPsocCommand(){ return E_PsocCommands_ReadAnalogsFeedbacksErrors; }
};

class PMHGetPsocStatus : public PsocMessageHandlerBase
{
public:
    virtual E_PsocCommands getPsocCommand(){ return E_PsocCommands_GetStatus; }
};

class PMHConfigTemperatureSensor : public PsocMessageHandlerBase
{
public:
    virtual E_PsocCommands getPsocCommand() {return E_PsocCommands_ConfigTemperatureSensor; }
};

class PMHConfigCurrentLoop : public PsocMessageHandlerBase
{
public:
    virtual E_PsocCommands getPsocCommand() {return E_PsocCommands_ConfigCurrentLoop; }
};

class PMHConfigDigitalIO : public PsocMessageHandlerBase
{
public:
    virtual E_PsocCommands getPsocCommand() {return E_PsocCommands_ConfigDigitalIO; }
};

class PMHConfigAnalogIO : public PsocMessageHandlerBase
{
public:
    virtual E_PsocCommands getPsocCommand() {return E_PsocCommands_ConfigAnalogIO; }
};

class PMHConfigChannelFunction : public PsocMessageHandlerBase
{
public:
    virtual E_PsocCommands getPsocCommand() {return E_PsocCommands_ConfigChannelFunction; }
};

class PMHStartBootloader : public PsocMessageHandlerBase
{
public:
    virtual E_PsocCommands getPsocCommand() {return E_PsocCommands_StartBootloader; }
};


#endif /* PSOCMESSAGEHANDLERBASE_H_ */
