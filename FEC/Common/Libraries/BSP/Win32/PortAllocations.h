/*
 * PortAllocations.h
 *
 *  Created on: 15 Nov 2015
 *      Author: maora
 */

#ifndef PORTALLOCATIONS_H_
#define PORTALLOCATIONS_H_

#ifdef WIN32
#ifdef __cplusplus
extern "C" {
#endif

#include <Win32MissingDefines.h>

	extern GPIO_TypeDef _GPIOA;
	extern GPIO_TypeDef _GPIOB;
	extern GPIO_TypeDef _GPIOC;
	extern GPIO_TypeDef _GPIOD;
	extern GPIO_TypeDef _GPIOE;
	extern GPIO_TypeDef _GPIOF;
	extern GPIO_TypeDef _GPIOG;
	extern GPIO_TypeDef _GPIOH;
	extern GPIO_TypeDef _GPIOI;
	extern GPIO_TypeDef _GPIOJ;
	extern GPIO_TypeDef _GPIOK;
#define GPIOA               ((GPIO_TypeDef *) &_GPIOA)
#define GPIOB               ((GPIO_TypeDef *) &_GPIOB)
#define GPIOC               ((GPIO_TypeDef *) &_GPIOC)
#define GPIOD               ((GPIO_TypeDef *) &_GPIOD)
#define GPIOE               ((GPIO_TypeDef *) &_GPIOE)
#define GPIOF               ((GPIO_TypeDef *) &_GPIOF)
#define GPIOG               ((GPIO_TypeDef *) &_GPIOG)
#define GPIOH               ((GPIO_TypeDef *) &_GPIOH)
#define GPIOI               ((GPIO_TypeDef *) &_GPIOI)
#define GPIOJ               ((GPIO_TypeDef *) &_GPIOJ)
#define GPIOK               ((GPIO_TypeDef *) &_GPIOK)

	extern USART_TypeDef _USART3;
	extern USART_TypeDef _USART6;
#define USART3 ((USART_TypeDef *)&_USART3)
#define USART6 ((USART_TypeDef *)&_USART6)

#define I2C1 NULL
#define I2C2 NULL
#define  I2C_Direction_Transmitter      ((uint8_t)0x00)
#define  I2C_Direction_Receiver         ((uint8_t)0x01)

#ifdef __cplusplus
}
#endif

#endif

#endif /* PORTALLOCATIONS_H_ */
