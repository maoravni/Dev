/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef TopicQos_pkg_h
#define TopicQos_pkg_h

extern DDS_Boolean
DDS_TopicQos_is_consistent(const struct DDS_TopicQos *self);

extern DDS_Boolean
DDS_TopicQos_immutable_is_equal(const struct DDS_TopicQos *left,
                                const struct DDS_TopicQos *right);

#endif /* TopicQos_pkg_h */
