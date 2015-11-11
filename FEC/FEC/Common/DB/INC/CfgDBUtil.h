#ifndef _CfgDButil_h_
#define _CfgDButil_h_

//#include "pq_defs.h" /* for U32 */
#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_CFG_STR_SIZE 256 /* should be multiplier of 8 because of using the sizeof (compiler struct alignment)*/
#define CfgDB_INT_DATA U32
#define CfgDB_ID_BIT_DATA_SIZE 32
#define CfgDB_INT_DATA_SIZE sizeof(CfgDB_INT_DATA)
#define CfgDB_DELETE_ID 0xffffffff


#pragma pack(1)

typedef enum {
	INSTANCE_GNRL=-1,
	INSTANCE_1=0,
	INSTANCE_2,
        MAX_NUM_OF_INSTANCE,
} INSTANCE_ID;

//#define MAX_NUM_OF_INSTANCE 2
#define MAX_LEN_PASSWORD	


typedef enum {
	CfgDB_GET_DIRECTLY_FROM_RAM=0x1, /* from RAM DB */
	CfgDB_GET_DIRECTLY_FROM_SOURCE=0x2, /* error if no get handle routine exist unless CfgDB_GET_DIRECTLY_FROM_RAM is set*/
	CfgDB_GET_DIRECTLY_FROM_DEFAULT_FILE=0x4, /* directly from modify DB file - error if not exist*/
	CfgDB_GET_DIRECTLY_FROM_MODIFIED_FILE=0x8,
	CfgDB_GET_WITH_CALLING_TO_HANDLE=0x10, /* at the end call to get handle if exists*/
	CfgDB_GET_IGNORE_BOARD_EXISTENCE=(0x20 |CfgDB_GET_DIRECTLY_FROM_MODIFIED_FILE | CfgDB_GET_DIRECTLY_FROM_DEFAULT_FILE)/* get to db file even if the board not exits*/
} CfgDB_GET_FLAG;

typedef enum {
	UNSIGNED_VALUE=0, /* 0- 2^32 */
	SIGNED_VALUE=1,
	DEFAULT_DATA_AS_POINTER=2,
        WRITE_RAM_BEFORE_SET_HANDLE=4,    //used when set_handle uses commniniaction (like modbus) to avoid racing condition on the MMI
        IGNORED_SET_HANDLE_DURING_INIT=8 //during power up don't call set handle
} CfgDB_VALUE_TYPE;


typedef enum {
	CfgDB_SET_TO_RAM=1, /* Update RAM part of the DB */
	CfgDB_SET_TO_FILE=2, /* Update FILE part of the DB*/
	CfgDB_SET_WITH_CALLING_TO_HANDLE=4,
	CfgDB_SET_WITH_INFORM_CLIENTS=8,
	CfgDB_SET_WITH_IGNORE_INFORM_REMOTE=0x10,
	CfgDB_SET_INFORM_REMOTE_ONLY_AND_IGNORED_REST_OF_THE_FLAG=0x20,
	CfgDB_SET_TEST_SANITY=0x40,
	CfgDB_SET_IGNORE_PROTECTED_MATE=0x80, /* Dont pass the current DB element modification to mate*/
	CfgDB_SET_INTERNAL_SET=0x100, /* not from the MMI (terminal or SNMPC)- use to bypass some tests*/
	CfgDB_SET_IGNORE_BOARD_EXISTENCE=0x200, /* write to db file even if the board not exits*/
	CfgDB_SET_FORCE_PROTECTED_MATE=0x400,  /* pass the current DB element modification to mate*/
	CfgDB_SET_PROTECTED_MATE_MISMATCH_CHECK=0x800,  /* pass the current DB element modification to mate for mismatch check only */
	CfgDB_SET_WITH_IGNORE_INFORM_BUSY_CLIENT=0x1000  /* ignore inform if client is busy or not exist (don't show error) */
} CfgDB_SET_FLAG;


#define CfgDB_SET_TO_DB (CfgDB_SET_TO_RAM| CfgDB_SET_TO_FILE | CfgDB_SET_WITH_CALLING_TO_HANDLE | CfgDB_SET_WITH_INFORM_CLIENTS)
#define CfgDB_SET_TO_DB_EXCLUDE_FILE (CfgDB_SET_TO_DB & ~CfgDB_SET_TO_FILE )
#define CfgDB_SET_TO_REMOTE (CfgDB_SET_WITH_CALLING_TO_HANDLE | CfgDB_SET_WITH_INFORM_CLIENTS)
#define CfgDB_GET_FROM_DB (CfgDB_GET_DIRECTLY_FROM_RAM )
#define CfgDB_TEST_SANITY (CfgDB_SET_TEST_SANITY | CfgDB_SET_WITH_CALLING_TO_HANDLE) /* without modifying the RAM nor the file, call the Set handle if exist with this flag*/
#define CfgDB_CMND_SET_TO_DB (CfgDB_SET_WITH_CALLING_TO_HANDLE | CfgDB_SET_WITH_INFORM_CLIENTS)

