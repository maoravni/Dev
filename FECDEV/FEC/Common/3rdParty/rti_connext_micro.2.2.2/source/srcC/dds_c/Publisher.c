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
#ifndef osapi_log_h
#include "osapi/osapi_log.h"
#endif
#ifndef reda_bufferpool_h
#include "reda/reda_bufferpool.h"
#endif
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif

#include "Entity.pkg.h"
#include "DataWriterImpl.pkg.h"
#include "DataWriterQos.pkg.h"
#include "PublisherImpl.pkg.h"
#include "PublisherQos.pkg.h"

/*** SOURCE_BEGIN ***/

DDS_ReturnCode_t
DDS_Publisher_enable(DDS_Entity *self)
{
    DB_ReturnCode_T dbrc;
    struct DDS_PublisherImpl *publisher = (struct DDS_PublisherImpl*) self;
    struct DDS_DataWriterImpl *writer;
    DB_Cursor_T handle = NULL;
    DDS_ReturnCode_t ddsrc;

    if (publisher == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (publisher->as_entity.state == RTIDDS_ENTITY_STATE_ENABLED)
    {
        return DDS_RETCODE_OK;
    }

    DB_Database_lock(publisher->config.db);

    if (!DDS_Entity_is_enabled(
              DDS_DomainParticipant_as_entity(publisher->participant)))
    {
        ddsrc = DDS_RETCODE_PRECONDITION_NOT_MET;
        goto done;
    }

    publisher->as_entity.state = RTIDDS_ENTITY_STATE_ENABLED;

    dbrc = DB_Table_select_all_default(
                                publisher->config.local_writer_table,
                                &handle);
    if (dbrc != DB_RETCODE_OK)
    {
        ddsrc = DDS_RETCODE_ERROR;
        goto done;
    }

    dbrc = DB_Cursor_get_next(handle, (DB_Record_T*)&writer);
    while ((dbrc != DB_RETCODE_NO_DATA) && (dbrc != DB_RETCODE_BAD_PARAMETER))
    {
        if (writer->publisher == publisher)
        {
            DDS_DataWriter_enable(&writer->as_entity);
        }
        dbrc = DB_Cursor_get_next(handle,(DB_Record_T*)&writer);
    }
    DB_Cursor_finish(publisher->config.local_writer_table,handle);

    ddsrc = DDS_RETCODE_OK;

done:

    DB_Database_unlock(publisher->config.db);

    return ddsrc;
}

DDS_DataWriter*
DDS_Publisher_create_datawriter(DDS_Publisher *self,
                                DDS_Topic *topic,
                                const struct DDS_DataWriterQos *qos,
                                const struct DDS_DataWriterListener *listener,
                                DDS_StatusMask mask)
{
    struct NDDS_DataWriterConfig dw_config = NDDS_DataWriterConfig_INITIALIZER;
    struct DDS_PublisherImpl *publisher = (struct DDS_PublisherImpl *)self;
    struct DDS_DataWriterImpl *datawriter = NULL;
    DDS_DataWriter *retval = NULL;
    DB_ReturnCode_T db_rc;
    DDS_Long dw_oid;

    if ((self == NULL) || (topic == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return NULL;
    }

    dw_oid = publisher->config.object_id_generator();
    dw_config.db = publisher->config.db;
    dw_config.registry = publisher->config.registry;
    dw_config.timer = publisher->config.timer;
    dw_config.bind_resolver = publisher->config.bind_resolver;
    dw_config.on_after_enabled = publisher->config.on_after_datawriter_enabled;
    dw_config.on_after_modified = publisher->config.on_after_datawriter_modified;
    dw_config.get_parent_handle = DDS_PublisherImpl_get_instance_handle;
    dw_config.default_multicast = publisher->config.default_multicast;
    dw_config.default_unicast = publisher->config.default_unicast;
    dw_config.default_meta_multicast = publisher->config.default_meta_multicast;
    dw_config.default_meta_unicast = publisher->config.default_meta_unicast;
    dw_config.enabled_transports = publisher->config.enabled_transports;
    dw_config.route_resolver = publisher->config.route_resolver;
    datawriter = NULL;

    DB_Database_lock(publisher->config.db);

    db_rc = DB_Table_create_record(publisher->config.local_writer_table,
                                 (DB_Record_T*)&datawriter);
    if (db_rc != DB_RETCODE_OK)
    {
        goto done;
    }

    if (!DDS_DataWriterImpl_initialize(datawriter,
                                       publisher,
                                       topic,
                                       qos,
                                       listener,
                                       mask,
                                       dw_oid,
                                       &dw_config))
    {
        goto done;
    }

    db_rc = DB_Table_insert_record(publisher->config.local_writer_table,
                                  (DB_Record_T*)datawriter);
    if (db_rc != DB_RETCODE_OK)
    {
        goto done;
    }

    ++publisher->dw_count;

    if ((self->as_entity.state == RTIDDS_ENTITY_STATE_ENABLED) &&
         self->qos.entity_factory.autoenable_created_entities)
    {
        if (DDS_DataWriter_enable(DDS_DataWriter_as_entity(datawriter))
                != DDS_RETCODE_OK)
        {
            goto done;
        }
    }

    if (publisher->config.on_after_datawriter_created)
    {
        publisher->config.on_after_datawriter_created(datawriter,qos);
    }

    retval = (DDS_DataWriter*)datawriter;

done:

    if ((retval == NULL) && (datawriter != NULL))
    {
        (void)DB_Table_delete_record(publisher->config.local_writer_table,
                                    (DB_Record_T*)datawriter);
    }

    DB_Database_unlock(publisher->config.db);

    return retval;
}

DDS_ReturnCode_t
DDS_Publisher_delete_datawriter(DDS_Publisher *self,
                                DDS_DataWriter *datawriter)
{
    struct DDS_PublisherImpl *publisher = (struct DDS_PublisherImpl *)self;
    DB_ReturnCode_T db_rc;
    DDS_ReturnCode_t retval = DDS_RETCODE_OK;

    if ((self == NULL) || (datawriter == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DB_Database_lock(publisher->config.db);

    if (publisher->config.on_before_datawriter_deleted)
    {
        publisher->config.on_before_datawriter_deleted(datawriter);
    }

    if (!DDS_DataWriterImpl_finalize(datawriter))
    {
        retval = DDS_RETCODE_ERROR;
        goto done;
    }

    db_rc = DB_Table_delete_record(publisher->config.local_writer_table,
                                  (DB_Record_T*)datawriter);
    if (db_rc != DB_RETCODE_OK)
    {
        retval = DDS_RETCODE_ERROR;
        goto done;
    }

    --self->dw_count;

done:
    DB_Database_unlock(publisher->config.db);

    return retval;
}

DDS_DomainParticipant*
DDS_Publisher_get_participant(DDS_Publisher *self)
{
    struct DDS_PublisherImpl *publisher = (struct DDS_PublisherImpl*)self;

    if (publisher == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return NULL;
    }

    return publisher->participant;
}

DDS_ReturnCode_t
DDS_Publisher_delete_contained_entities(DDS_Publisher * self)
{
    DB_ReturnCode_T dbrc;
    struct DDS_DataWriterImpl *datawriter = NULL;
    DB_Cursor_T handle = NULL;
    DDS_ReturnCode_t retcode = DDS_RETCODE_OK;
    struct DDS_PublisherImpl *publisher = (struct DDS_PublisherImpl*)self;

    if (publisher == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (self->qos.management.is_hidden)
    {
        return DDS_RETCODE_OK;
    }

    DB_Database_lock(publisher->config.db);

    dbrc = DB_Table_select_all_default(publisher->config.local_writer_table,
                                      &handle);
    do
    {
        dbrc = DB_Cursor_get_next(handle, (DB_Record_T*)&datawriter);
        if ((dbrc == DB_RETCODE_OK) && (datawriter->publisher == self))
        {
            if (publisher->config.on_before_datawriter_deleted)
            {
                publisher->config.on_before_datawriter_deleted(datawriter);
            }
            if (!DDS_DataWriterImpl_finalize(datawriter))
            {
                break;
            }
            dbrc = DB_Table_delete_record(publisher->config.local_writer_table,
                                        (DB_Record_T*)datawriter);
            if (dbrc != DB_RETCODE_OK)
            {
                break;
            }
            --self->dw_count;
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(publisher->config.local_writer_table,handle);

    if ((dbrc != DB_RETCODE_NO_DATA) && (dbrc != DB_RETCODE_OK))
    {
        retcode = DDS_RETCODE_ERROR;
    }

    DB_Database_unlock(publisher->config.db);

    return retcode;
}

DDS_ReturnCode_t
DDS_Publisher_set_listener(DDS_Publisher * self,
                           const struct DDS_PublisherListener *l,
                           DDS_StatusMask mask)
{
    DDS_ReturnCode_t retcode = DDS_RETCODE_PRECONDITION_NOT_MET;
    struct DDS_PublisherImpl *publisher = (struct DDS_PublisherImpl*) self;
    struct DDS_PublisherListener default_listener =
                                            DDS_PublisherListener_INITIALIZER;

    if (publisher == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DB_Database_lock(publisher->config.db);

    if (l == NULL)
    {
        if (mask == DDS_STATUS_MASK_NONE)
        {
            self->mask = mask;
            self->listener = default_listener;
            retcode = DDS_RETCODE_OK;
        }
        goto done;
    }

    if (!DDS_PublisherImpl_listener_is_consistent(l, mask))
    {
        DDSC_Log_error(LOG_INCONSISTENT_PUBLISHER_LISTENER);
        goto done;
    }

    self->listener = *l;
    self->mask = mask;

    retcode = DDS_RETCODE_OK;

done:

    DB_Database_unlock(publisher->config.db);

    return retcode;
}

/*******************************************************************************
 *                             OPTIONAL APIs
 ******************************************************************************/
#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_Publisher_get_default_datawriter_qos(DDS_Publisher *self,
                                         struct DDS_DataWriterQos * qos)
{
    struct DDS_PublisherImpl *publisher = (struct DDS_PublisherImpl *)self;;

    if ((publisher == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DB_Database_lock(publisher->config.db);

    DDS_DataWriterQos_copy(qos, &self->default_qos);

    DB_Database_unlock(publisher->config.db);

    return DDS_RETCODE_OK;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_Publisher_set_default_datawriter_qos(DDS_Publisher * self,
                                         const struct DDS_DataWriterQos * qos)
{
    struct DDS_PublisherImpl *publisher = (struct DDS_PublisherImpl *)self;;

    if ((publisher == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (!DDS_DataWriterQos_is_consistent(qos))
    {
        DDSC_Log_error(LOG_INCONSISTENT_DATAWRITER_QOS);
        return DDS_RETCODE_INCONSISTENT_POLICY;
    }

    DB_Database_lock(publisher->config.db);

    DDS_DataWriterQos_copy(&self->default_qos, qos);

    DB_Database_unlock(publisher->config.db);

    return DDS_RETCODE_OK;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_Publisher_get_qos(DDS_Publisher *self, struct DDS_PublisherQos *qos)
{
    struct DDS_PublisherImpl *publisher = (struct DDS_PublisherImpl*)self;

    if ((publisher == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    DB_Database_lock(publisher->config.db);

    *qos = self->qos;

    DB_Database_unlock(publisher->config.db);

    return DDS_RETCODE_OK;
}
#endif

#if INCLUDE_API_QOS
DDS_ReturnCode_t
DDS_Publisher_set_qos(DDS_Publisher * self, const struct DDS_PublisherQos * qos)
{
    DDS_ReturnCode_t retcode;
    struct DDS_PublisherImpl *publisher = (struct DDS_PublisherImpl*) self;

    if ((publisher == NULL) || (qos == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return DDS_RETCODE_BAD_PARAMETER;
    }

    if (!DDS_PublisherQos_is_consistent(qos))
    {
        DDSC_Log_error(LOG_INCONSISTENT_PUBLISHER_QOS);
        return DDS_RETCODE_INCONSISTENT_POLICY;
    }

    DB_Database_lock(publisher->config.db);

    if (DDS_Entity_is_enabled(&self->as_entity))
    {
        retcode = DDS_RETCODE_PRECONDITION_NOT_MET;
        goto done;
    }

    self->qos = *qos;

    retcode = DDS_RETCODE_OK;

done:

    DB_Database_unlock(publisher->config.db);

    return retcode;
}
#endif

#if INCLUDE_API_LOOKUP
DDS_DataWriter*
DDS_Publisher_lookup_datawriter(DDS_Publisher * self, const char *topic_name)
{
    DB_ReturnCode_T dbrc;
    DB_Cursor_T handle = NULL;
    struct DDS_DataWriterImpl *datawriter = NULL;
    struct DDS_PublisherImpl *publisher = (struct DDS_PublisherImpl*)self;

    if ((publisher == NULL) || (topic_name == NULL))
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return NULL;
    }

    DB_Database_lock(publisher->config.db);

    dbrc = DB_Table_select_all_default(publisher->config.local_writer_table,
                                           &handle);
    dbrc = DB_Cursor_get_next(handle, (DB_Record_T*)&datawriter);
    do
    {
        if (!DDS_String_cmp
            (DDS_TopicDescription_get_name
             (DDS_Topic_as_topicdescription(datawriter->topic)), topic_name))
        {
            break;
        }
        datawriter = NULL;
        dbrc = DB_Cursor_get_next(handle, (DB_Record_T*)&datawriter);
    } while (dbrc == DB_RETCODE_OK);

    if ((dbrc != DB_RETCODE_NO_DATA) && (dbrc != DB_RETCODE_OK))
    {
        DDSC_Log_error(LOG_FAIL_SELECT_DATAWRITER_RECORD);
    }

    DB_Cursor_finish(publisher->config.local_writer_table,handle);

    DB_Database_unlock(publisher->config.db);

    return datawriter;
}
#endif

#ifdef INCLUDE_API_LISTENER
struct DDS_PublisherListener
DDS_Publisher_get_listener(DDS_Publisher * self)
{
    struct DDS_PublisherListener retval = DDS_PublisherListener_INITIALIZER;
    struct DDS_PublisherImpl *publisher = (struct DDS_PublisherImpl*)self;

    if (publisher == NULL)
    {
        DDSC_Log_error(LOG_BAD_FIRST_PARAMETER_IS_NULL);
        return retval;
    }

    DB_Database_lock(publisher->config.db);

    retval = publisher->listener;

    DB_Database_unlock(publisher->config.db);

    return retval;
}
#endif
