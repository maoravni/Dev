/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef NETIORouteResolver_pkg_h
#define NETIORouteResolver_pkg_h

struct NETIO_RouteResolverRecord
{
    struct NETIO_Address address;
    struct NETIO_Netmask netmask;
    NETIO_Interface_T *intf;
};

struct NETIO_RouteResolver
{
    struct NETIO_RouteResolverProperty property;
    DB_Table_T route_table;
    DB_Database_T db;
};

extern void
NETIO_RouteResolver_print_route_table(NETIO_RouteResolver_T *r_table);

#endif /* NETIORouteResolver_pkg_h */