typedef enum {
	CfgDB_UnitGNRL,
	CfgDB_UnitTEMPERATURE,
        CfgDB_UnitOTHERS,
        CfgDB_UnitBTC,
	CfgDB_UnitLast
} CfgDB_UNIT_TYPE;


typedef enum {
	CfgDB_INT,
	CfgDB_STR
} CfgDB_DATA_TYPE;


typedef enum {
	CfgDB_NON_VOLATILE, /* read/write non volatile data*/
	CfgDB_COMMAND, /* read/write volatile data*/
	CfgDB_MONITOR, /* status / read only*/
    CfgDB_VOLATILE_STR /* non config string*/
} CfgDB_ACCESS_TYPE;


// TODO: refactor all status results of the database to return the enum type and not the a number.
// This way we can check on types, and also issue an OK message.
// TODO: Need to consolidate all error messages available to a single location.
typedef enum {
	CFG_DB_ERROR_ID_LIMIT=1000,  /*TBD replace the 1000 with global value*//* illegal id requeset */
	CFG_DB_ERROR_ID_INDEX_LIMIT, /* id index requeset is rejected by its suitable id descriptor*/
	CFG_DB_ERROR_ID_DATA_TYPE, /* illegal requesed data type */
	CFG_DB_ERROR_ID_ACCESS_TYPE, /* illegal requesed access type */
	CFG_DB_ERROR_ID_UNIT_TYPE, /* illegal requesed unit type */
	CFG_DB_ERROR_ID_INSTANCE_NUM, /* illegal requesed instance number */
	CFG_DB_ERROR_ID_ILLEGAL_SET_FLAG, /* set flag is not suitable to the operation*/
	CFG_DB_ERROR_ID_ILLEGAL_DATA_ELEMENT, /* data element is rejected by its descriptor*/
	CFG_DB_ERROR_ID_ILLEGAL_STR_LEN, /* string element is rejected because it's too long*/
	CFG_DB_ERROR_ID_HANDLE_RESULT,   /* if handle routine not return OK*/
	CFG_DB_ERROR_ID_HANDLE_BUSY,   /* if handle routine not completed the previous call*/
	CFG_DB_ERROR_ID_INFORM_BUSY,   /* if queue is full*/
	CFG_DB_ERROR_ID_CONFIG_MISMATCH
} CfgDB_ERROR_ID;

#define CfgDB_ID_AccessType_BitFieldSize 3
#define CfgDB_ID_DataType_BitFieldSize   1
#define CfgDB_ID_UnitType_BitFieldSize   5
#define CfgDB_ID_Id_Index_BitFieldSize   9
#define CfgDB_ID_Id_BitFieldSize         14
/*                                     ------
                               Total  32 bits */


//#if !WIN32 
/* the following strcture describe the U32 bit field structure of the database ID value*/
#if 0
typedef union {
	struct {/* for Motorola format - MSB first */
		U32 /*CfgDB_ACCESS_TYPE*/ AccessType : CfgDB_ID_AccessType_BitFieldSize;/* Configuration, command, monitor, etc*/
		U32 /*CfgDB_DATA_TYPE*/ DataType     : CfgDB_ID_DataType_BitFieldSize;  /* unit group type*/
		U32 /*CfgDB_UNIT_TYPE*/ UnitType     : CfgDB_ID_UnitType_BitFieldSize;  /* type of string or integer*/
		U32 Id_Index                 : CfgDB_ID_Id_Index_BitFieldSize;  /* ID index value*/
		U32 Id                       : CfgDB_ID_Id_BitFieldSize;        /* ID  */
	} Field;
	U32 DataAssign;
}  CfgDB_ID;
#endif


