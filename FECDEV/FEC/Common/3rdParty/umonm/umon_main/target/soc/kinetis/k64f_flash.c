/****************************************************************************
 Copyright (c) 2010-2013 Freescale Semiconductor, Inc.
 ALL RIGHTS RESERVED.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY DIRECT, 
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ***************************************************************************
 *
 * This code is derived from the Freescale SDK
 *
 ***************************************************************************/

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

typedef unsigned char BOOL;
typedef void (* PCALLBACK)(void);

typedef signed char INT8;
typedef unsigned char UINT8;
typedef volatile signed char VINT8;
typedef volatile unsigned char VUINT8;

typedef signed short INT16;
typedef unsigned short UINT16;
typedef volatile signed short VINT16;
typedef volatile unsigned short VUINT16;

typedef signed long INT32;
typedef unsigned long UINT32;
typedef volatile signed long VINT32;
typedef volatile unsigned long VUINT32;

#define WORD2BYTE(a)					a
#define ENTER_DEBUG_MODE				asm ("BKPT #0" )
#define GET_BIT_0_7(value)              ((UINT8)((value) & 0xFF))
#define GET_BIT_8_15(value)             ((UINT8)(((value)>>8) & 0xFF))
#define GET_BIT_16_23(value)            ((UINT8)(((value)>>16) & 0xFF))
#define GET_BIT_24_31(value)            ((UINT8)((value)>>24))
#define REG_BIT_SET(address, mask)      (*(VUINT8*)(address) |= (mask))
#define REG_BIT_CLEAR(address, mask)    (*(VUINT8*)(address) &= ~(mask))
#define REG_BIT_GET(address, mask)      (*(VUINT8 *)(address) & (UINT8)(mask))
#define REG_WRITE(address, value)       (*(VUINT8*)(address) = (value))
#define REG_READ(address)               ((UINT8)(*(VUINT8*)(address)))
#define REG_WRITE32(address, value)     (*(VUINT32*)(address) = (value))
#define REG_READ32(address)             ((UINT32)(*(VUINT32*)(address)))
#define NULL_CALLBACK                   ((PCALLBACK)0xFFFFFFFF)


typedef struct _ssd_config
{
    UINT32      ftfxRegBase;        /* FTFx control register base */
    UINT32      PFlashBlockBase;    /* base address of PFlash block */
    UINT32      PFlashBlockSize;    /* size of PFlash block */
    UINT32      DFlashBlockBase;    /* base address of DFlash block */
    UINT32      DFlashBlockSize;    /* size of DFlash block */
    UINT32      EERAMBlockBase;     /* base address of EERAM block */
    UINT32      EEEBlockSize;       /* size of EEE block */
    BOOL        DebugEnable;        /* background debug mode enable bit */
    PCALLBACK   CallBack;           /* pointer to callback function */
} FLASH_SSD_CONFIG, *PFLASH_SSD_CONFIG;

typedef UINT32 (*pFLASHCOMMANDSEQUENCE) (PFLASH_SSD_CONFIG PSSDConfig);

#define FTFx_PSECTOR_SIZE       0x00001000      /* 4 KB size */

#define FTFx_OK                            0x0000
#define FTFx_ERR_MGSTAT0                   0x0001
#define FTFx_ERR_PVIOL                     0x0010
#define FTFx_ERR_ACCERR                    0x0020
#define FTFx_ERR_CHANGEPROT                0x0100
#define FTFx_ERR_NOEEE                     0x0200
#define FTFx_ERR_EFLASHONLY                0x0400
#define FTFx_ERR_RAMRDY                    0x0800
#define FTFx_ERR_RANGE                     0x1000
#define FTFx_ERR_SIZE                      0x2000

