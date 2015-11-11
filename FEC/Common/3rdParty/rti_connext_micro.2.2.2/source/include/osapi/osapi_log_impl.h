/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef osapi_log_impl_h
#define osapi_log_impl_h



#if defined (RTI_EXTERNAL_BUILD)
  /* if it's an external build (ie, shipped source), we don't depend on
   * makehome's NDEBUG... intead, we always assume it's NDEBUG, and we depend
   * solely on explicit definition of RTI_LOG_DEBUG */
#define NDEBUG
#endif /*  */

#if !defined (NDEBUG)
#define RTI_LOG_DEBUG
#endif /* NDEBUG */

#define LOG_printDebug      OSAPI_Stdio_printf
#define LOG_printWarning    OSAPI_Stdio_printf
#define LOG_printError      OSAPI_Stdio_printf
#define LOG_printMessage    OSAPI_Stdio_printf

#if defined (RTI_LOG_DEBUG)
#define RTI_LOG_METHOD_NAME
#define RTI_LOG_WARNING
#define RTI_LOG_ERROR
#endif /* RTI_LOG_DEBUG */

#if defined (RTI_LOG_WARNING)
#define RTI_LOG_METHOD_NAME
#define RTI_LOG_ERROR
#endif /* RTI_LOG_WARNING */

#if defined (RTI_LOG_ERROR)
#define RTI_LOG_METHOD_NAME
#endif /* RTI_LOG_ERROR */

#if defined (RTI_LOG_METHOD_NAME)
#define LOG_methodNameDeclaration(name)
#define METHOD_NAME __func__

/*
 * #define LOG_methodNameDeclaration(name) \
 *     const char METHOD_NAME = name;
 */
#else
#define LOG_methodNameDeclaration(name)
#define METHOD_NAME
#endif


#define OSAPI_Log_error(code_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_OSAPI,__LINE__,0,0,0,0,0,0,0,0,0,0)

#define OSAPI_Log_error_p1(code_,p1_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_OSAPI,__LINE__,1,p1_,0,0,0,0,0,0,0,0)

#define OSAPI_Log_error_p2(code_,p1_,p2_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_OSAPI,__LINE__,2,p1_,p2_,0,0,0,0,0,0,0)

#define OSAPI_Log_error_p3(code_,p1_,p2_,p3_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_OSAPI,__LINE__,3,p1_,p2_,p3_,0,0,0,0,0,0)

#define OSAPI_Log_error_p4(code_,p1_,p2_,p3_,p4_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_OSAPI,__LINE__,4,p1_,p2_,p3_,p4_,0,0,0,0,0)

#define OSAPI_Log_error_p5(code_,p1_,p2_,p3_,p4_,p5_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_OSAPI,__LINE__,5,p1_,p2_,p3_,p4_,p5_,0,0,0,0)

#define OSAPI_Log_error_p6(code_,p1_,p2_,p3_,p4_,p5_,p6_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_OSAPI,__LINE__,6,p1_,p2_,p3_,p4_,p5_,p6_,0,0,0)

#define OSAPI_Log_error_p7(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_OSAPI,__LINE__,7,p1_,p2_,p3_,p4_,p5_,p6_,p7_,0,0)

#define OSAPI_Log_error_p8(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_OSAPI,__LINE__,8,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,0)

#define OSAPI_Log_error_p9(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_) \
    OSAPI_Log_msg_pN(code_ | LOG_ERROR | LOG_OSAPI,__LINE__,9,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_)

#ifdef OSAPI_LOG_ENABLE_WARNING

#define OSAPI_LOG_WARNING(code_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_OSAPI,__LINE__,0,0,0,0,0,0,0,0,0,0);

#define OSAPI_LOG_WARNING_P1(code_,p1_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_OSAPI,__LINE__,1,p1_,0,0,0,0,0,0,0,0);

#define OSAPI_LOG_WARNING_P2(code_,p1_,p2_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_OSAPI,__LINE__,2,p1_,p2_,0,0,0,0,0,0,0);

#define OSAPI_LOG_WARNING_P3(code_,p1_,p2_,p3_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_OSAPI,__LINE__,3,p1_,p2_,p3_,0,0,0,0,0,0);

#define OSAPI_LOG_WARNING_P4(code_,p1_,p2_,p3_,p4_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_OSAPI,__LINE__,4,p1_,p2_,p3_,p4_,0,0,0,0,0);

#define OSAPI_LOG_WARNING_P5(code_,p1_,p2_,p3_,p4_,p5_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_OSAPI,__LINE__,5,p1_,p2_,p3_,p4_,p5_,0,0,0,0);

#define OSAPI_LOG_WARNING_P6(code_,p1_,p2_,p3_,p4_,p5_,p6_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_OSAPI,__LINE__,6,p1_,p2_,p3_,p4_,p5_,p6_,0,0,0);

#define OSAPI_LOG_WARNING_P7(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_OSAPI,__LINE__,7,p1_,p2_,p3_,p4_,p5_,p6_,p7_,0,0);

