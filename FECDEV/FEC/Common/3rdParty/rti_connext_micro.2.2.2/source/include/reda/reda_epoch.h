/*
 (c) Copyright, Real-Time Innovations, 2012-2013.
 All rights reserved.
 
 No duplications, whole or partial, manual or electronic, may be made
 without express written permission.  Any such copies, or
 revisions thereof, must display this notice unaltered.
 This code contains trade secrets of Real-Time Innovations, Inc.
*/

#ifndef reda_epoch_h
#define reda_epoch_h



#ifndef reda_dll_h
#include "reda/reda_dll.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/* Doxygen file & module documentation ==================================*/
/*e \file 
    \brief Defines the REDA_Epoch interface
*/

/*e \defgroup REDA_EpochClass REDA_Epoch
  \ingroup REDAModule
  \brief used to determine staleness, version counting, and central to
  lockless table.

  That is, multiple threads can concurrently write the epoch.
  Multiple concurrent readers will never see intermediate garbage values. 
  We assume  writing an "unsigned int" is atomic on all architectures for now.

  All weakly referenced information has an epoch which is strictly
  monotonically increasing with every change.  If the referer finds that
  his idea of epoch is different than the current epoch, it means that
  his information is stale and therefore should not be used.
*/

/*e \ingroup REDA_EpochClass
  @brief Definition of the type
  @todo consider making the epoch the LONGEST int that can be written
  atomically (e.g. long) on every platform
*/
typedef RTI_INT32 REDA_Epoch_T;

/*e \ingroup REDA_EpochClass
  @brief Unknown REDA_Epoch

  Usually, when an epoch is unknown, the actor should assume the information
  tied to the unknown epoch is stale.  Depending on the usage, it may also
  mean that the caller wants to find the correct latest epoch.
*/
#define REDA_EPOCH_UNKNOWN (0)

/*i \ingroup REDA_EpochClass

  For wrap detection.
  WARNING: ASSUMPTION: an unsigned int is at least 32 bits (it's OK
  if it's bigger).
*/
#define REDA_EPOCH_HALF_VALUE ((REDA_Epoch)0x80000000)

/*e \ingroup REDA_EpochClass
  Tests whether this epoch is greater than the value
  @return 1 if true, 0 otherwise
*/
REDADllExport RTI_INT32
REDA_Epoch_isGreater(REDA_Epoch_T me, REDA_Epoch_T value);

/*e \ingroup REDA_EpochClass
  Tests whether this epoch is less than the value
  @return 1 if true, 0 otherwise
*/
REDADllExport RTI_INT32
REDA_Epoch_isLess(REDA_Epoch_T me, REDA_Epoch_T value);

#ifdef __cplusplus
}                               /* extern "C" */
#endif

#include "reda/reda_epoch_impl.h"

#endif /* reda_epoch_h */