/* Flash Status Register (FSTAT)*/
#define FTFx_SSD_FSTAT_OFFSET               0x00000000
/* Flash configuration register (FCNFG)*/
#define FTFx_SSD_FCNFG_OFFSET               0x00000001
/* Flash security register (FSEC) */
#define FTFx_SSD_FSEC_OFFSET                0x00000002
/* Flash Option Register (FOPT) */
#define FTFx_SSD_FOPT_OFFSET                0x00000003
/* Flash common command object registers (FCCOB0-B) */
#define FTFx_SSD_FCCOB0_OFFSET              0x00000007
#define FTFx_SSD_FCCOB1_OFFSET              0x00000006
#define FTFx_SSD_FCCOB2_OFFSET              0x00000005
#define FTFx_SSD_FCCOB3_OFFSET              0x00000004
#define FTFx_SSD_FCCOB4_OFFSET              0x0000000B
#define FTFx_SSD_FCCOB5_OFFSET              0x0000000A
#define FTFx_SSD_FCCOB6_OFFSET              0x00000009
#define FTFx_SSD_FCCOB7_OFFSET              0x00000008
#define FTFx_SSD_FCCOB8_OFFSET              0x0000000F
#define FTFx_SSD_FCCOB9_OFFSET              0x0000000E
#define FTFx_SSD_FCCOBA_OFFSET              0x0000000D
#define FTFx_SSD_FCCOBB_OFFSET              0x0000000C
/* P-Flash protection registers (FPROT0-3) */
#define FTFx_SSD_FPROT0_OFFSET              0x00000013
#define FTFx_SSD_FPROT1_OFFSET              0x00000012
#define FTFx_SSD_FPROT2_OFFSET              0x00000011
#define FTFx_SSD_FPROT3_OFFSET              0x00000010
/* D-Flash protection registers (FDPROT) */
#define FTFx_SSD_FDPROT_OFFSET              0x00000017
/* EERAM Protection Register (FEPROT)  */
#define FTFx_SSD_FEPROT_OFFSET              0x00000016

#define FTFx_SSD_FSTAT_CCIF                 0x80
#define FTFx_SSD_FSTAT_RDCOLERR             0x40
#define FTFx_SSD_FSTAT_ACCERR               0x20
#define FTFx_SSD_FSTAT_FPVIOL               0x10
#define FTFx_SSD_FSTAT_MGSTAT0              0x01
#define FTFx_SSD_FSTAT_ERROR_BITS           (FTFx_SSD_FSTAT_ACCERR \
                                            |FTFx_SSD_FSTAT_FPVIOL \
                                            |FTFx_SSD_FSTAT_MGSTAT0)

/*------------- Flash hardware algorithm operation commands -------------*/
#define FTFx_VERIFY_BLOCK               0x00
#define FTFx_VERIFY_SECTION             0x01
#define FTFx_PROGRAM_CHECK              0x02
#define FTFx_READ_RESOURCE              0x03
#define FTFx_PROGRAM_LONGWORD           0x06
#define FTFx_PROGRAM_PHRASE             0x07
#define FTFx_ERASE_BLOCK                0x08
#define FTFx_ERASE_SECTOR               0x09
#define FTFx_PROGRAM_SECTION            0x0B
#define FTFx_VERIFY_ALL_BLOCK           0x40
#define FTFx_READ_ONCE                  0x41
#define FTFx_PROGRAM_ONCE               0x43
#define FTFx_ERASE_ALL_BLOCK            0x44
#define FTFx_SECURITY_BY_PASS           0x45
#define FTFx_PFLASH_SWAP                0x46
#define FTFx_PROGRAM_PARTITION          0x80
#define FTFx_SET_EERAM                  0x81

/* PFlash swap control codes */
#define FTFx_SWAP_SET_INDICATOR_ADDR    0x01
#define FTFx_SWAP_SET_IN_PREPARE        0x02
#define FTFx_SWAP_SET_IN_COMPLETE       0x04
#define FTFx_SWAP_REPORT_STATUS         0x08


/* EERAM Function Control Code */
#define EEE_ENABLE                      0x00
#define EEE_DISABLE                     0xFF

/* PFlash swap states */
#define FTFx_SWAP_UNINIT                0x00
#define FTFx_SWAP_READY                 0x01
#define FTFx_SWAP_UPDATE                0x02
#define FTFx_SWAP_UPDATE_ERASED         0x03
#define FTFx_SWAP_COMPLETE              0x04

#define FTFx_PHRASE_SIZE			0x0008
#define PGM_SIZE_BYTE				FTFx_PHRASE_SIZE

#include "config.h"
#include "flash.h"
#include <stdio.h>
#include <string.h>

#define DUMMY_FLASHID	99
#define SECTORTOT		(PBLOCK_SIZE/FTFx_PSECTOR_SIZE)

