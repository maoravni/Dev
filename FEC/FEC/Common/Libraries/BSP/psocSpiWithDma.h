/*
 * spiWithDma.h
 *
 *  Created on: 5 бреб 2013
 *      Author: maora
 */

#ifndef SPIWITHDMA_H_
#define SPIWITHDMA_H_

#include <stdint.h>
#include <fec2/psoc_spi_port_definitions.h>
#include <cybootloaderutils/cybtldr_api.h>

#ifdef __cplusplus
extern "C"
{
#endif

// the frame size is 32, however since the CRC is calculated and sent by HW, the struct should contain only 31 bytes.
#define M_PSOC_SPI_MAX_TRANSFER_SIZE 64
#define M_PSOC_SPI_FRAME_SIZE 31

#define FEC3_PSOC_RESET_HIGH() GPIOF->BSRRL = GPIO_Pin_13
#define FEC3_PSOC_RESET_LOW() GPIOF->BSRRH = GPIO_Pin_13

typedef struct _psocSpiReceiveFrame
{
    uint8_t buffer[M_PSOC_SPI_MAX_TRANSFER_SIZE];
    uint8_t crcOk;
    uint8_t transferComplete;
} T_PsocSpiReceiveFrame;

void PSOC_setupSpiWithDma();
void PSOC_fullDuplexTransfer(uint8_t *transmitBuffer, uint8_t length);
void PSOC_sendBuffer(uint8_t *buf, uint8_t len);
void PSOC_startReceive();

/* Function declarations */
int PsocBootloader_OpenConnection(void);
int PsocBootloader_CloseConnection(void);
int PsocBootloader_ReadData(uint8_t* rdData, int byteCnt);
int PsocBootloader_WriteData(uint8_t* wrData, int byteCnt);
CyBtldr_CommunicationsData* PsocBootloader_Setup();

#ifdef __cplusplus
}
#endif

#endif /* SPIWITHDMA_H_ */
