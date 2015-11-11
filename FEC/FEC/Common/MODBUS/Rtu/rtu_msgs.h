/******************************************************************************
*                     (C) LandaLabs 2012                                      *
*******************************************************************************
*
*  PROJECT     : MODBUS_LIB 
*
*  FILE        : rtu_msgs.h
*   
*  PATH        : C:\Landalabs\Spitfire\Embedded\Projects\Common\MODBUS\Rtu 
*
*  PURPOSE     : 
*
*  TOOLCHAIN:  : IAR Embedded Workbench for ARM. Version 6.30.1.3142
*
*  TARGET      :  
*
*
*  DATE        NAME              VER.    COMMENTS
* -----------------------------------------------------------------------------
*  2012.06     Igor Natachanny   1.0.0   Initial version
*
*******************************************************************************
      Copyright and Proprietary Statment.

 Igor Natachanny 
 Independent Embedded System Professional.
 igornatachanny@hotmail.com 054-7656857 
******************************************************************************/

#ifndef _RTU_MSGS_H_
#define _RTU_MSGS_H_

//#ifdef __cplusplus
// extern "C" {
//#endif

/*---------------------------------------------------------------------------*/
/*               M B   R T U   R E Q U E S T  M E S S A G E S                */
/*---------------------------------------------------------------------------*/
   
  typedef enum _modbus_function_code_ {
    COILS_READ              = 1,
    HOLD_REGS_READ          = 3,
    INPUT_REGS_READ         = 4,
    SINGLE_COIL_READ        = 5,
    SINGLE_REG_WRITE        = 6,
    NUM_OF_MB_FUNC_CODES   /* keep this last */
  }MB_FUNC_CODE_ET;

  
    
  typedef struct _coils_read_request_data_ {
      uint16_t start_addr;
      uint16_t coils_quant;      
  }COILS_READ_REQ_DATA_ST, *COILS_READ_REQ_DATA_PST;
  
  typedef struct _hold_register_read_request_data_ {
      uint16_t start_addr;
      uint16_t regs_quant;      
  }HOLD_REGS_READ_REQ_DATA_ST, *HOLD_REGS_READ_REQ_DATA_PST;

  typedef struct _input_register_read_request_data_ {
      uint16_t start_addr;
      uint16_t regs_quant;
  }INPUT_REGS_READ_REQ_DATA_ST, *INPUT_REGS_READ_REQ_DATA_PST;
  
  typedef struct _single_coil_read_request_data_ {
      uint16_t output_addr;
      uint16_t output_val;      
  }SINGLE_COIL_WRITE_REQ_DATA_ST, *SINGLE_COIL_WRITE_REQ_DATA_PST;
  
  typedef struct _single_reg_write_request_data_ {
      uint16_t reg_addr;
      uint16_t reg_val;      
  }SINGLE_REG_WRITE_REQ_DATA_ST, *SINGLE_REG_WRITE_REQ_DATA_PST;
    
  typedef union  _modbus_rtu_request_data_ {
      COILS_READ_REQ_DATA_ST          coils_read_req;
      HOLD_REGS_READ_REQ_DATA_ST      hold_regs_read_req;
      INPUT_REGS_READ_REQ_DATA_ST     input_regs_read_req;
      SINGLE_REG_WRITE_REQ_DATA_ST    single_reg_write_req;
      SINGLE_COIL_WRITE_REQ_DATA_ST   single_coil_write_req;
      uint8_t buf[4];
  }REQ_DATA_UT;
 
    typedef struct _modbus_rtu_adu_frame_ {
      uint8_t           slave_id;
      MB_FUNC_CODE_ET   func_code;
      REQ_DATA_UT       req_data;
      uint16_t          crc16_val;
//      uint8_t           crc16_low;
//      uint8_t           crc16_hi;
    }MB_RTU_REQ_ADU_ST, *MB_RTU_REQ_ADU_PST;
  
    typedef union  _modbus_rtu_request_adu_ {
      MB_RTU_REQ_ADU_ST  adu;
      uint8_t buf[8];
    }MB_RTU_REQ_ADU_UT, *MB_RTU_REQ_ADU_PUT; 
  
 /* data field union should added here */
    
