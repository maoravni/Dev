#
# Makefile for building Lua to run on Cortex-M systems.
# Note that this takes a bit more heap that would typically be allocated
# to uMon at configuation time.
# I found that an ALLOCSIZE (in config.h) of 64K or more is needed for this.
# 
# Note1: Not heavily tested; but appears to run Lua scripts in TFS just fine.
# Note2: as of this writing (Jun 2014), this has only been tested on the
# FRDMK64F board (with uMon's ALLOCSIZE set to 64*1024 in config.h).
#

# Two different versions of LUA (different builds have differernt sets
# of files):
#LUABASE=lua-5.1.4
LUABASE=lua-5.2.3

#BOARD=SIMPLECORTEX
#BOARD=STM32F4DISCOVERY
BOARD=FRDMK64F

ifeq ($(BOARD),SIMPLECORTEX)
MONCOMPTR	= 0x00000122
RAMBASE		= 0x2007c000
ROMBASE		= 0x00038000
RAMLEN		= 0x8000
ROMLEN		= 0x18000
ERASERNG	= 
BOARDFLAGS	= -DLPC1769
endif

ifeq ($(BOARD),STM32F4DISCOVERY)
MONCOMPTR	= 0x080001fe
RAMBASE		= 0x20008000
ROMBASE		= 0x08040000
RAMLEN		= 0x8000
ROMLEN		= 0x18000
ERASERNG	= 6
BOARDFLAGS	=
endif

ifeq ($(BOARD),FRDMK64F)
MONCOMPTR	= 0x000004fe
RAMBASE		= 0x2000b000
ROMBASE		= 0x00038000
RAMLEN		= 0x10000
ROMLEN		= 0x40000
ERASERNG	= 56-104
BOARDFLAGS	=
endif


#########################################################################
#
#  Nothing below this point should have to be changed.
#

TARGET_IP	= $(shell echo $$TARGET_IP)
ENDIANNESS	= -D BYTE_ORDER=LITTLE_ENDIAN

APPNAME		= lua
LWIPDIR		= ./lwip
NM			= $(TOOL_PREFIX)-nm
AR			= $(TOOL_PREFIX)-ar
LD			= $(TOOL_PREFIX)-gcc
ASM			= $(TOOL_PREFIX)-as
CC			= $(TOOL_PREFIX)-gcc
STRIP		= $(TOOL_PREFIX)-strip
OBJCOPY		= $(TOOL_PREFIX)-objcopy
OBJDUMP		= $(TOOL_PREFIX)-objdump
UMONCOM		= ../../umon_main/target/common
INCPATHS	= -I . -I ./net -I $(LWIPDIR)/src/include/ipv4 \
			  -I $(LWIPDIR)/src/include -I $(LWIPDIR)/src/include/netif \
			  -I $(LWIPDIR)/src/netif/ppp -idirafter $(UMONCOM)


TOOL_PREFIX = arm-none-eabi

CFLAGS		= -g -c -Wall -fno-builtin -fno-toplevel-reorder \
			  -mcpu=cortex-m3 -O0 -mthumb -ffunction-sections \
			  $(BOARDFLAGS) $(INCPATHS) $(ENDIANNESS)



ifeq ($(LUABASE),lua-5.1.4)
LUALIB= lapi.o lcode.o ldebug.o ldo.o ldump.o lfunc.o lgc.o llex.o lmem.o \
	lobject.o lopcodes.o lparser.o lstate.o lstring.o ltable.o ltm.o lundump.o \
	lvm.o lzio.o lauxlib.o lbaselib.o ldblib.o liolib.o lmathlib.o loslib.o \
	ltablib.o lstrlib.o loadlib.o linit.o lua.o
endif

ifeq ($(LUABASE),lua-5.2.3)
LUALIB= lapi.o lbitlib.o lcode.o lcorolib.o lctype.o ldebug.o ldo.o ldump.o \
	lfunc.o lgc.o llex.o lmem.o \
	lobject.o lopcodes.o lparser.o lstate.o lstring.o ltable.o ltm.o lundump.o \
	lvm.o lzio.o lauxlib.o lbaselib.o ldblib.o liolib.o lmathlib.o loslib.o \
	ltablib.o lstrlib.o loadlib.o linit.o lua.o
endif

OBJS=$(CRT0) main.o monlib.o $(LUALIB) newlib_hooks.o

