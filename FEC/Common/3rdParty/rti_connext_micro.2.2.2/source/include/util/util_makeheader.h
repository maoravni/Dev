/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef util_makeheader_h
#define util_makeheader_h



#if defined(RTI_WIN32) || defined(_WIN32)
#pragma warning(disable:4068)
#endif

#define Public
#define Package
#define Peer
#define RTI_PRIVATE static
#ifndef __cplusplus
#define public
#define package
#define peer
#define rti_private static
#endif /* !__cplusplus */


#endif /* util_makeheader_h */
