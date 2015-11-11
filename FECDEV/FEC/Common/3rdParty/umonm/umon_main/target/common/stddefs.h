/*	stddefs.h:
 *
 *	General notice:
 *	This code is part of a boot-monitor package developed as a generic base
 *	platform for embedded system designs.  As such, it is likely to be
 *	distributed to various projects beyond the control of the original
 *	author.  Please notify the author of any enhancements made or bugs found
 *	so that all may benefit from the changes.  In addition, notification back
 *	to the author will allow the new user to pick up changes that may have
 *	been made by other users after this version of the code was distributed.
 *
 *	Note1: the majority of this code was edited with 4-space tabs.
 *	Note2: as more and more contributions are accepted, the term "author"
 *		   is becoming a mis-representation of credit.
 *
 *	Original author:	Ed Sutter
 *	Email:				ed.sutter@alcatel-lucent.com
 *	Phone:				908-582-2351
 */
#ifndef _STDDEFS_H_
#define _STDDEFS_H_

/* These typedefs will be part of any C99-compliant toolset.
 * They are listed here so that stddefs.h can be used 
 * instead of stdint.h for non C99-compliant environments.
 */

typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned long		uint32_t;
typedef unsigned long long	uint64_t;

typedef signed char			int8_t;
typedef signed short		int16_t;
typedef signed long			int32_t;
typedef signed long long	int64_t;

typedef volatile unsigned char		vuint8_t;
typedef volatile unsigned short		vuint16_t;
typedef volatile unsigned long		vuint32_t;
typedef volatile unsigned long long	vuint64_t;

typedef volatile signed char		vint8_t;
typedef volatile signed short		vint16_t;
typedef volatile signed long		vint32_t;
typedef volatile signed long long	vint64_t;

#if 1
// TODO: replace all uses of these typedefs with
// the C99-compliant ones above...
typedef unsigned char		uchar;
typedef unsigned short		ushort;
typedef unsigned long		ulong;
typedef unsigned int		uint;

typedef volatile unsigned char	vuchar;
typedef volatile unsigned short	vushort;
typedef volatile unsigned long	vulong;
typedef volatile unsigned int	vuint;
typedef volatile int			vint;
#endif

/* This structure is used to catch the cases where the
 * above (C99-compliant) typedefs do not match the assumed
 * sizes.  Got this from comments in an article originally
 * written by Michael Barr...
 * http://www.embedded.com/electronics-blogs/beginner-s-corner/4024913/Introduction-to-fixed-width-integers
 */
static volatile union 
{ 
	char char_incorrect[sizeof( char) == 1 ? 1 : -1]; 
	char int8_t_incorrect[sizeof( int8_t) == 1 ? 1 : -1]; 
	char uint8_t_incorrect[sizeof( uint8_t) == 1 ? 1 : -1]; 
	char int16_t_incorrect[sizeof( int16_t) == 2 ? 1 : -1]; 
	char uint16_t_incorrect[sizeof(uint16_t) == 2 ? 1 : -1]; 
	char int32_t_incorrect[sizeof( int32_t) == 4 ? 1 : -1]; 
	char uint32_t_incorrect[sizeof(uint32_t) == 4 ? 1 : -1]; 
	char int64_t_incorrect[sizeof( int64_t) == 8 ? 1 : -1]; 
	char uint64_t_incorrect[sizeof(uint64_t) == 8 ? 1 : -1]; 

} typetests;

#endif
