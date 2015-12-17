/**
  ******************************************************************************
  * @file    flash_if.h 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    07-October-2011
  * @brief   Header for flash_if.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_IF_H
#define __FLASH_IF_H

#ifdef __cplusplus
extern "C"
{
#endif

#if defined STM32F4XX
#include <stm32f4xx.h>
#elif defined WIN32
#include <Win32MissingDefines.h>
#define __IO
#else
#include <stm32f2xx.h>
#endif

#define USER_FLASH_FIRST_PAGE_ADDRESS 0x08020000 /* Only as example see comment */
#define USER_FLASH_LAST_PAGE_ADDRESS  0x08080000
#define BACKUP_FLASH_FIRST_PAGE_ADDRESS 0x08080000
#define BACKUP_FLASH_LAST_PAGE_ADDRESS 0x080E0000
#define FLASH_END_ADDRESS        0x080FFFFF

#define USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - USER_FLASH_FIRST_PAGE_ADDRESS)

#define APPL_PRESENT_KEY_ADDR    (USER_FLASH_FIRST_PAGE_ADDRESS - 4)
#define APPL_PRESENT_KEY_VAL     0xA5A5A5A5
#define APPL_VERIFIED_KEY_ADDR    (USER_FLASH_FIRST_PAGE_ADDRESS - 8)
#define APPL_VERIFIED_KEY_VAL     0xA5A5A5A5

#define BOOTLOADER_FLASH_FIRST_PAGE_ADDRESS 0x08000000

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint32_t FLASH_If_Write(__IO uint32_t* Address, uint32_t* Data, uint16_t DataLength);
int8_t FLASH_If_Erase(uint32_t StartSector);
void FLASH_If_Init(void);
void FLASH_If_Deinit();

void EraseBackupProgramArea();
void EraseMainProgramArea();
int CheckIfApplicationPresent();
void MarkApplicationAsDeleted();
void MarkApplicationAsPresent();
void JumpToUserApplication();
int VerifyApplicationCrc(uint32_t startAddress, uint32_t length);
int CopyApplicationFromBackupToMain(__IO uint32_t *destAddress, __IO uint32_t *srcAddress, uint32_t length);
int VerifyApplicationAndCopy();

#ifdef __cplusplus
}
#endif

#endif /* __FLASH_IF_H */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
