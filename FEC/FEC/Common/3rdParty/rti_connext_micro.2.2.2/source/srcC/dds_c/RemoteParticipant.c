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
#include "RemotePublication.pkg.h"
#include "RemoteSubscription.pkg.h"

const char *DDS_PARTICIPANT_BUILTIN_TOPIC_TYPE_NAME = "DDS_ParticipantBuiltinTopicData";
const char *DDS_PARTICIPANT_BUILTIN_TOPIC_NAME = "DCPSParticipant";

/*******************************************************************************
 *
 *                                 Internal API
 *
 ******************************************************************************/
struct DDS_RemoteParticipantImpl
{
    struct NDDS_RemoteEntityImpl as_entity;
    struct DDS_ParticipantBuiltinTopicData data;
    OSAPITimeoutHandle_t lease_duration_event;
    OSAPI_Timer_t timer;
    struct OSAPI_NtpTime lease_duration;
    DDS_BuiltinTopicKey_t orig_key;
};

RTI_PRIVATE DDS_ReturnCode_t
NDDS_RemoteParticipant_enable(DDS_DomainParticipant *const participant,
                              NDDS_RemoteEntity *entity,
                              const DDS_BuiltinTopicKey_t *new_key);

/*** SOURCE_BEGIN ***/

RTI_INT32
DDS_RemoteParticipantImpl_compare(DB_Select_T opcode,RTI_INT32 flags,
                           const DB_Record_T op1, void *op2, void *op3)
{
    struct DDS_RemoteParticipantImpl *record_left =
                                        (struct DDS_RemoteParticipantImpl*)op1;
    const DDS_BuiltinTopicKey_t *key_right;

    if (DB_SELECT_OP2_IS_KEY(flags))
    {
        key_right = (const DDS_BuiltinTopicKey_t*)op2;
    }
    else
    {
        key_right = &((struct DDS_RemoteParticipantImpl*)op2)->data.key;
    }

    return DDS_BuiltinTopicKey_compare(&record_left->data.key,key_right);
}