#####
#
# $(APPNAME):
# Top level target builds the application.
#
$(APPNAME): $(OBJS) $(LUALIB) setup makefile
	echo tools: $(TOOL_PREFIX)
	$(CC) -e Cstart -o $(APPNAME) -nostartfiles -Wl,-Tlink.ld -O0 \
	  -mcpu=cortex-m3 -mthumb -Wl,--gc-sections $(OBJS) -lm
	$(OBJDUMP) --source --disassemble $(APPNAME) >$(APPNAME).dis
	$(OBJCOPY) --output-target=binary --gap-fill 0xff $(APPNAME) $(APPNAME).bin

dis:
	$(OBJDUMP) --source --disassemble $(APPNAME) > $(APPNAME).dis

#####
#
# setup:
# Create files that are specific to the memory map.
# As of this writing, that includes...
#
#	link.ld:	linker file 
#	srvr:		start script.
#
setup:
	sed -e 's/RAMBASE/$(RAMBASE)/g' link.ld0 > link.ld1
	sed -e 's/RAMLEN/$(RAMLEN)/g' link.ld1 > link.ld2 
	sed -e 's/ROMBASE/$(ROMBASE)/g' link.ld2 > link.ld3 
	sed -e 's/ROMLEN/$(ROMLEN)/g' link.ld3 > link.ld
	rm link.ld[1-3]


#####
#
# Objects:
#
crt0_68k.o: crt0_68k.S
	$(CC) $(CFLAGS) -o $@ crt0_68k.S

crt0_arm.o: crt0_arm.S
	$(CC) $(CFLAGS) -o $@ crt0_arm.S

crt0_bfin.o: crt0_bfin.S
	$(CC) $(CFLAGS) -o $@ crt0_bfin.S

crt0_mips.o: crt0_mips.S
	$(CC) $(CFLAGS) -o $@ crt0_mips.S

crt0_mb.o: crt0_mb.S
	$(CC) $(CFLAGS) -o $@ crt0_mb.S

crt0_ppc.o: crt0_ppc.S
	$(CC) $(CFLAGS) -o $@ crt0_ppc.S

crt0_sh2.o: crt0_sh2.S
	$(CC) $(CFLAGS) -o $@ crt0_sh2.S

main.o: main.c 
	$(CC) $(CFLAGS) -D MONCOMPTR=$(MONCOMPTR) -o $@ main.c

mprintf.o: mprintf.c
	$(CC) $(CFLAGS) -o $@ mprintf.c

newlib_hooks.o: newlib_hooks.c
	$(CC) $(CFLAGS) -Wno-missing-noreturn -o $@ newlib_hooks.c

monlib.o: monlib.c
	$(CC) $(CFLAGS) -o $@ monlib.c

strace.o: strace.c 
	$(CC) $(CFLAGS) $(CPU) -o $@ strace.c


##############################################################
#
# LUA-specific objects:
#
LUASRC=$(LUABASE)/src
LUATST=$(LUABASE)/test
LUACFLAGS=$(CFLAGS) -DLUA_ANSI -I$(LUASRC)

lua.o:	$(LUASRC)/lua.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lua.c

lapi.o:		$(LUASRC)/lapi.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lapi.c

lbitlib.o:		$(LUASRC)/lbitlib.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lbitlib.c

lcode.o:	$(LUASRC)/lcode.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lcode.c

lcorolib.o:	$(LUASRC)/lcorolib.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lcorolib.c

lctype.o:	$(LUASRC)/lctype.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lctype.c

ldebug.o:	$(LUASRC)/ldebug.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/ldebug.c

ldo.o:		$(LUASRC)/ldo.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/ldo.c

ldump.o:	$(LUASRC)/ldump.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/ldump.c

lfunc.o:	$(LUASRC)/lfunc.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lfunc.c

lgc.o:		$(LUASRC)/lgc.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lgc.c

llex.o:		$(LUASRC)/llex.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/llex.c

lmem.o:		$(LUASRC)/lmem.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lmem.c

lobject.o:	$(LUASRC)/lobject.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lobject.c

lopcodes.o:	$(LUASRC)/lopcodes.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lopcodes.c

lparser.o:	$(LUASRC)/lparser.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lparser.c

lstate.o:	$(LUASRC)/lstate.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lstate.c

lstring.o:	$(LUASRC)/lstring.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lstring.c

ltable.o:	$(LUASRC)/ltable.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/ltable.c

ltm.o:		$(LUASRC)/ltm.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/ltm.c

lundump.o:	$(LUASRC)/lundump.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lundump.c

