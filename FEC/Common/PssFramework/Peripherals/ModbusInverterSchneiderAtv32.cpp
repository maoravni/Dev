/*
 * ModbusInverterSchneiderAtv32.cpp
 *
 *  Created on: 11 Jan 2015
 *      Author: maora
 */

#include "Peripherals/ModbusInverterSchneiderAtv32.h"
#include <logger.h>
#include "Persistency/ModbusPeripheralSerializers.h"

#define M_NUMBER_OF_RETRIES 10
#define M_INVERTER_SAMPLE_INTERVAL 200
#define M_INVERTER_FREQUENCY_SETPOINT_ADDRESS 8502
#define M_INVERTER_CONTROL_WORD_ADDRESS 8501
#define M_INVERTER_RESET_ADDRESS 7128
#define M_INVERTER_OUTPUT_FREQUENCY_ADDRESS 3202
#define M_INVERTER_OUTPUT_CURRENT_ADDRESS 3204
#define M_INVERTER_LOGIC_INPUTS_ADDRESS 5202
#define M_INVERTER_AUTOTUNE 9608
#define M_INVERTER_AUTOTUNE_STATE 9609
#define M_INVERTER_AUTOMATIC_AUTOTUNE 9615
#define M_INVERTER_COM_SCAN_INPUT_ADDRESS 12741
#define M_INVERTER_FAULT_ADDRESS 7121

//#define M_OUTPUT_ENABLE_RETRIES 10
struct T_FaultToString
{
    uint16_t code;
    char* message;
};

static const T_FaultToString g_Atv32FaultToString[] =
{
{ 0, "[No fault] (nOF)" },
{ 1, "[Calibration error] (InF)" },
{ 2, "[Control Eeprom] (EEF1)" },
{ 3, "[Incorrect config.] (CFF)" },
{ 4, "[Invalid config.] (CFI)" },
{ 5, "[Modbus com.] (SLF1)" },
{ 6, "[int. com.link] (ILF)" },
{ 7, "[Com. network] (CnF)" },
{ 8, "[External flt-LI/Bit] (EPF1)" },
{ 9, "[Overcurrent] (OCF)" },
{ 10, "[Precharge] (CrF)" },
{ 11, "[Speed fdback loss] (SPF)" },
{ 16, "[Drive overheat] (OHF)" },
{ 17, "[Motor overload] (OLF)" },
{ 18, "[Overbraking] (ObF)" },
{ 19, "[Mains overvoltage] (OSF)" },
{ 20, "[1 output phase loss] (OPF1)" },
{ 21, "[Input phase loss] (PHF)" },
{ 22, "[Undervoltage] (USF)" },
{ 23, "[Motor short circuit] (SCF1)" },
{ 24, "[Overspeed] (SOF)" },
{ 25, "[Auto-tuning] (tnF)" },
{ 26, "[Rating error] (InF1)" },
{ 27, "[PWR Calib.] (InF2)" },
{ 28, "[Int.serial link] (InF3)" },
{ 29, "[Int.Mfg area] (InF4)" },
{ 30, "[Power Eeprom] (EEF2)" },
{ 31, "[Impedant sh. circuit] (SCF2)" },
{ 32, "[Ground short circuit] (SCF3)" },
{ 33, "[3out ph loss] (OPF2)" },
{ 34, "[CAN com.] (COF)" },
{ 35, "[Brake control] (bLF)" },
{ 38, "[External fault com.] (EPF2)" },
{ 41, "[Brake feedback] (brF)" },
{ 42, "[PC com.] (SLF2)" },
{ 43, "[Enc. coupl.] (ECF)" },
{ 44, "[Torque/current lim] (SSF)" },
{ 45, "[HMI com.] (SLF3)" },
{ 46, "[Power removal] (PrF)" },
{ 49, "[LI6=PTC probe] (PtFL)" },
{ 50, "[PTC fault] (OtFL)" },
{ 51, "[Internal- I measure] (InF9)" },
{ 52, "[Internal-mains circuit] (InFA)" },
{ 53, "[Internal- th. sensor] (InFb)" },
{ 54, "[IGBT overheat] (tJF)" },
{ 55, "[IGBT short circuit] (SCF4)" },
{ 56, "[Motor short circuit] (SCF5)" },
{ 57, "[Torque time-out] (SrF)" },
{ 58, "[Out. contact. stuck] (FCF1)" },
{ 59, "[Out. contact. open.] (FCF2)" },
{ 61, "[AI2 input] (AI2F)" },
{ 64, "[input contactor] (LCF)" },
{ 66, "[Diff. I fault] (dCF)" },
{ 67, "[IGBT desaturation] (HdF)" },
{ 68, "[Internal-option] (InF6)" },
{ 69, "[internal- CPU] (InFE)" },
{ 71, "[AI3 4-20mA loss] (LFF3)" },
{ 73, "[Cards pairing] (HCF)" },
{ 76, "[Load fault] (dLF)" },
{ 77, "[Bad conf] (CFI2)" },
{ 99, "[Ch.sw. fault] (CSF)" },
{ 100, "[Pr.Underload.Flt] (ULF)" },
{ 101, "[Proc.Overload Flt] (OLC)" },
{ 105, "[Angle error] (ASF)" },
{ 107, "[Safety fault] (SAFF)" },
{ 108, "[FB fault] (FbE)" },
{ 109, "[FB stop flt.] (FbES)" }, };

