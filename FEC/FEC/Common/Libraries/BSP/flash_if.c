/**
 ******************************************************************************
 * @file    flash_if.c
 * @author  MCD Application Team
 * @version V1.1.0
 * @date    07-October-2011
 * @brief   This file provides high level routines to manage internal Flash
 *          programming (erase and write).
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"
#include "board_setup.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

typedef void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t JumpAddress;

/**
 * @brief  Unlocks Flash for write access
 * @param  None
 * @retval None
 */
void FLASH_If_Init(void)
{
    FLASH_Unlock();
}

void FLASH_If_Deinit()
{
    FLASH_Lock();
}

/**
 * @brief  This function does an erase of all user flash area
 * @param  StartSector: start of user flash area
 * @retval 0: user flash area successfully erased
 *         1: error occurred
 */
int8_t FLASH_If_Erase(uint32_t StartSector)
{
    uint32_t FlashAddress;

    FlashAddress = StartSector;

    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
     be done by word */

    if (FlashAddress <= (uint32_t) USER_FLASH_LAST_PAGE_ADDRESS)
    {
//        FLASH_EraseSector(FLASH_Sector_3, VoltageRange_3); /* 16 Kbyte */
        FLASH_EraseSector(FLASH_Sector_4, VoltageRange_3); /* 64 Kbyte */
        FLASH_EraseSector(FLASH_Sector_5, VoltageRange_3); /* 128 Kbyte */
        FLASH_EraseSector(FLASH_Sector_6, VoltageRange_3); /* 128 Kbyte */
        FLASH_EraseSector(FLASH_Sector_7, VoltageRange_3); /* 128 Kbyte */
        FLASH_EraseSector(FLASH_Sector_8, VoltageRange_3); /* 128 Kbyte */
        FLASH_EraseSector(FLASH_Sector_9, VoltageRange_3); /* 128 Kbyte */
        FLASH_EraseSector(FLASH_Sector_10, VoltageRange_3); /* 128 Kbyte */
        FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3); /* 128 Kbyte */
    }
    else
    {
        return (1);
    }

    return (0);
}
/**
 * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
 * @note   After writing data buffer, the flash content is checked.
 * @param  FlashAddress: start address for writing data buffer
 * @param  Data: pointer on data buffer
 * @param  DataLength: length of data buffer (unit is 32-bit word)
 * @retval 0: Data successfully written to Flash memory
 *         1: Error occurred while writing data in Flash memory
 *         2: Written Data in flash memory is different from expected one
 */
uint32_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data, uint16_t DataLength)
{
    uint32_t i = 0;

    printf("%x\n", *FlashAddress);
    for (i = 0; (i < DataLength) && (*FlashAddress <= (FLASH_END_ADDRESS - 4)); i++)
    {
        /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
         be done by word */
        if (FLASH_ProgramWord(*FlashAddress, *(uint32_t*) (Data + i)) == FLASH_COMPLETE)
        {
            /* Check the written value */
            if (*(uint32_t*) *FlashAddress != *(uint32_t*) (Data + i))
            {
                /* Flash content doesn't match SRAM content */
                return (2);
            }
            /* Increment FLASH destination address */
            *FlashAddress += 4;
        }
        else
        {
            /* Error occurred while writing data in Flash memory */
            return (1);
        }
    }

    return (0);
}

int CheckIfApplicationPresent()
{
    __IO uint32_t *app_pres_key_addr = (__IO uint32_t *) (APPL_PRESENT_KEY_ADDR);
    uint32_t app_pres_key_val = *app_pres_key_addr;

    return (app_pres_key_val == APPL_PRESENT_KEY_VAL);
}

void MarkApplicationAsPresent()
{
    uint32_t app_pres_key_val = APPL_PRESENT_KEY_VAL;
    __IO uint32_t app_pres_key_addr = APPL_PRESENT_KEY_ADDR;
    if (FLASH_If_Write(&app_pres_key_addr, &app_pres_key_val, 1) == 2)
    {
        MarkApplicationAsDeleted();
        FLASH_If_Write(&app_pres_key_addr, &app_pres_key_val, 1);
    }
}