/*---------------------------------------------------------------------------*/
/*                M B   R T U   R E S P O N C E  M E S S A G E S             */ 
/*---------------------------------------------------------------------------*/
  
  typedef enum _modbus_exception_code_ {   
    COMPLETE                   =    0, /* FUNCTION COMPLEET */
    ILG_FUNCT                  =    1, /* ILLEGAL FUNCTION */
    ILG_REG_ADDR               =    2, /* ILLEGAL DATA ADDRESS */
    ILG_REG_VAL                =    3, /* ILLEGAL DATA VALUE */
    SLAVE_DEV_FAIL             =    4, /* SLAVE DEVICE FAILURE */
    ACK                        =    5, /* ACKNOWLEDGE */
    SLAVE_DEV_BUSY             =    6, /* SLAVE DEVICE BUSY */
    MEM_PRTY_ERR               =    8, /* MEMORY PARITY ERROR */
    GTW_PATH_UNAVALB           = 0x0A, /* GATEWAY PATH UNAVAILABLE */
    GTW_TARGET_DEV_RESP_FAIL   = 0x0B, /* GATEWAY TARGET DEVICE FAILED TO RESPOND */
    MB_INVLD_SLAVE_ID          = 0xE0, /* The slave ID in the response does not match that of the request. */
    INVLD_FUNC                 = 0xE1, /* The function code in the response does not match that of the request. */
    RESP_TO                    = 0xE2,
    INVLD_CRC                  = 0xE3, /* CRC comparation fault */
    MEM_ALLOC_ERR,
    UNKNOWN_FUNC,
    UNKNOWN_ERR,
    NUM_OF_MB_EXCEPT_CODES   /* keep this last */      
  }MB_EXCEPT_CODE_ET;
  
  typedef struct _single_reg_wr_resp_data_ {
    uint16_t reg_addr;
    uint16_t reg_val;
  }SINGLE_REG_WRITE_RESP_DATA_ST;
  
  typedef struct _input_regs_read_resp_data_ {
    uint8_t byte_count;
    uint8_t val_buf[255];
  }INPUT_REGS_READ_RESP_DATA_ST;

  typedef struct _hold_regs_read_resp_data_ {
    uint8_t byte_count;
    uint8_t val_buf[255];
  }HOLD_REGS_READ_RESP_DATA_ST;  

  typedef union  _modbus_rtu_resp_data_ {
//      COILS_READ_RESP_DATA_ST          coils_read_resp;
      HOLD_REGS_READ_RESP_DATA_ST      hold_regs_read;
      INPUT_REGS_READ_RESP_DATA_ST     input_regs_read;
      SINGLE_REG_WRITE_RESP_DATA_ST    single_reg_write;
//     SINGLE_COIL_WRITE_RESP_DATA_ST   single_coil_write_resp;
//      uint8_t buf[252];
  }RESP_DATA_UT;

  typedef struct  _mb_rtu_resp_pdu_data_ {
    uint8_t        func_code;    /*MB_FUNC_CODE_ET*/
    RESP_DATA_UT   data;
  }MB_RTU_RESP_PDU_ST, MB_RTU_RESP_PDU_PST;
    
  typedef struct  _mb_rtu_exep_pdu_ {
    uint8_t             error_code;
    MB_EXCEPT_CODE_ET   exept_code;
  }MB_RTU_EXEP_PDU_ST, MB_RTU_EXEP_PDU_PST;
  
  typedef union _mb_rtu_resp_pdu_ {
    MB_RTU_RESP_PDU_ST   resp;
    MB_RTU_EXEP_PDU_ST   exep;
//    uint8_t buf[253];
  }MB_RTU_RESP_PDU_UT, *MB_RTU_RESP_PDU_PUT;
  
  typedef struct _modbus_rtu_resp_adu_ {
    uint8_t              dev_adr;
    MB_RTU_RESP_PDU_UT   pdu;
    uint16_t             crc16_val;    
  }MB_RTU_RESP_ADU_ST, *MB_RTU_RESP_ADU_PST;
  
  typedef union  _modbus_rtu_responce_adu_ {
    MB_RTU_RESP_ADU_ST  adu;
    uint8_t buf[256];
  }MB_RTU_RESP_ADU_UT, *MB_RTU_RESP_ADU_PUT;   


//#ifdef __cplusplus
//}
//#endif
#endif /* _RTU_MSGS_H_ */
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
