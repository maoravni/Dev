/**
  ******************************************************************************
  * File Name          : mx_gpio.c
  * Date               : 16/12/2013 17:54:21
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
  ******************************************************************************
  *
  * COPYRIGHT 2013 STMicroelectronics
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mx_gpio.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

void mx_pinout_config(void) {
	/* Private typedef ---------------------------------------------------------*/
	GPIO_InitTypeDef GPIO_InitStruct;


	/** ADC1 GPIO Configuration	
		 PC0	 ------> ADC1_IN10
		 PC2	 ------> ADC1_IN12
		 PC3	 ------> ADC1_IN13
		 PA0/WKUP	 ------> ADC1_IN0
		 PA3	 ------> ADC1_IN3
		 PA4	 ------> ADC1_IN4
		 PA5	 ------> ADC1_IN5
		 PB0	 ------> ADC1_IN8
		 PB1	 ------> ADC1_IN9
	*/


	/*Enable or disable the AHB1 peripheral clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOPC|RCC_AHB1Periph_GPIOPA|RCC_AHB1Periph_GPIOPB, ENABLE);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);


	/** ADC3 GPIO Configuration	
		 PF6	 ------> ADC3_IN4
		 PF7	 ------> ADC3_IN5
		 PF8	 ------> ADC3_IN6
	*/


	/*Enable or disable the AHB1 peripheral clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOPF, ENABLE);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStruct);


	/** CAN1 GPIO Configuration	
		 PD0	 ------> CAN1_RX
		 PD1	 ------> CAN1_TX
	*/


	/*Enable or disable the AHB1 peripheral clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOPD, ENABLE);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOD, &GPIO_InitStruct);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);


	/** ETH_RMII GPIO Configuration	
		 PC1	 ------> ETH_RMII_MDC
		 PA1	 ------> ETH_RMII_REF_CLK
		 PA2	 ------> ETH_RMII_MDIO
		 PA7	 ------> ETH_RMII_CRS_DV
		 PC4	 ------> ETH_RMII_RXD0
		 PC5	 ------> ETH_RMII_RXD1
		 PB11	 ------> ETH_RMII_TX_EN
		 PB12	 ------> ETH_RMII_TXD0
		 PB13	 ------> ETH_RMII_TXD1
	*/


	/*Enable or disable the AHB1 peripheral clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOPC|RCC_AHB1Periph_GPIOPA|RCC_AHB1Periph_GPIOPB, ENABLE);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOC, &GPIO_InitStruct);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOA, &GPIO_InitStruct);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOA, &GPIO_InitStruct);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOA, &GPIO_InitStruct);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOC, &GPIO_InitStruct);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOC, &GPIO_InitStruct);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOB, &GPIO_InitStruct);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOB, &GPIO_InitStruct);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_ETH);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_ETH);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_ETH);


	/** I2C1 GPIO Configuration	
		 PB7	 ------> I2C1_SDA
		 PB8	 ------> I2C1_SCL
	*/


	/*Enable or disable the AHB1 peripheral clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOPB, ENABLE);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOB, &GPIO_InitStruct);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);


	/** OTG_FS GPIO Configuration	
		 PA9	 ------> OTG_FS_VBUS
		 PA11	 ------> OTG_FS_DM
		 PA12	 ------> OTG_FS_DP
	*/


	/*Enable or disable the AHB1 peripheral clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOPA, ENABLE);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOA, &GPIO_InitStruct);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOA, &GPIO_InitStruct);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_OTG_FS);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_OTG_FS);


	/** SPI2 GPIO Configuration	
		 PB14	 ------> SPI2_MISO
		 PB15	 ------> SPI2_MOSI
		 PD3	 ------> SPI2_SCK
	*/


	/*Enable or disable the AHB1 peripheral clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOPB|RCC_AHB1Periph_GPIOPD, ENABLE);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOB, &GPIO_InitStruct);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOB, &GPIO_InitStruct);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_SPI2);


	/** SPI3 GPIO Configuration	
		 PC10	 ------> SPI3_SCK
		 PC11	 ------> SPI3_MISO
		 PC12	 ------> SPI3_MOSI
	*/


	/*Enable or disable the AHB1 peripheral clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOPC, ENABLE);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOC, &GPIO_InitStruct);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOC, &GPIO_InitStruct);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);


	/** SYS GPIO Configuration	
		 PC14/OSC32_IN	 ------> SYS_OSC32_IN
		 PC15/OSC32_OUT	 ------> SYS_OSC32_OUT
		 PH0/OSC_IN	 ------> SYS_OSC_IN
		 PH1/OSC_OUT	 ------> SYS_OSC_OUT
		 PA13	 ------> SYS_JTMS-SWDIO
		 PA14	 ------> SYS_JTCK-SWCLK
		 PA15	 ------> SYS_JTDI
		 PB3	 ------> SYS_JTDO-SWO
		 PB4	 ------> SYS_JTRST
	*/



	/** USART3 GPIO Configuration	
		 PD8	 ------> USART3_TX
		 PD9	 ------> USART3_RX
	*/


	/*Enable or disable the AHB1 peripheral clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOPD, ENABLE);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOD, &GPIO_InitStruct);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);


	/** USART6 GPIO Configuration	
		 PG9	 ------> USART6_RX
		 PG14	 ------> USART6_TX
	*/


	/*Enable or disable the AHB1 peripheral clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOPG, ENABLE);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOG, &GPIO_InitStruct);


	//!!! GPIO_Init is commented because some parameters are missing
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
	//GPIO_InitStruct.GPIO_Mode = ;
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	//GPIO_Init(GPIOG, &GPIO_InitStruct);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6);

	/*Configure GPIO pin alternate function */
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6);


	/** Configure pins as GPIO
		 PE2	 ------> GPIO_Output
		 PE3	 ------> GPIO_Output
		 PE4	 ------> GPIO_Output
		 PE5	 ------> GPIO_Output
		 PE6	 ------> GPIO_Output
		 PC13	 ------> GPIO_Output
		 PF0	 ------> GPIO_Output
		 PF1	 ------> GPIO_Input
		 PF2	 ------> GPIO_Output
		 PF3	 ------> GPIO_Output
		 PF4	 ------> GPIO_Output
		 PF5	 ------> GPIO_Input
		 PF9	 ------> GPIO_Output
		 PF10	 ------> GPIO_Output
		 PA6	 ------> GPIO_Output
		 PF11	 ------> GPIO_Output
		 PF12	 ------> GPIO_Output
		 PF13	 ------> GPIO_Output
		 PF14	 ------> GPIO_Output
		 PF15	 ------> GPIO_Output
		 PG0	 ------> GPIO_Output
		 PG1	 ------> GPIO_Output
		 PE7	 ------> GPIO_Output
		 PE8	 ------> GPIO_Output
		 PE9	 ------> GPIO_Input
		 PE10	 ------> GPIO_Output
		 PE11	 ------> GPIO_Input
		 PE12	 ------> GPIO_Output
		 PE13	 ------> GPIO_Output
		 PE14	 ------> GPIO_Input
		 PE15	 ------> GPIO_Output
		 PB10	 ------> GPIO_Input
		 PD10	 ------> GPIO_Output
		 PD11	 ------> GPIO_Output
		 PD12	 ------> GPIO_Output
		 PD13	 ------> GPIO_Input
		 PD14	 ------> GPIO_Output
		 PD15	 ------> GPIO_Output
		 PG2	 ------> GPIO_Output
		 PG3	 ------> GPIO_Output
		 PG4	 ------> GPIO_Output
		 PG7	 ------> GPIO_Input
		 PG8	 ------> GPIO_Input
		 PC6	 ------> GPIO_Output
		 PC7	 ------> GPIO_Output
		 PC8	 ------> GPIO_Output
		 PC9	 ------> GPIO_Output
		 PD2	 ------> GPIO_Output
		 PD4	 ------> GPIO_Input
		 PD5	 ------> GPIO_Input
		 PD6	 ------> GPIO_Input
		 PD7	 ------> GPIO_Output
		 PG10	 ------> GPIO_Input
		 PG11	 ------> GPIO_Input
		 PG12	 ------> GPIO_Input
		 PG13	 ------> GPIO_Input
		 PG15	 ------> GPIO_Input
		 PB5	 ------> GPIO_Output
		 PB6	 ------> GPIO_Output
		 PB9	 ------> GPIO_Output
		 PE0	 ------> GPIO_Output
		 PE1	 ------> GPIO_Output
	*/


	/*Enable or disable the AHB1 peripheral clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOPE|RCC_AHB1Periph_GPIOPC|RCC_AHB1Periph_GPIOPF|RCC_AHB1Periph_GPIOPA|RCC_AHB1Periph_GPIOPG|RCC_AHB1Periph_GPIOPB|RCC_AHB1Periph_GPIOPD, ENABLE);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15|GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOF, &GPIO_InitStruct);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStruct);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOG, &GPIO_InitStruct);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_2|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG, &GPIO_InitStruct);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
