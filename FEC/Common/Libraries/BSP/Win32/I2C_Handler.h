/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_Handler_H
#define __I2C_Handler_H

#if defined WIN32
#include "Win32MissingDefines.h"
#else
#include "stm32f4xx_it.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************************/
/*****************************************************************************************/

/* Uncomment the line below if you will use the I2C peripheral in 10-bit addressing
   mode */
//#define I2C_10BITS_ADDRESS
  
/* Define I2C Speed --------------------------------------------------------*/
#define I2C_SPEED 10000
#define I2C_DUTYCYCLE  I2C_DutyCycle_2
  
/* USER_TIMEOUT value for waiting loops. This timeout is just a guarantee that the
   application will not remain stuck if the I2C communication is corrupted. 
   You may modify this timeout value depending on CPU frequency and application
   conditions (interrupts routines, number of data to transfer, speed, CPU
   frequency...). */ 
  
/* I2Cx Communication boards Interface */
#define I2C1_DMA                      DMA1
#define I2C1_DMA_CHANNEL              DMA_Channel_1
#define I2C1_DR_ADDRESS               ((uint32_t)0x40005410)
#define I2C1_DMA_STREAM_TX            DMA1_Stream6
#define I2C1_DMA_STREAM_RX            DMA1_Stream5
#define I2C1_TX_DMA_TCFLAG            DMA_FLAG_TCIF6
#define I2C1_TX_DMA_FEIFLAG           DMA_FLAG_FEIF6
#define I2C1_TX_DMA_DMEIFLAG          DMA_FLAG_DMEIF6
#define I2C1_TX_DMA_TEIFLAG           DMA_FLAG_TEIF6
#define I2C1_TX_DMA_HTIFLAG           DMA_FLAG_HTIF6
#define I2C1_RX_DMA_TCFLAG            DMA_FLAG_TCIF5
#define I2C1_RX_DMA_FEIFLAG           DMA_FLAG_FEIF5
#define I2C1_RX_DMA_DMEIFLAG          DMA_FLAG_DMEIF5
#define I2C1_RX_DMA_TEIFLAG           DMA_FLAG_TEIF5
#define I2C1_RX_DMA_HTIFLAG           DMA_FLAG_HTIF5
#define DMA1_CLK                      RCC_AHB1Periph_DMA1    
#define NUM_OF_RX_BYTES               6  

/*****************************************************************************************/
/* I2C Comm Errors */
/*****************************************************************************************/

#define I2C_ERR_BERR	0x01
#define I2C_ERR_ARLO	0x02
#define I2C_ERR_AF		0x04
#define I2C_ERR_TIMEOUT	0x08
// Checksum calculation Error.
#define I2C_ERR_CHECKSUM_ER	0x10
/*****************************************************************************************/

/*********************************/
/* Raytek MI3 sensor definitions */
/*********************************/
#define AMB_BCKGRND_COMPENSATE_BIT      0x80
#define TEMP_UNITS_BIT                  0x01
#define FACTORY_DEFAULT                 0x55555555
#define LASER_ON_BIT                    0x01
#define LASER_FLASH_BIT                 0x02

/******************************************************/
/* I2C Events interrupts control state machine states */
/******************************************************/
#define CONNECT_2_SENSOR     0
#define READ_SENSOR          1

/*****************************/
/* External global veriables */
/*****************************/
/* MI3 Sensor ICD commands */
typedef enum
{
  e_Name                  = 0x01, //R/W
  e_Head_ID               = 0x02, //R
  e_Serial_Num            = 0x03, //R
  e_Version               = 0x04, //R
  e_Targ_Temp             = 0x05, //R
  e_Amb_Temp              = 0x06, //R
  e_Targ_Amb_Temp         = 0x07, //R
  e_Emissivity            = 0x08, //R
  e_Transmissivitiy       = 0x09, //R
  e_Bottom_Temp           = 0x0A, //R/W
  e_Top_Temp              = 0x0B, //R/W
  e_Status                = 0x0C, //R/W[0,7]
  e_Head_Address          = 0x0D, //R/W
  e_Special_Code          = 0x11, //R/W
  e_Head_IDENT0           = 0x12, //R
  e_Head_IDENT1           = 0x13, //R
  e_Head_IDENT2           = 0x14, //R
  e_Head_IDENT3           = 0x15, //R
  e_Adj_Gain              = 0x1A, //R/W
  e_Adj_Offset            = 0x1B, //R/W
  e_Detector_Power        = 0x1C, //R
  e_Amb_background        = 0x1D, //R/W
  e_Factory_Default       = 0x1F, //W
  e_Flicker_Filter        = 0x28, //R/W
  e_Laser_Control         = 0x3E, //R/W
  e_Cali_Date             = 0x40, //R
  e_Cali_Time             = 0x41, //R
  e_Cali_Amb_Low_Temp     = 0x42, //R
  e_Cali_Amb_High_Temp    = 0x43, //R
  e_Cali_Source_Temp1     = 0x44, //R
  e_Cali_Measure_Temp1    = 0x45, //R
  e_Cali_Source_Temp2     = 0x46, //R
  e_Cali_Measure_Temp2    = 0x47, //R
  e_Cali_Source_Temp3     = 0x48, //R
  e_Cali_Measure_Temp3    = 0x49, //R
  e_Cali_Source_Temp4     = 0x4A, //R
  e_Cali_Measure_Temp4    = 0x4B, //R
  e_Cali_Source_Temp5     = 0x4C, //R
  e_Cali_Measure_Temp5    = 0x4D  //R  
}MI3_Registers;

