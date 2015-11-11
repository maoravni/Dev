/* flashenv.c:
 * This code provides a very basic alternative to uMon's typical
 * shell variable environment.
 * It assumes it owns one sector of flash and it is dedicated to
 * storage (and retrieval) of environment variables.
 * 
 * Very basic implementation assumes each name/val pair will fit
 * within one FLASHENV_CHUNK size block of flash.  
 *
 * A given pair is simply "NAME \0 VALUE \0" padded to the end with
 * 0xff (erased flash).
 *
 */

#include "config.h"
#if INCLUDE_FLASHENV
#include "cli.h"
#include "flash.h"
#include "flashenv.h"
#include "genlib.h"

#ifndef FLASHENV_CHUNK
#define FLASHENV_CHUNK 32
#endif

#define FLASHENV_SNUM 255
#define FLASHENV_BASE ((char *)0x000ff000)
#define FLASHENV_END  ((char *)0x000fffff)

void
showFlashEnv(void)
{
	char *fptr;
	fptr = FLASHENV_BASE;

	while(fptr < FLASHENV_END) {
		if (*fptr == 0xff) {
			break;
		}
		else {
			char *name, *value;
			name = value = fptr;
			while(*value != 0) {
				value++;
				if (value >= FLASHENV_END)
					return;
			}
			printf("%s: %s\n",name,value+1);
			fptr += FLASHENV_CHUNK;
		}
	}
}

int
flashenv_erase(void)
{
	return(flasherase(FLASHENV_SNUM));
}

int
flashenv_cmd(char *arg1, char *arg2)
{
	if (arg1 == 0) {
		showFlashEnv();
	}
	else {
		flashenv_set(arg1,arg2);
	}

	return(CMD_SUCCESS);
}

char *
flashenv_get(char *name)
{
	char *fptr;
	fptr = FLASHENV_BASE;

	while(fptr < FLASHENV_END) {
		if (*fptr == 0xff) 
			break;
		else {
			if (strcmp(fptr,name) == 0) {
				while(*fptr != 0) {
					fptr++;
					if (fptr >= FLASHENV_END)
						return(0);
				}
				return(fptr+1);
			}
			fptr += FLASHENV_CHUNK;
		}
	}

	// If we're here, the we didn't find
	// the name in the environement, so as
	// a last resort, we check with some of
	// the system defaults...
	if (strcmp(name,"IPADD") == 0)
		return(DEFAULT_IPADD);
	if (strcmp(name,"ETHERADD") == 0)
		return(DEFAULT_ETHERADD);

	// If no match, return 0
	return(0);
}

static int
flashenv_del(char *name)
{
	char *fptr, *ram, *rbase;

	fptr = FLASHENV_BASE;
	ram = rbase = (char *)getAppRamStart();
	while(fptr < FLASHENV_END) {
		if (*fptr == 0xff) 
			break;
		else {
			if (strcmp(fptr,name)) {
				memcpy(ram,fptr,FLASHENV_CHUNK);
				ram += FLASHENV_CHUNK;
			}
			fptr += FLASHENV_CHUNK;
		}
	}
	flashenv_erase();
	return(AppFlashWrite((unsigned char *)FLASHENV_BASE,
				(unsigned char *)rbase,fptr-FLASHENV_BASE));
}

int
flashenv_set(char *name, char *value)
{
	unsigned char *fptr;
	int namelen, valuelen;
	char buf[FLASHENV_CHUNK];

	if (flashenv_get(name))
		flashenv_del(name);

	if (value == 0)
		return(0);

	namelen = strlen(name);
	valuelen = strlen(value);
	if ((namelen + valuelen + 2) >= FLASHENV_CHUNK)
		return(-1); 
	strcpy(buf,name);
	strcpy(buf+namelen+1,value);


	fptr = (unsigned char *)FLASHENV_BASE;
	while(fptr < (unsigned char *)FLASHENV_END) {
		if (*fptr == 0xff) {
			return(AppFlashWrite((unsigned char *)fptr,
				(unsigned char *)buf,namelen+valuelen+2));
		}
		else {
			fptr += FLASHENV_CHUNK;
		}
	}
	return(-1);
}

void
flashenv_boot(void)
{
	char *bootcmd = flashenv_get("BOOT");

	if (bootcmd) 
		docommand(bootcmd,1);
}
#endif
