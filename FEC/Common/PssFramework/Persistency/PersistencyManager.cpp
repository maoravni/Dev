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
#include <task.h>

PersistencyManager* PersistencyManager::p_instance = NULL;

PersistencyManager::PersistencyManager()
{
#ifdef NO_PERSISTENCY
    return;
#endif
    m_fecBoardConfiguration.getParametersFromBoard();
}

PersistencyManager::~PersistencyManager()
{
    // TODO Auto-generated destructor stub
}

void PersistencyManager::serializeConfiguration()
{
#ifdef NO_PERSISTENCY
    return;
#endif
    startSerialization();
    
    serializeBoard();
    serializeElements();
    serializePeripherals();
    serializeControls();

    endSerialization();
}

void PersistencyManager::deserializeConfiguration()
{
#ifdef NO_PERSISTENCY
    return;
#endif
    startSerialization();

    deleteAllEntities();

    try
    {
        deserializeBoard();
        m_fecBoardConfiguration.setParametersToBoard();
    } catch (char const *msg)
    {
        printf("deserializeBoards exception:", msg, "\n");
        deleteAllEntities();
    }
    try
    {
        deserializeElements();
    } catch (char const *msg)
    {
        printf("deserializeElements exception:", msg, "\n");
        deleteAllEntities();
    }
    try
    {
        deserializePeripherals();
    } catch (char const *msg)
    {
        printf("deserializePeripherals exception:", msg, "\n");
        deleteAllEntities();
    }
    try
    {
        deserializeControls();
    } catch (char const *msg)
    {
        printf("deserializeControls exception:", msg, "\n");
        deleteAllEntities();
    }

    endSerialization();
}

void PersistencyManager::serializeEntityBoard()
{
#ifdef NO_PERSISTENCY
    return;
#endif
    startSerialization();

    serializeBoard();

    endSerialization();
}

void PersistencyManager::serializeBoard()
{
#ifdef NO_PERSISTENCY
    return;
#endif
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "starting board serialization");
    F_FILE* f = f_open("board", "w+");
    Serializer<FecBoardConfiguration> s;
    s.serialize(f, m_fecBoardConfiguration);
    f_close(f);
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Board serialization ended");
//    return result;
}

void PersistencyManager::deserializeBoard()
{
#ifdef NO_PERSISTENCY
    return;
#endif
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "starting board deserialization");
    F_FILE* f = f_open("board", "r");

//    FecBoardConfiguration newBoardConfiguration;
//
    Serializer<FecBoardConfiguration> s;
    try
    {
        s.deserialize(f, m_fecBoardConfiguration);
    } catch (char const *msg)
    {
        printf("deserializeElements exception:", msg, "\n");
    }
    f_close(f);

//    if (m_fecBoardConfiguration.compareToCurrentParameters(newBoardConfiguration))
//        memcpy(&m_fecBoardConfiguration, &newBoardConfiguration, sizeof(FecBoardConfiguration));

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Board deserialization ended");
//    return result;
}

void PersistencyManager::serializeElements()
{
#ifdef NO_PERSISTENCY
    return;
#endif
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
#ifdef NO_PERSISTENCY
    return;
#endif
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
#ifdef NO_PERSISTENCY
    return;
#endif
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
#ifdef NO_PERSISTENCY
    return;
#endif
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
#ifdef NO_PERSISTENCY
    return;
#endif
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
#ifdef NO_PERSISTENCY
    return;
#endif
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "starting controls deserialization");
    F_FILE* f = f_open("controls", "r");
    Serializer<ControlRepository> s;
    s.deserialize(f, ControlRepository::getInstance());
    f_close(f);
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Controls deserialization ended");
//    return result;
}

void PersistencyManager::deleteAllEntities()
{
#ifdef NO_PERSISTENCY
    return;
#endif
    PscMessageHandler::getInstance()->reset();
//    PeripheralRepository::getInstance().destroyAllPeripherals();
//    ElementRepository::getInstance().destroyAllElements();
//    ControlRepository::getInstance().destroyAllControls();

}

void PersistencyManager::startSerialization()
{
#ifdef NO_PERSISTENCY
    return;
#endif
    PscMessageHandler::getInstance()->getPsocManager()->setIsInSerialization(true);

    Mi3I2CIrPeripheral* mi3Periph = PeripheralRepository::getInstance().getMi3I2cIrPeripheral();
    if (mi3Periph != NULL)
        mi3Periph->setIsInSerialization(true);

    I2C1_init(M_I2C_EEPROM_BAUD_RATE);

    int result = filesystemDriver_init();
    filesystemDriver_printFree();

}

void PersistencyManager::serializeEntity(PeripheralBase* periph)
{
#ifdef NO_PERSISTENCY
    return;
#endif
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "starting peripherals serialization");

    startSerialization();

    F_FILE* f = f_open("periphs", "r+");
    Serializer<PeripheralRepository> s;
    s.serializePeripheral(f, periph);
    f_close(f);

    endSerialization();

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Peripherals serialization ended");
//    return result;
}

void PersistencyManager::serializeEntity(ElementBase* element)
{
#ifdef NO_PERSISTENCY
    return;
#endif
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "starting element serialization");

    startSerialization();

    F_FILE* f = f_open("periphs", "r+");
    Serializer<ElementRepository> s;
    s.serializeElement(f, element);
    f_close(f);

    endSerialization();

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Peripherals serialization ended");
//    return result;
}

void PersistencyManager::serializeEntity(ControlBase* control)
{
#ifdef NO_PERSISTENCY
    return;
#endif
    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "starting control serialization");

    startSerialization();

    F_FILE* f = f_open("periphs", "r+");
    Serializer<ControlRepository> s;
    s.serializeControl(f, control);
    f_close(f);

    endSerialization();

    M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Peripherals serialization ended");
//    return result;
}

void PersistencyManager::endSerialization()
{
#ifdef NO_PERSISTENCY
    return;
#endif
    filesystemDriver_free();

    PscMessageHandler::getInstance()->getPsocManager()->setIsInSerialization(false);

    Mi3I2CIrPeripheral* mi3Periph = PeripheralRepository::getInstance().getMi3I2cIrPeripheral();

    I2C1_init(M_I2C_IR_SENSOR_BAUD_RATE);

    if (mi3Periph != NULL)
        mi3Periph->setIsInSerialization(false);
}