/* MI3 Sensor ICD data format */
typedef union
{
  char          c_Name[4];              //R/W
  uint32_t           u32_Head_ID;            //R
  uint32_t           u32_Serial_Num;         //R
  char          c_Version[4];           //R
  float         f_Targ_Temp;            //R
  float         f_Amb_Temp;             //R
  int16_t       i16_Targ_Amb_Temp[2];   //R
  float         f_Emissivity;           //R
  float         f_Transmissivitiy;      //R
  float         f_Bottom_Temp;          //R/W
  float         f_Top_Temp;             //R/W
  uint32_t           u32_Status_u8;          //R/W[1,7]
  uint32_t           u32_Head_Address_u8;    //R/W
  char          c_Special_Code;         //R/W
  char          c_Head_IDENT0;          //R
  char          c_Head_IDENT1;          //R
  char          c_Head_IDENT2;          //R
  char          c_Head_IDENT3;          //R
  float         f_Adj_Gain;             //R/W
  float         f_Adj_Offset;           //R/W
  int32_t       i32_Detector_Power;     //R
  float         f_Amb_background;       //R/W
  uint32_t           u32_Factory_Default;    // W
  uint32_t           u32_Flicker_Filter_u16; //R/W
  uint32_t           u32_Laser_Control_u8;   //R/W
  uint32_t           u32_Cali_Date;          //R
  uint32_t           u32_Cali_Time;          //R
  float         f_Cali_Amb_Low_Temp;    //R
  float         f_Cali_Amb_High_Temp;   //R
  float         f_Cali_Source_Temp1;    //R
  float         f_Cali_Measure_Temp1;   //R
  float         f_Cali_Source_Temp2;    //R
  float         f_Cali_Measure_Temp2;   //R
  float         f_Cali_Source_Temp3;    //R
  float         f_Cali_Measure_Temp3;   //R
  float         f_Cali_Source_Temp4;    //R
  float         f_Cali_Measure_Temp4;   //R
  float         f_Cali_Source_Temp5;    //R
  float         f_Cali_Measure_Temp5;   //R 
}U_MI3Data;

/* Data receiving structure */
typedef struct
{
  U_MI3Data     Data;
  uint8_t       SlaveRegByte;
  uint8_t       Checksum;
}T_RxPacketBytes;

/* Data transmitting structure */
typedef struct
{
  uint8_t       SlaveRegByte;  
  U_MI3Data     Data;
}T_TxPacketBytes;

/* Data receiving union */
typedef union
{
  T_RxPacketBytes   RxPacketBytes;
  uint8_t           RxPacket[6];
}U_MasterRxMsg;

/* Data transmitting union */
typedef union
{
  T_TxPacketBytes   TxPacketBytes;
  uint8_t           TxPacket[5];
}U_MasterTxMsg;

extern uint8_t g_I2C1_Rx_ready_flag;
extern uint8_t g_I2C1_ErrorReg;
extern U_MasterRxMsg g_MasterRxMsg;
extern U_MasterTxMsg g_MasterTxMsg;

/*******************************************************************************
* Function Name: void SPI_Config(void)
********************************************************************************
* Summary:
*
* Parameters:
*
* Return:
*   none
*******************************************************************************/
void I2C1_DMA_IRQ_init(void);


/*******************************************************************************
* Function Name: void Read_I2C1_EV_IRQMngr(void);
********************************************************************************
* Summary:
*
* Parameters:
*
* Return:
*   none
*******************************************************************************/
void Read_I2C1_EV_IRQMngr(void);

/*******************************************************************************
* Function Name: void Write_I2C1_EV_IRQMngr(void);
********************************************************************************
* Summary:
*
* Parameters:
*
* Return:
*   none
*******************************************************************************/
void Write_I2C1_EV_IRQMngr(void);

/*******************************************************************************
* Function Name: void Errors_I2C1_EV_IRQMngr(void);
********************************************************************************
* Summary:
*
* Parameters:
*
* Return:
*   none
*******************************************************************************/
void Errors_I2C1_EV_IRQMngr(void);

/*******************************************************************************
* Function Name: void MI3_Temperature_Sensors_Handler(void);
********************************************************************************
* Summary:
*
* Parameters:
*
* Return:
*   none
*******************************************************************************/
void MI3_Temperature_Sensors_Handler(void);

/*******************************************************************************
* Function Name: uint8_t MI3_Write(uint8_t SensAddrs, uint8_t *pTxData);
********************************************************************************
* Summary:
*
* Parameters:
*
* Return:
*   none
*******************************************************************************/
void MI3_Write(uint8_t SensAddrs, uint8_t SensReg, uint8_t *pTxData);

/*******************************************************************************
* Function Name: uint8_t MI3_Read(uint8_t SensAddrs, uint8_t SensReg, u8 * pRxData );
********************************************************************************
* Summary:
*
* Parameters:
*
* Return:
*   none
*******************************************************************************/
uint8_t MI3_Read(uint8_t SensAddrs, uint8_t SensReg, uint8_t * pRxData);


/*******************************************************************************
* Function Name: void PParse_Command(uint8_t *pTxData);
********************************************************************************
* Summary:
*
* Parameters:
*
* Return:
*   none
*******************************************************************************/
void Parse_Command(uint8_t *pTxData);

/*******************************************************************************
* Function Name: void Checksum_Calc(U_MasterRxMsg * pMasterRxMsg);
********************************************************************************
* Summary:
*
* Parameters:
*
* Return:
*   none
*******************************************************************************/
void Checksum_Calc(U_MasterRxMsg * pMasterRxMsg);


#ifdef __cplusplus
}
#endif

#endif
