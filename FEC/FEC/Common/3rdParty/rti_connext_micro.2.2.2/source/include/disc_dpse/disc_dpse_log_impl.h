/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef disc_dpse_log_impl_h
#define disc_dpse_log_impl_h



#define DPSE_Log_error(code_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_DISC_DPSE,__LINE__,0,0,0,0,0,0,0,0,0,0)

#define DPSE_Log_error_p1(code_,p1_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_DISC_DPSE,__LINE__,1,p1_,0,0,0,0,0,0,0,0)

#define DPSE_Log_error_p2(code_,p1_,p2_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_DISC_DPSE,__LINE__,2,p1_,p2_,0,0,0,0,0,0,0)

#define DPSE_Log_error_p3(code_,p1_,p2_,p3_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_DISC_DPSE,__LINE__,3,p1_,p2_,p3_,0,0,0,0,0,0)

#define DPSE_Log_error_p4(code_,p1_,p2_,p3_,p4_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_DISC_DPSE,__LINE__,4,p1_,p2_,p3_,p4_,0,0,0,0,0)

#define DPSE_Log_error_p5(code_,p1_,p2_,p3_,p4_,p5_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_DISC_DPSE,__LINE__,5,p1_,p2_,p3_,p4_,p5_,0,0,0,0)

#define DPSE_Log_error_p6(code_,p1_,p2_,p3_,p4_,p5_,p6_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_DISC_DPSE,__LINE__,6,p1_,p2_,p3_,p4_,p5_,p6_,0,0,0)

#define DPSE_Log_error_p7(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_DISC_DPSE,__LINE__,7,p1_,p2_,p3_,p4_,p5_,p6_,p7_,0,0)

#define DPSE_Log_error_p8(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_DISC_DPSE,__LINE__,8,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,0)

#define DPSE_Log_error_p9(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_DISC_DPSE,__LINE__,9,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_)

#ifdef OSAPI_LOG_ENABLE_WARNING

#define DPSE_LOG_WARNING(code_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_DISC_DPSE,__LINE__,0,0,0,0,0,0,0,0,0,0);

#define DPSE_LOG_WARNING_P1(code_,p1_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_DISC_DPSE,__LINE__,1,p1_,0,0,0,0,0,0,0,0);

#define DPSE_LOG_WARNING_P2(code_,p1_,p2_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_DISC_DPSE,__LINE__,2,p1_,p2_,0,0,0,0,0,0,0);

#define DPSE_LOG_WARNING_P3(code_,p1_,p2_,p3_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_DISC_DPSE,__LINE__,3,p1_,p2_,p3_,0,0,0,0,0,0);

#define DPSE_LOG_WARNING_P4(code_,p1_,p2_,p3_,p4_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_DISC_DPSE,__LINE__,4,p1_,p2_,p3_,p4_,0,0,0,0,0);

#define DPSE_LOG_WARNING_P5(code_,p1_,p2_,p3_,p4_,p5_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_DISC_DPSE,__LINE__,5,p1_,p2_,p3_,p4_,p5_,0,0,0,0);

#define DPSE_LOG_WARNING_P6(code_,p1_,p2_,p3_,p4_,p5_,p6_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_DISC_DPSE,__LINE__,6,p1_,p2_,p3_,p4_,p5_,p6_,0,0,0);

#define DPSE_LOG_WARNING_P7(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_DISC_DPSE,__LINE__,7,p1_,p2_,p3_,p4_,p5_,p6_,p7_,0,0);

#define DPSE_LOG_WARNING_P8(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_DISC_DPSE,__LINE__,8,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,0);

#define DPSE_LOG_WARNING_P9(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_DISC_DPSE,__LINE__,9,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_);

#else
#define DPSE_LOG_WARNING(code_)

#define DPSE_LOG_WARNING_P1(code_,p1_)

#define DPSE_LOG_WARNING_P2(code_,p1_,p2_)

#define DPSE_LOG_WARNING_P3(code_,p1_,p2_,p3_)

#define DPSE_LOG_WARNING_P4(code_,p1_,p2_,p3_,p4_)

#define DPSE_LOG_WARNING_P5(code_,p1_,p2_,p3_,p4_,p5_)

#define DPSE_LOG_WARNING_P6(code_,p1_,p2_,p3_,p4_,p5_,p6_)

#define DPSE_LOG_WARNING_P7(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_)

#define DPSE_LOG_WARNING_P8(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_)

#define DPSE_LOG_WARNING_P9(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_)

#endif

#ifdef OSAPI_LOG_ENABLE_DEBUG

#define DPSE_LOG_DEBUG(code_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_DISC_DPSE,__LINE__,0,0,0,0,0,0,0,0,0,0);

#define DPSE_LOG_DEBUG_P1(code_,p1_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_DISC_DPSE,__LINE__,1,p1_,0,0,0,0,0,0,0,0);

#define DPSE_LOG_DEBUG_P2(code_,p1_,p2_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_DISC_DPSE,__LINE__,2,p1_,p2_,0,0,0,0,0,0,0);

#define DPSE_LOG_DEBUG_P3(code_,p1_,p2_,p3_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_DISC_DPSE,__LINE__,3,p1_,p2_,p3_,0,0,0,0,0,0);

#define DPSE_LOG_DEBUG_P4(code_,p1_,p2_,p3_,p4_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_DISC_DPSE,__LINE__,4,p1_,p2_,p3_,p4_,0,0,0,0,0);

#define DPSE_LOG_DEBUG_P5(code_,p1_,p2_,p3_,p4_,p5_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_DISC_DPSE,__LINE__,5,p1_,p2_,p3_,p4_,p5_,0,0,0,0);

#define DPSE_LOG_DEBUG_P6(code_,p1_,p2_,p3_,p4_,p5_,p6_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_DISC_DPSE,__LINE__,6,p1_,p2_,p3_,p4_,p5_,p6_,0,0,0);

#define DPSE_LOG_DEBUG_P7(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_DISC_DPSE,__LINE__,7,p1_,p2_,p3_,p4_,p5_,p6_,p7_,0,0);

#define DPSE_LOG_DEBUG_P8(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_DISC_DPSE,__LINE__,8,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,0);

#define DPSE_LOG_DEBUG_P9(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_DISC_DPSE,__LINE__,9,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_);

#else
#define DPSE_LOG_DEBUG(code_)

#define DPSE_LOG_DEBUG_P1(code_,p1_)

#define DPSE_LOG_DEBUG_P2(code_,p1_,p2_)

#define DPSE_LOG_DEBUG_P3(code_,p1_,p2_,p3_)

#define DPSE_LOG_DEBUG_P4(code_,p1_,p2_,p3_,p4_)

#define DPSE_LOG_DEBUG_P5(code_,p1_,p2_,p3_,p4_,p5_)

#define DPSE_LOG_DEBUG_P6(code_,p1_,p2_,p3_,p4_,p5_,p6_)

#define DPSE_LOG_DEBUG_P7(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_)

#define DPSE_LOG_DEBUG_P8(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_)

#define DPSE_LOG_DEBUG_P9(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_)

#endif


#endif /* disc_dpse_log_impl_h */
