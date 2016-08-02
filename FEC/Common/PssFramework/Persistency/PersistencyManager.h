/*
 * PersistencyManager.h
 *
 *  Created on: 7 Apr 2016
 *      Author: maora
 */

#ifndef PERSISTENCYMANAGER_H_
#define PERSISTENCYMANAGER_H_

#include <stdlib.h>
#include "BoardSerializer.h"

class PeripheralBase;
class ElementBase;
class ControlBase;

class PersistencyManager
{
private:
    static PersistencyManager* p_instance;

    FecBoardConfiguration m_fecBoardConfiguration;

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

    void serializeConfiguration();
    void deserializeConfiguration();

    void serializeEntity(PeripheralBase* periph);
    void serializeEntity(ElementBase* element);
    void serializeEntity(ControlBase* control);
    void serializeEntityBoard();

private:
    void serializeElements();
    void deserializeElements();

    void serializePeripherals();
    void deserializePeripherals();

    void serializeControls();
    void deserializeControls();

    void serializeBoard();
    void deserializeBoard();

    void deleteAllEntities();

    void startSerialization();
    void endSerialization();

    friend class TestTask;
};

#endif /* PERSISTENCYMANAGER_H_ */
