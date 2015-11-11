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
#ifndef osapi_timer_h
#include "osapi/osapi_timer.h"
#endif
#ifndef reda_bufferpool_h
#include "reda/reda_bufferpool.h"
#endif
#ifndef netio_address_h
#include "netio/netio_address.h"
#endif
#ifndef netio_route_h
#include "netio/netio_route.h"
#endif
#ifndef dds_c_common_impl_h
#include "dds_c/dds_c_common_impl.h"
#endif
#ifndef dds_c_discovery_plugin_h
#include "dds_c/dds_c_discovery_plugin.h"
#endif

#include "QosPolicy.pkg.h"
#include "Entity.pkg.h"
#include "TopicDescription.pkg.h"
#include "Topic.pkg.h"
#include "DataWriterImpl.pkg.h"
#include "DataWriterEvent.pkg.h"
#include "DataWriterQos.pkg.h"
#include "DataWriterInterface.pkg.h"
#include "PublisherEvent.pkg.h"

/*** SOURCE_BEGIN ***/

DDS_Boolean
DDS_DataWriter_reader_is_compatible(DDS_DataWriter *datawriter,
                                    const struct DDS_DataReaderQos *dr_qos)
{
    struct DDS_DataWriterImpl *dw = (struct DDS_DataWriterImpl *)datawriter;
    DDS_Boolean retval = DDS_BOOLEAN_TRUE;

    if (!DDS_DeadlineQosPolicy_is_compatible(&dr_qos->deadline,
                                             &dw->qos.deadline))
    {
        datawriter->last_policy_id = DDS_DEADLINE_QOS_POLICY_ID;
        retval = DDS_BOOLEAN_FALSE;
    }

    if (!DDS_OwnershipQosPolicy_is_compatible(&dr_qos->ownership,
                                              &dw->qos.ownership))
    {
        datawriter->last_policy_id = DDS_OWNERSHIP_QOS_POLICY_ID;
        retval = DDS_BOOLEAN_FALSE;
    }

    if (!DDS_ReliabilityQosPolicy_is_compatible(&dr_qos->reliability,
                                                &dw->qos.reliability))
    {
        datawriter->last_policy_id = DDS_RELIABILITY_QOS_POLICY_ID;
        retval = DDS_BOOLEAN_FALSE;
    }

    if (!DDS_LivelinessQosPolicy_is_compatible(&dr_qos->liveliness,
                                               &dw->qos.liveliness))
    {
        datawriter->last_policy_id = DDS_LIVELINESS_QOS_POLICY_ID;
        retval = DDS_BOOLEAN_FALSE;
    }

    if (!DDS_DurabilityQosPolicy_is_compatible(&dr_qos->durability,
                                               &dw->qos.durability))
    {
        datawriter->last_policy_id = DDS_DURABILITY_QOS_POLICY_ID;
        retval = DDS_BOOLEAN_FALSE;
    }

    return retval;
}

/* TODO:
 * It should be possible to use the regular add_route call by using
 * the DDS_DataWriter_match_remote_reader, and just create a temporary
 * DDS_SubscriptionBuiltinTopicData (since the Qos is well-known)
 */
DDS_Boolean
DDS_DataWriter_add_anonymous_route(DDS_DataWriter *datawriter,
                                   struct NETIO_Address *dst_reader,
                                   struct NETIO_Address *via_address)
{
    struct DDS_DataWriterImpl *dw = (struct DDS_DataWriterImpl *)datawriter;
    DDS_InstanceHandle_t ih;
    DDS_BuiltinTopicKey_t dw_key;
    DDS_Boolean retval = DDS_BOOLEAN_TRUE;
    RTI_BOOL route_existed;


    ih = DDS_Entity_get_instance_handle(DDS_DataWriter_as_entity(datawriter));
    DDS_BuiltinTopicKey_from_guid(&dw_key,&ih);

    LOG_printDebug("DataWriter: adding route for topic: %p,%x.%x.%x.%x: %s\n",dw,
            dw_key.value[0],
            dw_key.value[1],
            dw_key.value[2],
            dw_key.value[3],
            DDS_TopicDescription_get_name(
                    DDS_Topic_as_topicdescription(dw->topic)));

    DB_Database_lock(datawriter->config.db);

