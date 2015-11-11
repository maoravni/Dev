/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef netio_route_h
#define netio_route_h



/*e \file
  \brief NETIO Route API
*/

/*e \defgroup NETIORouteClass NETIO Route
    \ingroup NETIOModule

    \brief NETIO Route API
 */
#ifndef netio_dll_h
#include "netio/netio_dll.h"
#endif

#ifndef osapi_config_h
#include "osapi/osapi_config.h"
#endif

#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif

#ifndef reda_string_h
#include "reda/reda_string.h"
#endif

#ifndef netio_address_h
#include "netio/netio_address.h"
#endif

#ifndef netio_interface_h
#include "netio/netio_interface.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*e \ingroup NETIORouteClass
 */
typedef enum
{
    NETIO_ROUTESTATE_ACTIVE,
    NETIO_ROUTESTATE_INACTIVE
} NETIO_RouteState_T;

/*e \ingroup NETIORouteClass
 */
typedef enum
{
    NETIO_ROUTEKIND_UNKNOWN,
    NETIO_ROUTEKIND_META,
    NETIO_ROUTEKIND_USER
} NETIO_RouteKind_T;

struct NETIORouteEntryKey
{
    struct NETIO_Address destination;
    NETIO_Interface_T *intf;
    struct NETIO_Address intf_address;
};

/*e \ingroup NETIORouteClass
 *
 * Route entry
 */
struct NETIORouteEntry
{
    struct NETIO_Address destination;
    NETIO_Interface_T *intf;
    struct NETIO_Address intf_address;
    NETIO_RouteState_T state;
};

/*e \ingroup NETIORouteClass
 *
 * Route entry
 */
struct NETIOBindEntryKey
{
    struct NETIO_Address source;
    struct NETIO_Address destination;
};

struct NETIOBindEntry
{
    struct NETIO_Address source;
    struct NETIO_Address destination;
    NETIO_Interface_T *intf;
};

typedef RTI_BOOL
(*NETIO_PortCalculateFunc_T)(void *param,RTI_UINT32 port_base,RTI_INT32 index, struct NETIO_Address *address);

struct NETIO_BindResolver;
typedef struct NETIO_BindResolver NETIO_BindResolver_T;

struct NETIO_BindResolverProperty
{
    RTI_SIZE_T max_routes;
};

#define NETIO_BindResolverProperty_INITIALIZER \
{\
    1\
}

struct NETIO_RouteResolverProperty
{
    RTI_SIZE_T max_routes;
};

#define NETIO_RouteResolverProperty_INITIALIZER \
{\
    16\
}

struct NETIO_RouteResolver;
typedef struct NETIO_RouteResolver NETIO_RouteResolver_T;

NETIODllExport NETIO_RouteResolver_T*
NETIO_RouteResolver_new(DB_Database_T db,
                     const char* const name,
                     struct NETIO_RouteResolverProperty *const property);

NETIODllExport RTI_BOOL
NETIO_RouteResolver_delete(NETIO_RouteResolver_T *r_table);

NETIODllExport RTI_BOOL
NETIO_RouteResolver_add_interface(NETIO_RouteResolver_T *r_table,
                               NETIO_Interface_T *intf,
                               struct NETIO_Address *address,
                               struct NETIO_Netmask *netmask,
                               RTI_BOOL *exists);

NETIODllExport RTI_BOOL
NETIO_RouteResolver_delete_interface(NETIO_RouteResolver_T *r_table,
                                  NETIO_Interface_T *intf,
                                  struct NETIO_Address *address,
                                  struct NETIO_Netmask *netmask,
                                  RTI_BOOL *exists);


NETIODllExport RTI_BOOL
NETIO_RouteResolver_add_route(NETIO_RouteResolver_T *r_table,
                           NETIO_Interface_T *src_intf,
                           struct NETIO_Address *dst_reader,
                           struct NETIO_Address *via_address,
                           struct NETIORouteProperty *property,
                           RTI_BOOL *route_existed);

