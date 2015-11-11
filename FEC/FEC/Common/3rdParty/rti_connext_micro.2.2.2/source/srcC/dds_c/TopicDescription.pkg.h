/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef TopicDescription_pkg_h
#define TopicDescription_pkg_h

struct DDS_TopicDescriptionImpl
{
    const char *topic_name;
    const char *type_name;
    DDS_DomainParticipant *participant;
    void *owner;
};

extern void
DDS_TopicDescriptionImpl_initialize(struct DDS_TopicDescriptionImpl *td,
                                    const char *topic_name,
                                    const char *type_name,
                                    void *topic);

extern void
DDS_TopicDescriptionImpl_finalize(struct DDS_TopicDescriptionImpl *td);

#endif /* TopicDescription_pkg_h */
