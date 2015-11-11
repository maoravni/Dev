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
#include "DataReaderImpl.pkg.h"
#include "DataReader.pkg.h"
#include "DataReaderEvent.pkg.h"
#include "DataReaderQos.pkg.h"
#include "DataReaderInterface.pkg.h"

/*** SOURCE_BEGIN ***/

DDS_Boolean
DDS_DataReader_writer_is_compatible(DDS_DataReader *datareader,
                                    const struct DDS_DataWriterQos *dw_qos)
{
    struct DDS_DataReaderImpl *dr = (struct DDS_DataReaderImpl *)datareader;
    DDS_Boolean retval = DDS_BOOLEAN_TRUE;

    if (!DDS_DeadlineQosPolicy_is_compatible(&dr->qos.deadline,
                                             &dw_qos->deadline))
    {
        dr->last_policy_id = DDS_DEADLINE_QOS_POLICY_ID;
        retval = DDS_BOOLEAN_FALSE;
    }

    if (!DDS_OwnershipQosPolicy_is_compatible(&dr->qos.ownership,
                                              &dw_qos->ownership))
    {
        dr->last_policy_id = DDS_OWNERSHIP_QOS_POLICY_ID;
        retval = DDS_BOOLEAN_FALSE;
    }

    if (!DDS_ReliabilityQosPolicy_is_compatible(&dr->qos.reliability,
                                                &dw_qos->reliability))
    {
        dr->last_policy_id = DDS_RELIABILITY_QOS_POLICY_ID;
        retval = DDS_BOOLEAN_FALSE;
    }

    if (!DDS_LivelinessQosPolicy_is_compatible(&dr->qos.liveliness,
                                               &dw_qos->liveliness))
    {
        dr->last_policy_id = DDS_LIVELINESS_QOS_POLICY_ID;
        retval = DDS_BOOLEAN_FALSE;
    }

    if (!DDS_DurabilityQosPolicy_is_compatible(&dr->qos.durability,
                                               &dw_qos->durability))
    {
        dr->last_policy_id = DDS_DURABILITY_QOS_POLICY_ID;
        retval = DDS_BOOLEAN_FALSE;
    }

    return retval;
}

DDS_Boolean
DDS_DataReader_add_anonymous_route(DDS_DataReader *self,
                         struct NETIO_Address *src_writer,
                         struct NETIO_Address *from_address)
{
    struct DDS_DataReaderImpl *dr = (struct DDS_DataReaderImpl *)self;
    DDS_InstanceHandle_t ih;
    DDS_BuiltinTopicKey_t dw_key;
    NETIO_Interface_T *ext_intf;
    NETIO_RouteKind_T bind_kind;
    RTI_BOOL route_existed = RTI_FALSE;

    if (self->as_entity.entity_id & 0xc0)
    {
        bind_kind = NETIO_ROUTEKIND_META;
    }
    else
    {
        bind_kind = NETIO_ROUTEKIND_USER;
    }

    ih = DDS_Entity_get_instance_handle(DDS_DataReader_as_entity(self));
    DDS_BuiltinTopicKey_from_guid(&dw_key,&ih);

    LOG_printDebug("DDS_DataReader_add_anonymous_route: adding anon route for topic: %p %x.%x.%x.%x: %s\n",
            self,
            dw_key.value[0],
            dw_key.value[1],
            dw_key.value[2],
            dw_key.value[3],
            DDS_TopicDescription_get_name(DDS_Topic_as_topicdescription(dr->topic)));

    DB_Database_lock(dr->config.db);

    if (from_address->kind == RTPS_LOCATOR_KIND_UDPv4)
    {
        NETIO_RouteResolver_add_route(dr->config.route_resolver,
                (NETIO_Interface_T*)dr->rtps_intf,src_writer,
                from_address,NULL,&route_existed);

        if (!NETIO_Interface_bind(dr->dr_intf,src_writer,NULL,NULL))
        {
            goto done;
        }

        if (!NETIO_Interface_bind(dr->rtps_intf,src_writer,NULL,NULL))
        {
            goto done;
        }

        if (!NETIO_Interface_get_external_interface(
                dr->dr_intf,src_writer,&ext_intf,from_address))
        {
            goto done;
        }

        if (!NETIO_Interface_bind_external(
                dr->rtps_intf,src_writer,ext_intf,src_writer,
                NULL,NULL))
        {
            goto done;
        }

        if (!NETIO_Interface_get_external_interface(
                dr->rtps_intf,src_writer,&ext_intf,from_address))
        {
            goto done;
        }

        NETIO_BindResolver_bind(dr->config.bind_resolver,
                (struct REDA_StringSeq*)dr->config.enabled_transports,
                bind_kind,
                from_address,ext_intf,src_writer);
    }

done:
    DB_Database_unlock(dr->config.db);

