/*
 * filesystemdriver.h
 *
 *  Created on: 7 Apr 2016
 *      Author: maora
 */

#ifndef FILESYSTEMDRIVER_H_
#define FILESYSTEMDRIVER_H_

#include <api/fat_sl.h>

#ifdef __cplusplus
extern "C"
{
#endif

int filesystemDriver_init();
int filesystemDriver_free();
void filesystemDriver_printFree();
int filesystemDriver_format();

#ifdef __cplusplus
}
#endif

#endif /* FILESYSTEMDRIVER_H_ */
