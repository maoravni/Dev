/*
 * PortAllocations.c
 *
 *  Created on: 15 Nov 2015
 *      Author: maora
 */
#include "PortAllocations.h"

#ifdef WIN32

GPIO_TypeDef _GPIOA;
GPIO_TypeDef _GPIOB;
GPIO_TypeDef _GPIOC;
GPIO_TypeDef _GPIOD;
GPIO_TypeDef _GPIOE;
GPIO_TypeDef _GPIOF;
GPIO_TypeDef _GPIOG;
GPIO_TypeDef _GPIOH;
GPIO_TypeDef _GPIOI;
GPIO_TypeDef _GPIOJ;
GPIO_TypeDef _GPIOK;

#define GPIOA               ((GPIO_TypeDef *) _GPIOA)
#define GPIOB               ((GPIO_TypeDef *) _GPIOB)
#define GPIOC               ((GPIO_TypeDef *) _GPIOC)
#define GPIOD               ((GPIO_TypeDef *) _GPIOD)
#define GPIOE               ((GPIO_TypeDef *) _GPIOE)
#define GPIOF               ((GPIO_TypeDef *) _GPIOF)
#define GPIOG               ((GPIO_TypeDef *) _GPIOG)
#define GPIOH               ((GPIO_TypeDef *) _GPIOH)
#define GPIOI               ((GPIO_TypeDef *) _GPIOI)
#define GPIOJ               ((GPIO_TypeDef *) _GPIOJ)
#define GPIOK               ((GPIO_TypeDef *) _GPIOK)

USART_TypeDef _USART3;
USART_TypeDef _USART6;


#endif