//#else
/* the following strcture describe the U32 bit field structure of the database ID value*/
typedef union {
	struct { /* for Intel (PC) format - LSB first */
		U32 Id                       : CfgDB_ID_Id_BitFieldSize;        /* ID  */
		U32 Id_Index                 : CfgDB_ID_Id_Index_BitFieldSize;  /* ID index value*/
		U32 /*CfgDB_UNIT_TYPE*/ UnitType     : CfgDB_ID_UnitType_BitFieldSize;  /* type of string or integer*/
		U32 /*CfgDB_DATA_TYPE*/ DataType     : CfgDB_ID_DataType_BitFieldSize;  /* unit group type*/
		U32 /*CfgDB_ACCESS_TYPE*/ AccessType: CfgDB_ID_AccessType_BitFieldSize;/* Configuration, command, monitor, etc*/
	} Field;
	U32 DataAssign;
}  CfgDB_ID;



/* extra operation for each set request on the database*/
typedef enum {
	INFORM_NOTHING=0,
	INFORM_GNRL=1,   /* at the end of the setting, send the request to the GNRL task*/
	INFORM_TEMPERATURE=2,   /* at the end of the setting, send the request to the CHAMBER task*/
        INFORM_OTHERS=3,   /* at the end of the setting, send the request to the OTHERS task*/
	INFORM_TBD=4,   /* at the end of the setting, send the request to the ODU task*/
	
        INFORM_FORCE_USE_GET_HANDLE=0x200, /* if get_handle exist use it - like the CfgDB_GET_WITH_CALLING_TO_HANDLE flag*/
	
	INFORM_CONSTANT_MAX_INDEX=0x400, /* don't modified the maxAllowedIndex during run time*/
	
} CfgDB_INFORM_CLIENTS;

/* descriptor structure for each integer element in the database*/
typedef struct {
    char  *IdText;
	CfgDB_INFORM_CLIENTS ClientWhichNeedToBeInform;   /* set on this ID need to be updated on this client*/
	CfgDB_INT_DATA MinLimitValue;  /* min available value*/
	CfgDB_INT_DATA MaxLimitValue ; /* max available value*/
    U8  SignedLimitCheck;   /* false for unsigned limitation check*/
	U8  MaxAllowedIndex; /* number of available entries  */
	CfgDB_INT_DATA DefaultValue;    /* the default value */
	STATUS (*GetHandle)(int InstanceId,int Index,int *data);
	STATUS (*SetHandle)(int InstanceId,int Index,U32 data,CfgDB_SET_FLAG *SetFlag);
}  CfgDB_ID_IntDescriptor;

/* descriptor structure for each string element in the database*/
typedef struct {
    char  *IdText;
	CfgDB_INFORM_CLIENTS ClientWhichNeedToBeInform;   /* set on this ID need to be updated on this client*/
	U16  MaxAllowedStrLen ; /* max allowed setting string length*/
	U8  MaxAllowedIndex; /* number of available entries  */
	char DefaultValue[MAX_CFG_STR_SIZE];
	STATUS (*GetHandle)(int InstanceId,int Index,char *data);
	STATUS (*SetHandle)(int InstanceId,int Index,char *data,CfgDB_SET_FLAG *SetFlag);
}  CfgDB_ID_StrDescriptor;


/*  data element in the database can be in one of the folowing types*/
typedef union {
	char StrData[MAX_CFG_STR_SIZE];
	U32 IntData;
}  CfgDB_ID_data;


#pragma pack()


#define CfgDB_ID_idMask ((1<<CfgDB_ID_Id_BitFieldSize)-1)

#define CfgDB_ID_id(id) (id & CfgDB_ID_idMask) /* leave only the id field*/

#define CfgDB_ID_AddIndex(id,Index) ((Index<<CfgDB_ID_Id_BitFieldSize) | id) /* add index field to id data element*/

#define CfgDB_ID_FirstId(Unit,Access,DataType) Access<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
							 DataType<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
							 Unit<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)
/*------------------------------------------------------------------------------------*/

/* for the GNRL */
#define ID_GNRL_CFG_INT_FIRST CfgDB_NON_VOLATILE<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
							 CfgDB_INT<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
							 CfgDB_UnitGNRL<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)

#define ID_GNRL_CFG_STR_FIRST CfgDB_NON_VOLATILE<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
							CfgDB_STR<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
							CfgDB_UnitGNRL<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)

#define ID_GNRL_CMND_INT_FIRST CfgDB_COMMAND<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
							CfgDB_INT<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
							CfgDB_UnitGNRL<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)

#define ID_GNRL_MNTR_INT_FIRST CfgDB_MONITOR<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
						    	CfgDB_INT<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
						     	CfgDB_UnitGNRL<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)

#define ID_GNRL_VOLATILE_STR_FIRST CfgDB_VOLATILE_STR<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
						    	CfgDB_STR<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
						     	CfgDB_UnitGNRL<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)



