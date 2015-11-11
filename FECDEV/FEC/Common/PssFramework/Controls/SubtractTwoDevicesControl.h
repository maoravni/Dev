/*
 * SubtractTwoDevicesControl.h
 *
 *  Created on: 6 Sep 2015
 *      Author: maora
 */

#ifndef SUBTRACTTWODEVICESCONTROL_H_
#define SUBTRACTTWODEVICESCONTROL_H_

#include "ControlBase.h"
#include "CalculateOnTwoDevicesControl.h"

class SubtractTwoDevicesControl: public CalculateOnTwoDevicesControl
{
public:
    SubtractTwoDevicesControl();
    virtual ~SubtractTwoDevicesControl();

    virtual void execute();
};

class AddTwoDevicesControl: public CalculateOnTwoDevicesControl
{
public:
    AddTwoDevicesControl();
    virtual ~AddTwoDevicesControl();

    virtual void execute();
};

class MultiplyTwoDevicesControl: public CalculateOnTwoDevicesControl
{
public:
    MultiplyTwoDevicesControl();
    virtual ~MultiplyTwoDevicesControl();

    virtual void execute();
};

class DivideTwoDevicesControl: public CalculateOnTwoDevicesControl
{
public:
    DivideTwoDevicesControl();
    virtual ~DivideTwoDevicesControl();

    virtual void execute();
};

class MaxTwoDevicesControl: public CalculateOnTwoDevicesControl
{
public:
    MaxTwoDevicesControl();
    virtual ~MaxTwoDevicesControl();

    virtual void execute();
};

class MinTwoDevicesControl: public CalculateOnTwoDevicesControl
{
public:
    MinTwoDevicesControl();
    virtual ~MinTwoDevicesControl();

    virtual void execute();
};

#endif /* SUBTRACTTWODEVICESCONTROL_H_ */
