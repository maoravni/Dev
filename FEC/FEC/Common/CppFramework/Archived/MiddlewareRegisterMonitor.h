/*
 * MiddlewareRegisterMonitor.h
 *
 *  Created on: 27 Dec 2012
 *      Author: maora
 */

#ifndef MIDDLEWAREREGISTERMONITOR_H_
#define MIDDLEWAREREGISTERMONITOR_H_

#include "ll_typedef.h"

class MiddlewareRegisterMonitor
{
private:
    //int m_numOfRegisters;

public:
    bool m_enabled;

    MiddlewareRegisterMonitor();
    virtual ~MiddlewareRegisterMonitor();

    void sendAllRegisters();

    void setRegister(unsigned short id, U32 value);

    /**
     * Wrapper around the middleware's get parameter function.
     * @param id
     * @param value
     * @return
     */
    U32 cfgGetParam(U32 instnace, U32 id, U32 index, U32 &value);

    /**
     * Wrapper around the middleware's put parameter function.
     * @param id
     * @param value
     * @return
     */
    U32 cfgPutParam(U32 instnace, U32 id, U32 index, U32 value);

    void sendValue(unsigned short registerId, U32 value);

    void reset();

};

#endif /* MIDDLEWAREREGISTERMONITOR_H_ */
