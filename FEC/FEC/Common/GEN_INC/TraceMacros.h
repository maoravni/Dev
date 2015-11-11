// TraceMacros.h
#ifndef	TRACE_MACROS_H
#define	TRACE_MACROS_H

extern long SysTickCounter;
////Called during the tick interrupt.
#define traceTASK_INCREMENT_TICK( tick ){	\
  SysTickCounter=	tick; }
  //LOG_AddSysLogMessageTask(0, NULL);}

#if _ENABLE_TRACE_
#define traceTASK_SWITCHED_IN() { \
  LOG_AddSysLogMessageTask(0, NULL);}
#endif
#endif