/* for Temperature*/
#define ID_TEMPERATURE_CFG_INT_FIRST CfgDB_NON_VOLATILE<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
							CfgDB_INT<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
							CfgDB_UnitTEMPERATURE<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)

#define ID_TEMPERATURE_CFG_STR_FIRST CfgDB_NON_VOLATILE<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
							CfgDB_STR<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
							CfgDB_UnitTEMPERATURE<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)

#define ID_TEMPERATURE_CMND_INT_FIRST CfgDB_COMMAND<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
							CfgDB_INT<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
							CfgDB_UnitTEMPERATURE<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)

#define ID_TEMPERATURE_MNTR_INT_FIRST CfgDB_MONITOR<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
						CfgDB_INT<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
						CfgDB_UnitTEMPERATURE<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)

#define ID_TEMPERATURE_VOLATILE_STR_FIRST CfgDB_VOLATILE_STR<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
						    	CfgDB_STR<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
						     	CfgDB_UnitTEMPERATURE<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)


                                                          

                                                          
/* for Others*/
#define ID_OTHERS_CFG_INT_FIRST CfgDB_NON_VOLATILE<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
							CfgDB_INT<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
							CfgDB_UnitOTHERS<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)

#define ID_OTHERS_CFG_STR_FIRST CfgDB_NON_VOLATILE<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
							CfgDB_STR<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
							CfgDB_UnitOTHERS<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)

#define ID_OTHERS_CMND_INT_FIRST CfgDB_COMMAND<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
							CfgDB_INT<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
							CfgDB_UnitOTHERS<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)

#define ID_OTHERS_MNTR_INT_FIRST CfgDB_MONITOR<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
						CfgDB_INT<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
						CfgDB_UnitOTHERS<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)

#define ID_OTHERS_VOLATILE_STR_FIRST CfgDB_VOLATILE_STR<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
						    	CfgDB_STR<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
						     	CfgDB_UnitOTHERS<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)


/* for BTC*/
#define ID_BTC_CFG_INT_FIRST CfgDB_NON_VOLATILE<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
                            CfgDB_INT<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
                            CfgDB_UnitBTC<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)

#define ID_BTC_CFG_STR_FIRST CfgDB_NON_VOLATILE<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
                            CfgDB_STR<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
                            CfgDB_UnitBTC<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)

#define ID_BTC_CMND_INT_FIRST CfgDB_COMMAND<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
                            CfgDB_INT<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
                            CfgDB_UnitBTC<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)

#define ID_BTC_MNTR_INT_FIRST CfgDB_MONITOR<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
                        CfgDB_INT<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
                        CfgDB_UnitBTC<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)

#define ID_BTC_VOLATILE_STR_FIRST CfgDB_VOLATILE_STR<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize) | \
                                CfgDB_STR<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize) | \
                                CfgDB_UnitBTC<<(CfgDB_ID_BIT_DATA_SIZE-CfgDB_ID_AccessType_BitFieldSize-CfgDB_ID_DataType_BitFieldSize-CfgDB_ID_UnitType_BitFieldSize)



/*------------------------------------------------------------------------------------*/


/* The folowing routines used by other databas elements*/
STATUS CfgDB_FindFilesPathAndNames(CfgDB_ID id, char * DefaultFileName,
								   char * ModifiedFileName, int InstanceId);


STATUS CfgDB_CreateIntRamDB (CfgDB_ID_IntDescriptor IntDescriptor[],
						  CfgDB_INT_DATA* IntBufPtr[],int MaxElements);

void CfgDB_DeleteIntRamDB (CfgDB_INT_DATA* IntBufPtr[],int MaxElements);

STATUS CfgDB_CreateStrRamDB (CfgDB_ID_StrDescriptor StrDescriptor[],
						  char * StrBufPtr[],int MaxElements);

void CfgDB_DeleteStrRamDB (char * StrBufPtr[],int MaxElements);

/* generic routine that handle (setting and getting) data element type from RAM DB buffer*/
STATUS  CfgDB_GetElement (int InstanceId, CfgDB_ID Id, CfgDB_ID_data * buff,int DataLen,
								 CfgDB_GET_FLAG GetFlag);

STATUS  CfgDB_SetElement (int InstanceId, CfgDB_ID Id, CfgDB_ID_data * buff,
								     CfgDB_SET_FLAG SetFlag);

STATUS  CfgDB_GetIntElement (int InstanceId,CfgDB_ID Id, CfgDB_INT_DATA* IntBufPtr[],
								   CfgDB_ID_IntDescriptor * IntDescriptor,
								   CfgDB_INT_DATA * Data, CfgDB_GET_FLAG GetFlag);

