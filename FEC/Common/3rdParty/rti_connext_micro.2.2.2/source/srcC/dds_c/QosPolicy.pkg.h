/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef QosPolicy_pkg_h
#define QosPolicy_pkg_h

extern DDS_Boolean
DDS_DurabilityQosPolicy_is_consistent(const struct DDS_DurabilityQosPolicy *self);

extern DDS_Boolean
DDS_DurabilityQosPolicy_is_compatible(const struct DDS_DurabilityQosPolicy *request,
                                      const struct DDS_DurabilityQosPolicy *offered);

extern DDS_Boolean
DDS_DeadlineQosPolicy_is_consistent(const struct DDS_DeadlineQosPolicy *self);

extern DDS_Boolean
DDS_DeadlineQosPolicy_is_compatible(const struct DDS_DeadlineQosPolicy *request,
                                    const struct DDS_DeadlineQosPolicy *offered);

extern DDS_Boolean
DDS_HistoryQosPolicy_is_consistent(const struct DDS_HistoryQosPolicy *self);

extern DDS_Boolean
DDS_SystemResourceLimitsQosPolicy_immutable_is_equal(
                    const struct DDS_SystemResourceLimitsQosPolicy *left,
                    const struct DDS_SystemResourceLimitsQosPolicy *right);

extern DDS_Boolean
DDS_SystemResourceLimitsQosPolicy_is_consistent(
                    const struct DDS_SystemResourceLimitsQosPolicy *self);

extern DDS_Boolean
DDS_ResourceLimitsQosPolicy_is_consistent(
                                const struct DDS_ResourceLimitsQosPolicy *self);

extern DDS_Boolean
DDS_OwnershipQosPolicy_is_consistent(const struct DDS_OwnershipQosPolicy *self);

extern DDS_Boolean
DDS_OwnershipQosPolicy_is_compatible(const struct DDS_OwnershipQosPolicy *request,
                                     const struct DDS_OwnershipQosPolicy *offered);

extern DDS_Boolean
DDS_OwnershipStrengthQosPolicy_is_consistent(
                            const struct DDS_OwnershipStrengthQosPolicy *self);

extern DDS_Boolean
DDS_LivelinessQosPolicy_is_consistent(
                                    const struct DDS_LivelinessQosPolicy *self);

extern DDS_Boolean
DDS_LivelinessQosPolicy_is_compatible(const struct DDS_LivelinessQosPolicy *request,
                                      const struct DDS_LivelinessQosPolicy *offered);

extern DDS_Boolean
DDS_TimeBasedFilterQosPolicy_is_consistent(
                               const struct DDS_TimeBasedFilterQosPolicy *self);

extern DDS_Boolean
DDS_ReliabilityQosPolicy_is_consistent(
                                   const struct DDS_ReliabilityQosPolicy *self);

extern DDS_Boolean
DDS_ReliabilityQosPolicy_is_compatible(const struct DDS_ReliabilityQosPolicy *reader,
                                       const struct DDS_ReliabilityQosPolicy *writer);

extern DDS_Boolean
DDS_TypeSupportQosPolicy_is_consistent(
                                    const struct DDS_TypeSupportQosPolicy *self);

extern DDS_Boolean
DDS_DataWriterProtocolQosPolicy_is_consistent(
                            const struct DDS_DataWriterProtocolQosPolicy *self);

extern DDS_Boolean
DDS_DataReaderResourceLimitsQosPolicy_is_consistent(
                    const struct DDS_DataReaderResourceLimitsQosPolicy *self);

extern DDS_Boolean
DDS_TransportQosPolicy_is_equal(const struct DDS_TransportQosPolicy *left,
                                const struct DDS_TransportQosPolicy *right);

extern DDS_Boolean
DDS_TransportQosPolicy_is_consistent(const struct DDS_TransportQosPolicy *self);

extern DDS_Boolean
DDS_RtpsReliableWriterProtocol_is_consistent(
                          const struct DDS_RtpsReliableWriterProtocol_t *self);

extern DDS_Boolean
DDS_EntityNameQosPolicy_is_equal(const struct DDS_EntityNameQosPolicy *left,
                                 const struct DDS_EntityNameQosPolicy *right);

extern DDS_Boolean
DDS_EntityNameQosPolicy_is_consistent(
                                 const struct DDS_EntityNameQosPolicy *self);

extern DDS_Boolean
DDS_DataReaderProtocolQosPolicy_is_consistent(
                            const struct DDS_DataReaderProtocolQosPolicy *self);

#endif /* QosPolicy_pkg_h */
