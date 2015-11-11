/*
 * HumidityZone.h
 *
 *  Created on: 17 Dec 2012
 *      Author: maora
 */

#ifndef HUMIDITYZONE_H_
#define HUMIDITYZONE_H_

#include "BaseZone.h"
#include <BaseMiddlewareWrapperZone.h>
/*
 *
 */
class HumidityZone: public BaseMiddlewareWrapperZone
{
protected:
    //! Id's for calling the middleware's API.
    U32 m_mwReadValueId;    //!< ID for reading the temperature

public:
    HumidityZone();
    virtual ~HumidityZone();

    /**
     * Config the values for calling the middleware's API
     * @param instance
     * @param index
     * @param readValue
     */
    virtual void setMiddlewareIds(int instance, int index, U32 readValue);

    /**
     * Read the value of the zone.
     * @param value Value to fill for the zone.
     * @return OK/0 on success, or error message.
     */
    virtual U32 readZone(U32 &value);

};

#endif /* HUMIDITYZONE_H_ */