struct T_Atv32StatusWordBits
{
    uint8_t readyToSwitchOn :1;
    uint8_t ready :1;
    uint8_t running :1;
    uint8_t faultDetected :1;

    uint8_t powerPresent :1;
    uint8_t emrStop :1;
    uint8_t switchOnDisable :1;
    uint8_t alarm :1;

    uint8_t reserved0 :1;
    uint8_t remoteCommand :1;
    uint8_t targetReached :1;
    uint8_t exceededLimits :1;

    uint8_t reserved1 :1;
    uint8_t reserved2 :1;
    uint8_t stopPressed :1;
    uint8_t direction :1;
};

union T_Atv32StatusWord
{
    uint16_t word;
    T_Atv32StatusWordBits bits;
};

struct T_Atv32RegisterSetup
{
    uint16_t regAddress;
    uint16_t value;
};

struct T_DriveReadReply
{
    int16_t setpoint;
    int16_t outputFrequency;
    int16_t outputCurrent;
    uint16_t controlMode;
    uint16_t ioWord;
    uint16_t driveStatus;
};

/**
 * During the Unidrive M200 setup we remap menu 0, so we can read multiple registers at the same time.
 */
static const T_Atv32RegisterSetup Atv32RegisterSetup[] =
{
{ 8401, 2 }, // set control mode to "separate"
        { 8413, 164 }, // set frequency reference 1 to modbus
        { 7010, 0 }, // disable modbus fault on timeout
        { 3107, 1 }, // noise reduction
        { 11102, 0 }, // signal is on level
        { 7122, 1 }, // automatic restart
        { 7123, 6 }, // unlimited restart time
        { 9003, 1 }, // default Dec Ramp Adapt to yes, if no breaking resistor is present.
        { 11230, 10 }, // default fast stop ramp divider is 10.
        { 11204, 133 }, // assign LI5 to fast stop.
        { M_INVERTER_AUTOMATIC_AUTOTUNE, 0 },
        // setup continuous modbus addresses:
        { 12701, 3201 }, // status word
        { 12702, M_INVERTER_OUTPUT_FREQUENCY_ADDRESS }, // output frequency
        { 12703, M_INVERTER_OUTPUT_CURRENT_ADDRESS }, // output current
        { 12704, M_INVERTER_LOGIC_INPUTS_ADDRESS }, // logic input states.
        { 0, 0 } };

//static const T_Atv32RegisterSetup Atv32DryerRegisterSetup[] =
//{
//{ 9602, 1100 }, // nominal frequency - 110
//        { 9604, 6400 }, // nominal RPM - 6400
//        { 9603, 80 }, // nominal current - 8 A
//        { 0, 0 } };

//static const uint16_t AllowedDryerInverterPssId[] =
//{ 13331, 13776, 29714, 29719, 46097, 46546, 62480, 62931 };

