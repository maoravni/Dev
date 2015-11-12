/*
 * DeviceBase.h
 *
 *  Created on: 9 Jun 2013
 *      Author: maora
 */

#ifndef DEVICEBASE_H_
#define DEVICEBASE_H_

#include <stdint.h>
#include <list>
#include <vector>

class ElementBase;

enum E_PeripheralType
{
    E_PeripheralType_AI = 0,
    E_PeripheralType_AO = 1,
    E_PeripheralType_DI = 2,
    E_PeripheralType_DO = 3,
    E_PeripheralType_InternalTemperature = 4,
    E_PeripheralType_GenericModbus = 5,
    E_PeripheralType_Puma = 6,
    E_PeripheralType_6RTD = 7,
    E_PeripheralType_8TC = 8,
    E_PeripheralType_DataCard = 9,
    E_PeripheralType_SwPwm = 10,
    E_PeripheralType_DryContact = 11,
    E_PeripheralType_Inverter = 12,
    E_PeripheralType_DIFeedback = 13,
    E_PeripheralType_BridgeCurrent = 14,
    E_PeripheralType_CurrentLoop = 15,
    E_PeripheralType_HwPWM = 16,
    E_PeripheralType_MI3IRSensorsPeripheral = 17,
    E_PeripheralType_SmcHrsChiller = 18,
    E_PeripheralType_VirtualFloatPeripheral = 19,
    E_PeripheralType_VirtualIntegerPeripheral = 20,
    E_PeripheralType_Size
};

class UpdateSchedulerTaskBase;

class PeripheralBase
{
private:
    uint16_t m_pssId; //!< Device ID in the SUM context.
    uint16_t m_peripheralRepIndex; //!< Device Index in the device repository

protected:
    uint16_t m_updateInterval; //!< Interval for the next update of the device.
    bool m_boardInReady;

public:
    PeripheralBase();
    virtual ~PeripheralBase();

    virtual void execute() = 0;

    virtual E_PeripheralType getPeripheralType() = 0;
    virtual int getElementCount() = 0;

    virtual uint16_t getPssId() const;
    virtual void setPssId(uint16_t pssId);
    uint16_t getPeripheralRepIndex() const;

    void setPeripheralRepIndex(uint16_t deviceRepIndex);
    uint16_t getUpdateInterval() const;
    virtual void setUpdateInterval(uint16_t updateInterval);
    virtual UpdateSchedulerTaskBase* getSchedulerTask();
    virtual void setBoardInReady(bool state);

    virtual ElementBase* getElementByIndex(int index) = 0;
    virtual ElementBase* getElementByPssId(int pssId) = 0;

    /**
     * In some peripherals we want to have the option for the peripheral to ignore some of the elements.
     * Or in other cases, like the digital outputs/sw pwm/hw pwm, where they share outputs,
     * we want to have only one peripheral use the output.
     * @param index
     * @return
     */
    virtual void enableElementByIndex(int index, bool enable) = 0;

    virtual void startRecovery() {}
};

typedef std::vector<PeripheralBase*> T_PeripheralList;
typedef std::vector<PeripheralBase*>::iterator T_PeripheralListIterator;

#endif /* DEVICEBASE_H_ */