FLASH_SSD_CONFIG flashSSDConfig = {
    0x40020000, 0, 0x100000, 0xffffffff, 0, 0xffffffff, 0, 0, NULL_CALLBACK
};

long k64f_ferase_buf[128];
long k64f_fwrite_buf[128];

/* k64f_erase():
 * This is a hack from the SDK function FlashEraseSector to fit
 * the uMon-M flash model.  This function assumes it resides in ram.
 */
int
k64f_erase(struct flashinfo *fdev, int snum)
{
	UINT32 dest;

	if (snum >= SECTORTOT)
		return(-1);

	dest = snum * FTFx_PSECTOR_SIZE; 
     
	/* clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear*/
	REG_WRITE(flashSSDConfig.ftfxRegBase + FTFx_SSD_FSTAT_OFFSET,FTFx_SSD_FSTAT_ERROR_BITS);   

	/* passing parameter to the command */
	REG_WRITE(flashSSDConfig.ftfxRegBase + FTFx_SSD_FCCOB0_OFFSET, FTFx_ERASE_SECTOR);
	REG_WRITE(flashSSDConfig.ftfxRegBase + FTFx_SSD_FCCOB1_OFFSET, GET_BIT_16_23(dest));
	REG_WRITE(flashSSDConfig.ftfxRegBase + FTFx_SSD_FCCOB2_OFFSET, GET_BIT_8_15(dest));
	REG_WRITE(flashSSDConfig.ftfxRegBase + FTFx_SSD_FCCOB3_OFFSET, GET_BIT_0_7(dest));

	/* clear CCIF to launch command */
	REG_BIT_SET(flashSSDConfig.ftfxRegBase + FTFx_SSD_FSTAT_OFFSET, FTFx_SSD_FSTAT_CCIF); 

	/* wait for completion of this command */
	while((REG_BIT_GET(flashSSDConfig.ftfxRegBase + FTFx_SSD_FSTAT_OFFSET, FTFx_SSD_FSTAT_CCIF)) == 0);
            
	if ((REG_READ(flashSSDConfig.ftfxRegBase + FTFx_SSD_FSTAT_OFFSET) & FTFx_SSD_FSTAT_ERROR_BITS) != 0)
		return(-2);

	return(0);
}
void
k64f_erase_end(void)
{}

int
k64f_write(struct flashinfo *fdev,unsigned char *_dest,unsigned char *pData,long size)
{
    UINT8 i;
    UINT32 temp, dest;
	char tmpsrc[PGM_SIZE_BYTE];

	dest = (UINT32)_dest;
    
    /* convert to byte addr */
	temp = flashSSDConfig.PFlashBlockBase;
	if((dest >= temp) && (dest < (temp + flashSSDConfig.PFlashBlockSize)))    {
		dest -= temp;
	} else {
		return(-2);
	}
    while(size > 0)
    {
		if (size < PGM_SIZE_BYTE) {
			for(i=0;i<PGM_SIZE_BYTE;i++)
				tmpsrc[i] = ((char *)dest)[i];
			for(i=0;i<size;i++)
				tmpsrc[i] = pData[i];
			pData = (unsigned char *)tmpsrc;
		}

        /* clear RDCOLERR & ACCERR & FPVIOL flag in flash status register. Write 1 to clear*/
        REG_WRITE(flashSSDConfig.ftfxRegBase + FTFx_SSD_FSTAT_OFFSET,FTFx_SSD_FSTAT_ERROR_BITS);   
        /* passing parameter to the command */
        REG_WRITE(flashSSDConfig.ftfxRegBase + FTFx_SSD_FCCOB0_OFFSET, FTFx_PROGRAM_PHRASE);
        REG_WRITE(flashSSDConfig.ftfxRegBase + FTFx_SSD_FCCOB1_OFFSET, GET_BIT_16_23(dest));
        REG_WRITE(flashSSDConfig.ftfxRegBase + FTFx_SSD_FCCOB2_OFFSET, GET_BIT_8_15(dest));
        REG_WRITE(flashSSDConfig.ftfxRegBase + FTFx_SSD_FCCOB3_OFFSET, GET_BIT_0_7(dest));
        
        for (i = 0; i <  PGM_SIZE_BYTE; i++)
             REG_WRITE(flashSSDConfig.ftfxRegBase + (i+8), *(pData +i));
 
		/* clear CCIF to launch command */
		REG_BIT_SET(flashSSDConfig.ftfxRegBase + FTFx_SSD_FSTAT_OFFSET, FTFx_SSD_FSTAT_CCIF); 

		/* wait for completion of this command */
		while((REG_BIT_GET(flashSSDConfig.ftfxRegBase + FTFx_SSD_FSTAT_OFFSET, FTFx_SSD_FSTAT_CCIF)) == 0);
            
		if ((REG_READ(flashSSDConfig.ftfxRegBase + FTFx_SSD_FSTAT_OFFSET) & FTFx_SSD_FSTAT_ERROR_BITS) != 0) {
			return(-3);
		}
            
		/* update destination address for next iteration */
		dest += PGM_SIZE_BYTE;
		/* update size for next iteration */
		size -= PGM_SIZE_BYTE;
		/* increment the source adress by 1 */
		pData += PGM_SIZE_BYTE;
    }
    return(0);
}

