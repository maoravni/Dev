#include "vcom.h"
#include "usbapi.h"
#include "usbvcom.h"

/* This is in a separate file because there is some weird
 * conflict between usbapi.h and some of the other header files.
 */

void
InitializeVituralComPort(void)
{
	//The following instructions are needed to set up USB in the main program.
	USB_init();
	VCOM_init();
	USBHwConnect(TRUE);
}
