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
#include "SubscriberQos.pkg.h"
#include "SubscriberImpl.pkg.h"
#include "DataReader.pkg.h"
#include "DataReaderQos.pkg.h"

/*** SOURCE_BEGIN ***/

DDS_Boolean
DDS_SubscriberEvent_on_requested_incompatible_qos(DDS_Subscriber *subscriber,
                   DDS_DataReader *reader,
                   const struct DDS_RequestedIncompatibleQosStatus*status)
{
    struct DDS_SubscriberImpl *self = (struct DDS_SubscriberImpl *)subscriber;
    DDS_Boolean retval = DDS_BOOLEAN_TRUE;

    if (self->mask & DDS_REQUESTED_INCOMPATIBLE_QOS_STATUS)
    {
        self->listener.as_datareaderlistener.on_requested_incompatible_qos(
           self->listener.as_datareaderlistener.as_listener.listener_data,
           reader, status);
    }
    else
    {
        self->config.on_requested_incompatible_qos(
                                            self->participant,reader, status);
    }

    return retval;
}

DDS_Boolean
DDS_SubscriberEvent_on_subscription_matched(DDS_Subscriber *subscriber,
                            DDS_DataReader *reader,
                            const struct DDS_SubscriptionMatchedStatus *status)
{
    struct DDS_SubscriberImpl *self = (struct DDS_SubscriberImpl *)subscriber;
    DDS_Boolean retval = DDS_BOOLEAN_TRUE;

    if (self->mask & DDS_SUBSCRIPTION_MATCHED_STATUS)
    {
        self->listener.as_datareaderlistener.on_subscription_matched(
                self->listener.as_datareaderlistener.as_listener.listener_data,
           reader, status);
    }
    else
    {
        self->config.on_subscription_matched(self->participant,reader, status);
    }

    return retval;
}

DDS_Boolean
DDS_SubscriberEvent_on_data_available(DDS_Subscriber *subscriber,
                                      DDS_DataReader *reader)
{
    struct DDS_SubscriberImpl *self = (struct DDS_SubscriberImpl *)subscriber;
    DDS_Boolean retval = DDS_BOOLEAN_TRUE;

    if (self->mask & DDS_DATA_AVAILABLE_STATUS)
    {
        self->listener.as_datareaderlistener.on_data_available(
                 self->listener.as_datareaderlistener.as_listener.listener_data,
                 reader);
    }
    else
    {
        self->config.on_data_available(self->participant,subscriber,reader);
    }

    return retval;
}

#ifdef ENABLE_STATUS_LISTENER
DDS_Boolean
DDS_SubscriberEvent_on_sample_rejected(DDS_Subscriber * subscriber,
                           DDS_DataReader *reader,
                           const struct DDS_SampleRejectedStatus *status)
{
    struct DDS_SubscriberImpl *self = (struct DDS_SubscriberImpl *)subscriber;
    DDS_Boolean retval = DDS_BOOLEAN_TRUE;

    if (self->mask & DDS_SAMPLE_REJECTED_STATUS)
    {
        self->listener.as_datareaderlistener.
            on_sample_rejected(
              self->listener.as_datareaderlistener.as_listener.listener_data,
              reader, status);
    }
    else
    {
        self->config.on_sample_rejected(self->participant,reader,status);
    }

    return retval;
}

DDS_Boolean
DDS_SubscriberEvent_on_requested_deadline_missed(DDS_Subscriber *subscriber,
                     DDS_DataReader *reader,
                     const struct DDS_RequestedDeadlineMissedStatus *status)
{
    struct DDS_SubscriberImpl *self = (struct DDS_SubscriberImpl *)subscriber;
    DDS_Boolean retval = DDS_BOOLEAN_TRUE;

    if (self->mask & DDS_REQUESTED_DEADLINE_MISSED_STATUS)
    {
        self->listener.as_datareaderlistener.on_requested_deadline_missed
            (self->listener.as_datareaderlistener.as_listener.listener_data,
             reader, status);
    }
    else
    {
        self->config.on_requested_deadline_missed(
                                             self->participant,reader,status);
    }

    return retval;
}

DDS_Boolean
DDS_SubscriberEvent_on_liveliness_changed(DDS_Subscriber * subscriber,
                   DDS_DataReader * reader,
                   const struct DDS_LivelinessChangedStatus *status)
{
    struct DDS_SubscriberImpl *self = (struct DDS_SubscriberImpl *)subscriber;
    DDS_Boolean retval = DDS_BOOLEAN_TRUE;

    if (self->mask & DDS_LIVELINESS_LOST_STATUS)
    {
        self->listener.as_datareaderlistener.on_liveliness_changed
            (self->listener.as_datareaderlistener.as_listener.listener_data,
             reader, status);
    }
    else
    {
        self->config.on_liveliness_changed(self->participant,reader,status);
    }

    return retval;
}

DDS_Boolean
DDS_SubscriberEvent_on_sample_lost(DDS_Subscriber * subscriber,
                               DDS_DataReader * reader,
                               const struct DDS_SampleLostStatus *status)
{
    struct DDS_SubscriberImpl *self = (struct DDS_SubscriberImpl *)subscriber;
    DDS_Boolean retval = DDS_BOOLEAN_TRUE;

    if (self->mask & DDS_SAMPLE_LOST_STATUS)
    {
        self->listener.as_datareaderlistener.on_sample_lost(
                self->listener.as_datareaderlistener.as_listener.listener_data,
                reader, status);
        return DDS_BOOLEAN_TRUE;
    }
    else
    {
        self->config.on_sample_lost(self->participant,reader,status);
    }

    return retval;
}

#endif

