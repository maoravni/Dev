/*
 * filesystemdriver.c
 *
 *  Created on: 7 Apr 2016
 *      Author: maora
 */

#include "api/fat_sl.h"
#include <media-drv/eeprom_mc24m02/eeprom_mc24m02.h>

int filesystemDriver_init()
{
    int result;
    result = f_initvolume(&eeprom_initfunc);

    if (result == F_ERR_NOTFORMATTED)
    {
        result = f_format(F_FAT12_MEDIA);
    }

    return result;
}

void filesystemDriver_printFree()
{
    F_SPACE space;
    fn_getfreespace(&space);
    printf("total %d free %d used %d bad %d totalH %d freeH %d usedH %d badH %d\n", space.total, space.free, space.used,
            space.bad, space.total_high, space.free_high, space.used_high, space.bad_high);
}
