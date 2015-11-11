/*
 * AppLoader.cpp
 *
 *  Created on: 11 Feb 2015
 *      Author: maora
 */

#include <AppLoader.h>
#include <flash_if.h>
#include <crc_ccitt.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <cybootloaderutils/cybtldr_parse.h>
#include <cybootloaderutils/cybtldr_command.h>
#include <cybootloaderutils/cybtldr_api.h>
#include <cybootloaderutils/cybtldr_api2.h>
#include <string.h>

AppLoaderBase::AppLoaderBase()
{
    m_appTarget = 0;
    m_lastSerialNumber = 0;
    m_lastMesageId = 0;
    m_lastPacketNumber = 0xffffffff;
    m_numOfPackets = 0;
}

AppLoaderBase::~AppLoaderBase()
{
    FLASH_If_Deinit();
}

StAppLoader::StAppLoader()
{
    m_numOfPackets = 0;
    m_lastPacketNumber = 0xffffffff;
}

void StAppLoader::startAppLoad(int numOfPackets, uint16_t target, uint32_t msgId, uint32_t sn)
{
    m_lastPacketNumber = 0xffffffff;
    FLASH_If_Init();
    EraseBackupProgramArea();
    m_numOfPackets = numOfPackets;
    m_lastFlashAddress = BACKUP_FLASH_FIRST_PAGE_ADDRESS;
    setAppTarget(target);
    m_lastSerialNumber = sn;
    m_lastMesageId = msgId;
}

E_PacketResult StAppLoader::nextPacket(uint32_t packetNumber, uint8_t* data, uint16_t packetLength, uint16_t crc)
{
    uint16_t tempCrc = 0;
    tempCrc = crc_ccitt(tempCrc, data, packetLength);
    int result;

    if (tempCrc == crc)
    {
        taskENTER_CRITICAL();
        result = FLASH_If_Write(&m_lastFlashAddress, (uint32_t*) data, packetLength >> 2);
        taskEXIT_CRITICAL();
        if (result != 0)
        {
            printf("Flash Write Failed: %x %d\n", m_lastFlashAddress, packetLength);
            if (result == 1)
                return E_PacketResult_FlashWriteFailed;
            else
                return E_PacketResult_FlashWriteCompareFailed;
        }
        else
            return E_PacketResult_Ok;
    }
    else
        return E_PacketResult_CrcError;
}

bool StAppLoader::endAppLoad()
{
    if (getAppTarget() == 0)
    {
        // verify the CRC:
        if (VerifyApplicationCrc(BACKUP_FLASH_FIRST_PAGE_ADDRESS,
        BACKUP_FLASH_LAST_PAGE_ADDRESS - BACKUP_FLASH_FIRST_PAGE_ADDRESS))
        {
            MarkApplicationAsDeleted();
            // copy application from backup to main
//            CopyApplicationFromBackupToMain((uint32_t*)USER_FLASH_FIRST_PAGE_ADDRESS, (uint32_t*)BACKUP_FLASH_FIRST_PAGE_ADDRESS, BACKUP_FLASH_LAST_PAGE_ADDRESS-BACKUP_FLASH_LAST_PAGE_ADDRESS);
//            // verify CRC again:
//            if (VerifyApplicationCrc(USER_FLASH_FIRST_PAGE_ADDRESS, USER_FLASH_LAST_PAGE_ADDRESS-USER_FLASH_FIRST_PAGE_ADDRESS))
//            {
//                // mark application as present:
//                MarkApplicationAsPresent();
//            }
            return true;
        }
        return false;
    }
    else
    {
        return true;
    }
}

PsocAppLoader::PsocAppLoader(PsocManager* psocManager) :
        m_pPsocManager(psocManager), m_chksumtype(0), m_siliconId(0), m_siliconRev(0), m_isFirstLine(1), m_reachedEol(
                0), m_currentBufferIndex(0)
{
}

void PsocAppLoader::startAppLoad(int numOfPackets, uint16_t target, uint32_t msgId, uint32_t sn)
{
    m_numOfPackets = numOfPackets;
    setAppTarget(target);
    m_lastSerialNumber = sn;
    m_lastMesageId = msgId;
    memset(m_packetBuffer, 0, sizeof(m_packetBuffer));
    m_currentBufferIndex = 0;
    m_lastPacketNumber = 0xffffffff;
    m_isFirstLine = true;
    m_siliconId = 0;
    m_siliconRev = 0;
    m_chksumtype = SUM_CHECKSUM;
    m_lastFlashAddress = BACKUP_FLASH_FIRST_PAGE_ADDRESS;
    m_reachedEol = false;
    FLASH_If_Init();
    EraseBackupProgramArea();
}

