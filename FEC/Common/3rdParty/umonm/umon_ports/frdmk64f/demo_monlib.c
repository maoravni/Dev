/* tiny_monlib.c:
 *  VERY SMALL version of monlib.c.
 *	Refer to monlib.c for details.
 */
#include "monlib.h"

static int		(*_printf)();
static int		(*_sprintf)();
static char		*(*_getenv)(char *);
static int		(*_setenv)(char *,char *);
static void		(*_getargv)(int *,char ***);
static char		*(*_malloc)(int);
static void		(*_free)(char *);
static void		(*_appexit)(int);

static int	(*_moncom)(int,void *,void *, void *);

int
monConnect(int (*mon)(int,void *,void *,void *),
	void (*lock)(void), void (*unlock)(void))
{
	int rc = 0;

	/* NOTE: this example doesn't use the locks */

	/* If the mon pointer is non-zero, then make the mon_ connections... */
	if (mon) {

		_moncom = mon;

		/* Make the connections between "mon_" functions that are			*/
		/* symbolically	accessible by the application and the corresponding	*/
		/* functions that exists in the monitor.							*/
		rc += _moncom(GETMONFUNC_PRINTF,&_printf,0,0);
		rc += _moncom(GETMONFUNC_SPRINTF,&_sprintf,0,0);
		rc += _moncom(GETMONFUNC_GETENV,&_getenv,0,0);
		rc += _moncom(GETMONFUNC_SETENV,&_setenv,0,0);
		rc += _moncom(GETMONFUNC_APPEXIT,&_appexit,0,0);
		rc += _moncom(GETMONFUNC_MALLOC,&_malloc,0,0);
		rc += _moncom(GETMONFUNC_FREE,&_free,0,0);
		rc += _moncom(GETMONFUNC_GETARGV,&_getargv,0,0);
	}

	return(rc);
}

int
mon_printf(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)
char *fmt;
int a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12;
{
	int	ret;

	ret = _printf(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12);
	return(ret);
}

int
mon_sprintf(buf,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)
char *fmt, *buf;
int a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12;
{
	int	ret;

	ret = _sprintf(buf,fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12);
	return(ret);
}

char *
mon_getenv(char *name)
{
	char *ret;

	ret = _getenv(name);
	return(ret);
}

int
mon_setenv(char *name,char *val)
{
	int ret;

	ret = _setenv(name,val);
	return(ret);
}

void
mon_getargv(int *argc,char ***argv)
{
	_getargv(argc,argv);
}

void
mon_appexit(int val)
{
	_appexit(val);
}
