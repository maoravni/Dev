/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DomainFactory_pkg_h
#define DomainFactory_pkg_h

struct DDS_DomainParticipantFactoryImpl
{
    DDS_Boolean is_initialized;
    struct OSAPI_Mutex *factory_lock;
    struct DDS_DomainParticipantFactoryQos qos;
    struct DDS_DomainParticipantQos default_participant_qos;
    RT_Registry_T *registry;
    REDA_BufferPool_T participant_pool;
    REDA_CircularList_T participants;
    DDS_Boolean immutable_qos_enabled;
    DB_Database_T db;
    DDS_Long instance_counter;
};

#endif /* DomainFactory_pkg_h */
