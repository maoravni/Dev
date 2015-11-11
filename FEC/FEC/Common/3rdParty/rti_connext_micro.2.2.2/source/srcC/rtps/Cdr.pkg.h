/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef Cdr_pkg_h
#define Cdr_pkg_h

extern RTI_BOOL
MIGRtpsGuid_serialize(struct CDR_Stream_t *stream,
                      const struct MIGRtpsGuid *guid,
                      void * param);

#endif /* Cdr_pkg_h */
