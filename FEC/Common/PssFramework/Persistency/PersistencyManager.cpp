/*
 * PersistencyManager.cpp
 *
 *  Created on: 7 Apr 2016
 *      Author: maora
 */

#include <Persistency/PersistencyManager.h>
#include <Elements/ElementRepository.h>
#include <Peripherals/PeripheralRepository.h>
#include <Controls/ControlRepository.h>
//#include "Serializer.h"
#include "ElementRepositorySerializer.h"
#include "PeripheralRepositorySerializer.h"
#include "ControlRepositorySerializer.h"
#include <logger.h>
#include <filesystemDriver.h>
#include <PscServer/PscMessageHandler.h>

PersistencyManager* PersistencyManager::p_instance = NULL;

PersistencyManager::PersistencyManager()
{
    // TODO Auto-generated constructor stub

}

PersistencyManager::~PersistencyManager()
{
    // TODO Auto-generated destructor stub
}

void PersistencyManager::serializeConfiguration()
{
    PscMessageHandler::getInstance()->getPsocManager()->setIsInSerialization(true);

    int result = filesystemDriver_init();
    int e = 1;

    filesystemDriver_printFree();

    serializeElements();
    serializePeripherals();

    filesystemDriver_free();

    PscMessageHandler::getInstance()->getPsocManager()->setIsInSerialization(false);
}

void PersistencyManager::deserializeConfiguration()
{
    PscMessageHandler::getInstance()->getPsocManager()->setIsInSerialization(true);

    int result = filesystemDriver_init();
    int e = 1;

    filesystemDriver_printFree();

    PeripheralRepository::getInstance().destroyAllPeripherals();
    ElementRepository::getInstance().destroyAllElements();

    try
    {
        deserializeElements();
    } catch (char const *msg)
    {
        printf("deserializeElements exception:", msg, "\n");
    }
    try
    {
        deserializePeripherals();
    } catch (char const *msg)
    {
        printf("deserializePeripherals exception:", msg, "\n");
    }

    filesystemDriver_free();

    PscMessageHandler::getInstance()->getPsocManager()->setIsInSerialization(false);
}

void PersistencyManager::serializeElements()
{
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "starting elements serialization");
    F_FILE* f = f_open("elements", "w+");
    Serializer<ElementRepository> s;
    s.serialize(f, ElementRepository::getInstance());
    f_close(f);
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Elements serialization ended");
//    return result;
}

void PersistencyManager::deserializeElements()
{
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "starting elements deserialization");
    F_FILE* f = f_open("elements", "r");
    Serializer<ElementRepository> s;
    try
    {
        s.deserialize(f, ElementRepository::getInstance());
    } catch (char const *msg)
    {
        printf("deserializeElements exception:", msg, "\n");
    }
    f_close(f);
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Elements deserialization ended");
//    return result;
}

void PersistencyManager::serializePeripherals()
{
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "starting peripherals serialization");
    F_FILE* f = f_open("periphs", "w+");
    Serializer<PeripheralRepository> s;
    s.serialize(f, PeripheralRepository::getInstance());
    f_close(f);
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Peripherals serialization ended");
//    return result;
}

void PersistencyManager::deserializePeripherals()
{
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "starting peripherals deserialization");
    F_FILE* f = f_open("periphs", "r");
    Serializer<PeripheralRepository> s;
    try
    {
        s.deserialize(f, PeripheralRepository::getInstance());
    } catch (char const *msg)
    {
        printf("deserializePeripherals exception:", msg, "\n");
    }
    f_close(f);
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Peripherals deserialization ended");
//    return result;
}

void PersistencyManager::serializeControls()
{
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "starting controls serialization");
    F_FILE* f = f_open("controls", "w+");
    Serializer<ControlRepository> s;
    s.serialize(f, ControlRepository::getInstance());
    f_close(f);
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Controls serialization ended");
//    return result;
}

void PersistencyManager::deserializeControls()
{
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "starting controls deserialization");
    F_FILE* f = f_open("controls", "r");
    Serializer<ControlRepository> s;
    s.deserialize(f, ControlRepository::getInstance());
    f_close(f);
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Controls deserialization ended");
//    return result;
}
