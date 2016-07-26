/*
 * DeviceRepository.h
 *
 *  Created on: 10 Jun 2013
 *      Author: maora
 */

#ifndef DEVICEREPOSITORY_H_
#define DEVICEREPOSITORY_H_

#include <DynamicArray.h>
#include "PeripheralBase.h"
#include "InternalTemperatureSensors.h"
#include "DigitalOutputsPeripheral.h"
#include "SwPwmOutputPeripheral.h"
#include "Modbus6RTDPeripheral.h"
#include "Modbus8TCPeripheral.h"
#include "ModbusDataCardPeripheral.h"
#include "ModbusPumaPeripheral.h"
#include "DryContactDigitalOutput.h"
#include "DigitalInputsPeripheral.h"
#include "AnalogOutCurrentPeripheral.h"
#include "ModbusInverterPeripheralBase.h"
#include "ModbusInverterCommanderSK.h"
#include "ModbusInverterUnidriveM200.h"
#include "AnalogInputPeripheral.h"
#include "ModbusInverterSchneiderAtv32.h"
#include "ModbusSmcHrsChiller.h"
#include "VirtualPeripheral.h"
#ifdef FEC2_BOARD
#include "Mi3I2CIrPeripheral.h"
#endif
//#include "Persistency/PeripheralSerializers.h"


typedef DynamicArray<PeripheralBase*> T_PeripheralVector;

class PeripheralRepository
{
private:
    static PeripheralRepository s_instance; //!< Instance of the telnet server singleton.

    T_PeripheralVector m_periphVector;

    PeripheralRepository();

    //initialize the on-board devices by default on instantiation:
    InternalTemperatureSensors *m_internalTemperatureSensors;
    DigitalOutputsPeripheral *m_digitalOutputs;
    SwPwmOutputPeripheral *m_swPwmOutput;
    DryContactDigitalOutput* m_dryContactOutput;
    DigitalInputsPeripheral* m_digitalInputs;
    AnalogInputPeripheral* m_analogInputs;
    AnalogOutCurrentPeripheral* m_analogCurrentOutputs;
    Mi3I2CIrPeripheral* m_mi3i2cIrSensorPeripheral;

public:
    virtual ~PeripheralRepository();

    static inline PeripheralRepository& getInstance()
    {
        return s_instance;
    }

    void setBoardInReady(bool state);
    void startRecovery();

    void initInternalPeripherals();
    bool initInternalTemperatureSensors(int pssId, int sampleInterval, int lpfWindow, int numberOfDevices);
    bool initDigitalOutputs(int pssId, int numberOfDevices);
    bool initSwPwmOutput(int pssId, int numberOfDevices);
    bool initDryContactOutput(int pssId);
    bool initDigitalInputs(int pssId, int numberOfDevices);
    bool initAnalogInputs(int pssId, int numberOfDevices);
    bool initAnalogCurrentOutputs(int pssId, int numberOfDevices);
    bool initMi3IrSensorsPeripheral(int pssId);

    void addPeripheral(PeripheralBase* device);

    PeripheralBase* getPeripheralByPssId(int pssId);
    PeripheralBase* getPeripheralByIndex(int index);
    PeripheralBase* getPeripheralContainingElementPssId(int pssId);
    int getPeripheralCount();

    InternalTemperatureSensors* getInternalTemperatureSensorsPeripheral();
    DigitalOutputsPeripheral* getDigitalOutputsPeripheral();
    SwPwmOutputPeripheral* getSwPwmOutputPeripheral();
    DigitalInputsPeripheral* getDigitalInputsPeripheral() {return m_digitalInputs;}
    Mi3I2CIrPeripheral* getMi3I2cIrPeripheral() {return m_mi3i2cIrSensorPeripheral;}

    void destroyAllPeripherals();

    DryContactDigitalOutput* getDryContactOutput() const
    {
        return m_dryContactOutput;
    }

    template <class T> friend class Serializer;

};


#endif /* DEVICEREPOSITORY_H_ */
