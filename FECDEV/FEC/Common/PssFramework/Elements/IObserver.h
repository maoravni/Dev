/*
 * IObserver.h
 *
 *  Created on: 10 Jun 2013
 *      Author: maora
 */

#ifndef IOBSERVER_H_
#define IOBSERVER_H_

//#include <Elements/ElementBase.h>
#include <DynamicArray.h>
#include <list>
#include <vector>
#include <stdint.h>

class ElementBase;

class IObserver
{
public:
    IObserver();
    virtual ~IObserver();

    virtual void updateNotification(ElementBase* element) = 0;

    /**
     * This method asks the observer if a value change is permitted.
     * It's main use is when an output is required to change it's value
     * and this means that some other controls should be disabled,
     * or the output change is not permitted.
     * @param element
     */
    virtual bool requestValueChange(ElementBase* element) = 0;
    virtual void timeoutExpired(uint16_t timeoutType) = 0;
};

//typedef DynamicArray<IObserver*> T_ObserverVector;
typedef std::vector<IObserver*> T_ObserverList;
typedef std::vector<IObserver*>::iterator T_ObserverListIterator;

#endif /* IOBSERVER_H_ */