    if (via_address->kind == RTPS_LOCATOR_KIND_UDPv4)
    {
        /* TODO: verify first/lastSn not needed specified */
        if (!NETIO_Interface_add_route(
                                (NETIO_Interface_T*)dw->dw_intf,dst_reader,
                                dw->rtps_intf,dst_reader,NULL,&route_existed))
        {
            retval = DDS_BOOLEAN_FALSE;
        }

        NETIO_RouteResolver_add_route(datawriter->config.route_resolver,
                (NETIO_Interface_T*)dw->rtps_intf,dst_reader,
                via_address,NULL,&route_existed);
    }

    DB_Database_unlock(datawriter->config.db);

    return DDS_BOOLEAN_TRUE;
}

DDS_Boolean
DDS_DataWriter_add_route(DDS_DataWriter *datawriter,
                         const DDS_BuiltinTopicKey_t *key,
                         const struct DDS_DataReaderQos *const qos,
                         const struct DDS_LocatorSeq *locator_seq,
                         RTI_BOOL *route_existed)
{
    struct DDS_DataWriterImpl *dw = (struct DDS_DataWriterImpl *)datawriter;
    struct NETIO_Address dst_reader;
    DDS_InstanceHandle_t ih;
    DDS_BuiltinTopicKey_t dw_key;
    DDS_Boolean retval = DDS_BOOLEAN_TRUE;
    struct DDS_Locator *a_locator;
    DDS_Long i,j;
    struct NETIO_Address via_address;
    struct RTPSRouteProperty route_property;
    struct DDSHST_WriterState *wh_state;
    RTI_BOOL local_route_existed;
    struct NETIO_Address from_address;
    NETIO_Interface_T *ext_intf;
    struct DDS_LocatorSeq *dr_locator_seq;
    NETIO_RouteKind_T recv_kind;

    ih = DDS_Entity_get_instance_handle(DDS_DataWriter_as_entity(datawriter));
    DDS_BuiltinTopicKey_from_guid(&dw_key,&ih);
    NETIO_Address_set_guid_from_key(&dst_reader,0,key);

    LOG_printDebug("DataWriter: adding route for topic: %p,%x.%x.%x.%x: %s\n",dw,
            dw_key.value[0],
            dw_key.value[1],
            dw_key.value[2],
            dw_key.value[3],
            DDS_TopicDescription_get_name(
                    DDS_Topic_as_topicdescription(dw->topic)));

    DB_Database_lock(dw->config.db);

    wh_state = DDSHST_Writer_get_state(dw->wh);

    /* A route is only added upon a match. Thus, add route is only concerned
     * with managing resources to reach its peer based on destination
     * information for the peer.
     */
    /* if the entity is within the same participant, use the loopback
     * interface.
     * TODO: Add an option to the data-reader to disable the loopback interface
     */

    /* select loopback from resource table netio_intf */

    if (DDS_BuiltinTopicKey_prefix_equals(key,&dw_key) &&
            !(dw->as_entity.entity_id & 0xc0))
    {
        if (!NETIO_RouteResolver_add_route(datawriter->config.route_resolver,
                (NETIO_Interface_T*)dw->dw_intf,&dst_reader,
                &dst_reader,NULL,route_existed))
        {
            retval = DDS_BOOLEAN_FALSE;
        }
    }
    else
    {
        /* All other communication paths relies on RTPS. The data-writer
         * creates and established the RTPS interface upon creation, thus
         * it already exists. All we need to do is add a route DW->RTPS and
         * a route from RTPS-> requested DR interface.
         */
        j = DDS_LocatorSeq_get_length(locator_seq);
        for (i = 0; i < j; i++)
        {
            a_locator = DDS_LocatorSeq_get_reference(locator_seq,i);

            if (a_locator->kind == RTPS_LOCATOR_KIND_UDPv4)
            {
                if (!NETIO_Interface_add_route(
                        (NETIO_Interface_T*)dw->dw_intf,&dst_reader,
                        dw->rtps_intf,&dst_reader,NULL,route_existed))
                {
                    retval = DDS_BOOLEAN_FALSE;
                }

                route_property.reliable =
                       (qos->reliability.kind == DDS_RELIABLE_RELIABILITY_QOS);
                route_property.first_sn = wh_state->low_sn;
                route_property.last_sn = wh_state->high_sn;
                via_address = *((struct NETIO_Address*)a_locator);

                if (!NETIO_RouteResolver_add_route(datawriter->config.route_resolver,
                        (NETIO_Interface_T*)dw->rtps_intf,&dst_reader,
                        &via_address,&route_property._parent,route_existed))
                {
                    retval = DDS_BOOLEAN_FALSE;
                }
            }
        }

        if (!NETIO_Interface_bind(dw->rtps_intf,
                                    &dst_reader,NULL,&local_route_existed))
        {
            if (!local_route_existed)
            {
                goto done;
            }
        }

        if (!NETIO_Interface_get_external_interface(
                dw->dw_intf,&dst_reader,&ext_intf,&from_address))
        {
            goto done;
        }

        if (!NETIO_Interface_bind_external(
                dw->rtps_intf,&dst_reader,ext_intf,&dst_reader,
                NULL,&local_route_existed))
        {
            if (!local_route_existed)
            {
                goto done;
            }
        }

        if (!NETIO_Interface_get_external_interface(
                dw->rtps_intf,&dst_reader,&ext_intf,&from_address))
        {
            goto done;
        }

        if (dw->as_entity.entity_id & 0xc0)
        {
            dr_locator_seq = dw->config.default_meta_unicast;
            recv_kind = NETIO_ROUTEKIND_META;
        }
        else
        {
            dr_locator_seq = dw->config.default_unicast;
            recv_kind = NETIO_ROUTEKIND_USER;
        }

        j = DDS_LocatorSeq_get_length(dr_locator_seq);
        for (i = 0; i < j; i++)
        {
            from_address = *(struct NETIO_Address*)
                  DDS_LocatorSeq_get_reference(dr_locator_seq,i);
            NETIO_BindResolver_bind(datawriter->config.bind_resolver,
                    (struct REDA_StringSeq*)datawriter->config.enabled_transports,
                    recv_kind,&from_address,
                    ext_intf,&dst_reader);
        }
    }

    retval = DDS_BOOLEAN_TRUE;

done:

    if (retval)
    {
        LOG_printDebug("DataWriter: Added route for topic: %p,%x.%x.%x.%x: %s\n",dw,
                dw_key.value[0],
                dw_key.value[1],
                dw_key.value[2],
                dw_key.value[3],
                DDS_TopicDescription_get_name(
                        DDS_Topic_as_topicdescription(dw->topic)));
    }
    else
    {
        LOG_printDebug("DataWriter: Failed to add route for topic: %p,%x.%x.%x.%x: %s\n",dw,
                dw_key.value[0],
                dw_key.value[1],
                dw_key.value[2],
                dw_key.value[3],
                DDS_TopicDescription_get_name(
                        DDS_Topic_as_topicdescription(dw->topic)));
    }
    DB_Database_unlock(dw->config.db);

    return retval;
}

