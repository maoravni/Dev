/*
 * HeaterZone.h
 *
 *  Created on: 13 Dec 2012
 *      Author: maora
 */

#ifndef HEATERZONE_H_
#define HEATERZONE_H_

#include "BaseZone.h"
#include "HeaterBase.h"
#include "TemperatureZone.h"

struct HeaterZoneSetPoint
{
    short lowWarningRange; //!< Below this value an error should be issued.
    short lowRange; //!< Below this value a warning should be issued.
    short highRange; //!< Above this range a warning shuld be issued.
    short highWarningRange; //!< Above this value an error should be issued.
    short temperature; //!< Temperature of the set point.
};

#define M_HEATER_ZONE_NUMBER_OF_SET_POINTS 3

struct T_HeaterMonitorReplyStruct
{
    unsigned short temperature;
    unsigned short power;
    unsigned short state;
};

/**
 * Class for abstraction of a heater + temperature readout
 */
class HeaterZone: public HeaterBase
{
protected:
    U32 m_mwEnableId; //!< ID for the enable command.
    U32 m_mwSetPoint; //!< ID for setting the set-point
    U32 m_mwMv; //!< The duty cycle of the heater PWM. In %.b
    U32 m_mwPidP; //!< PID P
    U32 m_mwPidI; //!< PID I
    U32 m_mwPidD; //!< PID D
    U32 m_mwPowerLimit; //!< Power limit
    U32 m_mwAutoTune; //!< AutoTune
    U32 m_mwTempId;    //!< ID for reading the temperature

    TemperatureZoneSetPoint m_setPoints[M_TEMPERATURE_ZONE_NUMBER_OF_SET_POINTS];
    TemperatureZoneSetPoint m_currentSetPoint;

    U32 m_lastSn; //!< Last serial number of the message requiring a sequence end.
    bool m_spWasSet;

    U32 m_lastTemperatureRead;

public:
    HeaterZone();
    virtual ~HeaterZone();

    /**
     * Enable/Disable heating of the zone.
     * @param enable
     */
    virtual STATUS enableZone(bool enable);

    /**
     * configure the values of a set point.
     *
     *     lowWarn        highRange
     * EEEE[WWWW[----|----]WWWW]EEEE
     *          lowRange       highWarn
     * W: issue warning.
     * E: issue error.
     *
     * @param setPoint index of the set point to set.
     * @param temperature temperature
     * @param lowWarn low limit of warning range.
     * @param lowRange low limit working range.
     * @param highRange high limit of working range.
     * @param highWarn high limit of warning range.
     * @return true on success
     */
    virtual bool configSetPoint(short setPoint, unsigned short temperature, short lowWarn, short lowRange, short highRange, short highWarn);

    /**
     * Config the temperature zone to a specific set of
     * @param temperature
     * @param lowWarn
     * @param lowRange
     * @param highRange
     * @param highWarn
     * @param sn
     * @return true on success
     */
    virtual bool configValues(unsigned short temperature, short lowWarn, short lowRange, short highRange, short highWarn, U32 sn);

    virtual STATUS setPowerLimit(unsigned short powerLimit);

    virtual STATUS setMaxTemperature(unsigned short maxTemperature);

    /**
     * Config the ID's for calling the middleware's API
     * @param instance
     * @param index
     * @param enable
     * @param readValue
     * @param setPoint
     * @param mv
     * @param pidP
     * @param pidI
     * @param pidD
     * @param powerLimit
     * @param autoTune
     */
    virtual void setMiddlewareIds(int instance, int index, U32 enable, U32 readValue, U32 setPoint, U32 mv,U32 PumaMv, U32 pidP, U32 pidI, U32 pidD, U32 powerLimit,
            U32 autoTune);

    /**
     * Activate a preset set point.
     * @param setPoint index of the set point to activate.
     * @return true on success
     */
    virtual bool enableSetPoint(short setPoint);

    /**
     * read all zone values and send the reply to the OPC. Called from the periodic monitor task.
     * @return error value. OK/0 on success.
     */
    virtual void executeZone();
    virtual void sendZoneStatus();
    virtual void sendPidValues();
    virtual void configPidValues(float kp, float ki, float kd, short setpointSmoothing, float iThreshold, unsigned short outputSmoothing);
    /**
     * get the current value of the temperature zone
     */
    virtual U32 executeTemperatureReadout(U32 mwid, U32 &value);

    /**
     * get the zone type.
     * @return
     */
    virtual E_ZoneType getZoneType() {return E_ZoneType_Heater;}

    /**
     * Start the auto-tune process of the PID.
     */
    virtual E_AckStatus startAutoTune(U16 setpoint, U16 overshoot, U16 powerStart, U16 powerStep, U16 lookback, bool reverseDirection, U32 sn);

    virtual void configSampleInterval(unsigned int interval) {};

    void setMwAutoTune(U32 mwAutoTune)
    {
        m_mwAutoTune = mwAutoTune;
    }

    void setMwEnableId(U32 mwEnableId)
    {
        m_mwEnableId = mwEnableId;
    }

    void setMwMv(U32 mwMv)
    {
        m_mwMv = mwMv;
    }

    void setMwPidD(U32 mwPidD)
    {
        m_mwPidD = mwPidD;
    }

    void setMwPidI(U32 mwPidI)
    {
        m_mwPidI = mwPidI;
    }

    void setMwPidP(U32 mwPidP)
    {
        m_mwPidP = mwPidP;
    }

    void setMwPowerLimit(U32 mwPowerLimit)
    {
        m_mwPowerLimit = mwPowerLimit;
    }

    void setMwSetPoint(U32 mwSetPoint)
    {
        m_mwSetPoint = mwSetPoint;
    }

    void setMwTempId(U32 mwTempId)
    {
        m_mwTempId = mwTempId;
    }
};

#endif /* HEATERZONE_H_ */
