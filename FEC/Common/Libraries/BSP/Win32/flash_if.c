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
}

void FLASH_If_Deinit()
{
}

/**
 * @brief  This function does an erase of all user flash area
 * @param  StartSector: start of user flash area
 * @retval 0: user flash area successfully erased
 *         1: error occurred
 */
int8_t FLASH_If_Erase(uint32_t StartSector)
{
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
    return (0);
}

int CheckIfApplicationPresent()
{
    return 1;
}

void MarkApplicationAsPresent()
{
}

void MarkApplicationAsDeleted()
{
}

void JumpToUserApplication()
{
}

void EraseMainProgramArea()
{
}

void EraseBackupProgramArea()
{
}

int VerifyApplicationCrc(uint32_t startAddress, uint32_t length)
{
}

int CopyApplicationFromBackupToMain(__IO uint32_t *destAddress, __IO uint32_t *srcAddress, uint32_t length)
{
    return 0;
}

int VerifyApplicationAndCopy()
{
    return 0;
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
