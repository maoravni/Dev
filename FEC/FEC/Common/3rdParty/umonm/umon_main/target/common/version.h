/* version.h:
 * The version number for -skinny umon- is 3 'dot' separated numbers.
 * Each number can be as large as is needed.
 *
 *	MAJOR_VERSION.MINOR_VERSION.TARGET_VERSION 

 * MAJOR, MINOR & BUILD apply to the common code applicable to all targets.
 * TARGET applies to the target-specific code.
 */
#ifndef _VERSION_H_
#define _VERSION_H_

/* MAJOR_VERSION:
 * Incremented as a result of a major change or enhancement to the core
 * monitor code or as a means of dropping the MINOR_VERSION back to zero;
 * hence, simply identifying a significant set of MINOR changes or some
 * big change.
 */
#define MAJOR_VERSION		0

/* MINOR_VERSION:
 * Incremented as a result of a new command or feature, or as a result
 * of a bug fix to the core monitor code.
 * When MAJOR_VERSION is incremented, MINOR_VERSION is reset to 0.
 *
 * 1->2 IPV6 link-local ping server works
 */
#define MINOR_VERSION	2

/* TARGET_VERSION:
 * Incremented as a result of a bug fix or change made to the
 * target-specific (i.e. port) portion of the code.
 * 
 * To keep a "cvs-like" log of the changes made to a port that is
 * not under CVS, it is recommended that the target_version.h file be
 * used as the log to keep track of changes in one place.
 */
#include "target_version.h"

#endif
