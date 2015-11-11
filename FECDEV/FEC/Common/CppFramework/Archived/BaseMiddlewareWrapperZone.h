/*
 * BaseMiddlewareWrapperZone.h
 *
 *  Created on: 17 Dec 2012
 *      Author: maora
 */

#ifndef BASEMIDDLEWAREWRAPPERZONE_H_
#define BASEMIDDLEWAREWRAPPERZONE_H_

#include "BaseZone.h"

class MiddlewareAccessor
{
//protected:
//    //! Id's for calling the middleware's API.
//    char m_mwInstance;       //!< MW Instance ID
//    char m_mwIndex;          //!< MW Index of the zone

public:
    MiddlewareAccessor();

//    void setMiddlewareIds(int instance, int index);

    /**
     * Wrapper around the middleware's get parameter function.
     * @param id
     * @param value
     * @return
     */
    U32 cfgGetParam(U32 id, U32 &value);
    U32 cfgGetParam(U32 id);

    /**
     * Wrapper around the middleware's put parameter function.
     * @param id
     * @param value
     * @return
     */
    U32 cfgPutParam(U32 id, U32 value);

    U32 cfgGetFromRam(U32 id);
    U32 cfgGetFromSource(U32 id);
};

/*
 *
 */
class BaseMiddlewareWrapperZone: public BaseZone, public MiddlewareAccessor
{
public:
    BaseMiddlewareWrapperZone();
    virtual ~BaseMiddlewareWrapperZone();
};

#endif /* BASEMIDDLEWAREWRAPPERZONE_H_ */
