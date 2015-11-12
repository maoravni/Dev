/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef Log_pkg_h
#define Log_pkg_h

struct OSAPI_LogMessageSeq
{
    OSAPI_LogMessage_T *_contiguous_buffer;
    RTI_INT32 _maximum;
    RTI_INT32 _length;
    RTI_INT32 _element_size;
};

#endif /* Log_pkg_h */
