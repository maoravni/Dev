/**
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __Win32MissingDefines_H
#define __Win32MissingDefines_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	* @brief  RTC Time structure definition
	*/
	typedef struct
	{
		uint8_t RTC_Hours;    /*!< Specifies the RTC Time Hour.
							  This parameter must be set to a value in the 0-12 range
							  if the RTC_HourFormat_12 is selected or 0-23 range if
							  the RTC_HourFormat_24 is selected. */

		uint8_t RTC_Minutes;  /*!< Specifies the RTC Time Minutes.
							  This parameter must be set to a value in the 0-59 range. */

		uint8_t RTC_Seconds;  /*!< Specifies the RTC Time Seconds.
							  This parameter must be set to a value in the 0-59 range. */

		uint8_t RTC_H12;      /*!< Specifies the RTC AM/PM Time.
							  This parameter can be a value of @ref RTC_AM_PM_Definitions */
	}RTC_TimeTypeDef;

	/**
	* @brief  RTC Date structure definition
	*/
	typedef struct
	{
		uint8_t RTC_WeekDay; /*!< Specifies the RTC Date WeekDay.
							 This parameter can be a value of @ref RTC_WeekDay_Definitions */

		uint8_t RTC_Month;   /*!< Specifies the RTC Date Month (in BCD format).
							 This parameter can be a value of @ref RTC_Month_Date_Definitions */

		uint8_t RTC_Date;     /*!< Specifies the RTC Date.
							  This parameter must be set to a value in the 1-31 range. */

		uint8_t RTC_Year;     /*!< Specifies the RTC Date Year.
							  This parameter must be set to a value in the 0-99 range. */
	}RTC_DateTypeDef;

	typedef struct
	{
		uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
		uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
		uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
		uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
		uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
		uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
		uint16_t BSRRL;    /*!< GPIO port bit set/reset low register,  Address offset: 0x18      */
		uint16_t BSRRH;    /*!< GPIO port bit set/reset high register, Address offset: 0x1A      */
		uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
		uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
	} GPIO_TypeDef;

#define GPIO_Pin_0                 ((uint16_t)0x0001)  /* Pin 0 selected */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /* Pin 1 selected */
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /* Pin 2 selected */
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /* Pin 3 selected */
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /* Pin 4 selected */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /* Pin 5 selected */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /* Pin 6 selected */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /* Pin 7 selected */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /* Pin 8 selected */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /* Pin 9 selected */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /* Pin 10 selected */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /* Pin 11 selected */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /* Pin 12 selected */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /* Pin 13 selected */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /* Pin 14 selected */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /* Pin 15 selected */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /* All pins selected */

#define TCP_MSS 536

#define lwip_close closesocket
#define lwip_socket socket
#define lwip_bind bind
#define lwip_listen listen
#define lwip_accept accept
#define lwip_setsockopt setsockopt
#define lwip_getsockopt getsockopt
#define lwip_recv recv
#define lwip_send ::send
#define lwip_sendto sendto

#define socklen_t int


#ifdef __cplusplus
}
#endif

typedef struct
{
	uint16_t CR1;        /*!< I2C Control register 1,     Address offset: 0x00 */
	uint16_t      RESERVED0;  /*!< Reserved, 0x02                                   */
	uint16_t CR2;        /*!< I2C Control register 2,     Address offset: 0x04 */
	uint16_t      RESERVED1;  /*!< Reserved, 0x06                                   */
	uint16_t OAR1;       /*!< I2C Own address register 1, Address offset: 0x08 */
	uint16_t      RESERVED2;  /*!< Reserved, 0x0A                                   */
	uint16_t OAR2;       /*!< I2C Own address register 2, Address offset: 0x0C */
	uint16_t      RESERVED3;  /*!< Reserved, 0x0E                                   */
	uint16_t DR;         /*!< I2C Data register,          Address offset: 0x10 */
	uint16_t      RESERVED4;  /*!< Reserved, 0x12                                   */
	uint16_t SR1;        /*!< I2C Status register 1,      Address offset: 0x14 */
	uint16_t      RESERVED5;  /*!< Reserved, 0x16                                   */
	uint16_t SR2;        /*!< I2C Status register 2,      Address offset: 0x18 */
	uint16_t      RESERVED6;  /*!< Reserved, 0x1A                                   */
	uint16_t CCR;        /*!< I2C Clock control register, Address offset: 0x1C */
	uint16_t      RESERVED7;  /*!< Reserved, 0x1E                                   */
	uint16_t TRISE;      /*!< I2C TRISE register,         Address offset: 0x20 */
	uint16_t      RESERVED8;  /*!< Reserved, 0x22                                   */
	uint16_t FLTR;       /*!< I2C FLTR register,          Address offset: 0x24 */
	uint16_t      RESERVED9;  /*!< Reserved, 0x26                                   */
} I2C_TypeDef;

/**
* @brief Universal Synchronous Asynchronous Receiver Transmitter
*/

typedef struct
{
	uint16_t SR;         /*!< USART Status register,                   Address offset: 0x00 */
	uint16_t      RESERVED0;  /*!< Reserved, 0x02                                                */
	uint16_t DR;         /*!< USART Data register,                     Address offset: 0x04 */
	uint16_t      RESERVED1;  /*!< Reserved, 0x06                                                */
	uint16_t BRR;        /*!< USART Baud rate register,                Address offset: 0x08 */
	uint16_t      RESERVED2;  /*!< Reserved, 0x0A                                                */
	uint16_t CR1;        /*!< USART Control register 1,                Address offset: 0x0C */
	uint16_t      RESERVED3;  /*!< Reserved, 0x0E                                                */
	uint16_t CR2;        /*!< USART Control register 2,                Address offset: 0x10 */
	uint16_t      RESERVED4;  /*!< Reserved, 0x12                                                */
	uint16_t CR3;        /*!< USART Control register 3,                Address offset: 0x14 */
	uint16_t      RESERVED5;  /*!< Reserved, 0x16                                                */
	uint16_t GTPR;       /*!< USART Guard time and prescaler register, Address offset: 0x18 */
	uint16_t      RESERVED6;  /*!< Reserved, 0x1A                                                */
} USART_TypeDef;

#endif /*__Win32MissingDefines_H */