DDS_Boolean
DDS_DataWriter_delete_route(DDS_DataWriter *datawriter,
                            const DDS_BuiltinTopicKey_t *key,
                            const struct DDS_LocatorSeq *locator_seq,
                            RTI_BOOL *route_existed)
{
    struct DDS_DataWriterImpl *dw = (struct DDS_DataWriterImpl *)datawriter;
    DDS_BuiltinTopicKey_t dw_key;
    DDS_InstanceHandle_t ih;
    struct NETIO_Address dst_reader;
    DDS_Boolean retval = DDS_BOOLEAN_FALSE;
    NETIO_Interface_T *ext_intf;
    struct NETIO_Address from_address;
    struct NETIO_Address via_address;
    DDS_Long i,j;
    NETIO_RouteKind_T recv_kind;
    struct DDS_LocatorSeq *dr_locator_seq;

    if (datawriter->as_entity.entity_id & 0xc0)
    {
        recv_kind = NETIO_ROUTEKIND_META;
    }
    else
    {
        recv_kind = NETIO_ROUTEKIND_USER;
    }

    ih = DDS_Entity_get_instance_handle(DDS_DataWriter_as_entity(datawriter));
    DDS_BuiltinTopicKey_from_guid(&dw_key,&ih);
    NETIO_Address_set_guid_from_key(&dst_reader,0,key);

    LOG_printDebug("DataWriter: Delete route for topic: %p,%x.%x.%x.%x: %s\n",dw,
            dw_key.value[0],
            dw_key.value[1],
            dw_key.value[2],
            dw_key.value[3],
            DDS_TopicDescription_get_name(
                    DDS_Topic_as_topicdescription(dw->topic)));

    DB_Database_lock(dw->config.db);

    *route_existed = RTI_TRUE;
    if (DDS_BuiltinTopicKey_prefix_equals(key,&dw_key))
    {
        if (!NETIO_RouteResolver_delete_route(datawriter->config.route_resolver,
                (NETIO_Interface_T*)dw->dw_intf,&dst_reader,
                &dst_reader,route_existed))
        {
            DB_Database_unlock(dw->config.db);
            return DDS_BOOLEAN_FALSE;
        }
    }
    else
    {
        if (!NETIO_Interface_delete_route(
                (NETIO_Interface_T*)dw->dw_intf,&dst_reader,
                dw->rtps_intf,&dst_reader,route_existed))
        {
            retval = DDS_BOOLEAN_FALSE;
            goto done;
        }

        if (!(*route_existed))
        {
            retval = DDS_BOOLEAN_TRUE;
            goto done;
        }

        /* All other communication paths relies on RTPS. The data-writer
         * creates and established the RTPS interface upon creation, thus
         * it already exists. All we need to do is add a route DW->RTPS and
         * a route from RTPS-> requested DR interface.
         */
        j = DDS_LocatorSeq_get_length(locator_seq);
        for (i = 0; i < j; i++)
        {
            via_address = *(struct NETIO_Address*)DDS_LocatorSeq_get_reference(locator_seq,i);
            if (!NETIO_RouteResolver_delete_route(datawriter->config.route_resolver,
                    (NETIO_Interface_T*)dw->rtps_intf,&dst_reader,
                    &via_address,route_existed))
            {
                retval = DDS_BOOLEAN_FALSE;
                goto done;
            }
        }

        if (!NETIO_Interface_unbind(dw->rtps_intf,&dst_reader,NULL,NULL))
        {
            goto done;
        }

        if (!NETIO_Interface_get_external_interface(
                dw->dw_intf,&dst_reader,&ext_intf,&from_address))
        {
            goto done;
        }

        if (!NETIO_Interface_unbind_external(
                dw->rtps_intf,&dst_reader,ext_intf,&dst_reader,NULL))
        {
            goto done;
        }

        if (dw->as_entity.entity_id & 0xc0)
        {
            dr_locator_seq = dw->config.default_meta_unicast;
            recv_kind = NETIO_ROUTEKIND_META;
        }
        else
        {
            dr_locator_seq = dw->config.default_unicast;
            recv_kind = NETIO_ROUTEKIND_USER;
        }

        j = DDS_LocatorSeq_get_length(dr_locator_seq);
        for (i = 0; i < j; i++)
        {
            from_address = *(struct NETIO_Address*)
                  DDS_LocatorSeq_get_reference(dr_locator_seq,i);
            NETIO_BindResolver_unbind(datawriter->config.bind_resolver,
                    (struct REDA_StringSeq*)datawriter->config.enabled_transports,
                    recv_kind,&from_address,
                    ext_intf,&dst_reader);
        }
    }

    retval = DDS_BOOLEAN_TRUE;

    LOG_printDebug("DataWriter: Deleted route for topic: %p,%x.%x.%x.%x: %s\n",dw,
            dw_key.value[0],
            dw_key.value[1],
            dw_key.value[2],
            dw_key.value[3],
            DDS_TopicDescription_get_name(
                    DDS_Topic_as_topicdescription(dw->topic)));

done:

    DB_Database_unlock(dw->config.db);

    return retval;
}