    return RTI_TRUE;
}

DDS_Boolean
DDS_DataReader_add_route(DDS_DataReader *self,
                         const DDS_BuiltinTopicKey_t *key,
                         const struct DDS_DataWriterQos *const qos,
                         const struct DDS_LocatorSeq *locator_seq,
                         RTI_BOOL *route_existed)
{
    struct DDS_DataReaderImpl *dr = (struct DDS_DataReaderImpl *)self;
    struct NETIO_Address dst_addr;
    struct NETIO_Address src_writer;
    DDS_InstanceHandle_t ih;
    DDS_BuiltinTopicKey_t dr_key;
    DDS_Boolean retval = DDS_BOOLEAN_FALSE;
    NETIO_Interface_T *ext_intf;
    struct DDS_Locator *a_locator;
    DDS_Long i,j;
    struct NETIO_Address from_address;
    RTI_BOOL local_route_existed;
    struct DDS_LocatorSeq *dr_locator_seq;
    NETIO_RouteKind_T bind_kind;
    struct NETIOBindProperty b_prop = NETIOBindProperty_INITIALIZER;

    if (self->as_entity.entity_id & 0xc0)
    {
        bind_kind = NETIO_ROUTEKIND_META;
    }
    else
    {
        bind_kind = NETIO_ROUTEKIND_USER;
    }

    ih = DDS_Entity_get_instance_handle(DDS_DataReader_as_entity(dr));
    DDS_BuiltinTopicKey_from_guid(&dr_key,&ih);
    NETIO_Address_set_guid_from_key(&src_writer,0,key);
    NETIO_Address_set_guid_from_key(&dst_addr,0,&dr_key);

    LOG_printDebug("DataReader: adding route for topic: %p,%x.%x.%x.%x: %s, %p\n",self,
            dr_key.value[0],
            dr_key.value[1],
            dr_key.value[2],
            dr_key.value[3],
            DDS_TopicDescription_get_name(DDS_Topic_as_topicdescription(dr->topic)),
            dr->rtps_intf);

    DB_Database_lock(dr->config.db);

    if (qos->ownership.kind == DDS_EXCLUSIVE_OWNERSHIP_QOS)
    {
        b_prop.strength = qos->ownership_strength.value;
    }

    /* If the loop-back interface exists, bind the reader interface to it
     * so the reader can receive data directly from the writer. Note that
     * for loop-back there is no back-channel, thus we only bind to it.
     */
    if (DDS_BuiltinTopicKey_prefix_equals(key,&dr_key) &&
                              !(self->as_entity.entity_id & 0xc0))
    {
        if (!NETIO_Interface_get_external_interface(
                dr->dr_intf,&src_writer,&ext_intf,&dst_addr))
        {
            goto done;
        }

        NETIO_BindResolver_bind(dr->config.bind_resolver,
                (struct REDA_StringSeq*)dr->config.enabled_transports,
                bind_kind,
                &src_writer,ext_intf,&dst_addr);

        if (!NETIO_Interface_bind(dr->dr_intf,&src_writer,&b_prop,NULL))
        {
            goto done;
        }
    }
    else
    {
        if (!NETIO_Interface_bind(dr->dr_intf,
                &src_writer,&b_prop,route_existed))
        {
            goto done;
        }

        if (!NETIO_Interface_bind(dr->rtps_intf,&src_writer,
                NULL,&local_route_existed))
        {
            if (!local_route_existed)
            {
                goto done;
            }
        }

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
                from_address = *((struct NETIO_Address*)a_locator);

                if (!NETIO_RouteResolver_add_route(dr->config.route_resolver,
                        dr->rtps_intf,&src_writer,&from_address,NULL,NULL))
                {
                    goto done;
                }

                if (!NETIO_Interface_get_external_interface(
                        dr->dr_intf,&src_writer,&ext_intf,&from_address))
                {
                    goto done;
                }

                /* Bind to RTPS interface */
                if (!NETIO_Interface_bind_external(
                        dr->rtps_intf,&src_writer,ext_intf,&src_writer,
                        NULL,&local_route_existed))
                {
                    if (!local_route_existed)
                    {
                        goto done;
                    }
                }
            }
        }

        if (!NETIO_Interface_get_external_interface(
                dr->rtps_intf,&src_writer,&ext_intf,&src_writer))
        {
            goto done;
        }

        if (dr->as_entity.entity_id & 0xc0)
        {
            dr_locator_seq = self->config.default_meta_unicast;
        }
        else
        {
            dr_locator_seq = self->config.default_unicast;
        }

        j = DDS_LocatorSeq_get_length(dr_locator_seq);
        for (i = 0; i < j; i++)
        {
            from_address = *(struct NETIO_Address*)
                  DDS_LocatorSeq_get_reference(dr_locator_seq,i);

            if (!NETIO_BindResolver_bind(dr->config.bind_resolver,
                    (struct REDA_StringSeq*)dr->config.enabled_transports,
                    bind_kind,&from_address,ext_intf,&src_writer))
            {
                goto done;
            }
        }

        if (dr->as_entity.entity_id & 0xc0)
        {
            dr_locator_seq = self->config.default_meta_multicast;
        }
        else
        {
            dr_locator_seq = self->config.default_multicast;
        }

        j = DDS_LocatorSeq_get_length(dr_locator_seq);
        for (i = 0; i < j; i++)
        {
            from_address = *(struct NETIO_Address*)
                  DDS_LocatorSeq_get_reference(dr_locator_seq,i);

            if (!NETIO_BindResolver_bind(dr->config.bind_resolver,
                    (struct REDA_StringSeq*)dr->config.enabled_transports,
                    bind_kind,&from_address,ext_intf,&src_writer))
            {
                goto done;
            }
        }
    }

    retval = DDS_BOOLEAN_TRUE;

