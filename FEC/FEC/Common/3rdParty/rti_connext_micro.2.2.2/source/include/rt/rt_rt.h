/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef rt_rt_h
#define rt_rt_h



#ifndef rt_dll_h
#include "rt/rt_dll.h"
#endif

#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifndef reda_circularlist_h
#include "reda/reda_circularlist.h"
#endif

#ifndef db_api_h
#include "db/db_api.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

/*******************************************************************************
 *                               RTConstants
 ******************************************************************************/
#define RT_MKINTERFACEID(class_,instance_) \
                                    ((((class_)<<16)&0xffff0000) | (instance_))

#define RT_INTERFACE_CLASS(id_)     ((id_) >> 16)
#define RT_INTERFACE_INSTANCE(id_)  ((id_) & 0xffff)

/* INTERFACE CLASSES */
#define RT_COMPONENT_CLASS_DISCOVERY            (0x0001)
#define RT_COMPONENT_CLASS_WHISTORY             (0x0002)
#define RT_COMPONENT_CLASS_RHISTORY             (0x0003)
#define RT_COMPONENT_CLASS_NETIO                (0x0004)

/* DDS Discovery class components */
#define RT_COMPONENT_INSTANCE_DISCOVERY_DPDE   (1)
#define RT_COMPONENT_INSTANCE_DISCOVERY_DPSE   (2)
#define RT_COMPONENT_INSTANCE_DISCOVERY_GROUP  (3)

/* DDS WriterHistory class components */
#define RT_COMPONENT_INSTANCE_WHISTORY_SM      (1)

/* DDS ReaderHistory class components */
#define RT_COMPONENT_INSTANCE_RHISTORY_SM      (1)

/* NETIO class components */
#define RT_COMPONENT_INSTANCE_UDP              (1)
#define RT_COMPONENT_INSTANCE_RTPS             (2)
#define RT_COMPONENT_INSTANCE_INTRA            (3)

typedef RTI_UINT32 RT_ComponentInterfaceId_t;

/*******************************************************************************
 *                               RT_Component
 ******************************************************************************/
struct RT_Component;
typedef struct RT_Component RT_Component_T;

struct RT_ComponentListener
{
    void *data;
};

#define RT_ComponentListener_INITIALIZER \
{ \
    NULL \
}

struct RT_ComponentProperty
{
    DB_Database_T db;
};

#define RT_ComponentProperty_INITIALIZER \
{\
   NULL\
}

typedef void
(*RT_ComponentRunFunc)(RT_Component_T * self);

struct RT_ComponentI
{
    RT_ComponentRunFunc _run;
};

struct RT_Component
{
    REDA_CircularListNode_T _node;
    struct RT_ComponentI *_intf;
    RTI_INT32 _flags;
    RTI_UINT32 id;
    struct RT_ComponentListener _listener;
    struct RT_ComponentProperty _property;
};

RTDllExport void
RT_Component_initialize(RT_Component_T *c,
                        struct RT_ComponentI *intf,
                        RTI_UINT32 id,
                        const struct RT_ComponentProperty *const property,
                        const struct RT_ComponentListener *const listener);

RTDllExport void
RT_Component_finalize(RT_Component_T *c);

RTDllExport void
RT_Component_run(RT_Component_T *self);

#define DEFAULT_COMPONENT_RUN RT_Component_run

/*******************************************************************************
 *                               RT_ComponentFactoryI
 ******************************************************************************/
struct RT_ComponentFactory;

struct RT_ComponentFactoryProperty
{
    DB_Database_T db;
};

#define RT_ComponentFactoryProperty_INITIALIZER \
{ \
  NULL\
}

struct RT_ComponentFactoryListener
{
    RTI_INT32 dummy;
};

typedef struct RT_ComponentFactory*
(*RT_ComponentFactory_initializeFactoryFunc)(
                                struct RT_ComponentFactoryProperty *property,
                                struct RT_ComponentFactoryListener *listener);

