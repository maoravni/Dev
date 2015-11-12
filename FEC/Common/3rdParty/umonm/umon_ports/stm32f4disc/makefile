###############################################################################
#
# STM32F4 Discovery (ST's ARM Cortex M4) uMon makefile
# This port assumes the DM-STF4BB base board from Embest
# (www.armkits.com/product/DM-STF4BB.asp)
#
# Prerequisites:
# CodeSourcery Lite is the toolset; using TOOL_PREFIX (below) as the path.
# 
# The Discovery board has a built-in ST-Link debug interface, so some of
# the rules (reset, flash, flashx, etc...) in this makefile use that.
#

UCONSYSIP		= 192.168.1.2
ST_LINK			= "C:/Program Files (x86)/STMicroelectronics/STM32 ST-LINK Utility/ST-LINK Utility/ST-LINK_CLI.exe"

PLATFORM		= STM32F4_DISCOVERY
TOPDIR			= $(UMONTOP)
TGTDIR			= lm3s6918
CPUTYPE			= arm
SOCDIR			= $(UMONTOP)/target/soc/stm32f4xx
FILETYPE		= elf
CUSTOM_INCLUDE	= -I $(SOCDIR)
CUSTOM_CFLAGS	= -mcpu=cortex-m4 -O0 -mthumb -ffunction-sections \
				  --std=gnu99 -fms-extensions \
				  -DSTM32F407VG -DSTM32F4XX -DUSE_STDPERIPH_DRIVER \
				  -D__ASSEMBLY__  -D VECT_TAB_SRAM

LFLAGS			= -O0 -nostartfiles -mcpu=cortex-m4 -Wl,--gc-sections \
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
LOCCSRC		= cpuio.c etherdev.c stm32f4x7_eth_bsp.c

SOCCSRC		= $(SOCDIR)/stm32f4xx_flash.c $(SOCDIR)/stm32f4xx_gpio.c \
			  $(SOCDIR)/stm32f4xx_pwr.c $(SOCDIR)/stm32f4xx_rcc.c \
			  $(SOCDIR)/stm32f4xx_syscfg.c $(SOCDIR)/stm32f4xx_usart.c \
			  $(SOCDIR)/system_stm32f4xx.c $(SOCDIR)/startup_stm32f4xx.c \
			  $(SOCDIR)/stm32f4xx_misc.c $(SOCDIR)/stm32f4x7_eth.c

COMCSRC		= arp.c cache.c chario.c cmdtbl.c docmd.c dhcp_00.c \
			  dhcpboot.c edit.c ee.c env.c ethernet.c flash.c genlib.c \
			  icmp.c ledit_vt100.c if.c mprintf.c memcmds.c malloc.c \
			  ipv6.c moncom.c misccmds.c misc.c start.c syslog.c \
			  tcpstuff.c timestuff.c tfs.c tfsapi.c tfsclean4.c \
			  tfscli.c tftp.c xmodem.c

CPUCSRC		= 
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
	rm -f stripped.elf 

clean_local:

help_local:

varcheck:

reset:
	-$(ST_LINK) -c SWD UR -Rst

# flash:
# Erase, program then reset...
flash:
	-$(ST_LINK) -c SWD UR -Q -w32 -SE 0 4 -P $(BUILDDIR)/boot.bin 0x08000000 -Rst

eraseall:
	-$(ST_LINK) -c SWD UR -Q -w32 -ME -P $(BUILDDIR)/boot.bin 0x08000000 -Rst