DDS_Boolean
DDS_DataWriter_match_reader(DDS_DataWriter *datawriter,
                            const struct DDS_BuiltinTopicKey_t *key,
                            const struct DDS_DataReaderQos *const qos,
                            const struct DDS_LocatorSeq *locator_seq)
{
    struct DDS_DataWriterImpl *dw = (struct DDS_DataWriterImpl *)datawriter;
    RTI_BOOL route_existed = DDS_BOOLEAN_FALSE;
    DDS_Boolean retval = DDS_BOOLEAN_FALSE;

    if (!DDS_DataWriter_reader_is_compatible(datawriter,qos))
    {
        LOG_printDebug("incompatible reader\n");
        retval = DDS_DataWriter_delete_route(dw,key,locator_seq,&route_existed);
        if (route_existed)
        {
            DDS_DataWriterEvent_on_publication_matched(
                                datawriter,key,route_existed,DDS_BOOLEAN_FALSE);
        }
        DDS_DataWriterEvent_on_incompatible_qos(datawriter,key);
    }
    else
    {
        LOG_printDebug("compatible reader\n");
        retval = DDS_DataWriter_add_route(
                     datawriter,key,qos,locator_seq,&route_existed);
        if (!route_existed)
        {
            DDS_DataWriterEvent_on_publication_matched(
                    datawriter,key,route_existed,DDS_BOOLEAN_TRUE);
        }
    }

    return retval;
}

