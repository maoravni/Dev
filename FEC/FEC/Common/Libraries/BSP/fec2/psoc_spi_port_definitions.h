/*
 * psoc_spi_port_definitions.h
 *
 *  Created on: 10 бреб 2013
 *      Author: maora
 */

#ifndef PSOC_SPI_PORT_DEFINITIONS_H_
#define PSOC_SPI_PORT_DEFINITIONS_H_

/* Definition for DMAx resources */
#define PSOC_SPI_PORT                           SPI3
#define PSOC_SPI_PORT_DR_ADDRESS                PSOC_SPI_PORT->DR

#define PSOC_SPI_PORT_DMA                       DMA1
#define PSOC_SPI_PORT_DMAx_CLK                  RCC_AHB1Periph_DMA1

#define PSOC_SPI_PORT_TX_DMA_CHANNEL            DMA_Channel_0
#define PSOC_SPI_PORT_TX_DMA_STREAM             DMA1_Stream7
#define PSOC_SPI_PORT_TX_DMA_FLAG_FEIF          DMA_FLAG_FEIF7
#define PSOC_SPI_PORT_TX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF7
#define PSOC_SPI_PORT_TX_DMA_FLAG_TEIF          DMA_FLAG_TEIF7
#define PSOC_SPI_PORT_TX_DMA_FLAG_HTIF          DMA_FLAG_HTIF7
#define PSOC_SPI_PORT_TX_DMA_FLAG_TCIF          DMA_FLAG_TCIF7
#define PSOC_SPI_PORT_TX_DMA_IT_FEIF            DMA_IT_FEIF7
#define PSOC_SPI_PORT_TX_DMA_IT_DMEIF           DMA_IT_DMEIF7
#define PSOC_SPI_PORT_TX_DMA_IT_TEIF            DMA_IT_TEIF7
#define PSOC_SPI_PORT_TX_DMA_IT_HTIF            DMA_IT_HTIF7
#define PSOC_SPI_PORT_TX_DMA_IT_TCIF            DMA_IT_TCIF7

#define PSOC_SPI_PORT_DMA_TX_IRQn               DMA1_Stream7_IRQn
#define PSOC_SPI_PORT_DMA_TX_IRQHandler         DMA1_Stream7_IRQHandler

#define PSOC_SPI_PORT_RX_DMA_CHANNEL            DMA_Channel_0
#define PSOC_SPI_PORT_RX_DMA_STREAM             DMA1_Stream2
#define PSOC_SPI_PORT_RX_DMA_FLAG_FEIF          DMA_FLAG_FEIF2
#define PSOC_SPI_PORT_RX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF2
#define PSOC_SPI_PORT_RX_DMA_FLAG_TEIF          DMA_FLAG_TEIF2
#define PSOC_SPI_PORT_RX_DMA_FLAG_HTIF          DMA_FLAG_HTIF2
#define PSOC_SPI_PORT_RX_DMA_FLAG_TCIF          DMA_FLAG_TCIF2
#define PSOC_SPI_PORT_RX_DMA_IT_FEIF            DMA_IT_FEIF2
#define PSOC_SPI_PORT_RX_DMA_IT_DMEIF           DMA_IT_DMEIF2
#define PSOC_SPI_PORT_RX_DMA_IT_TEIF            DMA_IT_TEIF2
#define PSOC_SPI_PORT_RX_DMA_IT_HTIF            DMA_IT_HTIF2
#define PSOC_SPI_PORT_RX_DMA_IT_TCIF            DMA_IT_TCIF2

#define PSOC_SPI_PORT_DMA_RX_IRQn               DMA1_Stream2_IRQn
#define PSOC_SPI_PORT_DMA_RX_IRQHandler         DMA1_Stream2_IRQHandler

#endif /* PSOC_SPI_PORT_DEFINITIONS_H_ */
