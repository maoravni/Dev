/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef osapi_types_h
#include "osapi/osapi_types.h"
#endif
#ifndef reda_bufferpool_h
#include "reda/reda_bufferpool.h"
#endif
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif
#ifndef dds_c_discovery_plugin_h
#include "dds_c/dds_c_discovery_plugin.h"
#endif

#include "Entity.pkg.h"
#include "SubscriberEvent.pkg.h"
#include "SubscriberQos.pkg.h"
#include "DataReaderQos.pkg.h"
#include "DataReaderEvent.pkg.h"
#include "DataReader.pkg.h"
#include "DataReaderImpl.pkg.h"
#include "SubscriberImpl.pkg.h"

/*** SOURCE_BEGIN ***/

DDS_ReturnCode_t
DDS_Subscriber_enable(DDS_Entity *self)
{
    DB_ReturnCode_T dbrc;
    struct DDS_SubscriberImpl *subscriber = (struct DDS_SubscriberImpl *)self;
    DB_Cursor_T handle;
    DDS_ReturnCode_t ddsrc = DDS_RETCODE_OK;
    struct DDS_DataReaderImpl *reader;

    if (subscriber == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (subscriber->as_entity.state == RTIDDS_ENTITY_STATE_ENABLED)
    {
        return DDS_RETCODE_OK;
    }

    DB_Database_lock(subscriber->config.db);

    if (!DDS_Entity_is_enabled
        (DDS_DomainParticipant_as_entity(subscriber->participant)))
    {
        ddsrc = DDS_RETCODE_PRECONDITION_NOT_MET;
        goto done;
    }

    self->state = RTIDDS_ENTITY_STATE_ENABLED;

    dbrc = DB_Table_select_all_default(subscriber->config.local_reader_table,&handle);
    do
    {
        dbrc = DB_Cursor_get_next(handle, (DB_Record_T*) &reader);
        if ((dbrc == DB_RETCODE_OK) && (reader->subscriber == subscriber))
        {
            DDS_DataReader_enable(&reader->as_entity);
        }
    }  while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(subscriber->config.local_reader_table,handle);

    if (dbrc != DB_RETCODE_OK)
    {
        ddsrc = DDS_RETCODE_ERROR;
    }

 done:
        DB_Database_unlock(subscriber->config.db);

        return DDS_RETCODE_OK;
}

DDS_DataReader*
DDS_Subscriber_create_datareader(DDS_Subscriber *self,
                                 DDS_TopicDescription *topic,
                                 const struct DDS_DataReaderQos *qos,
                                 const struct DDS_DataReaderListener *listener,
                                 DDS_StatusMask mask)
{
    struct NDDS_DataReaderConfig dr_config = NDDS_DataReaderConfig_INITIALIZER;
    struct DDS_SubscriberImpl *subscriber = (struct DDS_SubscriberImpl *)self;
    struct DDS_DataReaderImpl *datareader = NULL;
    DDS_DataReader *retval = NULL;
    DB_ReturnCode_T db_rc;
    DDS_Long dr_oid;

    if ((subscriber == NULL) || (topic == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return NULL;
    }

    dr_oid = subscriber->config.object_id_generator();
    dr_config.db = subscriber->config.db;
    dr_config.registry = subscriber->config.registry;
    dr_config.timer = subscriber->config.timer;
    dr_config.on_after_enabled = subscriber->config.on_after_datareader_enabled;
    dr_config.on_after_modified = subscriber->config.on_after_datareader_modified;
    dr_config.get_parent_handle = DDS_SubscriberImpl_get_instance_handle;
    dr_config.default_multicast = subscriber->config.default_multicast;
    dr_config.default_unicast = subscriber->config.default_unicast;
    dr_config.default_meta_multicast = subscriber->config.default_meta_multicast;
    dr_config.default_meta_unicast = subscriber->config.default_meta_unicast;
    dr_config.enabled_transports = subscriber->config.enabled_transports;
    dr_config.route_resolver = subscriber->config.route_resolver;
    dr_config.bind_resolver = subscriber->config.bind_resolver;

    datareader = NULL;

    DB_Database_lock(subscriber->config.db);

    db_rc = DB_Table_create_record(subscriber->config.local_reader_table,
                                 (DB_Record_T*)&datareader);
    if (db_rc != DB_RETCODE_OK)
    {
        goto done;
    }

    if (!DDS_DataReaderImpl_initialize(datareader,
                                       subscriber,
                                       topic,
                                       qos,
                                       listener,
                                       mask,
                                       dr_oid,
                                       &dr_config))
    {
        goto done;
    }

    db_rc = DB_Table_insert_record(subscriber->config.local_reader_table,
                                  (DB_Record_T*)datareader);
    if (db_rc != DB_RETCODE_OK)
    {
        goto done;
    }

    ++subscriber->dr_count;

    if ((self->as_entity.state == RTIDDS_ENTITY_STATE_ENABLED) &&
         self->qos.entity_factory.autoenable_created_entities)
    {
        if (DDS_DataReader_enable(DDS_DataReader_as_entity(datareader))
                != DDS_RETCODE_OK)
        {
            goto done;
        }
    }

    if (subscriber->config.on_after_datareader_created)
    {
        subscriber->config.on_after_datareader_created(datareader,qos);
    }

    retval = (DDS_DataReader*)datareader;

done:

    if ((retval == NULL) && (datareader != NULL))
    {
        db_rc = DB_Table_delete_record(subscriber->config.local_reader_table,
                                     (DB_Record_T*)datareader);
    }

    DB_Database_unlock(subscriber->config.db);

    return retval;
}

DDS_ReturnCode_t
DDS_Subscriber_delete_datareader(DDS_Subscriber *self,
                                 DDS_DataReader *datareader)
{
    struct DDS_SubscriberImpl *subscriber = (struct DDS_SubscriberImpl *)self;
    DB_ReturnCode_T db_rc;
    DDS_ReturnCode_t retval = DDS_RETCODE_OK;

    if ((self == NULL) || (datareader == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DB_Database_lock(subscriber->config.db);

    if (subscriber->config.on_before_datareader_deleted)
    {
        subscriber->config.on_before_datareader_deleted(datareader);
    }

    if (!DDS_DataReaderImpl_finalize(datareader))
    {
        retval = DDS_RETCODE_ERROR;
        goto done;
    }

    db_rc = DB_Table_delete_record(subscriber->config.local_reader_table,
                                  (DB_Record_T*)datareader);
    if (db_rc != DB_RETCODE_OK)
    {
        retval = DDS_RETCODE_ERROR;
        goto done;
    }

    --subscriber->dr_count;

done:
    DB_Database_unlock(subscriber->config.db);

    return retval;

}

DDS_DomainParticipant*
DDS_Subscriber_get_participant(DDS_Subscriber *self)
{
    struct DDS_SubscriberImpl *subscriber = (struct DDS_SubscriberImpl *)self;

    if (subscriber == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return NULL;
    }

    return subscriber->participant;
}

DDS_ReturnCode_t
DDS_Subscriber_delete_contained_entities(DDS_Subscriber * self)
{
    struct DDS_SubscriberImpl *subscriber = (struct DDS_SubscriberImpl*)self;
    struct DDS_DataReaderImpl *datareader = NULL;
    DB_ReturnCode_T dbrc;
    DB_Cursor_T handle = NULL;
    DDS_ReturnCode_t retcode = DDS_RETCODE_OK;

    if (subscriber == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (self->qos.management.is_hidden)
    {
        return DDS_RETCODE_OK;
    }

    DB_Database_lock(subscriber->config.db);

    dbrc = DB_Table_select_all_default(subscriber->config.local_reader_table,
                                      &handle);
    do
    {
        dbrc = DB_Cursor_get_next(handle, (DB_Record_T*)&datareader);
        if ((dbrc == DB_RETCODE_OK) && (datareader->subscriber == self))
        {
            if (subscriber->config.on_before_datareader_deleted)
            {
                subscriber->config.on_before_datareader_deleted(datareader);
            }
            if (!DDS_DataReaderImpl_finalize(datareader))
            {
                break;
            }
            dbrc = DB_Table_delete_record(subscriber->config.local_reader_table,
                                        (DB_Record_T*)datareader);
            if (dbrc != DB_RETCODE_OK)
            {
                break;
            }
            --subscriber->dr_count;
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(subscriber->config.local_reader_table,handle);

    if ((dbrc != DB_RETCODE_NO_DATA) && (dbrc != DB_RETCODE_OK))
    {
        retcode = DDS_RETCODE_ERROR;
    }

    DB_Database_unlock(subscriber->config.db);

    return retcode;
}

DDS_ReturnCode_t
DDS_Subscriber_set_listener(DDS_Subscriber * self,
                            const struct DDS_SubscriberListener * l,
                            DDS_StatusMask mask)
{
    DDS_ReturnCode_t retcode = DDS_RETCODE_PRECONDITION_NOT_MET;
    struct DDS_SubscriberListener default_listener =
                                            DDS_SubscriberListener_INITIALIZER;
    struct DDS_SubscriberImpl *subscriber = (struct DDS_SubscriberImpl*)self;

    if (subscriber == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DB_Database_lock(subscriber->config.db);

    if (l == NULL)
    {
        if (mask == DDS_STATUS_MASK_NONE)
        {
            subscriber->mask = mask;
            subscriber->listener = default_listener;
            retcode = DDS_RETCODE_OK;
        }
        goto done;
    }

    if (!DDS_SubscriberImpl_listener_is_consistent(l, mask))
    {
        DDSC_Log_error(LOG_INCONSISTENT_SUBSCRIBER_LISTENER);
        goto done;
    }

    subscriber->listener = *l;
    subscriber->mask = mask;

    retcode = DDS_RETCODE_OK;

done:
    DB_Database_unlock(subscriber->config.db);

    return retcode;
}

/*******************************************************************************
 *                             OPTIONAL APIs
 ******************************************************************************/
#ifdef INCLUDE_API_LISTENER
struct DDS_SubscriberListener
DDS_Subscriber_get_listener(DDS_Subscriber *self)
{
    struct DDS_SubscriberImpl *subscriber = (struct DDS_SubscriberImpl*)self;
    struct DDS_SubscriberListener retval = DDS_SubscriberListener_INITIALIZER;

    if (subscriber == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return retval;
    }

    DB_Database_lock(subscriber->config.db);

    retval = subscriber->listener;

    DB_Database_unlock(subscriber->config.db);

    return retval;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_Subscriber_get_default_datareader_qos(DDS_Subscriber * self,
                                          struct DDS_DataReaderQos * qos)
{
    struct DDS_SubscriberImpl *subscriber = (struct DDS_SubscriberImpl *)self;

    if (subscriber == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (qos == NULL)
    {
        DDSC_Log_error(LOG_BAD_SECOND_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DB_Database_lock(subscriber->config.db);

    DDS_DataReaderQos_copy(qos, &self->default_qos);

    DB_Database_unlock(subscriber->config.db);

    return DDS_RETCODE_OK;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_Subscriber_set_default_datareader_qos(DDS_Subscriber *self,
                                          const struct DDS_DataReaderQos * qos)
{
    struct DDS_SubscriberImpl *subscriber = (struct DDS_SubscriberImpl *)self;

    if (subscriber == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (qos == NULL)
    {
        DDSC_Log_error(LOG_BAD_SECOND_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (!DDS_DataReaderQos_is_consistent(qos))
    {
        DDSC_Log_error(LOG_INCONSISTENT_DATAREADER_QOS);
        return DDS_RETCODE_INCONSISTENT_POLICY;
    }

    DB_Database_lock(subscriber->config.db);

    DDS_DataReaderQos_copy(&self->default_qos, qos);

    DB_Database_unlock(subscriber->config.db);

    return DDS_RETCODE_OK;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_Subscriber_set_qos(DDS_Subscriber *self,
                       const struct DDS_SubscriberQos *qos)
{
    struct DDS_SubscriberImpl *subscriber = (struct DDS_SubscriberImpl*)self;
    DDS_ReturnCode_t retcode;

    if (subscriber)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (qos == NULL)
    {
        DDSC_Log_error(LOG_BAD_SECOND_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (!DDS_SubscriberQos_is_consistent(qos))
    {
        DDSC_Log_error(LOG_INCONSISTENT_SUBSCRIBER_QOS);
        return DDS_RETCODE_INCONSISTENT_POLICY;
    }

    DB_Database_lock(subscriber->config.db);

    if (DDS_Entity_is_enabled(&self->as_entity))
    {
        retcode = DDS_RETCODE_PRECONDITION_NOT_MET;
        goto done;
    }

    self->qos = *qos;

    retcode = DDS_RETCODE_OK;

done:

    DB_Database_unlock(subscriber->config.db);

    return retcode;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_Subscriber_get_qos(DDS_Subscriber *self, struct DDS_SubscriberQos *qos)
{
    struct DDS_SubscriberImpl *subscriber = (struct DDS_SubscriberImpl*)self;

    if (subscriber == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (qos == NULL)
    {
        DDSC_Log_error(LOG_BAD_SECOND_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DB_Database_lock(subscriber->config.db);

    *qos = self->qos;

    DB_Database_unlock(subscriber->config.db);

    return DDS_RETCODE_OK;
}
#endif

#if INCLUDE_API_LOOKUP
DDS_DataReader*
DDS_Subscriber_lookup_datareader(DDS_Subscriber *self, const char *topic_name)
{
    struct DDS_SubscriberImpl *subscriber = (struct DDS_SubscriberImpl *)self;
    DB_ReturnCode_T dbrc;
    DB_Cursor_T handle;
    struct DDS_DataReaderImpl *datareader = NULL;

    if (subscriber == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return NULL;
    }

    if (topic_name == NULL)
    {
        DDSC_Log_error(LOG_BAD_SECOND_PARAMETER_IS_NULL);
        return NULL;
    }

    DB_Database_lock(subscriber->config.db);

    dbrc = DB_Table_select_all_default(subscriber->config.local_reader_table,
                                           &handle);
    if (dbrc != DB_RETCODE_OK)
    {
        DDSC_Log_error(LOG_FAIL_SELECT_DATAWRITER_RECORD);
        DB_Database_unlock(subscriber->config.db);
        return NULL;
    }

    dbrc = DB_Cursor_get_next(handle, (DB_Record_T*)&datareader);
    while ((dbrc != DB_RETCODE_NO_DATA) && (dbrc != DB_RETCODE_BAD_PARAMETER))
    {
        if (!DDS_String_cmp
            (DDS_TopicDescription_get_name
             (DDS_Topic_as_topicdescription(datareader->topic)), topic_name))
        {
            break;
        }
        datareader = NULL;
        dbrc = DB_Cursor_get_next(handle, (DB_Record_T*)&datareader);
    }

    DB_Cursor_finish(subscriber->config.local_reader_table,handle);
    DB_Database_unlock(subscriber->config.db);

    return datareader;
}
#endif
