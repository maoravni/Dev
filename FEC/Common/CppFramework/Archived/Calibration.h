/*
 * Calibration.h
 *
 *  Created on: 8 Jan 2013
 *      Author: Yossis
 */

#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#include "ll_typedef.h"

#define EEPROM_TEMPERATURE_CALIBRATION_ADDR 0x3ff00   // 0x3ff00-0x3ff93 -> 12x4+96(spare)+4 (crc)= 148 bytes
#define EEPROM_ANALOG_OUT_MA_CALIBRATION_ADDR 0x3ff94   // 0x3ff94- 0x3ffc7 -> 3x4+16(spare)+4 (crc)= 52 bytes

#ifdef __cplusplus   
class Calibration
{
private:  
   int m_NV_StartDataAddress;
   int m_NV_StartCRCAddress;
   int m_NumberOfElements;
   int * CalibrationData;
   bool m_CalibrationReady;

public:
    
    Calibration(int NumberOfElements, int NV_StartAddress);
    
    virtual ~Calibration();
    
   bool Ready();
   int& operator[] (const int nIndex);
   Calibration operator=(const Calibration& other);
   int SaveData();
  // or  Calibration & operator=(const Calibration &rhs);
    
};    
#endif


#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
    bool CalibrationInit();
    void TemperatureCalibrationSetData(int Index,int Data);
    //get analog ma calibration data
    int TemperatureCalibrationData(int Index);
     
    void AnalogMaCalibrationSetData(int Index,int Data);
    //get analog ma calibration data
    int AnalogMaCalibrationData(int Index);



   

#ifdef __cplusplus
}
#endif // cplusplus

#endif /* CALIBRATION_H_ */
