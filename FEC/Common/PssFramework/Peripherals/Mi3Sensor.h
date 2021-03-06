/*
 * Mi3Sensor.h
 *
 *  Created on: 9 ���� 2014
 *      Author: maora
 */

#ifndef MI3SENSOR_H_
#define MI3SENSOR_H_

#include <Elements/ElementRepository.h>
#include <list>
#include <I2C_Handler.h>
//#include <cpal.h>
//#include <cpal_i2c.h>

enum E_Mi3Registers
{
    E_Mi3Registers_Name = 0x01, //R/W
    E_Mi3Registers_Head_ID = 0x02, //R
    E_Mi3Registers_Serial_Num = 0x03, //R
    E_Mi3Registers_Version = 0x04, //R
    E_Mi3Registers_TargetTemp = 0x05, //R
    E_Mi3Registers_Amb_Temp = 0x06, //R
    E_Mi3Registers_Targ_Amb_Temp = 0x07, //R
    E_Mi3Registers_Emissivity = 0x08, //R
    E_Mi3Registers_Transmissivity = 0x09, //R
    E_Mi3Registers_BottomTemp = 0x0A, //R/W
    E_Mi3Registers_TopTemp = 0x0B, //R/W
    E_Mi3Registers_Status = 0x0C, //R/W[0,7]
    E_Mi3Registers_Head_Address = 0x0D, //R/W
    E_Mi3Registers_Special_Code = 0x11, //R/W
    E_Mi3Registers_Head_IDENT0 = 0x12, //R
    E_Mi3Registers_Head_IDENT1 = 0x13, //R
    E_Mi3Registers_Head_IDENT2 = 0x14, //R
    E_Mi3Registers_Head_IDENT3 = 0x15, //R
    E_Mi3Registers_AdjGain = 0x1A, //R/W
    E_Mi3Registers_AdjOffset = 0x1B, //R/W
    E_Mi3Registers_Detector_Power = 0x1C, //R
    E_Mi3Registers_AmbBackground = 0x1D, //R/W
    E_Mi3Registers_Factory_Default = 0x1F, //W
    E_Mi3Registers_Flicker_Filter = 0x28, //R/W
    E_Mi3Registers_Laser_Control = 0x3E, //R/W
    E_Mi3Registers_Cali_Date = 0x40, //R
    E_Mi3Registers_Cali_Time = 0x41, //R
    E_Mi3Registers_Cali_Amb_Low_Temp = 0x42, //R
    E_Mi3Registers_Cali_Amb_High_Temp = 0x43, //R
    E_Mi3Registers_Cali_Source_Temp1 = 0x44, //R
    E_Mi3Registers_Cali_Measure_Temp1 = 0x45, //R
    E_Mi3Registers_Cali_Source_Temp2 = 0x46, //R
    E_Mi3Registers_Cali_Measure_Temp2 = 0x47, //R
    E_Mi3Registers_Cali_Source_Temp3 = 0x48, //R
    E_Mi3Registers_Cali_Measure_Temp3 = 0x49, //R
    E_Mi3Registers_Cali_Source_Temp4 = 0x4A, //R
    E_Mi3Registers_Cali_Measure_Temp4 = 0x4B, //R
    E_Mi3Registers_Cali_Source_Temp5 = 0x4C, //R
    E_Mi3Registers_Cali_Measure_Temp5 = 0x4D  //R
};

enum E_I2cErrors
{
    E_I2cErrors_Timeout = 0x1,
    E_I2cErrors_RegAddressError = 0x2,
    E_I2cErrors_ChecksumError = 0x4,
};

union Mi3Data
{
    char st[4];
    uint32_t ui;
    int32_t i;
    float f;
};

struct Mi3TxPacket
{
    uint8_t address;
    uint8_t reg;
    Mi3Data data;
};

struct Mi3RxPacket
{
    Mi3Data data;
    uint8_t reg2;
    uint8_t checksum;
};

class SensorConfiguration
{
public:
    bool m_ambientBackgroundCompensation;
    float m_emissivity;
    float m_transmissivity;
    float m_topTemp;
    float m_bottomTemp;
    float m_gain;
    float m_offset;
    float m_ambientBackground;
//    float m_flickerFilter;

    SensorConfiguration();
};

#define M_NUMBER_OF_RETRIES 10

class Mi3Sensor
{
private:
    ValidationElementFloat* m_targTempElement;
    uint8_t m_address;

    SensorConfiguration m_sensorConfiguration;

    uint8_t m_status;
    uint8_t m_numberOfReadFailures;

    I2C_TypeDef* m_i2cChannel;
    xSemaphoreHandle* m_semaphoreHandle;

public:
    Mi3Sensor();
    Mi3Sensor(F_FILE* f);

    void setTargTempElementPssId(int pssId);

    bool readTargetTemp();
    bool readStatus();

    void setAmbientBackgroundCompensation(bool val);
    void setEmissivity(float val);
    void setTransmissivity(float val);
    void setBottomTemp(float val);
    void setTopTemp(float val);
    void setGain(float val);
    void setOffset(float val);
    void setAmbientBackground(float val);
    bool downloadConfiguraton();

    uint8_t getAddress() const
    {
        return m_address;
    }

    void setAddress(uint8_t address)
    {
        m_address = address;
    }

    ValidationElementFloat* getTargTempElement() const
    {
        return m_targTempElement;
    }

    uint16_t getPssId()
    {
        return m_targTempElement->getPssId();
    }

    bool setI2CChannel(int channelIndex);
    int getI2CChannel();

private:
    uint8_t read(uint8_t reg, float &data);
    uint8_t read(uint8_t reg, uint32_t &data);
    uint8_t read(uint8_t reg, int32_t &data);
    uint8_t read(uint8_t reg, uint8_t* data);
    uint8_t write(uint8_t reg, float &data);
    uint8_t write(uint8_t reg, uint32_t &data);
    uint8_t write(uint8_t reg, int32_t &data);
    uint8_t write(uint8_t reg, uint8_t* data);

    bool writeAmbientBackgroundCompensation(bool val);
    bool writeEmissivity(float val);
    bool writeTransmissivity(float val);
    bool writeBottomTemp(float val);
    bool writeTopTemp(float val);
    bool writeGain(float val);
    bool writeOffset(float val);
    bool writeAmbientBackground(float val);

    template <class T> friend class Serializer;

};

#define M_I2C_IR_SENSOR_BAUD_RATE 10000
#define M_I2C_EEPROM_BAUD_RATE 200000

typedef std::vector<Mi3Sensor*> T_SensorList;
typedef std::vector<Mi3Sensor*>::iterator T_SensorListIterator;

#endif /* MI3SENSOR_H_ */
