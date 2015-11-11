###############################################################################
#
# SimpleCortex (NXP's LCP1769 ARM Cortex M3) uMon makefile
#
# Prerequisites:
# CodeSourcery Lite is the toolset; using TOOL_PREFIX (below) as the path.
#
# Notes:
# This is my first attempt at putting uMon on a smaller memory footprint
# system like the LPC1769, so this text documents some of the changes that
# I had to make to support that:
# - Arm-Thumb requires that hard-coded function entrypoints have the LSB
#   set to 1, so I added the ADJUST_THUMB_ADDRESS(a) macro to key points
#	in the code.  When running in thumb mode, its important to note that
#	both the compile and link steps need to be done in THUMB mode (still
#	learning about this).
#
# - The flash on the LPC1769 has internal ECC done on every 16-byte block,
#	so unlike normal NOR flash that allows the user to clear ANY single bit,
#	this device (and probably others in this family) only allows each 16-byte
#	block to be written once, EVEN IF ALL YOU WANT TO DO IS CLEAR A BIT!
#	As a result, I had to change some fundamentals in TFS so that the header
#	flags can be modified (to delete a file).
#
# - To reduce the use of RAM, there is no loader in TFS.  Images are placed
#	directly in flash and are built to run-in place (XIP); so only data
#   and bss need RAM.  To execute these applications, the code is placed
#	in flash, and a "call 0xNNNN" is used to start them.  As a result, the
#	flash is broken up into three distinct areas: boot, TFS, application.
#	
# - The typical uMon model is that there is enough RAM at APPRAMBASE that
#	a file can be transferred to RAM, then the flash-write is done from
#	ram to flash.  Can't do this with these little processors; there's just
#	not enough ram.  So to transfer a large file to TFS, uMON-M now writes
#	the data directly to the location in TFS where tfsadd() would place it
#	(using a new function called tfsnxtdataptr).  Then when tfsadd() is
#	called, it will detect the fact that the data is already in place and
# 	simply insert the header.
#
# - Powerhit safe defragmentation works (when there is no interrupt),
#	but has not been tested.
#
# As of this writing, THIS IS A WORK IN PROGRESS.  It is solid enough to 
# start playing with; but still needs testing and tweeks. 
#

######
#
# Some build details:
# To build the monitor, type: "make rebuild".
#
# To flash the monitor, you need to have installed the CoIDE tools and
# have the CoIDE debug cable (usb) plugged in.  Then "make flash" will
# use the CoIDE tools to burn the image to the board.
#
# To build the simplest example application type: "make demo".
#
# To install this, set the board up on ipaddress 192.168.1.2 (or whatever)
# and then run "make TARGET_IP=192.168.1.2 demodld"
#
# To set the board ip address quickly, just do this at the uMON prompt:
# "set IPADD 192.168.1.2; ether on"
#
# To run the demo (now installed at location 0x38000 in flash) just type
# "call 0x38000" at the uMON>prompt.  This will just print "hello world"
# (or something like that).
#
# 
VCOMPORT		= 12
UCONSYSIP		= 192.168.1.2
COFLASH			= C:/CooCox/CoIDE/bin/coflash.exe
COFLASHDRVR		= C:/CooCox/CoIDE/flash/LPC17xx_512.elf

PLATFORM		= SIMPLECORTEX
TOPDIR			= $(UMONTOP)
TGTDIR			= simplecortex
CPUTYPE			= arm
SOCDIR			= $(UMONTOP)/target/soc/lpc17xx
FILETYPE		= elf
CUSTOM_INCLUDE	= -I $(SOCDIR)
CUSTOM_CFLAGS	= -mcpu=cortex-m3 -O0 -mthumb -ffunction-sections -DLPC1769
LFLAGS			= -O0 -nostartfiles -mcpu=cortex-m3 -Wl,--gc-sections \
				  -Wl,-Tlink.ld 

# This port can be built to use either VCOM (the USB-host port)
# or UART0 on P0.2/P0.3 as the console.
#
# For UART0, use the USB-to-serial debug cable and connect
#     BLACK-to-GND, GRAY-to-P0.2 and WHITE-to-P0.3
# or (if it is a 6-pin header)...
#	  PIN1-to-GND, PIN5-to-P0.2 and PIN4-to-P0.3
#
# When changing between VCOM and UART0 in this makefile, do a rebuild.