void MarkApplicationAsDeleted()
{
    FLASH_EraseSector(FLASH_Sector_4, VoltageRange_3); /* 16 Kbyte */
}

void JumpToUserApplication()
{
    /* Jump to user application */
    JumpAddress = *(__IO uint32_t*) (USER_FLASH_FIRST_PAGE_ADDRESS + 4);
    Jump_To_Application = (pFunction) JumpAddress;
    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t*) USER_FLASH_FIRST_PAGE_ADDRESS);
    Jump_To_Application();
}

void EraseMainProgramArea()
{
    FLASH_EraseSector(FLASH_Sector_5, VoltageRange_3); /* 128 Kbyte */
    FLASH_EraseSector(FLASH_Sector_6, VoltageRange_3); /* 128 Kbyte */
    FLASH_EraseSector(FLASH_Sector_7, VoltageRange_3); /* 128 Kbyte */
}

void EraseBackupProgramArea()
{
    FLASH_EraseSector(FLASH_Sector_8, VoltageRange_3); /* 128 Kbyte */
    FLASH_EraseSector(FLASH_Sector_9, VoltageRange_3); /* 128 Kbyte */
    FLASH_EraseSector(FLASH_Sector_10, VoltageRange_3); /* 128 Kbyte */
    FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3); /* 128 Kbyte */
}

int VerifyApplicationCrc(uint32_t startAddress, uint32_t length)
{
    __IO uint32_t crcAddress = (startAddress + length - 2);
    uint16_t origCrc = *(uint16_t*) crcAddress;
    uint16_t sum = 0;
    sum = crc_ccitt(sum, startAddress, length - 2);

    return (sum == origCrc);
}

int CopyApplicationFromBackupToMain(__IO uint32_t *destAddress, __IO uint32_t *srcAddress, uint32_t length)
{
    int i;
    uint32_t data;

    for (i = 0; i < length; ++i)
    {
        data = *(uint32_t*)*srcAddress;
        if (FLASH_ProgramWord(*destAddress, data) == FLASH_COMPLETE)
        {
            /* Check the written value */
            if (*(uint32_t*) *destAddress != data)
            {
                /* Flash content doesn't match SRAM content */
                return (2);
            }
            /* Increment FLASH destination address */
            *destAddress += 4;
            *srcAddress += 4;
        }
        else
        {
            /* Error occurred while writing data in Flash memory */
            return (1);
        }
    }

    return 0;
}

int VerifyApplicationAndCopy()
{
    uint32_t srcAddress;
    uint32_t destAddress;
    if (VerifyApplicationCrc(BACKUP_FLASH_FIRST_PAGE_ADDRESS,
    BACKUP_FLASH_LAST_PAGE_ADDRESS - BACKUP_FLASH_FIRST_PAGE_ADDRESS))
    {
        FLASH_If_Init();
        MarkApplicationAsDeleted();
        EraseMainProgramArea();
        srcAddress = USER_FLASH_FIRST_PAGE_ADDRESS;
        destAddress = BACKUP_FLASH_FIRST_PAGE_ADDRESS;
        CopyApplicationFromBackupToMain(&srcAddress, &destAddress,
        (BACKUP_FLASH_LAST_PAGE_ADDRESS - BACKUP_FLASH_FIRST_PAGE_ADDRESS)>>2);
//        FLASH_If_Write(&srcAddress, &destAddress, (BACKUP_FLASH_LAST_PAGE_ADDRESS-BACKUP_FLASH_FIRST_PAGE_ADDRESS)>>2);
        if (VerifyApplicationCrc(USER_FLASH_FIRST_PAGE_ADDRESS,
        USER_FLASH_LAST_PAGE_ADDRESS - USER_FLASH_FIRST_PAGE_ADDRESS))
        {
            MarkApplicationAsPresent();
        }
        resetBoard(0);
        return 0;
    }
    return 1;
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