#define OSAPI_LOG_WARNING_P8(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_OSAPI,__LINE__,8,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,0);

#define OSAPI_LOG_WARNING_P9(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_) \
    OSAPI_Log_msg_pN(code_ | LOG_WARNING | LOG_OSAPI,__LINE__,9,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_);

#else
#define OSAPI_LOG_WARNING(code_)

#define OSAPI_LOG_WARNING_P1(code_,p1_)

#define OSAPI_LOG_WARNING_P2(code_,p1_,p2_)

#define OSAPI_LOG_WARNING_P3(code_,p1_,p2_,p3_)

#define OSAPI_LOG_WARNING_P4(code_,p1_,p2_,p3_,p4_)

#define OSAPI_LOG_WARNING_P5(code_,p1_,p2_,p3_,p4_,p5_)

#define OSAPI_LOG_WARNING_P6(code_,p1_,p2_,p3_,p4_,p5_,p6_)

#define OSAPI_LOG_WARNING_P7(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_)

#define OSAPI_LOG_WARNING_P8(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_)

#define OSAPI_LOG_WARNING_P9(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_)

#endif

#ifdef OSAPI_LOG_ENABLE_DEBUG

#define OSAPI_LOG_DEBUG(code_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_OSAPI,__LINE__,0,0,0,0,0,0,0,0,0,0);

#define OSAPI_LOG_DEBUG_P1(code_,p1_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_OSAPI,__LINE__,1,p1_,0,0,0,0,0,0,0,0);

#define OSAPI_LOG_DEBUG_P2(code_,p1_,p2_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_OSAPI,__LINE__,2,p1_,p2_,0,0,0,0,0,0,0);

#define OSAPI_LOG_DEBUG_P3(code_,p1_,p2_,p3_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_OSAPI,__LINE__,3,p1_,p2_,p3_,0,0,0,0,0,0);

#define OSAPI_LOG_DEBUG_P4(code_,p1_,p2_,p3_,p4_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_OSAPI,__LINE__,4,p1_,p2_,p3_,p4_,0,0,0,0,0);

#define OSAPI_LOG_DEBUG_P5(code_,p1_,p2_,p3_,p4_,p5_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_OSAPI,__LINE__,5,p1_,p2_,p3_,p4_,p5_,0,0,0,0);

#define OSAPI_LOG_DEBUG_P6(code_,p1_,p2_,p3_,p4_,p5_,p6_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_OSAPI,__LINE__,6,p1_,p2_,p3_,p4_,p5_,p6_,0,0,0);

#define OSAPI_LOG_DEBUG_P7(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_OSAPI,__LINE__,7,p1_,p2_,p3_,p4_,p5_,p6_,p7_,0,0);

#define OSAPI_LOG_DEBUG_P8(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_OSAPI,__LINE__,8,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,0);

#define OSAPI_LOG_DEBUG_P9(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_) \
    OSAPI_Log_msg_pN(code_ | LOG_DEBUG | LOG_OSAPI,__LINE__,9,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_);

#else
#define OSAPI_LOG_DEBUG(code_)

#define OSAPI_LOG_DEBUG_P1(code_,p1_)

#define OSAPI_LOG_DEBUG_P2(code_,p1_,p2_)

#define OSAPI_LOG_DEBUG_P3(code_,p1_,p2_,p3_)

#define OSAPI_LOG_DEBUG_P4(code_,p1_,p2_,p3_,p4_)

#define OSAPI_LOG_DEBUG_P5(code_,p1_,p2_,p3_,p4_,p5_)

#define OSAPI_LOG_DEBUG_P6(code_,p1_,p2_,p3_,p4_,p5_,p6_)

#define OSAPI_LOG_DEBUG_P7(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_)

#define OSAPI_LOG_DEBUG_P8(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_)

#define OSAPI_LOG_DEBUG_P9(code_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_)

#endif

#if defined (RTI_LOG_METHOD_NAME)
#define LOG_PRECONDITION_ENABLED 0
#define LOG_testPrecondition(preconditionExpression, failAction)    \
if (preconditionExpression)\
{\
  LOG_printDebug("precondition test failed: %s:%d\n", __FILE__, __LINE__);\
  failAction;\
}

#define LOG_PRECONDITION(cond_, action_) \
if ((cond_)) \
{\
    OSAPI_Log_msg_pN(LOG_PRECONDITION_ERROR,__LINE__,0,0,0,0,0,0,0,0,0,0);\
    LOG_printDebug("precondition failed: %s:%d\n", __FILE__, __LINE__);\
    action_;\
}

#define LOG_PRECONDITION_P1(cond_,action_,p1_) \
if ((cond_)) \
{\
    OSAPI_Log_msg_pN(LOG_PRECONDITION_ERROR,__LINE__,1,p1_,0,0,0,0,0,0,0,0);\
    LOG_printDebug("precondition failed: %s:%d\n", __FILE__, __LINE__);\
    action_;\
}