typedef void
(*RT_ComponentFactory_finalizeFactoryFunc)(struct RT_ComponentFactory *factory,
                                struct RT_ComponentFactoryProperty **property,
                                struct RT_ComponentFactoryListener **listener);

typedef RT_Component_T*
(*RT_ComponentFactory_createFunc)(struct RT_ComponentFactory *factory,
                                  struct RT_ComponentProperty *property,
                                  struct RT_ComponentListener *listener);

typedef void
(*RT_ComponentFactory_deleteFunc)(struct RT_ComponentFactory *factory,
                                 RT_Component_T* component);

typedef struct RT_ComponentI*
(*RT_ComponentFactory_get_ifFunc)(struct RT_ComponentFactory *factory);

#define RT_COMPONENT_FACTORY_ID_DEFAULT (0)

struct RT_ComponentFactoryI
{
    RTI_INT32 id;
    RT_ComponentFactory_initializeFactoryFunc initialize;
    RT_ComponentFactory_finalizeFactoryFunc finalize;
    RT_ComponentFactory_createFunc create_component;
    RT_ComponentFactory_deleteFunc delete_component;
    RT_ComponentFactory_get_ifFunc get_if;
};

#define RT_ComponentFactory_create_component(f_) ((f_)->intf)->create_component
#define RT_MAX_FACTORY_NAME 7

union RT_ComponentFactoryId
{
    struct
    {
        char _name[RT_MAX_FACTORY_NAME+1];
    } _name;
    struct
    {
        RTI_UINT32 _high;
        RTI_UINT32 _low;
    } _value;
};

typedef union RT_ComponentFactoryId  RT_ComponentFactoryId_T;

#define RT_ComponentFactoryId_clear(id_) \
    { (id_)->_value._high = 0; (id_)->_value._low = 0; }

#define RT_ComponentFactoryId_set_name(id_,name_) \
        OSAPI_Memory_copy((id_)->_name._name,name_,OSAPI_String_length(name_)+1);

struct RT_ComponentFactory
{
    struct RT_ComponentFactoryI *intf;
    struct RT_ComponentFactory *_factory;
    union RT_ComponentFactoryId _id;
};

typedef struct RT_ComponentFactory RT_ComponentFactory_T;

struct RT_RegistryProperty
{
    RTI_SIZE_T max_factories;
    DB_Database_T db;
};

#define RT_RegistryProperty_INITIALIZER \
{ \
    1,\
    NULL\
}

struct RT_Registry;
typedef struct RT_Registry RT_Registry_T;

RTDllExport RT_Registry_T*
RT_Registry_get_instance();

RTDllExport RTI_BOOL
RT_Registry_finalize(RT_Registry_T*);

RTDllExport RTI_INT32
RT_Registry_register(RT_Registry_T *registry,
                            const char *name,
                            struct RT_ComponentFactoryI *intf,
                            struct RT_ComponentFactoryProperty *property,
                            struct RT_ComponentFactoryListener *listener);

RTDllExport RTI_BOOL
RT_Registry_unregister(RT_Registry_T *registry,
                            const char *name,
                            struct RT_ComponentFactoryProperty **property,
                            struct RT_ComponentFactoryListener **listener);

RTDllExport struct RT_ComponentFactory*
RT_Registry_lookup(RT_Registry_T *registry,
                                  const char *name);

RTDllExport void
RT_Registry_get_property(RT_Registry_T *registry,
                        struct RT_RegistryProperty *property);

RTDllExport RTI_BOOL
RT_Registry_set_property(RT_Registry_T *registry,
                        struct RT_RegistryProperty *property);

RTDllExport RTI_INT32
RT_ComponentFactory_get_id(struct RT_ComponentFactory *factory);

extern RTDllVariable struct RT_RegistryProperty
                                 RTCOMPONENTFACTORY_REGISTRY_PROPERTY_DEFAULT;

#ifdef __cplusplus
}                               /* extern "C" */
#endif


#endif /* rt_rt_h */
