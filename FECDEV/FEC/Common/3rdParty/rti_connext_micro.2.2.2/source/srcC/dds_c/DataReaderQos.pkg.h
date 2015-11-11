/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef DataReaderQos_pkg_h
#define DataReaderQos_pkg_h

extern DDS_Boolean
DDS_DataReaderQos_immutable_is_equal(const struct DDS_DataReaderQos *left,
                                     const struct DDS_DataReaderQos *right);

extern DDS_Boolean
DDS_DataReaderQos_is_equal(const struct DDS_DataReaderQos *left,
                           const struct DDS_DataReaderQos *right);

extern DDS_Boolean
DDS_DataReaderQos_is_consistent(const struct DDS_DataReaderQos *self);

#endif /* DataReaderQos_pkg_h */
