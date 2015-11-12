/*
 * ValidationElementBase.h
 *
 *  Created on: 24 αιεμ 2013
 *      Author: maora
 */

#ifndef VALIDATIONELEMENTBASE_H_
#define VALIDATIONELEMENTBASE_H_

#include "ElementBase.h"

class ValidationElementBase: public ElementBase
{
public:
    ValidationElementBase();
    virtual ~ValidationElementBase();

    //    virtual bool isInAllowedRange() = 0;
    //
    //    virtual bool isInWorkingRange() = 0;
    //    virtual bool isAboveWorkingRange() = 0;
    //    virtual bool isBelowWorkingRange() = 0;
    //
    //    virtual bool isInWarningRange() = 0;
    //    virtual bool isAboveWarningRange() = 0;
    //    virtual bool isBelowWarningRange() = 0;
};

#endif /* VALIDATIONELEMENTBASE_H_ */
