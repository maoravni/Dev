###############################################################################
#
# LM3S6918 (TI's ARM Cortex M3) uMon makefile
#
# Prerequisites:
# CodeSourcery Lite is the toolset; using TOOL_PREFIX (below) as the path.
# Some of the rules (reset, flash, flashx, etc...) in this makefile assume
# a Segger J-Link device is conntected.
#
# Misc notes:
# For connecting CooCox to Segger:
# http://www.segger.com/IDE_Integration_coocox.html

UCONSYSIP		= 192.168.1.2

PLATFORM		= LM3S6918
TOPDIR			= $(UMONTOP)
TGTDIR			= lm3s6918
CPUTYPE			= arm
SOCDIR			= $(UMONTOP)/target/soc/lm3s6918
FILETYPE		= elf
CUSTOM_INCLUDE	= -I $(SOCDIR)
CUSTOM_CFLAGS	= -mcpu=cortex-m3 -O0 -mthumb -ffunction-sections -DLM3S6918
LFLAGS			= -O0 -nostartfiles -mcpu=cortex-m3 -Wl,--gc-sections \
				  -Wl,-Tlink.ld 

# It is assumed that the toolset is in the PATH, if not then just
# insert the full path here...
TOOL_PREFIX	= arm-none-eabi

include	$(TOPDIR)/target/make/common.make

LD = $(CC)

# Build each variable from a list of individual filenames...
#
LOCSSRC		= 
CPUSSRC		= 
LOCCSRC		= cpuio.c etherdev.c 

SOCCSRC		= $(SOCDIR)/lm3s6918_cpu.c $(SOCDIR)/lm3s6918_flash.c \
			  $(SOCDIR)/lm3s6918_ethernet.c \
			  $(SOCDIR)/lm3s6918_gpio.c $(SOCDIR)/lm3s6918_interrupt.c \
			  $(SOCDIR)/startup_lm3s.c $(SOCDIR)/lm3s6918_sysctl.c \
			  $(SOCDIR)/lm3s6918_uart.c

COMCSRC		= arp.c cache.c chario.c cmdtbl.c docmd.c dhcp_00.c \
			  dhcpboot.c edit.c ee.c env.c ethernet.c flash.c genlib.c \
			  icmp.c ledit_vt100.c if.c mprintf.c memcmds.c malloc.c \
			  moncom.c misccmds.c misc.c start.c syslog.c \
			  tcpstuff.c timestuff.c tfs.c tfsapi.c tfsclean4.c \
			  tfscli.c tftp.c xmodem.c

CPUCSRC		= core_cm3.c
IODEVSRC	= 
FLASHSRC	= 


include $(TOPDIR)/target/make/objects.make

OBJS	= 	$(LOCCOBJ) $(LOCSOBJ) $(SOCCOBJ) $(CPUSOBJ) $(CPUCOBJ) \
		    $(COMCOBJ) $(FLASHOBJ) $(IODEVOBJ)

##############################################################################
#
# Targets...
# For the Cortex-Mx boards there is only a boot target and the memory map
# is not configurable, so things are a bit simpler.

# boot:
# The default target is "boot", a shortcut to $(BUILDDIR)/boot.$(FILETYPE).
# This builds the bootflash image that can be used by 'newmon' to 
# load a new version onto an already running system.
#
boot:	$(BUILDDIR)/boot.$(FILETYPE)
	@echo Boot version of uMon built under $(BUILDDIR) ...
	@ls $(BUILDDIR)/boot*
	cp $(BUILDDIR)/boot.elf stripped.elf
	$(STRIP) stripped.elf
	elf -m stripped.elf

$(BUILDDIR)/boot.$(FILETYPE): $(BUILDDIR) $(OBJS) libz.a \
		libg.a makefile
	$(MAKE_MONBUILT)
	$(LINK) -O0 -e g_pfnVectors -nostartfiles -mcpu=cortex-m3 \
		-Wl,--gc-sections $(OBJS) monbuilt.o libz.a libg.a 
	$(MAKE_BINARY)
	$(MAKE_GNUSYMS)

ramtst:
	@echo "There is no ramtst image for this port"

include $(TOPDIR)/target/make/rules.make

##############################################################################
#
# Miscellaneous...
cscope_local:
	ls *.h >cscope.files
	find $(SOCDIR) -name '*.[ch]' >>cscope.files

clobber_local:
	rm -f stripped.elf 

clean_local:

help_local:

varcheck:

reset:
	-jlink jlink_reset.scr

flash:
	-jlink jlink_flash.scr

flashx:
	moncmd $(UCONSYSIP) "SCRCMD: CONNECT close"
	-jlink jlink_flash.scr
	sleep 2
	moncmd $(UCONSYSIP) "SCRCMD: CONNECT comport $(VCOMPORT) 19200"

