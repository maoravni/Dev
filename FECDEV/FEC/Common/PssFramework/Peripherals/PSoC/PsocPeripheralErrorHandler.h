/*
 * PsocPeripheralErrorHandler.h
 *
 *  Created on: 15 May 2015
 *      Author: maora
 */

#ifndef PSOCPERIPHERALERRORHANDLER_H_
#define PSOCPERIPHERALERRORHANDLER_H_

#include <PscServer/PscMessageStructs.h>
#include "PsocMessageStruct.h"

class PsocPeripheralErrorHandler
{
protected:
    uint16_t m_currentErrors;
    uint16_t m_currentWarnings;

public:
    PsocPeripheralErrorHandler();
    virtual ~PsocPeripheralErrorHandler();

    virtual void raiseError(E_PsocErrorBits error, bool state) = 0;
    virtual void raiseWarning(E_PSSWarnings warning, bool state) = 0;
};

#endif /* PSOCPERIPHERALERRORHANDLER_H_ */
