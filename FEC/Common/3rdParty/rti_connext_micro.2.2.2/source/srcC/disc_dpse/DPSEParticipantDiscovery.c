/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/
#ifndef osapi_time_h
#include "osapi/osapi_time.h"
#endif
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif
#ifndef dds_c_discovery_plugin_h
#include "dds_c/dds_c_discovery_plugin.h"
#endif
#ifndef disc_dpse_dpsediscovery_h
#include "disc_dpse/disc_dpse_dpsediscovery.h"
#endif
#ifndef disc_dpse_log_h
#include "disc_dpse/disc_dpse_log.h"
#endif

#include "DPSECdr.pkg.h"
#include "DPSEParticipantBuiltinTopicData.pkg.h"
#include "DPSEParticipantListener.pkg.h"
#include "DPSEDiscoveryPlugin.pkg.h"

/*** SOURCE_BEGIN ***/

/* Callback called when it is time to assert the local participant. */
OSAPITimeoutOp_t
DPSE_ParticipantDiscovery_assert_participant(
                                        struct OSAPITimeoutUserData *storage)
{
    struct DPSE_DiscoveryPlugin *dpse_plugin =
                            (struct DPSE_DiscoveryPlugin *)storage->field[0];
    void *local_participant_data = storage->field[1];

    DPSE_LOG_DEBUG(LOG_ANNOUNCE_SELF_TO_REMOTE_PARTICIPANT);

    /* Send out participant announcement */
    if (DDS_RETCODE_OK != DDS_DataWriter_write(dpse_plugin->participant_writer,
                                               local_participant_data,
                                               &DDS_HANDLE_NIL))
    {
        DPSE_Log_error(LOG_FAIL_ANNOUNCE_SELF_TO_REMOTE_PARTICIPANT);
    }

    /* If we are sending the initial three, we use the initial announcement
     * period to reschedule the event, otherwise use the regular announcement
     * interval
     */
    if (storage->count[0] > 0)
    {
        storage->count[0]--;
        return OSAPI_TIMEOUT_OP_AUTOMATIC;
    }


    if (!OSAPI_Timer_update_timeout(
                DDS_DomainParticipant_get_timer(DDS_Subscriber_get_participant
                  (dpse_plugin->participant_subscriber)),
                  &dpse_plugin->announcement_event,
                  DDS_Duration_to_ms(dpse_plugin->
                              properties.participant_liveliness_assert_period)))
    {
        DPSE_Log_error(LOG_FAIL_UPDATE_PARTICIPANT_ASSERTION);
    }
    return OSAPI_TIMEOUT_OP_MANUAL;
}

DDS_ReturnCode_t
DPSE_ParticipantDiscovery_schedule_fast_assertions(
        struct NDDS_Discovery_Plugin *discovery_plugin,
        const struct DDS_ParticipantBuiltinTopicData *local_participant_data,
        DDS_Boolean new_event)
{
    struct DPSE_DiscoveryPlugin *dpse_plugin =
                        (struct DPSE_DiscoveryPlugin *)discovery_plugin;
    struct OSAPITimeoutUserData storage;
    DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;

    DPSE_LOG_DEBUG(LOG_SCHEDULE_FAST_ASSERTIONS);

    if (new_event)
    {
        /* Advance the sequence number the first time */
        DDS_DataWriter_advance_sn(dpse_plugin->participant_writer);
    }

    /* Send out participant announcement */
    DDS_DataWriter_write(dpse_plugin->participant_writer,
                         (void *)local_participant_data, &DDS_HANDLE_NIL);

    storage.field[0] = (void *)dpse_plugin;
    storage.field[1] = (void *)local_participant_data;

    /* The number of initial announcements already sent is stored in the 
     * storage.count[0] variable passed to the event.  Since we have already
     * sent one announcement, we schedule sending the rest of the
     * initial announcements.
     */
    storage.count[0] =
        dpse_plugin->properties.initial_participant_announcements - 1;

    if (new_event)
    {
        if (!OSAPI_Timer_create_timeout(
                    DDS_DomainParticipant_get_timer(
                            DDS_Subscriber_get_participant(
                                    dpse_plugin->participant_subscriber)),
                    &dpse_plugin->announcement_event,
                    DDS_Duration_to_ms(dpse_plugin->properties.initial_participant_announcement_period),
                    OSAPI_TIMER_PERIODIC,
                    DPSE_ParticipantDiscovery_assert_participant,
                    &storage))
        {
            DPSE_Log_error(LOG_FAIL_SCHEDULE_FAST_ASSERTIONS);
            goto finally;
        }
    }
    else
    {
        /* Confirm that the announcement has been posted */
        if (dpse_plugin->announcement_event.epoch > 0)
        {
            if (!OSAPI_Timer_update_timeout(
                    DDS_DomainParticipant_get_timer(
                            DDS_Subscriber_get_participant(
                                    dpse_plugin->participant_subscriber)),
                    &dpse_plugin->announcement_event,
                    DDS_Duration_to_ms(dpse_plugin->properties.participant_liveliness_assert_period)))
            {
                DPSE_Log_error(LOG_FAIL_UPDATE_PARTICIPANT_ASSERTION);
                goto finally;
            }
        }
    }

    retcode = DDS_RETCODE_OK;

finally:
    return retcode;
}
