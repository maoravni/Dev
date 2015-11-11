/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#include "util/util_makeheader.h"
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef osapi_string_h
#include "osapi/osapi_string.h"
#endif
#ifndef db_api_h
#include "db/db_api.h"
#endif
#ifndef rt_log_h
#include "rt/rt_log.h"
#endif
#ifndef rt_rt_h
#include "rt/rt_rt.h"
#endif

struct RT_RegistryProperty RTCOMPONENTFACTORY_REGISTRY_PROPERTY_DEFAULT =
                                                RT_RegistryProperty_INITIALIZER;

#define RT_COMPONENT_FACTORY_NAME "rt"

struct RT_Registry
{
    RTI_BOOL _is_initialized;
    DB_Table_T factory_table;
    struct RT_RegistryProperty property;
};

RTI_PRIVATE struct RT_Registry RT_ComponentFactory_g_instance =
{
        RTI_FALSE,
        NULL,
        RT_RegistryProperty_INITIALIZER
};

/*** SOURCE_BEGIN ***/

RTI_PRIVATE RTI_INT32
RT_ComponentFactory_compare(DB_Select_T opcode,RTI_INT32 flags,
                           const DB_Record_T op1, void *op2, void *op3)
{
    struct RT_ComponentFactory *left_entry = (struct RT_ComponentFactory*)op1;
    union RT_ComponentFactoryId *id;

    if (DB_SELECT_OP2_IS_KEY(flags))
    {
        id = (union RT_ComponentFactoryId*)op2;
    }
    else
    {
        id = &((struct RT_ComponentFactory*)op2)->_id;
    }

    if (left_entry->_id._value._high > id->_value._high)
    {
        return 1;
    }
    if (left_entry->_id._value._high < id->_value._high)
    {
        return -1;
    }

    if (left_entry->_id._value._low > id->_value._low)
    {
        return 1;
    }
    if (left_entry->_id._value._low < id->_value._low)
    {
        return -1;
    }

    return 0;
}

void
RT_Registry_get_property(RT_Registry_T *registry,
                            struct RT_RegistryProperty *property)
{
    LOG_PRECONDITION_P2((registry == NULL) || (property == NULL),
                        return,
                        OSAPI_LOG_PTR_LOW(registry),
                        OSAPI_LOG_PTR_LOW(property))

    *property = registry->property;
}