done:

    if (!retval)
    {
        LOG_printDebug("DataReader: Route creation failed for topic: %p,%x.%x.%x.%x: %s\n",self,
            dr_key.value[0],
            dr_key.value[1],
            dr_key.value[2],
            dr_key.value[3],
            DDS_TopicDescription_get_name(DDS_Topic_as_topicdescription(dr->topic)));

    } else
    {
        LOG_printDebug("DataReader: Route creation succeeded for topic: %p,%x.%x.%x.%x: %s\n",self,
            dr_key.value[0],
            dr_key.value[1],
            dr_key.value[2],
            dr_key.value[3],
            DDS_TopicDescription_get_name(DDS_Topic_as_topicdescription(dr->topic)));

    }
    DB_Database_unlock(dr->config.db);
    return retval;
}

DDS_Boolean
DDS_DataReader_delete_route(DDS_DataReader *self,
                            const DDS_BuiltinTopicKey_t *key,
                            const struct DDS_LocatorSeq *locator_seq,
                            RTI_BOOL *route_existed)
{
    struct DDS_DataReaderImpl *dr = (struct DDS_DataReaderImpl *)self;
    struct NETIO_Address dst_addr;
    struct NETIO_Address src_writer;
    DDS_InstanceHandle_t ih;
    DDS_BuiltinTopicKey_t dr_key;
    DDS_Boolean retval = DDS_BOOLEAN_FALSE;
    NETIO_Interface_T *ext_intf;
    struct NETIO_Address from_address;
    struct DDS_Locator *a_locator;
    DDS_Long i,j;
    NETIO_RouteKind_T bind_kind;
    struct DDS_LocatorSeq *dr_locator_seq;

    if (self->as_entity.entity_id & 0xc0)
    {
        bind_kind = NETIO_ROUTEKIND_META;
    }
    else
    {
        bind_kind = NETIO_ROUTEKIND_USER;
    }

    ih = DDS_Entity_get_instance_handle(DDS_DataReader_as_entity(dr));
    DDS_BuiltinTopicKey_from_guid(&dr_key,&ih);
    NETIO_Address_set_guid_from_key(&src_writer,0,key);
    NETIO_Address_set_guid_from_key(&dst_addr,0,&dr_key);

    LOG_printDebug("DataReader: Deleting route for topic: %p,%x.%x.%x.%x: %s\n",dr,
            dr_key.value[0],
            dr_key.value[1],
            dr_key.value[2],
            dr_key.value[3],
            DDS_TopicDescription_get_name(
                    DDS_Topic_as_topicdescription(dr->topic)));

    /* If the loop-back interface exists, bind the reader interface to it
     * so the reader can receive data directly from the writer. Note that
     * for loop-back there is no back-channel, thus we only bind to it.
     */
    DB_Database_lock(dr->config.db);

    if (DDS_BuiltinTopicKey_prefix_equals(key,&dr_key))
    {
        if (!NETIO_Interface_get_external_interface(
                dr->dr_intf,&src_writer,&ext_intf,&dst_addr))
        {
            goto done;
        }

        LOG_printDebug("intra\n");

        if (!NETIO_BindResolver_unbind(dr->config.bind_resolver,
                (struct REDA_StringSeq*)dr->config.enabled_transports,
                bind_kind,
                &src_writer,
                ext_intf,&dst_addr))
        {
            goto done;
        }

        if (!NETIO_Interface_unbind(dr->dr_intf,
                                    &src_writer,dr->dr_intf,route_existed))
        {
            goto done;
        }
    }
    else
    {
        if (!NETIO_Interface_get_external_interface(
                dr->rtps_intf,&src_writer,&ext_intf,&src_writer))
        {
            goto done;
        }

        if (dr->as_entity.entity_id & 0xc0)
        {
            dr_locator_seq = self->config.default_meta_unicast;
        }
        else
        {
            dr_locator_seq = self->config.default_unicast;
        }

        j = DDS_LocatorSeq_get_length(dr_locator_seq);
        for (i = 0; i < j; i++)
        {
            from_address = *(struct NETIO_Address*)
                  DDS_LocatorSeq_get_reference(dr_locator_seq,i);

            if (!NETIO_BindResolver_unbind(dr->config.bind_resolver,
                    (struct REDA_StringSeq*)dr->config.enabled_transports,
                    bind_kind,&from_address,ext_intf,&src_writer))
            {
                LOG_printDebug("unbind failed 1\n");
                goto done;
            }
        }

        if (dr->as_entity.entity_id & 0xc0)
        {
            dr_locator_seq = self->config.default_meta_multicast;
        }
        else
        {
            dr_locator_seq = self->config.default_multicast;
        }

        j = DDS_LocatorSeq_get_length(dr_locator_seq);
        for (i = 0; i < j; i++)
        {
            from_address = *(struct NETIO_Address*)
                  DDS_LocatorSeq_get_reference(dr_locator_seq,i);

            if (!NETIO_BindResolver_unbind(dr->config.bind_resolver,
                    (struct REDA_StringSeq*)dr->config.enabled_transports,
                    bind_kind,&from_address,ext_intf,&src_writer))
            {
                LOG_printDebug("unbind failed 2\n");
                goto done;
            }
        }

        /* All other communication paths relies on RTPS. The data-writer
         * creates and established the RTPS interface upon creation, thus
         * it already exists. All we need to do is add a route DW->RTPS and
         * a route from RTPS-> requested DR interface.
         */
        /* All other communication paths relies on RTPS. The data-writer
         * creates and established the RTPS interface upon creation, thus
         * it already exists. All we need to do is add a route DW->RTPS and
         * a route from RTPS-> requested DR interface.
         */
        j = DDS_LocatorSeq_get_length(locator_seq);
        for (i = 0; i < j; i++)
        {
            a_locator = DDS_LocatorSeq_get_reference(locator_seq,i);
            from_address = *((struct NETIO_Address*)a_locator);

            if (a_locator->kind == RTPS_LOCATOR_KIND_UDPv4)
            {
                if (!NETIO_Interface_get_external_interface(
                        dr->dr_intf,&src_writer,&ext_intf,&from_address))
                {
                    goto done;
                }

                /* Bind to RTPS interface */
                if (!NETIO_Interface_unbind_external(
                        dr->rtps_intf,&src_writer,ext_intf,&src_writer,NULL))
                {
                    goto done;
                }

                if (!NETIO_Interface_unbind(dr->rtps_intf,&src_writer,NULL,NULL))
                {
                        goto done;
                }

                if (!NETIO_RouteResolver_delete_route(
                        dr->config.route_resolver,
                        dr->rtps_intf,&src_writer,
                        &from_address,NULL))
                {
                    goto done;
                }

                if (!NETIO_Interface_unbind(dr->dr_intf,
                                            &src_writer,NULL,route_existed))
                {
                    goto done;
                }
            }
        }
    }

    retval = DDS_BOOLEAN_TRUE;

    LOG_printDebug("DataReader: Deleted route for topic: %p,%x.%x.%x.%x: %s\n",dr,
            dr_key.value[0],
            dr_key.value[1],
            dr_key.value[2],
            dr_key.value[3],
            DDS_TopicDescription_get_name(
                    DDS_Topic_as_topicdescription(dr->topic)));

done:

    DB_Database_unlock(dr->config.db);
    return retval;
}

