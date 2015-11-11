/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif
#ifndef dds_c_discovery_plugin_h
#include "dds_c/dds_c_discovery_plugin.h"
#endif

#include "Entity.pkg.h"
#include "RemoteEntity.pkg.h"
#include "RemoteEndpoint.pkg.h"
#include "QosPolicy.pkg.h"
#include "BuiltinTopicKey.pkg.h"
#include "TopicDescription.pkg.h"
#include "TopicQos.pkg.h"
#include "Topic.pkg.h"
#include "Type.pkg.h"
#include "DomainParticipantQos.pkg.h"
#include "DomainParticipantEvent.pkg.h"
#include "DomainParticipant.pkg.h"
#include "RemoteParticipant.pkg.h"
#include "DataWriterDiscovery.pkg.h"

const char *DDS_SUBSCRIPTION_BUILTIN_TOPIC_TYPE_NAME = "DDS_SubscriptionBuiltinTopicData";
const char *DDS_SUBSCRIPTION_BUILTIN_TOPIC_NAME = "DCPSSubscription";

/*******************************************************************************
 *                                 Internal API
 ******************************************************************************/
struct DDS_RemoteSubscriptionImpl
{
    struct NDDS_RemoteEntityImpl as_entity;
    /* The first field in the builtin topic data is the GUID, 
     * which is the key 
     */
    struct DDS_SubscriptionBuiltinTopicData data;
    DDS_BuiltinTopicKey_t orig_key;
};

DDS_ReturnCode_t
NDDS_RemoteSubscription_enable(DDS_DomainParticipant *const participant,
                               NDDS_RemoteEntity *entity,
                               const DDS_BuiltinTopicKey_t *key);

/*** SOURCE_BEGIN ***/

RTI_INT32
DDS_RemoteSubscriptionImpl_compare(DB_Select_T opcode,RTI_INT32 flags,
                                   const DB_Record_T op1, void *op2, void *op3)
{
    struct DDS_RemoteSubscriptionImpl *record_left =
                                        (struct DDS_RemoteSubscriptionImpl*)op1;
    const DDS_BuiltinTopicKey_t *key_right;

    if (DB_SELECT_OP2_IS_KEY(flags))
    {
        key_right = (const DDS_BuiltinTopicKey_t*)op2;
    }
    else
    {
        key_right = &((struct DDS_RemoteSubscriptionImpl*)op2)->data.key;
    }

    return DDS_BuiltinTopicKey_compare(&record_left->data.key,key_right);
}

