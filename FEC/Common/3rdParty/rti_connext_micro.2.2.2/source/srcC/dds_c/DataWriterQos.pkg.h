/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DataWriterQos_pkg_h
#define DataWriterQos_pkg_h

extern DDS_Boolean
DDS_DataWriterQos_immutable_is_equal(const struct DDS_DataWriterQos *left,
                                     const struct DDS_DataWriterQos *right);

extern DDS_Boolean
DDS_DataWriterQos_is_consistent(const struct DDS_DataWriterQos *self);

#endif /* DataWriterQos_pkg_h */