NETIODllExport RTI_BOOL
NETIO_RouteResolver_delete_route(NETIO_RouteResolver_T *r_table,
                              NETIO_Interface_T *src_intf,
                              struct NETIO_Address *dst_reader,
                              struct NETIO_Address *via_address,
                              RTI_BOOL *route_existed);


NETIODllExport NETIO_BindResolver_T*
NETIO_BindResolver_new(DB_Database_T db,
                     const char* const name,
                     struct NETIO_BindResolverProperty *const property);

NETIODllExport RTI_BOOL
NETIO_BindResolver_delete(NETIO_BindResolver_T* r_table);

NETIODllExport RTI_BOOL
NETIO_BindResolver_add_route(NETIO_BindResolver_T *r_table,
                           NETIO_RouteKind_T kind,
                           RT_ComponentFactoryId_T *id,
                           struct NETIO_Address *address,
                           NETIO_Interface_T *intf,
                           NETIO_PortCalculateFunc_T port_resolve,
                           void *port_resolve_param,
                           RTI_BOOL *exists);

NETIODllExport RTI_BOOL
NETIO_BindResolver_delete_route(NETIO_BindResolver_T *r_table,
                              NETIO_RouteKind_T kind,
                              RT_ComponentFactoryId_T *id,
                              struct NETIO_Address *address,
                              RTI_BOOL *existed);

NETIODllExport RTI_BOOL
NETIO_BindResolver_lookup_by_name(NETIO_BindResolver_T *r_table,
                                NETIO_RouteKind_T kind,
                                RTI_INT32 try_index,
                                const char *const name,
                                RT_ComponentFactoryId_T *id,
                                NETIO_Interface_T **intf,
                                struct NETIO_AddressSeq *resolved);

NETIODllExport RTI_BOOL
NETIO_BindResolver_lookup_by_address(NETIO_BindResolver_T *r_table,
                                      NETIO_RouteKind_T kind,
                                      RT_ComponentFactoryId_T *id,
                                      struct NETIO_Address *address,
                                      NETIO_Interface_T **intf);

NETIODllExport RTI_BOOL
NETIO_BindResolver_lookup_by_kind(NETIO_BindResolver_T *r_table,
                                   NETIO_RouteKind_T kind,
                                   RT_ComponentFactoryId_T *id,
                                   struct NETIO_Address *address,
                                   NETIO_Interface_T **intf);

NETIODllExport RTI_BOOL
NETIO_BindResolver_release_addresses(NETIO_BindResolver_T* r_table,
                                     struct REDA_StringSeq *transport,
                                     NETIO_RouteKind_T kind,
                                     struct NETIO_AddressSeq *locator_seq);

NETIODllExport RTI_BOOL
NETIO_BindResolver_reserve_addresses(NETIO_BindResolver_T* r_table,
                                     RTI_INT32 try_index,
                                     struct REDA_StringSeq *transport,
                                     NETIO_RouteKind_T kind,
                                     struct NETIO_AddressSeq *reserved_address,
                                     struct NETIO_AddressSeq *mc_locator_seq,
                                     struct NETIO_AddressSeq *uc_locator_seq);

NETIODllExport RTI_BOOL
NETIO_BindResolver_unbind(NETIO_BindResolver_T* r_table,
                         struct REDA_StringSeq *transport,
                         NETIO_RouteKind_T kind,
                         struct NETIO_Address *from_address,
                         NETIO_Interface_T *to_intf,
                         struct NETIO_Address *to_address);

NETIODllExport RTI_BOOL
NETIO_BindResolver_bind(NETIO_BindResolver_T* r_table,
                        struct REDA_StringSeq *transport,
                        NETIO_RouteKind_T kind,
                        struct NETIO_Address *from_address,
                        NETIO_Interface_T *to_intf,
                        struct NETIO_Address *to_address);

#ifdef __cplusplus
}
#endif


#endif /* netio_route_h */