struct DDS_RemoteParticipantImpl*
NDDS_DomainParticipant_lookup_name(DDS_DomainParticipant *const participant,
                                   const char *name)
{
    struct DDS_DomainParticipantImpl *self =
                        (struct DDS_DomainParticipantImpl *)participant;
    DB_ReturnCode_T dbrc;
    DB_Cursor_T cursor = NULL;
    struct DDS_RemoteParticipantImpl *rem_participant = NULL;

    dbrc = DB_Table_select_all(self->remote_participant_table,
                                    DB_TABLE_DEFAULT_INDEX,&cursor);
    do
    {
        dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&rem_participant);
        if (dbrc == DB_RETCODE_OK)
        {
            if (!DDS_String_cmp(name,
                            rem_participant->data.participant_name.name))
            {
                break;
            }
            rem_participant = NULL;
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(self->remote_participant_table,cursor);

    return rem_participant;
}

RTI_PRIVATE OSAPITimeoutOp_t
NDDS_RemoteParticipant_on_liveliness(struct OSAPITimeoutUserData *storage)
{
    struct DDS_RemoteParticipantImpl *remote_participant;
    DDS_DomainParticipant *participant;

    remote_participant = (struct DDS_RemoteParticipantImpl *)storage->field[0];
    participant = (DDS_DomainParticipant *)storage->field[1];

    NDDS_Discovery_Plugin_on_remote_participant_liveliness_expired(
            participant->disc_plugin,
            participant,
            &remote_participant->data);

    return OSAPI_TIMEOUT_OP_AUTOMATIC;
}

/*******************************************************************************
 *
 *                                 Public API
 *
 ******************************************************************************/
DDS_ReturnCode_t
NDDS_DomainParticipant_assert_remote_participant(
        DDS_DomainParticipant *const self,
        struct DDS_ParticipantBuiltinTopicData *const data,
        DDS_Boolean *const is_new)
{
    struct DDS_DomainParticipantImpl *participant =
                                    (struct DDS_DomainParticipantImpl *)self;
    struct DDS_RemoteParticipantImpl *record = NULL;
    struct DDS_RemoteParticipantImpl *record_test = NULL;
    DB_ReturnCode_T dbrc = DB_RETCODE_NOT_EXISTS;
    DDS_Boolean assign_guid = DDS_BOOLEAN_FALSE;

    LOG_testPrecondition(participant == NULL || data == NULL,
                         return DDS_RETCODE_PRECONDITION_NOT_MET);

    *is_new = DDS_BOOLEAN_TRUE;

    /* The algorithm here is somewhat complicated.
     *
     * If the GUID is unknown (i.e not set in data, e.g, DPSE), then the 
     * remote_participant must have a name. However, if the GUID is known
     * (either static or dynamic discovery) then the participant does not
     * need to have a name. 
     *
     * After a participant has been enabled, the GUID will be updated to the 
     * real GUID. However, it could be that the QoS is changed even for static
     * discovery and we need to look up an existing participant. In that case
     * we will still use the name because the user does not know what the 
     * real GUID is. Thus, the algorithm to lookup a participant is this:
     * 
     * If the GUID passed in is AUTO/UNKNOWN we use the name. If the name
     * is empty an error is returned.
     *
     * If the GUID passed in is set, the name is ignored in the lookup
     */

    /* If the GUID is unknown, lookup the participant based on the name */

    DB_Database_lock(participant->database);

    if (DDS_BuiltinTopicKey_equals(&data->key, &DDS_BUILTINTOPICKEY_UNKNOWN))
    {
        if (data->participant_name.name[0] == 0)
        {
            DB_Database_unlock(participant->database);
            return DDS_RETCODE_PRECONDITION_NOT_MET;
        }
        record = NDDS_DomainParticipant_lookup_name(self,
                                                 data->participant_name.name);
        if (record == NULL)
        {
            assign_guid = DDS_BOOLEAN_TRUE;
        }
    }
    else
    {
        /* If the GUID is known, lookup the participant based on the GUID */
        record = NULL;
        dbrc = DB_Table_select_match(participant->remote_participant_table,
                    DB_TABLE_DEFAULT_INDEX,(DB_Record_T*)&record,
                    (DB_Key_T)&data->key);

        if ((dbrc == DB_RETCODE_OK) && (data->participant_name.name[0] != 0))
        {
            record_test = NDDS_DomainParticipant_lookup_name(participant,
                                                data->participant_name.name);
            if (record_test == NULL)
            {
                DB_Database_unlock(participant->database);
                return DDS_RETCODE_ENTITY_NOT_EXIST;
            }
            if (!DDS_BuiltinTopicKey_equals(&record->data.key,
                                            &record_test->data.key))
            {
                DB_Database_unlock(participant->database);
                return DDS_RETCODE_PRECONDITION_NOT_MET;
            }
        }
    }

    /* If the record already exists update it, otherwise create a new record */
    if ((record == NULL) && (dbrc == DB_RETCODE_NOT_EXISTS))
    {
        dbrc = DB_Table_create_record(participant->remote_participant_table,
                                     (DB_Record_T*)&record);
        if (dbrc != DB_RETCODE_OK)
        {
            DB_Database_unlock(participant->database);
            return DDS_RETCODE_OUT_OF_RESOURCES;
        }

        /* if all is ok, initialize the record */
        if (!DDS_ParticipantBuiltinTopicData_initialize(&record->data))
        {
            DB_Database_unlock(participant->database);
            return DDS_RETCODE_OUT_OF_RESOURCES;
        }

        /* if all is ok, update the record */
        if (!DDS_ParticipantBuiltinTopicData_copy(&record->data,data))
        {
            DB_Database_unlock(participant->database);
            return DDS_RETCODE_OUT_OF_RESOURCES;
        }

        if (assign_guid)
        {
            /* Assign an internal GUID so that remote endpoints can 
             * be asserted without having to store the name of the parent 
             * participant (that is, when the real GUID is known we can 
             * look up the internal GUID instead of having to look up the name
             */
            record->data.key.value[DDS_BUILTIN_TOPIC_KEY_TYPE_NATIVE_LENGTH-1]
                                           = OSAPI_System_get_next_object_id();
        }
        record->as_entity.enable_func = NDDS_RemoteParticipant_enable;
        record->as_entity.kind = DDS_PARTICIPANT_ENTITY_KIND;
        record->as_entity.state = RTIDDS_ENTITY_STATE_CREATED;
        record->timer = DDS_DomainParticipant_get_timer(participant);
        record->orig_key = record->data.key;

        /* Insert into the table */
        dbrc = DB_Table_insert_record(participant->remote_participant_table,
                                     (DB_Record_T)record);
        if (dbrc != DB_RETCODE_OK)
        {
            DB_Database_unlock(participant->database);
            return DDS_RETCODE_ERROR;
        }
    }
    else if (dbrc != DB_RETCODE_OK)
    {
        DB_Database_unlock(participant->database);
        return DDS_RETCODE_ERROR;
    }
    else
    {
        /* The record exists, there are two case: 
         * The record state is CREATED: Update the Qos if possible
         * The record state is ENABLED: Update the Qos if possible
         *
         * If the new Qos is the same as the old Qos, then do we
         * need to do anything?
         * 
         * TODO: Check if liveliness needs to be ensured from here
         * TODO: Do we need to check the data for consitency, or do we
         * assume that it is? Would like to avoid to sets of functions doing 
         * alsmost the same thing (BuiltinData vs Qos)
         */
        *is_new = DDS_BOOLEAN_FALSE;
        if (!DDS_ParticipantBuiltinTopicData_copy(&record->data,data))
        {
            DB_Database_unlock(participant->database);
            return DDS_RETCODE_OUT_OF_RESOURCES;
        }
    }

    DB_Database_unlock(participant->database);
    return DDS_RETCODE_OK;
}

DDS_ReturnCode_t
NDDS_DomainParticipant_refresh_remote_participant_liveliness(
        DDS_DomainParticipant *const self,
        const struct DDS_BuiltinTopicKey_t *const key)
{
    struct DDS_DomainParticipantImpl *participant =
                                    (struct DDS_DomainParticipantImpl *)self;
    struct DDS_RemoteParticipantImpl *remote_participant = NULL;
    DB_ReturnCode_T dbrc;

    LOG_testPrecondition(participant == NULL || key == NULL,
                         return DDS_RETCODE_PRECONDITION_NOT_MET);

    DB_Database_lock(participant->database);

    remote_participant = NULL;
    dbrc = DB_Table_select_match(participant->remote_participant_table,
                                DB_TABLE_DEFAULT_INDEX,
                                (DB_Record_T*)&remote_participant,
                                (DB_Key_T) key);
    if (dbrc != DB_RETCODE_OK)
    {
        DB_Database_unlock(participant->database);
        return DDS_RETCODE_ENTITY_NOT_EXIST;
    }

    if (DDS_Duration_is_infinite
        (&remote_participant->data.liveliness_lease_duration))
    {
        DB_Database_unlock(participant->database);
        return DDS_RETCODE_PRECONDITION_NOT_MET;
    }

    if (!OSAPI_Timer_update_timeout(remote_participant->timer,
                                   &remote_participant->lease_duration_event,
                                   remote_participant->data.
                                   liveliness_lease_duration.sec * 1000 +
                                   remote_participant->data.
                                   liveliness_lease_duration.nanosec / 1000000))
    {
        /*LOG_printError("Failed to update liveliness for remote participant\n"); */
        DDSC_Log_error(LOG_FAIL_UPDATE_LIVELINESS_REMOTE_PARTICIPANT);
    }

    DB_Database_unlock(participant->database);
    return DDS_RETCODE_OK;
}

RTI_PRIVATE DDS_ReturnCode_t
NDDS_RemoteParticipant_enable(DDS_DomainParticipant *const self,
                              NDDS_RemoteEntity *entity,
                              const DDS_BuiltinTopicKey_t *new_key)
{
    DB_ReturnCode_T dbrc;
    struct DDS_DomainParticipantImpl *participant =
                                    (struct DDS_DomainParticipantImpl *)self;
    struct DDS_RemoteParticipantImpl *remote_participant = NULL;
    struct OSAPITimeoutUserData storage;
    DB_Cursor_T cursor = NULL;
    struct DDS_RemotePublicationImpl *remote_pub;
    struct DDS_RemoteSubscriptionImpl *remote_sub;
    struct DDS_RemoteParticipantImpl *remote_participant2 = NULL;
    DDS_BuiltinTopicKey_t low_key = DDS_BuiltinTopicKey_t_INITIALIZER;
    DDS_BuiltinTopicKey_t high_key = DDS_BuiltinTopicKey_t_INITIALIZER;

    LOG_testPrecondition(entity == NULL,
                         return DDS_RETCODE_PRECONDITION_NOT_MET);

    DB_Database_lock(participant->database);

    if (entity->state == RTIDDS_ENTITY_STATE_ENABLED)
    {
        DB_Database_unlock(participant->database);
        return DDS_RETCODE_OK;
    }

    DDSC_LOG_DEBUG(LOG_ENABLE_REMOTE_PARTICIPANT);

    /* TODO: Check if the new GUID already exists for another DP, then it
     *       is an error.
     */
    remote_participant = (struct DDS_RemoteParticipantImpl*)entity;

    /* We allow changing the GUID, e.g for static discovery */
    if ((new_key != NULL) &&
        DDS_BuiltinTopicKey_prefix_equals(&remote_participant->data.key,
                                          &DDS_BUILTINTOPICKEY_PREFIX_UNKNOWN))
    {
        /* TODO: How to do this? */
        dbrc = DB_Table_remove_record(participant->remote_participant_table,
                (DB_Record_T*)&remote_participant2,
                (DB_Key_T)&remote_participant->data.key);
        if (dbrc != DB_RETCODE_OK)
        {
            DB_Database_unlock(participant->database);
            return DDS_RETCODE_PRECONDITION_NOT_MET;
        }
        remote_participant->data.key = *new_key;
        dbrc = DB_Table_insert_record(participant->remote_participant_table,
                                     (DB_Record_T)remote_participant);
        if (dbrc != DB_RETCODE_OK)
        {
            DB_Database_unlock(participant->database);
            return DDS_RETCODE_PRECONDITION_NOT_MET;
        }
    }

    remote_participant->as_entity.state = RTIDDS_ENTITY_STATE_ENABLED;
    low_key = remote_participant->orig_key;
    low_key.value[3]=0;
    high_key = remote_participant->orig_key;
    high_key.value[3]=0xffffffff;

    /* Publisher */
    if (new_key)
    {
        dbrc = DB_Table_select_range(participant->remote_publisher_table,
                                    DB_TABLE_DEFAULT_INDEX,
                                    &cursor,
                                    &low_key,&high_key);
    }
    else
    {
        dbrc = DB_Table_select_all(participant->remote_publisher_table,
                                  DB_TABLE_DEFAULT_INDEX,
                                  &cursor);
    }

    do
    {
        dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&remote_pub);
        if (dbrc == DB_RETCODE_OK)
        {
            if (DDS_BuiltinTopicKey_equals(&remote_participant->orig_key,
                                           &remote_pub->data.participant_key))
            {
                remote_pub->as_entity.enable_func(participant,
                        &remote_pub->as_entity, new_key);
            }
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(participant->remote_publisher_table,cursor);

    /* Subscriber */
    cursor = NULL;

    if (new_key)
    {
        dbrc = DB_Table_select_range(participant->remote_subscriber_table,
                                    DB_TABLE_DEFAULT_INDEX,
                                    &cursor,
                                    &low_key,&high_key);
    }
    else
    {
        dbrc = DB_Table_select_all(participant->remote_subscriber_table,
                                  DB_TABLE_DEFAULT_INDEX,
                                  &cursor);
    }

    do
    {
        dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*)&remote_sub);
        if (dbrc == DB_RETCODE_OK)
        {
            if (DDS_BuiltinTopicKey_equals(&remote_participant->orig_key,
                                           &remote_sub->data.participant_key))
            {
                remote_sub->as_entity.enable_func(participant,
                                          &remote_sub->as_entity, new_key);
            }
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(participant->remote_subscriber_table,cursor);

    if (!DDS_Duration_is_infinite
        (&remote_participant->data.liveliness_lease_duration))
    {
        storage.field[0] = (void *)remote_participant;
        storage.field[1] = (void *)participant;

        if (!OSAPI_Timer_create_timeout(remote_participant->timer,
                &remote_participant->lease_duration_event,
                DDS_Duration_to_ms(remote_participant->data.liveliness_lease_duration),
                OSAPI_TIMER_ONE_SHOT,
                NDDS_RemoteParticipant_on_liveliness,
                &storage))
        {
            DDSC_Log_error(LOG_FAIL_POST_DEADLINE_REMOTE_PARTICIPANT);
            DB_Database_unlock(participant->database);
            remote_participant->as_entity.state = RTIDDS_ENTITY_STATE_CREATED;
            return DDS_RETCODE_OUT_OF_RESOURCES;
        }
    }

    DB_Database_unlock(participant->database);
    return DDS_RETCODE_OK;
}

DDS_ReturnCode_t
NDDS_DomainParticipant_enable_remote_participant_name(
        DDS_DomainParticipant *const self,
        const struct DDS_ParticipantBuiltinTopicData *const data)
{
    struct DDS_DomainParticipantImpl *participant =
                                    (struct DDS_DomainParticipantImpl *)self;
    DDS_BuiltinTopicKey_t old_key;
    DDS_ReturnCode_t retcode;
    struct DDS_RemoteParticipantImpl *remote_participant = NULL;

    LOG_testPrecondition(participant == NULL || data == NULL,
                         return DDS_RETCODE_PRECONDITION_NOT_MET);

    DB_Database_lock(participant->database);

    remote_participant = NDDS_DomainParticipant_lookup_name(participant,
                            data->participant_name.name);

    if (remote_participant == NULL)
    {
        DB_Database_unlock(participant->database);
        return DDS_RETCODE_ENTITY_NOT_EXIST;
    }

    /* If the GUID is specified, it matches that of the record, otherwise 
     * it is an inconsistency
     */
    if (!DDS_BuiltinTopicKey_prefix_equals(&remote_participant->data.key,
                                           &DDS_BUILTINTOPICKEY_PREFIX_UNKNOWN)
        && !DDS_BuiltinTopicKey_equals(&remote_participant->data.key,
                                       &data->key))
    {
        DB_Database_unlock(participant->database);
        return DDS_RETCODE_PRECONDITION_NOT_MET;
    }

    /* Ensure that the GUID is not unknown (or internal) when it is enabled */
    if (DDS_BuiltinTopicKey_prefix_equals(&remote_participant->data.key,
                                          &DDS_BUILTINTOPICKEY_UNKNOWN) &&
        (DDS_BuiltinTopicKey_prefix_equals(&data->key,
                                           &DDS_BUILTINTOPICKEY_UNKNOWN) ||
         DDS_BuiltinTopicKey_suffix_equals(&data->key,
                                           &DDS_BUILTINTOPICKEY_UNKNOWN)))
    {
        DB_Database_unlock(participant->database);
        return DDS_RETCODE_PRECONDITION_NOT_MET;
    }

    /* TODO: Do we need to sanity check the input here or should that 
     * be done outside of this function?
     * NOTE: Do not copy the new key quite yet! This can only be done after
     *       it has been enabled
     */
    old_key = remote_participant->data.key;
    if (!DDS_ParticipantBuiltinTopicData_copy(&remote_participant->data,data))
    {
        DB_Database_unlock(participant->database);
        return DDS_RETCODE_OUT_OF_RESOURCES;
    }

    remote_participant->data.key = old_key;
    retcode = remote_participant->as_entity.enable_func(participant,
                                    (NDDS_RemoteEntity *)remote_participant,
                                     &data->key);

    DB_Database_unlock(participant->database);

    return retcode;
}

DDS_ReturnCode_t
NDDS_DomainParticipant_enable_remote_participant_guid(
                DDS_DomainParticipant *const self,
                const struct DDS_ParticipantBuiltinTopicData *const data)
{
    struct DDS_DomainParticipantImpl *participant =
                                    (struct DDS_DomainParticipantImpl *)self;
    struct DDS_RemoteParticipantImpl *record = NULL;
    DB_ReturnCode_T dbrc;
    DDS_ReturnCode_t retcode;

    LOG_testPrecondition(participant == NULL || data == NULL,
                         return DDS_RETCODE_PRECONDITION_NOT_MET);

    DB_Database_lock(participant->database);

    record = NULL;
    dbrc = DB_Table_select_match(participant->remote_participant_table,
                                DB_TABLE_DEFAULT_INDEX,
                                (DB_Record_T*)&record,
                                (DB_Key_T)&data->key);
    if (dbrc != DB_RETCODE_OK)
    {
        DB_Database_unlock(participant->database);
        return DDS_RETCODE_ENTITY_NOT_EXIST;
    }

    /* TODO: Do we need to sanity check the input here or should that 
     * be done outside of this function?
     */
    if (!DDS_ParticipantBuiltinTopicData_copy(&record->data,data))
    {
        DB_Database_unlock(participant->database);
        return DDS_RETCODE_OUT_OF_RESOURCES;
    }

    /* Assuming data is ok */
    retcode = record->as_entity.enable_func(
                             participant, (NDDS_RemoteEntity *) record,NULL);

    DB_Database_unlock(participant->database);

    return retcode;
}

RTI_BOOL
NDDS_RemoteParticipantRecord_finalize(const void *record, void *param)
{
    struct DDS_RemoteParticipantImpl *rem_part =
        (struct DDS_RemoteParticipantImpl *)record;

    DDS_ParticipantBuiltinTopicData_finalize(&rem_part->data);

    return RTI_TRUE;
}

RTI_PRIVATE DDS_ReturnCode_t
NDDS_RemoteParticipant_remove_internal(DDS_DomainParticipant *const self,
                                       const DDS_BuiltinTopicKey_t *const key,
                                       DDS_Boolean reset_entry)
{
    struct DDS_DomainParticipantImpl *participant =
                                    (struct DDS_DomainParticipantImpl *)self;
    struct DDS_RemoteParticipantImpl *remote_participant = NULL;
    DB_ReturnCode_T dbrc;
    struct DDS_RemotePublicationImpl *rem_publication;
    struct DDS_RemoteSubscriptionImpl *rem_subscription;
    DB_Cursor_T cursor = NULL;
    DDS_BuiltinTopicKey_t low_key = DDS_BuiltinTopicKey_t_INITIALIZER;
    DDS_BuiltinTopicKey_t high_key = DDS_BuiltinTopicKey_t_INITIALIZER;

    LOG_testPrecondition(participant == NULL || key == NULL,
                         return DDS_RETCODE_PRECONDITION_NOT_MET);

    DB_Database_lock(participant->database);

    dbrc = DB_Table_select_match(participant->remote_participant_table,
                                DB_TABLE_DEFAULT_INDEX,
                                (DB_Record_T*)&remote_participant,
                                (DB_Key_T)key);
    if (dbrc != DB_RETCODE_OK)
    {
        DB_Database_unlock(participant->database);
        return DDS_RETCODE_ENTITY_NOT_EXIST;
    }

    low_key = *key;
    high_key = *key;
    low_key.value[3] = 0;
    high_key.value[3] = 0xffffffff;

    cursor = NULL;
    dbrc = DB_Table_select_range(participant->remote_publisher_table,
                                  DB_TABLE_DEFAULT_INDEX,&cursor,
                                  &low_key,&high_key);
    do
    {
        dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*) &rem_publication);
        if (dbrc == DB_RETCODE_OK)
        {
            if (DDS_BuiltinTopicKey_equals(&remote_participant->data.key,
                    &rem_publication->data.participant_key))
            {
                NDDS_RemotePublication_remove_internal(participant,
                                    &rem_publication->data.key,reset_entry);
            }
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(participant->remote_publisher_table,cursor);

    cursor = NULL;
    dbrc = DB_Table_select_range(participant->remote_subscriber_table,
                                DB_TABLE_DEFAULT_INDEX,&cursor,
                                &low_key,&high_key);
    do
    {
        dbrc = DB_Cursor_get_next(cursor,(DB_Record_T*) &rem_subscription);
        if (dbrc == DB_RETCODE_OK)
        {
            if (DDS_BuiltinTopicKey_equals(&remote_participant->data.key,
                    &rem_subscription->data.participant_key))
            {
                NDDS_RemoteSubscription_remove_internal(participant,
                                    &rem_subscription->data.key,reset_entry);
            }
        }
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(participant->remote_subscriber_table,cursor);

    remote_participant = NULL;
    dbrc = DB_Table_remove_record(participant->remote_participant_table,
                                (DB_Record_T*)&remote_participant,
                                (DB_Key_T)key);
    if (dbrc != DB_RETCODE_OK)
    {
        DB_Database_unlock(participant->database);
        return DDS_RETCODE_ENTITY_NOT_EXIST;
    }

    if ((remote_participant->lease_duration_event.epoch > -1) &&
        !OSAPI_Timer_delete_timeout(remote_participant->timer,
                                   &remote_participant->lease_duration_event))
    {
        DB_Database_unlock(participant->database);
        return DDS_RETCODE_ENTITY_DELETE_ERROR;
    }

    if (reset_entry)
    {
        remote_participant->as_entity.state = RTIDDS_ENTITY_STATE_CREATED;
        remote_participant->data.key = remote_participant->orig_key;
        dbrc = DB_Table_insert_record(participant->remote_participant_table,
                                     (DB_Record_T)remote_participant);
        if (dbrc != DB_RETCODE_OK)
        {
            DB_Database_unlock(participant->database);
            return DDS_RETCODE_PRECONDITION_NOT_MET;
        }
    }
    else
    {
        if (!DDS_ParticipantBuiltinTopicData_finalize(&remote_participant->data))
        {
            DB_Database_unlock(participant->database);
            return DDS_RETCODE_PRECONDITION_NOT_MET;
        }
        dbrc = DB_Table_delete_record(participant->remote_participant_table,
                                     remote_participant);
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
NDDS_DomainParticipant_remove_remote_participant(
                                DDS_DomainParticipant *const participant,
                                const DDS_BuiltinTopicKey_t *const key)
{
    return NDDS_RemoteParticipant_remove_internal(participant,
                                                  key, DDS_BOOLEAN_FALSE);
}

DDS_ReturnCode_t
NDDS_DomainParticipant_reset_remote_participant(
                                DDS_DomainParticipant *const participant,
                                const struct DDS_BuiltinTopicKey_t *const key)
{
    return NDDS_RemoteParticipant_remove_internal(participant,
                                                  key, DDS_BOOLEAN_TRUE);
}
