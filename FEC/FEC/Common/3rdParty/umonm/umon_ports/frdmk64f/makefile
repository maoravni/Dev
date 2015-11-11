###############################################################################
#
# K64F Freedom (Freescale's ARM Cortex M4) uMon makefile
#
# This port was derived from picking apart a few different demo
# applications that came with the Kinetis SDK. The following files
# in this directory come directly out of the Freescale-supplied SDK:
#
#	board.h gpio_pins.c gpio_pins.h hardware_init.c pin_mux.c
#	pin_mux.h K64FN1Mxxx12_flash.ld K64FN1Mxxx12_ram.ld
#
# Prerequisites:
# CodeSourcery Lite is the toolset; using TOOL_PREFIX (below) as the path.
# Install the OpenSDAv2 bootloader from SEGGER onto the FRDM-K64F board...
# http://www.segger.com/opensda.html
#
# The interface to this board uses the OpenSDAv2 debugger built on the 
# board.  This eliminates the need for any cabled device to do basic
# flash updates.  Since this makefile uses JLink.exe, it is assumed that
# the Segger OpenSDAv2 bootloader is installed on the FRDM-K64F.
#
# Flash info for this device (FTFx_KX_1024K_0K_4K_4K_0K):
#   1024K P-Flash, 0K FlexNVM, 4K FlexRam,
#   4K P-Flash sector size, 0K D-Flash sector size
#
# Set BUILDTYPE below for either a uMonm or a uuMon (much smaller) build.
#

JLINK			= "C:/Program Files (x86)/SEGGER/JLinkARM_V484f/JLink.exe"

PLATFORM		= K64F_FRDM
TOPDIR			= $(UMONTOP)
TGTDIR			= frdmk64f
CPUTYPE			= arm
SOCDIR			= $(UMONTOP)/target/soc/kinetis
FILETYPE		= elf
#BUILDTYPE		= -D BUILD_FOR_UUMON=1
BUILDTYPE		= -D BUILD_FOR_UMONM=1
CUSTOM_INCLUDE	= -I $(SOCDIR)
CUSTOM_AFLAGS	= -D __NO_INIT_DATA_BSS $(BUILDTYPE)
CUSTOM_CFLAGS	= -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -fsigned-char \
				  -mfpu=fpv4-sp-d16 -O2 -fno-toplevel-reorder \
				  -D FRDM_K64F120M -D CPU_MK64FN1M0VMD12 -D USE_16BYTE_ERASE \
				  -I $(SOCDIR)/device $(BUILDTYPE)


LFLAGS			= -mcpu=cortex-m4 -nostartfiles -mthumb \
				  -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
				  -ffunction-sections -fdata-sections -Wl,--gc-sections


# It is assumed that the toolset is in the PATH, if not then just
# insert the full path here...
TOOL_PREFIX	= arm-none-eabi

include	$(TOPDIR)/target/make/common.make

LD = $(CC)

# Build each variable from a list of individual filenames...
#
LOCSSRC		= 
CPUSSRC		= 
LOCCSRC		= cpuio.c etherdev.c gpio_pins.c hardware_init.c led.c pin_mux.c


SOCCSRC		= $(SOCDIR)/system_MK64F12.c $(SOCDIR)/fsl_gpio_hal.c \
			  $(SOCDIR)/fsl_mcg_hal.c $(SOCDIR)/fsl_sim_hal.c \
			  $(SOCDIR)/fsl_sim_clock_module_names_K64F12.c \
			  $(SOCDIR)/fsl_clock_manager.c \
			  $(SOCDIR)/hal/enet/fsl_enet_hal.c \
			  $(SOCDIR)/k64f_flash.c 


# Needed if USE_UART_HAL is defined in cpuio.c:
#			  $(SOCDIR)/fsl_debug_uart.c \
#			  $(SOCDIR)/fsl_uart_hal_common_configurations.c \
#			  $(SOCDIR)/fsl_uart_hal_transfer_functions.c \
#			  $(SOCDIR)/fsl_uart_hal_status_flags.c 

SOCSSRC		= $(SOCDIR)/startup_MK64F12.S

COMCSRC		= arp.c cache.c chario.c cmdtbl.c docmd.c dhcp_00.c \
			  dhcpboot.c edit.c ee.c env.c ethernet.c flash.c flashenv.c \
			  genlib.c icmp.c ledit_vt100.c if.c mprintf.c memcmds.c \
			  malloc.c ipv6.c moncom.c misccmds.c misc.c start.c syslog.c \
			  tcpstuff.c timestuff.c tfs.c tfsapi.c tfsclean4.c \
			  tfscli.c tftp.c xmodem.c

CPUCSRC		= defaulthandler.c
IODEVSRC	= 
FLASHSRC	= 


include $(TOPDIR)/target/make/objects.make

OBJS	= 	$(SOCSOBJ) $(LOCCOBJ) $(LOCSOBJ) $(SOCCOBJ) $(CPUSOBJ) $(CPUCOBJ) \
		    $(COMCOBJ) $(FLASHOBJ) $(IODEVOBJ)


##############################################################################
#
# Targets...
# For the Cortex-MX boards there is only a boot target and the memory map
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

$(BUILDDIR)/boot.$(FILETYPE): $(BUILDDIR) mksubdirs $(OBJS) libg.a makefile
	$(MAKE_MONBUILT)
	$(LINK) -e Reset_Handler $(LFLAGS) $(OBJS) monbuilt.o libg.a
	$(MAKE_BINARY)
	$(MAKE_GNUSYMS)

ramtst:
	@echo "There is no ramtst image for this port"

include $(TOPDIR)/target/make/rules.make

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
MONCOMPTR = 0x000004fe
TARGET_IP = 135.222.138.20
MAPFILE = demo_map.ld

demo:
	$(CC) $(CFLAGS) -DMONCOMPTR=$(MONCOMPTR) demo_crt0.S demo_main.c demo_monlib.c
	$(LD) -e start -O0 -nostartfiles -mcpu=cortex-m3 -Wl,--gc-sections -Wl,-T$(MAPFILE) -mthumb -g \
		-o demo.elf demo_crt0.o demo_main.o demo_monlib.o
	$(STRIP) demo.elf
	$(OBJCOPY) --output-target=binary --gap-fill 0xff demo.elf demo.bin

# demodld:
# This assumes that the flash at 0x38000 is erased.  If not, use the "flash erase"
# command to erase the appropriate sectors on target... 
# Once installed, just run "call 0x38000" to go.
demodld:
	moncmd $(TARGET_IP) "flash erase 56"
	ttftp $(TARGET_IP) put demo.bin 0x38000


	
##############################################################################
#
# uumon...
UUOBJS = uu_arp.o uu_chario.o uu_cmdtbl.o uu_docmd.c uu_flash.o uu_main.o \
		 uu_phy.o uu_timestuff.o uu_bootp.o uu_cmds.o uu_cpuio.o uu_enetcore.o \
		 uu_genlib.o uu_mprintf.o uu_tftp.o uu_xmodem.c

uumon:	$(UUOBJS)


##############################################################################
#
# Miscellaneous...
cscope_local:
	find $(SOCDIR) -name '*.h' >cscope.files
	

clobber_local:
	rm -f stripped.elf  demo.elf demo.bin 

clean_local:
	rm -rf hal

help_local:

varcheck:

mksubdirs:
	mkdir -p hal/enet

# Rules that use the OpenSDAv2 (Segger) interface...
reset:
	-$(JLINK) jlink_reset.scr

flash:
	-$(JLINK) jlink_flash.scr