RTI_BOOL
RT_Registry_set_property(RT_Registry_T *registry,
                            struct RT_RegistryProperty *property)
{
    LOG_PRECONDITION_P2((registry == NULL) || (property == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(registry),
                        OSAPI_LOG_PTR_LOW(property))

    if (registry->_is_initialized)
    {
        RT_Log_error(RT_LOG_REGISTRY_SET_IMMUTABLE_PROPERTY);
        return RTI_FALSE;
    }

    registry->property = *property;

    return RTI_TRUE;
}

RTI_BOOL
RT_Registry_initialize(RT_Registry_T *registry)
{
    struct DB_TableProperty tbl_property = DB_TableProperty_INITIALIZER;
    DB_ReturnCode_T dbrc;

    LOG_PRECONDITION_P1((registry == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(registry))

    registry->factory_table = NULL;

    tbl_property.max_cursors = 1;
    tbl_property.max_indices = 0;
    tbl_property.max_records = registry->property.max_factories;

    dbrc = DB_Database_create_table(&registry->factory_table,
                                   registry->property.db,
                                   RT_COMPONENT_FACTORY_NAME,
                                   sizeof(struct RT_ComponentFactory),
                                   NULL,
                                   RT_ComponentFactory_compare,
                                   &tbl_property);

    if (dbrc != DB_RETCODE_OK)
    {
        RT_Log_error_p1(RT_LOG_REGISTRY_INIT_FAILURE,dbrc);
        return RTI_FALSE;
    }

    registry->_is_initialized = RTI_TRUE;

    return RTI_TRUE;
}

RTI_BOOL
RT_Registry_finalize(RT_Registry_T *registry)
{
    DB_ReturnCode_T dbrc;

    LOG_PRECONDITION_P1((registry == NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(registry))

    dbrc = DB_Database_delete_table(
                            registry->property.db,registry->factory_table);

    registry->_is_initialized = RTI_FALSE;

    return (dbrc == DB_RETCODE_OK);
}

RT_Registry_T*
RT_Registry_get_instance(void)
{
    return &RT_ComponentFactory_g_instance;
}

RTI_BOOL
RT_Registry_register(RT_Registry_T *registry,
                     const char *name,
                     struct RT_ComponentFactoryI *intf,
                     struct RT_ComponentFactoryProperty *property,
                     struct RT_ComponentFactoryListener *listener)
{
    struct RT_ComponentFactory *reg_entry = NULL;
    union RT_ComponentFactoryId id;
    DB_ReturnCode_T  dbrc;

    LOG_PRECONDITION_P3((registry==NULL) || (name==NULL) || (intf==NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(registry),
                        OSAPI_LOG_PTR_LOW(name),
                        OSAPI_LOG_PTR_LOW(intf))

    if (!registry->_is_initialized)
    {
        if (!RT_Registry_initialize(registry))
        {
            return RTI_FALSE;
        }
    }

    RT_ComponentFactoryId_clear(&id);
    RT_ComponentFactoryId_set_name(&id,name);

    dbrc = DB_Table_select_match(registry->factory_table,
                DB_TABLE_DEFAULT_INDEX,(DB_Record_T*)&reg_entry,(DB_Key_T)&id);
    if (dbrc == DB_RETCODE_OK)
    {
        RT_LOG_WARNING_P1(RT_LOG_REGISTRY_ENTRY_EXISTS,
                          OSAPI_LOG_PTR_LOW(name));
        return RTI_FALSE;
    }

    dbrc = DB_Table_create_record(registry->factory_table,(DB_Record_T*)&reg_entry);
    if (dbrc != DB_RETCODE_OK)
    {
        RT_Log_error_p1(RT_LOG_REGISTRY_REGISTER_FAILURE,dbrc);
        return RTI_FALSE;
    }
    reg_entry->_factory = intf->initialize(property, listener);
    reg_entry->_id._value = id._value;
    reg_entry->intf = intf;
    reg_entry->_factory->_id._value = id._value;

    dbrc = DB_Table_insert_record(registry->factory_table,(DB_Record_T)reg_entry);
    if (dbrc != DB_RETCODE_OK)
    {
        RT_Log_error_p1(RT_LOG_REGISTRY_REGISTER_FAILURE,dbrc);
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

RTI_BOOL
RT_Registry_unregister(RT_Registry_T *registry,
                       const char *name,
                       struct RT_ComponentFactoryProperty **property,
                       struct RT_ComponentFactoryListener **listener)
{
    union RT_ComponentFactoryId id;
    struct RT_ComponentFactory *reg_entry;
    DB_ReturnCode_T  dbrc;

    LOG_PRECONDITION_P2((registry==NULL) || (name==NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(registry),
                        OSAPI_LOG_PTR_LOW(name))

    if (!registry->_is_initialized)
    {
        if (!RT_Registry_initialize(registry))
        {
            return RTI_FALSE;
        }
    }

    RT_ComponentFactoryId_clear(&id);
    RT_ComponentFactoryId_set_name(&id,name);

    dbrc = DB_Table_select_match(registry->factory_table,
             DB_TABLE_DEFAULT_INDEX,(DB_Record_T*)&reg_entry,(DB_Key_T)&id);
    if (dbrc != DB_RETCODE_OK)
    {
        return RTI_FALSE;
    }

    if (reg_entry->_factory->intf->finalize)
    {
        reg_entry->_factory->intf->finalize(reg_entry->_factory,property,listener);
    }

    dbrc = DB_Table_delete_record(registry->factory_table,(DB_Key_T)reg_entry);

    return (dbrc == DB_RETCODE_OK);
}

struct RT_ComponentFactory*
RT_Registry_lookup(RT_Registry_T *registry,const char *name)
{
    union RT_ComponentFactoryId id;
    struct RT_ComponentFactory *reg_entry;
    DB_ReturnCode_T  dbrc;

    LOG_PRECONDITION_P2((registry==NULL) || (name==NULL),
                        return RTI_FALSE,
                        OSAPI_LOG_PTR_LOW(registry),
                        OSAPI_LOG_PTR_LOW(name))

    if (!registry->_is_initialized)
    {
        if (!RT_Registry_initialize(registry))
        {
            return NULL;
        }
    }

    RT_ComponentFactoryId_clear(&id);
    RT_ComponentFactoryId_set_name(&id,name);

    dbrc = DB_Table_select_match(registry->factory_table,
                DB_TABLE_DEFAULT_INDEX,(DB_Record_T*)&reg_entry,(DB_Key_T)&id);
    if (dbrc != DB_RETCODE_OK)
    {
        return NULL;
    }

    return reg_entry->_factory;
}

RTI_INT32
RT_ComponentFactory_get_id(struct RT_ComponentFactory *factory)
{
    return factory->intf->id;
}