DDS_Boolean
DDS_DataReader_match_writer(DDS_DataReader *self,
                            const struct DDS_BuiltinTopicKey_t *key,
                            const struct DDS_DataWriterQos *const qos,
                            const struct DDS_LocatorSeq *locator_seq)

{
    RTI_BOOL route_existed = DDS_BOOLEAN_FALSE;
    DDS_Boolean retval = DDS_BOOLEAN_FALSE;

    if (!DDS_DataReader_writer_is_compatible(self,qos))
    {
        retval = DDS_DataReader_delete_route(self,key,locator_seq,&route_existed);
        if (route_existed)
        {
            DDS_DataReaderEvent_on_subscription_matched(
                                    self,key,route_existed,DDS_BOOLEAN_FALSE);
        }
        DDS_DataReaderEvent_on_incompatible_qos(self,key);
    }
    else
    {
        retval = DDS_DataReader_add_route(self,key,qos,locator_seq,&route_existed);
        if (!route_existed)
        {
            DDS_DataReaderEvent_on_subscription_matched(
                                    self,key,route_existed,DDS_BOOLEAN_TRUE);
        }
    }

    return retval;
}

DDS_Boolean
DDS_DataReader_unmatch_writer(DDS_DataReader *datareader,
                              const struct DDS_BuiltinTopicKey_t *key,
                              const struct DDS_DataWriterQos *const qos,
                              const struct DDS_LocatorSeq *locator_seq)
{
    DDS_Boolean retval;
    RTI_BOOL route_existed = DDS_BOOLEAN_FALSE;

    if (datareader->qos.management.is_anonymous)
    {
        return DDS_BOOLEAN_TRUE;
    }
    retval = DDS_DataReader_delete_route(datareader,key,locator_seq,&route_existed);

    if (route_existed)
    {
        DDS_DataReaderEvent_on_subscription_matched(
                                datareader,key,route_existed,DDS_BOOLEAN_FALSE);
    }

    return retval;
}

