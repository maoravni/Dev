/*
 * PersistencyManager.cpp
 *
 *  Created on: 7 Apr 2016
 *      Author: maora
 */

#include <Persistency/PersistencyManager.h>
#include <Elements/ElementRepository.h>
#include <Controls/ControlRepository.h>
//#include "Serializer.h"
#include "RepositorySerializers.h"
#include <logger.h>

PersistencyManager* PersistencyManager::p_instance = NULL;

PersistencyManager::PersistencyManager()
{
    // TODO Auto-generated constructor stub

}

PersistencyManager::~PersistencyManager()
{
    // TODO Auto-generated destructor stub
}

int PersistencyManager::serializeElements()
{
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "starting elements serialization");
    F_FILE* f = f_open("elements", "w+");
    Serializer<ElementRepository> s;
    if (s.serialize(f, ElementRepository::getInstance()) == 0)
    {
        f_close(f);
        return 0;
    }
    f_close(f);
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Elements serialization ended");
    return 1;
}

int PersistencyManager::deserializeElements()
{
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "starting elements deserialization");
    F_FILE* f = f_open("elements", "r");
    Serializer<ElementRepository> s;
    s.deserialize(f, ElementRepository::getInstance());
    f_close(f);
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Elements deserialization ended");
}
