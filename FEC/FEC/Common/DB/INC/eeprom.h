#ifndef _eeprom_h_
#define _eeprom_h_




#define EEPROM_VERSION_STRUCT 0x01


#define EEPROM_BOARD_SERIAL_NUM_SIZE 11
#define EEPROM_BOARD_PART_NUM_SIZE 11
#define EEPROM_COMMENT_TEXT_SIZE 127
#define EEPROM_MAX_NUM_OF_INTERFACES 3
#define EEPROM_WAS_ZEROS_MARK 'YOSI'

typedef enum {
			   EEPROM_BOARD_NA,
			   EEPROM_BOARD_IDC,
               EEPROM_BOARD_MUX,
			   EEPROM_BOARD_MODEM,
			   EEPROM_BOARD_SD
			  } EEPROM_BOARD_TYPE;

typedef enum {
			   EEPROM_MUX_BOARD_NA,
			   EEPROM_MUX_BOARD_STM_REG,
			   EEPROM_MUX_BOARD_FAST_E1_T1,
/*			   EEPROM_MUX_BOARD_STM_REG_HITLESS,*/
#ifndef _GBE
			   EEPROM_MUX_BOARD_GIGABIT_FAST,
#else
			   EEPROM_MUX_BOARD_GIGABIT_E1_T1,
#endif
			   EEPROM_MUX_BOARD_DS3_E3,
			   EEPROM_MUX_BOARD_STM4_REG,
#ifdef _SUMMIT_
			   EEPROM_MUX_BOARD_SUMMIT_E1_T1,
#endif
			   EEPROM_MUX_BOARD_SUB_LAST
			  } EEPROM_MUX_BOARD_SUB_TYPE;

typedef enum {
			   EEPROM_MODEM_BOARD_NA,
			   EEPROM_MODEM_BOARD_EMULATION,
			   EEPROM_MODEM_BOARD_SQUBA,
               EEPROM_MODEM_BOARD_PDH
			  } EEPROM_MODEM_BOARD_SUB_TYPE;

typedef enum {
               EEPROM_MUX_FASTE1T1_FAST8E1,
			   EEPROM_MUX_FASTE1T1_FAST8T1,
			   EEPROM_MUX_FASTE1T1_FAST8T1_ENCRYPT
			 } EEPROM_MUX_FASTE1T1_FIRMWARE;



typedef struct{
		U8 HwConfigStructureVersion;
        U8 /*EEPROM_BOARD_TYPE*/ BoardType;
		U8 /*MUX_BOARD_SUB_TYPE*/ BoardSubType;
		U8 Firmware;
		U8 Hardware;
        U8 InterfaceType[EEPROM_MAX_NUM_OF_INTERFACES];
		U8 NumOfInterface[EEPROM_MAX_NUM_OF_INTERFACES];
		U8 Spare[5];
		U8 CRC_VAL;
        } EEPROM_HW_CONFIG_STRUCT,*EEPROM_HW_CONFIG_STRUCT_PTR;


typedef union
  {
    struct
     {
		short   NumOfAvaliableInterface[3];
#ifdef _FAL_TIT
		short   MrmcMaxSupportBitRate;
#else
		short   NotUsed;
#endif

     } Mux;
	struct
	{
		short   Rx_H_IQ_Delay;
		short   Tx_H_IQ_Delay;
		short   Rx_L_IQ_Delay;
		short   Tx_L_IQ_Delay;
	} Modem;
    } PRODUCTION_COMMON;