void
k64f_write_end(void)
{}

int
k64f_ewrite(struct flashinfo *fdev,unsigned char *dest,unsigned char *src,long bytecnt)
{
    printf("Ewrite not implemented yet.\n");
    return(-1);
}

int
k64f_type(struct flashinfo *fdev)
{
	fdev->id = DUMMY_FLASHID;
	return(fdev->id);
}

int
k64f_lock(struct flashinfo *fdev,int snum,int operation)
{
    if (operation == FLASH_LOCKABLE)
        return(0);
    return(-1);
}

struct flashdesc FlashNamId[] = {
	{ DUMMY_FLASHID,	"K64F-Flash"}
};

struct sectorinfo sinfoK64F[SECTORTOT];

/* FlashBankInit():
 * Initialize flash structures and determine flash device type.
 */
int
FlashBankInit(struct flashinfo *fbnk, int snum)
{
	int	i, ssize;
	unsigned long addr;

	addr = (unsigned long)(fbnk->base);
	fbnk->sectorcnt = SECTORTOT;
	ssize = 0x1000;

	for(i=0;i<SECTORTOT;i++) {
		fbnk->sectors[i].snum = snum+i;
		fbnk->sectors[i].size = ssize;
		fbnk->sectors[i].begin = (unsigned char *)addr;
		fbnk->sectors[i].end = (unsigned char *)(addr + ssize - 1);
		fbnk->sectors[i].protected = 0;
		addr += ssize;
	}
	fbnk->end = (unsigned char *)(addr - 1);
	return(SECTORTOT);
}

/* FlashInit():
 * Initialize data structures for each bank of flash...
 */
int
FlashInit(void)
{
	int		snum, fsize;
	unsigned long from, to;
	struct	flashinfo *fbnk;

	snum = 0;
	FlashCurrentBank = 0;

	fbnk = &FlashBank[0];
	k64f_type(fbnk);
	fbnk->base = (unsigned char *)FLASH_BANK0_BASE_ADDR;
	fbnk->width = 0;	/* Not needed for this device. */

	from = (unsigned long)(k64f_erase) - 1;
	fsize = (unsigned long)(k64f_erase_end) - (unsigned long)(k64f_erase);
	to = (unsigned long)k64f_ferase_buf;
	if (fsize > sizeof(k64f_ferase_buf))
		printf("FErr1\n");
	else
		memcpy((void *)to,(void *)from,fsize);
	fbnk->flerase = (int(*)())(to|1);

	from = (unsigned long)(k64f_write) - 1;
	fsize = (unsigned long)(k64f_write_end) - (unsigned long)(k64f_write);
	to = (unsigned long)k64f_fwrite_buf;
	if (fsize > sizeof(k64f_fwrite_buf))
		printf("FErr2\n");
	else
		memcpy((void *)to,(void *)from,fsize);
	fbnk->flwrite = (int(*)())(to|1);

	fbnk->fltype = k64f_type;
	fbnk->flewrite = k64f_ewrite;
	fbnk->fllock = k64f_lock;

	fbnk->sectors = sinfoK64F;
	snum += FlashBankInit(fbnk,snum);

	sectorProtect(FLASH_PROTECT_RANGE,1);

	return(0);
}