E_PacketResult PsocAppLoader::nextPacket(uint32_t packetNumber, uint8_t* data, uint16_t packetLength, uint16_t crc)
{
    int i;
    int lastError;

    for (i = 0; i < packetLength; ++i)
    {
        if (!m_reachedEol)
        {
            if (isEol(data[i]))
                m_reachedEol = true;
            else
                m_packetBuffer[m_currentBufferIndex++] = data[i];
        }
        else
        {
            if (!isEol(data[i]))
            {
                m_reachedEol = false;
                if (m_isFirstLine)
                {
                    lastError = CyBtldr_ParseHeader(m_currentBufferIndex, m_packetBuffer, &m_siliconId, &m_siliconRev,
                            &m_chksumtype);
                    if (lastError != CYRET_SUCCESS)
                    {
                        return E_PacketResult_CrcError;
                    }
                    m_isFirstLine = false;
                }
                else
                {
                    lastError = writeRowDataToFlash(false);
                    if (lastError != CYRET_SUCCESS)
                        return E_PacketResult_CrcError;
                }
                // reset the current index and continue:
                m_currentBufferIndex = 0;
                // copy the current char, which is the start of the new line, to the buffer:
                m_packetBuffer[m_currentBufferIndex++] = data[i];
            }
        }
        if (m_currentBufferIndex >= MAX_BUFFER_SIZE)
            return E_PacketResult_CrcError;
    }
    return E_PacketResult_Ok;
}

bool PsocAppLoader::endAppLoad()
{
    // try write the buffer remainder to flash:
    if (m_currentBufferIndex != 0)
    {
        if (writeRowDataToFlash(true) != CYRET_SUCCESS)
            return false;
    }

    if (writeRowDataFromFlashToPSoC() == CYRET_SUCCESS)
        return true;
    else
        return false;
}

int PsocAppLoader::writeRowDataToFlash(bool isLast)
{
    int lastError;
    FlashRowHeader flashRowHeader;

    static unsigned char buffer[MAX_BUFFER_SIZE];

    lastError = CyBtldr_ParseRowData(m_currentBufferIndex, m_packetBuffer, &flashRowHeader.arrayId,
            &flashRowHeader.rowNum, buffer, &flashRowHeader.bufSize, &flashRowHeader.checksum);

    if (lastError != CYRET_SUCCESS)
        return lastError;

    flashRowHeader.isLast = isLast;
    // write the header to the temporary flash area:
    taskENTER_CRITICAL();
    lastError = FLASH_If_Write(&m_lastFlashAddress, (uint32_t*) &flashRowHeader, sizeof(FlashRowHeader) >> 2);
    taskEXIT_CRITICAL();

//    tempFlashRowHeader =(FlashRowHeader*)currentFlashAddress;
//    if (tempFlashRowHeader->arrayId != flashRowHeader.arrayId ||
//            tempFlashRowHeader->bufSize != flashRowHeader.bufSize ||
//            tempFlashRowHeader->checksum != flashRowHeader.checksum ||
//            tempFlashRowHeader->isLast != flashRowHeader.isLast ||
//            tempFlashRowHeader->rowNum != flashRowHeader.rowNum)
//    {
//        lastError = CYRET_ERR_DATA;
//        return lastError;
//    }
//
    if (lastError != CYRET_SUCCESS)
        return CYRET_ERR_ROW;

    // write the buffer to the temporary flash area:
    taskENTER_CRITICAL();
    if ((flashRowHeader.bufSize % 4) == 0)
        lastError = FLASH_If_Write(&m_lastFlashAddress, (uint32_t*) &buffer, (flashRowHeader.bufSize >> 2));
    else
        lastError = FLASH_If_Write(&m_lastFlashAddress, (uint32_t*) &buffer, (flashRowHeader.bufSize >> 2) + 1);
    taskEXIT_CRITICAL();

//    for (int i = 0; i < flashRowHeader.bufSize; ++i)
//    {
//        char c = *(unsigned char*)currentFlashAddress++;
//        if (buffer[i] != c)
//        {
//            lastError = CYRET_ERR_DATA;
//            return lastError;
//        }
//    }

    if (lastError != CYRET_SUCCESS)
        return CYRET_ERR_ROW;

    return CYRET_SUCCESS;
}

