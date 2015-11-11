/*
 * operatorNew.cpp
 *
 *  Created on: 16 Dec 2012
 *      Author: maora
 */

#include "FreeRTOS.h"
#include <xmemory>
//
// Define the 'new' operator for C++ to use the freeRTOS memory management
// functions. THIS IS NOT OPTIONAL!
//
void *operator new(size_t size){
    void *p=pvPortMalloc(size);
#ifdef  __EXCEPTIONS
    if (p==0) // did pvPortMalloc succeed?
        throw std::bad_alloc(); // ANSI/ISO compliant behavior
#endif
    return p;
}

void *operator new[](size_t size){
    void *p=pvPortMalloc(size);
#ifdef  __EXCEPTIONS
    if (p==0) // did pvPortMalloc succeed?
        throw std::bad_alloc(); // ANSI/ISO compliant behavior
#endif
    return p;
}

//
// Define the 'delete' operator for C++ to use the freeRTOS memory management
// functions. THIS IS NOT OPTIONAL!
//
void operator delete(void *p){
    vPortFree( p );
}

