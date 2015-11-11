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
#ifndef dds_c_discovery_plugin_h
#include "dds_c/dds_c_discovery_plugin.h"
#endif

#include "Entity.pkg.h"
#include "PublisherImpl.pkg.h"

/*** SOURCE_BEGIN ***/

DDS_Boolean
DDS_PublisherEvent_on_offered_incompatible_qos(DDS_Publisher *self,
                    DDS_DataWriter *writer,
                    const struct DDS_OfferedIncompatibleQosStatus *status)
{
    struct DDS_PublisherImpl *publisher = (struct DDS_PublisherImpl *)self;
    DDS_Boolean retval = DDS_BOOLEAN_TRUE;

    if (publisher->mask & DDS_OFFERED_INCOMPATIBLE_QOS_STATUS)
    {
        publisher->listener.as_datawriterlistener.
            on_offered_incompatible_qos(publisher->listener.
                                        as_datawriterlistener.as_listener.
                                        listener_data, writer, status);
        return DDS_BOOLEAN_TRUE;
    }
    else
    {
        publisher->config.on_offered_incompatible_qos(
                                      publisher->participant, writer, status);
    }

    return retval;
}

DDS_Boolean
DDS_PublisherEvent_on_publication_matched(DDS_Publisher *self,
                     DDS_DataWriter *writer,
                     const struct DDS_PublicationMatchedStatus *status)
{
    struct DDS_PublisherImpl *publisher = (struct DDS_PublisherImpl *)self;
    DDS_Boolean retval = DDS_BOOLEAN_TRUE;

    if (publisher->mask & DDS_PUBLICATION_MATCHED_STATUS)
    {
        publisher->listener.as_datawriterlistener.
            on_publication_matched(publisher->listener.as_datawriterlistener.
                                   as_listener.listener_data, writer, status);
        return DDS_BOOLEAN_TRUE;
    }
    else
    {
        publisher->config.on_publication_matched(
                                      publisher->participant, writer, status);
    }

    return retval;
}

#ifdef ENABLE_STATUS_LISTENER
DDS_Boolean
DDS_PublisherEvent_on_liveliness_lost(DDS_Publisher *self,
                                  DDS_DataWriter *writer,
                                  const struct DDS_LivelinessLostStatus *status)
{
    struct DDS_PublisherImpl *publisher = (struct DDS_PublisherImpl *)self;
    DDS_Boolean retval = DDS_BOOLEAN_TRUE;

    if (publisher->mask & DDS_LIVELINESS_LOST_STATUS)
    {
        publisher->listener.as_datawriterlistener.on_liveliness_lost
            (publisher->listener.as_datawriterlistener.as_listener.
             listener_data, writer, status);
    }
    else
    {
        publisher->config.on_liveliness_lost(publisher->participant,
                                             writer, status);
    }

    return retval;
}

DDS_Boolean
DDS_PublisherEvent_on_offered_deadline_missed(DDS_Publisher *self,
                    DDS_DataWriter *writer,
                    const struct DDS_OfferedDeadlineMissedStatus *status)
{
    struct DDS_PublisherImpl *publisher = (struct DDS_PublisherImpl *)self;
    DDS_Boolean retval = DDS_BOOLEAN_TRUE;

    if (publisher->mask & DDS_OFFERED_DEADLINE_MISSED_STATUS)
    {
        publisher->listener.as_datawriterlistener.
            on_offered_deadline_missed(publisher->listener.
                                       as_datawriterlistener.as_listener.
                                       listener_data, writer, status);
        return DDS_BOOLEAN_TRUE;
    }
    else
    {
        publisher->config.on_offered_deadline_missed(
                                publisher->participant, writer, status);
    }

    return retval;
}
#endif
