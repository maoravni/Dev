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
#include "RemoteParticipant.pkg.h"
#include "DomainParticipantQos.pkg.h"
#include "DomainParticipant.pkg.h"
#include "RemotePublication.pkg.h"
#include "RemoteSubscription.pkg.h"

/*** SOURCE_BEGIN ***/

struct DDS_RemoteParticipantImpl*
NDDS_RemoteParticipant_lookup_name(DDS_DomainParticipant * const self,
                                   const char *name)
{
    DB_ReturnCode_T dbrc;
    DB_Cursor_T cursor;
    struct DDS_RemoteParticipantImpl *db_record;
    struct DDS_DomainParticipantImpl *participant = (struct DDS_DomainParticipantImpl*)self;

    dbrc = DB_Table_select_all(participant->remote_participant_table,
                              DB_TABLE_DEFAULT_INDEX,
                              &cursor);
    do
    {
        dbrc = DB_Cursor_get_next(cursor, (DB_Record_T*)&db_record);
        if (dbrc == DB_RETCODE_OK &&
            !DDS_String_cmp(name, db_record->data.participant_name.name))
        {
            break;
        }
        db_record = NULL;
    } while (dbrc == DB_RETCODE_OK);
    DB_Cursor_finish(participant->remote_participant_table,cursor);

    return db_record;
}

struct DDS_RemoteParticipantImpl*
NDDS_RemoteEndpoint_find_parent(DDS_DomainParticipant *self,
                   const DDS_BuiltinTopicKey_t *participant_key,
                   const char *participant_name,
                   const DDS_BuiltinTopicKey_t *endpoint_key)
{
    DB_ReturnCode_T db_retcode;
    struct DDS_RemoteParticipantImpl *result = NULL;
    struct DDS_RemoteParticipantImpl *result2 = NULL;
    struct DDS_DomainParticipantImpl *participant = (struct DDS_DomainParticipantImpl*)self;
    /*
     * A remote endpoint must have at least the object id specified, but
     * can be asserted with an unknown prefix as in the case of static
     * discovery. If the prefix is unknown, we rely on the participant GUID/
     * name to look it up. This logic is fairly complex and designed to catch
     * as many errors as possibly related to discovery, in particular
     * static discovery.
     *
     * Terms:
     * endpoint, endpoint_prefix
     * participant_name, participant_name_prefix
     * participant_key, participant_key_prefix
     *
     * If an endpoint_key is UNKNOWN:
     * --> Return => Error
     *
     * If an endpoint_prefix is UNKNOWN:
     *
     * --> If the participant_key is UNKNOWN:
     *   --> If the participant_name == NULL => Error
     *   --> Lookup participant:
     *     --> If participant not found => Error
     *   --> OK: Link endpoint to participant by using the internal GUID of
     *           participant_name
     *
     * --> If the participant_key_prefix is UNKNOWN || participant_key is KNOWN
     *   --> Lookup participant_key
     *   --> If participant not found => Error
     *   --> If participant_name is not NULL
     *     --> Lookup participant_name
     *       --> If participant not found => Error
     *       --> if participant_name_key != participant_key => Error
     *   --> OK: Link endpoint to participant by using the internal GUID of
     *           participant_name
     *
     * If an endpoint_prefix is KNOWN:
     * --> If the participant_key prefix does not match endpoint_prefix => Error
     * --> Lookup participant_key
     *   --> If participant does not exist => Error
     * --> If the participant_name is NULL
     *   --> OK: Link endpoint to participant by using the GUID of
     *           participant_key
     * --> If participant_name does not exist => Error
     * --> If participant_name prefix != participant_key prefix => Error
     * --> OK: Link endpoint to participant by using the GUID of
     *         participant_key
     */

    if (DDS_BuiltinTopicKey_prefix_equals(endpoint_key,
                                          &DDS_BUILTINTOPICKEY_UNKNOWN) &&
        DDS_BuiltinTopicKey_equals(participant_key,
                                   &DDS_BUILTINTOPICKEY_UNKNOWN))
    {

        if (participant_name == NULL)
        {
            return NULL;
        }

        result = NDDS_RemoteParticipant_lookup_name(participant,
                                                    participant_name);
        if (result == NULL)
        {
            DDSC_Log_error(LOG_UNKNOWN_REMOTE_PARTICIPANT);
            return NULL;
        }
    }
    else if ((DDS_BuiltinTopicKey_prefix_equals(endpoint_key,
                                                &DDS_BUILTINTOPICKEY_UNKNOWN) &&
              !DDS_BuiltinTopicKey_suffix_equals(participant_key,
                                                 &DDS_BUILTINTOPICKEY_UNKNOWN))
             || DDS_BuiltinTopicKey_prefix_equals(endpoint_key,
                                                  participant_key))
    {
        result = NULL;
        db_retcode = DB_Table_select_match(participant->remote_participant_table,
                                          DB_TABLE_DEFAULT_INDEX,
                                          (DB_Record_T*)&result,
                                          (DB_Key_T) participant_key);
        if (db_retcode != DB_RETCODE_OK)
        {
            DDSC_Log_error_p4(LOG_UNKNOWN_REMOTE_PARTICIPANT_BY_KEY,
                               participant_key->value[0],
                               participant_key->value[1],
                               participant_key->value[2],
                               participant_key->value[3]);
            return NULL;
        }

        if (participant_name != NULL)
        {
            result2 = NDDS_RemoteParticipant_lookup_name(participant,
                                                        participant_name);
            if (result2 == NULL)
            {
                DDSC_Log_error(LOG_UNKNOWN_REMOTE_PARTICIPANT);
                return NULL;
            }
            if (!DDS_BuiltinTopicKey_equals(&result->data.key,
                                            (DDS_BuiltinTopicKey_t *) &
                                            result2->data.key))
            {
                DDSC_Log_error(LOG_REM_PARTICIPANT_KEY_NOTEQUALS_RECORD_KEY);
                return NULL;
            }
        }
    }
    else if (!DDS_BuiltinTopicKey_prefix_equals
             (endpoint_key, &DDS_BUILTINTOPICKEY_UNKNOWN)
             && !DDS_BuiltinTopicKey_prefix_equals(endpoint_key,
                                                   (DDS_BuiltinTopicKey_t *)
                                                   participant_key))
    {
        DDSC_Log_error(LOG_ENDPOINT_NOT_CHILD_OF_PARTICIPANT);
        return NULL;
    }
    else
    {
        DDSC_Log_error(LOG_INVALID_ENDPOINT_GUID);
        return NULL;
    }

    return result;
}

