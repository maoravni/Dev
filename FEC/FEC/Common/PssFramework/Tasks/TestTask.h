/*
 * TestTask.h
 *
 *  Created on: 11 Jun 2013
 *      Author: maora
 */

#ifndef TESTTASK_H_
#define TESTTASK_H_

#include <AManagedTask.h>

class TestTask: public AManagedTask
{
public:
    TestTask();
    virtual ~TestTask();

    /**
     * Task control loop.
     */
    virtual void run();

};

#endif /* TESTTASK_H_ */
