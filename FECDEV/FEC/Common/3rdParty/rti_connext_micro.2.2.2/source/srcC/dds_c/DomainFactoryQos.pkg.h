/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DomainFactoryQos_pkg_h
#define DomainFactoryQos_pkg_h

extern DDS_Boolean
DDS_DomainParticipantFactoryQos_immutable_is_equal(
                     const struct DDS_DomainParticipantFactoryQos *left,
                     const struct DDS_DomainParticipantFactoryQos *right);

extern DDS_Boolean
DDS_DomainParticipantFactoryQos_is_equal(
                        const struct DDS_DomainParticipantFactoryQos *left,
                        const struct DDS_DomainParticipantFactoryQos *right);

extern DDS_Boolean
DDS_DomainParticipantFactoryQos_is_consistent(
                          const struct DDS_DomainParticipantFactoryQos *self);

#endif /* DomainFactoryQos_pkg_h */
