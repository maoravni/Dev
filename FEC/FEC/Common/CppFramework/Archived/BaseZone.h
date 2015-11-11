/*
 * BaseZone.h
 *
 *  Created on: 6 Dec 2012
 *      Author: maora
 */

#ifndef BASEZONE_H_
#define BASEZONE_H_

//#include "ll_typedef.h"
#include <MessageStructs.h>

#include "MwAccessor.h"

enum E_ZoneState
{
    E_ZoneState_On, E_ZoneState_Standby, E_ZoneState_Moving2Ready, E_ZoneState_Ready,
//E_ZoneState_Off,
};

struct T_ZoneStatusSplit
{
    char enabled :1;
    char hasErrors :1;
    char hasWarnings :1;
    E_ZoneState state :2;
};

union T_ZoneStatus
{
    char full;
    T_ZoneStatusSplit split;
};

/**
 * Base class for abstraction of the various zones connected to the board.
 */
class BaseZone
{
protected:
    //E_ZoneType m_zoneType;

    bool m_enabled;
    bool m_hasErrors;
    bool m_hasWarnings;
    bool m_monitorEnabled;
    E_ZoneState m_state;

    unsigned int m_currentErrorState;
    unsigned int m_currentWarningState;

protected:
    unsigned char m_controllerId;
    unsigned char m_zoneId;

public:
    //BaseZone();
    BaseZone();
    virtual ~BaseZone();

    U32 m_lastSn; //!< Last serial number of the message requiring a sequence end.

//    inline void assignZoneId(unsigned char id) {m_zoneId = id;}
//    inline void assignDeviceType(unsigned char type) {m_deviceTypeId = type;}
//    inline void assignControlledId(unsigned char id) {m_controllerId = id;}
    void assignIds(unsigned char controllerId, unsigned char zoneId);
    inline unsigned char getZoneId()
    {
        return m_zoneId;
    }
    inline unsigned char getControllerId()
    {
        return m_controllerId;
    }
    /**
     * get the zone type.
     * @return
     */
    virtual E_ZoneType getZoneType() = 0;

    /**
     * Set the zone enabled status.
     * @param enable
     * @return OK/0 if the enable succeeded.
     */
    virtual STATUS enableZone(bool enable);

    virtual bool configValues(short value);
    /**
     * check if the zone is enabled.
     * @return
     */
    virtual bool isEnabled()
    {
        return m_enabled;
    }

    /**
     * get the state of the zone
     * @return
     */
    virtual E_ZoneState getState()
    {
        return m_state;
    }

    char getStatusStruct();

    /**
     * Enable/Disable periodic monitoring for this zone.
     * @param enable
     */
    virtual void enableMonitoring(bool enable)
    {
        m_monitorEnabled = enable;
    }

    /**
     * @return true if monitoring is enabled for this zone.
     */
    virtual bool isMonitoringEnabled()
    {
        return (m_monitorEnabled);
    }

    /**
     * Read the value of the zone.
     * @param value Value to fill for the zone.
     * @return OK/0 on success, or error message.
     */
    //virtual U32 readZone(U32 &value) {return 0;}
    /**
     * Read zone values and send the values to the OPC. Called from the periodic monitor task.
     */
    virtual void executeZone()=0;

    /**
     * send the zone status to the SUM.
     */
    virtual void sendZoneStatus()=0;

    /**
     * Reset the zone to its default state.
     */
    virtual void reset();
    virtual void sendError(E_TmsErrors error);

    virtual void configSampleInterval(unsigned int interval) = 0;

protected:
    /**
     * Read zone values and send the values to the OPC. Called from the periodic monitor task.
     * @return error value. OK/0 on success.
     */
    virtual void sendMonitorReply(E_DataTypes dataType, U32 value);
    virtual void sendMonitorReply(U32 dataType, char* data, int length);
    virtual void sendSeqEnded(unsigned long messageId, unsigned long sn, char controllerId, char zoneId, int status);
    virtual void sendWarning(E_TmsWarnings warning);

    virtual void raiseError(E_TmsErrors error, bool errorState);
    virtual void raiseWarning(E_TmsWarnings warning, bool warningState);

};

#endif /* BASEZONE_H_ */
