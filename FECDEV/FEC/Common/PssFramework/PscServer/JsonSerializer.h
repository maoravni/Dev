/*
 * JsonSerializer.h
 *
 *  Created on: 10 Sep 2014
 *      Author: maora
 */

#ifndef JSONSERIALIZER_H_
#define JSONSERIALIZER_H_

#include "JsonHandler.h"

class JsonSerializer
{
public:
    JsonSerializer();
    virtual ~JsonSerializer();

    virtual void describeClass() = 0;
};

#endif /* JSONSERIALIZER_H_ */
