#ifndef _EepromDB_Def_h_
#define _EepromDB_Def_h_

#include "CfgDButil.h"

#ifdef __cplusplus
 extern "C" {
#endif

#include "CfgDBUtil.h"

#define _EEPROM_DB_START_ADDR  1024  //0x400  EEPROM INT CFG DB starting from this address
   
#define _EEPROM_DB_MAX_NUM_OF_INSTANSES  8    
#define _EEPROM_DB_MAX_NUM_OF_UNITS  4

//for INT type
#define _EEPROM_DB_INT_MAX_NUM_OF_IDS 64  //maxiumum number of IDs of type INT per UNIT
#define _EEPROM_DB_INT_MAX_NUM_OF_ID_INDX  16 //maximum number of Indexes per ID of type INT
   
#define _EEPROM_DB_INT_ID_SIZE   ( _EEPROM_DB_INT_MAX_NUM_OF_ID_INDX * sizeof(UINT32)) //128
#define _EEPROM_DB_INT_UNIT_SIZE  (_EEPROM_DB_INT_ID_SIZE *  _EEPROM_DB_INT_MAX_NUM_OF_IDS) //8192
#define _EEPROM_DB_INT_INSTANSE_SIZE (_EEPROM_DB_MAX_NUM_OF_UNITS *_EEPROM_DB_INT_UNIT_SIZE) //32768
#define _EEPROM_DB_INT_TOTAL_SIZE   (_EEPROM_DB_MAX_NUM_OF_INSTANSES *_EEPROM_DB_INT_INSTANSE_SIZE) //128KB

   //macro that find the starting address in the EEPROM for INT type ID
#define EEPROM_DB_INT_ID_ADDR(Instanse,Unit,Id,Id_indx)  (_EEPROM_DB_START_ADDR+(Instanse * _EEPROM_DB_INT_INSTANSE_SIZE) + \
  (Unit * _EEPROM_DB_INT_UNIT_SIZE) + (Id * _EEPROM_DB_INT_ID_SIZE) + (Id_indx * sizeof(UINT32)))

//for STR type   
#define _EEPROM_DB_START_STR_ADDR  (_EEPROM_DB_START_ADDR+_EEPROM_DB_INT_INSTANSE_SIZE)  //0x20400 : 0xEEPROM STR CFG DB starting from this address
#define _EEPROM_DB_STR_MAX_NUM_OF_IDS 8  //maxiumum number of IDs of type STR per UNIT
#define _EEPROM_DB_STR_MAX_NUM_OF_ID_INDX  1  //maximum number of Indexes per ID of type STR

#define _EEPROM_DB_STR_ID_SIZE   (_EEPROM_DB_STR_MAX_NUM_OF_ID_INDX * MAX_CFG_STR_SIZE) //256
#define _EEPROM_DB_STR_UNIT_SIZE  (_EEPROM_DB_STR_MAX_NUM_OF_IDS *_EEPROM_DB_STR_ID_SIZE ) //2048
#define _EEPROM_DB_STR_INSTANSE_SIZE (_EEPROM_DB_MAX_NUM_OF_UNITS  * _EEPROM_DB_STR_UNIT_SIZE ) //8192
#define _EEPROM_DB_STR_TOTAL_SIZE   (_EEPROM_DB_MAX_NUM_OF_INSTANSES *_EEPROM_DB_STR_INSTANSE_SIZE) //64KB   

   //macro that find the starting address in the EEPROM for STR type ID   
#define EEPROM_DB_STR_ID_ADDR(Instanse,Unit,Id,Id_indx)  (_EEPROM_DB_START_STR_ADDR+(Instanse * _EEPROM_DB_STR_INSTANSE_SIZE) + \
  (Unit * _EEPROM_DB_STR_UNIT_SIZE) + (Id * _EEPROM_DB_STR_ID_SIZE) + (Id_indx * MAX_CFG_STR_SIZE))   
   


   
   
#ifdef __cplusplus
}
#endif
#endif
