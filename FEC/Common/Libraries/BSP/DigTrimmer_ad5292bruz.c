/******************************************************************************
 *                     (C) LandaLabs 2012                                      *
 *******************************************************************************
 *
 *  PROJECT     : BSP_LIB
 *
 *  FILE        : ad5292bruz.c
 *
 *  PATH        : C:\Landalabs\Spitfire\Branches_Embedded\RigDryerFirstPrintFEC-Branch\Common\Libraries\BSP
 *
 *  PURPOSE     :
 *
 *  TOOLCHAIN:  : IAR ARM Embedded Workbench 6.400 Evaluation
 *
 *  TARGET      :
 *
 *
 *  DATE        NAME              VER.    COMMENTS
 * -----------------------------------------------------------------------------
 *  2012.12     Igor Natachanny   1.0.0   Initial version
 *
 *******************************************************************************
 Copyright and Proprietary Statment.

 Igor Natachanny 
 Independent Embedded System Professional.
 igornatachanny@hotmail.com 054-7656857 
 *******************************************************************************/
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/
/*============================================================================*/
/*                                * I M P O R T *                             */
/*============================================================================*/
/* --- GLOBAL_INCLUDE_FILES ------------------------------------------------- */
#ifdef STM32F4XX
#include <stm32f4xx.h>
#else
#include <stm32f2xx.h>
#endif
#include "DigTrimmer_ad5292bruz.h"
/* --- PACKAGE_INCLUDE_FILES ------------------------------------------------ */
/* --- GLOBAL_EXTERNALS ----------------------------------------------------- */
/* --- PACKAGE_EXTERNALS ---------------------------------------------------- */
/*============================================================================*/
/*                                * E X P O R T *                             */
/*============================================================================*/
/* --- PUBLIC_DATA ---------------------------------------------------------- */
/* --- PUBLIC_FUNCTIONS ----------------------------------------------------- */
/*============================================================================*/
/*                              * L O C A L S *                               */
/*============================================================================*/
/* --- PRIVATE_CONSTANTS ---------------------------------------------------- */
/* --- PRIVATE MACROS ------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS ---------------------------------------------------- */
uint16_t Send_Receive_HalfWord(uint16_t HalfWord);
u16 write_2_Trimmer(u8 trimmer_number, u16 command, u16 data); //trimmer_number TRIMMER1 or TRIMMER2,
u16 read_from_Trimmer(u8 trimmer_number, u16 command, u16 data);
void set_current(u8 trimmer_number, u16 current);
u16 get_current(u8 trimmer_number);  //trimmer_number TRIMMER1 or TRIMMER2 return carrent uA
void write_CONTROL(u8 trimmer_number, u16 control);  //trimmer_number TRIMMER1 or TRIMMER2,  control control Reg_vall
u16 read_CONTROL(u8 trimmer_number);
/* --- PRIVATE_DATA --------------------------------------------------------- */
/*============================================================================*/
/*                  * L O C A L   C O N F I G U R A T I O N *                 */
/*============================================================================*/

/*============================================================================*/
/*     * P U B L I C   F U N C T I O N S   I M P L E M E N T A T I O N *      */
/*============================================================================*/
//----------------------------------------------------------
u16 Get_AO_mA(u8 trimmer_number)
{  //trimmer_number TRIMMER1 or TRIMMER2 return carrent uA
    u16 vall;

    vall = read_from_Trimmer(trimmer_number, 0x0800, 0);
    vall = vall & 0x03ff;
    return ((u16) ((((u32) (1024 - vall)) * 20000) >> 10));

}/********************* u16 Get_AO_mA(u8 trimmer_number) **********************/
//---------------------------------

void Set_AO_mA(u8 trimmer_number, u16 current)
{ //trimmer_number TRIMMER1 or TRIMMER2, carrent in uA

    u16 data;

    data = (u16) (((float) ((((u32) (current)) << 10)) / 20000) + 0.5);
    if (data < 1024)
        data = 1024 - data;
    else
        data = 1;
    data--;

    write_2_Trimmer(trimmer_number, 0x0400, data);

}/********************* void Set_AO_mA(u8 trimmer_number) **********************/

void DigitalTrimmersInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    /*!< Enable the SPI clock */

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

    /*!< Enable GPIO clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF,
            ENABLE);

    /*!< SPI pins configuration *************************************************/


    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

    /*!< SPI SCK MISO MOSI pin configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /*!< Connect SPI3 pins to AF6 */
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3 );
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3 );
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3 );

    /*!< Configure   CS1 CS2 pin in output pushpull mode ********************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    //disable trimmers
    CS1_HIGH();
    CS2_HIGH();
    CS1_LOW();
    CS2_LOW();
    CS1_HIGH();
    CS2_HIGH();

    /*!< Configure   RDY1 RDY2 pin in input  mode ********************/

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    SPI_I2S_DeInit(SPI3);
    /*!< SPI configuration */
    SPI_StructInit(&SPI_InitStructure);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;   //SPI_CPHA_1Edge
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;

    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 15;
    SPI_Init(SPI3, &SPI_InitStructure);

    /*!< Enable the SPI  */
    SPI_Cmd(SPI3, ENABLE);

    write_CONTROL(TRIMMER1, 2);
    write_CONTROL(TRIMMER2, 2);
#if 0  
    /* disable trimmers */
    CS1_HIGH();
    CS2_HIGH();

    write_CONTROL(TRIMMER1,2);
    write_CONTROL(TRIMMER2,2);
#endif
}
/*============================================================================*/
/*     * P R I V A T E   F U N C T I O N S   I M P L E M E N T A T I O N *    */
/*============================================================================*/
uint16_t Send_Receive_HalfWord(uint16_t HalfWord)
{

    /*!< Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE ) == RESET)
        ;

    /*!< Send Half Word through the the SPI bus */
    SPI_I2S_SendData(SPI3, HalfWord);

    /*!< Wait to receive a Half Word */
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE ) == RESET)
        ;
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY ) == SET)
        ;
    /*!< Return the Half Word read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI3 );

}

//------------------------------------------------------------

u16 write_2_Trimmer(u8 trimmer_number, u16 command, u16 data)
{   //trimmer_number TRIMMER1 or TRIMMER2,
    u16 vall = command + data;
    if (trimmer_number == TRIMMER1)
    {
//        while (!RDY1)
//        {
//        };
    }
    else
    {
//        while (!RDY2)
//        {
//        };
    }
    __NOP();
    if (trimmer_number == TRIMMER1)
        CS1_LOW();
    else
        CS2_LOW();
    vall = Send_Receive_HalfWord(vall);
    if (trimmer_number == TRIMMER1)
        CS1_HIGH();
    else
        CS2_HIGH();
    return vall;
}
//----------------------------------------------------------
u16 read_from_Trimmer(u8 trimmer_number, u16 command, u16 data)
{

    write_2_Trimmer(trimmer_number, command, data);
    return (write_2_Trimmer(trimmer_number, 0, 0));
}
//---------------------------------------

void write_CONTROL(u8 trimmer_number, u16 control)
{  //trimmer_number TRIMMER1 or TRIMMER2,  control control Reg_vall

    write_2_Trimmer(trimmer_number, 0x1800, control);
}
//---------------------------------------------------
u16 read_CONTROL(u8 trimmer_number)
{  //trimmer_number TRIMMER1 or TRIMMER2 return control Reg_vall
    u16 vall = read_from_Trimmer(trimmer_number, 0x1C00, 0);
    return (vall & 0x000f);
}
/*============================================================================*/
/*                     (C) Copyright LandaLabs 2012                           */
/*=========================================================== end_of_file ====*/
/*----------------------------------------------------------------------------
 synop
 *----------------------------------------------------------------------------*/