#CONSOLE = VCOM
CONSOLE = UART0
CUSTOM_CFLAGS	+= -D CONSOLE_IS_$(CONSOLE)=1
ifeq ($(CONSOLE),VCOM)
CONSOLE_FILES = $(SOCDIR)/USBVCOM.c $(SOCDIR)/usbcontrol.c \
			    $(SOCDIR)/usbhw_lpc.c $(SOCDIR)/usbinit.c \
				$(SOCDIR)/usbstdreq.c $(SOCDIR)/vcom.c
endif

# It is assumed that the toolset is in the PATH, if not then just
# insert the full path here...
TOOL_PREFIX	= arm-none-eabi

include	$(TOPDIR)/target/make/common.make

LD = $(CC)

# Build each variable from a list of individual filenames...
#
LOCSSRC		= 
CPUSSRC		= 
LOCCSRC		= cpuio.c etherdev.c target.c

SOCCSRC		= $(SOCDIR)/lpc17xx_clkpwr.c \
			  $(SOCDIR)/lpc17xx_emac.c $(SOCDIR)/lpc17xx_gpio.c \
			  $(SOCDIR)/lpc17xx_libcfg_default.c \
			  $(SOCDIR)/lpc17xx_rit.c $(SOCDIR)/lpc17xx_systick.c \
			  $(SOCDIR)/lpc17xx_flash.c $(SOCDIR)/serial_fifo.c \
			  $(SOCDIR)/startup_LPC17xx.c $(SOCDIR)/system_LPC17xx.c \
			  $(CONSOLE_FILES)

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
# For the Cortex-M boards there is only a boot target and the memory map
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
	rm -f stripped.elf demo*.o demo*.bin demoapp*

clean_local:

help_local:

varcheck:

flash:
	$(COFLASH) program LPC1769 build_SIMPLECORTEX/boot.bin --adapter-name=ColinkEx --port=SWD --adapter-clk=1000000 --erase=affected --driver=$(COFLASHDRVR)

flashx:
	moncmd $(UCONSYSIP) "SCRCMD: CONNECT close"
	$(COFLASH) program LPC1769 build_SIMPLECORTEX/boot.bin --adapter-name=ColinkEx --port=SWD --adapter-clk=1000000 --erase=affected --driver=$(COFLASHDRVR)
	sleep 2
	moncmd $(UCONSYSIP) "SCRCMD: CONNECT comport $(VCOMPORT) 19200"
	

##############################################################################
#
# demo-app stuff:
# This code is usually part of the umon_apps; however since the simplecortex
# requires a VERY minimal demo application, it seemed best to just put it with the port.
#
# Note regarding MONCOMPTR:
# The value of MONCOMPTR used below depends on how the monitor was built.
# To determine the correct value, type "help -i" at the uMON> prompt (on target)
# and use the value that follows "Moncomptr: ".
MONCOMPTR = 0x00000122
TARGET_IP = 192.168.1.100 
MAPFILE = demo_map.ld

demo:
	$(CC) $(CFLAGS) -DMONCOMPTR=$(MONCOMPTR) demo_crt0.S demo_main.c demo_monlib.c
	$(LD) -e start -O0 -nostartfiles -mcpu=cortex-m3 -Wl,--gc-sections -Wl,-T$(MAPFILE) -mthumb -g \
		-o demo demo_crt0.o demo_main.o demo_monlib.o
	$(STRIP) demo
	$(OBJCOPY) --output-target=binary --gap-fill 0xff demo demo.bin

# demodld:
# This assumes that the flash at 0x38000 is erased.  If not, use the "flash erase"
# command to erase the appropriate sectors on target... 
demodld:
	ttftp $(TARGET_IP) put demo.bin 0x38000
	
	
# Environment setup:
# (this could be done by bashrc)
# cut-n-paste at startup:
# export PATH="/cygdrive/c/Program Files (x86)/GNU Tools ARM Embedded/4.7 2013q1/bin":$PATH
# export GCC_EXEC_PREFIX=
