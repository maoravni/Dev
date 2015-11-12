#include "monlib.h"
#include "string.h"
#include "httpd.h"

static int lidx;
static char line[64];

void
console_prompt(void)
{
	mon_printf("APP:");
}

void
console_init()
{
	lidx = 0;
	console_prompt();
}

int
console_poll(void)
{
	char c;

	if (!mon_gotachar())
		return(0);

	c = mon_getchar();
	if (c == '\b') {
		if (lidx > 0) {
			mon_printf("\b \b");
			lidx--;
		}
		return(0);
	}
	if ((lidx == (sizeof(line)-1)) || (c == '\r') || (c == '\n')) {
		mon_printf("\r\n");
		if (lidx == 0) {
			console_prompt();
			return(0);
		}
		line[lidx] = 0;
		if (strcmp(line,"exit") == 0)
			return(-1);

		mon_com(CHARFUNC_PUTCHAR,(void *)websocd_putchar,0,0);
		mon_docommand(line,1);
		console_prompt();
		mon_com(CHARFUNC_PUTCHAR,0,0,0);
		lidx = 0;
	}
	else {
		mon_putchar(c);
		if (lidx < (sizeof(line)-2))
			line[lidx++] = c;
	}
	return(0);
}