ModbusInverterSchneiderAtv32::ModbusInverterSchneiderAtv32(uint8_t slaveId) :
        ModbusInverterPeripheralBase(slaveId)
{
    setInverterType();
    m_minSpeed = (int16_t) 0x8000;
    m_maxSpeed = (int16_t) 0x8000;
    m_accelRate = (int16_t) 0x8000;
    m_decelRate = (int16_t) 0x8000;
    m_nominalRpm = (int16_t) 0x8000;
    m_nominalCurrent = (int16_t) 0x8000;
    m_nominalFrequency = (int16_t) 0x8000;

    m_lastFaultCode = 0;

    m_driveStatus = ElementRepository::getInstance().addValidationElementU16();
}

ModbusInverterSchneiderAtv32::~ModbusInverterSchneiderAtv32()
{
}

void ModbusInverterSchneiderAtv32::updateOutputs()
{
    m_setpointUpdated = true;
//    writeSingleReg(getSlaveId(), M_INVERTER_FREQUENCY_SETPOINT_ADDRESS,
//            (int16_t)(m_frequencySetpoint->getValueF() * m_setpointMultiplier));
}

void ModbusInverterSchneiderAtv32::readInputs()
{
    int16_t outFreq;
    int16_t outCurrent;
    uint16_t logicInputs;
    T_Atv32StatusWord statusWord;
    int16_t inverterData[4];
    uint32_t buffLength;

    E_ModbusError error;

//    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "(%x) readInputs %d", this, getSlaveId());

    error = readHoldingRegs(getSlaveId(), M_INVERTER_COM_SCAN_INPUT_ADDRESS, 4, (uint8_t*) &inverterData, buffLength);
    if (error != E_ModbusError_Ok)
    {
        ++m_numOfFailedReads;
        if (m_numOfFailedReads > M_NUMBER_OF_RETRIES)
        {
            // TODO: Set the control at an error.
            m_outputCurrent->setValueValid(false);
            m_outputFrequency->setValueValid(false);
            m_outputFrequency->setValue(0);
            m_outputCurrent->setValue(0);
            setUpdateInterval(10000);
        }
        return;
    }

    // if we're here it means the inverter went back to communication.
    if (m_numOfFailedReads > 0)
    {
        // setup the inverter again.
        setupInverter();
        setUpdateInterval(M_INVERTER_SAMPLE_INTERVAL);
        m_outputCurrent->setValueValid(true);
        m_outputFrequency->setValueValid(true);
    }
    m_numOfFailedReads = 0;

    statusWord.word = SWAP_16(inverterData[0]);
    outFreq = SWAP_16(inverterData[1]);
    outCurrent = SWAP_16(inverterData[2]);
    logicInputs = SWAP_16(inverterData[3]);

    m_outputFrequency->setValue((float) (outFreq * m_frequencyMultiplier));
    m_outputCurrent->setValue((float) (outCurrent * m_currentMultiplier));
    m_driveStatus->setValue(statusWord.word);

    if (m_outputEnable != NULL)
    {
        if ((logicInputs & 1) != (m_outputEnable->getValueU32() != 0))
            ++m_numOfFailedEnables;
        else
        {
            if (m_numOfFailedEnables > M_NUMBER_OF_RETRIES)
                m_outputFrequency->updateErrorBits(E_PSSErrors_InverterForwardNotEnabled, false);
            m_numOfFailedEnables = 0;
        }

        if (m_numOfFailedEnables > M_NUMBER_OF_RETRIES)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "{[PSSID:%d]}: inverter enable not set at inverter.", getPssId());
            m_outputFrequency->updateErrorBits(E_PSSErrors_InverterForwardNotEnabled, true);
        }
    }

    if (m_setpointUpdated)
    {
        error = writeSingleReg(getSlaveId(), M_INVERTER_FREQUENCY_SETPOINT_ADDRESS,
                (int16_t)(m_frequencySetpoint->getValueF() * m_setpointMultiplier));
        if (error != E_ModbusError_Ok)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Modbus Device %d error: setpoint update failed.", getSlaveId());
            ++m_numOfFailedReads;
            return;
        }
        m_setpointUpdated = false;
    }

    if (statusWord.bits.faultDetected == 1)
    {
        uint16_t faultCode;
        error = readHoldingRegs(getSlaveId(), M_INVERTER_FAULT_ADDRESS, 1, (uint8_t*) &faultCode, buffLength);
        if (error != E_ModbusError_Ok)
        {
            ++m_numOfFailedReads;
            if (m_numOfFailedReads > M_NUMBER_OF_RETRIES)
            {
                // TODO: Set the control at an error.
                m_outputCurrent->setValueValid(false);
                m_outputFrequency->setValueValid(false);
                m_outputFrequency->setValue(0);
                m_outputCurrent->setValue(0);
                setUpdateInterval(10000);
            }
            return;
        }

        faultCode = SWAP_16(faultCode);

        if (faultCode != m_lastFaultCode)
        {
            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "Modbus Inverter {[PSSID:%d]} Fault Code %d: %s", getPssId(), faultCode,
                    g_Atv32FaultToString[faultCode].message);
            m_lastFaultCode = faultCode;
//            m_outputFrequency->setValueValid(false);
//            m_outputCurrent->setValueValid(false);
        }
        m_outputFrequency->updateErrorBits(E_PSSErrors_InverterFault, true);
        m_outputCurrent->updateErrorBits(E_PSSErrors_InverterFault, true);
    }
    else
    {
        m_lastFaultCode = -1;
        m_outputFrequency->updateErrorBits(E_PSSErrors_InverterFault, false);
        m_outputCurrent->updateErrorBits(E_PSSErrors_InverterFault, false);
    }
}

