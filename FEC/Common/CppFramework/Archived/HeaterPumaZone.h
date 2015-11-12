/*
 * HeaterPumaZone.h
 *
 *  Created on: Feb 18, 2013
 *      Author: yossis
 */

#ifndef HEATERPUMAZONE_H_
#define HEATERPUMAZONE_H_

#include <HeaterZone.h>

class HeaterPumaZone: public HeaterZone
{
protected:
    U32 m_mwMvFromPuma;

public:
    HeaterPumaZone();
    virtual ~HeaterPumaZone();

    /**
     * read all zone values and send the reply to the OPC. Called from the periodic monitor task.
     * @return error value. OK/0 on success.
     */
    virtual void executeZone();

    void setMwMvFromPuma(U32 mwMvFromPuma)
    {
        m_mwMvFromPuma = mwMvFromPuma;
    }
};

#endif /* HEATERPUMAZONE_H_ */
