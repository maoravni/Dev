/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef SubscriptionListener_pkg_h
#define SubscriptionListener_pkg_h

extern void
DPDE_SubscriptionBuiltinDataReaderListener_initialize(
                                       struct DPDE_DiscoveryPlugin *plugin,
                                       struct DDS_DataReaderListener *listener);

extern void
DPDE_SubscriptionBuiltinDataWriterListener_initialize(
                                    struct DPDE_DiscoveryPlugin *plugin,
                                    struct DDS_DataWriterListener *listener);

#endif /* SubscriptionListener_pkg_h */