void ModbusInverterSchneiderAtv32::setInverterType()
{
    m_inverterType = E_ModbusInverterType_SchneiderAltivar32;
    m_frequencyMultiplier = 0.1;
    m_setpointMultiplier = 10;
    m_currentMultiplier = 0.1;
}

void ModbusInverterSchneiderAtv32::setupInverterMinSpeed(float value)
{
    m_minSpeed = (int16_t)(value * 10);
}

void ModbusInverterSchneiderAtv32::setupInverterMaxSpeed(float value)
{
    m_maxSpeed = (int16_t)(value * 10);
}

void ModbusInverterSchneiderAtv32::setupInverterAccelRate(float value)
{
    m_accelRate = (int16_t)(value * 10);
}

void ModbusInverterSchneiderAtv32::setupInverterDecelRate(float value)
{
    m_decelRate = (int16_t)(value * 10);
}

void ModbusInverterSchneiderAtv32::setupInverter()
{
    E_ModbusError result;
    uint16_t value;
    uint32_t bufLength;
    int i, j;

    if (m_minSpeed != (int16_t) 0x8000)
    {
        result = writeSingleReg(getSlaveId(), 3105, m_minSpeed);
        vTaskDelay(10);
    }

    if (result != E_ModbusError_Ok)
    {
        m_numOfFailedReads = M_NUMBER_OF_RETRIES + 1;
        return;
    }

    if (m_maxSpeed != (int16_t) 0x8000)
    {
        result = writeSingleReg(getSlaveId(), 3103, m_maxSpeed);
        vTaskDelay(10);
        // write the high speed
        result = writeSingleReg(getSlaveId(), 3104, m_maxSpeed);
        vTaskDelay(10);
    }

    if (result != E_ModbusError_Ok)
    {
        m_numOfFailedReads = M_NUMBER_OF_RETRIES + 1;
        return;
    }

    if (m_accelRate != (int16_t) 0x8000)
    {
        result = writeSingleReg(getSlaveId(), 9001, m_accelRate);
        vTaskDelay(10);
    }

    if (result != E_ModbusError_Ok)
    {
        m_numOfFailedReads = M_NUMBER_OF_RETRIES + 1;
        return;
    }

    if (m_decelRate != (int16_t) 0x8000)
    {
        result = writeSingleReg(getSlaveId(), 9002, m_decelRate);
        vTaskDelay(10);
    }

    if (result != E_ModbusError_Ok)
    {
        m_numOfFailedReads = M_NUMBER_OF_RETRIES + 1;
        return;
    }

    if (m_nominalCurrent != (int16_t) 0x8000)
    {
        result = writeSingleReg(getSlaveId(), 9603, m_nominalCurrent);
        vTaskDelay(10);
    }

    if (result != E_ModbusError_Ok)
    {
        m_numOfFailedReads = M_NUMBER_OF_RETRIES + 1;
        return;
    }

    if (m_nominalFrequency != (int16_t) 0x8000)
    {
        result = writeSingleReg(getSlaveId(), 9602, m_nominalFrequency);
        vTaskDelay(10);
    }

    if (result != E_ModbusError_Ok)
    {
        m_numOfFailedReads = M_NUMBER_OF_RETRIES + 1;
        return;
    }

    if (m_nominalRpm != (int16_t) 0x8000)
    {
        result = writeSingleReg(getSlaveId(), 9604, m_nominalRpm);
        vTaskDelay(10);
    }

    if (result != E_ModbusError_Ok)
    {
        m_numOfFailedReads = M_NUMBER_OF_RETRIES + 1;
        return;
    }

    for (i = 0; i < sizeof(Atv32RegisterSetup) && Atv32RegisterSetup[i].regAddress != 0; ++i)
    {
        result = readHoldingRegs(getSlaveId(), Atv32RegisterSetup[i].regAddress, 1, (uint8_t*) &value, bufLength);
        value = SWAP_16(value);
        if (value != Atv32RegisterSetup[i].value)
        {
            vTaskDelay(10);
            result = writeSingleReg(getSlaveId(), Atv32RegisterSetup[i].regAddress, Atv32RegisterSetup[i].value);
            vTaskDelay(10);
            result = readHoldingRegs(getSlaveId(), Atv32RegisterSetup[i].regAddress, 1, (uint8_t*) &value, bufLength);
            value = SWAP_16(value);
            if (value != Atv32RegisterSetup[i].value)
                printf("setup register %d failed: Requested: %d, Read: %d\n", Atv32RegisterSetup[i].regAddress,
                        Atv32RegisterSetup[i].regAddress, value);
        }
        if (result != E_ModbusError_Ok)
        {
            m_numOfFailedReads = M_NUMBER_OF_RETRIES + 1;
            return;
        }
    }

    writeGeneralSetupVector();

    storeConfiguration();
}