DDS_Boolean
DDS_DataWriter_unmatch_reader(DDS_DataWriter *datawriter,
                            const struct DDS_BuiltinTopicKey_t *key,
                            const struct DDS_DataReaderQos *const qos,
                            const struct DDS_LocatorSeq *locator_seq)
{
    DDS_Boolean retval;
    RTI_BOOL route_existed = DDS_BOOLEAN_FALSE;

    if (datawriter->qos.management.is_anonymous)
    {
        return DDS_BOOLEAN_TRUE;
    }

    retval = DDS_DataWriter_delete_route(datawriter,key,locator_seq,&route_existed);
    if (route_existed)
    {
        DDS_DataWriterEvent_on_publication_matched(
                datawriter,key,route_existed,DDS_BOOLEAN_FALSE);
    }

    return retval;
}

DDS_Boolean
DDS_DataWriter_match_local_reader(DDS_DataWriter *datawriter,
                                  struct DDS_BuiltinTopicKey_t *key,
                                  const char *topic_name,
                                  const char *type_name,
                                  const struct DDS_DataReaderQos *const qos)
{
    struct DDS_DataWriterImpl *dw = (struct DDS_DataWriterImpl *)datawriter;

    if (!DDS_Topic_is_compatible(dw->topic,topic_name,type_name))
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_DataWriter_match_reader(datawriter,key,qos,NULL);
}

DDS_Boolean
DDS_DataWriter_match_remote_reader(DDS_DataWriter *self,
        const struct DDS_ParticipantBuiltinTopicData *const parent_data,
        const struct DDS_SubscriptionBuiltinTopicData *const data)
{
    struct DDS_DataWriterImpl *dw = (struct DDS_DataWriterImpl *)self;
    struct DDS_DataReaderQos dr_qos = DDS_DataReaderQos_INITIALIZER;
    const struct DDS_LocatorSeq *locator_seq;

    /* SubscriptionBuiltinTopicData is a subset of the DataReaderQos
     */
    dr_qos.deadline = data->deadline;
    dr_qos.liveliness = data->liveliness;
    dr_qos.ownership = data->ownership;
    dr_qos.reliability = data->reliability;
    dr_qos.durability = data->durability;

    /* If the topic is not compatible it is not an incompatible qos */
    if (!DDS_Topic_is_compatible(dw->topic,data->topic_name,data->type_name))
    {
        return DDS_BOOLEAN_FALSE;
    }

    /* Choose the correct locators to use, meta or unicast */
    if (data->key.value[3] & 0xc0)
    {
        locator_seq = &parent_data->metatraffic_unicast_locators;
    }
    else
    {
        locator_seq = &data->unicast_locator;
        if (DDS_LocatorSeq_get_length(locator_seq) == 0)
        {
            locator_seq = &parent_data->default_unicast_locators;
        }
    }

    return DDS_DataWriter_match_reader(self,&data->key,&dr_qos,locator_seq);
}


DDS_Boolean
DDS_DataWriter_unmatch_local_reader(DDS_DataWriter *self,
                                    struct DDS_BuiltinTopicKey_t *key,
                                    struct DDS_DataReaderQos *const qos)
{
    return DDS_DataWriter_unmatch_reader(self,key,qos,NULL);
}

DDS_Boolean
DDS_DataWriter_unmatch_remote_reader(DDS_DataWriter *self,
        const struct DDS_ParticipantBuiltinTopicData *const parent_data,
        const struct DDS_SubscriptionBuiltinTopicData *const data)
{
    struct DDS_DataReaderQos dr_qos = DDS_DataReaderQos_INITIALIZER;
    const struct DDS_LocatorSeq *locator_seq;

    /* Choose the correct locators to use, meta or unicast */
    if (data->key.value[3] & 0xc0)
    {
        locator_seq = &parent_data->metatraffic_unicast_locators;
    }
    else
    {
        locator_seq = &data->unicast_locator;
        if (DDS_LocatorSeq_get_length(locator_seq) == 0)
        {
            locator_seq = &parent_data->default_unicast_locators;
        }
    }

    return DDS_DataWriter_unmatch_reader(self,&data->key,&dr_qos,locator_seq);
}