typedef struct{
		U8 ProductIdStructureVersion;
		char  	SerialNum[EEPROM_BOARD_SERIAL_NUM_SIZE];
		char	PartNum[EEPROM_BOARD_PART_NUM_SIZE];
		U32		NumOfWorkingHours;
		U32		DateCode;
		U8		HASS_Num;
		U32		Test;
		U32		Feature;
		U32		WasZeros; /*0x1234 if was zeros */
		PRODUCTION_COMMON ProductionCommon;

		U32		spare[2];
		char	Comment1[EEPROM_COMMENT_TEXT_SIZE];
		char	Comment2[EEPROM_COMMENT_TEXT_SIZE];
		char	Comment3[EEPROM_COMMENT_TEXT_SIZE];
        } EEPROM_PRODUCTION_BOARD_IDENTIFICATION_STRUCT, *EEPROM_PRODUCTION_BOARD_IDENTIFICATION_STRUCT_PTR;


typedef struct{
	U8 spare[50];
} EEPROM_GAP_STRUCT;


EEPROM_HW_CONFIG_STRUCT HwConfigBackUp;
typedef struct
{
	EEPROM_HW_CONFIG_STRUCT HwConfigStruct;
	EEPROM_GAP_STRUCT Gap;
	EEPROM_PRODUCTION_BOARD_IDENTIFICATION_STRUCT ProductionIdentificationStruct;
	EEPROM_HW_CONFIG_STRUCT BackupHwConfig;
} EEPROM_MEMORY_STRUCT;

STATUS EEPROM_GetHwConfigurationValues(CARRIER_ID CarrierId,BOARD_TYPE BoardType,EEPROM_HW_CONFIG_STRUCT_PTR HwConfigPtr);
STATUS EEPROM_SetHwConfigurationValues(CARRIER_ID CarrierId,BOARD_TYPE BoardType,EEPROM_HW_CONFIG_STRUCT_PTR HwConfigPtr);
STATUS EEPROM_GetBoardIdValues(CARRIER_ID CarrierId,BOARD_TYPE BoardType,EEPROM_PRODUCTION_BOARD_IDENTIFICATION_STRUCT_PTR ProductionBoardIdPtr);
STATUS EEPROM_SetBoardIdValues(CARRIER_ID CarrierId,BOARD_TYPE BoardType,EEPROM_PRODUCTION_BOARD_IDENTIFICATION_STRUCT_PTR ProductionBoardIdPtr);
STATUS EEPROM_SetTestRegister(CARRIER_ID CarrierId,BOARD_TYPE BoardType,U32 TestValue);
STATUS EEPROM_GetTestRegister(CARRIER_ID CarrierId,BOARD_TYPE BoardType,U32 * TestRegisterPtr);
STATUS EEPROM_SetComment3(CARRIER_ID CarrierId,BOARD_TYPE BoardType,char * Comment);
STATUS EEPROM_SetComment2(CARRIER_ID CarrierId,BOARD_TYPE BoardType,char * Comment);
STATUS EEPROM_SetComment1(CARRIER_ID CarrierId,BOARD_TYPE BoardType,char * Comment);
STATUS EEPROM_SetHASSnum(CARRIER_ID CarrierId,BOARD_TYPE BoardType,U8 HASS_num);
STATUS EEPROM_SetDateCode(CARRIER_ID CarrierId,BOARD_TYPE BoardType,U32 DateCode);
STATUS EEPROM_SetNumOfWorkingHours(CARRIER_ID CarrierId,BOARD_TYPE BoardType,U32 NumOfWorkingHours);
STATUS EEPROM_SetPartNum(CARRIER_ID CarrierId,BOARD_TYPE BoardType,char * PartNum);
STATUS EEPROM_SetSerialNum(CARRIER_ID CarrierId,BOARD_TYPE BoardType,char * SerialNum);
int EEPROM_FindSpiDevice(CARRIER_ID CarrierId,BOARD_TYPE BoardType);
STATUS EEPROM_SetFeature(CARRIER_ID CarrierId,BOARD_TYPE BoardType,U32 Feature);
STATUS EEPROM_ZerosContents (CARRIER_ID CarrierId,BOARD_TYPE BoardType);
STATUS EEPROM_IsWasZeros(CARRIER_ID CarrierId,BOARD_TYPE BoardType,BOOL *WasZeros);

#endif
