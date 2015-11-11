#line 1 "..\\src\\api\\api_lib.c"
/*
 * @file
 * Sequential API External module
 *
 */
 
/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/* This is the part of the API that is linked with
   the application */

#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/opt.h"
/*
 * @file
 *
 * lwIP Options Configuration
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */



/*
 * Include user defined options first. Anything not defined in these files
 * will be set to standard values. Override anything you dont like!
 */
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\ConfigFiles\\lwipopts.h"
/**
  ******************************************************************************
  * @file    lwipopts.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    07-October-2011
  * @brief   lwIP Options Confuration.
  *          This file is based on Utilities\lwip_v1.3.2\src\include\lwip\opt.h 
  *          and contains the lwIP configuration for the STM32F2x7 demonstration.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */




//#define LWIP_RAND rand
//#define LWIP_PLATFORM_DIAG printf


/**
 * SYS_LIGHTWEIGHT_PROT==1: if you want inter-task protection for certain
 * critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 */


//#define IP_REASSEMBLY           1
//#define IP_FRAG                 1
//#define ARP_QUEUEING            1
//#define MEMP_NUM_REASSDATA              10

// Define keep-alive to true to detect when the socket is killed.





// enable timeouts:


/* ---------- Memory options ---------- */
/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which
   lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2
   byte alignment -> define MEM_ALIGNMENT to 2. */


/* MEM_SIZE: the size of the heap memory. If the application will send
a lot of data that needs to be copied, this should be set high. */


/* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
   sends a lot of data out of ROM (or other static memory), this
   should be set high. */
//#define MEMP_NUM_PBUF           100
//#define PBUF_POOL_SIZE                  32
/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
   per active UDP "connection". */

/* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP
   connections. */

/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP
   connections. */

/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP
   segments. */

/* MEMP_NUM_SYS_TIMEOUT: the number of simulateously active
   timeouts. */



/**
 * MEMP_NUM_NETBUF: the number of struct netbufs.
 * (only needed if you use the sequential API, like api_lib.c)
 */

// Maor: This is an assessment - we need 2*number of number TCP connections.



/* ---------- Pbuf options ---------- */
/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */
//#define PBUF_POOL_SIZE          4

/* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. */
//#define PBUF_POOL_BUFSIZE       700


/* ---------- TCP options ---------- */



/* Controls if TCP should queue segments that arrive out of
   order. Define to 0 if your device is low on memory. */


/* TCP Maximum segment size. */
// default MSS is 536
//#define TCP_MSS                 (500 - 40)	  /* TCP_MSS = (Ethernet MTU - IP header size - TCP header size) */

/* TCP sender buffer space (bytes). */


/*  TCP_SND_QUEUELEN: TCP sender buffer space (pbufs). This must be at least
  as much as (2 * TCP_SND_BUF/TCP_MSS) for things to work. */



/* TCP receive window. */



/* ---------- ICMP options ---------- */
//#define LWIP_ICMP                       1


/* ---------- DHCP options ---------- */
/* Define LWIP_DHCP to 1 if you want DHCP configuration of
   interfaces. DHCP is not implemented in lwIP 0.5.1, however, so
   turning this on does currently not work. */






/* ---------- UDP options ---------- */




/* ---------- Statistics options ---------- */






/*
   --------------------------------------
   ---------- Checksum options ----------
   --------------------------------------
*/

/* 
The STM32F2x7 allows computing and verifying the IP, UDP, TCP and ICMP checksums by hardware:
 - To use this feature let the following define uncommented.
 - To disable it and process by CPU comment the  the checksum.
*/




  /* CHECKSUM_GEN_IP==0: Generate checksums by hardware for outgoing IP packets.*/

  /* CHECKSUM_GEN_UDP==0: Generate checksums by hardware for outgoing UDP packets.*/

  /* CHECKSUM_GEN_TCP==0: Generate checksums by hardware for outgoing TCP packets.*/

  /* CHECKSUM_CHECK_IP==0: Check checksums by hardware for incoming IP packets.*/

  /* CHECKSUM_CHECK_UDP==0: Check checksums by hardware for incoming UDP packets.*/

  /* CHECKSUM_CHECK_TCP==0: Check checksums by hardware for incoming TCP packets.*/
#line 192 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\ConfigFiles\\lwipopts.h"


/*
   ----------------------------------------------
   ---------- Sequential layer options ----------
   ----------------------------------------------
*/
/**
 * LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
 */


/*
   ------------------------------------
   ---------- Socket options ----------
   ------------------------------------
*/
/**
 * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
 */



/*
   -----------------------------------
   ---------- DEBUG options ----------
   -----------------------------------
*/

//#define LWIP_DEBUG                      1

#line 233 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\ConfigFiles\\lwipopts.h"

/*
   ---------------------------------
   ---------- OS options ----------
   ---------------------------------
*/

#line 247 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\ConfigFiles\\lwipopts.h"






/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
#line 46 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/opt.h"
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/debug.h"
/**
  * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
  * All rights reserved. 
  * 
  * Redistribution and use in source and binary forms, with or without modification, 
  * are permitted provided that the following conditions are met:
  *
  * 1. Redistributions of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. The name of the author may not be used to endorse or promote products
  *    derived from this software without specific prior written permission. 
  *
  * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
  * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
  * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
  * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
  * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
  * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
  * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
  * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
  * OF SUCH DAMAGE.
  *
  * This file is part of the lwIP TCP/IP stack.
  * 
  * Author: Adam Dunkels <adam@sics.se>
  *
  */



#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/arch.h"
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */











#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/cc.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */



#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch\\cpu.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */





#line 36 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/cc.h"

typedef unsigned   char    u8_t;
typedef signed     char    s8_t;
typedef unsigned   short   u16_t;
typedef signed     short   s16_t;
typedef unsigned   long    u32_t;
typedef signed     long    s32_t;
typedef u32_t mem_ptr_t;
typedef int sys_prot_t;


#line 54 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/cc.h"





/* define compiler specific symbols */








#line 90 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/cc.h"



#line 44 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/arch.h"

/** Temporary: define format string for size_t if not defined in cc.h */




























#line 131 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/arch.h"



#line 198 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/arch.h"





#line 222 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/arch.h"


extern int errno;








#line 36 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/debug.h"
#line 1 "C:\\iar_ew\\arm\\inc\\c\\stdio.h"
/* stdio.h standard header */
/* Copyright 2003-2010 IAR Systems AB.  */




  #pragma system_include


#line 1 "C:\\iar_ew\\arm\\inc\\c\\ycheck.h"
/* ycheck.h internal checking header file. */
/* Copyright 2005-2010 IAR Systems AB. */

/* Note that there is no include guard for this header. This is intentional. */


  #pragma system_include


/* __INTRINSIC
 *
 * Note: Redefined each time ycheck.h is included, i.e. for each
 * system header, to ensure that intrinsic support could be turned off
 * individually for each file.
 */










/* __AEABI_PORTABILITY_INTERNAL_LEVEL
 *
 * Note: Redefined each time ycheck.h is included, i.e. for each
 * system header, to ensure that ABI support could be turned off/on
 * individually for each file.
 *
 * Possible values for this preprocessor symbol:
 *
 * 0 - ABI portability mode is disabled.
 *
 * 1 - ABI portability mode (version 1) is enabled.
 *
 * All other values are reserved for future use.
 */






#line 67 "C:\\iar_ew\\arm\\inc\\c\\ycheck.h"




/* A definiton for a function of what effects it has.
   NS  = no_state, i.e. it uses no internal or external state. It may write
         to errno though
   NE  = no_state, no_errno,  i.e. it uses no internal or external state,
         not even writing to errno. 
   NRx = no_read(x), i.e. it doesn't read through pointer parameter x.
   NWx = no_write(x), i.e. it doesn't write through pointer parameter x.
*/

#line 99 "C:\\iar_ew\\arm\\inc\\c\\ycheck.h"









#line 11 "C:\\iar_ew\\arm\\inc\\c\\stdio.h"
#line 1 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"
/* yvals.h internal configuration header file. */
/* Copyright 2001-2010 IAR Systems AB. */





  #pragma system_include


#line 1 "C:\\iar_ew\\arm\\inc\\c\\ycheck.h"
/* ycheck.h internal checking header file. */
/* Copyright 2005-2010 IAR Systems AB. */

/* Note that there is no include guard for this header. This is intentional. */


  #pragma system_include


/* __INTRINSIC
 *
 * Note: Redefined each time ycheck.h is included, i.e. for each
 * system header, to ensure that intrinsic support could be turned off
 * individually for each file.
 */










/* __AEABI_PORTABILITY_INTERNAL_LEVEL
 *
 * Note: Redefined each time ycheck.h is included, i.e. for each
 * system header, to ensure that ABI support could be turned off/on
 * individually for each file.
 *
 * Possible values for this preprocessor symbol:
 *
 * 0 - ABI portability mode is disabled.
 *
 * 1 - ABI portability mode (version 1) is enabled.
 *
 * All other values are reserved for future use.
 */






#line 67 "C:\\iar_ew\\arm\\inc\\c\\ycheck.h"

#line 12 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"

                /* Convenience macros */









/* Used to refer to '__aeabi' symbols in the library. */ 


                /* Versions */










/*
 * Support for some C99 or other symbols
 *
 * This setting makes available some macros, functions, etc that are
 * beneficial.
 *
 * Default is to include them.
 *
 * Disabling this in C++ mode will not compile (some C++ functions uses C99
 * functionality).
 */


  /* Default turned on when compiling C++, EC++, or C99. */
#line 59 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"





#line 70 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"

                /* Configuration */
#line 1 "C:\\iar_ew\\arm\\inc\\c\\DLib_Defaults.h"
/***************************************************
 *
 * DLib_Defaults.h is the library configuration manager.
 *
 * Copyright 2003-2010 IAR Systems AB.  
 *
 * This configuration header file performs the following tasks:
 *
 * 1. Includes the configuration header file, defined by _DLIB_CONFIG_FILE,
 *    that sets up a particular runtime environment.
 *
 * 2. Includes the product configuration header file, DLib_Product.h, that
 *    specifies default values for the product and makes sure that the
 *    configuration is valid.
 *
 * 3. Sets up default values for all remaining configuration symbols.
 *
 * This configuration header file, the one defined by _DLIB_CONFIG_FILE, and
 * DLib_Product.h configures how the runtime environment should behave. This
 * includes all system headers and the library itself, i.e. all system headers
 * includes this configuration header file, and the library has been built
 * using this configuration header file.
 *
 ***************************************************
 *
 * DO NOT MODIFY THIS FILE!
 *
 ***************************************************/





  #pragma system_include


/* Include the main configuration header file. */
#line 1 "C:/IAR_EW/arm/inc/c/DLib_Config_Normal.h"
/* Customer-specific DLib configuration. */
/* Copyright (C) 2003 IAR Systems.  All rights reserved. */





  #pragma system_include


/* No changes to the defaults. */

#line 40 "C:\\iar_ew\\arm\\inc\\c\\DLib_Defaults.h"
  /* _DLIB_CONFIG_FILE_STRING is the quoted variant of above */
#line 47 "C:\\iar_ew\\arm\\inc\\c\\DLib_Defaults.h"

/* Include the product specific header file. */
#line 1 "C:\\iar_ew\\arm\\inc\\c\\DLib_Product.h"




   #pragma system_include



/*********************************************************************
*
*       Configuration
*
*********************************************************************/

/* Wide character and multi byte character support in library.
 * This is not allowed to vary over configurations, since math-library
 * is built with wide character support.
 */


/* ARM uses the large implementation of DLib */


/* This ensures that the standard header file "string.h" includes
 * the Arm-specific file "DLib_Product_string.h". */


/* This ensures that the standard header file "fenv.h" includes
 * the Arm-specific file "DLib_Product_fenv.h". */


/* Max buffer used for swap in qsort */




/* Enable system locking  */
#line 45 "C:\\iar_ew\\arm\\inc\\c\\DLib_Product.h"

/* Enable AEABI support */


/* Enable rtmodel for setjump buffer size */


/* Enable parsing of hex floats */






/* Special placement for locale structures when building ropi libraries */




/* CPP-library uses software floatingpoint interface */


/* Use speedy implementation of floats (simple quad). */


/* Configure generic ELF init routines. */
#line 99 "C:\\iar_ew\\arm\\inc\\c\\DLib_Product.h"







#line 51 "C:\\iar_ew\\arm\\inc\\c\\DLib_Defaults.h"



/*
 * The remainder of the file sets up defaults for a number of
 * configuration symbols, each corresponds to a feature in the
 * libary.
 *
 * The value of the symbols should either be 1, if the feature should
 * be supported, or 0 if it shouldn't. (Except where otherwise
 * noted.)
 */


/*
 * Small or Large target
 *
 * This define determines whether the target is large or small. It must be 
 * setup in the DLib_Product header or in the compiler itself.
 *
 * For a small target some functionality in the library will not deliver 
 * the best available results. For instance the _accurate variants will not use
 * the extra precision packet for large arguments.
 * 
 */







/*
 * File handling
 *
 * Determines whether FILE descriptors and related functions exists or not.
 * When this feature is selected, i.e. set to 1, then FILE descriptors and
 * related functions (e.g. fprintf, fopen) exist. All files, even stdin,
 * stdout, and stderr will then be handled with a file system mechanism that
 * buffers files before accessing the lowlevel I/O interface (__open, __read,
 * __write, etc).
 *
 * If not selected, i.e. set to 0, then FILE descriptors and related functions
 * (e.g. fprintf, fopen) does not exist. All functions that normally uses
 * stderr will use stdout instead. Functions that uses stdout and stdin (like
 * printf and scanf) will access the lowlevel I/O interface directly (__open,
 * __read, __write, etc), i.e. there will not be any buffering.
 *
 * The default is not to have support for FILE descriptors.
 */





/*
 * Use static buffers for stdout
 *
 * This setting controls whether the stream stdout uses a static 80 bytes
 * buffer or uses a one byte buffer allocated in the file descriptor. This
 * setting is only applicable if the FILE descriptors are enabled above.
 *
 * Default is to use a static 80 byte buffer.
 */





/*
 * Support of locale interface
 *
 * "Locale" is the system in C that support language- and
 * contry-specific settings for a number of areas, including currency
 * symbols, date and time, and multibyte encodings.
 *
 * This setting determines whether the locale interface exist or not.
 * When this feature is selected, i.e. set to 1, the locale interface exist
 * (setlocale, etc). A number of preselected locales can be activated during
 * runtime. The preselected locales and encodings is choosen by defining any
 * number of _LOCALE_USE_xxx and _ENCODING_USE_xxx symbols. The application
 * will start with the "C" locale choosen. (Single byte encoding is always
 * supported in this mode.)
 *
 *
 * If not selected, i.e. set to 0, the locale interface (setlocale, etc) does
 * not exist. One preselected locale and one preselected encoding is then used
 * directly. That locale can not be changed during runtime. The preselected
 * locale and encoding is choosen by defining at most one of _LOCALE_USE_xxx
 * and at most one of _ENCODING_USE_xxx. The default is to use the "C" locale
 * and the single byte encoding, respectively.
 *
 * The default is not to have support for the locale interface with the "C"
 * locale and the single byte encoding.
 *
 * Supported locales
 * -----------------
 * _LOCALE_USE_C                  C standard locale (the default)
 * _LOCALE_USE_POSIX ISO-8859-1   Posix locale
 * _LOCALE_USE_CS_CZ ISO-8859-2   Czech language locale for Czech Republic
 * _LOCALE_USE_DA_DK ISO-8859-1   Danish language locale for Denmark
 * _LOCALE_USE_DA_EU ISO-8859-15  Danish language locale for Europe
 * _LOCALE_USE_DE_AT ISO-8859-1   German language locale for Austria
 * _LOCALE_USE_DE_BE ISO-8859-1   German language locale for Belgium
 * _LOCALE_USE_DE_CH ISO-8859-1   German language locale for Switzerland
 * _LOCALE_USE_DE_DE ISO-8859-1   German language locale for Germany
 * _LOCALE_USE_DE_EU ISO-8859-15  German language locale for Europe
 * _LOCALE_USE_DE_LU ISO-8859-1   German language locale for Luxemburg
 * _LOCALE_USE_EL_EU ISO-8859-7x  Greek language locale for Europe
 *                                (Euro symbol added)
 * _LOCALE_USE_EL_GR ISO-8859-7   Greek language locale for Greece
 * _LOCALE_USE_EN_AU ISO-8859-1   English language locale for Australia
 * _LOCALE_USE_EN_CA ISO-8859-1   English language locale for Canada
 * _LOCALE_USE_EN_DK ISO_8859-1   English language locale for Denmark
 * _LOCALE_USE_EN_EU ISO-8859-15  English language locale for Europe
 * _LOCALE_USE_EN_GB ISO-8859-1   English language locale for United Kingdom
 * _LOCALE_USE_EN_IE ISO-8859-1   English language locale for Ireland
 * _LOCALE_USE_EN_NZ ISO-8859-1   English language locale for New Zealand
 * _LOCALE_USE_EN_US ISO-8859-1   English language locale for USA
 * _LOCALE_USE_ES_AR ISO-8859-1   Spanish language locale for Argentina
 * _LOCALE_USE_ES_BO ISO-8859-1   Spanish language locale for Bolivia
 * _LOCALE_USE_ES_CL ISO-8859-1   Spanish language locale for Chile
 * _LOCALE_USE_ES_CO ISO-8859-1   Spanish language locale for Colombia
 * _LOCALE_USE_ES_DO ISO-8859-1   Spanish language locale for Dominican Republic
 * _LOCALE_USE_ES_EC ISO-8859-1   Spanish language locale for Equador
 * _LOCALE_USE_ES_ES ISO-8859-1   Spanish language locale for Spain
 * _LOCALE_USE_ES_EU ISO-8859-15  Spanish language locale for Europe
 * _LOCALE_USE_ES_GT ISO-8859-1   Spanish language locale for Guatemala
 * _LOCALE_USE_ES_HN ISO-8859-1   Spanish language locale for Honduras
 * _LOCALE_USE_ES_MX ISO-8859-1   Spanish language locale for Mexico
 * _LOCALE_USE_ES_PA ISO-8859-1   Spanish language locale for Panama
 * _LOCALE_USE_ES_PE ISO-8859-1   Spanish language locale for Peru
 * _LOCALE_USE_ES_PY ISO-8859-1   Spanish language locale for Paraguay
 * _LOCALE_USE_ES_SV ISO-8859-1   Spanish language locale for Salvador
 * _LOCALE_USE_ES_US ISO-8859-1   Spanish language locale for USA
 * _LOCALE_USE_ES_UY ISO-8859-1   Spanish language locale for Uruguay
 * _LOCALE_USE_ES_VE ISO-8859-1   Spanish language locale for Venezuela
 * _LOCALE_USE_ET_EE ISO-8859-1   Estonian language for Estonia
 * _LOCALE_USE_EU_ES ISO-8859-1   Basque language locale for Spain
 * _LOCALE_USE_FI_EU ISO-8859-15  Finnish language locale for Europe
 * _LOCALE_USE_FI_FI ISO-8859-1   Finnish language locale for Finland
 * _LOCALE_USE_FO_FO ISO-8859-1   Faroese language locale for Faroe Islands
 * _LOCALE_USE_FR_BE ISO-8859-1   French language locale for Belgium
 * _LOCALE_USE_FR_CA ISO-8859-1   French language locale for Canada
 * _LOCALE_USE_FR_CH ISO-8859-1   French language locale for Switzerland
 * _LOCALE_USE_FR_EU ISO-8859-15  French language locale for Europe
 * _LOCALE_USE_FR_FR ISO-8859-1   French language locale for France
 * _LOCALE_USE_FR_LU ISO-8859-1   French language locale for Luxemburg
 * _LOCALE_USE_GA_EU ISO-8859-15  Irish language locale for Europe
 * _LOCALE_USE_GA_IE ISO-8859-1   Irish language locale for Ireland
 * _LOCALE_USE_GL_ES ISO-8859-1   Galician language locale for Spain
 * _LOCALE_USE_HR_HR ISO-8859-2   Croatian language locale for Croatia
 * _LOCALE_USE_HU_HU ISO-8859-2   Hungarian language locale for Hungary
 * _LOCALE_USE_ID_ID ISO-8859-1   Indonesian language locale for Indonesia
 * _LOCALE_USE_IS_EU ISO-8859-15  Icelandic language locale for Europe
 * _LOCALE_USE_IS_IS ISO-8859-1   Icelandic language locale for Iceland
 * _LOCALE_USE_IT_EU ISO-8859-15  Italian language locale for Europe
 * _LOCALE_USE_IT_IT ISO-8859-1   Italian language locale for Italy
 * _LOCALE_USE_IW_IL ISO-8859-8   Hebrew language locale for Israel
 * _LOCALE_USE_KL_GL ISO-8859-1   Greenlandic language locale for Greenland
 * _LOCALE_USE_LT_LT   BALTIC     Lithuanian languagelocale for Lithuania
 * _LOCALE_USE_LV_LV   BALTIC     Latvian languagelocale for Latvia
 * _LOCALE_USE_NL_BE ISO-8859-1   Dutch language locale for Belgium
 * _LOCALE_USE_NL_EU ISO-8859-15  Dutch language locale for Europe
 * _LOCALE_USE_NL_NL ISO-8859-9   Dutch language locale for Netherlands
 * _LOCALE_USE_NO_EU ISO-8859-15  Norwegian language locale for Europe
 * _LOCALE_USE_NO_NO ISO-8859-1   Norwegian language locale for Norway
 * _LOCALE_USE_PL_PL ISO-8859-2   Polish language locale for Poland
 * _LOCALE_USE_PT_BR ISO-8859-1   Portugese language locale for Brazil
 * _LOCALE_USE_PT_EU ISO-8859-15  Portugese language locale for Europe
 * _LOCALE_USE_PT_PT ISO-8859-1   Portugese language locale for Portugal
 * _LOCALE_USE_RO_RO ISO-8859-2   Romanian language locale for Romania
 * _LOCALE_USE_RU_RU ISO-8859-5   Russian language locale for Russia
 * _LOCALE_USE_SL_SI ISO-8859-2   Slovenian language locale for Slovenia
 * _LOCALE_USE_SV_EU ISO-8859-15  Swedish language locale for Europe
 * _LOCALE_USE_SV_FI ISO-8859-1   Swedish language locale for Finland
 * _LOCALE_USE_SV_SE ISO-8859-1   Swedish language locale for Sweden
 * _LOCALE_USE_TR_TR ISO-8859-9   Turkish language locale for Turkey
 *
 *  Supported encodings
 *  -------------------
 * n/a                            Single byte (used if no other is defined).
 * _ENCODING_USE_UTF8             UTF8 encoding.
 */






/* We need to have the "C" locale if we have full locale support. */






/*
 * Support of multibytes in printf- and scanf-like functions
 *
 * This is the default value for _DLIB_PRINTF_MULTIBYTE and
 * _DLIB_SCANF_MULTIBYTE. See them for a description.
 *
 * Default is to not have support for multibytes in printf- and scanf-like
 * functions.
 */






/*
 * Throw handling in the EC++ library
 *
 * This setting determines what happens when the EC++ part of the library
 * fails (where a normal C++ library 'throws').
 *
 * The following alternatives exists (setting of the symbol):
 * 0                - The application does nothing, i.e. continues with the
 *                    next statement.
 * 1                - The application terminates by calling the 'abort'
 *                    function directly.
 * <anything else>  - An object of class "exception" is created.  This
 *                    object contains a string describing the problem.
 *                    This string is later emitted on "stderr" before
 *                    the application terminates by calling the 'abort'
 *                    function directly.
 *
 * Default is to do nothing.
 */






/*
 * Hexadecimal floating-point numbers in strtod
 *
 * If selected, i.e. set to 1, strtod supports C99 hexadecimal floating-point
 * numbers. This also enables hexadecimal floating-points in internal functions
 * used for converting strings and wide strings to float, double, and long
 * double.
 *
 * If not selected, i.e. set to 0, C99 hexadecimal floating-point numbers
 * aren't supported.
 *
 * Default is not to support hexadecimal floating-point numbers.
 */






/*
 * Printf configuration symbols.
 *
 * All the configuration symbols described further on controls the behaviour
 * of printf, sprintf, and the other printf variants.
 *
 * The library proves four formatters for printf: 'tiny', 'small',
 * 'large', and 'default'.  The setup in this file controls all except
 * 'tiny'.  Note that both small' and 'large' explicitly removes
 * some features.
 */

/*
 * Handle multibytes in printf
 *
 * This setting controls whether multibytes and wchar_ts are supported in
 * printf. Set to 1 to support them, otherwise set to 0.
 *
 * See _DLIB_FORMATTED_MULTIBYTE for the default setting.
 */





/*
 * Long long formatting in printf
 *
 * This setting controls long long support (%lld) in printf. Set to 1 to
 * support it, otherwise set to 0.

 * Note, if long long should not be supported and 'intmax_t' is larger than
 * an ordinary 'long', then %jd and %jn will not be supported.
 *
 * Default is to support long long formatting.
 */

#line 351 "C:\\iar_ew\\arm\\inc\\c\\DLib_Defaults.h"






/*
 * Floating-point formatting in printf
 *
 * This setting controls whether printf supports floating-point formatting.
 * Set to 1 to support them, otherwise set to 0.
 *
 * Default is to support floating-point formatting.
 */





/*
 * Hexadecimal floating-point formatting in printf
 *
 * This setting controls whether the %a format, i.e. the output of
 * floating-point numbers in the C99 hexadecimal format. Set to 1 to support
 * it, otherwise set to 0.
 *
 * Default is to support %a in printf.
 */





/*
 * Output count formatting in printf
 *
 * This setting controls whether the output count specifier (%n) is supported
 * or not in printf. Set to 1 to support it, otherwise set to 0.
 *
 * Default is to support %n in printf.
 */





/*
 * Support of qualifiers in printf
 *
 * This setting controls whether qualifiers that enlarges the input value
 * [hlLjtz] is supported in printf or not. Set to 1 to support them, otherwise
 * set to 0. See also _DLIB_PRINTF_INT_TYPE_IS_INT and
 * _DLIB_PRINTF_INT_TYPE_IS_LONG.
 *
 * Default is to support [hlLjtz] qualifiers in printf.
 */





/*
 * Support of flags in printf
 *
 * This setting controls whether flags (-+ #0) is supported in printf or not.
 * Set to 1 to support them, otherwise set to 0.
 *
 * Default is to support flags in printf.
 */





/*
 * Support widths and precisions in printf
 *
 * This setting controls whether widths and precisions are supported in printf.
 * Set to 1 to support them, otherwise set to 0.
 *
 * Default is to support widths and precisions in printf.
 */





/*
 * Support of unsigned integer formatting in printf
 *
 * This setting controls whether unsigned integer formatting is supported in
 * printf. Set to 1 to support it, otherwise set to 0.
 *
 * Default is to support unsigned integer formatting in printf.
 */





/*
 * Support of signed integer formatting in printf
 *
 * This setting controls whether signed integer formatting is supported in
 * printf. Set to 1 to support it, otherwise set to 0.
 *
 * Default is to support signed integer formatting in printf.
 */





/*
 * Support of formatting anything larger than int in printf
 *
 * This setting controls if 'int' should be used internally in printf, rather
 * than the largest existing integer type. If 'int' is used, any integer or
 * pointer type formatting use 'int' as internal type even though the
 * formatted type is larger. Set to 1 to use 'int' as internal type, otherwise
 * set to 0.
 *
 * See also next configuration.
 *
 * Default is to internally use largest existing internally type.
 */





/*
 * Support of formatting anything larger than long in printf
 *
 * This setting controls if 'long' should be used internally in printf, rather
 * than the largest existing integer type. If 'long' is used, any integer or
 * pointer type formatting use 'long' as internal type even though the
 * formatted type is larger. Set to 1 to use 'long' as internal type,
 * otherwise set to 0.
 *
 * See also previous configuration.
 *
 * Default is to internally use largest existing internally type.
 */









/*
 * Emit a char a time in printf
 *
 * This setting controls internal output handling. If selected, i.e. set to 1,
 * then printf emits one character at a time, which requires less code but
 * can be slightly slower for some types of output.
 *
 * If not selected, i.e. set to 0, then printf buffers some outputs.
 *
 * Note that it is recommended to either use full file support (see
 * _DLIB_FILE_DESCRIPTOR) or -- for debug output -- use the linker
 * option "-e__write_buffered=__write" to enable buffered I/O rather
 * than deselecting this feature.
 */






/*
 * Scanf configuration symbols.
 *
 * All the configuration symbols described here controls the
 * behaviour of scanf, sscanf, and the other scanf variants.
 *
 * The library proves three formatters for scanf: 'small', 'large',
 * and 'default'.  The setup in this file controls all, however both
 * 'small' and 'large' explicitly removes some features.
 */

/*
 * Handle multibytes in scanf
 *
 * This setting controls whether multibytes and wchar_t:s are supported in
 * scanf. Set to 1 to support them, otherwise set to 0.
 *
 * See _DLIB_FORMATTED_MULTIBYTE for the default.
 */





/*
 * Long long formatting in scanf
 *
 * This setting controls whether scanf supports long long support (%lld). It
 * also controls, if 'intmax_t' is larger than an ordinary 'long', i.e. how
 * the %jd and %jn specifiers behaves. Set to 1 to support them, otherwise set
 * to 0.
 *
 * Default is to support long long formatting in scanf.
 */

#line 566 "C:\\iar_ew\\arm\\inc\\c\\DLib_Defaults.h"





/*
 * Support widths in scanf
 *
 * This controls whether scanf supports widths. Set to 1 to support them,
 * otherwise set to 0.
 *
 * Default is to support widths in scanf.
 */





/*
 * Support qualifiers [hjltzL] in scanf
 *
 * This setting controls whether scanf supports qualifiers [hjltzL] or not. Set
 * to 1 to support them, otherwise set to 0.
 *
 * Default is to support qualifiers in scanf.
 */





/*
 * Support floating-point formatting in scanf
 *
 * This setting controls whether scanf supports floating-point formatting. Set
 * to 1 to support them, otherwise set to 0.
 *
 * Default is to support floating-point formatting in scanf.
 */





/*
 * Support output count formatting (%n)
 *
 * This setting controls whether scanf supports output count formatting (%n).
 * Set to 1 to support it, otherwise set to 0.
 *
 * Default is to support output count formatting in scanf.
 */





/*
 * Support scansets ([]) in scanf
 *
 * This setting controls whether scanf supports scansets ([]) or not. Set to 1
 * to support them, otherwise set to 0.
 *
 * Default is to support scansets in scanf.
 */





/*
 * Support signed integer formatting in scanf
 *
 * This setting controls whether scanf supports signed integer formatting or
 * not. Set to 1 to support them, otherwise set to 0.
 *
 * Default is to support signed integer formatting in scanf.
 */





/*
 * Support unsigned integer formatting in scanf
 *
 * This setting controls whether scanf supports unsigned integer formatting or
 * not. Set to 1 to support them, otherwise set to 0.
 *
 * Default is to support unsigned integer formatting in scanf.
 */





/*
 * Support assignment suppressing [*] in scanf
 *
 * This setting controls whether scanf supports assignment suppressing [*] or
 * not. Set to 1 to support them, otherwise set to 0.
 *
 * Default is to support assignment suppressing in scanf.
 */





/*
 * Handle multibytes in asctime and strftime.
 *
 * This setting controls whether multibytes and wchar_ts are
 * supported.Set to 1 to support them, otherwise set to 0.
 *
 * See _DLIB_FORMATTED_MULTIBYTE for the default setting.
 */





/*
 * True if "qsort" should be implemented using bubble sort.
 *
 * Bubble sort is less efficient than quick sort but requires less RAM
 * and ROM resources.
 */





/*
 * Set Buffert size used in qsort
 */





/*
 * The default "rand" function uses an array of 32 long:s of memory to
 * store the current state.
 *
 * The simple "rand" function uses only a single long. However, the
 * quality of the generated psuedo-random numbers are not as good as
 * the default implementation.
 */





/*
 * Wide character and multi byte character support in library.
 */





/*
 * Set attributes on the function used by the C-SPY debug interface to set a
 * breakpoint in.
 */





/*
 * Support threading in the library
 *
 * 0    No thread support
 * 1    Thread support with a, b, and d.
 * 2    Thread support with a, b, and e.
 * 3    Thread support with all thread-local storage in a dynamically allocated
 *        memory area and a, and b.
 *      a. Lock on heap accesses
 *      b. Optional lock on file accesses (see _DLIB_FILE_OP_LOCKS below)
 *      d. Use an external thread-local storage interface for all the
 *         libraries static and global variables.
 *      e. Static and global variables aren't safe for access from several
 *         threads.
 *
 * Note that if locks are used the following symbols must be defined:
 *
 *   _DLIB_THREAD_LOCK_ONCE_TYPE
 *   _DLIB_THREAD_LOCK_ONCE_MACRO(control_variable, init_function)
 *   _DLIB_THREAD_LOCK_ONCE_TYPE_INIT
 *
 * They will be used to initialize the needed locks only once. TYPE is the
 * type for the static control variable, MACRO is the expression that will be
 * evaluated at each usage of a lock, and INIT is the initializer for the
 * control variable.
 *
 * Note that if thread model 3 is used the symbol _DLIB_TLS_POINTER must be
 * defined. It is a thread local pointer to a dynamic memory area that
 * contains all used TLS variables for the library. Optionally the following
 * symbols can be defined as well (default is to use the default const data
 * and data memories):
 *
 *   _DLIB_TLS_INITIALIZER_MEMORY The memory to place the initializers for the
 *                                TLS memory area
 *   _DLIB_TLS_MEMORY             The memory to use for the TLS memory area. A
 *                                pointer to this memory must be castable to a
 *                                default pointer and back.
 *   _DLIB_TLS_REQUIRE_INIT       Set to 1 to require __cstart_init_tls
 *                                when needed to initialize the TLS data
 *                                segment for the main thread.
 *   _DLIB_TLS_SEGMENT_DATA       The name of the TLS RAM data segment
 *   _DLIB_TLS_SEGMENT_INIT       The name of the used to initialize the
 *                                TLS data segment.
 *
 * See DLib_Threads.h for a description of what interfaces needs to be
 * defined for thread support.
 */





/*
 * Used by products where one runtime library can be used by applications
 * with different data models, in order to reduce the total number of
 * libraries required. Typically, this is used when the pointer types does
 * not change over the data models used, but the placement of data variables
 * or/and constant variables do.
 *
 * If defined, this symbol is typically defined to the memory attribute that
 * is used by the runtime library. The actual define must use a
 * _Pragma("type_attribute = xxx") construct. In the header files, it is used
 * on all statically linked data objects seen by the application.
 */




#line 812 "C:\\iar_ew\\arm\\inc\\c\\DLib_Defaults.h"


/*
 * Turn on support for the Target-specific ABI. The ABI is based on the
 * ARM AEABI. A target, except ARM, may deviate from it.
 */

#line 826 "C:\\iar_ew\\arm\\inc\\c\\DLib_Defaults.h"


  /* Possible AEABI deviations */
#line 836 "C:\\iar_ew\\arm\\inc\\c\\DLib_Defaults.h"

#line 844 "C:\\iar_ew\\arm\\inc\\c\\DLib_Defaults.h"
  /*
   * The "difunc" table contains information about C++ objects that
   * should be dynamically initialized, where each entry in the table
   * represents an initialization function that should be called. When
   * the symbol _DLIB_AEABI_DIFUNC_CONTAINS_OFFSETS is true, each
   * entry in the table is encoded as an offset from the entry
   * location. When false, the entries contain the actual addresses to
   * call.
   */






/*
 * Turn on usage of a pragma to tell the linker the number of elements used
 * in a setjmp jmp_buf.
 */





/*
 * If true, the product supplies a "DLib_Product_string.h" file that
 * is included from "string.h".
 */





/*
 * Determine whether the math fma routines are fast or not.
 */




/*
 * Rtti support.
 */

#line 899 "C:\\iar_ew\\arm\\inc\\c\\DLib_Defaults.h"

/*
 * Use the "pointers to short" or "pointers to long" implementation of 
 * the basic floating point routines (like Dnorm, Dtest, Dscale, and Dunscale).
 */




/*
 * Use 64-bit long long as intermediary type in Dtest, and fabs.
 * Default is to do this if long long is 64-bits.
 */




/*
 * Favor speed versus some size enlargements in floating point functions.
 */




/*
 * Include dlmalloc as an alternative heap manager in product.
 *
 * Typically, an application will use a "malloc" heap manager that is
 * relatively small but not that efficient. An application can
 * optionally use the "dlmalloc" package, which provides a more
 * effective "malloc" heap manager, if it is included in the product
 * and supported by the settings.
 *
 * See the product documentation on how to use it, and whether or not
 * it is included in the product.
 */

  /* size_t/ptrdiff_t must be a 4 bytes unsigned integer. */
#line 943 "C:\\iar_ew\\arm\\inc\\c\\DLib_Defaults.h"





/*
 * Allow the 64-bit time_t interface?
 *
 * Default is yes if long long is 64 bits.
 */

  #pragma language = save 
  #pragma language = extended





  #pragma language = restore






/*
 * Is time_t 64 or 32 bits?
 *
 * Default is 32 bits.
 */




/*
 * Do we include math functions that demands lots of constant bytes?
 * (like erf, erfc, expm1, fma, lgamma, tgamma, and *_accurate)
 *
 */




/*
 * Set this to __weak, if supported.
 *
 */
#line 997 "C:\\iar_ew\\arm\\inc\\c\\DLib_Defaults.h"


/*
 * Deleted options
 *
 */







#line 73 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"











                /* Floating-point */

/*
 * Whenever a floating-point type is equal to another, we try to fold those
 * two types into one. This means that if float == double then we fold float to
 * use double internally. Example sinf(float) will use _Sin(double, uint).
 *
 * _X_FNAME is a redirector for internal support routines. The X can be
 *          D (double), F (float), or L (long double). It redirects by using
 *          another prefix. Example calls to Dtest will be __iar_Dtest,
 *          __iar_FDtest, or __iarLDtest.
 * _X_FUN   is a redirector for functions visible to the customer. As above, the
 *          X can be D, F, or L. It redirects by using another suffix. Example
 *          calls to sin will be sin, sinf, or sinl.
 * _X_TYPE  The type that one type is folded to.
 * _X_PTRCAST is a redirector for a cast operation involving a pointer.
 * _X_CAST  is a redirector for a cast involving the float type.
 *
 * _FLOAT_IS_DOUBLE signals that all internal float routines aren't needed.
 * _LONG_DOUBLE_IS_DOUBLE signals that all internal long double routines
 *                        aren't needed.
 */
#line 147 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"





                /* NAMING PROPERTIES */


/* Has support for fixed point types */




/* Has support for secure functions (printf_s, scanf_s, etc) */
/* Will not compile if enabled */
#line 170 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"

/* Has support for complex C types */




/* If is Embedded C++ language */






/* If is true C++ language */






/* True C++ language setup */
#line 233 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"











                /* NAMESPACE CONTROL */
#line 292 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"









#line 308 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"








#line 1 "C:\\iar_ew\\arm\\inc\\c\\xencoding_limits.h"
/* xencoding_limits.h internal header file */
/* Copyright 2003-2010 IAR Systems AB.  */





  #pragma system_include


#line 1 "C:\\iar_ew\\arm\\inc\\c\\ycheck.h"
/* ycheck.h internal checking header file. */
/* Copyright 2005-2010 IAR Systems AB. */

/* Note that there is no include guard for this header. This is intentional. */


  #pragma system_include


/* __INTRINSIC
 *
 * Note: Redefined each time ycheck.h is included, i.e. for each
 * system header, to ensure that intrinsic support could be turned off
 * individually for each file.
 */










/* __AEABI_PORTABILITY_INTERNAL_LEVEL
 *
 * Note: Redefined each time ycheck.h is included, i.e. for each
 * system header, to ensure that ABI support could be turned off/on
 * individually for each file.
 *
 * Possible values for this preprocessor symbol:
 *
 * 0 - ABI portability mode is disabled.
 *
 * 1 - ABI portability mode (version 1) is enabled.
 *
 * All other values are reserved for future use.
 */






#line 67 "C:\\iar_ew\\arm\\inc\\c\\ycheck.h"

#line 12 "C:\\iar_ew\\arm\\inc\\c\\xencoding_limits.h"
#line 1 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"
/* yvals.h internal configuration header file. */
/* Copyright 2001-2010 IAR Systems AB. */

#line 707 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"

/*
 * Copyright (c) 1992-2009 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V5.04:0576 */
#line 13 "C:\\iar_ew\\arm\\inc\\c\\xencoding_limits.h"

                                /* Multibyte encoding length. */


#line 24 "C:\\iar_ew\\arm\\inc\\c\\xencoding_limits.h"




#line 42 "C:\\iar_ew\\arm\\inc\\c\\xencoding_limits.h"

                                /* Utility macro */














#line 317 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"



                /* FLOATING-POINT PROPERTIES */

                /* float properties */
#line 335 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"

                /* double properties */
#line 360 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"

                /* long double properties */
                /* (must be same as double) */




#line 382 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"


                /* INTEGER PROPERTIES */

                                /* MB_LEN_MAX */







  #pragma language=save
  #pragma language=extended
  typedef long long _Longlong;
  typedef unsigned long long _ULonglong;
  #pragma language=restore
#line 405 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"






  typedef unsigned short int _Wchart;
  typedef unsigned short int _Wintt;


#line 424 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"

#line 432 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"

                /* POINTER PROPERTIES */


typedef signed int  _Ptrdifft;
typedef unsigned int     _Sizet;

/* IAR doesn't support restrict  */


                /* stdarg PROPERTIES */
#line 454 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"
  typedef _VA_LIST __Va_list;



__intrinsic __nounwind void __iar_Atexit(void (*)(void));



  typedef struct
  {       /* state of a multibyte translation */
    unsigned int _Wchar;
    unsigned int _State;
  } _Mbstatet;
#line 477 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"










typedef struct
{       /* file position */

  _Longlong _Off;    /* can be system dependent */



  _Mbstatet _Wstate;
} _Fpost;







                /* THREAD AND LOCALE CONTROL */

#line 1 "C:\\iar_ew\\arm\\inc\\c\\DLib_Threads.h"
/***************************************************
 *
 * DLib_Threads.h is the library threads manager.
 *
 * Copyright 2004-2010 IAR Systems AB.  
 *
 * This configuration header file sets up how the thread support in the library
 * should work.
 *
 ***************************************************
 *
 * DO NOT MODIFY THIS FILE!
 *
 ***************************************************/





  #pragma system_include


/*
 * DLib can support a multithreaded environment. The preprocessor symbol 
 * _DLIB_THREAD_SUPPORT governs the support. It can be 0 (no support), 
 * 1 (currently not supported), 2 (locks only), and 3 (simulated TLS and locks).
 */

/*
 * This header sets the following symbols that governs the rest of the
 * library:
 * ------------------------------------------
 * _DLIB_MULTI_THREAD     0 No thread support
 *                        1 Multithread support
 * _DLIB_GLOBAL_VARIABLES 0 Use external TLS interface for the libraries global
 *                          and static variables
 *                        1 Use a lock for accesses to the locale and no 
 *                          security for accesses to other global and static
 *                          variables in the library
 * _DLIB_FILE_OP_LOCKS    0 No file-atomic locks
 *                        1 File-atomic locks

 * _DLIB_COMPILER_TLS     0 No Thread-Local-Storage support in the compiler
 *                        1 Thread-Local-Storage support in the compiler
 * _DLIB_TLS_QUAL         The TLS qualifier, define only if _COMPILER_TLS == 1
 *
 * _DLIB_THREAD_MACRO_SETUP_DONE Whether to use the standard definitions of
 *                               TLS macros defined in xtls.h or the definitions
 *                               are provided here.
 *                        0 Use default macros
 *                        1 Macros defined for xtls.h
 *
 * _DLIB_THREAD_LOCK_ONCE_TYPE
 *                        type for control variable in once-initialization of 
 *                        locks
 * _DLIB_THREAD_LOCK_ONCE_MACRO(control_variable, init_function)
 *                        expression that will be evaluated at each lock access
 *                        to determine if an initialization must be done
 * _DLIB_THREAD_LOCK_ONCE_TYPE_INIT
 *                        initial value for the control variable
 *
 ****************************************************************************
 * Description
 * -----------
 *
 * If locks are to be used (_DLIB_MULTI_THREAD != 0), the following options
 * has to be used in ilink: 
 *   --redirect __iar_Locksyslock=__iar_Locksyslock_mtx
 *   --redirect __iar_Unlocksyslock=__iar_Unlocksyslock_mtx
 *   --redirect __iar_Lockfilelock=__iar_Lockfilelock_mtx
 *   --redirect __iar_Unlockfilelock=__iar_Unlockfilelock_mtx
 *   --keep     __iar_Locksyslock_mtx
 * and, if C++ is used, also:
 *   --redirect __iar_Initdynamicfilelock=__iar_Initdynamicfilelock_mtx
 *   --redirect __iar_Dstdynamicfilelock=__iar_Dstdynamicfilelock_mtx
 *   --redirect __iar_Lockdynamicfilelock=__iar_Lockdynamicfilelock_mtx
 *   --redirect __iar_Unlockdynamicfilelock=__iar_Unlockdynamicfilelock_mtx
 * Xlink uses similar options (-e and -g). The following lock interface must
 * also be implemented: 
 *   typedef void *__iar_Rmtx;                   // Lock info object
 *
 *   void __iar_system_Mtxinit(__iar_Rmtx *);    // Initialize a system lock
 *   void __iar_system_Mtxdst(__iar_Rmtx *);     // Destroy a system lock
 *   void __iar_system_Mtxlock(__iar_Rmtx *);    // Lock a system lock
 *   void __iar_system_Mtxunlock(__iar_Rmtx *);  // Unlock a system lock
 * The interface handles locks for the heap, the locale, the file system
 * structure, the initialization of statics in functions, etc. 
 *
 * The following lock interface is optional to be implemented:
 *   void __iar_file_Mtxinit(__iar_Rmtx *);    // Initialize a file lock
 *   void __iar_file_Mtxdst(__iar_Rmtx *);     // Destroy a file lock
 *   void __iar_file_Mtxlock(__iar_Rmtx *);    // Lock a file lock
 *   void __iar_file_Mtxunlock(__iar_Rmtx *);  // Unlock a file lock
 * The interface handles locks for each file stream.
 * 
 * These three once-initialization symbols must also be defined, if the 
 * default initialization later on in this file doesn't work (done in 
 * DLib_product.h):
 *
 *   _DLIB_THREAD_LOCK_ONCE_TYPE
 *   _DLIB_THREAD_LOCK_ONCE_MACRO(control_variable, init_function)
 *   _DLIB_THREAD_LOCK_ONCE_TYPE_INIT
 *
 * If an external TLS interface is used, the following must
 * be defined:
 *   typedef int __iar_Tlskey_t;
 *   typedef void (*__iar_Tlsdtor_t)(void *);
 *   int __iar_Tlsalloc(__iar_Tlskey_t *, __iar_Tlsdtor_t); 
 *                                                    // Allocate a TLS element
 *   int __iar_Tlsfree(__iar_Tlskey_t);               // Free a TLS element
 *   int __iar_Tlsset(__iar_Tlskey_t, void *);        // Set a TLS element
 *   void *__iar_Tlsget(__iar_Tlskey_t);              // Get a TLS element
 *
 */

/* We don't have a compiler that supports tls declarations */



#line 157 "C:\\iar_ew\\arm\\inc\\c\\DLib_Threads.h"

  /* Thread support, library supports threaded variables in a user specified
     memory area, locks on heap and on FILE */

  /* See Documentation/ThreadsInternal.html for a description. */





  


#line 176 "C:\\iar_ew\\arm\\inc\\c\\DLib_Threads.h"





#line 187 "C:\\iar_ew\\arm\\inc\\c\\DLib_Threads.h"





  #pragma language=save 
  #pragma language=extended
  __intrinsic __nounwind void __iar_dlib_perthread_initialize(void  *);
  __intrinsic __nounwind void  *__iar_dlib_perthread_allocate(void);
  __intrinsic __nounwind void __iar_dlib_perthread_destroy(void);
  __intrinsic __nounwind void __iar_dlib_perthread_deallocate(void  *);









  #pragma segment = "__DLIB_PERTHREAD" 
  #pragma segment = "__DLIB_PERTHREAD_init" 


























#line 242 "C:\\iar_ew\\arm\\inc\\c\\DLib_Threads.h"

  /* The thread-local variable access function */
  void  *__iar_dlib_perthread_access(void  *);
  #pragma language=restore



























    /* Make sure that each destructor is inserted into _Deallocate_TLS */
  









  /* Internal function declarations. */






  





  
  typedef void *__iar_Rmtx;
  

  
  __intrinsic __nounwind void __iar_system_Mtxinit(__iar_Rmtx *m);
  __intrinsic __nounwind void __iar_system_Mtxdst(__iar_Rmtx *m);
  __intrinsic __nounwind void __iar_system_Mtxlock(__iar_Rmtx *m);
  __intrinsic __nounwind void __iar_system_Mtxunlock(__iar_Rmtx *m);

  __intrinsic __nounwind void __iar_file_Mtxinit(__iar_Rmtx *m);
  __intrinsic __nounwind void __iar_file_Mtxdst(__iar_Rmtx *m);
  __intrinsic __nounwind void __iar_file_Mtxlock(__iar_Rmtx *m);
  __intrinsic __nounwind void __iar_file_Mtxunlock(__iar_Rmtx *m);

  /* Function to destroy the locks. Should be called after atexit and 
     _Close_all. */
  __intrinsic __nounwind void __iar_clearlocks(void);


#line 323 "C:\\iar_ew\\arm\\inc\\c\\DLib_Threads.h"

  





  

#line 341 "C:\\iar_ew\\arm\\inc\\c\\DLib_Threads.h"

  typedef unsigned _Once_t;



  













#line 506 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"

#line 516 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"

                /* THREAD-LOCAL STORAGE */
#line 524 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"


                /* MULTITHREAD PROPERTIES */

  
  
  /* The lock interface for DLib to use. */ 
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Locksyslock_Locale(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Locksyslock_Malloc(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Locksyslock_Stream(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Locksyslock_Debug(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Locksyslock_StaticGuard(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Locksyslock(unsigned int);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlocksyslock_Locale(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlocksyslock_Malloc(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlocksyslock_Stream(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlocksyslock_Debug(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlocksyslock_StaticGuard(void);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlocksyslock(unsigned int);

  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Initdynamicfilelock(__iar_Rmtx *);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Dstdynamicfilelock(__iar_Rmtx *);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Lockdynamicfilelock(__iar_Rmtx *);
  _Pragma("object_attribute = __weak") __intrinsic __nounwind void __iar_Unlockdynamicfilelock(__iar_Rmtx *);
  
  
#line 564 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"

                /* LOCK MACROS */
#line 572 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"

#line 690 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"

                /* MISCELLANEOUS MACROS AND FUNCTIONS*/





#line 705 "C:\\iar_ew\\arm\\inc\\c\\yvals.h"



/*
 * Copyright (c) 1992-2009 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V5.04:0576 */
#line 12 "C:\\iar_ew\\arm\\inc\\c\\stdio.h"
#line 1 "C:\\iar_ew\\arm\\inc\\c\\ysizet.h"
/* ysizet.h internal header file. */
/* Copyright 2003-2010 IAR Systems AB.  */





  #pragma system_include


#line 1 "C:\\iar_ew\\arm\\inc\\c\\ycheck.h"
/* ycheck.h internal checking header file. */
/* Copyright 2005-2010 IAR Systems AB. */

/* Note that there is no include guard for this header. This is intentional. */


  #pragma system_include


/* __INTRINSIC
 *
 * Note: Redefined each time ycheck.h is included, i.e. for each
 * system header, to ensure that intrinsic support could be turned off
 * individually for each file.
 */










/* __AEABI_PORTABILITY_INTERNAL_LEVEL
 *
 * Note: Redefined each time ycheck.h is included, i.e. for each
 * system header, to ensure that ABI support could be turned off/on
 * individually for each file.
 *
 * Possible values for this preprocessor symbol:
 *
 * 0 - ABI portability mode is disabled.
 *
 * 1 - ABI portability mode (version 1) is enabled.
 *
 * All other values are reserved for future use.
 */






#line 67 "C:\\iar_ew\\arm\\inc\\c\\ycheck.h"

#line 12 "C:\\iar_ew\\arm\\inc\\c\\ysizet.h"



                /* type definitions */




typedef _Sizet size_t;




typedef unsigned int __data_size_t;











#line 13 "C:\\iar_ew\\arm\\inc\\c\\stdio.h"
#line 1 "C:\\iar_ew\\arm\\inc\\c\\ystdio.h"
/* ystdio.h internal header */
/* Copyright 2009-2010 IAR Systems AB. */




  #pragma system_include







#line 58 "C:\\iar_ew\\arm\\inc\\c\\ystdio.h"
  
/* File system functions that have debug variants. They are agnostic on 
   whether the library is full or normal. */

__intrinsic __nounwind int remove(const char *);
__intrinsic __nounwind int rename(const char *, const char *);











/*
 * Copyright (c) 1992-2009 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V5.042:0576 */
#line 14 "C:\\iar_ew\\arm\\inc\\c\\stdio.h"



/* Module consistency. */
#pragma rtmodel="__dlib_file_descriptor","0"

                /* macros */








#line 66 "C:\\iar_ew\\arm\\inc\\c\\stdio.h"

#line 88 "C:\\iar_ew\\arm\\inc\\c\\stdio.h"

#line 99 "C:\\iar_ew\\arm\\inc\\c\\stdio.h"










                /* type definitions */
typedef _Fpost fpos_t;

                /* printf and scanf pragma support */
#pragma language=save
#pragma language=extended

#line 125 "C:\\iar_ew\\arm\\inc\\c\\stdio.h"

#line 177 "C:\\iar_ew\\arm\\inc\\c\\stdio.h"


             /* Corresponds to fgets(char *, int, stdin); */
_Pragma("function_effects = no_read(1)")    __intrinsic __nounwind char * __gets(char *, int);
_Pragma("function_effects = no_read(1)")    __intrinsic __nounwind char * gets(char *);
_Pragma("function_effects = no_write(1)")    __intrinsic __nounwind void perror(const char *);
_Pragma("function_effects = no_write(1)")    _Pragma("__printf_args") _Pragma("library_default_requirements _Printf = unknown") __intrinsic __nounwind int printf(const char *, ...);
_Pragma("function_effects = no_write(1)")    __intrinsic __nounwind int puts(const char *);
_Pragma("function_effects = no_write(1)")    _Pragma("__scanf_args") _Pragma("library_default_requirements _Scanf = unknown")  __intrinsic __nounwind int scanf(const char *, ...);
_Pragma("function_effects = no_read(1), no_write(2)") _Pragma("__printf_args") _Pragma("library_default_requirements _Printf = unknown") __intrinsic __nounwind int sprintf(char *, 
                                                 const char *, ...);
_Pragma("function_effects = no_write(1,2)") _Pragma("__scanf_args") _Pragma("library_default_requirements _Scanf = unknown")  __intrinsic __nounwind int sscanf(const char *, 
                                                const char *, ...);
             __intrinsic __nounwind char * tmpnam(char *);
             /* Corresponds to "ungetc(c, stdout)" */
             __intrinsic __nounwind int __ungetchar(int);
_Pragma("function_effects = no_write(1)")    _Pragma("__printf_args") _Pragma("library_default_requirements _Printf = unknown") __intrinsic __nounwind int vprintf(const char *,
                                                 __Va_list);

  _Pragma("function_effects = no_write(1)")    _Pragma("__scanf_args") _Pragma("library_default_requirements _Scanf = unknown")  __intrinsic __nounwind int vscanf(const char *, 
                                                  __Va_list);
  _Pragma("function_effects = no_write(1,2)") _Pragma("__scanf_args") _Pragma("library_default_requirements _Scanf = unknown")  __intrinsic __nounwind int vsscanf(const char *, 
                                                   const char *, 
                                                   __Va_list);

_Pragma("function_effects = no_read(1), no_write(2)")  _Pragma("__printf_args") _Pragma("library_default_requirements _Printf = unknown") __intrinsic __nounwind int vsprintf(char *, 
                                                   const char *,
                                                   __Va_list);
              /* Corresponds to fwrite(p, x, y, stdout); */
_Pragma("function_effects = no_write(1)")      __intrinsic __nounwind size_t __write_array(const void *, size_t, size_t);

  _Pragma("function_effects = no_read(1), no_write(3)") _Pragma("__printf_args") _Pragma("library_default_requirements _Printf = unknown") __intrinsic __nounwind int snprintf(char *, size_t, 
                                                    const char *, ...);
  _Pragma("function_effects = no_read(1), no_write(3)") _Pragma("__printf_args") _Pragma("library_default_requirements _Printf = unknown") __intrinsic __nounwind int vsnprintf(char *, size_t,
                                                     const char *, 
                                                     __Va_list);


              __intrinsic __nounwind int getchar(void);
              __intrinsic __nounwind int putchar(int);



#pragma language=restore

#line 238 "C:\\iar_ew\\arm\\inc\\c\\stdio.h"




#line 292 "C:\\iar_ew\\arm\\inc\\c\\stdio.h"

/*
 * Copyright (c) 1992-2002 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V5.04:0576 */
#line 37 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/debug.h"

/** lower two bits indicate debug level
 * - 0 all
 * - 1 warning
 * - 2 serious
 * - 3 severe
 */
#line 50 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/debug.h"

/** flag for LWIP_DEBUGF to enable that debug message */

/** flag for LWIP_DEBUGF to disable that debug message */


/** flag for LWIP_DEBUGF indicating a tracing message (to follow program flow) */

/** flag for LWIP_DEBUGF indicating a state debug message (to follow module states) */

/** flag for LWIP_DEBUGF indicating newly added code, not thoroughly tested yet */

/** flag for LWIP_DEBUGF to halt after printing this debug message */


#line 71 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/debug.h"

/** if "expression" isn't true, then print "message" and execute "handler" expression */





#line 100 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/debug.h"



#line 47 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/opt.h"

/*
   -----------------------------------------------
   ---------- Platform specific locking ----------
   -----------------------------------------------
*/

/**
 * SYS_LIGHTWEIGHT_PROT==1: if you want inter-task protection for certain
 * critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 */




/** 
 * NO_SYS==1: Provides VERY minimal functionality. Otherwise,
 * use lwIP facilities.
 */




/**
 * MEMCPY: override this if you have a faster implementation at hand than the
 * one included in your C library
 */




/**
 * SMEMCPY: override this with care! Some compilers (e.g. gcc) can inline a
 * call to memcpy() if the length is known at compile time and is small.
 */




/*
   ------------------------------------
   ---------- Memory options ----------
   ------------------------------------
*/
/**
 * MEM_LIBC_MALLOC==1: Use malloc/free/realloc provided by your C-library
 * instead of the lwip internal allocator. Can save code size if you
 * already use it.
 */




/**
* MEMP_MEM_MALLOC==1: Use mem_malloc/mem_free instead of the lwip pool allocator.
* Especially useful with MEM_LIBC_MALLOC but handle with care regarding execution
* speed and usage from interrupts!
*/




/**
 * MEM_ALIGNMENT: should be set to the alignment of the CPU
 *    4 byte alignment -> #define MEM_ALIGNMENT 4
 *    2 byte alignment -> #define MEM_ALIGNMENT 2
 */




/**
 * MEM_SIZE: the size of the heap memory. If the application will send
 * a lot of data that needs to be copied, this should be set high.
 */




/**
 * MEMP_OVERFLOW_CHECK: memp overflow protection reserves a configurable
 * amount of bytes before and after each memp element in every pool and fills
 * it with a prominent default value.
 *    MEMP_OVERFLOW_CHECK == 0 no checking
 *    MEMP_OVERFLOW_CHECK == 1 checks each element when it is freed
 *    MEMP_OVERFLOW_CHECK >= 2 checks each element in every pool every time
 *      memp_malloc() or memp_free() is called (useful but slow!)
 */




/**
 * MEMP_SANITY_CHECK==1: run a sanity check after each memp_free() to make
 * sure that there are no cycles in the linked lists.
 */




/**
 * MEM_USE_POOLS==1: Use an alternative to malloc() by allocating from a set
 * of memory pools of various sizes. When mem_malloc is called, an element of
 * the smallest pool that can provide the length needed is returned.
 * To use this, MEMP_USE_CUSTOM_POOLS also has to be enabled.
 */




/**
 * MEM_USE_POOLS_TRY_BIGGER_POOL==1: if one malloc-pool is empty, try the next
 * bigger pool - WARNING: THIS MIGHT WASTE MEMORY but it can make a system more
 * reliable. */




/**
 * MEMP_USE_CUSTOM_POOLS==1: whether to include a user file lwippools.h
 * that defines additional pools beyond the "standard" ones required
 * by lwIP. If you set this to 1, you must have lwippools.h in your 
 * inlude path somewhere. 
 */




/**
 * Set this to 1 if you want to free PBUF_RAM pbufs (or call mem_free()) from
 * interrupt context (or another context that doesn't allow waiting for a
 * semaphore).
 * If set to 1, mem_malloc will be protected by a semaphore and SYS_ARCH_PROTECT,
 * while mem_free will only use SYS_ARCH_PROTECT. mem_malloc SYS_ARCH_UNPROTECTs
 * with each loop so that mem_free can run.
 *
 * ATTENTION: As you can see from the above description, this leads to dis-/
 * enabling interrupts often, which can be slow! Also, on low memory, mem_malloc
 * can need longer.
 *
 * If you don't want that, at least for NO_SYS=0, you can still use the following
 * functions to enqueue a deallocation call which then runs in the tcpip_thread
 * context:
 * - pbuf_free_callback(p);
 * - mem_free_callback(m);
 */




/*
   ------------------------------------------------
   ---------- Internal Memory Pool Sizes ----------
   ------------------------------------------------
*/
/**
 * MEMP_NUM_PBUF: the number of memp struct pbufs (used for PBUF_ROM and PBUF_REF).
 * If the application sends a lot of data out of ROM (or other static memory),
 * this should be set high.
 */




/**
 * MEMP_NUM_RAW_PCB: Number of raw connection PCBs
 * (requires the LWIP_RAW option)
 */




/**
 * MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
 * per active UDP "connection".
 * (requires the LWIP_UDP option)
 */




/**
 * MEMP_NUM_TCP_PCB: the number of simulatenously active TCP connections.
 * (requires the LWIP_TCP option)
 */




/**
 * MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP connections.
 * (requires the LWIP_TCP option)
 */




/**
 * MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP segments.
 * (requires the LWIP_TCP option)
 */




/**
 * MEMP_NUM_REASSDATA: the number of simultaneously IP packets queued for
 * reassembly (whole packets, not fragments!)
 */




/**
 * MEMP_NUM_ARP_QUEUE: the number of simulateously queued outgoing
 * packets (pbufs) that are waiting for an ARP request (to resolve
 * their destination address) to finish.
 * (requires the ARP_QUEUEING option)
 */




/**
 * MEMP_NUM_IGMP_GROUP: The number of multicast groups whose network interfaces
 * can be members et the same time (one per netif - allsystems group -, plus one
 * per netif membership).
 * (requires the LWIP_IGMP option)
 */




/**
 * MEMP_NUM_SYS_TIMEOUT: the number of simulateously active timeouts.
 * (requires NO_SYS==0)
 */




/**
 * MEMP_NUM_NETBUF: the number of struct netbufs.
 * (only needed if you use the sequential API, like api_lib.c)
 */




/**
 * MEMP_NUM_NETCONN: the number of struct netconns.
 * (only needed if you use the sequential API, like api_lib.c)
 */




/**
 * MEMP_NUM_TCPIP_MSG_API: the number of struct tcpip_msg, which are used
 * for callback/timeout API communication. 
 * (only needed if you use tcpip.c)
 */




/**
 * MEMP_NUM_TCPIP_MSG_INPKT: the number of struct tcpip_msg, which are used
 * for incoming packets. 
 * (only needed if you use tcpip.c)
 */




/**
 * PBUF_POOL_SIZE: the number of buffers in the pbuf pool. 
 */




/*
   ---------------------------------
   ---------- ARP options ----------
   ---------------------------------
*/
/**
 * LWIP_ARP==1: Enable ARP functionality.
 */




/**
 * ARP_TABLE_SIZE: Number of active MAC-IP address pairs cached.
 */




/**
 * ARP_QUEUEING==1: Outgoing packets are queued during hardware address
 * resolution.
 */




/**
 * ETHARP_TRUST_IP_MAC==1: Incoming IP packets cause the ARP table to be
 * updated with the source MAC and IP addresses supplied in the packet.
 * You may want to disable this if you do not trust LAN peers to have the
 * correct addresses, or as a limited approach to attempt to handle
 * spoofing. If disabled, lwIP will need to make a new ARP request if
 * the peer is not already in the ARP table, adding a little latency.
 */




/**
 * ETHARP_SUPPORT_VLAN==1: support receiving ethernet packets with VLAN header.
 * Additionally, you can define ETHARP_VLAN_CHECK to an u16_t VLAN ID to check.
 * If ETHARP_VLAN_CHECK is defined, only VLAN-traffic for this VLAN is accepted.
 * If ETHARP_VLAN_CHECK is not defined, all traffic is accepted.
 */




/*
   --------------------------------
   ---------- IP options ----------
   --------------------------------
*/
/**
 * IP_FORWARD==1: Enables the ability to forward IP packets across network
 * interfaces. If you are going to run lwIP on a device with only one network
 * interface, define this to 0.
 */




/**
 * IP_OPTIONS_ALLOWED: Defines the behavior for IP options.
 *      IP_OPTIONS_ALLOWED==0: All packets with IP options are dropped.
 *      IP_OPTIONS_ALLOWED==1: IP options are allowed (but not parsed).
 */




/**
 * IP_REASSEMBLY==1: Reassemble incoming fragmented IP packets. Note that
 * this option does not affect outgoing packet sizes, which can be controlled
 * via IP_FRAG.
 */




/**
 * IP_FRAG==1: Fragment outgoing IP packets if their size exceeds MTU. Note
 * that this option does not affect incoming packet sizes, which can be
 * controlled via IP_REASSEMBLY.
 */




/**
 * IP_REASS_MAXAGE: Maximum time (in multiples of IP_TMR_INTERVAL - so seconds, normally)
 * a fragmented IP packet waits for all fragments to arrive. If not all fragments arrived
 * in this time, the whole packet is discarded.
 */




/**
 * IP_REASS_MAX_PBUFS: Total maximum amount of pbufs waiting to be reassembled.
 * Since the received pbufs are enqueued, be sure to configure
 * PBUF_POOL_SIZE > IP_REASS_MAX_PBUFS so that the stack is still able to receive
 * packets even if the maximum amount of fragments is enqueued for reassembly!
 */




/**
 * IP_FRAG_USES_STATIC_BUF==1: Use a static MTU-sized buffer for IP
 * fragmentation. Otherwise pbufs are allocated and reference the original
 * packet data to be fragmented.
 */




/**
 * IP_FRAG_MAX_MTU: Assumed max MTU on any interface for IP frag buffer
 * (requires IP_FRAG_USES_STATIC_BUF==1)
 */




/**
 * IP_DEFAULT_TTL: Default value for Time-To-Live used by transport layers.
 */




/**
 * IP_SOF_BROADCAST=1: Use the SOF_BROADCAST field to enable broadcast
 * filter per pcb on udp and raw send operations. To enable broadcast filter
 * on recv operations, you also have to set IP_SOF_BROADCAST_RECV=1.
 */




/**
 * IP_SOF_BROADCAST_RECV (requires IP_SOF_BROADCAST=1) enable the broadcast
 * filter on recv operations.
 */




/*
   ----------------------------------
   ---------- ICMP options ----------
   ----------------------------------
*/
/**
 * LWIP_ICMP==1: Enable ICMP module inside the IP stack.
 * Be careful, disable that make your product non-compliant to RFC1122
 */




/**
 * ICMP_TTL: Default value for Time-To-Live used by ICMP packets.
 */




/**
 * LWIP_BROADCAST_PING==1: respond to broadcast pings (default is unicast only)
 */




/**
 * LWIP_MULTICAST_PING==1: respond to multicast pings (default is unicast only)
 */




/*
   ---------------------------------
   ---------- RAW options ----------
   ---------------------------------
*/
/**
 * LWIP_RAW==1: Enable application layer to hook into the IP layer itself.
 */




/**
 * LWIP_RAW==1: Enable application layer to hook into the IP layer itself.
 */




/*
   ----------------------------------
   ---------- DHCP options ----------
   ----------------------------------
*/
/**
 * LWIP_DHCP==1: Enable DHCP module.
 */




/**
 * DHCP_DOES_ARP_CHECK==1: Do an ARP check on the offered address.
 */




/*
   ------------------------------------
   ---------- AUTOIP options ----------
   ------------------------------------
*/
/**
 * LWIP_AUTOIP==1: Enable AUTOIP module.
 */




/**
 * LWIP_DHCP_AUTOIP_COOP==1: Allow DHCP and AUTOIP to be both enabled on
 * the same interface at the same time.
 */




/**
 * LWIP_DHCP_AUTOIP_COOP_TRIES: Set to the number of DHCP DISCOVER probes
 * that should be sent before falling back on AUTOIP. This can be set
 * as low as 1 to get an AutoIP address very quickly, but you should
 * be prepared to handle a changing IP address when DHCP overrides
 * AutoIP.
 */




/*
   ----------------------------------
   ---------- SNMP options ----------
   ----------------------------------
*/
/**
 * LWIP_SNMP==1: Turn on SNMP module. UDP must be available for SNMP
 * transport.
 */




/**
 * SNMP_CONCURRENT_REQUESTS: Number of concurrent requests the module will
 * allow. At least one request buffer is required. 
 */




/**
 * SNMP_TRAP_DESTINATIONS: Number of trap destinations. At least one trap
 * destination is required
 */




/**
 * SNMP_PRIVATE_MIB: 
 */




/**
 * Only allow SNMP write actions that are 'safe' (e.g. disabeling netifs is not
 * a safe action and disabled when SNMP_SAFE_REQUESTS = 1).
 * Unsafe requests are disabled by default!
 */




/*
   ----------------------------------
   ---------- IGMP options ----------
   ----------------------------------
*/
/**
 * LWIP_IGMP==1: Turn on IGMP module. 
 */




/*
   ----------------------------------
   ---------- DNS options -----------
   ----------------------------------
*/
/**
 * LWIP_DNS==1: Turn on DNS module. UDP must be available for DNS
 * transport.
 */




/** DNS maximum number of entries to maintain locally. */




/** DNS maximum host name length supported in the name table. */




/** The maximum of DNS servers */




/** DNS do a name checking between the query and the response. */




/** DNS use a local buffer if DNS_USES_STATIC_BUF=0, a static one if
    DNS_USES_STATIC_BUF=1, or a dynamic one if DNS_USES_STATIC_BUF=2.
    The buffer will be of size DNS_MSG_SIZE */




/** DNS message max. size. Default value is RFC compliant. */




/** DNS_LOCAL_HOSTLIST: Implements a local host-to-address list. If enabled,
 *  you have to define
 *    #define DNS_LOCAL_HOSTLIST_INIT {{"host1", 0x123}, {"host2", 0x234}}
 *  (an array of structs name/address, where address is an u32_t in network
 *  byte order).
 *
 *  Instead, you can also use an external function:
 *  #define DNS_LOOKUP_LOCAL_EXTERN(x) extern u32_t my_lookup_function(const char *name)
 *  that returns the IP address or INADDR_NONE if not found.
 */




/** If this is turned on, the local host-list can be dynamically changed
 *  at runtime. */




/*
   ---------------------------------
   ---------- UDP options ----------
   ---------------------------------
*/
/**
 * LWIP_UDP==1: Turn on UDP.
 */




/**
 * LWIP_UDPLITE==1: Turn on UDP-Lite. (Requires LWIP_UDP)
 */




/**
 * UDP_TTL: Default Time-To-Live value.
 */




/**
 * LWIP_NETBUF_RECVINFO==1: append destination addr and port to every netbuf.
 */




/*
   ---------------------------------
   ---------- TCP options ----------
   ---------------------------------
*/
/**
 * LWIP_TCP==1: Turn on TCP.
 */




/**
 * TCP_TTL: Default Time-To-Live value.
 */




/**
 * TCP_WND: The size of a TCP window.  This must be at least 
 * (2 * TCP_MSS) for things to work well
 */




/**
 * TCP_MAXRTX: Maximum number of retransmissions of data segments.
 */




/**
 * TCP_SYNMAXRTX: Maximum number of retransmissions of SYN segments.
 */




/**
 * TCP_QUEUE_OOSEQ==1: TCP will queue segments that arrive out of order.
 * Define to 0 if your device is low on memory.
 */




/**
 * TCP_MSS: TCP Maximum segment size. (default is 536, a conservative default,
 * you might want to increase this.)
 * For the receive side, this MSS is advertised to the remote side
 * when opening a connection. For the transmit size, this MSS sets
 * an upper limit on the MSS advertised by the remote host.
 */




/**
 * TCP_CALCULATE_EFF_SEND_MSS: "The maximum size of a segment that TCP really
 * sends, the 'effective send MSS,' MUST be the smaller of the send MSS (which
 * reflects the available reassembly buffer size at the remote host) and the
 * largest size permitted by the IP layer" (RFC 1122)
 * Setting this to 1 enables code that checks TCP_MSS against the MTU of the
 * netif used for a connection and limits the MSS if it would be too big otherwise.
 */





/**
 * TCP_SND_BUF: TCP sender buffer space (bytes). 
 */




/**
 * TCP_SND_QUEUELEN: TCP sender buffer space (pbufs). This must be at least
 * as much as (2 * TCP_SND_BUF/TCP_MSS) for things to work.
 */




/**
 * TCP_SNDLOWAT: TCP writable space (bytes). This must be less than or equal
 * to TCP_SND_BUF. It is the amount of space which must be available in the
 * TCP snd_buf for select to return writable.
 */




/**
 * TCP_LISTEN_BACKLOG: Enable the backlog option for tcp listen pcb.
 */




/**
 * The maximum allowed backlog for TCP listen netconns.
 * This backlog is used unless another is explicitly specified.
 * 0xff is the maximum (u8_t).
 */




/**
 * LWIP_TCP_TIMESTAMPS==1: support the TCP timestamp option.
 */




/**
 * TCP_WND_UPDATE_THRESHOLD: difference in window to trigger an
 * explicit window update
 */




/**
 * LWIP_EVENT_API and LWIP_CALLBACK_API: Only one of these should be set to 1.
 *     LWIP_EVENT_API==1: The user defines lwip_tcp_event() to receive all
 *         events (accept, sent, etc) that happen in the system.
 *     LWIP_CALLBACK_API==1: The PCB callback function is called directly
 *         for the event.
 */
#line 880 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/opt.h"


/*
   ----------------------------------
   ---------- Pbuf options ----------
   ----------------------------------
*/
/**
 * PBUF_LINK_HLEN: the number of bytes that should be allocated for a
 * link level header. The default is 14, the standard value for
 * Ethernet.
 */




/**
 * PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. The default is
 * designed to accomodate single full size TCP frame in one pbuf, including
 * TCP_MSS, IP header, and link header.
 */




/*
   ------------------------------------------------
   ---------- Network Interfaces options ----------
   ------------------------------------------------
*/
/**
 * LWIP_NETIF_HOSTNAME==1: use DHCP_OPTION_HOSTNAME with netif's hostname
 * field.
 */




/**
 * LWIP_NETIF_API==1: Support netif api (in netifapi.c)
 */




/**
 * LWIP_NETIF_STATUS_CALLBACK==1: Support a callback function whenever an interface
 * changes its up/down status (i.e., due to DHCP IP acquistion)
 */




/**
 * LWIP_NETIF_LINK_CALLBACK==1: Support a callback function from an interface
 * whenever the link changes (i.e., link down)
 */




/**
 * LWIP_NETIF_HWADDRHINT==1: Cache link-layer-address hints (e.g. table
 * indices) in struct netif. TCP and UDP can make use of this to prevent
 * scanning the ARP table for every sent packet. While this is faster for big
 * ARP tables or many concurrent connections, it might be counterproductive
 * if you have a tiny ARP table or if there never are concurrent connections.
 */




/**
 * LWIP_NETIF_LOOPBACK==1: Support sending packets with a destination IP
 * address equal to the netif IP address, looping them back up the stack.
 */




/**
 * LWIP_LOOPBACK_MAX_PBUFS: Maximum number of pbufs on queue for loopback
 * sending for each netif (0 = disabled)
 */




/**
 * LWIP_NETIF_LOOPBACK_MULTITHREADING: Indicates whether threading is enabled in
 * the system, as netifs must change how they behave depending on this setting
 * for the LWIP_NETIF_LOOPBACK option to work.
 * Setting this is needed to avoid reentering non-reentrant functions like
 * tcp_input().
 *    LWIP_NETIF_LOOPBACK_MULTITHREADING==1: Indicates that the user is using a
 *       multithreaded environment like tcpip.c. In this case, netif->input()
 *       is called directly.
 *    LWIP_NETIF_LOOPBACK_MULTITHREADING==0: Indicates a polling (or NO_SYS) setup.
 *       The packets are put on a list and netif_poll() must be called in
 *       the main application loop.
 */




/**
 * LWIP_NETIF_TX_SINGLE_PBUF: if this is set to 1, lwIP tries to put all data
 * to be sent into one single pbuf. This is for compatibility with DMA-enabled
 * MACs that do not support scatter-gather.
 * Beware that this might involve CPU-memcpy before transmitting that would not
 * be needed without this flag! Use this only if you need to!
 *
 * @todo: TCP and IP-frag do not work with this, yet:
 */




/*
   ------------------------------------
   ---------- LOOPIF options ----------
   ------------------------------------
*/
/**
 * LWIP_HAVE_LOOPIF==1: Support loop interface (127.0.0.1) and loopif.c
 */




/*
   ------------------------------------
   ---------- SLIPIF options ----------
   ------------------------------------
*/
/**
 * LWIP_HAVE_SLIPIF==1: Support slip interface and slipif.c
 */




/*
   ------------------------------------
   ---------- Thread options ----------
   ------------------------------------
*/
/**
 * TCPIP_THREAD_NAME: The name assigned to the main tcpip thread.
 */




/**
 * TCPIP_THREAD_STACKSIZE: The stack size used by the main tcpip thread.
 * The stack size value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */




/**
 * TCPIP_THREAD_PRIO: The priority assigned to the main tcpip thread.
 * The priority value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */




/**
 * TCPIP_MBOX_SIZE: The mailbox size for the tcpip thread messages
 * The queue size value itself is platform-dependent, but is passed to
 * sys_mbox_new() when tcpip_init is called.
 */




/**
 * SLIPIF_THREAD_NAME: The name assigned to the slipif_loop thread.
 */




/**
 * SLIP_THREAD_STACKSIZE: The stack size used by the slipif_loop thread.
 * The stack size value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */




/**
 * SLIPIF_THREAD_PRIO: The priority assigned to the slipif_loop thread.
 * The priority value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */




/**
 * PPP_THREAD_NAME: The name assigned to the pppMain thread.
 */




/**
 * PPP_THREAD_STACKSIZE: The stack size used by the pppMain thread.
 * The stack size value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */




/**
 * PPP_THREAD_PRIO: The priority assigned to the pppMain thread.
 * The priority value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */




/**
 * DEFAULT_THREAD_NAME: The name assigned to any other lwIP thread.
 */




/**
 * DEFAULT_THREAD_STACKSIZE: The stack size used by any other lwIP thread.
 * The stack size value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */




/**
 * DEFAULT_THREAD_PRIO: The priority assigned to any other lwIP thread.
 * The priority value itself is platform-dependent, but is passed to
 * sys_thread_new() when the thread is created.
 */




/**
 * DEFAULT_RAW_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_RAW. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */




/**
 * DEFAULT_UDP_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_UDP. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */




/**
 * DEFAULT_TCP_RECVMBOX_SIZE: The mailbox size for the incoming packets on a
 * NETCONN_TCP. The queue size value itself is platform-dependent, but is passed
 * to sys_mbox_new() when the recvmbox is created.
 */




/**
 * DEFAULT_ACCEPTMBOX_SIZE: The mailbox size for the incoming connections.
 * The queue size value itself is platform-dependent, but is passed to
 * sys_mbox_new() when the acceptmbox is created.
 */




/*
   ----------------------------------------------
   ---------- Sequential layer options ----------
   ----------------------------------------------
*/
/**
 * LWIP_TCPIP_CORE_LOCKING: (EXPERIMENTAL!)
 * Don't use it if you're not an active lwIP project member
 */




/**
 * LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
 */




/*
   ------------------------------------
   ---------- Socket options ----------
   ------------------------------------
*/
/**
 * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
 */




/**
 * LWIP_COMPAT_SOCKETS==1: Enable BSD-style sockets functions names.
 * (only used if you use sockets.c)
 */




/**
 * LWIP_POSIX_SOCKETS_IO_NAMES==1: Enable POSIX-style sockets functions names.
 * Disable this option if you use a POSIX operating system that uses the same
 * names (read, write & close). (only used if you use sockets.c)
 */




/**
 * LWIP_TCP_KEEPALIVE==1: Enable TCP_KEEPIDLE, TCP_KEEPINTVL and TCP_KEEPCNT
 * options processing. Note that TCP_KEEPIDLE and TCP_KEEPINTVL have to be set
 * in seconds. (does not require sockets.c, and will affect tcp.c)
 */




/**
 * LWIP_SO_RCVTIMEO==1: Enable SO_RCVTIMEO processing.
 */




/**
 * LWIP_SO_RCVBUF==1: Enable SO_RCVBUF processing.
 */




/**
 * If LWIP_SO_RCVBUF is used, this is the default value for recv_bufsize.
 */




/**
 * SO_REUSE==1: Enable SO_REUSEADDR and SO_REUSEPORT options. DO NOT USE!
 */




/*
   ----------------------------------------
   ---------- Statistics options ----------
   ----------------------------------------
*/
/**
 * LWIP_STATS==1: Enable statistics collection in lwip_stats.
 */






/**
 * LWIP_STATS_DISPLAY==1: Compile in the statistics output functions.
 */




/**
 * LINK_STATS==1: Enable link stats.
 */




/**
 * ETHARP_STATS==1: Enable etharp stats.
 */




/**
 * IP_STATS==1: Enable IP stats.
 */




/**
 * IPFRAG_STATS==1: Enable IP fragmentation stats. Default is
 * on if using either frag or reass.
 */




/**
 * ICMP_STATS==1: Enable ICMP stats.
 */




/**
 * IGMP_STATS==1: Enable IGMP stats.
 */




/**
 * UDP_STATS==1: Enable UDP stats. Default is on if
 * UDP enabled, otherwise off.
 */




/**
 * TCP_STATS==1: Enable TCP stats. Default is on if TCP
 * enabled, otherwise off.
 */




/**
 * MEM_STATS==1: Enable mem.c stats.
 */




/**
 * MEMP_STATS==1: Enable memp.c pool stats.
 */




/**
 * SYS_STATS==1: Enable system stats (sem and mbox counts, etc).
 */




#line 1374 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/opt.h"

/*
   ---------------------------------
   ---------- PPP options ----------
   ---------------------------------
*/
/**
 * PPP_SUPPORT==1: Enable PPP.
 */




/**
 * PPPOE_SUPPORT==1: Enable PPP Over Ethernet
 */




/**
 * PPPOS_SUPPORT==1: Enable PPP Over Serial
 */




#line 1538 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/opt.h"

/*
   --------------------------------------
   ---------- Checksum options ----------
   --------------------------------------
*/
/**
 * CHECKSUM_GEN_IP==1: Generate checksums in software for outgoing IP packets.
 */



 
/**
 * CHECKSUM_GEN_UDP==1: Generate checksums in software for outgoing UDP packets.
 */



 
/**
 * CHECKSUM_GEN_TCP==1: Generate checksums in software for outgoing TCP packets.
 */



 
/**
 * CHECKSUM_CHECK_IP==1: Check checksums in software for incoming IP packets.
 */



 
/**
 * CHECKSUM_CHECK_UDP==1: Check checksums in software for incoming UDP packets.
 */




/**
 * CHECKSUM_CHECK_TCP==1: Check checksums in software for incoming TCP packets.
 */




/*
   ---------------------------------------
   ---------- Debugging options ----------
   ---------------------------------------
*/
/**
 * LWIP_DBG_MIN_LEVEL: After masking, the value of the debug is
 * compared against this value. If it is smaller, then debugging
 * messages are written.
 */




/**
 * LWIP_DBG_TYPES_ON: A mask that can be used to globally enable/disable
 * debug messages of certain types.
 */




/**
 * ETHARP_DEBUG: Enable debugging in etharp.c.
 */




/**
 * NETIF_DEBUG: Enable debugging in netif.c.
 */




/**
 * PBUF_DEBUG: Enable debugging in pbuf.c.
 */




/**
 * API_LIB_DEBUG: Enable debugging in api_lib.c.
 */




/**
 * API_MSG_DEBUG: Enable debugging in api_msg.c.
 */




/**
 * SOCKETS_DEBUG: Enable debugging in sockets.c.
 */




/**
 * ICMP_DEBUG: Enable debugging in icmp.c.
 */




/**
 * IGMP_DEBUG: Enable debugging in igmp.c.
 */




/**
 * INET_DEBUG: Enable debugging in inet.c.
 */




/**
 * IP_DEBUG: Enable debugging for IP.
 */




/**
 * IP_REASS_DEBUG: Enable debugging in ip_frag.c for both frag & reass.
 */




/**
 * RAW_DEBUG: Enable debugging in raw.c.
 */




/**
 * MEM_DEBUG: Enable debugging in mem.c.
 */




/**
 * MEMP_DEBUG: Enable debugging in memp.c.
 */




/**
 * SYS_DEBUG: Enable debugging in sys.c.
 */




/**
 * TCP_DEBUG: Enable debugging for TCP.
 */




/**
 * TCP_INPUT_DEBUG: Enable debugging in tcp_in.c for incoming debug.
 */




/**
 * TCP_FR_DEBUG: Enable debugging in tcp_in.c for fast retransmit.
 */




/**
 * TCP_RTO_DEBUG: Enable debugging in TCP for retransmit
 * timeout.
 */




/**
 * TCP_CWND_DEBUG: Enable debugging for TCP congestion window.
 */




/**
 * TCP_WND_DEBUG: Enable debugging in tcp_in.c for window updating.
 */




/**
 * TCP_OUTPUT_DEBUG: Enable debugging in tcp_out.c output functions.
 */




/**
 * TCP_RST_DEBUG: Enable debugging for TCP with the RST message.
 */




/**
 * TCP_QLEN_DEBUG: Enable debugging for TCP queue lengths.
 */




/**
 * UDP_DEBUG: Enable debugging in UDP.
 */




/**
 * TCPIP_DEBUG: Enable debugging in tcpip.c.
 */




/**
 * PPP_DEBUG: Enable debugging for PPP.
 */




/**
 * SLIP_DEBUG: Enable debugging in slipif.c.
 */




/**
 * DHCP_DEBUG: Enable debugging in dhcp.c.
 */




/**
 * AUTOIP_DEBUG: Enable debugging in autoip.c.
 */




/**
 * SNMP_MSG_DEBUG: Enable debugging for SNMP messages.
 */




/**
 * SNMP_MIB_DEBUG: Enable debugging for SNMP MIBs.
 */




/**
 * DNS_DEBUG: Enable debugging for DNS.
 */




#line 43 "..\\src\\api\\api_lib.c"



#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/api.h"
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 **/







#line 1 "C:\\iar_ew\\arm\\inc\\c\\stddef.h"
/* stddef.h standard header */
/* Copyright 2009-2010 IAR Systems AB. */




  #pragma system_include


#line 1 "C:\\iar_ew\\arm\\inc\\c\\ycheck.h"
/* ycheck.h internal checking header file. */
/* Copyright 2005-2010 IAR Systems AB. */

/* Note that there is no include guard for this header. This is intentional. */


  #pragma system_include


/* __INTRINSIC
 *
 * Note: Redefined each time ycheck.h is included, i.e. for each
 * system header, to ensure that intrinsic support could be turned off
 * individually for each file.
 */










/* __AEABI_PORTABILITY_INTERNAL_LEVEL
 *
 * Note: Redefined each time ycheck.h is included, i.e. for each
 * system header, to ensure that ABI support could be turned off/on
 * individually for each file.
 *
 * Possible values for this preprocessor symbol:
 *
 * 0 - ABI portability mode is disabled.
 *
 * 1 - ABI portability mode (version 1) is enabled.
 *
 * All other values are reserved for future use.
 */






#line 67 "C:\\iar_ew\\arm\\inc\\c\\ycheck.h"

#line 11 "C:\\iar_ew\\arm\\inc\\c\\stddef.h"
#line 1 "C:\\iar_ew\\arm\\inc\\c\\ysizet.h"
/* ysizet.h internal header file. */
/* Copyright 2003-2010 IAR Systems AB.  */

#line 30 "C:\\iar_ew\\arm\\inc\\c\\ysizet.h"







#line 13 "C:\\iar_ew\\arm\\inc\\c\\stddef.h"



                /* macros */








                /* type definitions */




  typedef _Ptrdifft ptrdiff_t;




  typedef _Wchart wchar_t;




#line 49 "C:\\iar_ew\\arm\\inc\\c\\stddef.h"


/*
 * Copyright (c) 1992-2009 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V5.04:0576 */
#line 40 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/api.h"

#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/netbuf.h"
/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */



#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/pbuf.h"
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */




#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/err.h"
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */










/** Define LWIP_ERR_T in cc.h if you want to use
 *  a different type for your platform (must be signed). */



 typedef s8_t err_t;


/* Definitions for error constants. */




































#line 38 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/pbuf.h"








typedef enum {
  PBUF_TRANSPORT,
  PBUF_IP,
  PBUF_LINK,
  PBUF_RAW
} pbuf_layer;

typedef enum {
  PBUF_RAM, /* pbuf data is stored in RAM */
  PBUF_ROM, /* pbuf data is stored in ROM */
  PBUF_REF, /* pbuf comes from the pbuf pool */
  PBUF_POOL /* pbuf payload refers to RAM */
} pbuf_type;


/** indicates this packet's data should be immediately passed to the application */


struct pbuf {
  /** next pbuf in singly linked pbuf chain */
  struct pbuf *next;

  /** pointer to the actual data in the buffer */
  void *payload;
  
  /**
   * total length of this buffer and all next buffers in chain
   * belonging to the same packet.
   *
   * For non-queue packet chains this is the invariant:
   * p->tot_len == p->len + (p->next? p->next->tot_len: 0)
   */
  u16_t tot_len;
  
  /** length of this buffer */
  u16_t len;  

  /** pbuf_type as u8_t instead of enum to save space */
  u8_t /*pbuf_type*/ type;

  /** misc flags */
  u8_t flags;

  /**
   * the reference count always equals the number of pointers
   * that refer to this pbuf. This can be pointers from an application,
   * the stack itself, or pbuf->next pointers from a chain.
   */
  u16_t ref;
  
};

/* Initializes the pbuf module. This call is empty for now, but may not be in future. */


struct pbuf *pbuf_alloc(pbuf_layer l, u16_t size, pbuf_type type);
void pbuf_realloc(struct pbuf *p, u16_t size); 
u8_t pbuf_header(struct pbuf *p, s16_t header_size);
void pbuf_ref(struct pbuf *p);
void pbuf_ref_chain(struct pbuf *p);
u8_t pbuf_free(struct pbuf *p);
u8_t pbuf_clen(struct pbuf *p);  
void pbuf_cat(struct pbuf *head, struct pbuf *tail);
void pbuf_chain(struct pbuf *head, struct pbuf *tail);
struct pbuf *pbuf_dechain(struct pbuf *p);
err_t pbuf_copy(struct pbuf *p_to, struct pbuf *p_from);
u16_t pbuf_copy_partial(struct pbuf *p, void *dataptr, u16_t len, u16_t offset);
err_t pbuf_take(struct pbuf *buf, const void *dataptr, u16_t len);
struct pbuf *pbuf_coalesce(struct pbuf *p, pbuf_layer layer);





#line 37 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/netbuf.h"
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/ip_addr.h"
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 **/





#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/inet.h"
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */









/* For compatibility with BSD code */
struct in_addr {
  u32_t s_addr;
};






u32_t inet_addr(const char *cp);
int inet_aton(const char *cp, struct in_addr *addr);
char *inet_ntoa(struct in_addr addr); /* returns ptr to static buffer; not reentrant! */

#line 67 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/inet.h"





#line 91 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/inet.h"
u16_t htons(u16_t x);
u16_t ntohs(u16_t x);
u32_t htonl(u32_t x);
u32_t ntohl(u32_t x);








#line 38 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/ip_addr.h"





#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/bpstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack(1)


#line 45 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/ip_addr.h"


struct ip_addr {
  u32_t addr;
} ;

#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/epstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack()


#line 53 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/ip_addr.h"


/*
 * struct ipaddr2 is used in the definition of the ARP packet format in
 * order to support compilers that don't have structure packing.
 */
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/bpstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack(1)


#line 61 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/ip_addr.h"


struct ip_addr2 {
  u16_t addrw[2];
} ;

#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/epstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack()


#line 69 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/ip_addr.h"


struct netif;

extern const struct ip_addr ip_addr_any;
extern const struct ip_addr ip_addr_broadcast;

/** IP_ADDR_ can be used as a fixed IP address
 *  for the wildcard and the broadcast address
 */



/* Definitions of the bits in an Internet address integer.

   On subnets, host and network parts are found according to
   the subnet mask, not these masks.  */






































/**
 * Determine if two address are on the same network.
 *
 * @arg addr1 IP address 1
 * @arg addr2 IP address 2
 * @arg mask network identifier mask
 * @return !0 if the network identifiers of both address match
 */








u8_t ip_addr_isbroadcast(struct ip_addr *, struct netif *);





#line 156 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/ip_addr.h"

/* These are cast to u16_t, with the intent that they are often arguments
 * to printf using the U16_F format from cc.h. */





/**
 * Same as inet_ntoa() but takes a struct ip_addr*
 */






#line 38 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/netbuf.h"





struct netbuf {
  struct pbuf *p, *ptr;
  struct ip_addr *addr;
  u16_t port;




};

/* Network buffer functions: */
struct netbuf *   netbuf_new      (void);
void              netbuf_delete   (struct netbuf *buf);
void *            netbuf_alloc    (struct netbuf *buf, u16_t size);
void              netbuf_free     (struct netbuf *buf);
err_t             netbuf_ref      (struct netbuf *buf,
           const void *dataptr, u16_t size);
void              netbuf_chain    (struct netbuf *head,
           struct netbuf *tail);

u16_t             netbuf_len      (struct netbuf *buf);
err_t             netbuf_data     (struct netbuf *buf,
           void **dataptr, u16_t *len);
s8_t              netbuf_next     (struct netbuf *buf);
void              netbuf_first    (struct netbuf *buf);


#line 81 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/netbuf.h"





#line 42 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/api.h"
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/sys.h"
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 **/









#line 68 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/sys.h"

/** Return code for timeouts from sys_arch_mbox_fetch and sys_arch_sem_wait */


/* sys_mbox_tryfetch returns SYS_MBOX_EMPTY if appropriate.
 * For now we use the same magic value, but we allow this to change in future.
 */


#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/sys_arch.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */



#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\FreeRTOS.h"
/*
    FreeRTOS V8.1.2 - Copyright (C) 2014 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that has become a de facto standard.             *
     *                                                                       *
     *    Help yourself get started quickly and support the FreeRTOS         *
     *    project by purchasing a FreeRTOS tutorial book, reference          *
     *    manual, or both from: http://www.FreeRTOS.org/Documentation        *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available from the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?"                                     *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/




/*
 * Include the generic headers required for the FreeRTOS port being used.
 */
#line 1 "C:\\iar_ew\\arm\\inc\\c\\stddef.h"
/* stddef.h standard header */
/* Copyright 2009-2010 IAR Systems AB. */
#line 39 "C:\\iar_ew\\arm\\inc\\c\\stddef.h"

#line 49 "C:\\iar_ew\\arm\\inc\\c\\stddef.h"


/*
 * Copyright (c) 1992-2009 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V5.04:0576 */
#line 73 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\FreeRTOS.h"

/*
 * If stdint.h cannot be located then:
 *   + If using GCC ensure the -nostdint options is *not* being used.
 *   + Ensure the project's include path includes the directory in which your
 *     compiler stores stdint.h.
 *   + Set any compiler options necessary for it to support C99, as technically
 *     stdint.h is only mandatory with C99 (FreeRTOS does not require C99 in any
 *     other way).
 *   + The FreeRTOS download includes a simple stdint.h definition that can be
 *     used in cases where none is provided by the compiler.  The files only
 *     contains the typedefs required to build FreeRTOS.  Read the instructions
 *     in FreeRTOS/source/stdint.readme for more information.
 */
#line 1 "C:\\iar_ew\\arm\\inc\\c\\stdint.h"
/* stdint.h standard header */
/* Copyright 2003-2010 IAR Systems AB.  */




  #pragma system_include


#line 1 "C:\\iar_ew\\arm\\inc\\c\\ycheck.h"
/* ycheck.h internal checking header file. */
/* Copyright 2005-2010 IAR Systems AB. */

/* Note that there is no include guard for this header. This is intentional. */


  #pragma system_include


/* __INTRINSIC
 *
 * Note: Redefined each time ycheck.h is included, i.e. for each
 * system header, to ensure that intrinsic support could be turned off
 * individually for each file.
 */










/* __AEABI_PORTABILITY_INTERNAL_LEVEL
 *
 * Note: Redefined each time ycheck.h is included, i.e. for each
 * system header, to ensure that ABI support could be turned off/on
 * individually for each file.
 *
 * Possible values for this preprocessor symbol:
 *
 * 0 - ABI portability mode is disabled.
 *
 * 1 - ABI portability mode (version 1) is enabled.
 *
 * All other values are reserved for future use.
 */






#line 67 "C:\\iar_ew\\arm\\inc\\c\\ycheck.h"

#line 11 "C:\\iar_ew\\arm\\inc\\c\\stdint.h"



/* Fixed size types. These are all optional. */

  typedef signed char   int8_t;
  typedef unsigned char uint8_t;



  typedef signed short int   int16_t;
  typedef unsigned short int uint16_t;



  typedef signed int   int32_t;
  typedef unsigned int uint32_t;



  #pragma language=save
  #pragma language=extended
  typedef signed long long int   int64_t;
  typedef unsigned long long int uint64_t;
  #pragma language=restore


/* Types capable of holding at least a certain number of bits.
   These are not optional for the sizes 8, 16, 32, 64. */
typedef signed char   int_least8_t;
typedef unsigned char uint_least8_t;

typedef signed short int   int_least16_t;
typedef unsigned short int uint_least16_t;

typedef signed int   int_least32_t;
typedef unsigned int uint_least32_t;

/* This isn't really optional, but make it so for now. */

  #pragma language=save
  #pragma language=extended
  typedef signed long long int int_least64_t;
  #pragma language=restore


  #pragma language=save
  #pragma language=extended
  typedef unsigned long long int uint_least64_t;
  #pragma language=restore


/* The fastest type holding at least a certain number of bits.
   These are not optional for the size 8, 16, 32, 64.
   For now, the 64 bit size is optional in IAR compilers. */
typedef signed int   int_fast8_t;
typedef unsigned int uint_fast8_t;

typedef signed int   int_fast16_t;
typedef unsigned int uint_fast16_t;

typedef signed int   int_fast32_t;
typedef unsigned int uint_fast32_t;


  #pragma language=save
  #pragma language=extended
  typedef signed long long int int_fast64_t;
  #pragma language=restore


  #pragma language=save
  #pragma language=extended
  typedef unsigned long long int uint_fast64_t;
  #pragma language=restore


/* The integer type capable of holding the largest number of bits. */
#pragma language=save
#pragma language=extended
typedef signed long long int   intmax_t;
typedef unsigned long long int uintmax_t;
#pragma language=restore

/* An integer type large enough to be able to hold a pointer.
   This is optional, but always supported in IAR compilers. */
typedef signed long int   intptr_t;
typedef unsigned long int uintptr_t;

/* An integer capable of holding a pointer to a specific memory type. */



typedef int __data_intptr_t; typedef unsigned int __data_uintptr_t;


/* Minimum and maximum limits. */






























































































/* Macros expanding to integer constants. */

































#line 258 "C:\\iar_ew\\arm\\inc\\c\\stdint.h"

/*
 * Copyright (c) 1992-2009 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V5.04:0576 */
#line 88 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\FreeRTOS.h"





/* Application specific configuration options. */
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\ConfigFiles\\FreeRTOSConfig.h"
/*
    FreeRTOS V6.0.1 - Copyright (C) 2009 Real Time Engineers Ltd.

    ***************************************************************************
    *                                                                         *
    * If you are:                                                             *
    *                                                                         *
    *    + New to FreeRTOS,                                                   *
    *    + Wanting to learn FreeRTOS or multitasking in general quickly       *
    *    + Looking for basic training,                                        *
    *    + Wanting to improve your FreeRTOS skills and productivity           *
    *                                                                         *
    * then take a look at the FreeRTOS eBook                                  *
    *                                                                         *
    *        "Using the FreeRTOS Real Time Kernel - a Practical Guide"        *
    *                  http://www.FreeRTOS.org/Documentation                  *
    *                                                                         *
    * A pdf reference manual is also available.  Both are usually delivered   *
    * to your inbox within 20 minutes to two hours when purchased between 8am *
    * and 8pm GMT (although please allow up to 24 hours in case of            *
    * exceptional circumstances).  Thank you for your support!                *
    *                                                                         *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public 
    License and the FreeRTOS license exception along with FreeRTOS; if not it 
    can be viewed here: http://www.freertos.org/a00114.html and also obtained 
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/





/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

#line 95 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\ConfigFiles\\FreeRTOSConfig.h"



/* Co-routine definitions. */






//#define configUSE_ALTERNATIVE_API		0

//#define configUSE_RECURSIVE_MUTEXES		1
//#define configQUEUE_REGISTRY_SIZE		1




/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#line 123 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\ConfigFiles\\FreeRTOSConfig.h"


// for debug only 
//#define INCLUDE_xTaskGetIdleTaskHandle 1
//#define INCLUDE_xTaskGetSchedulerState		1
//#define INCLUDE_xTaskGetCurrentTaskHandle	1




/* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255
(lowest) to 0 (1?) (highest). */




/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */


//#ifdef __ICCARM__
//#include <trcHooks.h>
//#endif






#line 95 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\FreeRTOS.h"

/* Basic FreeRTOS definitions. */
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\projdefs.h"
/*
    FreeRTOS V8.1.2 - Copyright (C) 2014 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that has become a de facto standard.             *
     *                                                                       *
     *    Help yourself get started quickly and support the FreeRTOS         *
     *    project by purchasing a FreeRTOS tutorial book, reference          *
     *    manual, or both from: http://www.FreeRTOS.org/Documentation        *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available from the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?"                                     *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/




/*
 * Defines the prototype to which task functions must conform.  Defined in this
 * file to ensure the type is known before portable.h is included.
 */
typedef void (*TaskFunction_t)( void * );

/* Converts a time in milliseconds to a time in ticks. */










/* Error definitions. */








#line 98 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\FreeRTOS.h"

/* Definitions specific to the port being used. */
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\portable.h"
/*
    FreeRTOS V8.1.2 - Copyright (C) 2014 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that has become a de facto standard.             *
     *                                                                       *
     *    Help yourself get started quickly and support the FreeRTOS         *
     *    project by purchasing a FreeRTOS tutorial book, reference          *
     *    manual, or both from: http://www.FreeRTOS.org/Documentation        *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available from the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?"                                     *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/*-----------------------------------------------------------
 * Portable layer API.  Each function must be defined for each port.
 *----------------------------------------------------------*/




/* Include the macro file relevant to the port being used.
NOTE:  The following definitions are *DEPRECATED* as it is preferred to instead
just add the path to the correct portmacro.h header file to the compiler's
include path. */










































































































































































#line 254 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\portable.h"

#line 262 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\portable.h"







#line 276 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\portable.h"






































/* Catch all to ensure portmacro.h is included in the build.  Newer demos
have the path as part of the project options, rather than as relative from
the project location.  If portENTER_CRITICAL() has not been defined then
portmacro.h has not yet been included - as every portmacro.h provides a
portENTER_CRITICAL() definition.  Check the demo application for your demo
to find the path to the correct portmacro.h file. */
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\portable\\IAR\\ARM_CM4F\\portmacro.h"
/*
    FreeRTOS V8.1.2 - Copyright (C) 2014 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that has become a de facto standard.             *
     *                                                                       *
     *    Help yourself get started quickly and support the FreeRTOS         *
     *    project by purchasing a FreeRTOS tutorial book, reference          *
     *    manual, or both from: http://www.FreeRTOS.org/Documentation        *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available from the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?"                                     *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/









/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

/* Type definitions. */
#line 92 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\portable\\IAR\\ARM_CM4F\\portmacro.h"

typedef uint32_t StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;





	typedef uint32_t TickType_t;


/*-----------------------------------------------------------*/

/* Architecture specifics. */



/*-----------------------------------------------------------*/

/* Scheduler utilities. */
extern void vPortYield( void );





/*-----------------------------------------------------------*/

/* Architecture specific optimisations. */




#line 143 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\portable\\IAR\\ARM_CM4F\\portmacro.h"
/*-----------------------------------------------------------*/

/* Critical section management. */
extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );
extern uint32_t ulPortSetInterruptMask( void );
extern void vPortClearInterruptMask( uint32_t ulNewMask );

#line 157 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\portable\\IAR\\ARM_CM4F\\portmacro.h"
/*-----------------------------------------------------------*/

/* Tickless idle/low power functionality. */

	extern void vPortSuppressTicksAndSleep( TickType_t xExpectedIdleTime );



/*-----------------------------------------------------------*/

/* Task function macros as described on the FreeRTOS.org WEB site.  These are
not necessary for to use this port.  They are defined so the common demo files
(which build with all the ports) will build. */


/*-----------------------------------------------------------*/






/* portNOP() is not required by this port. */


/* Suppress warnings that are generated by the IAR tools, but cannot be fixed in
the source code because to do so would cause other compilers to generate
warnings. */
#pragma diag_suppress=Pe191
#pragma diag_suppress=Pa082







#line 322 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\portable.h"






























#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\mpu_wrappers.h"
/*
    FreeRTOS V8.1.2 - Copyright (C) 2014 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that has become a de facto standard.             *
     *                                                                       *
     *    Help yourself get started quickly and support the FreeRTOS         *
     *    project by purchasing a FreeRTOS tutorial book, reference          *
     *    manual, or both from: http://www.FreeRTOS.org/Documentation        *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available from the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?"                                     *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/




/* This file redefines API functions to be called through a wrapper macro, but
only for ports that are using the MPU. */
#line 144 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\mpu_wrappers.h"










#line 353 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\portable.h"

/*
 * Setup the stack of a new task so it is ready to be placed under the
 * scheduler control.  The registers have to be placed on the stack in
 * the order that the port expects to find them.
 *
 */



	StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters ) ;


/* Used by heap_5.c. */
typedef struct HeapRegion
{
	uint8_t *pucStartAddress;
	size_t xSizeInBytes;
} HeapRegion_t;

/* 
 * Used to define multiple heap regions for use by heap_5.c.  This function
 * must be called before any calls to pvPortMalloc() - not creating a task,
 * queue, semaphore, mutex, software timer, event group, etc. will result in
 * pvPortMalloc being called.
 *
 * pxHeapRegions passes in an array of HeapRegion_t structures - each of which
 * defines a region of memory that can be used as the heap.  The array is 
 * terminated by a HeapRegions_t structure that has a size of 0.  The region 
 * with the lowest start address must appear first in the array.
 */
void vPortDefineHeapRegions( const HeapRegion_t * const pxHeapRegions );


/*
 * Map to the memory management routines required for the port.
 */
void *pvPortMalloc( size_t xSize ) ;
void vPortFree( void *pv ) ;
void vPortInitialiseBlocks( void ) ;
size_t xPortGetFreeHeapSize( void ) ;
size_t xPortGetMinimumEverFreeHeapSize( void ) ;

/*
 * Setup the hardware ready for the scheduler to take control.  This generally
 * sets up a tick interrupt and sets timers for the correct tick frequency.
 */
BaseType_t xPortStartScheduler( void ) ;

/*
 * Undo any hardware/ISR setup that was performed by xPortStartScheduler() so
 * the hardware is left in its original condition after the scheduler stops
 * executing.
 */
void vPortEndScheduler( void ) ;

/*
 * The structures and methods of manipulating the MPU are contained within the
 * port layer.
 *
 * Fills the xMPUSettings structure with the memory region information
 * contained in xRegions.
 */











#line 101 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\FreeRTOS.h"

/*
 * Check all the required application specific macros have been defined.
 * These macros are application specific and (as downloaded) are defined
 * within FreeRTOSConfig.h.
 */















































































































































#line 256 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\FreeRTOS.h"

/* The timers module relies on xTaskGetSchedulerState(). */


























































/* Remove any unused trace macros. */

	/* Used to perform any necessary initialisation - for example, open a file
	into which trace is to be written. */




	/* Use to close a trace, for example close a file into which trace has been
	written. */




	/* Called after a task has been selected to run.  pxCurrentTCB holds a pointer
	to the task control block of the selected task. */




	/* Called before stepping the tick count after waking from tickless idle
	sleep. */




	/* Called immediately before entering tickless idle. */




	/* Called when returning to the Idle task after a tickless idle. */




	/* Called before a task has been selected to run.  pxCurrentTCB holds a pointer
	to the task control block of the task being switched out. */




	/* Called when a task attempts to take a mutex that is already held by a
	lower priority task.  pxTCBOfMutexHolder is a pointer to the TCB of the task
	that holds the mutex.  uxInheritedPriority is the priority the mutex holder
	will inherit (the priority of the task that is attempting to obtain the
	muted. */




	/* Called when a task releases a mutex, the holding of which had resulted in
	the task inheriting the priority of a higher priority task.
	pxTCBOfMutexHolder is a pointer to the TCB of the task that is releasing the
	mutex.  uxOriginalPriority is the task's configured (base) priority. */




	/* Task is about to block because it cannot read from a
	queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
	upon which the read was attempted.  pxCurrentTCB points to the TCB of the
	task that attempted the read. */




	/* Task is about to block because it cannot write to a
	queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
	upon which the write was attempted.  pxCurrentTCB points to the TCB of the
	task that attempted the write. */







/* The following event macros are embedded in the kernel API calls. */





























































































































































































































#line 629 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\FreeRTOS.h"





























































































/* Definitions to allow backward compatibility with FreeRTOS versions prior to
V8 if desired. */




#line 744 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\FreeRTOS.h"

	/* Backward compatibility within the scheduler code only - these definitions
	are not really required but are included for completeness. */












#line 36 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/sys_arch.h"
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\task.h"
/*
    FreeRTOS V8.1.2 - Copyright (C) 2014 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that has become a de facto standard.             *
     *                                                                       *
     *    Help yourself get started quickly and support the FreeRTOS         *
     *    project by purchasing a FreeRTOS tutorial book, reference          *
     *    manual, or both from: http://www.FreeRTOS.org/Documentation        *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available from the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?"                                     *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/









#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\list.h"
/*
    FreeRTOS V8.1.2 - Copyright (C) 2014 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that has become a de facto standard.             *
     *                                                                       *
     *    Help yourself get started quickly and support the FreeRTOS         *
     *    project by purchasing a FreeRTOS tutorial book, reference          *
     *    manual, or both from: http://www.FreeRTOS.org/Documentation        *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available from the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?"                                     *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/*
 * This is the list implementation used by the scheduler.  While it is tailored
 * heavily for the schedulers needs, it is also available for use by
 * application code.
 *
 * list_ts can only store pointers to list_item_ts.  Each ListItem_t contains a
 * numeric value (xItemValue).  Most of the time the lists are sorted in
 * descending item value order.
 *
 * Lists are created already containing one list item.  The value of this
 * item is the maximum possible that can be stored, it is therefore always at
 * the end of the list and acts as a marker.  The list member pxHead always
 * points to this marker - even though it is at the tail of the list.  This
 * is because the tail contains a wrap back pointer to the true head of
 * the list.
 *
 * In addition to it's value, each list item contains a pointer to the next
 * item in the list (pxNext), a pointer to the list it is in (pxContainer)
 * and a pointer to back to the object that contains it.  These later two
 * pointers are included for efficiency of list manipulation.  There is
 * effectively a two way link between the object containing the list item and
 * the list item itself.
 *
 *
 * \page ListIntroduction List Implementation
 * \ingroup FreeRTOSIntro
 */





/*
 * The list structure members are modified from within interrupts, and therefore
 * by rights should be declared volatile.  However, they are only modified in a
 * functionally atomic way (within critical sections of with the scheduler
 * suspended) and are either passed by reference into a function or indexed via
 * a volatile variable.  Therefore, in all use cases tested so far, the volatile
 * qualifier can be omitted in order to provide a moderate performance
 * improvement without adversely affecting functional behaviour.  The assembly
 * instructions generated by the IAR, ARM and GCC compilers when the respective
 * compiler's options were set for maximum optimisation has been inspected and
 * deemed to be as intended.  That said, as compiler technology advances, and
 * especially if aggressive cross module optimisation is used (a use case that
 * has not been exercised to any great extend) then it is feasible that the
 * volatile qualifier will be needed for correct optimisation.  It is expected
 * that a compiler removing essential code because, without the volatile
 * qualifier on the list structure members and with aggressive cross module
 * optimisation, the compiler deemed the code unnecessary will result in
 * complete and obvious failure of the scheduler.  If this is ever experienced
 * then the volatile qualifier can be inserted in the relevant places within the
 * list structures by simply defining configLIST_VOLATILE to volatile in
 * FreeRTOSConfig.h (as per the example at the bottom of this comment block).
 * If configLIST_VOLATILE is not defined then the preprocessor directives below
 * will simply #define configLIST_VOLATILE away completely.
 *
 * To use volatile list structure members then add the following line to
 * FreeRTOSConfig.h (without the quotes):
 * "#define configLIST_VOLATILE volatile"
 */







/*
 * Definition of the only type of object that a list can contain.
 */
struct xLIST_ITEM
{
	 TickType_t xItemValue;			/*< The value being listed.  In most cases this is used to sort the list in descending order. */
	struct xLIST_ITEM *  pxNext;		/*< Pointer to the next ListItem_t in the list. */
	struct xLIST_ITEM *  pxPrevious;	/*< Pointer to the previous ListItem_t in the list. */
	void * pvOwner;										/*< Pointer to the object (normally a TCB) that contains the list item.  There is therefore a two way link between the object containing the list item and the list item itself. */
	void *  pvContainer;				/*< Pointer to the list in which this list item is placed (if any). */
};
typedef struct xLIST_ITEM ListItem_t;					/* For some reason lint wants this as two separate definitions. */

struct xMINI_LIST_ITEM
{
	 TickType_t xItemValue;
	struct xLIST_ITEM *  pxNext;
	struct xLIST_ITEM *  pxPrevious;
};
typedef struct xMINI_LIST_ITEM MiniListItem_t;

/*
 * Definition of the type of queue used by the scheduler.
 */
typedef struct xLIST
{
	 UBaseType_t uxNumberOfItems;
	ListItem_t *  pxIndex;		/*< Used to walk through the list.  Points to the last item returned by a call to listGET_OWNER_OF_NEXT_ENTRY (). */
	MiniListItem_t xListEnd;						/*< List item that contains the maximum possible item value meaning it is always at the end of the list and is therefore used as a marker. */
} List_t;

/*
 * Access macro to set the owner of a list item.  The owner of a list item
 * is the object (usually a TCB) that contains the list item.
 *
 * \page listSET_LIST_ITEM_OWNER listSET_LIST_ITEM_OWNER
 * \ingroup LinkedList
 */


/*
 * Access macro to get the owner of a list item.  The owner of a list item
 * is the object (usually a TCB) that contains the list item.
 *
 * \page listSET_LIST_ITEM_OWNER listSET_LIST_ITEM_OWNER
 * \ingroup LinkedList
 */


/*
 * Access macro to set the value of the list item.  In most cases the value is
 * used to sort the list in descending order.
 *
 * \page listSET_LIST_ITEM_VALUE listSET_LIST_ITEM_VALUE
 * \ingroup LinkedList
 */


/*
 * Access macro to retrieve the value of the list item.  The value can
 * represent anything - for example the priority of a task, or the time at
 * which a task should be unblocked.
 *
 * \page listGET_LIST_ITEM_VALUE listGET_LIST_ITEM_VALUE
 * \ingroup LinkedList
 */


/*
 * Access macro to retrieve the value of the list item at the head of a given
 * list.
 *
 * \page listGET_LIST_ITEM_VALUE listGET_LIST_ITEM_VALUE
 * \ingroup LinkedList
 */


/*
 * Return the list item at the head of the list.
 *
 * \page listGET_HEAD_ENTRY listGET_HEAD_ENTRY
 * \ingroup LinkedList
 */


/*
 * Return the list item at the head of the list.
 *
 * \page listGET_NEXT listGET_NEXT
 * \ingroup LinkedList
 */


/*
 * Return the list item that marks the end of the list
 *
 * \page listGET_END_MARKER listGET_END_MARKER
 * \ingroup LinkedList
 */


/*
 * Access macro to determine if a list contains any items.  The macro will
 * only have the value true if the list is empty.
 *
 * \page listLIST_IS_EMPTY listLIST_IS_EMPTY
 * \ingroup LinkedList
 */


/*
 * Access macro to return the number of items in the list.
 */


/*
 * Access function to obtain the owner of the next entry in a list.
 *
 * The list member pxIndex is used to walk through a list.  Calling
 * listGET_OWNER_OF_NEXT_ENTRY increments pxIndex to the next item in the list
 * and returns that entry's pxOwner parameter.  Using multiple calls to this
 * function it is therefore possible to move through every item contained in
 * a list.
 *
 * The pxOwner parameter of a list item is a pointer to the object that owns
 * the list item.  In the scheduler this is normally a task control block.
 * The pxOwner parameter effectively creates a two way link between the list
 * item and its owner.
 *
 * @param pxTCB pxTCB is set to the address of the owner of the next list item.
 * @param pxList The list from which the next item owner is to be returned.
 *
 * \page listGET_OWNER_OF_NEXT_ENTRY listGET_OWNER_OF_NEXT_ENTRY
 * \ingroup LinkedList
 */
#line 280 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\list.h"


/*
 * Access function to obtain the owner of the first entry in a list.  Lists
 * are normally sorted in ascending item value order.
 *
 * This function returns the pxOwner member of the first item in the list.
 * The pxOwner parameter of a list item is a pointer to the object that owns
 * the list item.  In the scheduler this is normally a task control block.
 * The pxOwner parameter effectively creates a two way link between the list
 * item and its owner.
 *
 * @param pxList The list from which the owner of the head item is to be
 * returned.
 *
 * \page listGET_OWNER_OF_HEAD_ENTRY listGET_OWNER_OF_HEAD_ENTRY
 * \ingroup LinkedList
 */


/*
 * Check to see if a list item is within a list.  The list item maintains a
 * "container" pointer that points to the list it is in.  All this macro does
 * is check to see if the container and the list match.
 *
 * @param pxList The list we want to know if the list item is within.
 * @param pxListItem The list item we want to know if is in the list.
 * @return pdTRUE if the list item is in the list, otherwise pdFALSE.
 */


/*
 * Return the list a list item is contained within (referenced from).
 *
 * @param pxListItem The list item being queried.
 * @return A pointer to the List_t object that references the pxListItem
 */


/*
 * This provides a crude means of knowing if a list has been initialised, as
 * pxList->xListEnd.xItemValue is set to portMAX_DELAY by the vListInitialise()
 * function.
 */


/*
 * Must be called before a list is used!  This initialises all the members
 * of the list structure and inserts the xListEnd item into the list as a
 * marker to the back of the list.
 *
 * @param pxList Pointer to the list being initialised.
 *
 * \page vListInitialise vListInitialise
 * \ingroup LinkedList
 */
void vListInitialise( List_t * const pxList );

/*
 * Must be called before a list item is used.  This sets the list container to
 * null so the item does not think that it is already contained in a list.
 *
 * @param pxItem Pointer to the list item being initialised.
 *
 * \page vListInitialiseItem vListInitialiseItem
 * \ingroup LinkedList
 */
void vListInitialiseItem( ListItem_t * const pxItem );

/*
 * Insert a list item into a list.  The item will be inserted into the list in
 * a position determined by its item value (descending item value order).
 *
 * @param pxList The list into which the item is to be inserted.
 *
 * @param pxNewListItem The item that is to be placed in the list.
 *
 * \page vListInsert vListInsert
 * \ingroup LinkedList
 */
void vListInsert( List_t * const pxList, ListItem_t * const pxNewListItem );

/*
 * Insert a list item into a list.  The item will be inserted in a position
 * such that it will be the last item within the list returned by multiple
 * calls to listGET_OWNER_OF_NEXT_ENTRY.
 *
 * The list member pvIndex is used to walk through a list.  Calling
 * listGET_OWNER_OF_NEXT_ENTRY increments pvIndex to the next item in the list.
 * Placing an item in a list using vListInsertEnd effectively places the item
 * in the list position pointed to by pvIndex.  This means that every other
 * item within the list will be returned by listGET_OWNER_OF_NEXT_ENTRY before
 * the pvIndex parameter again points to the item being inserted.
 *
 * @param pxList The list into which the item is to be inserted.
 *
 * @param pxNewListItem The list item to be inserted into the list.
 *
 * \page vListInsertEnd vListInsertEnd
 * \ingroup LinkedList
 */
void vListInsertEnd( List_t * const pxList, ListItem_t * const pxNewListItem );

/*
 * Remove an item from a list.  The list item has a pointer to the list that
 * it is in, so only the list item need be passed into the function.
 *
 * @param uxListRemove The item to be removed.  The item will remove itself from
 * the list pointed to by it's pxContainer parameter.
 *
 * @return The number of items that remain in the list after the list item has
 * been removed.
 *
 * \page uxListRemove uxListRemove
 * \ingroup LinkedList
 */
UBaseType_t uxListRemove( ListItem_t * const pxItemToRemove );







#line 75 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\task.h"





/*-----------------------------------------------------------
 * MACROS AND DEFINITIONS
 *----------------------------------------------------------*/






/**
 * task. h
 *
 * Type by which tasks are referenced.  For example, a call to xTaskCreate
 * returns (via a pointer parameter) an TaskHandle_t variable that can then
 * be used as a parameter to vTaskDelete to delete the task.
 *
 * \defgroup TaskHandle_t TaskHandle_t
 * \ingroup Tasks
 */
typedef void * TaskHandle_t;

/*
 * Defines the prototype to which the application task hook function must
 * conform.
 */
typedef BaseType_t (*TaskHookFunction_t)( void * );

/* Task states returned by eTaskGetState. */
typedef enum
{
	eRunning = 0,	/* A task is querying the state of itself, so must be running. */
	eReady,			/* The task being queried is in a read or pending ready list. */
	eBlocked,		/* The task being queried is in the Blocked state. */
	eSuspended,		/* The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
	eDeleted		/* The task being queried has been deleted, but its TCB has not yet been freed. */
} eTaskState;

/*
 * Used internally only.
 */
typedef struct xTIME_OUT
{
	BaseType_t xOverflowCount;
	TickType_t  xTimeOnEntering;
} TimeOut_t;

/*
 * Defines the memory ranges allocated to the task when an MPU is used.
 */
typedef struct xMEMORY_REGION
{
	void *pvBaseAddress;
	uint32_t ulLengthInBytes;
	uint32_t ulParameters;
} MemoryRegion_t;

/*
 * Parameters required to create an MPU protected task.
 */
typedef struct xTASK_PARAMETERS
{
	TaskFunction_t pvTaskCode;
	const char * const pcName;	/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
	uint16_t usStackDepth;
	void *pvParameters;
	UBaseType_t uxPriority;
	StackType_t *puxStackBuffer;
	MemoryRegion_t xRegions[ 1 ];
} TaskParameters_t;

/* Used with the uxTaskGetSystemState() function to return the state of each task
in the system. */
typedef struct xTASK_STATUS
{
	TaskHandle_t xHandle;			/* The handle of the task to which the rest of the information in the structure relates. */
	const char *pcTaskName;			/* A pointer to the task's name.  This value will be invalid if the task was deleted since the structure was populated! */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
	UBaseType_t xTaskNumber;		/* A number unique to the task. */
	eTaskState eCurrentState;		/* The state in which the task existed when the structure was populated. */
	UBaseType_t uxCurrentPriority;	/* The priority at which the task was running (may be inherited) when the structure was populated. */
	UBaseType_t uxBasePriority;		/* The priority to which the task will return if the task's current priority has been inherited to avoid unbounded priority inversion when obtaining a mutex.  Only valid if configUSE_MUTEXES is defined as 1 in FreeRTOSConfig.h. */
	uint32_t ulRunTimeCounter;		/* The total run time allocated to the task so far, as defined by the run time stats clock.  See http://www.freertos.org/rtos-run-time-stats.html.  Only valid when configGENERATE_RUN_TIME_STATS is defined as 1 in FreeRTOSConfig.h. */
	uint16_t usStackHighWaterMark;	/* The minimum amount of stack space that has remained for the task since the task was created.  The closer this value is to zero the closer the task has come to overflowing its stack. */
} TaskStatus_t;

/* Possible return values for eTaskConfirmSleepModeStatus(). */
typedef enum
{
	eAbortSleep = 0,		/* A task has been made ready or a context switch pended since portSUPPORESS_TICKS_AND_SLEEP() was called - abort entering a sleep mode. */
	eStandardSleep,			/* Enter a sleep mode that will not last any longer than the expected idle time. */
	eNoTasksWaitingTimeout	/* No tasks are waiting for a timeout so it is safe to enter a sleep mode that can only be exited by an external interrupt. */
} eSleepModeStatus;


/**
 * Defines the priority used by the idle task.  This must not be modified.
 *
 * \ingroup TaskUtils
 */


/**
 * task. h
 *
 * Macro for forcing a context switch.
 *
 * \defgroup taskYIELD taskYIELD
 * \ingroup SchedulerControl
 */


/**
 * task. h
 *
 * Macro to mark the start of a critical code region.  Preemptive context
 * switches cannot occur when in a critical region.
 *
 * NOTE: This may alter the stack (depending on the portable implementation)
 * so must be used with care!
 *
 * \defgroup taskENTER_CRITICAL taskENTER_CRITICAL
 * \ingroup SchedulerControl
 */


/**
 * task. h
 *
 * Macro to mark the end of a critical code region.  Preemptive context
 * switches cannot occur when in a critical region.
 *
 * NOTE: This may alter the stack (depending on the portable implementation)
 * so must be used with care!
 *
 * \defgroup taskEXIT_CRITICAL taskEXIT_CRITICAL
 * \ingroup SchedulerControl
 */


/**
 * task. h
 *
 * Macro to disable all maskable interrupts.
 *
 * \defgroup taskDISABLE_INTERRUPTS taskDISABLE_INTERRUPTS
 * \ingroup SchedulerControl
 */


/**
 * task. h
 *
 * Macro to enable microcontroller interrupts.
 *
 * \defgroup taskENABLE_INTERRUPTS taskENABLE_INTERRUPTS
 * \ingroup SchedulerControl
 */


/* Definitions returned by xTaskGetSchedulerState().  taskSCHEDULER_SUSPENDED is
0 to generate more optimal code when configASSERT() is defined as the constant
is used in assert() statements. */





/*-----------------------------------------------------------
 * TASK CREATION API
 *----------------------------------------------------------*/

/**
 * task. h
 *<pre>
 BaseType_t xTaskCreate(
							  TaskFunction_t pvTaskCode,
							  const char * const pcName,
							  uint16_t usStackDepth,
							  void *pvParameters,
							  UBaseType_t uxPriority,
							  TaskHandle_t *pvCreatedTask
						  );</pre>
 *
 * Create a new task and add it to the list of tasks that are ready to run.
 *
 * xTaskCreate() can only be used to create a task that has unrestricted
 * access to the entire microcontroller memory map.  Systems that include MPU
 * support can alternatively create an MPU constrained task using
 * xTaskCreateRestricted().
 *
 * @param pvTaskCode Pointer to the task entry function.  Tasks
 * must be implemented to never return (i.e. continuous loop).
 *
 * @param pcName A descriptive name for the task.  This is mainly used to
 * facilitate debugging.  Max length defined by configMAX_TASK_NAME_LEN - default
 * is 16.
 *
 * @param usStackDepth The size of the task stack specified as the number of
 * variables the stack can hold - not the number of bytes.  For example, if
 * the stack is 16 bits wide and usStackDepth is defined as 100, 200 bytes
 * will be allocated for stack storage.
 *
 * @param pvParameters Pointer that will be used as the parameter for the task
 * being created.
 *
 * @param uxPriority The priority at which the task should run.  Systems that
 * include MPU support can optionally create tasks in a privileged (system)
 * mode by setting bit portPRIVILEGE_BIT of the priority parameter.  For
 * example, to create a privileged task at priority 2 the uxPriority parameter
 * should be set to ( 2 | portPRIVILEGE_BIT ).
 *
 * @param pvCreatedTask Used to pass back a handle by which the created task
 * can be referenced.
 *
 * @return pdPASS if the task was successfully created and added to a ready
 * list, otherwise an error code defined in the file projdefs.h
 *
 * Example usage:
   <pre>
 // Task to be created.
 void vTaskCode( void * pvParameters )
 {
	 for( ;; )
	 {
		 // Task code goes here.
	 }
 }

 // Function that creates a task.
 void vOtherFunction( void )
 {
 static uint8_t ucParameterToPass;
 TaskHandle_t xHandle = NULL;

	 // Create the task, storing the handle.  Note that the passed parameter ucParameterToPass
	 // must exist for the lifetime of the task, so in this case is declared static.  If it was just an
	 // an automatic stack variable it might no longer exist, or at least have been corrupted, by the time
	 // the new task attempts to access it.
	 xTaskCreate( vTaskCode, "NAME", STACK_SIZE, &ucParameterToPass, tskIDLE_PRIORITY, &xHandle );
     configASSERT( xHandle );

	 // Use the handle to delete the task.
     if( xHandle != NULL )
     {
	     vTaskDelete( xHandle );
     }
 }
   </pre>
 * \defgroup xTaskCreate xTaskCreate
 * \ingroup Tasks
 */


/**
 * task. h
 *<pre>
 BaseType_t xTaskCreateRestricted( TaskParameters_t *pxTaskDefinition, TaskHandle_t *pxCreatedTask );</pre>
 *
 * xTaskCreateRestricted() should only be used in systems that include an MPU
 * implementation.
 *
 * Create a new task and add it to the list of tasks that are ready to run.
 * The function parameters define the memory regions and associated access
 * permissions allocated to the task.
 *
 * @param pxTaskDefinition Pointer to a structure that contains a member
 * for each of the normal xTaskCreate() parameters (see the xTaskCreate() API
 * documentation) plus an optional stack buffer and the memory region
 * definitions.
 *
 * @param pxCreatedTask Used to pass back a handle by which the created task
 * can be referenced.
 *
 * @return pdPASS if the task was successfully created and added to a ready
 * list, otherwise an error code defined in the file projdefs.h
 *
 * Example usage:
   <pre>
// Create an TaskParameters_t structure that defines the task to be created.
static const TaskParameters_t xCheckTaskParameters =
{
	vATask,		// pvTaskCode - the function that implements the task.
	"ATask",	// pcName - just a text name for the task to assist debugging.
	100,		// usStackDepth	- the stack size DEFINED IN WORDS.
	NULL,		// pvParameters - passed into the task function as the function parameters.
	( 1UL | portPRIVILEGE_BIT ),// uxPriority - task priority, set the portPRIVILEGE_BIT if the task should run in a privileged state.
	cStackBuffer,// puxStackBuffer - the buffer to be used as the task stack.

	// xRegions - Allocate up to three separate memory regions for access by
	// the task, with appropriate access permissions.  Different processors have
	// different memory alignment requirements - refer to the FreeRTOS documentation
	// for full information.
	{
		// Base address					Length	Parameters
        { cReadWriteArray,				32,		portMPU_REGION_READ_WRITE },
        { cReadOnlyArray,				32,		portMPU_REGION_READ_ONLY },
        { cPrivilegedOnlyAccessArray,	128,	portMPU_REGION_PRIVILEGED_READ_WRITE }
	}
};

int main( void )
{
TaskHandle_t xHandle;

	// Create a task from the const structure defined above.  The task handle
	// is requested (the second parameter is not NULL) but in this case just for
	// demonstration purposes as its not actually used.
	xTaskCreateRestricted( &xRegTest1Parameters, &xHandle );

	// Start the scheduler.
	vTaskStartScheduler();

	// Will only get here if there was insufficient memory to create the idle
	// and/or timer task.
	for( ;; );
}
   </pre>
 * \defgroup xTaskCreateRestricted xTaskCreateRestricted
 * \ingroup Tasks
 */


/**
 * task. h
 *<pre>
 void vTaskAllocateMPURegions( TaskHandle_t xTask, const MemoryRegion_t * const pxRegions );</pre>
 *
 * Memory regions are assigned to a restricted task when the task is created by
 * a call to xTaskCreateRestricted().  These regions can be redefined using
 * vTaskAllocateMPURegions().
 *
 * @param xTask The handle of the task being updated.
 *
 * @param xRegions A pointer to an MemoryRegion_t structure that contains the
 * new memory region definitions.
 *
 * Example usage:
   <pre>
// Define an array of MemoryRegion_t structures that configures an MPU region
// allowing read/write access for 1024 bytes starting at the beginning of the
// ucOneKByte array.  The other two of the maximum 3 definable regions are
// unused so set to zero.
static const MemoryRegion_t xAltRegions[ portNUM_CONFIGURABLE_REGIONS ] =
{
	// Base address		Length		Parameters
	{ ucOneKByte,		1024,		portMPU_REGION_READ_WRITE },
	{ 0,				0,			0 },
	{ 0,				0,			0 }
};

void vATask( void *pvParameters )
{
	// This task was created such that it has access to certain regions of
	// memory as defined by the MPU configuration.  At some point it is
	// desired that these MPU regions are replaced with that defined in the
	// xAltRegions const struct above.  Use a call to vTaskAllocateMPURegions()
	// for this purpose.  NULL is used as the task handle to indicate that this
	// function should modify the MPU regions of the calling task.
	vTaskAllocateMPURegions( NULL, xAltRegions );

	// Now the task can continue its function, but from this point on can only
	// access its stack and the ucOneKByte array (unless any other statically
	// defined or shared regions have been declared elsewhere).
}
   </pre>
 * \defgroup xTaskCreateRestricted xTaskCreateRestricted
 * \ingroup Tasks
 */
void vTaskAllocateMPURegions( TaskHandle_t xTask, const MemoryRegion_t * const pxRegions ) ;

/**
 * task. h
 * <pre>void vTaskDelete( TaskHandle_t xTask );</pre>
 *
 * INCLUDE_vTaskDelete must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Remove a task from the RTOS real time kernel's management.  The task being
 * deleted will be removed from all ready, blocked, suspended and event lists.
 *
 * NOTE:  The idle task is responsible for freeing the kernel allocated
 * memory from tasks that have been deleted.  It is therefore important that
 * the idle task is not starved of microcontroller processing time if your
 * application makes any calls to vTaskDelete ().  Memory allocated by the
 * task code is not automatically freed, and should be freed before the task
 * is deleted.
 *
 * See the demo application file death.c for sample code that utilises
 * vTaskDelete ().
 *
 * @param xTask The handle of the task to be deleted.  Passing NULL will
 * cause the calling task to be deleted.
 *
 * Example usage:
   <pre>
 void vOtherFunction( void )
 {
 TaskHandle_t xHandle;

	 // Create the task, storing the handle.
	 xTaskCreate( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY, &xHandle );

	 // Use the handle to delete the task.
	 vTaskDelete( xHandle );
 }
   </pre>
 * \defgroup vTaskDelete vTaskDelete
 * \ingroup Tasks
 */
void vTaskDelete( TaskHandle_t xTaskToDelete ) ;

/*-----------------------------------------------------------
 * TASK CONTROL API
 *----------------------------------------------------------*/

/**
 * task. h
 * <pre>void vTaskDelay( const TickType_t xTicksToDelay );</pre>
 *
 * Delay a task for a given number of ticks.  The actual time that the
 * task remains blocked depends on the tick rate.  The constant
 * portTICK_PERIOD_MS can be used to calculate real time from the tick
 * rate - with the resolution of one tick period.
 *
 * INCLUDE_vTaskDelay must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 *
 * vTaskDelay() specifies a time at which the task wishes to unblock relative to
 * the time at which vTaskDelay() is called.  For example, specifying a block
 * period of 100 ticks will cause the task to unblock 100 ticks after
 * vTaskDelay() is called.  vTaskDelay() does not therefore provide a good method
 * of controlling the frequency of a periodic task as the path taken through the
 * code, as well as other task and interrupt activity, will effect the frequency
 * at which vTaskDelay() gets called and therefore the time at which the task
 * next executes.  See vTaskDelayUntil() for an alternative API function designed
 * to facilitate fixed frequency execution.  It does this by specifying an
 * absolute time (rather than a relative time) at which the calling task should
 * unblock.
 *
 * @param xTicksToDelay The amount of time, in tick periods, that
 * the calling task should block.
 *
 * Example usage:

 void vTaskFunction( void * pvParameters )
 {
 // Block for 500ms.
 const TickType_t xDelay = 500 / portTICK_PERIOD_MS;

	 for( ;; )
	 {
		 // Simply toggle the LED every 500ms, blocking between each toggle.
		 vToggleLED();
		 vTaskDelay( xDelay );
	 }
 }

 * \defgroup vTaskDelay vTaskDelay
 * \ingroup TaskCtrl
 */
void vTaskDelay( const TickType_t xTicksToDelay ) ;

/**
 * task. h
 * <pre>void vTaskDelayUntil( TickType_t *pxPreviousWakeTime, const TickType_t xTimeIncrement );</pre>
 *
 * INCLUDE_vTaskDelayUntil must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Delay a task until a specified time.  This function can be used by periodic
 * tasks to ensure a constant execution frequency.
 *
 * This function differs from vTaskDelay () in one important aspect:  vTaskDelay () will
 * cause a task to block for the specified number of ticks from the time vTaskDelay () is
 * called.  It is therefore difficult to use vTaskDelay () by itself to generate a fixed
 * execution frequency as the time between a task starting to execute and that task
 * calling vTaskDelay () may not be fixed [the task may take a different path though the
 * code between calls, or may get interrupted or preempted a different number of times
 * each time it executes].
 *
 * Whereas vTaskDelay () specifies a wake time relative to the time at which the function
 * is called, vTaskDelayUntil () specifies the absolute (exact) time at which it wishes to
 * unblock.
 *
 * The constant portTICK_PERIOD_MS can be used to calculate real time from the tick
 * rate - with the resolution of one tick period.
 *
 * @param pxPreviousWakeTime Pointer to a variable that holds the time at which the
 * task was last unblocked.  The variable must be initialised with the current time
 * prior to its first use (see the example below).  Following this the variable is
 * automatically updated within vTaskDelayUntil ().
 *
 * @param xTimeIncrement The cycle time period.  The task will be unblocked at
 * time *pxPreviousWakeTime + xTimeIncrement.  Calling vTaskDelayUntil with the
 * same xTimeIncrement parameter value will cause the task to execute with
 * a fixed interface period.
 *
 * Example usage:
   <pre>
 // Perform an action every 10 ticks.
 void vTaskFunction( void * pvParameters )
 {
 TickType_t xLastWakeTime;
 const TickType_t xFrequency = 10;

	 // Initialise the xLastWakeTime variable with the current time.
	 xLastWakeTime = xTaskGetTickCount ();
	 for( ;; )
	 {
		 // Wait for the next cycle.
		 vTaskDelayUntil( &xLastWakeTime, xFrequency );

		 // Perform action here.
	 }
 }
   </pre>
 * \defgroup vTaskDelayUntil vTaskDelayUntil
 * \ingroup TaskCtrl
 */
void vTaskDelayUntil( TickType_t * const pxPreviousWakeTime, const TickType_t xTimeIncrement ) ;

/**
 * task. h
 * <pre>UBaseType_t uxTaskPriorityGet( TaskHandle_t xTask );</pre>
 *
 * INCLUDE_uxTaskPriorityGet must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Obtain the priority of any task.
 *
 * @param xTask Handle of the task to be queried.  Passing a NULL
 * handle results in the priority of the calling task being returned.
 *
 * @return The priority of xTask.
 *
 * Example usage:
   <pre>
 void vAFunction( void )
 {
 TaskHandle_t xHandle;

	 // Create a task, storing the handle.
	 xTaskCreate( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY, &xHandle );

	 // ...

	 // Use the handle to obtain the priority of the created task.
	 // It was created with tskIDLE_PRIORITY, but may have changed
	 // it itself.
	 if( uxTaskPriorityGet( xHandle ) != tskIDLE_PRIORITY )
	 {
		 // The task has changed it's priority.
	 }

	 // ...

	 // Is our priority higher than the created task?
	 if( uxTaskPriorityGet( xHandle ) < uxTaskPriorityGet( NULL ) )
	 {
		 // Our priority (obtained using NULL handle) is higher.
	 }
 }
   </pre>
 * \defgroup uxTaskPriorityGet uxTaskPriorityGet
 * \ingroup TaskCtrl
 */
UBaseType_t uxTaskPriorityGet( TaskHandle_t xTask ) ;

/**
 * task. h
 * <pre>eTaskState eTaskGetState( TaskHandle_t xTask );</pre>
 *
 * INCLUDE_eTaskGetState must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Obtain the state of any task.  States are encoded by the eTaskState
 * enumerated type.
 *
 * @param xTask Handle of the task to be queried.
 *
 * @return The state of xTask at the time the function was called.  Note the
 * state of the task might change between the function being called, and the
 * functions return value being tested by the calling task.
 */
eTaskState eTaskGetState( TaskHandle_t xTask ) ;

/**
 * task. h
 * <pre>void vTaskPrioritySet( TaskHandle_t xTask, UBaseType_t uxNewPriority );</pre>
 *
 * INCLUDE_vTaskPrioritySet must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Set the priority of any task.
 *
 * A context switch will occur before the function returns if the priority
 * being set is higher than the currently executing task.
 *
 * @param xTask Handle to the task for which the priority is being set.
 * Passing a NULL handle results in the priority of the calling task being set.
 *
 * @param uxNewPriority The priority to which the task will be set.
 *
 * Example usage:
   <pre>
 void vAFunction( void )
 {
 TaskHandle_t xHandle;

	 // Create a task, storing the handle.
	 xTaskCreate( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY, &xHandle );

	 // ...

	 // Use the handle to raise the priority of the created task.
	 vTaskPrioritySet( xHandle, tskIDLE_PRIORITY + 1 );

	 // ...

	 // Use a NULL handle to raise our priority to the same value.
	 vTaskPrioritySet( NULL, tskIDLE_PRIORITY + 1 );
 }
   </pre>
 * \defgroup vTaskPrioritySet vTaskPrioritySet
 * \ingroup TaskCtrl
 */
void vTaskPrioritySet( TaskHandle_t xTask, UBaseType_t uxNewPriority ) ;

/**
 * task. h
 * <pre>void vTaskSuspend( TaskHandle_t xTaskToSuspend );</pre>
 *
 * INCLUDE_vTaskSuspend must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Suspend any task.  When suspended a task will never get any microcontroller
 * processing time, no matter what its priority.
 *
 * Calls to vTaskSuspend are not accumulative -
 * i.e. calling vTaskSuspend () twice on the same task still only requires one
 * call to vTaskResume () to ready the suspended task.
 *
 * @param xTaskToSuspend Handle to the task being suspended.  Passing a NULL
 * handle will cause the calling task to be suspended.
 *
 * Example usage:
   <pre>
 void vAFunction( void )
 {
 TaskHandle_t xHandle;

	 // Create a task, storing the handle.
	 xTaskCreate( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY, &xHandle );

	 // ...

	 // Use the handle to suspend the created task.
	 vTaskSuspend( xHandle );

	 // ...

	 // The created task will not run during this period, unless
	 // another task calls vTaskResume( xHandle ).

	 //...


	 // Suspend ourselves.
	 vTaskSuspend( NULL );

	 // We cannot get here unless another task calls vTaskResume
	 // with our handle as the parameter.
 }
   </pre>
 * \defgroup vTaskSuspend vTaskSuspend
 * \ingroup TaskCtrl
 */
void vTaskSuspend( TaskHandle_t xTaskToSuspend ) ;

/**
 * task. h
 * <pre>void vTaskResume( TaskHandle_t xTaskToResume );</pre>
 *
 * INCLUDE_vTaskSuspend must be defined as 1 for this function to be available.
 * See the configuration section for more information.
 *
 * Resumes a suspended task.
 *
 * A task that has been suspended by one or more calls to vTaskSuspend ()
 * will be made available for running again by a single call to
 * vTaskResume ().
 *
 * @param xTaskToResume Handle to the task being readied.
 *
 * Example usage:
   <pre>
 void vAFunction( void )
 {
 TaskHandle_t xHandle;

	 // Create a task, storing the handle.
	 xTaskCreate( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY, &xHandle );

	 // ...

	 // Use the handle to suspend the created task.
	 vTaskSuspend( xHandle );

	 // ...

	 // The created task will not run during this period, unless
	 // another task calls vTaskResume( xHandle ).

	 //...


	 // Resume the suspended task ourselves.
	 vTaskResume( xHandle );

	 // The created task will once again get microcontroller processing
	 // time in accordance with its priority within the system.
 }
   </pre>
 * \defgroup vTaskResume vTaskResume
 * \ingroup TaskCtrl
 */
void vTaskResume( TaskHandle_t xTaskToResume ) ;

/**
 * task. h
 * <pre>void xTaskResumeFromISR( TaskHandle_t xTaskToResume );</pre>
 *
 * INCLUDE_xTaskResumeFromISR must be defined as 1 for this function to be
 * available.  See the configuration section for more information.
 *
 * An implementation of vTaskResume() that can be called from within an ISR.
 *
 * A task that has been suspended by one or more calls to vTaskSuspend ()
 * will be made available for running again by a single call to
 * xTaskResumeFromISR ().
 *
 * xTaskResumeFromISR() should not be used to synchronise a task with an
 * interrupt if there is a chance that the interrupt could arrive prior to the
 * task being suspended - as this can lead to interrupts being missed. Use of a
 * semaphore as a synchronisation mechanism would avoid this eventuality.
 *
 * @param xTaskToResume Handle to the task being readied.
 *
 * @return pdTRUE if resuming the task should result in a context switch,
 * otherwise pdFALSE. This is used by the ISR to determine if a context switch
 * may be required following the ISR.
 *
 * \defgroup vTaskResumeFromISR vTaskResumeFromISR
 * \ingroup TaskCtrl
 */
BaseType_t xTaskResumeFromISR( TaskHandle_t xTaskToResume ) ;

/*-----------------------------------------------------------
 * SCHEDULER CONTROL
 *----------------------------------------------------------*/

/**
 * task. h
 * <pre>void vTaskStartScheduler( void );</pre>
 *
 * Starts the real time kernel tick processing.  After calling the kernel
 * has control over which tasks are executed and when.
 *
 * See the demo application file main.c for an example of creating
 * tasks and starting the kernel.
 *
 * Example usage:
   <pre>
 void vAFunction( void )
 {
	 // Create at least one task before starting the kernel.
	 xTaskCreate( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );

	 // Start the real time kernel with preemption.
	 vTaskStartScheduler ();

	 // Will not get here unless a task calls vTaskEndScheduler ()
 }
   </pre>
 *
 * \defgroup vTaskStartScheduler vTaskStartScheduler
 * \ingroup SchedulerControl
 */
void vTaskStartScheduler( void ) ;

/**
 * task. h
 * <pre>void vTaskEndScheduler( void );</pre>
 *
 * NOTE:  At the time of writing only the x86 real mode port, which runs on a PC
 * in place of DOS, implements this function.
 *
 * Stops the real time kernel tick.  All created tasks will be automatically
 * deleted and multitasking (either preemptive or cooperative) will
 * stop.  Execution then resumes from the point where vTaskStartScheduler ()
 * was called, as if vTaskStartScheduler () had just returned.
 *
 * See the demo application file main. c in the demo/PC directory for an
 * example that uses vTaskEndScheduler ().
 *
 * vTaskEndScheduler () requires an exit function to be defined within the
 * portable layer (see vPortEndScheduler () in port. c for the PC port).  This
 * performs hardware specific operations such as stopping the kernel tick.
 *
 * vTaskEndScheduler () will cause all of the resources allocated by the
 * kernel to be freed - but will not free resources allocated by application
 * tasks.
 *
 * Example usage:
   <pre>
 void vTaskCode( void * pvParameters )
 {
	 for( ;; )
	 {
		 // Task code goes here.

		 // At some point we want to end the real time kernel processing
		 // so call ...
		 vTaskEndScheduler ();
	 }
 }

 void vAFunction( void )
 {
	 // Create at least one task before starting the kernel.
	 xTaskCreate( vTaskCode, "NAME", STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );

	 // Start the real time kernel with preemption.
	 vTaskStartScheduler ();

	 // Will only get here when the vTaskCode () task has called
	 // vTaskEndScheduler ().  When we get here we are back to single task
	 // execution.
 }
   </pre>
 *
 * \defgroup vTaskEndScheduler vTaskEndScheduler
 * \ingroup SchedulerControl
 */
void vTaskEndScheduler( void ) ;

/**
 * task. h
 * <pre>void vTaskSuspendAll( void );</pre>
 *
 * Suspends the scheduler without disabling interrupts.  Context switches will
 * not occur while the scheduler is suspended.
 *
 * After calling vTaskSuspendAll () the calling task will continue to execute
 * without risk of being swapped out until a call to xTaskResumeAll () has been
 * made.
 *
 * API functions that have the potential to cause a context switch (for example,
 * vTaskDelayUntil(), xQueueSend(), etc.) must not be called while the scheduler
 * is suspended.
 *
 * Example usage:
   <pre>
 void vTask1( void * pvParameters )
 {
	 for( ;; )
	 {
		 // Task code goes here.

		 // ...

		 // At some point the task wants to perform a long operation during
		 // which it does not want to get swapped out.  It cannot use
		 // taskENTER_CRITICAL ()/taskEXIT_CRITICAL () as the length of the
		 // operation may cause interrupts to be missed - including the
		 // ticks.

		 // Prevent the real time kernel swapping out the task.
		 vTaskSuspendAll ();

		 // Perform the operation here.  There is no need to use critical
		 // sections as we have all the microcontroller processing time.
		 // During this time interrupts will still operate and the kernel
		 // tick count will be maintained.

		 // ...

		 // The operation is complete.  Restart the kernel.
		 xTaskResumeAll ();
	 }
 }
   </pre>
 * \defgroup vTaskSuspendAll vTaskSuspendAll
 * \ingroup SchedulerControl
 */
void vTaskSuspendAll( void ) ;

/**
 * task. h
 * <pre>BaseType_t xTaskResumeAll( void );</pre>
 *
 * Resumes scheduler activity after it was suspended by a call to
 * vTaskSuspendAll().
 *
 * xTaskResumeAll() only resumes the scheduler.  It does not unsuspend tasks
 * that were previously suspended by a call to vTaskSuspend().
 *
 * @return If resuming the scheduler caused a context switch then pdTRUE is
 *		  returned, otherwise pdFALSE is returned.
 *
 * Example usage:
   <pre>
 void vTask1( void * pvParameters )
 {
	 for( ;; )
	 {
		 // Task code goes here.

		 // ...

		 // At some point the task wants to perform a long operation during
		 // which it does not want to get swapped out.  It cannot use
		 // taskENTER_CRITICAL ()/taskEXIT_CRITICAL () as the length of the
		 // operation may cause interrupts to be missed - including the
		 // ticks.

		 // Prevent the real time kernel swapping out the task.
		 vTaskSuspendAll ();

		 // Perform the operation here.  There is no need to use critical
		 // sections as we have all the microcontroller processing time.
		 // During this time interrupts will still operate and the real
		 // time kernel tick count will be maintained.

		 // ...

		 // The operation is complete.  Restart the kernel.  We want to force
		 // a context switch - but there is no point if resuming the scheduler
		 // caused a context switch already.
		 if( !xTaskResumeAll () )
		 {
			  taskYIELD ();
		 }
	 }
 }
   </pre>
 * \defgroup xTaskResumeAll xTaskResumeAll
 * \ingroup SchedulerControl
 */
BaseType_t xTaskResumeAll( void ) ;

/*-----------------------------------------------------------
 * TASK UTILITIES
 *----------------------------------------------------------*/

/**
 * task. h
 * <PRE>TickType_t xTaskGetTickCount( void );</PRE>
 *
 * @return The count of ticks since vTaskStartScheduler was called.
 *
 * \defgroup xTaskGetTickCount xTaskGetTickCount
 * \ingroup TaskUtils
 */
TickType_t xTaskGetTickCount( void ) ;

/**
 * task. h
 * <PRE>TickType_t xTaskGetTickCountFromISR( void );</PRE>
 *
 * @return The count of ticks since vTaskStartScheduler was called.
 *
 * This is a version of xTaskGetTickCount() that is safe to be called from an
 * ISR - provided that TickType_t is the natural word size of the
 * microcontroller being used or interrupt nesting is either not supported or
 * not being used.
 *
 * \defgroup xTaskGetTickCountFromISR xTaskGetTickCountFromISR
 * \ingroup TaskUtils
 */
TickType_t xTaskGetTickCountFromISR( void ) ;

/**
 * task. h
 * <PRE>uint16_t uxTaskGetNumberOfTasks( void );</PRE>
 *
 * @return The number of tasks that the real time kernel is currently managing.
 * This includes all ready, blocked and suspended tasks.  A task that
 * has been deleted but not yet freed by the idle task will also be
 * included in the count.
 *
 * \defgroup uxTaskGetNumberOfTasks uxTaskGetNumberOfTasks
 * \ingroup TaskUtils
 */
UBaseType_t uxTaskGetNumberOfTasks( void ) ;

/**
 * task. h
 * <PRE>char *pcTaskGetTaskName( TaskHandle_t xTaskToQuery );</PRE>
 *
 * @return The text (human readable) name of the task referenced by the handle
 * xTaskToQuery.  A task can query its own name by either passing in its own
 * handle, or by setting xTaskToQuery to NULL.  INCLUDE_pcTaskGetTaskName must be
 * set to 1 in FreeRTOSConfig.h for pcTaskGetTaskName() to be available.
 *
 * \defgroup pcTaskGetTaskName pcTaskGetTaskName
 * \ingroup TaskUtils
 */
char *pcTaskGetTaskName( TaskHandle_t xTaskToQuery ) ; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

/**
 * task.h
 * <PRE>UBaseType_t uxTaskGetStackHighWaterMark( TaskHandle_t xTask );</PRE>
 *
 * INCLUDE_uxTaskGetStackHighWaterMark must be set to 1 in FreeRTOSConfig.h for
 * this function to be available.
 *
 * Returns the high water mark of the stack associated with xTask.  That is,
 * the minimum free stack space there has been (in words, so on a 32 bit machine
 * a value of 1 means 4 bytes) since the task started.  The smaller the returned
 * number the closer the task has come to overflowing its stack.
 *
 * @param xTask Handle of the task associated with the stack to be checked.
 * Set xTask to NULL to check the stack of the calling task.
 *
 * @return The smallest amount of free stack space there has been (in words, so
 * actual spaces on the stack rather than bytes) since the task referenced by
 * xTask was created.
 */
UBaseType_t uxTaskGetStackHighWaterMark( TaskHandle_t xTask ) ;

/* When using trace macros it is sometimes necessary to include task.h before
FreeRTOS.h.  When this is done TaskHookFunction_t will not yet have been defined,
so the following two prototypes will cause a compilation error.  This can be
fixed by simply guarding against the inclusion of these two prototypes unless
they are explicitly required by the configUSE_APPLICATION_TASK_TAG configuration
constant. */
#line 1138 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\task.h"

/**
 * task.h
 * <pre>BaseType_t xTaskCallApplicationTaskHook( TaskHandle_t xTask, void *pvParameter );</pre>
 *
 * Calls the hook function associated with xTask.  Passing xTask as NULL has
 * the effect of calling the Running tasks (the calling task) hook function.
 *
 * pvParameter is passed to the hook function for the task to interpret as it
 * wants.  The return value is the value returned by the task hook function
 * registered by the user.
 */
BaseType_t xTaskCallApplicationTaskHook( TaskHandle_t xTask, void *pvParameter ) ;

/**
 * xTaskGetIdleTaskHandle() is only available if
 * INCLUDE_xTaskGetIdleTaskHandle is set to 1 in FreeRTOSConfig.h.
 *
 * Simply returns the handle of the idle task.  It is not valid to call
 * xTaskGetIdleTaskHandle() before the scheduler has been started.
 */
TaskHandle_t xTaskGetIdleTaskHandle( void );

/**
 * configUSE_TRACE_FACILITY must be defined as 1 in FreeRTOSConfig.h for
 * uxTaskGetSystemState() to be available.
 *
 * uxTaskGetSystemState() populates an TaskStatus_t structure for each task in
 * the system.  TaskStatus_t structures contain, among other things, members
 * for the task handle, task name, task priority, task state, and total amount
 * of run time consumed by the task.  See the TaskStatus_t structure
 * definition in this file for the full member list.
 *
 * NOTE:  This function is intended for debugging use only as its use results in
 * the scheduler remaining suspended for an extended period.
 *
 * @param pxTaskStatusArray A pointer to an array of TaskStatus_t structures.
 * The array must contain at least one TaskStatus_t structure for each task
 * that is under the control of the RTOS.  The number of tasks under the control
 * of the RTOS can be determined using the uxTaskGetNumberOfTasks() API function.
 *
 * @param uxArraySize The size of the array pointed to by the pxTaskStatusArray
 * parameter.  The size is specified as the number of indexes in the array, or
 * the number of TaskStatus_t structures contained in the array, not by the
 * number of bytes in the array.
 *
 * @param pulTotalRunTime If configGENERATE_RUN_TIME_STATS is set to 1 in
 * FreeRTOSConfig.h then *pulTotalRunTime is set by uxTaskGetSystemState() to the
 * total run time (as defined by the run time stats clock, see
 * http://www.freertos.org/rtos-run-time-stats.html) since the target booted.
 * pulTotalRunTime can be set to NULL to omit the total run time information.
 *
 * @return The number of TaskStatus_t structures that were populated by
 * uxTaskGetSystemState().  This should equal the number returned by the
 * uxTaskGetNumberOfTasks() API function, but will be zero if the value passed
 * in the uxArraySize parameter was too small.
 *
 * Example usage:
   <pre>
    // This example demonstrates how a human readable table of run time stats
	// information is generated from raw data provided by uxTaskGetSystemState().
	// The human readable table is written to pcWriteBuffer
	void vTaskGetRunTimeStats( char *pcWriteBuffer )
	{
	TaskStatus_t *pxTaskStatusArray;
	volatile UBaseType_t uxArraySize, x;
	uint32_t ulTotalRunTime, ulStatsAsPercentage;

		// Make sure the write buffer does not contain a string.
		*pcWriteBuffer = 0x00;

		// Take a snapshot of the number of tasks in case it changes while this
		// function is executing.
		uxArraySize = uxTaskGetNumberOfTasks();

		// Allocate a TaskStatus_t structure for each task.  An array could be
		// allocated statically at compile time.
		pxTaskStatusArray = pvPortMalloc( uxArraySize * sizeof( TaskStatus_t ) );

		if( pxTaskStatusArray != NULL )
		{
			// Generate raw status information about each task.
			uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, &ulTotalRunTime );

			// For percentage calculations.
			ulTotalRunTime /= 100UL;

			// Avoid divide by zero errors.
			if( ulTotalRunTime > 0 )
			{
				// For each populated position in the pxTaskStatusArray array,
				// format the raw data as human readable ASCII data
				for( x = 0; x < uxArraySize; x++ )
				{
					// What percentage of the total run time has the task used?
					// This will always be rounded down to the nearest integer.
					// ulTotalRunTimeDiv100 has already been divided by 100.
					ulStatsAsPercentage = pxTaskStatusArray[ x ].ulRunTimeCounter / ulTotalRunTime;

					if( ulStatsAsPercentage > 0UL )
					{
						sprintf( pcWriteBuffer, "%s\t\t%lu\t\t%lu%%\r\n", pxTaskStatusArray[ x ].pcTaskName, pxTaskStatusArray[ x ].ulRunTimeCounter, ulStatsAsPercentage );
					}
					else
					{
						// If the percentage is zero here then the task has
						// consumed less than 1% of the total run time.
						sprintf( pcWriteBuffer, "%s\t\t%lu\t\t<1%%\r\n", pxTaskStatusArray[ x ].pcTaskName, pxTaskStatusArray[ x ].ulRunTimeCounter );
					}

					pcWriteBuffer += strlen( ( char * ) pcWriteBuffer );
				}
			}

			// The array is no longer needed, free the memory it consumes.
			vPortFree( pxTaskStatusArray );
		}
	}
	</pre>
 */
UBaseType_t uxTaskGetSystemState( TaskStatus_t * const pxTaskStatusArray, const UBaseType_t uxArraySize, uint32_t * const pulTotalRunTime );

/**
 * task. h
 * <PRE>void vTaskList( char *pcWriteBuffer );</PRE>
 *
 * configUSE_TRACE_FACILITY and configUSE_STATS_FORMATTING_FUNCTIONS must
 * both be defined as 1 for this function to be available.  See the
 * configuration section of the FreeRTOS.org website for more information.
 *
 * NOTE 1: This function will disable interrupts for its duration.  It is
 * not intended for normal application runtime use but as a debug aid.
 *
 * Lists all the current tasks, along with their current state and stack
 * usage high water mark.
 *
 * Tasks are reported as blocked ('B'), ready ('R'), deleted ('D') or
 * suspended ('S').
 *
 * PLEASE NOTE:
 *
 * This function is provided for convenience only, and is used by many of the
 * demo applications.  Do not consider it to be part of the scheduler.
 *
 * vTaskList() calls uxTaskGetSystemState(), then formats part of the
 * uxTaskGetSystemState() output into a human readable table that displays task
 * names, states and stack usage.
 *
 * vTaskList() has a dependency on the sprintf() C library function that might
 * bloat the code size, use a lot of stack, and provide different results on
 * different platforms.  An alternative, tiny, third party, and limited
 * functionality implementation of sprintf() is provided in many of the
 * FreeRTOS/Demo sub-directories in a file called printf-stdarg.c (note
 * printf-stdarg.c does not provide a full snprintf() implementation!).
 *
 * It is recommended that production systems call uxTaskGetSystemState()
 * directly to get access to raw stats data, rather than indirectly through a
 * call to vTaskList().
 *
 * @param pcWriteBuffer A buffer into which the above mentioned details
 * will be written, in ASCII form.  This buffer is assumed to be large
 * enough to contain the generated report.  Approximately 40 bytes per
 * task should be sufficient.
 *
 * \defgroup vTaskList vTaskList
 * \ingroup TaskUtils
 */
void vTaskList( char * pcWriteBuffer ) ; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

/**
 * task. h
 * <PRE>void vTaskGetRunTimeStats( char *pcWriteBuffer );</PRE>
 *
 * configGENERATE_RUN_TIME_STATS and configUSE_STATS_FORMATTING_FUNCTIONS
 * must both be defined as 1 for this function to be available.  The application
 * must also then provide definitions for
 * portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() and portGET_RUN_TIME_COUNTER_VALUE()
 * to configure a peripheral timer/counter and return the timers current count
 * value respectively.  The counter should be at least 10 times the frequency of
 * the tick count.
 *
 * NOTE 1: This function will disable interrupts for its duration.  It is
 * not intended for normal application runtime use but as a debug aid.
 *
 * Setting configGENERATE_RUN_TIME_STATS to 1 will result in a total
 * accumulated execution time being stored for each task.  The resolution
 * of the accumulated time value depends on the frequency of the timer
 * configured by the portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() macro.
 * Calling vTaskGetRunTimeStats() writes the total execution time of each
 * task into a buffer, both as an absolute count value and as a percentage
 * of the total system execution time.
 *
 * NOTE 2:
 *
 * This function is provided for convenience only, and is used by many of the
 * demo applications.  Do not consider it to be part of the scheduler.
 *
 * vTaskGetRunTimeStats() calls uxTaskGetSystemState(), then formats part of the
 * uxTaskGetSystemState() output into a human readable table that displays the
 * amount of time each task has spent in the Running state in both absolute and
 * percentage terms.
 *
 * vTaskGetRunTimeStats() has a dependency on the sprintf() C library function
 * that might bloat the code size, use a lot of stack, and provide different
 * results on different platforms.  An alternative, tiny, third party, and
 * limited functionality implementation of sprintf() is provided in many of the
 * FreeRTOS/Demo sub-directories in a file called printf-stdarg.c (note
 * printf-stdarg.c does not provide a full snprintf() implementation!).
 *
 * It is recommended that production systems call uxTaskGetSystemState() directly
 * to get access to raw stats data, rather than indirectly through a call to
 * vTaskGetRunTimeStats().
 *
 * @param pcWriteBuffer A buffer into which the execution times will be
 * written, in ASCII form.  This buffer is assumed to be large enough to
 * contain the generated report.  Approximately 40 bytes per task should
 * be sufficient.
 *
 * \defgroup vTaskGetRunTimeStats vTaskGetRunTimeStats
 * \ingroup TaskUtils
 */
void vTaskGetRunTimeStats( char *pcWriteBuffer ) ; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

/*-----------------------------------------------------------
 * SCHEDULER INTERNALS AVAILABLE FOR PORTING PURPOSES
 *----------------------------------------------------------*/

/*
 * THIS FUNCTION MUST NOT BE USED FROM APPLICATION CODE.  IT IS ONLY
 * INTENDED FOR USE WHEN IMPLEMENTING A PORT OF THE SCHEDULER AND IS
 * AN INTERFACE WHICH IS FOR THE EXCLUSIVE USE OF THE SCHEDULER.
 *
 * Called from the real time kernel tick (either preemptive or cooperative),
 * this increments the tick count and checks if any tasks that are blocked
 * for a finite period required removing from a blocked list and placing on
 * a ready list.  If a non-zero value is returned then a context switch is
 * required because either:
 *   + A task was removed from a blocked list because its timeout had expired,
 *     or
 *   + Time slicing is in use and there is a task of equal priority to the
 *     currently running task.
 */
BaseType_t xTaskIncrementTick( void ) ;

/*
 * THIS FUNCTION MUST NOT BE USED FROM APPLICATION CODE.  IT IS AN
 * INTERFACE WHICH IS FOR THE EXCLUSIVE USE OF THE SCHEDULER.
 *
 * THIS FUNCTION MUST BE CALLED WITH INTERRUPTS DISABLED.
 *
 * Removes the calling task from the ready list and places it both
 * on the list of tasks waiting for a particular event, and the
 * list of delayed tasks.  The task will be removed from both lists
 * and replaced on the ready list should either the event occur (and
 * there be no higher priority tasks waiting on the same event) or
 * the delay period expires.
 *
 * The 'unordered' version replaces the event list item value with the
 * xItemValue value, and inserts the list item at the end of the list.
 *
 * The 'ordered' version uses the existing event list item value (which is the
 * owning tasks priority) to insert the list item into the event list is task
 * priority order.
 *
 * @param pxEventList The list containing tasks that are blocked waiting
 * for the event to occur.
 *
 * @param xItemValue The item value to use for the event list item when the
 * event list is not ordered by task priority.
 *
 * @param xTicksToWait The maximum amount of time that the task should wait
 * for the event to occur.  This is specified in kernel ticks,the constant
 * portTICK_PERIOD_MS can be used to convert kernel ticks into a real time
 * period.
 */
void vTaskPlaceOnEventList( List_t * const pxEventList, const TickType_t xTicksToWait ) ;
void vTaskPlaceOnUnorderedEventList( List_t * pxEventList, const TickType_t xItemValue, const TickType_t xTicksToWait ) ;

/*
 * THIS FUNCTION MUST NOT BE USED FROM APPLICATION CODE.  IT IS AN
 * INTERFACE WHICH IS FOR THE EXCLUSIVE USE OF THE SCHEDULER.
 *
 * THIS FUNCTION MUST BE CALLED WITH INTERRUPTS DISABLED.
 *
 * This function performs nearly the same function as vTaskPlaceOnEventList().
 * The difference being that this function does not permit tasks to block
 * indefinitely, whereas vTaskPlaceOnEventList() does.
 *
 */
void vTaskPlaceOnEventListRestricted( List_t * const pxEventList, const TickType_t xTicksToWait ) ;

/*
 * THIS FUNCTION MUST NOT BE USED FROM APPLICATION CODE.  IT IS AN
 * INTERFACE WHICH IS FOR THE EXCLUSIVE USE OF THE SCHEDULER.
 *
 * THIS FUNCTION MUST BE CALLED WITH INTERRUPTS DISABLED.
 *
 * Removes a task from both the specified event list and the list of blocked
 * tasks, and places it on a ready queue.
 *
 * xTaskRemoveFromEventList()/xTaskRemoveFromUnorderedEventList() will be called
 * if either an event occurs to unblock a task, or the block timeout period
 * expires.
 *
 * xTaskRemoveFromEventList() is used when the event list is in task priority
 * order.  It removes the list item from the head of the event list as that will
 * have the highest priority owning task of all the tasks on the event list.
 * xTaskRemoveFromUnorderedEventList() is used when the event list is not
 * ordered and the event list items hold something other than the owning tasks
 * priority.  In this case the event list item value is updated to the value
 * passed in the xItemValue parameter.
 *
 * @return pdTRUE if the task being removed has a higher priority than the task
 * making the call, otherwise pdFALSE.
 */
BaseType_t xTaskRemoveFromEventList( const List_t * const pxEventList ) ;
BaseType_t xTaskRemoveFromUnorderedEventList( ListItem_t * pxEventListItem, const TickType_t xItemValue ) ;

/*
 * THIS FUNCTION MUST NOT BE USED FROM APPLICATION CODE.  IT IS ONLY
 * INTENDED FOR USE WHEN IMPLEMENTING A PORT OF THE SCHEDULER AND IS
 * AN INTERFACE WHICH IS FOR THE EXCLUSIVE USE OF THE SCHEDULER.
 *
 * Sets the pointer to the current TCB to the TCB of the highest priority task
 * that is ready to run.
 */
void vTaskSwitchContext( void ) ;

/*
 * THESE FUNCTIONS MUST NOT BE USED FROM APPLICATION CODE.  THEY ARE USED BY
 * THE EVENT BITS MODULE.
 */
TickType_t uxTaskResetEventItemValue( void ) ;

/*
 * Return the handle of the calling task.
 */
TaskHandle_t xTaskGetCurrentTaskHandle( void ) ;

/*
 * Capture the current time status for future reference.
 */
void vTaskSetTimeOutState( TimeOut_t * const pxTimeOut ) ;

/*
 * Compare the time status now with that previously captured to see if the
 * timeout has expired.
 */
BaseType_t xTaskCheckForTimeOut( TimeOut_t * const pxTimeOut, TickType_t * const pxTicksToWait ) ;

/*
 * Shortcut used by the queue implementation to prevent unnecessary call to
 * taskYIELD();
 */
void vTaskMissedYield( void ) ;

/*
 * Returns the scheduler state as taskSCHEDULER_RUNNING,
 * taskSCHEDULER_NOT_STARTED or taskSCHEDULER_SUSPENDED.
 */
BaseType_t xTaskGetSchedulerState( void ) ;

/*
 * Raises the priority of the mutex holder to that of the calling task should
 * the mutex holder have a priority less than the calling task.
 */
void vTaskPriorityInherit( TaskHandle_t const pxMutexHolder ) ;

/*
 * Set the priority of a task back to its proper priority in the case that it
 * inherited a higher priority while it was holding a semaphore.
 */
BaseType_t xTaskPriorityDisinherit( TaskHandle_t const pxMutexHolder ) ;

/*
 * Generic version of the task creation function which is in turn called by the
 * xTaskCreate() and xTaskCreateRestricted() macros.
 */
BaseType_t xTaskGenericCreate( TaskFunction_t pxTaskCode, const char * const pcName, const uint16_t usStackDepth, void * const pvParameters, UBaseType_t uxPriority, TaskHandle_t * const pxCreatedTask, StackType_t * const puxStackBuffer, const MemoryRegion_t * const xRegions ) ; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

/*
 * Get the uxTCBNumber assigned to the task referenced by the xTask parameter.
 */
UBaseType_t uxTaskGetTaskNumber( TaskHandle_t xTask ) ;

/*
 * Set the uxTaskNumber of the task referenced by the xTask parameter to
 * uxHandle.
 */
void vTaskSetTaskNumber( TaskHandle_t xTask, const UBaseType_t uxHandle ) ;

/*
 * Only available when configUSE_TICKLESS_IDLE is set to 1.
 * If tickless mode is being used, or a low power mode is implemented, then
 * the tick interrupt will not execute during idle periods.  When this is the
 * case, the tick count value maintained by the scheduler needs to be kept up
 * to date with the actual execution time by being skipped forward by a time
 * equal to the idle period.
 */
void vTaskStepTick( const TickType_t xTicksToJump ) ;

/*
 * Only avilable when configUSE_TICKLESS_IDLE is set to 1.
 * Provided for use within portSUPPRESS_TICKS_AND_SLEEP() to allow the port
 * specific sleep function to determine if it is ok to proceed with the sleep,
 * and if it is ok to proceed, if it is ok to sleep indefinitely.
 *
 * This function is necessary because portSUPPRESS_TICKS_AND_SLEEP() is only
 * called with the scheduler suspended, not from within a critical section.  It
 * is therefore possible for an interrupt to request a context switch between
 * portSUPPRESS_TICKS_AND_SLEEP() and the low power mode actually being
 * entered.  eTaskConfirmSleepModeStatus() should be called from a short
 * critical section between the timer being stopped and the sleep mode being
 * entered to ensure it is ok to proceed into the sleep mode.
 */
eSleepModeStatus eTaskConfirmSleepModeStatus( void ) ;

/*
 * For internal use only.  Increment the mutex held count when a mutex is
 * taken and return the handle of the task that has taken the mutex.
 */
void *pvTaskIncrementMutexHeldCount( void );








#line 37 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/sys_arch.h"
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\queue.h"
/*
    FreeRTOS V8.1.2 - Copyright (C) 2014 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that has become a de facto standard.             *
     *                                                                       *
     *    Help yourself get started quickly and support the FreeRTOS         *
     *    project by purchasing a FreeRTOS tutorial book, reference          *
     *    manual, or both from: http://www.FreeRTOS.org/Documentation        *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available from the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?"                                     *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/














/**
 * Type by which queues are referenced.  For example, a call to xQueueCreate()
 * returns an QueueHandle_t variable that can then be used as a parameter to
 * xQueueSend(), xQueueReceive(), etc.
 */
typedef void * QueueHandle_t;

/**
 * Type by which queue sets are referenced.  For example, a call to
 * xQueueCreateSet() returns an xQueueSet variable that can then be used as a
 * parameter to xQueueSelectFromSet(), xQueueAddToSet(), etc.
 */
typedef void * QueueSetHandle_t;

/**
 * Queue sets can contain both queues and semaphores, so the
 * QueueSetMemberHandle_t is defined as a type to be used where a parameter or
 * return value can be either an QueueHandle_t or an SemaphoreHandle_t.
 */
typedef void * QueueSetMemberHandle_t;

/* For internal use only. */




/* For internal use only.  These definitions *must* match those in queue.c. */
#line 112 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\queue.h"

/**
 * queue. h
 * <pre>
 QueueHandle_t xQueueCreate(
							  UBaseType_t uxQueueLength,
							  UBaseType_t uxItemSize
						  );
 * </pre>
 *
 * Creates a new queue instance.  This allocates the storage required by the
 * new queue and returns a handle for the queue.
 *
 * @param uxQueueLength The maximum number of items that the queue can contain.
 *
 * @param uxItemSize The number of bytes each item in the queue will require.
 * Items are queued by copy, not by reference, so this is the number of bytes
 * that will be copied for each posted item.  Each item on the queue must be
 * the same size.
 *
 * @return If the queue is successfully create then a handle to the newly
 * created queue is returned.  If the queue cannot be created then 0 is
 * returned.
 *
 * Example usage:
   <pre>
 struct AMessage
 {
	char ucMessageID;
	char ucData[ 20 ];
 };

 void vATask( void *pvParameters )
 {
 QueueHandle_t xQueue1, xQueue2;

	// Create a queue capable of containing 10 uint32_t values.
	xQueue1 = xQueueCreate( 10, sizeof( uint32_t ) );
	if( xQueue1 == 0 )
	{
		// Queue was not created and must not be used.
	}

	// Create a queue capable of containing 10 pointers to AMessage structures.
	// These should be passed by pointer as they contain a lot of data.
	xQueue2 = xQueueCreate( 10, sizeof( struct AMessage * ) );
	if( xQueue2 == 0 )
	{
		// Queue was not created and must not be used.
	}

	// ... Rest of task code.
 }
 </pre>
 * \defgroup xQueueCreate xQueueCreate
 * \ingroup QueueManagement
 */


/**
 * queue. h
 * <pre>
 BaseType_t xQueueSendToToFront(
								   QueueHandle_t	xQueue,
								   const void		*pvItemToQueue,
								   TickType_t		xTicksToWait
							   );
 * </pre>
 *
 * This is a macro that calls xQueueGenericSend().
 *
 * Post an item to the front of a queue.  The item is queued by copy, not by
 * reference.  This function must not be called from an interrupt service
 * routine.  See xQueueSendFromISR () for an alternative which may be used
 * in an ISR.
 *
 * @param xQueue The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @param xTicksToWait The maximum amount of time the task should block
 * waiting for space to become available on the queue, should it already
 * be full.  The call will return immediately if this is set to 0 and the
 * queue is full.  The time is defined in tick periods so the constant
 * portTICK_PERIOD_MS should be used to convert to real time if this is required.
 *
 * @return pdTRUE if the item was successfully posted, otherwise errQUEUE_FULL.
 *
 * Example usage:
   <pre>
 struct AMessage
 {
	char ucMessageID;
	char ucData[ 20 ];
 } xMessage;

 uint32_t ulVar = 10UL;

 void vATask( void *pvParameters )
 {
 QueueHandle_t xQueue1, xQueue2;
 struct AMessage *pxMessage;

	// Create a queue capable of containing 10 uint32_t values.
	xQueue1 = xQueueCreate( 10, sizeof( uint32_t ) );

	// Create a queue capable of containing 10 pointers to AMessage structures.
	// These should be passed by pointer as they contain a lot of data.
	xQueue2 = xQueueCreate( 10, sizeof( struct AMessage * ) );

	// ...

	if( xQueue1 != 0 )
	{
		// Send an uint32_t.  Wait for 10 ticks for space to become
		// available if necessary.
		if( xQueueSendToFront( xQueue1, ( void * ) &ulVar, ( TickType_t ) 10 ) != pdPASS )
		{
			// Failed to post the message, even after 10 ticks.
		}
	}

	if( xQueue2 != 0 )
	{
		// Send a pointer to a struct AMessage object.  Don't block if the
		// queue is already full.
		pxMessage = & xMessage;
		xQueueSendToFront( xQueue2, ( void * ) &pxMessage, ( TickType_t ) 0 );
	}

	// ... Rest of task code.
 }
 </pre>
 * \defgroup xQueueSend xQueueSend
 * \ingroup QueueManagement
 */


/**
 * queue. h
 * <pre>
 BaseType_t xQueueSendToBack(
								   QueueHandle_t	xQueue,
								   const void		*pvItemToQueue,
								   TickType_t		xTicksToWait
							   );
 * </pre>
 *
 * This is a macro that calls xQueueGenericSend().
 *
 * Post an item to the back of a queue.  The item is queued by copy, not by
 * reference.  This function must not be called from an interrupt service
 * routine.  See xQueueSendFromISR () for an alternative which may be used
 * in an ISR.
 *
 * @param xQueue The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @param xTicksToWait The maximum amount of time the task should block
 * waiting for space to become available on the queue, should it already
 * be full.  The call will return immediately if this is set to 0 and the queue
 * is full.  The  time is defined in tick periods so the constant
 * portTICK_PERIOD_MS should be used to convert to real time if this is required.
 *
 * @return pdTRUE if the item was successfully posted, otherwise errQUEUE_FULL.
 *
 * Example usage:
   <pre>
 struct AMessage
 {
	char ucMessageID;
	char ucData[ 20 ];
 } xMessage;

 uint32_t ulVar = 10UL;

 void vATask( void *pvParameters )
 {
 QueueHandle_t xQueue1, xQueue2;
 struct AMessage *pxMessage;

	// Create a queue capable of containing 10 uint32_t values.
	xQueue1 = xQueueCreate( 10, sizeof( uint32_t ) );

	// Create a queue capable of containing 10 pointers to AMessage structures.
	// These should be passed by pointer as they contain a lot of data.
	xQueue2 = xQueueCreate( 10, sizeof( struct AMessage * ) );

	// ...

	if( xQueue1 != 0 )
	{
		// Send an uint32_t.  Wait for 10 ticks for space to become
		// available if necessary.
		if( xQueueSendToBack( xQueue1, ( void * ) &ulVar, ( TickType_t ) 10 ) != pdPASS )
		{
			// Failed to post the message, even after 10 ticks.
		}
	}

	if( xQueue2 != 0 )
	{
		// Send a pointer to a struct AMessage object.  Don't block if the
		// queue is already full.
		pxMessage = & xMessage;
		xQueueSendToBack( xQueue2, ( void * ) &pxMessage, ( TickType_t ) 0 );
	}

	// ... Rest of task code.
 }
 </pre>
 * \defgroup xQueueSend xQueueSend
 * \ingroup QueueManagement
 */


/**
 * queue. h
 * <pre>
 BaseType_t xQueueSend(
							  QueueHandle_t xQueue,
							  const void * pvItemToQueue,
							  TickType_t xTicksToWait
						 );
 * </pre>
 *
 * This is a macro that calls xQueueGenericSend().  It is included for
 * backward compatibility with versions of FreeRTOS.org that did not
 * include the xQueueSendToFront() and xQueueSendToBack() macros.  It is
 * equivalent to xQueueSendToBack().
 *
 * Post an item on a queue.  The item is queued by copy, not by reference.
 * This function must not be called from an interrupt service routine.
 * See xQueueSendFromISR () for an alternative which may be used in an ISR.
 *
 * @param xQueue The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @param xTicksToWait The maximum amount of time the task should block
 * waiting for space to become available on the queue, should it already
 * be full.  The call will return immediately if this is set to 0 and the
 * queue is full.  The time is defined in tick periods so the constant
 * portTICK_PERIOD_MS should be used to convert to real time if this is required.
 *
 * @return pdTRUE if the item was successfully posted, otherwise errQUEUE_FULL.
 *
 * Example usage:
   <pre>
 struct AMessage
 {
	char ucMessageID;
	char ucData[ 20 ];
 } xMessage;

 uint32_t ulVar = 10UL;

 void vATask( void *pvParameters )
 {
 QueueHandle_t xQueue1, xQueue2;
 struct AMessage *pxMessage;

	// Create a queue capable of containing 10 uint32_t values.
	xQueue1 = xQueueCreate( 10, sizeof( uint32_t ) );

	// Create a queue capable of containing 10 pointers to AMessage structures.
	// These should be passed by pointer as they contain a lot of data.
	xQueue2 = xQueueCreate( 10, sizeof( struct AMessage * ) );

	// ...

	if( xQueue1 != 0 )
	{
		// Send an uint32_t.  Wait for 10 ticks for space to become
		// available if necessary.
		if( xQueueSend( xQueue1, ( void * ) &ulVar, ( TickType_t ) 10 ) != pdPASS )
		{
			// Failed to post the message, even after 10 ticks.
		}
	}

	if( xQueue2 != 0 )
	{
		// Send a pointer to a struct AMessage object.  Don't block if the
		// queue is already full.
		pxMessage = & xMessage;
		xQueueSend( xQueue2, ( void * ) &pxMessage, ( TickType_t ) 0 );
	}

	// ... Rest of task code.
 }
 </pre>
 * \defgroup xQueueSend xQueueSend
 * \ingroup QueueManagement
 */


/**
 * queue. h
 * <pre>
 BaseType_t xQueueOverwrite(
							  QueueHandle_t xQueue,
							  const void * pvItemToQueue
						 );
 * </pre>
 *
 * Only for use with queues that have a length of one - so the queue is either
 * empty or full.
 *
 * Post an item on a queue.  If the queue is already full then overwrite the
 * value held in the queue.  The item is queued by copy, not by reference.
 *
 * This function must not be called from an interrupt service routine.
 * See xQueueOverwriteFromISR () for an alternative which may be used in an ISR.
 *
 * @param xQueue The handle of the queue to which the data is being sent.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @return xQueueOverwrite() is a macro that calls xQueueGenericSend(), and
 * therefore has the same return values as xQueueSendToFront().  However, pdPASS
 * is the only value that can be returned because xQueueOverwrite() will write
 * to the queue even when the queue is already full.
 *
 * Example usage:
   <pre>

 void vFunction( void *pvParameters )
 {
 QueueHandle_t xQueue;
 uint32_t ulVarToSend, ulValReceived;

	// Create a queue to hold one uint32_t value.  It is strongly
	// recommended *not* to use xQueueOverwrite() on queues that can
	// contain more than one value, and doing so will trigger an assertion
	// if configASSERT() is defined.
	xQueue = xQueueCreate( 1, sizeof( uint32_t ) );

	// Write the value 10 to the queue using xQueueOverwrite().
	ulVarToSend = 10;
	xQueueOverwrite( xQueue, &ulVarToSend );

	// Peeking the queue should now return 10, but leave the value 10 in
	// the queue.  A block time of zero is used as it is known that the
	// queue holds a value.
	ulValReceived = 0;
	xQueuePeek( xQueue, &ulValReceived, 0 );

	if( ulValReceived != 10 )
	{
		// Error unless the item was removed by a different task.
	}

	// The queue is still full.  Use xQueueOverwrite() to overwrite the
	// value held in the queue with 100.
	ulVarToSend = 100;
	xQueueOverwrite( xQueue, &ulVarToSend );

	// This time read from the queue, leaving the queue empty once more.
	// A block time of 0 is used again.
	xQueueReceive( xQueue, &ulValReceived, 0 );

	// The value read should be the last value written, even though the
	// queue was already full when the value was written.
	if( ulValReceived != 100 )
	{
		// Error!
	}

	// ...
}
 </pre>
 * \defgroup xQueueOverwrite xQueueOverwrite
 * \ingroup QueueManagement
 */



/**
 * queue. h
 * <pre>
 BaseType_t xQueueGenericSend(
									QueueHandle_t xQueue,
									const void * pvItemToQueue,
									TickType_t xTicksToWait
									BaseType_t xCopyPosition
								);
 * </pre>
 *
 * It is preferred that the macros xQueueSend(), xQueueSendToFront() and
 * xQueueSendToBack() are used in place of calling this function directly.
 *
 * Post an item on a queue.  The item is queued by copy, not by reference.
 * This function must not be called from an interrupt service routine.
 * See xQueueSendFromISR () for an alternative which may be used in an ISR.
 *
 * @param xQueue The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @param xTicksToWait The maximum amount of time the task should block
 * waiting for space to become available on the queue, should it already
 * be full.  The call will return immediately if this is set to 0 and the
 * queue is full.  The time is defined in tick periods so the constant
 * portTICK_PERIOD_MS should be used to convert to real time if this is required.
 *
 * @param xCopyPosition Can take the value queueSEND_TO_BACK to place the
 * item at the back of the queue, or queueSEND_TO_FRONT to place the item
 * at the front of the queue (for high priority messages).
 *
 * @return pdTRUE if the item was successfully posted, otherwise errQUEUE_FULL.
 *
 * Example usage:
   <pre>
 struct AMessage
 {
	char ucMessageID;
	char ucData[ 20 ];
 } xMessage;

 uint32_t ulVar = 10UL;

 void vATask( void *pvParameters )
 {
 QueueHandle_t xQueue1, xQueue2;
 struct AMessage *pxMessage;

	// Create a queue capable of containing 10 uint32_t values.
	xQueue1 = xQueueCreate( 10, sizeof( uint32_t ) );

	// Create a queue capable of containing 10 pointers to AMessage structures.
	// These should be passed by pointer as they contain a lot of data.
	xQueue2 = xQueueCreate( 10, sizeof( struct AMessage * ) );

	// ...

	if( xQueue1 != 0 )
	{
		// Send an uint32_t.  Wait for 10 ticks for space to become
		// available if necessary.
		if( xQueueGenericSend( xQueue1, ( void * ) &ulVar, ( TickType_t ) 10, queueSEND_TO_BACK ) != pdPASS )
		{
			// Failed to post the message, even after 10 ticks.
		}
	}

	if( xQueue2 != 0 )
	{
		// Send a pointer to a struct AMessage object.  Don't block if the
		// queue is already full.
		pxMessage = & xMessage;
		xQueueGenericSend( xQueue2, ( void * ) &pxMessage, ( TickType_t ) 0, queueSEND_TO_BACK );
	}

	// ... Rest of task code.
 }
 </pre>
 * \defgroup xQueueSend xQueueSend
 * \ingroup QueueManagement
 */
BaseType_t xQueueGenericSend( QueueHandle_t xQueue, const void * const pvItemToQueue, TickType_t xTicksToWait, const BaseType_t xCopyPosition ) ;

/**
 * queue. h
 * <pre>
 BaseType_t xQueuePeek(
							 QueueHandle_t xQueue,
							 void *pvBuffer,
							 TickType_t xTicksToWait
						 );</pre>
 *
 * This is a macro that calls the xQueueGenericReceive() function.
 *
 * Receive an item from a queue without removing the item from the queue.
 * The item is received by copy so a buffer of adequate size must be
 * provided.  The number of bytes copied into the buffer was defined when
 * the queue was created.
 *
 * Successfully received items remain on the queue so will be returned again
 * by the next call, or a call to xQueueReceive().
 *
 * This macro must not be used in an interrupt service routine.  See
 * xQueuePeekFromISR() for an alternative that can be called from an interrupt
 * service routine.
 *
 * @param xQueue The handle to the queue from which the item is to be
 * received.
 *
 * @param pvBuffer Pointer to the buffer into which the received item will
 * be copied.
 *
 * @param xTicksToWait The maximum amount of time the task should block
 * waiting for an item to receive should the queue be empty at the time
 * of the call.	 The time is defined in tick periods so the constant
 * portTICK_PERIOD_MS should be used to convert to real time if this is required.
 * xQueuePeek() will return immediately if xTicksToWait is 0 and the queue
 * is empty.
 *
 * @return pdTRUE if an item was successfully received from the queue,
 * otherwise pdFALSE.
 *
 * Example usage:
   <pre>
 struct AMessage
 {
	char ucMessageID;
	char ucData[ 20 ];
 } xMessage;

 QueueHandle_t xQueue;

 // Task to create a queue and post a value.
 void vATask( void *pvParameters )
 {
 struct AMessage *pxMessage;

	// Create a queue capable of containing 10 pointers to AMessage structures.
	// These should be passed by pointer as they contain a lot of data.
	xQueue = xQueueCreate( 10, sizeof( struct AMessage * ) );
	if( xQueue == 0 )
	{
		// Failed to create the queue.
	}

	// ...

	// Send a pointer to a struct AMessage object.  Don't block if the
	// queue is already full.
	pxMessage = & xMessage;
	xQueueSend( xQueue, ( void * ) &pxMessage, ( TickType_t ) 0 );

	// ... Rest of task code.
 }

 // Task to peek the data from the queue.
 void vADifferentTask( void *pvParameters )
 {
 struct AMessage *pxRxedMessage;

	if( xQueue != 0 )
	{
		// Peek a message on the created queue.  Block for 10 ticks if a
		// message is not immediately available.
		if( xQueuePeek( xQueue, &( pxRxedMessage ), ( TickType_t ) 10 ) )
		{
			// pcRxedMessage now points to the struct AMessage variable posted
			// by vATask, but the item still remains on the queue.
		}
	}

	// ... Rest of task code.
 }
 </pre>
 * \defgroup xQueueReceive xQueueReceive
 * \ingroup QueueManagement
 */


/**
 * queue. h
 * <pre>
 BaseType_t xQueuePeekFromISR(
									QueueHandle_t xQueue,
									void *pvBuffer,
								);</pre>
 *
 * A version of xQueuePeek() that can be called from an interrupt service
 * routine (ISR).
 *
 * Receive an item from a queue without removing the item from the queue.
 * The item is received by copy so a buffer of adequate size must be
 * provided.  The number of bytes copied into the buffer was defined when
 * the queue was created.
 *
 * Successfully received items remain on the queue so will be returned again
 * by the next call, or a call to xQueueReceive().
 *
 * @param xQueue The handle to the queue from which the item is to be
 * received.
 *
 * @param pvBuffer Pointer to the buffer into which the received item will
 * be copied.
 *
 * @return pdTRUE if an item was successfully received from the queue,
 * otherwise pdFALSE.
 *
 * \defgroup xQueuePeekFromISR xQueuePeekFromISR
 * \ingroup QueueManagement
 */
BaseType_t xQueuePeekFromISR( QueueHandle_t xQueue, void * const pvBuffer ) ;

/**
 * queue. h
 * <pre>
 BaseType_t xQueueReceive(
								 QueueHandle_t xQueue,
								 void *pvBuffer,
								 TickType_t xTicksToWait
							);</pre>
 *
 * This is a macro that calls the xQueueGenericReceive() function.
 *
 * Receive an item from a queue.  The item is received by copy so a buffer of
 * adequate size must be provided.  The number of bytes copied into the buffer
 * was defined when the queue was created.
 *
 * Successfully received items are removed from the queue.
 *
 * This function must not be used in an interrupt service routine.  See
 * xQueueReceiveFromISR for an alternative that can.
 *
 * @param xQueue The handle to the queue from which the item is to be
 * received.
 *
 * @param pvBuffer Pointer to the buffer into which the received item will
 * be copied.
 *
 * @param xTicksToWait The maximum amount of time the task should block
 * waiting for an item to receive should the queue be empty at the time
 * of the call.	 xQueueReceive() will return immediately if xTicksToWait
 * is zero and the queue is empty.  The time is defined in tick periods so the
 * constant portTICK_PERIOD_MS should be used to convert to real time if this is
 * required.
 *
 * @return pdTRUE if an item was successfully received from the queue,
 * otherwise pdFALSE.
 *
 * Example usage:
   <pre>
 struct AMessage
 {
	char ucMessageID;
	char ucData[ 20 ];
 } xMessage;

 QueueHandle_t xQueue;

 // Task to create a queue and post a value.
 void vATask( void *pvParameters )
 {
 struct AMessage *pxMessage;

	// Create a queue capable of containing 10 pointers to AMessage structures.
	// These should be passed by pointer as they contain a lot of data.
	xQueue = xQueueCreate( 10, sizeof( struct AMessage * ) );
	if( xQueue == 0 )
	{
		// Failed to create the queue.
	}

	// ...

	// Send a pointer to a struct AMessage object.  Don't block if the
	// queue is already full.
	pxMessage = & xMessage;
	xQueueSend( xQueue, ( void * ) &pxMessage, ( TickType_t ) 0 );

	// ... Rest of task code.
 }

 // Task to receive from the queue.
 void vADifferentTask( void *pvParameters )
 {
 struct AMessage *pxRxedMessage;

	if( xQueue != 0 )
	{
		// Receive a message on the created queue.  Block for 10 ticks if a
		// message is not immediately available.
		if( xQueueReceive( xQueue, &( pxRxedMessage ), ( TickType_t ) 10 ) )
		{
			// pcRxedMessage now points to the struct AMessage variable posted
			// by vATask.
		}
	}

	// ... Rest of task code.
 }
 </pre>
 * \defgroup xQueueReceive xQueueReceive
 * \ingroup QueueManagement
 */



/**
 * queue. h
 * <pre>
 BaseType_t xQueueGenericReceive(
									   QueueHandle_t	xQueue,
									   void	*pvBuffer,
									   TickType_t	xTicksToWait
									   BaseType_t	xJustPeek
									);</pre>
 *
 * It is preferred that the macro xQueueReceive() be used rather than calling
 * this function directly.
 *
 * Receive an item from a queue.  The item is received by copy so a buffer of
 * adequate size must be provided.  The number of bytes copied into the buffer
 * was defined when the queue was created.
 *
 * This function must not be used in an interrupt service routine.  See
 * xQueueReceiveFromISR for an alternative that can.
 *
 * @param xQueue The handle to the queue from which the item is to be
 * received.
 *
 * @param pvBuffer Pointer to the buffer into which the received item will
 * be copied.
 *
 * @param xTicksToWait The maximum amount of time the task should block
 * waiting for an item to receive should the queue be empty at the time
 * of the call.	 The time is defined in tick periods so the constant
 * portTICK_PERIOD_MS should be used to convert to real time if this is required.
 * xQueueGenericReceive() will return immediately if the queue is empty and
 * xTicksToWait is 0.
 *
 * @param xJustPeek When set to true, the item received from the queue is not
 * actually removed from the queue - meaning a subsequent call to
 * xQueueReceive() will return the same item.  When set to false, the item
 * being received from the queue is also removed from the queue.
 *
 * @return pdTRUE if an item was successfully received from the queue,
 * otherwise pdFALSE.
 *
 * Example usage:
   <pre>
 struct AMessage
 {
	char ucMessageID;
	char ucData[ 20 ];
 } xMessage;

 QueueHandle_t xQueue;

 // Task to create a queue and post a value.
 void vATask( void *pvParameters )
 {
 struct AMessage *pxMessage;

	// Create a queue capable of containing 10 pointers to AMessage structures.
	// These should be passed by pointer as they contain a lot of data.
	xQueue = xQueueCreate( 10, sizeof( struct AMessage * ) );
	if( xQueue == 0 )
	{
		// Failed to create the queue.
	}

	// ...

	// Send a pointer to a struct AMessage object.  Don't block if the
	// queue is already full.
	pxMessage = & xMessage;
	xQueueSend( xQueue, ( void * ) &pxMessage, ( TickType_t ) 0 );

	// ... Rest of task code.
 }

 // Task to receive from the queue.
 void vADifferentTask( void *pvParameters )
 {
 struct AMessage *pxRxedMessage;

	if( xQueue != 0 )
	{
		// Receive a message on the created queue.  Block for 10 ticks if a
		// message is not immediately available.
		if( xQueueGenericReceive( xQueue, &( pxRxedMessage ), ( TickType_t ) 10 ) )
		{
			// pcRxedMessage now points to the struct AMessage variable posted
			// by vATask.
		}
	}

	// ... Rest of task code.
 }
 </pre>
 * \defgroup xQueueReceive xQueueReceive
 * \ingroup QueueManagement
 */
BaseType_t xQueueGenericReceive( QueueHandle_t xQueue, void * const pvBuffer, TickType_t xTicksToWait, const BaseType_t xJustPeek ) ;

/**
 * queue. h
 * <pre>UBaseType_t uxQueueMessagesWaiting( const QueueHandle_t xQueue );</pre>
 *
 * Return the number of messages stored in a queue.
 *
 * @param xQueue A handle to the queue being queried.
 *
 * @return The number of messages available in the queue.
 *
 * \defgroup uxQueueMessagesWaiting uxQueueMessagesWaiting
 * \ingroup QueueManagement
 */
UBaseType_t uxQueueMessagesWaiting( const QueueHandle_t xQueue ) ;

/**
 * queue. h
 * <pre>UBaseType_t uxQueueSpacesAvailable( const QueueHandle_t xQueue );</pre>
 *
 * Return the number of free spaces available in a queue.  This is equal to the
 * number of items that can be sent to the queue before the queue becomes full
 * if no items are removed.
 *
 * @param xQueue A handle to the queue being queried.
 *
 * @return The number of spaces available in the queue.
 *
 * \defgroup uxQueueMessagesWaiting uxQueueMessagesWaiting
 * \ingroup QueueManagement
 */
UBaseType_t uxQueueSpacesAvailable( const QueueHandle_t xQueue ) ;

/**
 * queue. h
 * <pre>void vQueueDelete( QueueHandle_t xQueue );</pre>
 *
 * Delete a queue - freeing all the memory allocated for storing of items
 * placed on the queue.
 *
 * @param xQueue A handle to the queue to be deleted.
 *
 * \defgroup vQueueDelete vQueueDelete
 * \ingroup QueueManagement
 */
void vQueueDelete( QueueHandle_t xQueue ) ;

/**
 * queue. h
 * <pre>
 BaseType_t xQueueSendToFrontFromISR(
										 QueueHandle_t xQueue,
										 const void *pvItemToQueue,
										 BaseType_t *pxHigherPriorityTaskWoken
									  );
 </pre>
 *
 * This is a macro that calls xQueueGenericSendFromISR().
 *
 * Post an item to the front of a queue.  It is safe to use this macro from
 * within an interrupt service routine.
 *
 * Items are queued by copy not reference so it is preferable to only
 * queue small items, especially when called from an ISR.  In most cases
 * it would be preferable to store a pointer to the item being queued.
 *
 * @param xQueue The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @param pxHigherPriorityTaskWoken xQueueSendToFrontFromISR() will set
 * *pxHigherPriorityTaskWoken to pdTRUE if sending to the queue caused a task
 * to unblock, and the unblocked task has a priority higher than the currently
 * running task.  If xQueueSendToFromFromISR() sets this value to pdTRUE then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return pdTRUE if the data was successfully sent to the queue, otherwise
 * errQUEUE_FULL.
 *
 * Example usage for buffered IO (where the ISR can obtain more than one value
 * per call):
   <pre>
 void vBufferISR( void )
 {
 char cIn;
 BaseType_t xHigherPrioritTaskWoken;

	// We have not woken a task at the start of the ISR.
	xHigherPriorityTaskWoken = pdFALSE;

	// Loop until the buffer is empty.
	do
	{
		// Obtain a byte from the buffer.
		cIn = portINPUT_BYTE( RX_REGISTER_ADDRESS );

		// Post the byte.
		xQueueSendToFrontFromISR( xRxQueue, &cIn, &xHigherPriorityTaskWoken );

	} while( portINPUT_BYTE( BUFFER_COUNT ) );

	// Now the buffer is empty we can switch context if necessary.
	if( xHigherPriorityTaskWoken )
	{
		taskYIELD ();
	}
 }
 </pre>
 *
 * \defgroup xQueueSendFromISR xQueueSendFromISR
 * \ingroup QueueManagement
 */



/**
 * queue. h
 * <pre>
 BaseType_t xQueueSendToBackFromISR(
										 QueueHandle_t xQueue,
										 const void *pvItemToQueue,
										 BaseType_t *pxHigherPriorityTaskWoken
									  );
 </pre>
 *
 * This is a macro that calls xQueueGenericSendFromISR().
 *
 * Post an item to the back of a queue.  It is safe to use this macro from
 * within an interrupt service routine.
 *
 * Items are queued by copy not reference so it is preferable to only
 * queue small items, especially when called from an ISR.  In most cases
 * it would be preferable to store a pointer to the item being queued.
 *
 * @param xQueue The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @param pxHigherPriorityTaskWoken xQueueSendToBackFromISR() will set
 * *pxHigherPriorityTaskWoken to pdTRUE if sending to the queue caused a task
 * to unblock, and the unblocked task has a priority higher than the currently
 * running task.  If xQueueSendToBackFromISR() sets this value to pdTRUE then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return pdTRUE if the data was successfully sent to the queue, otherwise
 * errQUEUE_FULL.
 *
 * Example usage for buffered IO (where the ISR can obtain more than one value
 * per call):
   <pre>
 void vBufferISR( void )
 {
 char cIn;
 BaseType_t xHigherPriorityTaskWoken;

	// We have not woken a task at the start of the ISR.
	xHigherPriorityTaskWoken = pdFALSE;

	// Loop until the buffer is empty.
	do
	{
		// Obtain a byte from the buffer.
		cIn = portINPUT_BYTE( RX_REGISTER_ADDRESS );

		// Post the byte.
		xQueueSendToBackFromISR( xRxQueue, &cIn, &xHigherPriorityTaskWoken );

	} while( portINPUT_BYTE( BUFFER_COUNT ) );

	// Now the buffer is empty we can switch context if necessary.
	if( xHigherPriorityTaskWoken )
	{
		taskYIELD ();
	}
 }
 </pre>
 *
 * \defgroup xQueueSendFromISR xQueueSendFromISR
 * \ingroup QueueManagement
 */


/**
 * queue. h
 * <pre>
 BaseType_t xQueueOverwriteFromISR(
							  QueueHandle_t xQueue,
							  const void * pvItemToQueue,
							  BaseType_t *pxHigherPriorityTaskWoken
						 );
 * </pre>
 *
 * A version of xQueueOverwrite() that can be used in an interrupt service
 * routine (ISR).
 *
 * Only for use with queues that can hold a single item - so the queue is either
 * empty or full.
 *
 * Post an item on a queue.  If the queue is already full then overwrite the
 * value held in the queue.  The item is queued by copy, not by reference.
 *
 * @param xQueue The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @param pxHigherPriorityTaskWoken xQueueOverwriteFromISR() will set
 * *pxHigherPriorityTaskWoken to pdTRUE if sending to the queue caused a task
 * to unblock, and the unblocked task has a priority higher than the currently
 * running task.  If xQueueOverwriteFromISR() sets this value to pdTRUE then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return xQueueOverwriteFromISR() is a macro that calls
 * xQueueGenericSendFromISR(), and therefore has the same return values as
 * xQueueSendToFrontFromISR().  However, pdPASS is the only value that can be
 * returned because xQueueOverwriteFromISR() will write to the queue even when
 * the queue is already full.
 *
 * Example usage:
   <pre>

 QueueHandle_t xQueue;

 void vFunction( void *pvParameters )
 {
 	// Create a queue to hold one uint32_t value.  It is strongly
	// recommended *not* to use xQueueOverwriteFromISR() on queues that can
	// contain more than one value, and doing so will trigger an assertion
	// if configASSERT() is defined.
	xQueue = xQueueCreate( 1, sizeof( uint32_t ) );
}

void vAnInterruptHandler( void )
{
// xHigherPriorityTaskWoken must be set to pdFALSE before it is used.
BaseType_t xHigherPriorityTaskWoken = pdFALSE;
uint32_t ulVarToSend, ulValReceived;

	// Write the value 10 to the queue using xQueueOverwriteFromISR().
	ulVarToSend = 10;
	xQueueOverwriteFromISR( xQueue, &ulVarToSend, &xHigherPriorityTaskWoken );

	// The queue is full, but calling xQueueOverwriteFromISR() again will still
	// pass because the value held in the queue will be overwritten with the
	// new value.
	ulVarToSend = 100;
	xQueueOverwriteFromISR( xQueue, &ulVarToSend, &xHigherPriorityTaskWoken );

	// Reading from the queue will now return 100.

	// ...

	if( xHigherPrioritytaskWoken == pdTRUE )
	{
		// Writing to the queue caused a task to unblock and the unblocked task
		// has a priority higher than or equal to the priority of the currently
		// executing task (the task this interrupt interrupted).  Perform a context
		// switch so this interrupt returns directly to the unblocked task.
		portYIELD_FROM_ISR(); // or portEND_SWITCHING_ISR() depending on the port.
	}
}
 </pre>
 * \defgroup xQueueOverwriteFromISR xQueueOverwriteFromISR
 * \ingroup QueueManagement
 */


/**
 * queue. h
 * <pre>
 BaseType_t xQueueSendFromISR(
									 QueueHandle_t xQueue,
									 const void *pvItemToQueue,
									 BaseType_t *pxHigherPriorityTaskWoken
								);
 </pre>
 *
 * This is a macro that calls xQueueGenericSendFromISR().  It is included
 * for backward compatibility with versions of FreeRTOS.org that did not
 * include the xQueueSendToBackFromISR() and xQueueSendToFrontFromISR()
 * macros.
 *
 * Post an item to the back of a queue.  It is safe to use this function from
 * within an interrupt service routine.
 *
 * Items are queued by copy not reference so it is preferable to only
 * queue small items, especially when called from an ISR.  In most cases
 * it would be preferable to store a pointer to the item being queued.
 *
 * @param xQueue The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @param pxHigherPriorityTaskWoken xQueueSendFromISR() will set
 * *pxHigherPriorityTaskWoken to pdTRUE if sending to the queue caused a task
 * to unblock, and the unblocked task has a priority higher than the currently
 * running task.  If xQueueSendFromISR() sets this value to pdTRUE then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return pdTRUE if the data was successfully sent to the queue, otherwise
 * errQUEUE_FULL.
 *
 * Example usage for buffered IO (where the ISR can obtain more than one value
 * per call):
   <pre>
 void vBufferISR( void )
 {
 char cIn;
 BaseType_t xHigherPriorityTaskWoken;

	// We have not woken a task at the start of the ISR.
	xHigherPriorityTaskWoken = pdFALSE;

	// Loop until the buffer is empty.
	do
	{
		// Obtain a byte from the buffer.
		cIn = portINPUT_BYTE( RX_REGISTER_ADDRESS );

		// Post the byte.
		xQueueSendFromISR( xRxQueue, &cIn, &xHigherPriorityTaskWoken );

	} while( portINPUT_BYTE( BUFFER_COUNT ) );

	// Now the buffer is empty we can switch context if necessary.
	if( xHigherPriorityTaskWoken )
	{
		// Actual macro used here is port specific.
		portYIELD_FROM_ISR ();
	}
 }
 </pre>
 *
 * \defgroup xQueueSendFromISR xQueueSendFromISR
 * \ingroup QueueManagement
 */


/**
 * queue. h
 * <pre>
 BaseType_t xQueueGenericSendFromISR(
										   QueueHandle_t		xQueue,
										   const	void	*pvItemToQueue,
										   BaseType_t	*pxHigherPriorityTaskWoken,
										   BaseType_t	xCopyPosition
									   );
 </pre>
 *
 * It is preferred that the macros xQueueSendFromISR(),
 * xQueueSendToFrontFromISR() and xQueueSendToBackFromISR() be used in place
 * of calling this function directly.
 *
 * Post an item on a queue.  It is safe to use this function from within an
 * interrupt service routine.
 *
 * Items are queued by copy not reference so it is preferable to only
 * queue small items, especially when called from an ISR.  In most cases
 * it would be preferable to store a pointer to the item being queued.
 *
 * @param xQueue The handle to the queue on which the item is to be posted.
 *
 * @param pvItemToQueue A pointer to the item that is to be placed on the
 * queue.  The size of the items the queue will hold was defined when the
 * queue was created, so this many bytes will be copied from pvItemToQueue
 * into the queue storage area.
 *
 * @param pxHigherPriorityTaskWoken xQueueGenericSendFromISR() will set
 * *pxHigherPriorityTaskWoken to pdTRUE if sending to the queue caused a task
 * to unblock, and the unblocked task has a priority higher than the currently
 * running task.  If xQueueGenericSendFromISR() sets this value to pdTRUE then
 * a context switch should be requested before the interrupt is exited.
 *
 * @param xCopyPosition Can take the value queueSEND_TO_BACK to place the
 * item at the back of the queue, or queueSEND_TO_FRONT to place the item
 * at the front of the queue (for high priority messages).
 *
 * @return pdTRUE if the data was successfully sent to the queue, otherwise
 * errQUEUE_FULL.
 *
 * Example usage for buffered IO (where the ISR can obtain more than one value
 * per call):
   <pre>
 void vBufferISR( void )
 {
 char cIn;
 BaseType_t xHigherPriorityTaskWokenByPost;

	// We have not woken a task at the start of the ISR.
	xHigherPriorityTaskWokenByPost = pdFALSE;

	// Loop until the buffer is empty.
	do
	{
		// Obtain a byte from the buffer.
		cIn = portINPUT_BYTE( RX_REGISTER_ADDRESS );

		// Post each byte.
		xQueueGenericSendFromISR( xRxQueue, &cIn, &xHigherPriorityTaskWokenByPost, queueSEND_TO_BACK );

	} while( portINPUT_BYTE( BUFFER_COUNT ) );

	// Now the buffer is empty we can switch context if necessary.  Note that the
	// name of the yield function required is port specific.
	if( xHigherPriorityTaskWokenByPost )
	{
		taskYIELD_YIELD_FROM_ISR();
	}
 }
 </pre>
 *
 * \defgroup xQueueSendFromISR xQueueSendFromISR
 * \ingroup QueueManagement
 */
BaseType_t xQueueGenericSendFromISR( QueueHandle_t xQueue, const void * const pvItemToQueue, BaseType_t * const pxHigherPriorityTaskWoken, const BaseType_t xCopyPosition ) ;

/**
 * queue. h
 * <pre>
 BaseType_t xQueueReceiveFromISR(
									   QueueHandle_t	xQueue,
									   void	*pvBuffer,
									   BaseType_t *pxTaskWoken
								   );
 * </pre>
 *
 * Receive an item from a queue.  It is safe to use this function from within an
 * interrupt service routine.
 *
 * @param xQueue The handle to the queue from which the item is to be
 * received.
 *
 * @param pvBuffer Pointer to the buffer into which the received item will
 * be copied.
 *
 * @param pxTaskWoken A task may be blocked waiting for space to become
 * available on the queue.  If xQueueReceiveFromISR causes such a task to
 * unblock *pxTaskWoken will get set to pdTRUE, otherwise *pxTaskWoken will
 * remain unchanged.
 *
 * @return pdTRUE if an item was successfully received from the queue,
 * otherwise pdFALSE.
 *
 * Example usage:
   <pre>

 QueueHandle_t xQueue;

 // Function to create a queue and post some values.
 void vAFunction( void *pvParameters )
 {
 char cValueToPost;
 const TickType_t xTicksToWait = ( TickType_t )0xff;

	// Create a queue capable of containing 10 characters.
	xQueue = xQueueCreate( 10, sizeof( char ) );
	if( xQueue == 0 )
	{
		// Failed to create the queue.
	}

	// ...

	// Post some characters that will be used within an ISR.  If the queue
	// is full then this task will block for xTicksToWait ticks.
	cValueToPost = 'a';
	xQueueSend( xQueue, ( void * ) &cValueToPost, xTicksToWait );
	cValueToPost = 'b';
	xQueueSend( xQueue, ( void * ) &cValueToPost, xTicksToWait );

	// ... keep posting characters ... this task may block when the queue
	// becomes full.

	cValueToPost = 'c';
	xQueueSend( xQueue, ( void * ) &cValueToPost, xTicksToWait );
 }

 // ISR that outputs all the characters received on the queue.
 void vISR_Routine( void )
 {
 BaseType_t xTaskWokenByReceive = pdFALSE;
 char cRxedChar;

	while( xQueueReceiveFromISR( xQueue, ( void * ) &cRxedChar, &xTaskWokenByReceive) )
	{
		// A character was received.  Output the character now.
		vOutputCharacter( cRxedChar );

		// If removing the character from the queue woke the task that was
		// posting onto the queue cTaskWokenByReceive will have been set to
		// pdTRUE.  No matter how many times this loop iterates only one
		// task will be woken.
	}

	if( cTaskWokenByPost != ( char ) pdFALSE;
	{
		taskYIELD ();
	}
 }
 </pre>
 * \defgroup xQueueReceiveFromISR xQueueReceiveFromISR
 * \ingroup QueueManagement
 */
BaseType_t xQueueReceiveFromISR( QueueHandle_t xQueue, void * const pvBuffer, BaseType_t * const pxHigherPriorityTaskWoken ) ;

/*
 * Utilities to query queues that are safe to use from an ISR.  These utilities
 * should be used only from witin an ISR, or within a critical section.
 */
BaseType_t xQueueIsQueueEmptyFromISR( const QueueHandle_t xQueue ) ;
BaseType_t xQueueIsQueueFullFromISR( const QueueHandle_t xQueue ) ;
UBaseType_t uxQueueMessagesWaitingFromISR( const QueueHandle_t xQueue ) ;


/*
 * xQueueAltGenericSend() is an alternative version of xQueueGenericSend().
 * Likewise xQueueAltGenericReceive() is an alternative version of
 * xQueueGenericReceive().
 *
 * The source code that implements the alternative (Alt) API is much
 * simpler	because it executes everything from within a critical section.
 * This is	the approach taken by many other RTOSes, but FreeRTOS.org has the
 * preferred fully featured API too.  The fully featured API has more
 * complex	code that takes longer to execute, but makes much less use of
 * critical sections.  Therefore the alternative API sacrifices interrupt
 * responsiveness to gain execution speed, whereas the fully featured API
 * sacrifices execution speed to ensure better interrupt responsiveness.
 */
BaseType_t xQueueAltGenericSend( QueueHandle_t xQueue, const void * const pvItemToQueue, TickType_t xTicksToWait, BaseType_t xCopyPosition );
BaseType_t xQueueAltGenericReceive( QueueHandle_t xQueue, void * const pvBuffer, TickType_t xTicksToWait, BaseType_t xJustPeeking );





/*
 * The functions defined above are for passing data to and from tasks.  The
 * functions below are the equivalents for passing data to and from
 * co-routines.
 *
 * These functions are called from the co-routine macro implementation and
 * should not be called directly from application code.  Instead use the macro
 * wrappers defined within croutine.h.
 */
BaseType_t xQueueCRSendFromISR( QueueHandle_t xQueue, const void *pvItemToQueue, BaseType_t xCoRoutinePreviouslyWoken );
BaseType_t xQueueCRReceiveFromISR( QueueHandle_t xQueue, void *pvBuffer, BaseType_t *pxTaskWoken );
BaseType_t xQueueCRSend( QueueHandle_t xQueue, const void *pvItemToQueue, TickType_t xTicksToWait );
BaseType_t xQueueCRReceive( QueueHandle_t xQueue, void *pvBuffer, TickType_t xTicksToWait );

/*
 * For internal use only.  Use xSemaphoreCreateMutex(),
 * xSemaphoreCreateCounting() or xSemaphoreGetMutexHolder() instead of calling
 * these functions directly.
 */
QueueHandle_t xQueueCreateMutex( const uint8_t ucQueueType ) ;
QueueHandle_t xQueueCreateCountingSemaphore( const UBaseType_t uxMaxCount, const UBaseType_t uxInitialCount ) ;
void* xQueueGetMutexHolder( QueueHandle_t xSemaphore ) ;

/*
 * For internal use only.  Use xSemaphoreTakeMutexRecursive() or
 * xSemaphoreGiveMutexRecursive() instead of calling these functions directly.
 */
BaseType_t xQueueTakeMutexRecursive( QueueHandle_t xMutex, TickType_t xTicksToWait ) ;
BaseType_t xQueueGiveMutexRecursive( QueueHandle_t pxMutex ) ;

/*
 * Reset a queue back to its original empty state.  pdPASS is returned if the
 * queue is successfully reset.  pdFAIL is returned if the queue could not be
 * reset because there are tasks blocked on the queue waiting to either
 * receive from the queue or send to the queue.
 */


/*
 * The registry is provided as a means for kernel aware debuggers to
 * locate queues, semaphores and mutexes.  Call vQueueAddToRegistry() add
 * a queue, semaphore or mutex handle to the registry if you want the handle
 * to be available to a kernel aware debugger.  If you are not using a kernel
 * aware debugger then this function can be ignored.
 *
 * configQUEUE_REGISTRY_SIZE defines the maximum number of handles the
 * registry can hold.  configQUEUE_REGISTRY_SIZE must be greater than 0
 * within FreeRTOSConfig.h for the registry to be available.  Its value
 * does not effect the number of queues, semaphores and mutexes that can be
 * created - just the number that the registry can hold.
 *
 * @param xQueue The handle of the queue being added to the registry.  This
 * is the handle returned by a call to xQueueCreate().  Semaphore and mutex
 * handles can also be passed in here.
 *
 * @param pcName The name to be associated with the handle.  This is the
 * name that the kernel aware debugger will display.  The queue registry only
 * stores a pointer to the string - so the string must be persistent (global or
 * preferably in ROM/Flash), not on the stack.
 */

	void vQueueAddToRegistry( QueueHandle_t xQueue, const char *pcName ) ; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */


/*
 * The registry is provided as a means for kernel aware debuggers to
 * locate queues, semaphores and mutexes.  Call vQueueAddToRegistry() add
 * a queue, semaphore or mutex handle to the registry if you want the handle
 * to be available to a kernel aware debugger, and vQueueUnregisterQueue() to
 * remove the queue, semaphore or mutex from the register.  If you are not using
 * a kernel aware debugger then this function can be ignored.
 *
 * @param xQueue The handle of the queue being removed from the registry.
 */

	void vQueueUnregisterQueue( QueueHandle_t xQueue ) ;


/*
 * Generic version of the queue creation function, which is in turn called by
 * any queue, semaphore or mutex creation function or macro.
 */
QueueHandle_t xQueueGenericCreate( const UBaseType_t uxQueueLength, const UBaseType_t uxItemSize, const uint8_t ucQueueType ) ;

/*
 * Queue sets provide a mechanism to allow a task to block (pend) on a read
 * operation from multiple queues or semaphores simultaneously.
 *
 * See FreeRTOS/Source/Demo/Common/Minimal/QueueSet.c for an example using this
 * function.
 *
 * A queue set must be explicitly created using a call to xQueueCreateSet()
 * before it can be used.  Once created, standard FreeRTOS queues and semaphores
 * can be added to the set using calls to xQueueAddToSet().
 * xQueueSelectFromSet() is then used to determine which, if any, of the queues
 * or semaphores contained in the set is in a state where a queue read or
 * semaphore take operation would be successful.
 *
 * Note 1:  See the documentation on http://wwwFreeRTOS.org/RTOS-queue-sets.html
 * for reasons why queue sets are very rarely needed in practice as there are
 * simpler methods of blocking on multiple objects.
 *
 * Note 2:  Blocking on a queue set that contains a mutex will not cause the
 * mutex holder to inherit the priority of the blocked task.
 *
 * Note 3:  An additional 4 bytes of RAM is required for each space in a every
 * queue added to a queue set.  Therefore counting semaphores that have a high
 * maximum count value should not be added to a queue set.
 *
 * Note 4:  A receive (in the case of a queue) or take (in the case of a
 * semaphore) operation must not be performed on a member of a queue set unless
 * a call to xQueueSelectFromSet() has first returned a handle to that set member.
 *
 * @param uxEventQueueLength Queue sets store events that occur on
 * the queues and semaphores contained in the set.  uxEventQueueLength specifies
 * the maximum number of events that can be queued at once.  To be absolutely
 * certain that events are not lost uxEventQueueLength should be set to the
 * total sum of the length of the queues added to the set, where binary
 * semaphores and mutexes have a length of 1, and counting semaphores have a
 * length set by their maximum count value.  Examples:
 *  + If a queue set is to hold a queue of length 5, another queue of length 12,
 *    and a binary semaphore, then uxEventQueueLength should be set to
 *    (5 + 12 + 1), or 18.
 *  + If a queue set is to hold three binary semaphores then uxEventQueueLength
 *    should be set to (1 + 1 + 1 ), or 3.
 *  + If a queue set is to hold a counting semaphore that has a maximum count of
 *    5, and a counting semaphore that has a maximum count of 3, then
 *    uxEventQueueLength should be set to (5 + 3), or 8.
 *
 * @return If the queue set is created successfully then a handle to the created
 * queue set is returned.  Otherwise NULL is returned.
 */
QueueSetHandle_t xQueueCreateSet( const UBaseType_t uxEventQueueLength ) ;

/*
 * Adds a queue or semaphore to a queue set that was previously created by a
 * call to xQueueCreateSet().
 *
 * See FreeRTOS/Source/Demo/Common/Minimal/QueueSet.c for an example using this
 * function.
 *
 * Note 1:  A receive (in the case of a queue) or take (in the case of a
 * semaphore) operation must not be performed on a member of a queue set unless
 * a call to xQueueSelectFromSet() has first returned a handle to that set member.
 *
 * @param xQueueOrSemaphore The handle of the queue or semaphore being added to
 * the queue set (cast to an QueueSetMemberHandle_t type).
 *
 * @param xQueueSet The handle of the queue set to which the queue or semaphore
 * is being added.
 *
 * @return If the queue or semaphore was successfully added to the queue set
 * then pdPASS is returned.  If the queue could not be successfully added to the
 * queue set because it is already a member of a different queue set then pdFAIL
 * is returned.
 */
BaseType_t xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore, QueueSetHandle_t xQueueSet ) ;

/*
 * Removes a queue or semaphore from a queue set.  A queue or semaphore can only
 * be removed from a set if the queue or semaphore is empty.
 *
 * See FreeRTOS/Source/Demo/Common/Minimal/QueueSet.c for an example using this
 * function.
 *
 * @param xQueueOrSemaphore The handle of the queue or semaphore being removed
 * from the queue set (cast to an QueueSetMemberHandle_t type).
 *
 * @param xQueueSet The handle of the queue set in which the queue or semaphore
 * is included.
 *
 * @return If the queue or semaphore was successfully removed from the queue set
 * then pdPASS is returned.  If the queue was not in the queue set, or the
 * queue (or semaphore) was not empty, then pdFAIL is returned.
 */
BaseType_t xQueueRemoveFromSet( QueueSetMemberHandle_t xQueueOrSemaphore, QueueSetHandle_t xQueueSet ) ;

/*
 * xQueueSelectFromSet() selects from the members of a queue set a queue or
 * semaphore that either contains data (in the case of a queue) or is available
 * to take (in the case of a semaphore).  xQueueSelectFromSet() effectively
 * allows a task to block (pend) on a read operation on all the queues and
 * semaphores in a queue set simultaneously.
 *
 * See FreeRTOS/Source/Demo/Common/Minimal/QueueSet.c for an example using this
 * function.
 *
 * Note 1:  See the documentation on http://wwwFreeRTOS.org/RTOS-queue-sets.html
 * for reasons why queue sets are very rarely needed in practice as there are
 * simpler methods of blocking on multiple objects.
 *
 * Note 2:  Blocking on a queue set that contains a mutex will not cause the
 * mutex holder to inherit the priority of the blocked task.
 *
 * Note 3:  A receive (in the case of a queue) or take (in the case of a
 * semaphore) operation must not be performed on a member of a queue set unless
 * a call to xQueueSelectFromSet() has first returned a handle to that set member.
 *
 * @param xQueueSet The queue set on which the task will (potentially) block.
 *
 * @param xTicksToWait The maximum time, in ticks, that the calling task will
 * remain in the Blocked state (with other tasks executing) to wait for a member
 * of the queue set to be ready for a successful queue read or semaphore take
 * operation.
 *
 * @return xQueueSelectFromSet() will return the handle of a queue (cast to
 * a QueueSetMemberHandle_t type) contained in the queue set that contains data,
 * or the handle of a semaphore (cast to a QueueSetMemberHandle_t type) contained
 * in the queue set that is available, or NULL if no such queue or semaphore
 * exists before before the specified block time expires.
 */
QueueSetMemberHandle_t xQueueSelectFromSet( QueueSetHandle_t xQueueSet, const TickType_t xTicksToWait ) ;

/*
 * A version of xQueueSelectFromSet() that can be used from an ISR.
 */
QueueSetMemberHandle_t xQueueSelectFromSetFromISR( QueueSetHandle_t xQueueSet ) ;

/* Not public API functions. */
void vQueueWaitForMessageRestricted( QueueHandle_t xQueue, TickType_t xTicksToWait ) ;
BaseType_t xQueueGenericReset( QueueHandle_t xQueue, BaseType_t xNewQueue ) ;
void vQueueSetQueueNumber( QueueHandle_t xQueue, UBaseType_t uxQueueNumber ) ;
UBaseType_t uxQueueGetQueueNumber( QueueHandle_t xQueue ) ;
uint8_t ucQueueGetQueueType( QueueHandle_t xQueue ) ;








#line 38 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/sys_arch.h"
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\semphr.h"
/*
    FreeRTOS V8.1.2 - Copyright (C) 2014 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that has become a de facto standard.             *
     *                                                                       *
     *    Help yourself get started quickly and support the FreeRTOS         *
     *    project by purchasing a FreeRTOS tutorial book, reference          *
     *    manual, or both from: http://www.FreeRTOS.org/Documentation        *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available from the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?"                                     *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/










typedef QueueHandle_t SemaphoreHandle_t;






/**
 * semphr. h
 * <pre>vSemaphoreCreateBinary( SemaphoreHandle_t xSemaphore )</pre>
 *
 * This old vSemaphoreCreateBinary() macro is now deprecated in favour of the
 * xSemaphoreCreateBinary() function.  Note that binary semaphores created using
 * the vSemaphoreCreateBinary() macro are created in a state such that the
 * first call to 'take' the semaphore would pass, whereas binary semaphores
 * created using xSemaphoreCreateBinary() are created in a state such that the
 * the semaphore must first be 'given' before it can be 'taken'.
 *
 * <i>Macro</i> that implements a semaphore by using the existing queue mechanism.
 * The queue length is 1 as this is a binary semaphore.  The data size is 0
 * as we don't want to actually store any data - we just want to know if the
 * queue is empty or full.
 *
 * This type of semaphore can be used for pure synchronisation between tasks or
 * between an interrupt and a task.  The semaphore need not be given back once
 * obtained, so one task/interrupt can continuously 'give' the semaphore while
 * another continuously 'takes' the semaphore.  For this reason this type of
 * semaphore does not use a priority inheritance mechanism.  For an alternative
 * that does use priority inheritance see xSemaphoreCreateMutex().
 *
 * @param xSemaphore Handle to the created semaphore.  Should be of type SemaphoreHandle_t.
 *
 * Example usage:
 <pre>
 SemaphoreHandle_t xSemaphore = NULL;

 void vATask( void * pvParameters )
 {
    // Semaphore cannot be used before a call to vSemaphoreCreateBinary ().
    // This is a macro so pass the variable in directly.
    vSemaphoreCreateBinary( xSemaphore );

    if( xSemaphore != NULL )
    {
        // The semaphore was created successfully.
        // The semaphore can now be used.
    }
 }
 </pre>
 * \defgroup vSemaphoreCreateBinary vSemaphoreCreateBinary
 * \ingroup Semaphores
 */
#line 135 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\FreeRTOSV8.1.2\\include\\semphr.h"

/**
 * semphr. h
 * <pre>SemaphoreHandle_t xSemaphoreCreateBinary( void )</pre>
 *
 * The old vSemaphoreCreateBinary() macro is now deprecated in favour of this
 * xSemaphoreCreateBinary() function.  Note that binary semaphores created using
 * the vSemaphoreCreateBinary() macro are created in a state such that the
 * first call to 'take' the semaphore would pass, whereas binary semaphores
 * created using xSemaphoreCreateBinary() are created in a state such that the
 * the semaphore must first be 'given' before it can be 'taken'.
 *
 * Function that creates a semaphore by using the existing queue mechanism.
 * The queue length is 1 as this is a binary semaphore.  The data size is 0
 * as nothing is actually stored - all that is important is whether the queue is
 * empty or full (the binary semaphore is available or not).
 *
 * This type of semaphore can be used for pure synchronisation between tasks or
 * between an interrupt and a task.  The semaphore need not be given back once
 * obtained, so one task/interrupt can continuously 'give' the semaphore while
 * another continuously 'takes' the semaphore.  For this reason this type of
 * semaphore does not use a priority inheritance mechanism.  For an alternative
 * that does use priority inheritance see xSemaphoreCreateMutex().
 *
 * @return Handle to the created semaphore.
 *
 * Example usage:
 <pre>
 SemaphoreHandle_t xSemaphore = NULL;

 void vATask( void * pvParameters )
 {
    // Semaphore cannot be used before a call to vSemaphoreCreateBinary ().
    // This is a macro so pass the variable in directly.
    xSemaphore = xSemaphoreCreateBinary();

    if( xSemaphore != NULL )
    {
        // The semaphore was created successfully.
        // The semaphore can now be used.
    }
 }
 </pre>
 * \defgroup vSemaphoreCreateBinary vSemaphoreCreateBinary
 * \ingroup Semaphores
 */


/**
 * semphr. h
 * <pre>xSemaphoreTake(
 *                   SemaphoreHandle_t xSemaphore,
 *                   TickType_t xBlockTime
 *               )</pre>
 *
 * <i>Macro</i> to obtain a semaphore.  The semaphore must have previously been
 * created with a call to vSemaphoreCreateBinary(), xSemaphoreCreateMutex() or
 * xSemaphoreCreateCounting().
 *
 * @param xSemaphore A handle to the semaphore being taken - obtained when
 * the semaphore was created.
 *
 * @param xBlockTime The time in ticks to wait for the semaphore to become
 * available.  The macro portTICK_PERIOD_MS can be used to convert this to a
 * real time.  A block time of zero can be used to poll the semaphore.  A block
 * time of portMAX_DELAY can be used to block indefinitely (provided
 * INCLUDE_vTaskSuspend is set to 1 in FreeRTOSConfig.h).
 *
 * @return pdTRUE if the semaphore was obtained.  pdFALSE
 * if xBlockTime expired without the semaphore becoming available.
 *
 * Example usage:
 <pre>
 SemaphoreHandle_t xSemaphore = NULL;

 // A task that creates a semaphore.
 void vATask( void * pvParameters )
 {
    // Create the semaphore to guard a shared resource.
    vSemaphoreCreateBinary( xSemaphore );
 }

 // A task that uses the semaphore.
 void vAnotherTask( void * pvParameters )
 {
    // ... Do other things.

    if( xSemaphore != NULL )
    {
        // See if we can obtain the semaphore.  If the semaphore is not available
        // wait 10 ticks to see if it becomes free.
        if( xSemaphoreTake( xSemaphore, ( TickType_t ) 10 ) == pdTRUE )
        {
            // We were able to obtain the semaphore and can now access the
            // shared resource.

            // ...

            // We have finished accessing the shared resource.  Release the
            // semaphore.
            xSemaphoreGive( xSemaphore );
        }
        else
        {
            // We could not obtain the semaphore and can therefore not access
            // the shared resource safely.
        }
    }
 }
 </pre>
 * \defgroup xSemaphoreTake xSemaphoreTake
 * \ingroup Semaphores
 */


/**
 * semphr. h
 * xSemaphoreTakeRecursive(
 *                          SemaphoreHandle_t xMutex,
 *                          TickType_t xBlockTime
 *                        )
 *
 * <i>Macro</i> to recursively obtain, or 'take', a mutex type semaphore.
 * The mutex must have previously been created using a call to
 * xSemaphoreCreateRecursiveMutex();
 *
 * configUSE_RECURSIVE_MUTEXES must be set to 1 in FreeRTOSConfig.h for this
 * macro to be available.
 *
 * This macro must not be used on mutexes created using xSemaphoreCreateMutex().
 *
 * A mutex used recursively can be 'taken' repeatedly by the owner. The mutex
 * doesn't become available again until the owner has called
 * xSemaphoreGiveRecursive() for each successful 'take' request.  For example,
 * if a task successfully 'takes' the same mutex 5 times then the mutex will
 * not be available to any other task until it has also  'given' the mutex back
 * exactly five times.
 *
 * @param xMutex A handle to the mutex being obtained.  This is the
 * handle returned by xSemaphoreCreateRecursiveMutex();
 *
 * @param xBlockTime The time in ticks to wait for the semaphore to become
 * available.  The macro portTICK_PERIOD_MS can be used to convert this to a
 * real time.  A block time of zero can be used to poll the semaphore.  If
 * the task already owns the semaphore then xSemaphoreTakeRecursive() will
 * return immediately no matter what the value of xBlockTime.
 *
 * @return pdTRUE if the semaphore was obtained.  pdFALSE if xBlockTime
 * expired without the semaphore becoming available.
 *
 * Example usage:
 <pre>
 SemaphoreHandle_t xMutex = NULL;

 // A task that creates a mutex.
 void vATask( void * pvParameters )
 {
    // Create the mutex to guard a shared resource.
    xMutex = xSemaphoreCreateRecursiveMutex();
 }

 // A task that uses the mutex.
 void vAnotherTask( void * pvParameters )
 {
    // ... Do other things.

    if( xMutex != NULL )
    {
        // See if we can obtain the mutex.  If the mutex is not available
        // wait 10 ticks to see if it becomes free.
        if( xSemaphoreTakeRecursive( xSemaphore, ( TickType_t ) 10 ) == pdTRUE )
        {
            // We were able to obtain the mutex and can now access the
            // shared resource.

            // ...
            // For some reason due to the nature of the code further calls to
			// xSemaphoreTakeRecursive() are made on the same mutex.  In real
			// code these would not be just sequential calls as this would make
			// no sense.  Instead the calls are likely to be buried inside
			// a more complex call structure.
            xSemaphoreTakeRecursive( xMutex, ( TickType_t ) 10 );
            xSemaphoreTakeRecursive( xMutex, ( TickType_t ) 10 );

            // The mutex has now been 'taken' three times, so will not be
			// available to another task until it has also been given back
			// three times.  Again it is unlikely that real code would have
			// these calls sequentially, but instead buried in a more complex
			// call structure.  This is just for illustrative purposes.
            xSemaphoreGiveRecursive( xMutex );
			xSemaphoreGiveRecursive( xMutex );
			xSemaphoreGiveRecursive( xMutex );

			// Now the mutex can be taken by other tasks.
        }
        else
        {
            // We could not obtain the mutex and can therefore not access
            // the shared resource safely.
        }
    }
 }
 </pre>
 * \defgroup xSemaphoreTakeRecursive xSemaphoreTakeRecursive
 * \ingroup Semaphores
 */



/*
 * xSemaphoreAltTake() is an alternative version of xSemaphoreTake().
 *
 * The source code that implements the alternative (Alt) API is much
 * simpler	because it executes everything from within a critical section.
 * This is	the approach taken by many other RTOSes, but FreeRTOS.org has the
 * preferred fully featured API too.  The fully featured API has more
 * complex	code that takes longer to execute, but makes much less use of
 * critical sections.  Therefore the alternative API sacrifices interrupt
 * responsiveness to gain execution speed, whereas the fully featured API
 * sacrifices execution speed to ensure better interrupt responsiveness.
 */


/**
 * semphr. h
 * <pre>xSemaphoreGive( SemaphoreHandle_t xSemaphore )</pre>
 *
 * <i>Macro</i> to release a semaphore.  The semaphore must have previously been
 * created with a call to vSemaphoreCreateBinary(), xSemaphoreCreateMutex() or
 * xSemaphoreCreateCounting(). and obtained using sSemaphoreTake().
 *
 * This macro must not be used from an ISR.  See xSemaphoreGiveFromISR () for
 * an alternative which can be used from an ISR.
 *
 * This macro must also not be used on semaphores created using
 * xSemaphoreCreateRecursiveMutex().
 *
 * @param xSemaphore A handle to the semaphore being released.  This is the
 * handle returned when the semaphore was created.
 *
 * @return pdTRUE if the semaphore was released.  pdFALSE if an error occurred.
 * Semaphores are implemented using queues.  An error can occur if there is
 * no space on the queue to post a message - indicating that the
 * semaphore was not first obtained correctly.
 *
 * Example usage:
 <pre>
 SemaphoreHandle_t xSemaphore = NULL;

 void vATask( void * pvParameters )
 {
    // Create the semaphore to guard a shared resource.
    vSemaphoreCreateBinary( xSemaphore );

    if( xSemaphore != NULL )
    {
        if( xSemaphoreGive( xSemaphore ) != pdTRUE )
        {
            // We would expect this call to fail because we cannot give
            // a semaphore without first "taking" it!
        }

        // Obtain the semaphore - don't block if the semaphore is not
        // immediately available.
        if( xSemaphoreTake( xSemaphore, ( TickType_t ) 0 ) )
        {
            // We now have the semaphore and can access the shared resource.

            // ...

            // We have finished accessing the shared resource so can free the
            // semaphore.
            if( xSemaphoreGive( xSemaphore ) != pdTRUE )
            {
                // We would not expect this call to fail because we must have
                // obtained the semaphore to get here.
            }
        }
    }
 }
 </pre>
 * \defgroup xSemaphoreGive xSemaphoreGive
 * \ingroup Semaphores
 */


/**
 * semphr. h
 * <pre>xSemaphoreGiveRecursive( SemaphoreHandle_t xMutex )</pre>
 *
 * <i>Macro</i> to recursively release, or 'give', a mutex type semaphore.
 * The mutex must have previously been created using a call to
 * xSemaphoreCreateRecursiveMutex();
 *
 * configUSE_RECURSIVE_MUTEXES must be set to 1 in FreeRTOSConfig.h for this
 * macro to be available.
 *
 * This macro must not be used on mutexes created using xSemaphoreCreateMutex().
 *
 * A mutex used recursively can be 'taken' repeatedly by the owner. The mutex
 * doesn't become available again until the owner has called
 * xSemaphoreGiveRecursive() for each successful 'take' request.  For example,
 * if a task successfully 'takes' the same mutex 5 times then the mutex will
 * not be available to any other task until it has also  'given' the mutex back
 * exactly five times.
 *
 * @param xMutex A handle to the mutex being released, or 'given'.  This is the
 * handle returned by xSemaphoreCreateMutex();
 *
 * @return pdTRUE if the semaphore was given.
 *
 * Example usage:
 <pre>
 SemaphoreHandle_t xMutex = NULL;

 // A task that creates a mutex.
 void vATask( void * pvParameters )
 {
    // Create the mutex to guard a shared resource.
    xMutex = xSemaphoreCreateRecursiveMutex();
 }

 // A task that uses the mutex.
 void vAnotherTask( void * pvParameters )
 {
    // ... Do other things.

    if( xMutex != NULL )
    {
        // See if we can obtain the mutex.  If the mutex is not available
        // wait 10 ticks to see if it becomes free.
        if( xSemaphoreTakeRecursive( xMutex, ( TickType_t ) 10 ) == pdTRUE )
        {
            // We were able to obtain the mutex and can now access the
            // shared resource.

            // ...
            // For some reason due to the nature of the code further calls to
			// xSemaphoreTakeRecursive() are made on the same mutex.  In real
			// code these would not be just sequential calls as this would make
			// no sense.  Instead the calls are likely to be buried inside
			// a more complex call structure.
            xSemaphoreTakeRecursive( xMutex, ( TickType_t ) 10 );
            xSemaphoreTakeRecursive( xMutex, ( TickType_t ) 10 );

            // The mutex has now been 'taken' three times, so will not be
			// available to another task until it has also been given back
			// three times.  Again it is unlikely that real code would have
			// these calls sequentially, it would be more likely that the calls
			// to xSemaphoreGiveRecursive() would be called as a call stack
			// unwound.  This is just for demonstrative purposes.
            xSemaphoreGiveRecursive( xMutex );
			xSemaphoreGiveRecursive( xMutex );
			xSemaphoreGiveRecursive( xMutex );

			// Now the mutex can be taken by other tasks.
        }
        else
        {
            // We could not obtain the mutex and can therefore not access
            // the shared resource safely.
        }
    }
 }
 </pre>
 * \defgroup xSemaphoreGiveRecursive xSemaphoreGiveRecursive
 * \ingroup Semaphores
 */


/*
 * xSemaphoreAltGive() is an alternative version of xSemaphoreGive().
 *
 * The source code that implements the alternative (Alt) API is much
 * simpler	because it executes everything from within a critical section.
 * This is	the approach taken by many other RTOSes, but FreeRTOS.org has the
 * preferred fully featured API too.  The fully featured API has more
 * complex	code that takes longer to execute, but makes much less use of
 * critical sections.  Therefore the alternative API sacrifices interrupt
 * responsiveness to gain execution speed, whereas the fully featured API
 * sacrifices execution speed to ensure better interrupt responsiveness.
 */


/**
 * semphr. h
 * <pre>
 xSemaphoreGiveFromISR(
                          SemaphoreHandle_t xSemaphore,
                          BaseType_t *pxHigherPriorityTaskWoken
                      )</pre>
 *
 * <i>Macro</i> to  release a semaphore.  The semaphore must have previously been
 * created with a call to vSemaphoreCreateBinary() or xSemaphoreCreateCounting().
 *
 * Mutex type semaphores (those created using a call to xSemaphoreCreateMutex())
 * must not be used with this macro.
 *
 * This macro can be used from an ISR.
 *
 * @param xSemaphore A handle to the semaphore being released.  This is the
 * handle returned when the semaphore was created.
 *
 * @param pxHigherPriorityTaskWoken xSemaphoreGiveFromISR() will set
 * *pxHigherPriorityTaskWoken to pdTRUE if giving the semaphore caused a task
 * to unblock, and the unblocked task has a priority higher than the currently
 * running task.  If xSemaphoreGiveFromISR() sets this value to pdTRUE then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return pdTRUE if the semaphore was successfully given, otherwise errQUEUE_FULL.
 *
 * Example usage:
 <pre>
 \#define LONG_TIME 0xffff
 \#define TICKS_TO_WAIT	10
 SemaphoreHandle_t xSemaphore = NULL;

 // Repetitive task.
 void vATask( void * pvParameters )
 {
    for( ;; )
    {
        // We want this task to run every 10 ticks of a timer.  The semaphore
        // was created before this task was started.

        // Block waiting for the semaphore to become available.
        if( xSemaphoreTake( xSemaphore, LONG_TIME ) == pdTRUE )
        {
            // It is time to execute.

            // ...

            // We have finished our task.  Return to the top of the loop where
            // we will block on the semaphore until it is time to execute
            // again.  Note when using the semaphore for synchronisation with an
			// ISR in this manner there is no need to 'give' the semaphore back.
        }
    }
 }

 // Timer ISR
 void vTimerISR( void * pvParameters )
 {
 static uint8_t ucLocalTickCount = 0;
 static BaseType_t xHigherPriorityTaskWoken;

    // A timer tick has occurred.

    // ... Do other time functions.

    // Is it time for vATask () to run?
	xHigherPriorityTaskWoken = pdFALSE;
    ucLocalTickCount++;
    if( ucLocalTickCount >= TICKS_TO_WAIT )
    {
        // Unblock the task by releasing the semaphore.
        xSemaphoreGiveFromISR( xSemaphore, &xHigherPriorityTaskWoken );

        // Reset the count so we release the semaphore again in 10 ticks time.
        ucLocalTickCount = 0;
    }

    if( xHigherPriorityTaskWoken != pdFALSE )
    {
        // We can force a context switch here.  Context switching from an
        // ISR uses port specific syntax.  Check the demo task for your port
        // to find the syntax required.
    }
 }
 </pre>
 * \defgroup xSemaphoreGiveFromISR xSemaphoreGiveFromISR
 * \ingroup Semaphores
 */


/**
 * semphr. h
 * <pre>
 xSemaphoreTakeFromISR(
                          SemaphoreHandle_t xSemaphore,
                          BaseType_t *pxHigherPriorityTaskWoken
                      )</pre>
 *
 * <i>Macro</i> to  take a semaphore from an ISR.  The semaphore must have
 * previously been created with a call to vSemaphoreCreateBinary() or
 * xSemaphoreCreateCounting().
 *
 * Mutex type semaphores (those created using a call to xSemaphoreCreateMutex())
 * must not be used with this macro.
 *
 * This macro can be used from an ISR, however taking a semaphore from an ISR
 * is not a common operation.  It is likely to only be useful when taking a
 * counting semaphore when an interrupt is obtaining an object from a resource
 * pool (when the semaphore count indicates the number of resources available).
 *
 * @param xSemaphore A handle to the semaphore being taken.  This is the
 * handle returned when the semaphore was created.
 *
 * @param pxHigherPriorityTaskWoken xSemaphoreTakeFromISR() will set
 * *pxHigherPriorityTaskWoken to pdTRUE if taking the semaphore caused a task
 * to unblock, and the unblocked task has a priority higher than the currently
 * running task.  If xSemaphoreTakeFromISR() sets this value to pdTRUE then
 * a context switch should be requested before the interrupt is exited.
 *
 * @return pdTRUE if the semaphore was successfully taken, otherwise
 * pdFALSE
 */


/**
 * semphr. h
 * <pre>SemaphoreHandle_t xSemaphoreCreateMutex( void )</pre>
 *
 * <i>Macro</i> that implements a mutex semaphore by using the existing queue
 * mechanism.
 *
 * Mutexes created using this macro can be accessed using the xSemaphoreTake()
 * and xSemaphoreGive() macros.  The xSemaphoreTakeRecursive() and
 * xSemaphoreGiveRecursive() macros should not be used.
 *
 * This type of semaphore uses a priority inheritance mechanism so a task
 * 'taking' a semaphore MUST ALWAYS 'give' the semaphore back once the
 * semaphore it is no longer required.
 *
 * Mutex type semaphores cannot be used from within interrupt service routines.
 *
 * See vSemaphoreCreateBinary() for an alternative implementation that can be
 * used for pure synchronisation (where one task or interrupt always 'gives' the
 * semaphore and another always 'takes' the semaphore) and from within interrupt
 * service routines.
 *
 * @return xSemaphore Handle to the created mutex semaphore.  Should be of type
 *		SemaphoreHandle_t.
 *
 * Example usage:
 <pre>
 SemaphoreHandle_t xSemaphore;

 void vATask( void * pvParameters )
 {
    // Semaphore cannot be used before a call to xSemaphoreCreateMutex().
    // This is a macro so pass the variable in directly.
    xSemaphore = xSemaphoreCreateMutex();

    if( xSemaphore != NULL )
    {
        // The semaphore was created successfully.
        // The semaphore can now be used.
    }
 }
 </pre>
 * \defgroup vSemaphoreCreateMutex vSemaphoreCreateMutex
 * \ingroup Semaphores
 */



/**
 * semphr. h
 * <pre>SemaphoreHandle_t xSemaphoreCreateRecursiveMutex( void )</pre>
 *
 * <i>Macro</i> that implements a recursive mutex by using the existing queue
 * mechanism.
 *
 * Mutexes created using this macro can be accessed using the
 * xSemaphoreTakeRecursive() and xSemaphoreGiveRecursive() macros.  The
 * xSemaphoreTake() and xSemaphoreGive() macros should not be used.
 *
 * A mutex used recursively can be 'taken' repeatedly by the owner. The mutex
 * doesn't become available again until the owner has called
 * xSemaphoreGiveRecursive() for each successful 'take' request.  For example,
 * if a task successfully 'takes' the same mutex 5 times then the mutex will
 * not be available to any other task until it has also  'given' the mutex back
 * exactly five times.
 *
 * This type of semaphore uses a priority inheritance mechanism so a task
 * 'taking' a semaphore MUST ALWAYS 'give' the semaphore back once the
 * semaphore it is no longer required.
 *
 * Mutex type semaphores cannot be used from within interrupt service routines.
 *
 * See vSemaphoreCreateBinary() for an alternative implementation that can be
 * used for pure synchronisation (where one task or interrupt always 'gives' the
 * semaphore and another always 'takes' the semaphore) and from within interrupt
 * service routines.
 *
 * @return xSemaphore Handle to the created mutex semaphore.  Should be of type
 *		SemaphoreHandle_t.
 *
 * Example usage:
 <pre>
 SemaphoreHandle_t xSemaphore;

 void vATask( void * pvParameters )
 {
    // Semaphore cannot be used before a call to xSemaphoreCreateMutex().
    // This is a macro so pass the variable in directly.
    xSemaphore = xSemaphoreCreateRecursiveMutex();

    if( xSemaphore != NULL )
    {
        // The semaphore was created successfully.
        // The semaphore can now be used.
    }
 }
 </pre>
 * \defgroup vSemaphoreCreateMutex vSemaphoreCreateMutex
 * \ingroup Semaphores
 */


/**
 * semphr. h
 * <pre>SemaphoreHandle_t xSemaphoreCreateCounting( UBaseType_t uxMaxCount, UBaseType_t uxInitialCount )</pre>
 *
 * <i>Macro</i> that creates a counting semaphore by using the existing
 * queue mechanism.
 *
 * Counting semaphores are typically used for two things:
 *
 * 1) Counting events.
 *
 *    In this usage scenario an event handler will 'give' a semaphore each time
 *    an event occurs (incrementing the semaphore count value), and a handler
 *    task will 'take' a semaphore each time it processes an event
 *    (decrementing the semaphore count value).  The count value is therefore
 *    the difference between the number of events that have occurred and the
 *    number that have been processed.  In this case it is desirable for the
 *    initial count value to be zero.
 *
 * 2) Resource management.
 *
 *    In this usage scenario the count value indicates the number of resources
 *    available.  To obtain control of a resource a task must first obtain a
 *    semaphore - decrementing the semaphore count value.  When the count value
 *    reaches zero there are no free resources.  When a task finishes with the
 *    resource it 'gives' the semaphore back - incrementing the semaphore count
 *    value.  In this case it is desirable for the initial count value to be
 *    equal to the maximum count value, indicating that all resources are free.
 *
 * @param uxMaxCount The maximum count value that can be reached.  When the
 *        semaphore reaches this value it can no longer be 'given'.
 *
 * @param uxInitialCount The count value assigned to the semaphore when it is
 *        created.
 *
 * @return Handle to the created semaphore.  Null if the semaphore could not be
 *         created.
 *
 * Example usage:
 <pre>
 SemaphoreHandle_t xSemaphore;

 void vATask( void * pvParameters )
 {
 SemaphoreHandle_t xSemaphore = NULL;

    // Semaphore cannot be used before a call to xSemaphoreCreateCounting().
    // The max value to which the semaphore can count should be 10, and the
    // initial value assigned to the count should be 0.
    xSemaphore = xSemaphoreCreateCounting( 10, 0 );

    if( xSemaphore != NULL )
    {
        // The semaphore was created successfully.
        // The semaphore can now be used.
    }
 }
 </pre>
 * \defgroup xSemaphoreCreateCounting xSemaphoreCreateCounting
 * \ingroup Semaphores
 */


/**
 * semphr. h
 * <pre>void vSemaphoreDelete( SemaphoreHandle_t xSemaphore );</pre>
 *
 * Delete a semaphore.  This function must be used with care.  For example,
 * do not delete a mutex type semaphore if the mutex is held by a task.
 *
 * @param xSemaphore A handle to the semaphore to be deleted.
 *
 * \defgroup vSemaphoreDelete vSemaphoreDelete
 * \ingroup Semaphores
 */


/**
 * semphr.h
 * <pre>TaskHandle_t xSemaphoreGetMutexHolder( SemaphoreHandle_t xMutex );</pre>
 *
 * If xMutex is indeed a mutex type semaphore, return the current mutex holder.
 * If xMutex is not a mutex type semaphore, or the mutex is available (not held
 * by a task), return NULL.
 *
 * Note: This is a good way of determining if the calling task is the mutex
 * holder, but not a good way of determining the identity of the mutex holder as
 * the holder may change between the function exiting and the returned value
 * being tested.
 */





#line 39 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/sys_arch.h"





typedef SemaphoreHandle_t sys_sem_t;
typedef QueueHandle_t sys_mbox_t;
typedef TaskHandle_t sys_thread_t;

typedef struct _sys_arch_state_t
{
	// Task creation data.
	char cTaskName[( 16 )];
	unsigned short nStackDepth;
	unsigned short nTaskCount;
} sys_arch_state_t;



//extern sys_arch_state_t s_sys_arch_state;

//void sys_set_default_state();
//void sys_set_state(signed char *pTaskName, unsigned short nStackSize);

/* Message queue constants. */



#line 79 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/sys.h"

typedef void (* sys_timeout_handler)(void *arg);

struct sys_timeo {
  struct sys_timeo *next;
  u32_t time;
  sys_timeout_handler h;
  void *arg;
};

struct sys_timeouts {
  struct sys_timeo *next;
};

/* sys_init() must be called before anthing else. */
void sys_init(void);

/*
 * sys_timeout():
 *
 * Schedule a timeout a specified amount of milliseconds in the
 * future. When the timeout occurs, the specified timeout handler will
 * be called. The handler will be passed the "arg" argument when
 * called.
 *
 */
void sys_timeout(u32_t msecs, sys_timeout_handler h, void *arg);
void sys_untimeout(sys_timeout_handler h, void *arg);
struct sys_timeouts *sys_arch_timeouts(void);

/* Semaphore functions. */
sys_sem_t sys_sem_new(u8_t count);
void sys_sem_signal(sys_sem_t sem);
u32_t sys_arch_sem_wait(sys_sem_t sem, u32_t timeout);
void sys_sem_free(sys_sem_t sem);
void sys_sem_wait(sys_sem_t sem);
int sys_sem_wait_timeout(sys_sem_t sem, u32_t timeout);

/* Time functions. */

void sys_msleep(u32_t ms); /* only has a (close to) 1 jiffy resolution. */


u32_t sys_jiffies(void); /* since power up. */


/* Mailbox functions. */
sys_mbox_t sys_mbox_new(int size);
void sys_mbox_post(sys_mbox_t mbox, void *msg);
err_t sys_mbox_trypost(sys_mbox_t mbox, void *msg);
u32_t sys_arch_mbox_fetch(sys_mbox_t mbox, void **msg, u32_t timeout);

u32_t sys_arch_mbox_tryfetch(sys_mbox_t mbox, void **msg);

/* For now, we map straight to sys_arch implementation. */

void sys_mbox_free(sys_mbox_t mbox);
void sys_mbox_fetch(sys_mbox_t mbox, void **msg);

/* Thread functions. */
sys_thread_t sys_thread_new(char *name, void (* thread)(void *arg), void *arg, int stacksize, int prio);



/** Returns the current time in milliseconds. */
u32_t sys_now(void);

/* Critical Region Protection */
/* These functions must be implemented in the sys_arch.c file.
   In some implementations they can provide a more light-weight protection
   mechanism than using semaphores. Otherwise semaphores can be used for
   implementation */

/** SYS_LIGHTWEIGHT_PROT
 * define SYS_LIGHTWEIGHT_PROT in lwipopts.h if you want inter-task protection
 * for certain critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 */


/** SYS_ARCH_DECL_PROTECT
 * declare a protection variable. This macro will default to defining a variable of
 * type sys_prot_t. If a particular port needs a different implementation, then
 * this macro may be defined in sys_arch.h.
 */

/** SYS_ARCH_PROTECT
 * Perform a "fast" protect. This could be implemented by
 * disabling interrupts for an embedded system or by using a semaphore or
 * mutex. The implementation should allow calling SYS_ARCH_PROTECT when
 * already protected. The old protection level is returned in the variable
 * "lev". This macro will default to calling the sys_arch_protect() function
 * which should be implemented in sys_arch.c. If a particular port needs a
 * different implementation, then this macro may be defined in sys_arch.h
 */

/** SYS_ARCH_UNPROTECT
 * Perform a "fast" set of the protection level to "lev". This could be
 * implemented by setting the interrupt level to "lev" within the MACRO or by
 * using a semaphore or mutex.  This macro will default to calling the
 * sys_arch_unprotect() function which should be implemented in
 * sys_arch.c. If a particular port needs a different implementation, then
 * this macro may be defined in sys_arch.h
 */

sys_prot_t sys_arch_protect(void);
void sys_arch_unprotect(sys_prot_t pval);

#line 194 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/sys.h"



/*
 * Macros to set/get and increase/decrease variables in a thread-safe way.
 * Use these for accessing variable that are used from more than one thread.
 */

#line 210 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/sys.h"

#line 219 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/sys.h"

#line 228 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/sys.h"

#line 237 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/sys.h"






#line 43 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/api.h"







/* Throughout this file, IP addresses and port numbers are expected to be in
 * the same byte order as in the corresponding pcb.
 */

/* Flags for netconn_write */





/* Helpers to process several netconn_types by the same code */



enum netconn_type {
  NETCONN_INVALID    = 0,
  /* NETCONN_TCP Group */
  NETCONN_TCP        = 0x10,
  /* NETCONN_UDP Group */
  NETCONN_UDP        = 0x20,
  NETCONN_UDPLITE    = 0x21,
  NETCONN_UDPNOCHKSUM= 0x22,
  /* NETCONN_RAW Group */
  NETCONN_RAW        = 0x40
};

enum netconn_state {
  NETCONN_NONE,
  NETCONN_WRITE,
  NETCONN_LISTEN,
  NETCONN_CONNECT,
  NETCONN_CLOSE
};

enum netconn_evt {
  NETCONN_EVT_RCVPLUS,
  NETCONN_EVT_RCVMINUS,
  NETCONN_EVT_SENDPLUS,
  NETCONN_EVT_SENDMINUS
};


enum netconn_igmp {
  NETCONN_JOIN,
  NETCONN_LEAVE
};


/* forward-declare some structs to avoid to include their headers */
struct ip_pcb;
struct tcp_pcb;
struct udp_pcb;
struct raw_pcb;
struct netconn;

/** A callback prototype to inform about events for a netconn */
typedef void (* netconn_callback)(struct netconn *, enum netconn_evt, u16_t len);

/** A netconn descriptor */
struct netconn {
  /** type of the netconn (TCP, UDP or RAW) */
  enum netconn_type type;
  /** current state of the netconn */
  enum netconn_state state;
  /** the lwIP internal protocol control block */
  union {
    struct ip_pcb  *ip;
    struct tcp_pcb *tcp;
    struct udp_pcb *udp;
    struct raw_pcb *raw;
  } pcb;
  /** the last error this netconn had */
  err_t err;
  /** sem that is used to synchroneously execute functions in the core context */
  sys_sem_t op_completed;
  /** mbox where received packets are stored until they are fetched
      by the netconn application thread (can grow quite big) */
  sys_mbox_t recvmbox;
  /** mbox where new connections are stored until processed
      by the application thread */
  sys_mbox_t acceptmbox;
  /** only used for socket layer */
  int socket;

  /** timeout to wait for new data to be received
      (or connections to arrive for listening netconns) */
  int recv_timeout;





  s16_t recv_avail;

  /** TCP: when data passed to netconn_write doesn't fit into the send buffer,
      this temporarily stores the message. */
  struct api_msg_msg *write_msg;
  /** TCP: when data passed to netconn_write doesn't fit into the send buffer,
      this temporarily stores how much is already sent. */
  size_t write_offset;
#line 157 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/api.h"
  /** A callback function that is informed about events for this netconn */
  netconn_callback callback;
};

/* Register an Network connection event */




/* Network connection functions: */


struct
netconn *netconn_new_with_proto_and_callback(enum netconn_type t, u8_t proto,
                                   netconn_callback callback);
err_t             netconn_delete  (struct netconn *conn);
/** Get the type of a netconn (as enum netconn_type). */


err_t             netconn_getaddr (struct netconn *conn,
                                   struct ip_addr *addr,
                                   u16_t *port,
                                   u8_t local);



err_t             netconn_bind    (struct netconn *conn,
                                   struct ip_addr *addr,
                                   u16_t port);
err_t             netconn_connect (struct netconn *conn,
                                   struct ip_addr *addr,
                                   u16_t port);
err_t             netconn_disconnect (struct netconn *conn);
err_t             netconn_listen_with_backlog(struct netconn *conn, u8_t backlog);

struct netconn *  netconn_accept  (struct netconn *conn);
struct netbuf *   netconn_recv    (struct netconn *conn);
err_t             netconn_sendto  (struct netconn *conn,
                                   struct netbuf *buf, struct ip_addr *addr, u16_t port);
err_t             netconn_send    (struct netconn *conn,
                                   struct netbuf *buf);
err_t             netconn_write   (struct netconn *conn,
                                   const void *dataptr, size_t size,
                                   u8_t apiflags);
err_t             netconn_close   (struct netconn *conn);


err_t             netconn_join_leave_group (struct netconn *conn,
                                            struct ip_addr *multiaddr,
                                            struct ip_addr *interface,
                                            enum netconn_igmp join_or_leave);


err_t             netconn_gethostbyname(const char *name, struct ip_addr *addr);











#line 47 "..\\src\\api\\api_lib.c"
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcpip.h"
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 **/







#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/api_msg.h"
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 **/







#line 1 "C:\\iar_ew\\arm\\inc\\c\\stddef.h"
/* stddef.h standard header */
/* Copyright 2009-2010 IAR Systems AB. */
#line 39 "C:\\iar_ew\\arm\\inc\\c\\stddef.h"

#line 49 "C:\\iar_ew\\arm\\inc\\c\\stddef.h"


/*
 * Copyright (c) 1992-2009 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V5.04:0576 */
#line 40 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/api_msg.h"

#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/igmp.h"
/**
 * Copyright (c) 2002 CITEL Technologies Ltd.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 * 3. Neither the name of CITEL Technologies Ltd nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY CITEL TECHNOLOGIES AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED.  IN NO EVENT SHALL CITEL TECHNOLOGIES OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE. 
 *
 * This file is a contribution to the lwIP TCP/IP stack.
 * The Swedish Institute of Computer Science and Adam Dunkels
 * are specifically granted permission to redistribute this
 * source code.
**/




#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/netif.h"
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */














struct dhcp;


struct autoip;






/* Throughout this file, IP addresses are expected to be in
 * the same byte order as in IP_PCB. */

/** must be the maximum of all used hardware address lengths
    across all types of interfaces in use */


/** TODO: define the use (where, when, whom) of netif flags */

/** whether the network interface is 'up'. this is
 * a software flag used to control whether this network
 * interface is enabled and processes traffic.
 */

/** if set, the netif has broadcast capability */

/** if set, the netif is one end of a point-to-point connection */

/** if set, the interface is configured using DHCP */

/** if set, the interface has an active link
 *  (set by the network interface driver) */

/** if set, the netif is an device using ARP */

/** if set, the netif has IGMP capability */


/** Generic data structure used for all lwIP network interfaces.
 *  The following fields should be filled in by the initialization
 *  function for the device driver: hwaddr_len, hwaddr[], mtu, flags */

struct netif {
  /** pointer to next in linked list */
  struct netif *next;

  /** IP address configuration in network byte order */
  struct ip_addr ip_addr;
  struct ip_addr netmask;
  struct ip_addr gw;

  /** This function is called by the network device driver
   *  to pass a packet up the TCP/IP stack. */
  err_t (* input)(struct pbuf *p, struct netif *inp);
  /** This function is called by the IP module when it wants
   *  to send a packet on the interface. This function typically
   *  first resolves the hardware address, then sends the packet. */
  err_t (* output)(struct netif *netif, struct pbuf *p,
       struct ip_addr *ipaddr);
  /** This function is called by the ARP module when it wants
   *  to send a packet on the interface. This function outputs
   *  the pbuf as-is on the link medium. */
  err_t (* linkoutput)(struct netif *netif, struct pbuf *p);
#line 119 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/netif.h"
  /** This field can be set by the device driver and could point
   *  to state information for the device. */
  void *state;

  /** the DHCP client state information for this netif */
  struct dhcp *dhcp;


  /** the AutoIP client state information for this netif */
  struct autoip *autoip;





  /** maximum transfer unit (in bytes) */
  u16_t mtu;
  /** number of bytes used in hwaddr */
  u8_t hwaddr_len;
  /** link level hardware address of this interface */
  u8_t hwaddr[6U];
  /** flags (see NETIF_FLAG_ above) */
  u8_t flags;
  /** descriptive abbreviation */
  char name[2];
  /** number of this interface */
  u8_t num;
#line 164 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/netif.h"
  /* This function could be called to add or delete a entry in the multicast filter table of the ethernet MAC.*/
  err_t (*igmp_mac_filter)( struct netif *netif, struct ip_addr *group, u8_t action);
#line 178 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/netif.h"
};

#line 198 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/netif.h"


/** The list of network interfaces. */
extern struct netif *netif_list;
/** The default network interface. */
extern struct netif *netif_default;



struct netif *netif_add(struct netif *netif, struct ip_addr *ipaddr, struct ip_addr *netmask,
      struct ip_addr *gw,
      void *state,
      err_t (* init)(struct netif *netif),
      err_t (* input)(struct pbuf *p, struct netif *netif));

void
netif_set_addr(struct netif *netif,struct ip_addr *ipaddr, struct ip_addr *netmask,
    struct ip_addr *gw);
void netif_remove(struct netif * netif);

/* Returns a network interface given its name. The name is of the form
   "et0", where the first two letters are the "name" field in the
   netif structure, and the digit is in the num field in the same
   structure. */
struct netif *netif_find(char *name);

void netif_set_default(struct netif *netif);

void netif_set_ipaddr(struct netif *netif, struct ip_addr *ipaddr);
void netif_set_netmask(struct netif *netif, struct ip_addr *netmask);
void netif_set_gw(struct netif *netif, struct ip_addr *gw);

void netif_set_up(struct netif *netif);
void netif_set_down(struct netif *netif);
u8_t netif_is_up(struct netif *netif);

#line 240 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/netif.h"

#line 250 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/netif.h"





#line 262 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/netif.h"

#line 41 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/igmp.h"








/* 
 * IGMP constants
 */






/*
 * IGMP message types, including version number.
 */





/* IGMP timer */




/* MAC Filter Actions */



/* Group  membership states */




/*
 * IGMP packet format.
 */
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/bpstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack(1)


#line 85 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/igmp.h"


struct igmp_msg {
 u8_t igmp_msgtype;
 u8_t igmp_maxresp;
 u16_t igmp_checksum;
 struct ip_addr igmp_group_address;
} ;

#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/epstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack()


#line 96 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/igmp.h"


/* 
 * now a group structure - there is
 * a list of groups for each interface
 * these should really be linked from the interface, but
 * if we keep them separate we will not affect the lwip original code
 * too much
 * 
 * There will be a group for the all systems group address but this 
 * will not run the state machine as it is used to kick off reports
 * from all the other groups
 */

struct igmp_group {
  struct igmp_group *next;
  struct netif      *interface;
  struct ip_addr     group_address;
  u8_t               last_reporter_flag; /* signifies we were the last person to report */
  u8_t               group_state;
  u16_t              timer;
  u8_t               use; /* counter of simultaneous uses */
};


/*  Prototypes */
void   igmp_init(void);

err_t  igmp_start( struct netif *netif);

err_t  igmp_stop( struct netif *netif);

void   igmp_report_groups( struct netif *netif);

struct igmp_group *igmp_lookfor_group( struct netif *ifp, struct ip_addr *addr);

struct igmp_group *igmp_lookup_group( struct netif *ifp, struct ip_addr *addr);

err_t  igmp_remove_group( struct igmp_group *group);

void   igmp_input( struct pbuf *p, struct netif *inp, struct ip_addr *dest);

err_t  igmp_joingroup( struct ip_addr *ifaddr, struct ip_addr *groupaddr);

err_t  igmp_leavegroup( struct ip_addr *ifaddr, struct ip_addr *groupaddr);

void   igmp_tmr(void);

void   igmp_timeout( struct igmp_group *group);

void   igmp_start_timer( struct igmp_group *group, u8_t max_time);

void   igmp_stop_timer( struct igmp_group *group);

void   igmp_delaying_member( struct igmp_group *group, u8_t maxresp);

err_t  igmp_ip_output_if( struct pbuf *p, struct ip_addr *src, struct ip_addr *dest, u8_t ttl, u8_t proto, struct netif *netif);

void   igmp_send( struct igmp_group *group, u8_t type);







#line 45 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/api_msg.h"






/* IP addresses and port numbers are expected to be in
 * the same byte order as in the corresponding pcb.
 */
/** This struct includes everything that is necessary to execute a function
    for a netconn in another thread context (mainly used to process netconns
    in the tcpip_thread context to be thread safe). */
struct api_msg_msg {
  /** The netconn which to process - always needed: it includes the semaphore
      which is used to block the application thread until the function finished. */
  struct netconn *conn;
  /** Depending on the executed function, one of these union members is used */
  union {
    /** used for do_send */
    struct netbuf *b;
    /** used for do_newconn */
    struct {
      u8_t proto;
    } n;
    /** used for do_bind and do_connect */
    struct {
      struct ip_addr *ipaddr;
      u16_t port;
    } bc;
    /** used for do_getaddr */
    struct {
      struct ip_addr *ipaddr;
      u16_t *port;
      u8_t local;
    } ad;
    /** used for do_write */
    struct {
      const void *dataptr;
      size_t len;
      u8_t apiflags;
    } w;
    /** used for do_recv */
    struct {
      u16_t len;
    } r;

    /** used for do_join_leave_group */
    struct {
      struct ip_addr *multiaddr;
      struct ip_addr *interface;
      enum netconn_igmp join_or_leave;
    } jl;
#line 103 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/api_msg.h"
  } msg;
};

/** This struct contains a function to execute in another thread context and
    a struct api_msg_msg that serves as an argument for this function.
    This is passed to tcpip_apimsg to execute functions in tcpip_thread context. */
struct api_msg {
  /** function to execute in tcpip_thread context */
  void (* function)(struct api_msg_msg *msg);
  /** arguments for this function */
  struct api_msg_msg msg;
};


/** As do_gethostbyname requires more arguments but doesn't require a netconn,
    it has its own struct (to avoid struct api_msg getting bigger than necessary).
    do_gethostbyname must be called using tcpip_callback instead of tcpip_apimsg
    (see netconn_gethostbyname). */
struct dns_api_msg {
  /** Hostname to query or dotted IP address string */
  const char *name;
  /** Rhe resolved address is stored here */
  struct ip_addr *addr;
  /** This semaphore is posted when the name is resolved, the application thread
      should wait on it. */
  sys_sem_t sem;
  /** Errors are given back here */
  err_t *err;
};


void do_newconn         ( struct api_msg_msg *msg);
void do_delconn         ( struct api_msg_msg *msg);
void do_bind            ( struct api_msg_msg *msg);
void do_connect         ( struct api_msg_msg *msg);
void do_disconnect      ( struct api_msg_msg *msg);
void do_listen          ( struct api_msg_msg *msg);
void do_send            ( struct api_msg_msg *msg);
void do_recv            ( struct api_msg_msg *msg);
void do_write           ( struct api_msg_msg *msg);
void do_getaddr         ( struct api_msg_msg *msg);
void do_close           ( struct api_msg_msg *msg);

void do_join_leave_group( struct api_msg_msg *msg);



void do_gethostbyname(void *arg);


struct netconn* netconn_alloc(enum netconn_type t, netconn_callback callback);
void netconn_free(struct netconn *conn);







#line 40 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcpip.h"
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/netifapi.h"
/*
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 */
 







#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/dhcp.h"
/** @file
 **/








#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/udp.h"
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */







#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/ip.h"
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */





#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/def.h"
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */



/* this might define NULL already */












#line 38 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/ip.h"









/** Currently, the function ip_output_if_opt() is only used with IGMP */









/* This is passed as the destination address to ip_output_if (not
   to ip_output), meaning that an IP header already is constructed
   in the pbuf. This is used when TCP retransmits. */











/* This is the common part of all PCB types. It needs to be at the
   beginning of a PCB type definition. It is located here so that
   changes to this common part are made in one location instead of
   having to change all PCB structs. */
#line 87 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/ip.h"

struct ip_pcb {
/* Common members of all PCB types */
  struct ip_addr local_ip; struct ip_addr remote_ip; u16_t so_options; u8_t tos; u8_t ttl ;
};

/*
 * Option flags per-socket. These are the same like SO_XXX.
 */
#line 106 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/ip.h"


#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/bpstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack(1)


#line 110 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/ip.h"


struct ip_hdr {
  /* version / header length / type of service */
  u16_t _v_hl_tos;
  /* total length */
  u16_t _len;
  /* identification */
  u16_t _id;
  /* fragment offset field */
  u16_t _offset;




  /* time to live / protocol*/
  u16_t _ttl_proto;
  /* checksum */
  u16_t _chksum;
  /* source and destination IP addresses */
  struct ip_addr src;
  struct ip_addr dest; 
} ;

#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/epstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack()


#line 136 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/ip.h"


#line 147 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/ip.h"

#line 155 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/ip.h"

/** The interface that provided the packet for the current callback invocation. */
extern struct netif *current_netif;
/** Header of the input packet currently being processed. */
extern const struct ip_hdr *current_header;


struct netif *ip_route(struct ip_addr *dest);
err_t ip_input(struct pbuf *p, struct netif *inp);
err_t ip_output(struct pbuf *p, struct ip_addr *src, struct ip_addr *dest,
       u8_t ttl, u8_t tos, u8_t proto);
err_t ip_output_if(struct pbuf *p, struct ip_addr *src, struct ip_addr *dest,
       u8_t ttl, u8_t tos, u8_t proto,
       struct netif *netif);





err_t ip_output_if_opt(struct pbuf *p, struct ip_addr *src, struct ip_addr *dest,
       u8_t ttl, u8_t tos, u8_t proto, struct netif *netif, void *ip_options,
       u16_t optlen);

/** Get the interface that received the current packet.
 * This function must only be called from a receive callback (udp_recv,
 * raw_recv, tcp_accept). It will return NULL otherwise. */

/** Get the IP header of the current packet.
 * This function must only be called from a receive callback (udp_recv,
 * raw_recv, tcp_accept). It will return NULL otherwise. */
#line 191 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/ip.h"








#line 43 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/udp.h"







/* Fields are (of course) in network byte order. */
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/bpstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack(1)


#line 53 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/udp.h"


struct udp_hdr {
  u16_t src;
  u16_t dest;  /* src/dest UDP ports */
  u16_t len;
  u16_t chksum;
} ;

#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/epstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack()


#line 64 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/udp.h"






struct udp_pcb {
/* Common members of all PCB types */
  struct ip_addr local_ip; struct ip_addr remote_ip; u16_t so_options; u8_t tos; u8_t ttl ;

/* Protocol specific PCB members */

  struct udp_pcb *next;

  u8_t flags;
  /* ports are in host byte order */
  u16_t local_port, remote_port;


  /* outgoing network interface for multicast packets */
  struct ip_addr multicast_ip;







  /* receive callback function
   * addr and port are in same byte order as in the pcb
   * The callback is responsible for freeing the pbuf
   * if it's not used any more.
   *
   * ATTENTION: Be aware that 'addr' points into the pbuf 'p' so freeing this pbuf
   *            makes 'addr' invalid, too.
   *
   * @param arg user supplied argument (udp_pcb.recv_arg)
   * @param pcb the udp_pcb which received data
   * @param p the packet buffer that was received
   * @param addr the remote IP address from which the packet was received
   * @param port the remote port from which the packet was received
   */
  void (* recv)(void *arg, struct udp_pcb *pcb, struct pbuf *p,
    struct ip_addr *addr, u16_t port);
  /* user-supplied argument for the recv callback */
  void *recv_arg;  
};
/* udp_pcbs export for exernal reference (e.g. SNMP agent) */
extern struct udp_pcb *udp_pcbs;

/* The following functions is the application layer interface to the
   UDP code. */
struct udp_pcb * udp_new        (void);
void             udp_remove     (struct udp_pcb *pcb);
err_t            udp_bind       (struct udp_pcb *pcb, struct ip_addr *ipaddr,
                 u16_t port);
err_t            udp_connect    (struct udp_pcb *pcb, struct ip_addr *ipaddr,
                 u16_t port);
void             udp_disconnect    (struct udp_pcb *pcb);
void             udp_recv       (struct udp_pcb *pcb,
         void (* recv)(void *arg, struct udp_pcb *upcb,
                 struct pbuf *p,
                 struct ip_addr *addr,
                 u16_t port),
         void *recv_arg);
err_t            udp_sendto_if  (struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *dst_ip, u16_t dst_port, struct netif *netif);
err_t            udp_sendto     (struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *dst_ip, u16_t dst_port);
err_t            udp_send       (struct udp_pcb *pcb, struct pbuf *p);




/* The following functions are the lower layer interface to UDP. */
void             udp_input      (struct pbuf *p, struct netif *inp);















#line 13 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/dhcp.h"





/** period (in seconds) of the application calling dhcp_coarse_tmr() */

/** period (in milliseconds) of the application calling dhcp_coarse_tmr() */

/** period (in milliseconds) of the application calling dhcp_fine_tmr() */


struct dhcp
{
  /** transaction identifier of last sent request */ 
  u32_t xid;
  /** our connection to the DHCP server */ 
  struct udp_pcb *pcb;
  /** incoming msg */
  struct dhcp_msg *msg_in;
  /** incoming msg options */
  void *options_in; 
  /** ingoing msg options length */
  u16_t options_in_len;
  /** current DHCP state machine state */
  u8_t state;
  /** retries of current request */
  u8_t tries;

  struct pbuf *p_out; /* pbuf of outcoming msg */
  struct dhcp_msg *msg_out; /* outgoing msg */
  u16_t options_out_len; /* outgoing msg options length */
  u16_t request_timeout; /* #ticks with period DHCP_FINE_TIMER_SECS for request timeout */
  u16_t t1_timeout;  /* #ticks with period DHCP_COARSE_TIMER_SECS for renewal time */
  u16_t t2_timeout;  /* #ticks with period DHCP_COARSE_TIMER_SECS for rebind time */
  struct ip_addr server_ip_addr; /* dhcp server address that offered this lease */
  struct ip_addr offered_ip_addr;
  struct ip_addr offered_sn_mask;
  struct ip_addr offered_gw_addr;
  struct ip_addr offered_bc_addr;

  u32_t dns_count; /* actual number of DNS servers obtained */
  struct ip_addr offered_dns_addr[2]; /* DNS server addresses */
 
  u32_t offered_t0_lease; /* lease period (in seconds) */
  u32_t offered_t1_renew; /* recommended renew time (usually 50% of lease period) */
  u32_t offered_t2_rebind; /* recommended rebind time (usually 66% of lease period)  */



/** Patch #1308
 *  TODO: See dhcp.c "LwIP_TODO"s
 */




};

/* MUST be compiled with "pack structs" or equivalent! */
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/bpstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack(1)


#line 75 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/dhcp.h"


/** minimum set of fields of any DHCP message */
struct dhcp_msg
{
  u8_t op;
  u8_t htype;
  u8_t hlen;
  u8_t hops;
  u32_t xid;
  u16_t secs;
  u16_t flags;
  struct ip_addr ciaddr;
  struct ip_addr yiaddr;
  struct ip_addr siaddr;
  struct ip_addr giaddr;

  u8_t chaddr[16U];

  u8_t sname[64U];

  u8_t file[128U];
  u32_t cookie;

/** make sure user does not configure this too small */



/** allow this to be configured in lwipopts.h, but not too small */

/** set this to be sufficient for your options in outgoing DHCP msgs */


  u8_t options[68U];
} ;

#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/epstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack()


#line 113 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/dhcp.h"


/** start DHCP configuration */
err_t dhcp_start(struct netif *netif);
/** enforce early lease renewal (not needed normally)*/
err_t dhcp_renew(struct netif *netif);
/** release the DHCP lease, usually called before dhcp_stop()*/
err_t dhcp_release(struct netif *netif);
/** stop DHCP configuration */
void dhcp_stop(struct netif *netif);
/** inform server of our manual IP address */
void dhcp_inform(struct netif *netif);
/** Handle a possible change in the network configuration */
void dhcp_network_changed(struct netif *netif);

/** if enabled, check whether the offered IP address is not in use, using ARP */

void dhcp_arp_reply(struct netif *netif, struct ip_addr *addr);


/** to be called every minute */
void dhcp_coarse_tmr(void);
/** to be called every half second */
void dhcp_fine_tmr(void);
 
/** DHCP message item offsets and length */
#line 155 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/dhcp.h"







/** DHCP client states */
#line 173 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/dhcp.h"
/** not yet implemented #define DHCP_RELEASING 11 */



/** AUTOIP cooperatation flags */


 



#line 192 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/dhcp.h"








/** BootP options */
#line 211 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/dhcp.h"

/** DHCP options */














#line 233 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/dhcp.h"

/** possible combinations of overloading the file and sname fields with options */











#line 38 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/netifapi.h"
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/autoip.h"
/**
 * @file
 *
 * AutoIP Automatic LinkLocal IP Configuration
 **/

/*
 *
 * Copyright (c) 2007 Dominik Spies <kontakt@dspies.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * Author: Dominik Spies <kontakt@dspies.de>
 *
 * This is a AutoIP implementation for the lwIP TCP/IP stack. It aims to conform
 * with RFC 3927.
 *
 *
 * Please coordinate changes and requests with Dominik Spies
 * <kontakt@dspies.de>
 */
 







#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\netif/etharp.h"
/**
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * Copyright (c) 2003-2004 Leon Woestenberg <leon.woestenberg@axon.tv>
 * Copyright (c) 2003-2004 Axon Digital Design B.V., The Netherlands.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 **/

























#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/bpstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack(1)


#line 61 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\netif/etharp.h"


struct eth_addr {
  u8_t addr[6];
} ;

#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/epstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack()


#line 69 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\netif/etharp.h"


#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/bpstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack(1)


#line 73 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\netif/etharp.h"


struct eth_hdr {



  struct eth_addr dest;
  struct eth_addr src;
  u16_t type;
} ;

#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/epstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack()


#line 86 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\netif/etharp.h"




#line 109 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\netif/etharp.h"

#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/bpstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack(1)


#line 112 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\netif/etharp.h"


/** the ARP message */
struct etharp_hdr {
  u16_t hwtype;
  u16_t proto;
  u16_t _hwlen_protolen;
  u16_t opcode;
  struct eth_addr shwaddr;
  struct ip_addr2 sipaddr;
  struct eth_addr dhwaddr;
  struct ip_addr2 dipaddr;
} ;

#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/epstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack()


#line 128 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\netif/etharp.h"





/** 5 seconds period */








/** ARP message types (opcodes) */




/** struct for queueing outgoing packets for unknown address
  * defined here to be accessed by memp.h
  */
struct etharp_q_entry {
  struct etharp_q_entry *next;
  struct pbuf *p;
};



void etharp_tmr(void);
s8_t etharp_find_addr(struct netif *netif, struct ip_addr *ipaddr,
         struct eth_addr **eth_ret, struct ip_addr **ip_ret);
void etharp_ip_input(struct netif *netif, struct pbuf *p);
void etharp_arp_input(struct netif *netif, struct eth_addr *ethaddr,
         struct pbuf *p);
err_t etharp_output(struct netif *netif, struct pbuf *q, struct ip_addr *ipaddr);
err_t etharp_query(struct netif *netif, struct ip_addr *ipaddr, struct pbuf *q);
err_t etharp_request(struct netif *netif, struct ip_addr *ipaddr);
/** For Ethernet network interfaces, we might want to send "gratuitous ARP";
 *  this is an ARP packet sent by a node in order to spontaneously cause other
 *  nodes to update an entry in their ARP cache.
 *  From RFC 3220 "IP Mobility Support for IPv4" section 4.6. */


err_t ethernet_input(struct pbuf *p, struct netif *netif);


err_t etharp_raw(struct netif *netif, const struct eth_addr *ethsrc_addr,
                 const struct eth_addr *ethdst_addr,
                 const struct eth_addr *hwsrc_addr, const struct ip_addr *ipsrc_addr,
                 const struct eth_addr *hwdst_addr, const struct ip_addr *ipdst_addr,
                 const u16_t opcode);




extern const struct eth_addr ethbroadcast, ethzero;







#line 54 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/autoip.h"





/* AutoIP Timing */



/* RFC 3927 Constants */
#line 74 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/autoip.h"

/* AutoIP client states */





struct autoip
{
  struct ip_addr llipaddr;  /* the currently selected, probed, announced or used LL IP-Address */
  u8_t state;               /* current AutoIP state machine state */
  u8_t sent_num;            /* sent number of probes or announces, dependent on state */
  u16_t ttw;                /* ticks to wait, tick is AUTOIP_TMR_INTERVAL long */
  u8_t lastconflict;        /* ticks until a conflict can be solved by defending */
  u8_t tried_llipaddr;      /* total number of probed/used Link Local IP-Addresses */
};


/** Init srand, has to be called before entering mainloop */
void autoip_init(void);

/** Start AutoIP client */
err_t autoip_start(struct netif *netif);

/** Stop AutoIP client */
err_t autoip_stop(struct netif *netif);

/** Handles every incoming ARP Packet, called by etharp_arp_input */
void autoip_arp_reply(struct netif *netif, struct etharp_hdr *hdr);

/** Has to be called in loop every AUTOIP_TMR_INTERVAL milliseconds */
void autoip_tmr(void);

/** Handle a possible change in the network configuration */
void autoip_network_changed(struct netif *netif);







#line 39 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/netifapi.h"





struct netifapi_msg_msg {

  sys_sem_t sem;

  err_t err;
  struct netif *netif;
  union {
    struct {
      struct ip_addr *ipaddr;
      struct ip_addr *netmask;
      struct ip_addr *gw;
      void *state;
      err_t (* init) (struct netif *netif);
      err_t (* input)(struct pbuf *p, struct netif *netif);
    } add;
    struct {
      void  (* voidfunc)(struct netif *netif);
      err_t (* errtfunc)(struct netif *netif);
    } common;
  } msg;
};

struct netifapi_msg {
  void (* function)(struct netifapi_msg_msg *msg);
  struct netifapi_msg_msg msg;
};


/* API for application */
err_t netifapi_netif_add       ( struct netif *netif,
                                 struct ip_addr *ipaddr,
                                 struct ip_addr *netmask,
                                 struct ip_addr *gw,
                                 void *state,
                                 err_t (* init)(struct netif *netif),
                                 err_t (* input)(struct pbuf *p, struct netif *netif) );

err_t netifapi_netif_set_addr  ( struct netif *netif,
                                 struct ip_addr *ipaddr,
                                 struct ip_addr *netmask,
                                 struct ip_addr *gw );

err_t netifapi_netif_common    ( struct netif *netif,
                                 void  (* voidfunc)(struct netif *netif),
                                 err_t (* errtfunc)(struct netif *netif) );

#line 98 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/netifapi.h"







#line 41 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcpip.h"









#line 67 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcpip.h"

void tcpip_init(void (* tcpip_init_done)(void *), void *arg);


err_t tcpip_apimsg(struct api_msg *apimsg);





err_t tcpip_input(struct pbuf *p, struct netif *inp);


err_t tcpip_netifapi(struct netifapi_msg *netifapimsg);





err_t tcpip_callback_with_block(void (*f)(void *ctx), void *ctx, u8_t block);


/* free pbufs or heap memory from another context without blocking */
err_t pbuf_free_callback(struct pbuf *p);
err_t mem_free_callback(void *m);

err_t tcpip_timeout(u32_t msecs, sys_timeout_handler h, void *arg);
err_t tcpip_untimeout(sys_timeout_handler h, void *arg);

enum tcpip_msg_type {

  TCPIP_MSG_API,

  TCPIP_MSG_INPKT,

  TCPIP_MSG_NETIFAPI,

  TCPIP_MSG_CALLBACK,
  TCPIP_MSG_TIMEOUT,
  TCPIP_MSG_UNTIMEOUT
};

struct tcpip_msg {
  enum tcpip_msg_type type;
  sys_sem_t *sem;
  union {

    struct api_msg *apimsg;


    struct netifapi_msg *netifapimsg;

    struct {
      struct pbuf *p;
      struct netif *netif;
    } inp;
    struct {
      void (*f)(void *ctx);
      void *ctx;
    } cb;
    struct {
      u32_t msecs;
      sys_timeout_handler h;
      void *arg;
    } tmo;
  } msg;
};







#line 48 "..\\src\\api\\api_lib.c"
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/memp.h"
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */










/* Create the list of all memory pools managed by memp. MEMP_MAX represents a NULL pool at the end */
typedef enum {
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/memp_std.h"
/**
 * SETUP: Make sure we define everything we will need.
 *
 * We have create three types of pools:
 *   1) MEMPOOL - standard pools
 *   2) MALLOC_MEMPOOL - to be used by mem_malloc in mem.c
 *   3) PBUF_MEMPOOL - a mempool of pbuf's, so include space for the pbuf struct
 *
 * If the include'r doesn't require any special treatment of each of the types
 * above, then will declare #2 & #3 to be just standard mempools.
 */

/* This treats "malloc pools" just like any other pool.
   The pools are a little bigger to provide 'size' as the amount of user data. */






/* This treats "pbuf pools" just like any other pool.
 * Allocates buffers for a pbuf struct AND a payload size */




/*
 * A list of internal pools used by LWIP.
 *
 * LWIP_MEMPOOL(pool_name, number_elements, element_size, pool_description)
 *     creates a pool name MEMP_pool_name. description is used in stats.c
 */

MEMP_RAW_PCB,



MEMP_UDP_PCB,



MEMP_TCP_PCB,
MEMP_TCP_PCB_LISTEN,
MEMP_TCP_SEG,



MEMP_REASSDATA,



MEMP_NETBUF,
MEMP_NETCONN,



MEMP_TCPIP_MSG_API,
MEMP_TCPIP_MSG_INPKT,



MEMP_ARP_QUEUE,



MEMP_IGMP_GROUP,



MEMP_SYS_TIMEOUT,



/*
 * A list of pools of pbuf's used by LWIP.
 *
 * LWIP_PBUF_MEMPOOL(pool_name, number_elements, pbuf_payload_size, pool_description)
 *     creates a pool name MEMP_pool_name. description is used in stats.c
 *     This allocates enough space for the pbuf struct and a payload.
 *     (Example: pbuf_payload_size=0 allocates only size for the struct)
 */
MEMP_PBUF,
MEMP_PBUF_POOL,


/*
 * Allow for user-defined pools; this must be explicitly set in lwipopts.h
 * since the default is to NOT look for lwippools.h
 */




/*
 * REQUIRED CLEANUP: Clear up so we don't get "multiply defined" error later
 * (#undef is ignored for something that is not defined)
 */
#line 46 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/memp.h"
  MEMP_MAX
} memp_t;

#line 77 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/memp.h"





#line 91 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/memp.h"

#line 99 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/memp.h"

void  memp_init(void);





void *memp_malloc(memp_t type);

void  memp_free(memp_t type, void *mem);







#line 49 "..\\src\\api\\api_lib.c"

#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/raw.h"
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 **/
















struct raw_pcb {
/* Common members of all PCB types */
  struct ip_addr local_ip; struct ip_addr remote_ip; u16_t so_options; u8_t tos; u8_t ttl ;

  struct raw_pcb *next;

  u8_t protocol;

  /* receive callback function
   * @param arg user supplied argument (raw_pcb.recv_arg)
   * @param pcb the raw_pcb which received data
   * @param p the packet buffer that was received
   * @param addr the remote IP address from which the packet was received
   * @return 1 if the packet was 'eaten' (aka. deleted),
   *         0 if the packet lives on
   * If returning 1, the callback is responsible for freeing the pbuf
   * if it's not used any more.
   */
  u8_t (* recv)(void *arg, struct raw_pcb *pcb, struct pbuf *p,
    struct ip_addr *addr);
  /* user-supplied argument for the recv callback */
  void *recv_arg;
};

/* The following functions is the application layer interface to the
   RAW code. */
struct raw_pcb * raw_new        (u8_t proto);
void             raw_remove     (struct raw_pcb *pcb);
err_t            raw_bind       (struct raw_pcb *pcb, struct ip_addr *ipaddr);
err_t            raw_connect    (struct raw_pcb *pcb, struct ip_addr *ipaddr);

void             raw_recv       (struct raw_pcb *pcb,
                                 u8_t (* recv)(void *arg, struct raw_pcb *pcb,
                                              struct pbuf *p,
                                              struct ip_addr *addr),
                                 void *recv_arg);
err_t            raw_sendto     (struct raw_pcb *pcb, struct pbuf *p, struct ip_addr *ipaddr);
err_t            raw_send       (struct raw_pcb *pcb, struct pbuf *p);

/* The following functions are the lower layer interface to RAW. */
u8_t             raw_input      (struct pbuf *p, struct netif *inp);








#line 52 "..\\src\\api\\api_lib.c"
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */







#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/mem.h"
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */









#line 69 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/mem.h"

/* MEM_SIZE would have to be aligned, but using 64000 here instead of
 * 65535 leaves some room for alignment...
 */



typedef u16_t mem_size_t;


#line 86 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/mem.h"
/* lwIP alternative malloc */
void  mem_init(void);
void *mem_realloc(void *mem, mem_size_t size);

void *mem_malloc(mem_size_t size);
void *mem_calloc(mem_size_t count, mem_size_t size);
void  mem_free(void *mem);














#line 41 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/icmp.h"
/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 **/












#line 55 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/icmp.h"

enum icmp_dur_type {
  ICMP_DUR_NET = 0,    /* net unreachable */
  ICMP_DUR_HOST = 1,   /* host unreachable */
  ICMP_DUR_PROTO = 2,  /* protocol unreachable */
  ICMP_DUR_PORT = 3,   /* port unreachable */
  ICMP_DUR_FRAG = 4,   /* fragmentation needed and DF set */
  ICMP_DUR_SR = 5      /* source route failed */
};

enum icmp_te_type {
  ICMP_TE_TTL = 0,     /* time to live exceeded in transit */
  ICMP_TE_FRAG = 1     /* fragment reassembly time exceeded */
};

#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/bpstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack(1)


#line 72 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/icmp.h"

/** This is the standard ICMP header only that the u32_t data
 *  is splitted to two u16_t like ICMP echo needs it.
 *  This header is also used for other ICMP types that do not
 *  use the data part.
 */

struct icmp_echo_hdr {
  u8_t type;
  u8_t code;
  u16_t chksum;
  u16_t id;
  u16_t seqno;
} ;

#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/epstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack()


#line 89 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\ipv4\\lwip/icmp.h"





/** Combines type and code to an u16_t */






void icmp_input(struct pbuf *p, struct netif *inp);
void icmp_dest_unreach(struct pbuf *p, enum icmp_dur_type t);
void icmp_time_exceeded(struct pbuf *p, enum icmp_te_type t);







#line 44 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"






struct tcp_pcb;

/* Functions for interfacing with TCP: */

/* Lower layer interface to TCP: */

void             tcp_tmr     (void);  /* Must be called every
                                         TCP_TMR_INTERVAL
                                         ms. (Typically 250 ms). */
/* Application program's interface: */
struct tcp_pcb * tcp_new     (void);
struct tcp_pcb * tcp_alloc   (u8_t prio);

void             tcp_arg     (struct tcp_pcb *pcb, void *arg);
void             tcp_accept  (struct tcp_pcb *pcb,
                              err_t (* accept)(void *arg, struct tcp_pcb *newpcb,
                 err_t err));
void             tcp_recv    (struct tcp_pcb *pcb,
                              err_t (* recv)(void *arg, struct tcp_pcb *tpcb,
                              struct pbuf *p, err_t err));
void             tcp_sent    (struct tcp_pcb *pcb,
                              err_t (* sent)(void *arg, struct tcp_pcb *tpcb,
                              u16_t len));
void             tcp_poll    (struct tcp_pcb *pcb,
                              err_t (* poll)(void *arg, struct tcp_pcb *tpcb),
                              u8_t interval);
void             tcp_err     (struct tcp_pcb *pcb,
                              void (* err)(void *arg, err_t err));













void             tcp_recved  (struct tcp_pcb *pcb, u16_t len);
err_t            tcp_bind    (struct tcp_pcb *pcb, struct ip_addr *ipaddr,
                              u16_t port);
err_t            tcp_connect (struct tcp_pcb *pcb, struct ip_addr *ipaddr,
                              u16_t port, err_t (* connected)(void *arg,
                              struct tcp_pcb *tpcb,
                              err_t err));

struct tcp_pcb * tcp_listen_with_backlog(struct tcp_pcb *pcb, u8_t backlog);


void             tcp_abandon (struct tcp_pcb *pcb, int reset);

err_t            tcp_close   (struct tcp_pcb *pcb);

/* Flags for "apiflags" parameter in tcp_write and tcp_enqueue */



err_t            tcp_write   (struct tcp_pcb *pcb, const void *dataptr, u16_t len,
                              u8_t apiflags);

void             tcp_setprio (struct tcp_pcb *pcb, u8_t prio);





/* It is also possible to call these two functions at the right
   intervals (instead of calling tcp_tmr()). */
void             tcp_slowtmr (void);
void             tcp_fasttmr (void);


/* Only used by IP to pass a TCP segment to TCP: */
void             tcp_input   (struct pbuf *p, struct netif *inp);
/* Used within the TCP code only: */
err_t            tcp_send_empty_ack(struct tcp_pcb *pcb);
err_t            tcp_output  (struct tcp_pcb *pcb);
void             tcp_rexmit  (struct tcp_pcb *pcb);
void             tcp_rexmit_rto  (struct tcp_pcb *pcb);
void             tcp_rexmit_fast (struct tcp_pcb *pcb);
u32_t            tcp_update_rcv_ann_wnd(struct tcp_pcb *pcb);

/**
 * This is the Nagle algorithm: try to combine user data to send as few TCP
 * segments as possible. Only send if
 * - no previously transmitted data on the connection remains unacknowledged or
 * - the TF_NODELAY flag is set (nagle algorithm turned off for this pcb) or
 * - the only unsent segment is at least pcb->mss bytes long (or there is more
 *   than one unsent segment - with lwIP, this can happen although unsent->len < mss)
 * - or if we are in fast-retransmit (TF_INFR)
 */
#line 150 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"






/* is b<=a<=c? */
#line 169 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"



/* Length of the TCP header, excluding options. */























/* Keepalive values, compliant with RFC 1122. Don't change this unless you know what you're doing */














/* Fields are (of course) in network byte order.
 * Some fields are converted to host byte order in tcp_input().
 */
#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/bpstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack(1)


#line 216 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"


struct tcp_hdr {
  u16_t src;
  u16_t dest;
  u32_t seqno;
  u32_t ackno;
  u16_t _hdrlen_rsvd_flags;
  u16_t wnd;
  u16_t chksum;
  u16_t urgp;
} ;

#line 1 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\port\\STM32F4x7\\arch/epstruct.h"
/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#pragma pack()


#line 231 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"














enum tcp_state {
  CLOSED      = 0,
  LISTEN      = 1,
  SYN_SENT    = 2,
  SYN_RCVD    = 3,
  ESTABLISHED = 4,
  FIN_WAIT_1  = 5,
  FIN_WAIT_2  = 6,
  CLOSE_WAIT  = 7,
  CLOSING     = 8,
  LAST_ACK    = 9,
  TIME_WAIT   = 10
};

/** Flags used on input processing, not on pcb->flags
*/






  /* Function to call when a listener has been connected.
   * @param arg user-supplied argument (tcp_pcb.callback_arg)
   * @param pcb a new tcp_pcb that now is connected
   * @param err an error argument (TODO: that is current always ERR_OK?)
   * @return ERR_OK: accept the new connection,
   *                 any other err_t abortsthe new connection
   */





/**
 * members common to struct tcp_pcb and struct tcp_listen_pcb
 */
#line 291 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"


/* the TCP protocol control block */
struct tcp_pcb {
/** common PCB members */
  struct ip_addr local_ip; struct ip_addr remote_ip; u16_t so_options; u8_t tos; u8_t ttl ;
/** protocol specific PCB members */
  struct tcp_pcb *next; enum tcp_state state; u8_t prio; void *callback_arg; u16_t local_port; err_t (* accept)(void *arg, struct tcp_pcb *newpcb, err_t err);

  /* ports are in host byte order */
  u16_t remote_port;
  
  u8_t flags;
#line 311 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"

  /* the rest of the fields are in host byte order
     as we have to do some math with them */
  /* receiver variables */
  u32_t rcv_nxt;   /* next seqno expected */
  u16_t rcv_wnd;   /* receiver window available */
  u16_t rcv_ann_wnd; /* receiver window to announce */
  u32_t rcv_ann_right_edge; /* announced right edge of window */

  /* Timers */
  u32_t tmr;
  u8_t polltmr, pollinterval;
  
  /* Retransmission timer. */
  s16_t rtime;
  
  u16_t mss;   /* maximum segment size */
  
  /* RTT (round trip time) estimation variables */
  u32_t rttest; /* RTT estimate in 500ms ticks */
  u32_t rtseq;  /* sequence number being timed */
  s16_t sa, sv; /* @todo document this */

  s16_t rto;    /* retransmission time-out */
  u8_t nrtx;    /* number of retransmissions */

  /* fast retransmit/recovery */
  u32_t lastack; /* Highest acknowledged seqno. */
  u8_t dupacks;
  
  /* congestion avoidance/control variables */
  u16_t cwnd;  
  u16_t ssthresh;

  /* sender variables */
  u32_t snd_nxt;   /* next new seqno to be sent */
  u16_t snd_wnd;   /* sender window */
  u32_t snd_wl1, snd_wl2; /* Sequence and acknowledgement numbers of last
                             window update. */
  u32_t snd_lbb;       /* Sequence number of next byte to be buffered. */

  u16_t acked;
  
  u16_t snd_buf;   /* Available buffer space for sending (in bytes). */

  u16_t snd_queuelen; /* Available buffer space for sending (in tcp_segs). */
  
  
  /* These are ordered by sequence number: */
  struct tcp_seg *unsent;   /* Unsent (queued) segments. */
  struct tcp_seg *unacked;  /* Sent but unacknowledged segments. */




  struct pbuf *refused_data; /* Data previously received but not yet taken by upper layer */


  /* Function to be called when more send buffer space is available.
   * @param arg user-supplied argument (tcp_pcb.callback_arg)
   * @param pcb the tcp_pcb which has send buffer space available
   * @param space the amount of bytes available
   * @return ERR_OK: try to send some data by calling tcp_output
   */
  err_t (* sent)(void *arg, struct tcp_pcb *pcb, u16_t space);
  
  /* Function to be called when (in-sequence) data has arrived.
   * @param arg user-supplied argument (tcp_pcb.callback_arg)
   * @param pcb the tcp_pcb for which data has arrived
   * @param p the packet buffer which arrived
   * @param err an error argument (TODO: that is current always ERR_OK?)
   * @return ERR_OK: try to send some data by calling tcp_output
   */
  err_t (* recv)(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);

  /* Function to be called when a connection has been set up.
   * @param arg user-supplied argument (tcp_pcb.callback_arg)
   * @param pcb the tcp_pcb that now is connected
   * @param err an error argument (TODO: that is current always ERR_OK?)
   * @return value is currently ignored
   */
  err_t (* connected)(void *arg, struct tcp_pcb *pcb, err_t err);

  /* Function which is called periodically.
   * The period can be adjusted in multiples of the TCP slow timer interval
   * by changing tcp_pcb.polltmr.
   * @param arg user-supplied argument (tcp_pcb.callback_arg)
   * @param pcb the tcp_pcb to poll for
   * @return ERR_OK: try to send some data by calling tcp_output
   */
  err_t (* poll)(void *arg, struct tcp_pcb *pcb);

  /* Function to be called whenever a fatal error occurs.
   * There is no pcb parameter since most of the times, the pcb is
   * already deallocated (or there is no pcb) when this function is called.
   * @param arg user-supplied argument (tcp_pcb.callback_arg)
   * @param err an indication why the error callback is called:
   *            ERR_ABRT: aborted through tcp_abort or by a TCP timer
   *            ERR_RST: the connection was reset by the remote host
   */
  void (* errf)(void *arg, err_t err);







  /* idle time before KEEPALIVE is sent */
  u32_t keep_idle;

  u32_t keep_intvl;
  u32_t keep_cnt;

  
  /* Persist timer counter */
  u32_t persist_cnt;
  /* Persist timer back-off */
  u8_t persist_backoff;

  /* KEEPALIVE counter */
  u8_t keep_cnt_sent;
};

struct tcp_pcb_listen {  
/* Common members of all PCB types */
  struct ip_addr local_ip; struct ip_addr remote_ip; u16_t so_options; u8_t tos; u8_t ttl ;
/* Protocol specific PCB members */
  struct tcp_pcb_listen *next; enum tcp_state state; u8_t prio; void *callback_arg; u16_t local_port; err_t (* accept)(void *arg, struct tcp_pcb *newpcb, err_t err);





};

#line 477 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"

#line 484 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"

#line 491 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"

#line 500 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"

#line 507 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"

#line 514 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"









/* This structure represents a TCP segment on the unsent and unacked queues */
struct tcp_seg {
  struct tcp_seg *next;    /* used when putting segements on a queue */
  struct pbuf *p;          /* buffer containing data + TCP header */
  void *dataptr;           /* pointer to the TCP data in the pbuf */
  u16_t len;               /* the TCP length of this segment */
  u8_t  flags;


  struct tcp_hdr *tcphdr;  /* the TCP header */
};





/** This returns a TCP header option for MSS in an u32_t */





/* Internal functions and global variables: */
struct tcp_pcb *tcp_pcb_copy(struct tcp_pcb *pcb);
void tcp_pcb_purge(struct tcp_pcb *pcb);
void tcp_pcb_remove(struct tcp_pcb **pcblist, struct tcp_pcb *pcb);

u8_t tcp_segs_free(struct tcp_seg *seg);
u8_t tcp_seg_free(struct tcp_seg *seg);
struct tcp_seg *tcp_seg_copy(struct tcp_seg *seg);

#line 565 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"







err_t tcp_send_ctrl(struct tcp_pcb *pcb, u8_t flags);
err_t tcp_enqueue(struct tcp_pcb *pcb, void *dataptr, u16_t len,
                  u8_t flags, u8_t apiflags, u8_t optflags);

void tcp_rexmit_seg(struct tcp_pcb *pcb, struct tcp_seg *seg);

void tcp_rst(u32_t seqno, u32_t ackno,
       struct ip_addr *local_ip, struct ip_addr *remote_ip,
       u16_t local_port, u16_t remote_port);

u32_t tcp_next_iss(void);

void tcp_keepalive(struct tcp_pcb *pcb);
void tcp_zero_window_probe(struct tcp_pcb *pcb);


u16_t tcp_eff_send_mss(u16_t sendmss, struct ip_addr *addr);



err_t tcp_recv_null(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);


extern struct tcp_pcb *tcp_input_pcb;
extern u32_t tcp_ticks;

const char* tcp_debug_state_str(enum tcp_state s);
#line 612 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"




void tcp_timer_needed(void);


/* The TCP PCB lists. */
union tcp_listen_pcbs_t { /* List of all TCP PCBs in LISTEN state. */
  struct tcp_pcb_listen *listen_pcbs; 
  struct tcp_pcb *pcbs;
};
extern union tcp_listen_pcbs_t tcp_listen_pcbs;
extern struct tcp_pcb *tcp_active_pcbs;  /* List of all TCP PCBs that are in a
              state in which they accept or send
              data. */
extern struct tcp_pcb *tcp_tw_pcbs;      /* List of all TCP PCBs in TIME-WAIT. */

extern struct tcp_pcb *tcp_tmp_pcb;      /* Only used for temporary storage. */

/* Axioms about the above lists:   
   1) Every TCP PCB that is not CLOSED is in one of the lists.
   2) A PCB is only in one of the lists.
   3) All PCBs in the tcp_listen_pcbs list is in LISTEN state.
   4) All PCBs in the tcp_tw_pcbs list is in TIME-WAIT state.
*/

/* Define two macros, TCP_REG and TCP_RMV that registers a TCP PCB
   with a PCB list or removes a PCB from a list, respectively. */
#line 673 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"

#line 680 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"

#line 698 "C:\\projects\\Spitfire\\Branches_Embedded\\FECInjector\\Common\\Libraries\\STM32F4x7_ETH_LwIP\\Utilities\\Third_Party\\lwip_v1.3.2\\src\\include\\lwip/tcp.h"









#line 54 "..\\src\\api\\api_lib.c"

#line 1 "C:\\iar_ew\\arm\\inc\\c\\string.h"
/* string.h standard header */
/* Copyright 2009-2010 IAR Systems AB. */




  #pragma system_include


#line 1 "C:\\iar_ew\\arm\\inc\\c\\ycheck.h"
/* ycheck.h internal checking header file. */
/* Copyright 2005-2010 IAR Systems AB. */

/* Note that there is no include guard for this header. This is intentional. */


  #pragma system_include


/* __INTRINSIC
 *
 * Note: Redefined each time ycheck.h is included, i.e. for each
 * system header, to ensure that intrinsic support could be turned off
 * individually for each file.
 */










/* __AEABI_PORTABILITY_INTERNAL_LEVEL
 *
 * Note: Redefined each time ycheck.h is included, i.e. for each
 * system header, to ensure that ABI support could be turned off/on
 * individually for each file.
 *
 * Possible values for this preprocessor symbol:
 *
 * 0 - ABI portability mode is disabled.
 *
 * 1 - ABI portability mode (version 1) is enabled.
 *
 * All other values are reserved for future use.
 */






#line 67 "C:\\iar_ew\\arm\\inc\\c\\ycheck.h"

#line 11 "C:\\iar_ew\\arm\\inc\\c\\string.h"
#line 1 "C:\\iar_ew\\arm\\inc\\c\\ysizet.h"
/* ysizet.h internal header file. */
/* Copyright 2003-2010 IAR Systems AB.  */

#line 30 "C:\\iar_ew\\arm\\inc\\c\\ysizet.h"







#line 13 "C:\\iar_ew\\arm\\inc\\c\\string.h"

#line 1 "C:\\iar_ew\\arm\\inc\\c\\DLib_Product_string.h"
/**************************************************
 *
 * ARM-specific configuration for string.h in DLib.
 *
 * Copyright 2006 IAR Systems. All rights reserved.
 *
 * $Id: DLib_Product_string.h 78576 2014-05-05 13:37:27Z mats $
 *
 **************************************************/





  #pragma system_include




  
  

  /*
   * The following is pre-declared by the compiler.
   *
   * __INTRINSIC void __aeabi_memset (void *, size_t, int);
   * __INTRINSIC void __aeabi_memcpy (void *, const void *, size_t);
   * __INTRINSIC void __aeabi_memmove(void *, const void *, size_t);
   */


  /*
   * Inhibit inline definitions for routines with an effective
   * ARM-specific implementation.
   *
   * Not in Cortex-M1 and Cortex-MS1
   */



#line 47 "C:\\iar_ew\\arm\\inc\\c\\DLib_Product_string.h"



  /*
   * Redirect calls to standard functions to the corresponding
   * __aeabi_X function.
   */


  #pragma inline=forced_no_body
  __intrinsic __nounwind void * memcpy(void * _D, const void * _S, size_t _N)
  {
    __aeabi_memcpy(_D, _S, _N);
    return _D;
  }


  #pragma inline=forced_no_body
  __intrinsic __nounwind void * memmove(void * _D, const void * _S, size_t _N)
  {
    __aeabi_memmove(_D, _S, _N);
    return _D;
  }


  #pragma inline=forced_no_body
  __intrinsic __nounwind void * memset(void * _D, int _C, size_t _N)
  {
    __aeabi_memset(_D, _N, _C);
    return _D;
  }

  
  



#line 16 "C:\\iar_ew\\arm\\inc\\c\\string.h"



                /* macros */




                /* declarations */

_Pragma("function_effects = no_state, no_errno, no_write(1,2)")   __intrinsic __nounwind int        memcmp(const void *, const void *,
                                                size_t);
_Pragma("function_effects = no_state, no_errno, no_read(1), no_write(2), returns 1") __intrinsic __nounwind void *     memcpy(void *, 
                                                const void *, size_t);
_Pragma("function_effects = no_state, no_errno, no_read(1), no_write(2), returns 1") __intrinsic __nounwind void *     memmove(void *, const void *, size_t);
_Pragma("function_effects = no_state, no_errno, no_read(1), returns 1")    __intrinsic __nounwind void *     memset(void *, int, size_t);
_Pragma("function_effects = no_state, no_errno, no_write(2), returns 1")    __intrinsic __nounwind char *     strcat(char *, 
                                                const char *);
_Pragma("function_effects = no_state, no_errno, no_write(1,2)")   __intrinsic __nounwind int        strcmp(const char *, const char *);
_Pragma("function_effects = no_write(1,2)")     __intrinsic __nounwind int        strcoll(const char *, const char *);
_Pragma("function_effects = no_state, no_errno, no_read(1), no_write(2), returns 1") __intrinsic __nounwind char *     strcpy(char *, 
                                                const char *);
_Pragma("function_effects = no_state, no_errno, no_write(1,2)")   __intrinsic __nounwind size_t     strcspn(const char *, const char *);
                 __intrinsic __nounwind char *     strerror(int);
_Pragma("function_effects = no_state, no_errno, no_write(1)")      __intrinsic __nounwind size_t     strlen(const char *);
_Pragma("function_effects = no_state, no_errno, no_write(2), returns 1")    __intrinsic __nounwind char *     strncat(char *, 
                                                 const char *, size_t);
_Pragma("function_effects = no_state, no_errno, no_write(1,2)")   __intrinsic __nounwind int        strncmp(const char *, const char *, 
                                                 size_t);
_Pragma("function_effects = no_state, no_errno, no_read(1), no_write(2), returns 1") __intrinsic __nounwind char *     strncpy(char *, 
                                                 const char *, size_t);
_Pragma("function_effects = no_state, no_errno, no_write(1,2)")   __intrinsic __nounwind size_t     strspn(const char *, const char *);
_Pragma("function_effects = no_write(2)")        __intrinsic __nounwind char *     strtok(char *, 
                                                const char *);
_Pragma("function_effects = no_write(2)")        __intrinsic __nounwind size_t     strxfrm(char *, 
                                                 const char *, size_t);


  _Pragma("function_effects = no_write(1)")      __intrinsic __nounwind char *   strdup(const char *);
  _Pragma("function_effects = no_write(1,2)")   __intrinsic __nounwind int      strcasecmp(const char *, const char *);
  _Pragma("function_effects = no_write(1,2)")   __intrinsic __nounwind int      strncasecmp(const char *, const char *, 
                                                   size_t);
  _Pragma("function_effects = no_state, no_errno, no_write(2)")    __intrinsic __nounwind char *   strtok_r(char *, const char *, char **);
  _Pragma("function_effects = no_state, no_errno, no_write(1)")    __intrinsic __nounwind size_t   strnlen(char const *, size_t);




#line 81 "C:\\iar_ew\\arm\\inc\\c\\string.h"
  _Pragma("function_effects = no_state, no_errno, no_write(1)")    __intrinsic __nounwind void *memchr(const void *_S, int _C, size_t _N);
  _Pragma("function_effects = no_state, no_errno, no_write(1)")    __intrinsic __nounwind char *strchr(const char *_S, int _C);
  _Pragma("function_effects = no_state, no_errno, no_write(1,2)") __intrinsic __nounwind char *strpbrk(const char *_S, const char *_P);
  _Pragma("function_effects = no_state, no_errno, no_write(1)")    __intrinsic __nounwind char *strrchr(const char *_S, int _C);
  _Pragma("function_effects = no_state, no_errno, no_write(1,2)") __intrinsic __nounwind char *strstr(const char *_S, const char *_P);




                /* Inline definitions. */


                /* The implementations. */

_Pragma("function_effects = no_state, no_errno, no_write(1)")    __intrinsic __nounwind void *__iar_Memchr(const void *, int, size_t);
_Pragma("function_effects = no_state, no_errno, no_write(1)")    __intrinsic __nounwind char *__iar_Strchr(const char *, int);
               __intrinsic __nounwind char *__iar_Strerror(int, char *);
_Pragma("function_effects = no_state, no_errno, no_write(1,2)") __intrinsic __nounwind char *__iar_Strpbrk(const char *, const char *);
_Pragma("function_effects = no_state, no_errno, no_write(1)")    __intrinsic __nounwind char *__iar_Strrchr(const char *, int);
_Pragma("function_effects = no_state, no_errno, no_write(1,2)") __intrinsic __nounwind char *__iar_Strstr(const char *, const char *);


                /* inlines and overloads, for C and C++ */
#line 168 "C:\\iar_ew\\arm\\inc\\c\\string.h"
                /* Then the overloads for C. */
    #pragma inline
    void *memchr(const void *_S, int _C, size_t _N)
    {
      return (__iar_Memchr(_S, _C, _N));
    }

    #pragma inline
    char *strchr(const char *_S, int _C)
    {
      return (__iar_Strchr(_S, _C));
    }

    #pragma inline
    char *strpbrk(const char *_S, const char *_P)
    {
      return (__iar_Strpbrk(_S, _P));
    }

    #pragma inline
    char *strrchr(const char *_S, int _C)
    {
      return (__iar_Strrchr(_S, _C));
    }

    #pragma inline
    char *strstr(const char *_S, const char *_P)
    {
      return (__iar_Strstr(_S, _P));
    }


  #pragma inline
  char *strerror(int _Err)
  {
    return (__iar_Strerror(_Err, 0));
  }

#line 451 "C:\\iar_ew\\arm\\inc\\c\\string.h"






#line 479 "C:\\iar_ew\\arm\\inc\\c\\string.h"

/*
 * Copyright (c) 1992-2009 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V5.04:0576 */
#line 56 "..\\src\\api\\api_lib.c"

/**
 * Create a new netconn (of a specific type) that has a callback function.
 * The corresponding pcb is also created.
 *
 * @param t the type of 'connection' to create (@see enum netconn_type)
 * @param proto the IP protocol for RAW IP pcbs
 * @param callback a function to call on status changes (RX available, TX'ed)
 * @return a newly allocated struct netconn or
 *         NULL on memory error
 */
struct netconn*
netconn_new_with_proto_and_callback(enum netconn_type t, u8_t proto, netconn_callback callback)
{
  struct netconn *conn;
  struct api_msg msg;

  conn = netconn_alloc(t, callback);
  if (conn != 0 ) {
    msg.function = do_newconn;
    msg.msg.msg.n.proto = proto;
    msg.msg.conn = conn;
    tcpip_apimsg(&msg);

    if (conn->err != 0) {
      do { if(!(conn->pcb . tcp == 0)) ; } while(0);
      do { if(!(conn->op_completed != (SemaphoreHandle_t)0)) ; } while(0);
      do { if(!(conn->recvmbox != (QueueHandle_t)0)) ; } while(0);
      do { if(!(conn->acceptmbox == (QueueHandle_t)0)) ; } while(0);
      sys_sem_free(conn->op_completed);
      sys_mbox_free(conn->recvmbox);
      memp_free(MEMP_NETCONN, conn);
      return 0;
    }
  }
  return conn;
}

/**
 * Close a netconn 'connection' and free its resources.
 * UDP and RAW connection are completely closed, TCP pcbs might still be in a waitstate
 * after this returns.
 *
 * @param conn the netconn to delete
 * @return ERR_OK if the connection was deleted
 */
err_t
netconn_delete(struct netconn *conn)
{
  struct api_msg msg;

  /* No ASSERT here because possible to get a (conn == NULL) if we got an accept error */
  if (conn == 0) {
    return 0;
  }

  msg.function = do_delconn;
  msg.msg.conn = conn;
  tcpip_apimsg(&msg);

  conn->pcb.tcp = 0;
  netconn_free(conn);

  return 0;
}

/**
 * Get the local or remote IP address and port of a netconn.
 * For RAW netconns, this returns the protocol instead of a port!
 *
 * @param conn the netconn to query
 * @param addr a pointer to which to save the IP address
 * @param port a pointer to which to save the port (or protocol for RAW)
 * @param local 1 to get the local IP address, 0 to get the remote one
 * @return ERR_CONN for invalid connections
 *         ERR_OK if the information was retrieved
 */
err_t
netconn_getaddr(struct netconn *conn, struct ip_addr *addr, u16_t *port, u8_t local)
{
  struct api_msg msg;

  do { if (!((conn != 0))) { ; return -10;;}} while(0);
  do { if (!((addr != 0))) { ; return -10;;}} while(0);
  do { if (!((port != 0))) { ; return -10;;}} while(0);

  msg.function = do_getaddr;
  msg.msg.conn = conn;
  msg.msg.msg.ad.ipaddr = addr;
  msg.msg.msg.ad.port = port;
  msg.msg.msg.ad.local = local;
  tcpip_apimsg(&msg);

  return conn->err;
}

/**
 * Bind a netconn to a specific local IP address and port.
 * Binding one netconn twice might not always be checked correctly!
 *
 * @param conn the netconn to bind
 * @param addr the local IP address to bind the netconn to (use IP_ADDR_ANY
 *             to bind to all addresses)
 * @param port the local port to bind the netconn to (not used for RAW)
 * @return ERR_OK if bound, any other err_t on failure
 */
err_t
netconn_bind(struct netconn *conn, struct ip_addr *addr, u16_t port)
{
  struct api_msg msg;

  do { if (!((conn != 0))) { ; return -10;;}} while(0);

  msg.function = do_bind;
  msg.msg.conn = conn;
  msg.msg.msg.bc.ipaddr = addr;
  msg.msg.msg.bc.port = port;
  tcpip_apimsg(&msg);
  return conn->err;
}

/**
 * Connect a netconn to a specific remote IP address and port.
 *
 * @param conn the netconn to connect
 * @param addr the remote IP address to connect to
 * @param port the remote port to connect to (no used for RAW)
 * @return ERR_OK if connected, return value of tcp_/udp_/raw_connect otherwise
 */
err_t
netconn_connect(struct netconn *conn, struct ip_addr *addr, u16_t port)
{
  struct api_msg msg;

  do { if (!((conn != 0))) { ; return -10;;}} while(0);

  msg.function = do_connect;
  msg.msg.conn = conn;
  msg.msg.msg.bc.ipaddr = addr;
  msg.msg.msg.bc.port = port;
  /* This is the only function which need to not block tcpip_thread */
  tcpip_apimsg(&msg);
  return conn->err;
}

/**
 * Disconnect a netconn from its current peer (only valid for UDP netconns).
 *
 * @param conn the netconn to disconnect
 * @return TODO: return value is not set here...
 */
err_t
netconn_disconnect(struct netconn *conn)
{
  struct api_msg msg;

  do { if (!((conn != 0))) { ; return -10;;}} while(0);

  msg.function = do_disconnect;
  msg.msg.conn = conn;
  tcpip_apimsg(&msg);
  return conn->err;
}

/**
 * Set a TCP netconn into listen mode
 *
 * @param conn the tcp netconn to set to listen mode
 * @param backlog the listen backlog, only used if TCP_LISTEN_BACKLOG==1
 * @return ERR_OK if the netconn was set to listen (UDP and RAW netconns
 *         don't return any error (yet?))
 */
err_t
netconn_listen_with_backlog(struct netconn *conn, u8_t backlog)
{
  struct api_msg msg;

  /* This does no harm. If TCP_LISTEN_BACKLOG is off, backlog is unused. */
  (void)backlog;

  do { if (!((conn != 0))) { ; return -10;;}} while(0);

  msg.function = do_listen;
  msg.msg.conn = conn;



  tcpip_apimsg(&msg);
  return conn->err;
}

/**
 * Accept a new connection on a TCP listening netconn.
 *
 * @param conn the TCP listen netconn
 * @return the newly accepted netconn or NULL on timeout
 */
struct netconn *
netconn_accept(struct netconn *conn)
{
  struct netconn *newconn;

  do { if (!((conn != 0))) { ; return 0;;}} while(0);
  do { if (!((conn->acceptmbox != (QueueHandle_t)0))) { ; return 0;;}} while(0);


  if (sys_arch_mbox_fetch(conn->acceptmbox, (void *)&newconn, conn->recv_timeout) == 0xffffffffUL) {
    newconn = 0;
  } else



  {
    /* Register event with callback */
    if (conn->callback) { (*conn->callback)(conn, NETCONN_EVT_RCVMINUS, 0); };

#line 281 "..\\src\\api\\api_lib.c"
  }

  return newconn;
}

/**
 * Receive data (in form of a netbuf containing a packet buffer) from a netconn
 *
 * @param conn the netconn from which to receive data
 * @return a new netbuf containing received data or NULL on memory error or timeout
 */
struct netbuf *
netconn_recv(struct netconn *conn)
{
  struct api_msg msg;
  struct netbuf *buf = 0;
  struct pbuf *p;
  u16_t len;

  do { if (!((conn != 0))) { ; return 0;;}} while(0);

  if (conn->recvmbox == (QueueHandle_t)0) {
    /* @todo: should calling netconn_recv on a TCP listen conn be fatal (ERR_CONN)?? */
    /* TCP listen conns don't have a recvmbox! */
    conn->err = -8;
    return 0;
  }

  if (((conn->err) < -4)) {
    return 0;
  }

  if (conn->type == NETCONN_TCP) {

    if (conn->state == NETCONN_LISTEN) {
      /* @todo: should calling netconn_recv on a TCP listen conn be fatal?? */
      conn->err = -8;
      return 0;
    }

    buf = memp_malloc(MEMP_NETBUF);

    if (buf == 0) {
      conn->err = -1;
      return 0;
    }


    if (sys_arch_mbox_fetch(conn->recvmbox, (void *)&p, conn->recv_timeout)==0xffffffffUL) {
      memp_free(MEMP_NETBUF, buf);
      conn->err = -3;
      return 0;
    }




    if (p != 0) {
      len = p->tot_len;
      do { sys_prot_t old_level; old_level = sys_arch_protect(); conn->recv_avail -= len; sys_arch_unprotect(old_level); } while(0);
    } else {
      len = 0;
    }

    /* Register event with callback */
    if (conn->callback) { (*conn->callback)(conn, NETCONN_EVT_RCVMINUS, len); };

    /* If we are closed, we indicate that we no longer wish to use the socket */
    if (p == 0) {
      memp_free(MEMP_NETBUF, buf);
      /* Avoid to lose any previous error code */
      if (conn->err == 0) {
        conn->err = -7;
      }
      return 0;
    }

    buf->p = p;
    buf->ptr = p;
    buf->port = 0;
    buf->addr = 0;

    /* Let the stack know that we have taken the data. */
    msg.function = do_recv;
    msg.msg.conn = conn;
    if (buf != 0) {
      msg.msg.msg.r.len = buf->p->tot_len;
    } else {
      msg.msg.msg.r.len = 1;
    }
    tcpip_apimsg(&msg);

  } else {


    if (sys_arch_mbox_fetch(conn->recvmbox, (void *)&buf, conn->recv_timeout)==0xffffffffUL) {
      buf = 0;
    }



    if (buf!=0) {
      do { sys_prot_t old_level; old_level = sys_arch_protect(); conn->recv_avail -= buf->p->tot_len; sys_arch_unprotect(old_level); } while(0);
      /* Register event with callback */
      if (conn->callback) { (*conn->callback)(conn, NETCONN_EVT_RCVMINUS, buf->p->tot_len); };
    }

  }

  ;

  return buf;
}

/**
 * Send data (in form of a netbuf) to a specific remote IP address and port.
 * Only to be used for UDP and RAW netconns (not TCP).
 *
 * @param conn the netconn over which to send data
 * @param buf a netbuf containing the data to send
 * @param addr the remote IP address to which to send the data
 * @param port the remote port to which to send the data
 * @return ERR_OK if data was sent, any other err_t on error
 */
err_t
netconn_sendto(struct netconn *conn, struct netbuf *buf, struct ip_addr *addr, u16_t port)
{
  if (buf != 0) {
    buf->addr = addr;
    buf->port = port;
    return netconn_send(conn, buf);
  }
  return -9;
}

/**
 * Send data over a UDP or RAW netconn (that is already connected).
 *
 * @param conn the UDP or RAW netconn over which to send data
 * @param buf a netbuf containing the data to send
 * @return ERR_OK if data was sent, any other err_t on error
 */
err_t
netconn_send(struct netconn *conn, struct netbuf *buf)
{
  struct api_msg msg;

  do { if (!((conn != 0))) { ; return -10;;}} while(0);

  ;
  msg.function = do_send;
  msg.msg.conn = conn;
  msg.msg.msg.b = buf;
  tcpip_apimsg(&msg);
  return conn->err;
}

/**
 * Send data over a TCP netconn.
 *
 * @param conn the TCP netconn over which to send data
 * @param dataptr pointer to the application buffer that contains the data to send
 * @param size size of the application data to send
 * @param apiflags combination of following flags :
 * - NETCONN_COPY (0x01) data will be copied into memory belonging to the stack
 * - NETCONN_MORE (0x02) for TCP connection, PSH flag will be set on last segment sent
 * @return ERR_OK if data was sent, any other err_t on error
 */
err_t
netconn_write(struct netconn *conn, const void *dataptr, size_t size, u8_t apiflags)
{
  struct api_msg msg;

  do { if (!((conn != 0))) { ; return -10;;}} while(0);
  do { if (!((conn->type == NETCONN_TCP))) { ; return -9;;}} while(0);

  msg.function = do_write;
  msg.msg.conn = conn;
  msg.msg.msg.w.dataptr = dataptr;
  msg.msg.msg.w.apiflags = apiflags;
  msg.msg.msg.w.len = size;
  /* For locking the core: this _can_ be delayed on low memory/low send buffer,
     but if it is, this is done inside api_msg.c:do_write(), so we can use the
     non-blocking version here. */
  tcpip_apimsg(&msg);
  return conn->err;
}

/**
 * Close a TCP netconn (doesn't delete it).
 *
 * @param conn the TCP netconn to close
 * @return ERR_OK if the netconn was closed, any other err_t on error
 */
err_t
netconn_close(struct netconn *conn)
{
  struct api_msg msg;

  do { if (!((conn != 0))) { ; return -10;;}} while(0);

  msg.function = do_close;
  msg.msg.conn = conn;
  tcpip_apimsg(&msg);
  return conn->err;
}


/**
 * Join multicast groups for UDP netconns.
 *
 * @param conn the UDP netconn for which to change multicast addresses
 * @param multiaddr IP address of the multicast group to join or leave
 * @param interface the IP address of the network interface on which to send
 *                  the igmp message
 * @param join_or_leave flag whether to send a join- or leave-message
 * @return ERR_OK if the action was taken, any err_t on error
 */
err_t
netconn_join_leave_group(struct netconn *conn,
                         struct ip_addr *multiaddr,
                         struct ip_addr *interface,
                         enum netconn_igmp join_or_leave)
{
  struct api_msg msg;

  do { if (!((conn != 0))) { ; return -10;;}} while(0);

  msg.function = do_join_leave_group;
  msg.msg.conn = conn;
  msg.msg.msg.jl.multiaddr = multiaddr;
  msg.msg.msg.jl.interface = interface;
  msg.msg.msg.jl.join_or_leave = join_or_leave;
  tcpip_apimsg(&msg);
  return conn->err;
}



/**
 * Execute a DNS query, only one IP address is returned
 *
 * @param name a string representation of the DNS host name to query
 * @param addr a preallocated struct ip_addr where to store the resolved IP address
 * @return ERR_OK: resolving succeeded
 *         ERR_MEM: memory error, try again later
 *         ERR_ARG: dns client not initialized or invalid hostname
 *         ERR_VAL: dns server response was invalid
 */
err_t
netconn_gethostbyname(const char *name, struct ip_addr *addr)
{
  struct dns_api_msg msg;
  err_t err;
  sys_sem_t sem;

  do { if (!((name != 0))) { ; return -10;;}} while(0);
  do { if (!((addr != 0))) { ; return -10;;}} while(0);

  sem = sys_sem_new(0);
  if (sem == (SemaphoreHandle_t)0) {
    return -1;
  }

  msg.name = name;
  msg.addr = addr;
  msg.err = &err;
  msg.sem = sem;

  tcpip_callback_with_block(do_gethostbyname, &msg, 1);
  sys_sem_wait(sem);
  sys_sem_free(sem);

  return err;
}