DDS_Boolean
DDS_DataReader_match_local_writer(DDS_DataReader *self,
                                  struct DDS_BuiltinTopicKey_t *key,
                                  const char *topic_name,
                                  const char *type_name,
                                  const struct DDS_DataWriterQos *const qos)
{
    struct DDS_DataReaderImpl *dr = (struct DDS_DataReaderImpl *)self;

    if (!DDS_Topic_is_compatible(dr->topic,topic_name,type_name))
    {
        return DDS_BOOLEAN_FALSE;
    }

    return DDS_DataReader_match_writer(self,key,qos,NULL);
}

DDS_Boolean
DDS_DataReader_match_remote_writer(DDS_DataReader *self,
        const struct DDS_ParticipantBuiltinTopicData *const parent_data,
        const struct DDS_PublicationBuiltinTopicData *const data)
{
    struct DDS_DataReaderImpl *dr = (struct DDS_DataReaderImpl *)self;
    struct DDS_DataWriterQos dw_qos = DDS_DataWriterQos_INITIALIZER;
    const struct DDS_LocatorSeq *locator_seq;

    /* PublicationBuiltinTopicData is a subset of the DDS_DataWriterQos
     */
    dw_qos.deadline = data->deadline;
    dw_qos.liveliness = data->liveliness;
    dw_qos.ownership = data->ownership;
    dw_qos.ownership_strength = data->ownership_strength;
    dw_qos.reliability = data->reliability;
    dw_qos.durability = data->durability;

    /* TODO: Add conversion of transports */

    if (!DDS_Topic_is_compatible(dr->topic,data->topic_name,data->type_name))
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

    return DDS_DataReader_match_writer(self,&data->key,&dw_qos,locator_seq);
}

DDS_Boolean
DDS_DataReader_unmatch_local_writer(DDS_DataReader *self,
                                    struct DDS_BuiltinTopicKey_t *key,
                                    struct DDS_DataWriterQos *const qos)
{
    return DDS_DataReader_unmatch_writer(self,key,qos,NULL);
}

DDS_Boolean
DDS_DataReader_unmatch_remote_writer(DDS_DataReader *self,
                    const struct DDS_ParticipantBuiltinTopicData *const parent_data,
                    const struct DDS_PublicationBuiltinTopicData *const data)
{
    struct DDS_DataWriterQos dw_qos = DDS_DataWriterQos_INITIALIZER;
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

    return DDS_DataReader_unmatch_writer(self,&data->key,&dw_qos,locator_seq);
}

