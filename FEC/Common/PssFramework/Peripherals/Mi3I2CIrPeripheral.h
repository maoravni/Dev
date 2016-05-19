/*
 * Mi3I2CIrPeripheral.h
 *
 *  Created on: 9 αιεπ 2014
 *      Author: maora
 */

#ifndef MI3I2CIRPERIPHERAL_H_
#define MI3I2CIRPERIPHERAL_H_

#include "InputPeripheralBase.h"
#include <Elements/ElementRepository.h>
#include <list>
#include "Mi3Sensor.h"
//#include <cpal.h>
//#include <cpal_i2c.h>

//typedef std::list<ValidationElementFloat*> T_ElementList;
//typedef std::list<ValidationElementFloat*>::iterator T_ElementListIterator;

/** This peripheral is different to the other peripherals:
 * - The number of devices is undefined in advance.
 * - Each device can have a different address.
 *
 * To deal with this, when we request an element by index, we actually define that device/element.
 * From this moment when the peripheral reads its devices (readInputs) all devices will be read,
 * each according to its "index", which means the address.
 * Note that addresses with mask 0xa // 0x5 are reserved for the EEPROM.
 */

class Mi3I2CIrPeripheral: public InputPeripheralBase, public AManagedTask
{
private:
    T_SensorList m_sensorList;
    portTickType m_previousWakeupTime;

    ElementU32* m_resetCount;

    GPIO_TypeDef* m_powerGpioPort;
    uint16_t m_powerGpioPin;

#ifdef CPAL_USE_I2C1
    CPAL_TransferTypeDef m_rxStruct;
    CPAL_TransferTypeDef m_txStruct;
    I2C_InitTypeDef I2C_InitStruct;
    uint8_t m_rxBuffer[6];
    uint8_t m_txBuffer[6];
#endif

    bool m_performReset;

public:
//    static CBinarySemaphore m_transferCompleteSemaphore;

public:
    Mi3I2CIrPeripheral();
    Mi3I2CIrPeripheral(F_FILE* f);
    virtual ~Mi3I2CIrPeripheral();

    /**
     * Task control loop.
     */
    virtual void run();

    virtual void readInputs();

    virtual E_PeripheralType getPeripheralType() {return E_PeripheralType_MI3IRSensorsPeripheral;}
    virtual int getElementCount() {return m_sensorList.size();}

    virtual ElementBase* getElementByIndex(int address);
    virtual ElementBase* getElementByPssId(int pssId);
    virtual void enableElementByIndex(int index, bool enable);

    Mi3Sensor* getSensorByAddress(int address);
    bool configureSensor(int address, SensorConfiguration &sensorConfig);
    void setResetCountPssId(uint16_t pssId);

    virtual void setUpdateInterval(uint16_t updateInterval);

    virtual portBASE_TYPE onCreate(const portCHAR * const pcName, unsigned portSHORT usStackDepth, unsigned portBASE_TYPE uxPriority);

    virtual void setBoardInReady(bool state);

    virtual void startRecovery();

    void powerEnable()
    {
        if (m_powerGpioPort != NULL)
            m_powerGpioPort->BSRRH = m_powerGpioPin;
    }
    void powerDisable()
    {
        if (m_powerGpioPort != NULL)
            m_powerGpioPort->BSRRL = m_powerGpioPin;
    }

    virtual void serialize(F_FILE* f);
    virtual int deserialize(F_FILE* f);

    template <class T> friend class Serializer;

};

#endif /* MI3I2CIRPERIPHERAL_H_ */
