/*
 * PersistencyManager.h
 *
 *  Created on: 7 Apr 2016
 *      Author: maora
 */

#ifndef PERSISTENCYMANAGER_H_
#define PERSISTENCYMANAGER_H_

#include <stdlib.h>

class PersistencyManager
{
private:
    static PersistencyManager* p_instance;

    PersistencyManager();
public:
    virtual ~PersistencyManager();

    static PersistencyManager* getInstance()
    {
        if (p_instance == NULL)
        {
            p_instance = new PersistencyManager();
        }

        return p_instance;
    }

    void serializeElements();
    void deserializeElements();

    void serializePeripherals();
    void deserializePeripherals();
};

#endif /* PERSISTENCYMANAGER_H_ */