lvm.o:		$(LUASRC)/lvm.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lvm.c

lzio.o:		$(LUASRC)/lzio.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lzio.c

lauxlib.o:	$(LUASRC)/lauxlib.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lauxlib.c

lbaselib.o:	$(LUASRC)/lbaselib.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lbaselib.c

ldblib.o:	$(LUASRC)/ldblib.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/ldblib.c

liolib.o:	$(LUASRC)/liolib.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/liolib.c

lmathlib.o:	$(LUASRC)/lmathlib.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lmathlib.c

loslib.o:	$(LUASRC)/loslib.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/loslib.c

ltablib.o:	$(LUASRC)/ltablib.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/ltablib.c

lstrlib.o:	$(LUASRC)/lstrlib.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/lstrlib.c

loadlib.o:	$(LUASRC)/loadlib.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/loadlib.c

linit.o:	$(LUASRC)/linit.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/linit.c

print.o:	$(LUASRC)/print.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/print.c

luac.o:	$(LUASRC)/luac.c
	$(CC) $(LUACFLAGS) -o $@ $(LUASRC)/luac.c


#
# End LUA-specific objects
#
##############################################################

#####
#
# clean:
# Remove all files created by this make.
#
clean:
	rm -f *.o $(APPNAME) $(APPNAME).bin $(APPNAME).sym $(APPNAME).dis
	rm -f cscope* tags make.out

cscope:
	find . -name '*.[ch]' | grep -v min.c >cscope.files

ctags: cscope
	ctags --file-tags=yes -n -L cscope.files

#####
#
# sym:
# Create and download the symbol table file that can be used by uMon
# with this application...
#
sym: targetipcheck
	@if ! test -f $(APPNAME).sym; then echo Must build $(APPNAME) first; exit 1; fi
	monsym -p0x $(APPNAME).sym >symtbl
	ttftp $(TARGET_IP) put symtbl


	
#####
#
# dld:
# Use the ttftp tool (supplied with MicroMonitor) to download the
# application to the target.
#
dld:
	moncmd -q -w3 $(TARGET_IP) "reset -x"
	moncmd -q -w2 $(TARGET_IP) "flash erase $(ERASERNG)"
	ttftp $(TARGET_IP) put $(APPNAME).bin $(ROMBASE)

test:
	ttftp $(TARGET_IP) put $(LUATST)/bisect.lua bisect.lua
	ttftp $(TARGET_IP) put $(LUATST)/cf.lua cf.lua
	ttftp $(TARGET_IP) put $(LUATST)/echo.lua echo.lua
	ttftp $(TARGET_IP) put $(LUATST)/env.lua env.lua
	ttftp $(TARGET_IP) put $(LUATST)/factorial.lua factorial.lua
	ttftp $(TARGET_IP) put $(LUATST)/fib.lua fib.lua
	ttftp $(TARGET_IP) put $(LUATST)/fibfor.lua fibfor.lua
	ttftp $(TARGET_IP) put $(LUATST)/globals.lua globals.lua
	ttftp $(TARGET_IP) put $(LUATST)/hello.lua hello.lua
	ttftp $(TARGET_IP) put $(LUATST)/life.lua life.lua
	ttftp $(TARGET_IP) put $(LUATST)/luac.lua luac.lua
	ttftp $(TARGET_IP) put $(LUATST)/printf.lua printf.lua
	ttftp $(TARGET_IP) put $(LUATST)/readonly.lua readonly.lua
	ttftp $(TARGET_IP) put $(LUATST)/sieve.lua sieve.lua
	ttftp $(TARGET_IP) put $(LUATST)/sort.lua sort.lua
	ttftp $(TARGET_IP) put $(LUATST)/table.lua table.lua
	ttftp $(TARGET_IP) put $(LUATST)/trace-calls.lua trace_calls.lua
	ttftp $(TARGET_IP) put $(LUATST)/trace-globals.lua trace_gloabals.lua
	ttftp $(TARGET_IP) put $(LUATST)/xd.lua xd.lua

#####
#
# zdld:
# Compress the elf file using the 'elf' tool (supplied with MicroMonitor)
# The output of this is "$(APPNAME).ezip", then download that compressed file.
#
zdld: targetipcheck 
	@if ! test -f $(APPNAME); then echo Must build $(APPNAME) first; exit 1; fi
	elf -z6 $(APPNAME)
	ttftp $(TARGET_IP) put $(APPNAME).ezip $(APPNAME),Ec