STATUS  CfgDB_SetIntElement (int InstanceId,CfgDB_ID Id, CfgDB_INT_DATA* IntBufPtr[],
								   CfgDB_ID_IntDescriptor * IntDescriptor,
								   CfgDB_INT_DATA Data, CfgDB_SET_FLAG SetFlag);

STATUS  CfgDB_GetStrElement (int InstanceId,CfgDB_ID Id, char* StrBufPtr[],
								   CfgDB_ID_StrDescriptor * StrDescriptor,
								   char * Data, int DataLen,CfgDB_GET_FLAG GetFlag);


STATUS  CfgDB_SetStrElement (int InstanceId,CfgDB_ID Id, char* StrBufPtr[],
								   CfgDB_ID_StrDescriptor * StrDescriptor,
								   char * Data, int DataLen,CfgDB_SET_FLAG SetFlag);

STATUS CfgDB_CallToAllIntSetHandlesAndInformClients(int InstanceId,CfgDB_INT_DATA* IntBufPtr[],
									CfgDB_ID_IntDescriptor IntDescriptor[],
								    int FirstId,int LastId);

STATUS CfgDB_CallToAllStrSetHandlesAndInformClients(int InstanceId,char* StrBufPtr[],
									CfgDB_ID_StrDescriptor StrDescriptor[],
								    int FirstId,int LastId);

STATUS CfgDB_UpdateIntDbFromSource(int InstanceId, CfgDB_INT_DATA* IntBufPtr[],
									CfgDB_ID_IntDescriptor IntDescriptor[],
								    int FirstId,int LastId,BOOL MismatchCheck,
									BOOL LoadConfigFromUnit);

STATUS CfgDB_UpdateStrDbFromSource(int InstanceId,char* StrBufPtr[],
									CfgDB_ID_StrDescriptor StrDescriptor[],
								    int FirstId,int LastId,BOOL MismatchCheck,
									BOOL LoadConfigFromUnit);

void  CfgDB_SetMaxIndexInIntDescriptor(U32 Id,CfgDB_ID_IntDescriptor IntDescriptor[],
								   int MaxIndex);
void  CfgDB_SetMaxIndexInStrDescriptor(U32 id,CfgDB_ID_StrDescriptor StrDescriptor[],
								   int MaxIndex);

//STATUS UpdatePostResetHeaderVersionAndDescriptionStrDb(int InstanceId,DOWNLOAD_FileType FileType, int FileSubType,int Firmware,int FirmwareIndex,int SelectVer);
char * Id2Str(int Id, char * RetStr);
char * CfgUnit2Str(int Id);
char * Access2Str(int Id);
char * DataType2Str(int Id);
char * Err2Str(CfgDB_ERROR_ID ErrId);
BOOL CfgIdWasModified(int InstanceId, CfgDB_INT_DATA Id);
CfgDB_INT_DATA CfgDB_GetDefaultValueFromIntDecscriptor(U32 id,CfgDB_ID_IntDescriptor IntDescriptor[]);
STATUS CfgDB_CopyStrDbToMate(int InstanceId,char* StrBufPtr[],
									CfgDB_ID_StrDescriptor StrDescriptor[],
								    int FirstId,int LastId,int MismatchCheckMode);
STATUS CfgDB_CopyIntDbToMate(int InstanceId, CfgDB_INT_DATA* IntBufPtr[],
									CfgDB_ID_IntDescriptor IntDescriptor[],
								    int FirstId,int LastId,int MismatchCheckMode);

void CfgTimeOutCreate(int InstanceId,CfgDB_INT_DATA Id, int data,int count);

void CfgTimeOutDelete(int InstanceId,CfgDB_INT_DATA Id);

STATUS  CfgDB_GetMinMaxLimitValueFromIntDescriptor(U32 id,CfgDB_ID_IntDescriptor IntDescriptor[],
								   int * MinLimitValue,int * MaxLimitValue);
void CfgSetFactoryDefault(int InstanceId, BOOL EntireDisk);
STATUS  CfgDB_MinMax(CfgDB_INT_DATA ReqValue, CfgDB_INT_DATA MinValue, CfgDB_INT_DATA MaxValue, BOOL SignedCheck);
STATUS CfgExcludeIdPrefix(char * IdText, char * IdWithoutPrefix);

BOOL InSimulationMode();
void ActiveSimulationMode(BOOL Acitive);

#ifdef __cplusplus
}
#endif

#endif