#define LOG_PRECONDITION_P2(cond_, action_,p1_,p2_)\
if ((cond_)) \
{\
    OSAPI_Log_msg_pN(LOG_PRECONDITION_ERROR,__LINE__,2,p1_,p2_,0,0,0,0,0,0,0);\
    LOG_printDebug("precondition failed: %s:%d\n", __FILE__, __LINE__);\
    action_;\
}

#define LOG_PRECONDITION_P3(cond_, action_,p1_,p2_,p3_)\
if ((cond_)) \
{\
    OSAPI_Log_msg_pN(LOG_PRECONDITION_ERROR,__LINE__,3,p1_,p2_,p3_,0,0,0,0,0,0);\
    LOG_printDebug("precondition failed: %s:%d\n", __FILE__, __LINE__);\
    action_;\
}

#define LOG_PRECONDITION_P4(cond_, action_,p1_,p2_,p3_,p4_)\
if ((cond_)) \
{\
    OSAPI_Log_msg_pN(LOG_PRECONDITION_ERROR,__LINE__,4,p1_,p2_,p3_,p4_,0,0,0,0,0);\
    LOG_printDebug("precondition failed: %s:%d\n", __FILE__, __LINE__);\
    action_;\
}

#define LOG_PRECONDITION_P5(cond_, action_,p1_,p2_,p3_,p4_,p5_)\
if ((cond_)) \
{\
    OSAPI_Log_msg_pN(LOG_PRECONDITION_ERROR,__LINE__,5,p1_,p2_,p3_,p4_,p5_,0,0,0,0);\
    LOG_printDebug("precondition failed: %s:%d\n", __FILE__, __LINE__);\
    action_;\
}

#define LOG_PRECONDITION_P6(cond_, action_,p1_,p2_,p3_,p4_,p5_,p6_) \
if ((cond_)) \
{\
    OSAPI_Log_msg_pN(LOG_PRECONDITION_ERROR,__LINE__,6,p1_,p2_,p3_,p4_,p5_,p6_,0,0,0);\
    LOG_printDebug("precondition failed: %s:%d\n", __FILE__, __LINE__);\
    action_;\
}

#define LOG_PRECONDITION_P7(cond_,action_,p1_,p2_,p3_,p4_,p5_,p6_,p7_) \
if ((cond_)) \
{\
    OSAPI_Log_msg_pN(LOG_PRECONDITION_ERROR,__LINE__,7,p1_,p2_,p3_,p4_,p5_,p6_,p7_,0,0);\
    LOG_printDebug("precondition failed: %s:%d\n", __FILE__, __LINE__);\
    action_;\
}

#define LOG_PRECONDITION_P8(cond_, action_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_) \
if ((cond_)) \
{\
    OSAPI_Log_msg_pN(LOG_PRECONDITION_ERROR,__LINE__,8,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,0);\
    LOG_printDebug("precondition failed: %s:%d\n", __FILE__, __LINE__);\
    action_;\
}

#define LOG_PRECONDITION_P9(cond_, action_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_)\
if ((cond_)) \
{\
    OSAPI_Log_msg_pN(LOG_PRECONDITION_ERROR,__LINE__,9,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_);\
    LOG_printDebug("precondition failed: %s:%d\n", __FILE__, __LINE__);\
    action_;\
}

#else /*  */
#ifdef LOG_PRECONDITION_ENABLED
#undef LOG_PRECONDITION_ENABLED
#endif
#define LOG_testPrecondition(preconditionExpression, failAction) ((void)0)
#define LOG_PRECONDITION(cond_, action_)
#define LOG_PRECONDITION_P1(cond_, action_,p1_)
#define LOG_PRECONDITION_P2(cond_, action_,p1_,p2_)
#define LOG_PRECONDITION_P3(cond_, action_,p1_,p2_,p3_)
#define LOG_PRECONDITION_P4(cond_, action_,p1_,p2_,p3_,p4_)
#define LOG_PRECONDITION_P5(cond_, action_,p1_,p2_,p3_,p4_,p5_)
#define LOG_PRECONDITION_P6(cond_, action_,p1_,p2_,p3_,p4_,p5_,p6_)
#define LOG_PRECONDITION_P7(cond_, action_,p1_,p2_,p3_,p4_,p5_,p6_,p7_)
#define LOG_PRECONDITION_P8(cond_, action_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_)
#define LOG_PRECONDITION_P9(cond_, action_,p1_,p2_,p3_,p4_,p5_,p6_,p7_,p8_,p9_)
#endif /*  */

#define OSAPI_LOG_PTR_HIGH(a_) ((sizeof(void*) == 4) ? 0 : ((RTI_INT32)(a_)>>32))
#define OSAPI_LOG_PTR_LOW(a_)  ((sizeof(void*) == 4) ? 0 : ((RTI_INT32)(a_)&0xffffffff))


#endif /* osapi_log_impl_h */