void ModbusInverterSchneiderAtv32::resetInverter()
{
    E_ModbusError result;
    uint16_t regValue;
    uint32_t bufLength;
    // reset the inverter
    writeSingleReg(getSlaveId(), M_INVERTER_CONTROL_WORD_ADDRESS, 1 << 7);
    vTaskDelay(30);

}

void ModbusInverterSchneiderAtv32::storeConfiguration()
{
    writeSingleReg(getSlaveId(), 8001, 1);
    vTaskDelay(10);
}

void ModbusInverterSchneiderAtv32::performInverterAutotune()
{
    E_ModbusError result;
    uint16_t value;
    uint32_t bufLength;

    result = writeSingleReg(getSlaveId(), M_INVERTER_AUTOTUNE, 1);
    do
    {
        result = readHoldingRegs(getSlaveId(), M_INVERTER_AUTOTUNE_STATE, 1, (uint8_t*) &value, bufLength);
        value = SWAP_16(value);
        vTaskDelay(10);
    } while (value != 1 && value != 2); // while value is not "pending" and not "in progress".
}

void ModbusInverterSchneiderAtv32::setupInverterMotorFrequency(float value)
{
    m_nominalFrequency = value * 10;
}

void ModbusInverterSchneiderAtv32::setupInverterMotorCurrent(float value)
{
    m_nominalCurrent = value * 10;
}

void ModbusInverterSchneiderAtv32::setupInverterMotorNominalRPM(float value)
{
    m_nominalRpm = value;
}

void ModbusInverterSchneiderAtv32::serialize(F_FILE* f)
{
    Serializer<ModbusInverterSchneiderAtv32> s;
    s.serialize(f, *this);
}

ModbusInverterSchneiderAtv32::ModbusInverterSchneiderAtv32(F_FILE* f) :
        ModbusInverterPeripheralBase(f)
{
    Serializer<ModbusInverterSchneiderAtv32> s;
    s.deserialize(f, *this);
}
