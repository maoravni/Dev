#ifndef _CfgDB_h_
#define _CfgDB_h_

#include "ll_typedef.h"
#include "CfgDButil.h"

#ifdef __cplusplus
extern "C"
{
#endif


/* These are the export routine for the DB handle*/
STATUS CfgInit();


/* Write element (integer or string) into the database.
   The InstanceId is irrelevant for GNRL database.
   The only available date type for this routine are: integers and null terminated strings
*/

STATUS CfgPutParam(int InstanceId, CfgDB_INT_DATA id,void * Data,CfgDB_SET_FLAG SetFlag);
/* Read element (integer or string) from the database.
   The InstanceId is irrelevant for GNRL database.
   The DataLen is irrelevant for integer type elenemt, it use for illiminate
   the length of the data.

*/

STATUS CfgGetParam(int InstanceId,CfgDB_INT_DATA id,void * Data, int DataLen,CfgDB_GET_FLAG GetFlag);

/* get integer data type from GNRL database*/
STATUS CfgGetGnrlInt(CfgDB_INT_DATA id,int * Data);

/*return the date type from database*/
int CfgRetInt(int InstanceId,CfgDB_INT_DATA id);
int CfgRetGnrlInt(CfgDB_INT_DATA id);

/* sanity test for element in the database */
STATUS CfgTestInt(int InstanceId,CfgDB_INT_DATA id,int Data);

STATUS  CfgCopyGnrlCfgDbToMate(void);
STATUS  CfgCheckGnrlCfgDbMismatchToMate(void);
STATUS  CfgCopyDrawerCfgDbToMate(int InstanceId);
STATUS CfgCheckDrawerCfgDbMismatchToMate(int InstanceId);
STATUS CfgGetMinMaxLimitValueFromIntDescriptor(int InstanceId, U32 id, int * MinLimitValue,int * MaxLimitValue);
STATUS CfgIdTextFromDescriptor(int InstanceId, U32 id, char *IdText, int MaxTextLen);
STATUS CfgDB_TakeCfgSem(int InstanceId);
STATUS CfgDB_GiveCfgSem(int InstanceId,bool FromIsr);

#ifdef __cplusplus
}
#endif

#endif
