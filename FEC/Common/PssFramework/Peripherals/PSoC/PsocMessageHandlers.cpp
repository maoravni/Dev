/*
 * PsocMessageHandlerBase.cpp
 *
 *  Created on: 22 בדצמ 2013
 *      Author: maora
 */

#include "PsocMessageHandlers.h"
#include "PsocHandler.h"
#include <logger.h>

E_PsocSpiError PsocMessageHandlerBase::handleReceiveBuffer(PsocHandler* psocHandler, T_MessagePacket* processedBuffer, uint8_t serialNumber)
{
//    if (psocSpiResponseBuffer.transferComplete == 0 || psocSpiResponseBuffer.crcOk == 0)
//        M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "PSoC %d TC %d CRC OK %d", psocHandler->getPsocIndex(), psocSpiResponseBuffer.transferComplete, psocSpiResponseBuffer.crcOk);

    return copyReceivedBuffer(processedBuffer, serialNumber);
}

inline void PsocMessageHandlerBase::prepareTransmitBuffer(T_MessagePacket* transmitBuffer, uint8_t serialNumber)
{
    transmitBuffer->header.start = M_PSOC_SOH;
    transmitBuffer->header.commandStatus = getPsocCommand();
//    transmitBuffer->header.serialNumber = serialNumber;
}

E_PsocSpiError PMHReadTemperaturePwmDISensors::handleReceiveBuffer(PsocHandler* psocHandler,
        T_MessagePacket* processedBuffer, uint8_t serialNumber)
{
    E_PsocSpiError result = PsocMessageHandlerBase::handleReceiveBuffer(psocHandler, processedBuffer, serialNumber);

    if (result == E_PsocSpiError_Ok)
        psocHandler->updateShadowTemperaturePwmDiValues(processedBuffer->data.readTemperaturePwmDISensors);

    return result;
}

E_PsocSpiError PMHReadAnalogsFeedbacksErrors::handleReceiveBuffer(PsocHandler* psocHandler,
        T_MessagePacket* processedBuffer, uint8_t serialNumber)
{
    E_PsocSpiError result = PsocMessageHandlerBase::handleReceiveBuffer(psocHandler, processedBuffer, serialNumber);

    if (result == E_PsocSpiError_Ok)
        psocHandler->updateShadowAnalogsFeedbacksErrorsValues(processedBuffer->data.readAnalogsFeedbacksErrors);

    return result;
}

void PMHWriteMultipleDevices::prepareTransmitBufferPwm(T_MessagePacket* transmitBuffer, uint8_t serialNumber, uint8_t* pwmValues)
{
    PsocMessageHandlerBase::prepareTransmitBuffer(transmitBuffer, serialNumber);
    transmitBuffer->data.writeMultipleDevices.writeBitfield = 0xfc;
    memcpy(transmitBuffer->data.writeMultipleDevices.digitalOutputs, pwmValues, sizeof(transmitBuffer->data.writeMultipleDevices.digitalOutputs));
}

void PMHWriteMultipleDevices::prepareTransmitBufferAnalogOuts(T_MessagePacket* transmitBuffer, uint8_t serialNumber, uint8_t* analogValues)
{
    PsocMessageHandlerBase::prepareTransmitBuffer(transmitBuffer, serialNumber);
    transmitBuffer->data.writeMultipleDevices.writeBitfield = 0x3;
    memcpy(transmitBuffer->data.writeMultipleDevices.analogOutputs, analogValues, sizeof(transmitBuffer->data.writeMultipleDevices.analogOutputs));
}

//void PMHWriteMultipleDevices::handleReceiveBuffer(PsocHandler* psocHandler, T_MessagePacket* processedBuffer, uint8_t serialNumber)
//{
//    transmitBuffer->data.writeMultipleDevices.writeBitfield = 0;
//    PsocMessageHandlerBase::handleReceiveBuffer(PsocHandler, processedBuffer, serialNumber);
//}

void PMHGetCableID::prepareTransmitBuffer(T_MessagePacket* transmitBuffer, uint8_t serialNumber, float boardVoltage)
{
    PsocMessageHandlerBase::prepareTransmitBuffer(transmitBuffer, serialNumber);
    transmitBuffer->data.psocRequestWithVoltage.voltage = boardVoltage;
}

void PMHReadTemperaturePwmDISensors::prepareTransmitBufferOutputs(T_MessagePacket* transmitBuffer, uint8_t serialNumber,
        uint8_t* pwmValues, uint8_t* analogValues, float voltage)
{
    PsocMessageHandlerBase::prepareTransmitBuffer(transmitBuffer, serialNumber);
    transmitBuffer->data.writeMultipleDevices.writeBitfield = 0xfc | 0x3;
    memcpy(transmitBuffer->data.writeMultipleDevices.digitalOutputs, pwmValues, sizeof(transmitBuffer->data.writeMultipleDevices.digitalOutputs));
    memcpy(transmitBuffer->data.writeMultipleDevices.analogOutputs, analogValues, sizeof(transmitBuffer->data.writeMultipleDevices.analogOutputs));
    transmitBuffer->data.writeMultipleDevices.voltage = voltage;
}

void PMHReadAnalogsFeedbacksErrors::prepareTransmitBufferOutputs(T_MessagePacket* transmitBuffer, uint8_t serialNumber,
        uint8_t* pwmValues, uint8_t* analogValues, float voltage)
{
    PsocMessageHandlerBase::prepareTransmitBuffer(transmitBuffer, serialNumber);
    transmitBuffer->data.writeMultipleDevices.writeBitfield = 0xfc | 0x3;
    memcpy(transmitBuffer->data.writeMultipleDevices.digitalOutputs, pwmValues, sizeof(transmitBuffer->data.writeMultipleDevices.digitalOutputs));
    memcpy(transmitBuffer->data.writeMultipleDevices.analogOutputs, analogValues, sizeof(transmitBuffer->data.writeMultipleDevices.analogOutputs));
    transmitBuffer->data.writeMultipleDevices.voltage = voltage;
}
