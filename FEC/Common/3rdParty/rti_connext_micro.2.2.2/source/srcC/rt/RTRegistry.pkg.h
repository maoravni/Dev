/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef RTRegistry_pkg_h
#define RTRegistry_pkg_h

struct RT_Registry
{
    RTI_BOOL _is_initialized;
    DB_Table_T factory_table;
    struct RT_RegistryProperty property;
};

#endif /* RTRegistry_pkg_h */
