/******************************************************************************
*                     (C) LandaLabs 2013                                      *
*******************************************************************************
*
*  PROJECT     : FEC
*
*  FILE        : Calibration.cpp
*   
*  PATH        :  C:\Landalabs\Spitfire\Embedded\Projects\Common\DB\SRC
*
*  PURPOSE     :  Handle the calibration data for both temperatures and current analog inputs .
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. 
*
*  TARGET      :  
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2013.01     Yossi Sarusi   1.0.0   Initial version
*
*******************************************************************************/

#include "FreeRTOS.h"
#include "ll_typedef.h"
#include "Calibration.h"
#include <xmemory>
#include "eeprom_cnfg.h"
#include "cfg_db_eeprom.h"
#include <stm32f2xx.h>
#include "TemperatureCfgDB.h"
#include "OthersCfgDB.h"
#include "logger.h"

static __IO uint32_t CRCValue = 0;
Calibration * TemperatureCalibration; //temperature calibration
    
Calibration * Analog4_20maCalibration; //current analog input calibration


Calibration::Calibration(int NumberOfElements, int NV_StartAddress)
{
    m_NV_StartDataAddress=NV_StartAddress;
    m_NumberOfElements=NumberOfElements;
    m_NV_StartCRCAddress=m_NV_StartDataAddress+m_NumberOfElements*sizeof(int); //last element is the CRC
    
    /* Enable CRC clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
  
    int NumberOfElementsPlusCrcInBytes=(m_NumberOfElements+1)*sizeof(int);  //add one element for the CRC value
    CalibrationData=(int*)pvPortMalloc(NumberOfElementsPlusCrcInBytes);
    //read data from eeprom including the CRC value
    eeprom_rd_buf(m_NV_StartDataAddress, NumberOfElementsPlusCrcInBytes, (uint8_t*) CalibrationData);
    CRC_ResetDR();
    U32 CRCValue = CRC_CalcBlockCRC((uint32_t *)CalibrationData, m_NumberOfElements); //exclude the CRC from the calculation
    
    if (CRCValue!=CalibrationData[NumberOfElements])
    {  //error in CRC -- create a zero calibration data
      for (int i=0;i<NumberOfElements;i++)
        CalibrationData[i]=0;   // clear the calibration data
      m_CalibrationReady=false;
    }
    else
      m_CalibrationReady=true;    
   
}
	
Calibration::~Calibration()
{
  vPortFree(CalibrationData);
}

bool Calibration::Ready()
{
      return m_CalibrationReady;
}
    
int& Calibration::operator[] (const int nIndex)
{
    assert_param(nIndex >= 0 && nIndex < m_NumberOfElements);
    
    return CalibrationData[nIndex];
    
}


/**
  * @brief: Save calibration element in the eeprom and protect with CRC  
  *
  * @param:IN: None.
  *       OUT: None.
  *
  * @retval: None.
**/

int Calibration::SaveData()
{
  int NumberOfElementsPlusCrcInBytes=(m_NumberOfElements+1)*sizeof(int);  //add one element for the CRC value
  CRC_ResetDR();
  U32 CRCValue=CRC_CalcBlockCRC((uint32_t *)CalibrationData, m_NumberOfElements); //calculate CRC value
  CalibrationData[m_NumberOfElements]=CRCValue;
  eeprom_wr_buf(m_NV_StartDataAddress, NumberOfElementsPlusCrcInBytes,(uint8_t*) CalibrationData); //store the data and the CRC 
  
  return OK;
  
}


//Global routines

/**
  * @brief: Init the calibration
  *
  * @param:IN: None.
  *       OUT: None.
  *
  * @retval: None.
**/
bool CalibrationInit()
{
  bool RetValue=OK;
  
  //create temperature calibration object
  TemperatureCalibration= new Calibration(NUMBER_OF_BOARD_TEMPERATURE_ZONES, EEPROM_TEMPERATURE_CALIBRATION_ADDR);
  
  if (!TemperatureCalibration->Ready())
  {
    //M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,"ERROR- temperature calibration not ready");
            
    RetValue=ERROR;
  }
  
  //create current input calibration object 
  Analog4_20maCalibration=new Calibration(NUM_OF_ANALOG_OUT_MA, EEPROM_ANALOG_OUT_MA_CALIBRATION_ADDR); 
  if (!Analog4_20maCalibration->Ready())
  {
   // M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR,"ERROR- Analog input calibration not ready");
    RetValue=ERROR;
  }
  
  return RetValue;
}

/**
  * @brief: set new temperature calibration data
  *
  * @param:IN: Index element value.
  * @param:IN: data value. 
  *        OUT: None.
  *
  * @retval: None.
**/

void TemperatureCalibrationSetData(int Index,int Data)
{
  (*TemperatureCalibration)[Index]=Data;
  if (Index==NUMBER_OF_BOARD_TEMPERATURE_ZONES-1)
    TemperatureCalibration->SaveData();
}

/**
  * @brief: get temperature calibration data
  *
  * @param:IN: Index element value.
  *        OUT: none.
  *
  * @retval: data value.
**/

int TemperatureCalibrationData(int Index)
{
  return (*TemperatureCalibration)[Index];
}

/**
  * @brief: set new Analog ma calibration data
  *
  * @param:IN: Index element value.
  * @param:IN: data value. 
  *        OUT: None.
  *
  * @retval: None.
**/

void AnalogMaCalibrationSetData(int Index,int Data)
{
  (*Analog4_20maCalibration)[Index]=Data;
  if (Index==NUMBER_OF_ANALOG_INP_MA-1)
    Analog4_20maCalibration->SaveData();
}

/**
  * @brief: get analog ma calibration data
  *
  * @param:IN: Index element value.
  *        OUT: none.
  *
  * @retval: data value.
**/
int AnalogMaCalibrationData(int Index)
{
  return (*Analog4_20maCalibration)[Index];
}



        