int PsocAppLoader::writeRowDataFromFlashToPSoC()
{
    int psocIndex;
    int rowCount;
    const unsigned long BL_VER_SUPPORT_VERIFY = 0x010214; /* Support for full flash verify added in v2.20 of cy_boot */
    FlashRowHeader* pFlashRowHeader;

    // todo: change from INT to ENUM. This should improve the readability of the code.
    static int targetMask[M_MAX_NUMBER_OF_PSOC];
    int appTarget = getAppTarget();

    unsigned long blVer = 0;
    unsigned char g_abort = 0;
    unsigned char checksum2 = 0;
    static unsigned char buffer[MAX_BUFFER_SIZE];
    int err;

    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "start bootloader");

    E_PsocSpiError result = m_pPsocManager->startBootloader();
    if (result != E_PsocSpiError_Ok)
    {
        M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "start bootloader failed");
        return CYRET_ERR_BTLDR;
    }

    // prepare the mask array:
    for (psocIndex = 0; psocIndex < M_MAX_NUMBER_OF_PSOC; ++psocIndex)
    {
        targetMask[psocIndex] = (appTarget & 1);
        appTarget >>= 1;
    }

    // go over all psocs in the mask and put them in bootloader:
    for (psocIndex = 0; psocIndex < M_MAX_NUMBER_OF_PSOC; ++psocIndex)
    {
        if (targetMask[psocIndex] == 1)
        {
            result = m_pPsocManager->startPsocBootloader(psocIndex);
        }
    }

    M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "bootloader setup");
    // setup the callbacks for the app transfer
    CyBtldr_CommunicationsData* comm = PsocBootloader_Setup();

    vTaskDelay(1000);

    for (psocIndex = 0; psocIndex < M_MAX_NUMBER_OF_PSOC; ++psocIndex)
    {
        m_pPsocManager->setPsocCsLow(psocIndex);
        m_lastFlashAddress = BACKUP_FLASH_FIRST_PAGE_ADDRESS;
        if (targetMask[psocIndex] == 1)
        {
            CyBtldr_SetCheckSumType((CyBtldr_ChecksumType) m_chksumtype);
            M_LOGGER_LOGF(M_LOGGER_LEVEL_TRACE, "CyBtldr_StartBootloadOperation");
            vTaskDelay(1000);
            err = CyBtldr_StartBootloadOperation(comm, m_siliconId, m_siliconRev, &blVer);
            if (err != 0)
                M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "CyBtldr_StartBootloadOperation err=%d", err);
            if (CYRET_SUCCESS != err)
            {
                targetMask[psocIndex] = 2;
            }

            rowCount = 0;
            do
            {
                if (g_abort)
                {
                    err = CYRET_ABORT;
                    break;
                }

                pFlashRowHeader = (FlashRowHeader*) m_lastFlashAddress;
                m_lastFlashAddress += sizeof(FlashRowHeader);
                if ((pFlashRowHeader->bufSize % 4) == 0)
                {
                    memcpy(buffer, (unsigned char*) m_lastFlashAddress, pFlashRowHeader->bufSize);
                    m_lastFlashAddress += pFlashRowHeader->bufSize;
                }
                else
                {
                    memcpy(buffer, (unsigned char*) m_lastFlashAddress, ((pFlashRowHeader->bufSize >> 2) + 1) << 2);
                    m_lastFlashAddress += ((pFlashRowHeader->bufSize >> 2) + 1) << 2;
                }

                if (CYRET_SUCCESS == err)
                {
                    printf("CyBtldr_ProgramRow %d %x(%x) %d ", pFlashRowHeader->arrayId, pFlashRowHeader->rowNum,
                            rowCount++, pFlashRowHeader->bufSize);

                    for (int i = 0; i < 16; ++i)
                    {
                        printf("%02x ", buffer[i]);
                    }
                    puts("");
                    // go over all psocs in the mask and program the current row:
                    if (targetMask[psocIndex] == 1)
                    {
                        err = CyBtldr_ProgramRow(pFlashRowHeader->arrayId, pFlashRowHeader->rowNum, buffer,
                                pFlashRowHeader->bufSize);
                        if (CYRET_SUCCESS != err)
                        {
                            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "CyBtldr_ProgramRow: index=%d, err=%d", psocIndex, err);
                            targetMask[psocIndex] = 2;
                        }
                    }
                    /* Continue on to verify the row that was programmed */
                    checksum2 = (unsigned char) (pFlashRowHeader->checksum + pFlashRowHeader->arrayId
                            + pFlashRowHeader->rowNum + (pFlashRowHeader->rowNum >> 8) + pFlashRowHeader->bufSize
                            + (pFlashRowHeader->bufSize >> 8));
                    // go over all psocs in the mask and verify the current row
                    if (targetMask[psocIndex] == 1)
                    {
                        err = CyBtldr_VerifyRow(pFlashRowHeader->arrayId, pFlashRowHeader->rowNum, checksum2);
                        if (CYRET_SUCCESS != err)
                        {
                            M_LOGGER_LOGF(M_LOGGER_LEVEL_ERROR, "CyBtldr_VerifyRow: index=%d, err=%d", psocIndex, err);
                            targetMask[psocIndex] = 2;
                        }
                    }
                }
                else if (CYRET_ERR_EOF == err)
                {
                    err = CYRET_SUCCESS;
                    break;
                }
            } while (CYRET_SUCCESS == err && pFlashRowHeader->isLast == (char) false && targetMask[psocIndex] == 1);
        }

        // check if the mask is still "1" from the previous phase.
        if (targetMask[psocIndex] == 1)
        {
            /* Verify that the entire application is valid */
            if ((blVer >= BL_VER_SUPPORT_VERIFY))
                err = CyBtldr_VerifyApplication();
        }

        // if the psoc is in bootloader, need to exit the bootloader regardless of the boot success.
        if (targetMask[psocIndex] != 0)
        {
            // TODO: print error handling code here.
            M_LOGGER_LOGF(M_LOGGER_LEVEL_DEBUG, "Writing Application to PSoC %d is finished. ", psocIndex);
            err = CyBtldr_EndBootloadOperation();
        }

        m_pPsocManager->setPsocCsHigh(psocIndex);

    }

    return err;
}