void
NDDS_RemoteSubscription_match_local_writer(DDS_DomainParticipant *const self,
        const struct DDS_ParticipantBuiltinTopicData *const parent_data,
        const struct DDS_SubscriptionBuiltinTopicData *const data)
{
    struct DDS_DomainParticipantImpl *participant =
                                    (struct DDS_DomainParticipantImpl *)self;
    struct DDS_DataWriterImpl *local_writer;
    DB_Cursor_T dw_cursor = NULL;
    DB_ReturnCode_T dbrc;

    (void)DB_Table_select_all(participant->local_writer_table,
                             DB_TABLE_DEFAULT_INDEX,&dw_cursor);
    do
    {
        dbrc = DB_Cursor_get_next(dw_cursor,(DB_Record_T*)&local_writer);
        if (dbrc == DB_RETCODE_OK)
        {
            (void)DDS_DataWriter_match_remote_reader(local_writer,parent_data,data);
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(participant->local_writer_table,dw_cursor);
}

void
NDDS_RemoteSubscription_unmatch_local_writer(DDS_DomainParticipant *const self,
        const struct DDS_ParticipantBuiltinTopicData *const parent_data,
        const struct DDS_SubscriptionBuiltinTopicData *const data)
{
    struct DDS_DomainParticipantImpl *participant =
                                    (struct DDS_DomainParticipantImpl *)self;
    struct DDS_DataWriterImpl *local_writer;
    DB_Cursor_T dw_cursor = NULL;
    DB_ReturnCode_T dbrc;

    (void)DB_Table_select_all(participant->local_writer_table,
                             DB_TABLE_DEFAULT_INDEX,&dw_cursor);
    do
    {
        dbrc = DB_Cursor_get_next(dw_cursor,(DB_Record_T*)&local_writer);
        if (dbrc == DB_RETCODE_OK)
        {
            (void)DDS_DataWriter_unmatch_remote_reader(local_writer,
                    parent_data,data);
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(participant->local_writer_table,dw_cursor);
}

/*******************************************************************************
 *
 *                                 Public API
 *
 ******************************************************************************/
DDS_ReturnCode_t
NDDS_DomainParticipant_assert_remote_subscription(
                    DDS_DomainParticipant *const participant,
                    const char *const participant_name,
                    const struct DDS_SubscriptionBuiltinTopicData *const data,
                    NDDS_TypePluginKeyKind key_kind)
{
    struct DDS_DomainParticipantImpl *self =
                        (struct DDS_DomainParticipantImpl *)participant;
    struct DDS_RemoteSubscriptionImpl *rem_subscription = NULL;
    DB_ReturnCode_T dbrc;
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;
    struct DDS_RemoteParticipantImpl *rem_participant = NULL;

    if ((participant == NULL) || (data == NULL))
    {
        return DDS_RETCODE_BAD_PARAMETER;
    }

    /* Note that in RTI DDS, Subscriptions (Publisher + DataWriter) are 
     * published. This makes the data-model a little cluttered. We could
     * keep the data for a publication in one table, at the expense of
     * duplicating data for the parent.
     */

    /* TODO: Define DDS_PARTICIPANT_NAME_NIL */
    /* NOTE: We allow participant_name to be NULL. This means the data
     * contains the GUID of the parent participant. If name != NULL, it means
     * we explicitly specify the parent name.
     */
    if (DDS_BuiltinTopicKey_suffix_equals(&data->key,
                                          &DDS_BUILTINTOPICKEY_UNKNOWN))
    {
        return DDS_RETCODE_PRECONDITION_NOT_MET;
    }

    DB_Database_lock(self->database);

    rem_participant = NDDS_RemoteEndpoint_find_parent(participant,
                   &data->participant_key,participant_name,&data->key);

    if (rem_participant == NULL)
    {
        DB_Database_unlock(self->database);
        return retcode;
    }

    rem_subscription = NULL;
    dbrc = DB_Table_select_match(self->remote_subscriber_table,
            DB_TABLE_DEFAULT_INDEX,(DB_Record_T*)&rem_subscription,
            (DB_Key_T)&data->key);


    if (dbrc == DB_RETCODE_NOT_EXISTS)
    {
        dbrc = DB_Table_create_record(self->remote_subscriber_table,
                                     (DB_Record_T*)&rem_subscription);
        if (dbrc != DB_RETCODE_OK)
        {
            retcode = DDS_RETCODE_OUT_OF_RESOURCES;
            goto done;
        }

        if (!DDS_SubscriptionBuiltinTopicData_initialize(&rem_subscription->data))
        {
            retcode = DDS_RETCODE_OUT_OF_RESOURCES;
            goto done;
        }

        if (!DDS_SubscriptionBuiltinTopicData_copy(&rem_subscription->data,data))
        {
            retcode = DDS_RETCODE_OUT_OF_RESOURCES;
            goto done;
        }

        if (key_kind == NDDS_TYPEPLUGIN_USER_KEY)
        {
            rem_subscription->data.key.value[3] =
                        (rem_subscription->data.key.value[3] << 8) |
                        MIG_RTPS_OBJECT_NORMAL_USER_CST_READER;
        }
        else if (key_kind == NDDS_TYPEPLUGIN_NO_KEY)
        {
            rem_subscription->data.key.value[3] =
                        (rem_subscription->data.key.value[3] << 8) |
                        MIG_RTPS_OBJECT_NORMAL_USER_SUBSCRIPTION;
        }

        /* Update the participant_key field as it may have changed from the 
         * based on the input parameters
         */
        rem_subscription->data.participant_key = rem_participant->data.key;
        rem_subscription->as_entity.enable_func = NDDS_RemoteSubscription_enable;
        rem_subscription->as_entity.kind = DDS_SUBSCRIBER_ENTITY_KIND;
        rem_subscription->as_entity.state = RTIDDS_ENTITY_STATE_CREATED;
        rem_subscription->orig_key = rem_subscription->data.key;

        /* Insert into the table */
        dbrc = DB_Table_insert_record(self->remote_subscriber_table,
                rem_subscription);
        if (dbrc != DB_RETCODE_OK)
        {
            DDSC_Log_error(LOG_FAIL_INSERT_REMOTE_SUBSCRIPTION_RECORD);
            retcode = DDS_RETCODE_ERROR;
            goto done;
        }

        if (NDDS_RemoteEntity_is_enabled((NDDS_RemoteEntity *)rem_participant))
        {
            retcode = NDDS_RemoteSubscription_enable(participant,
                    &rem_subscription->as_entity,&rem_participant->data.key);
            if (retcode != DDS_RETCODE_OK)
            {
                retcode = DDS_RETCODE_ERROR;
                goto done;
            }
        }
    }
    else if (dbrc == DB_RETCODE_OK)
    {
        /* Record already exists, check new data */
        /* TODO: If record already exist, check immutable/mutable Qos */
        /*
         * if (DDS_PublicationBuiltinTopicData_copy(&rem_publication->data,
         * data) != DDS_RETCODE_OK) {
         * return DDS_RETCODE_OUT_OF_RESOURCES;
         * }
         */
        if (rem_subscription->as_entity.state == RTIDDS_ENTITY_STATE_ENABLED)
        {
            NDDS_RemoteSubscription_match_local_writer(participant,
                                                       &rem_participant->data,
                                                       &rem_subscription->data);
        }
    }
    else
    {
        DDSC_Log_error(LOG_FAIL_LOOKUP_REMOTE_SUBSCRIPTION_RECORD);
        retcode = DDS_RETCODE_ERROR;
        goto done;
    }

    retcode = DDS_RETCODE_OK;

done:
    DB_Database_unlock(self->database);
    return retcode;
}

DDS_ReturnCode_t
NDDS_RemoteSubscription_enable(DDS_DomainParticipant *const self,
                               NDDS_RemoteEntity *entity,
                               const DDS_BuiltinTopicKey_t *new_key)
{
    struct DDS_DomainParticipantImpl *participant =
                        (struct DDS_DomainParticipantImpl *)self;
    struct DDS_RemoteSubscriptionImpl *rem_subscription = NULL;
    struct DDS_RemoteSubscriptionImpl *rem_subscription2 = NULL;
    struct DDS_RemoteParticipantImpl *remote_participant = NULL;
    DB_ReturnCode_T dbrc;
    DDS_BuiltinTopicKey_t new_sub_key;

    LOG_testPrecondition(entity == NULL || participant == NULL,
                         return DDS_RETCODE_PRECONDITION_NOT_MET);

    rem_subscription = (struct DDS_RemoteSubscriptionImpl *)entity;

    DB_Database_lock(participant->database);

    if (rem_subscription->as_entity.state == RTIDDS_ENTITY_STATE_ENABLED)
    {
        DB_Database_unlock(participant->database);
        return DDS_RETCODE_OK;
    }

    remote_participant = NULL;
    if (new_key == NULL)
    {
        dbrc = DB_Table_select_match(participant->remote_participant_table,
                    DB_TABLE_DEFAULT_INDEX,
                    (DB_Record_T*)&remote_participant,
                    (DB_Key_T)&rem_subscription->data.participant_key);
    }
    else
    {
        dbrc = DB_Table_select_match(participant->remote_participant_table,
                                    DB_TABLE_DEFAULT_INDEX,
                                    (DB_Record_T*)&remote_participant,
                                    (DB_Key_T)new_key);

    }

    if (dbrc != DB_RETCODE_OK)
    {
        DB_Database_unlock(participant->database);
        return DDS_RETCODE_ENTITY_NOT_EXIST;
    }

    if (!NDDS_RemoteEntity_is_enabled(&remote_participant->as_entity))
    {
        DB_Database_unlock(participant->database);
        return DDS_RETCODE_NOT_ENABLED;
    }

    /* We allow changing the host and app id part of the GUID, e.g for static
     * discovery.
     */
    if ((new_key != NULL) &&
        !DDS_BuiltinTopicKey_prefix_equals(new_key,
                                           &DDS_BUILTINTOPICKEY_UNKNOWN))
    {

        /* If the GUID is specified, it matches that of the record, otherwise 
         * it is an inconsistency
         */
        if (!DDS_BuiltinTopicKey_prefix_equals(&remote_participant->data.key,
                                               &DDS_BUILTINTOPICKEY_UNKNOWN) &&
            !DDS_BuiltinTopicKey_equals(&remote_participant->data.key, new_key))
        {
            DB_Database_unlock(participant->database);
            return DDS_RETCODE_PRECONDITION_NOT_MET;
        }

        /* new_key is parent key, can only update the GUID prefix */
        new_sub_key = *new_key;
        DDS_BuiltinTopicKey_copy_suffix(&new_sub_key,
                                        &rem_subscription->data.key);

        /* Do the actual update. We let URTDB do the actual update, becuase
         * we cannot remove and re-insert a record as it could be that someone
         * already has a reference to the record. Normally that should not 
         * happen, but this is a pre-emptive strike against that situation
         */
        dbrc = DB_Table_remove_record(participant->remote_subscriber_table,
                                     (DB_Record_T*)&rem_subscription2,
                                     (DB_Key_T)&rem_subscription->data.key);
        if (dbrc != DB_RETCODE_OK)
        {
            DB_Database_unlock(participant->database);
            return DDS_RETCODE_ENTITY_NOT_EMPTY;
        }

        rem_subscription2->data.participant_key = *new_key;
        rem_subscription2->data.key = new_sub_key;
        dbrc = DB_Table_insert_record(participant->remote_subscriber_table,
                                     (DB_Record_T)rem_subscription2);
        if (dbrc != DB_RETCODE_OK)
        {
            DB_Database_unlock(participant->database);
            return DDS_RETCODE_ENTITY_NOT_EMPTY;
        }
    }

    /*LOG_printDebug("enable remote subscription\n"); */
    DDSC_LOG_DEBUG(LOG_ENABLE_REMOTE_SUBSCRIPTION);
    rem_subscription->as_entity.state = RTIDDS_ENTITY_STATE_ENABLED;

    NDDS_RemoteSubscription_match_local_writer(participant,
                                               &remote_participant->data,
                                               &rem_subscription->data);
    DB_Database_unlock(participant->database);

    return DDS_RETCODE_OK;
}

DDS_ReturnCode_t
NDDS_RemoteSubscription_remove_internal(DDS_DomainParticipant *const self,
                                        const DDS_BuiltinTopicKey_t *key,
                                        DDS_Boolean reset_entry)
{
    struct DDS_DomainParticipantImpl *participant =
                                    (struct DDS_DomainParticipantImpl *)self;
    struct DDS_RemoteSubscriptionImpl *rem_subscription = NULL;
    struct DDS_RemoteSubscriptionImpl *rem_subscription2 = NULL;
    DB_ReturnCode_T dbrc;
    struct DDS_RemoteParticipantImpl *remote_participant = NULL;

    LOG_testPrecondition(participant == NULL || key == NULL,
                         return DDS_RETCODE_PRECONDITION_NOT_MET);

    DDSC_LOG_DEBUG(LOG_REMOVE_REMOTE_SUBSCRIPTION);

    DB_Database_lock(participant->database);

    if (reset_entry)
    {
        dbrc = DB_Table_select_match(participant->remote_subscriber_table,
                                    DB_TABLE_DEFAULT_INDEX,
                                    (DB_Record_T*)&rem_subscription,
                                    (DB_Key_T)key);
    }
    else
    {
        dbrc = DB_Table_remove_record(participant->remote_subscriber_table,
                                    (DB_Record_T*)&rem_subscription,
                                     (DB_Key_T)key);
    }

    if (dbrc != DB_RETCODE_OK)
    {
        DB_Database_unlock(participant->database);
        return DDS_RETCODE_ENTITY_NOT_EXIST;
    }

    remote_participant = NULL;
    dbrc = DB_Table_select_match(participant->remote_participant_table,
                    DB_TABLE_DEFAULT_INDEX,
                    (DB_Record_T*)&remote_participant,
                    (DB_Key_T)&rem_subscription->data.participant_key);
    if (dbrc != DDS_RETCODE_OK)
    {
        DB_Database_unlock(participant->database);
        return DDS_RETCODE_ENTITY_NOT_EXIST;
    }

    NDDS_RemoteSubscription_unmatch_local_writer(participant,
                            &remote_participant->data,
                            &rem_subscription->data);

    if (reset_entry)
    {
        rem_subscription->as_entity.state = RTIDDS_ENTITY_STATE_CREATED;
        if (!DDS_BuiltinTopicKey_prefix_equals(&rem_subscription->data.key,
                &DDS_BUILTINTOPICKEY_PREFIX_UNKNOWN))
        {
            dbrc = DB_Table_remove_record(participant->remote_subscriber_table,
                    (DB_Record_T*)&rem_subscription2,
                    (DB_Key_T)&rem_subscription->data.key);
            if (dbrc != DB_RETCODE_OK)
            {
                DB_Database_unlock(participant->database);
                return DDS_RETCODE_ENTITY_NOT_EMPTY;
            }
            rem_subscription2->data.key = rem_subscription->orig_key;
            rem_subscription2->data.participant_key = remote_participant->orig_key;
            dbrc = DB_Table_insert_record(participant->remote_subscriber_table,
                    (DB_Record_T)rem_subscription2);
            if (dbrc != DB_RETCODE_OK)
            {
                DB_Database_unlock(participant->database);
                return DDS_RETCODE_ENTITY_NOT_EMPTY;
            }
        }
    }
    else
    {
        if (!DDS_SubscriptionBuiltinTopicData_finalize(&rem_subscription->data))
        {
            DB_Database_unlock(participant->database);
            return DDS_RETCODE_PRECONDITION_NOT_MET;
        }
        dbrc = DB_Table_delete_record(participant->remote_subscriber_table,
                                     (DB_Record_T)rem_subscription);
        if (dbrc != DB_RETCODE_OK)
        {
            DB_Database_unlock(participant->database);
            return DDS_RETCODE_ENTITY_DELETE_ERROR;
        }
    }

    DB_Database_unlock(participant->database);
    return DDS_RETCODE_OK;
}

DDS_ReturnCode_t
NDDS_DomainParticipant_remove_remote_subscription(
                                    DDS_DomainParticipant *const participant,
                                    const DDS_BuiltinTopicKey_t *key)
{
    return NDDS_RemoteSubscription_remove_internal(participant, key,
                                                   DDS_BOOLEAN_FALSE);
}
