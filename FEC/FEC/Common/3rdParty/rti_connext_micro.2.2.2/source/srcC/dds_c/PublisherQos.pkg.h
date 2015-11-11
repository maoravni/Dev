/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef PublisherQos_pkg_h
#define PublisherQos_pkg_h

extern DDS_ReturnCode_t
DDS_PublisherQos_finalize(struct DDS_PublisherQos * self);

extern DDS_Boolean
DDS_PublisherQos_is_equal(const struct DDS_PublisherQos *left,
                          const struct DDS_PublisherQos *right);

extern DDS_Boolean
DDS_PublisherQos_is_consistent(const struct DDS_PublisherQos *self);

#endif /* PublisherQos_pkg_h */
