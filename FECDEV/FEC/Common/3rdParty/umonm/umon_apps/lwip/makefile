#########################################################################
# 
#  uMon/LWIP demonstration application.
#  Simple application that demonstrates how a generic uMon application
#  can run an HTTP server that uses LWIP and TFS.
# 
#  In the uMon-M environment this application is installed at an address
#  in flash so that it can run XIP (execute in place).
#  It is not a file in TFS, it is raw binary in flash space outside of
#  TFS allocated area.  It is executed with the command "call XXXXX"
#  (where 0xXXXXX is the base address at which it is installed).
#  This can be put in a startup script for autoboot.
#
#  Similar to other uMon demo applications, the user must provide
#  a few target-specific parameters.  The 'setup' rule below creates
#  a few files needed by the build process.  It assumes that 
#  The values RAMBASE,ROMBASE,RAMLEN,ROMLEN and MONCOMPTR have been
#  defined (see below).
#
#  Some of the make rules assume that the user has installed the
#  standard set of uMon host-based tools.  Things like 'ttftp' and
#  'moncmd' are assumed to be in the path.
#
#  To build and install set the environment variable 'TARGET_IP'
#  to your board's IP address and run the following commands:
#
#	- make clean	<< clean up old files
#	- make			<< build the application and target-specific setup 
#	- make html		<< install the web files in TFS
#	- make dld		<< install the executable to flash
#
#  Once on the target, use the "call" command to execute the
#  command:
#
#	call -A 0xXXXXX srvrs
#
#  The file 'srvr' in this directory can also be installed as a
#  shortcut for this...
#
#	ttftp N.N.N.N put srvr srvr,eB
#
#  Refer to main.c for more information regarding command line
#  arguments.  
#
#  The webpages installed here demonstrate HTTP/WebSockets application
#  and have been tested (briefly) on IE10, Chrome 28 and Firefox.

#BOARD=X1AP
#BOARD=SIMPLECORTEX
#BOARD=STM32F4DISCOVERY
BOARD=FRDMK64F

# These values are target specific, but we have a few pre-configured sets...
ifeq ($(BOARD),X1AP)
MONCOMPTR	= 0x0000016e
RAMBASE		= 0x20007000
ROMBASE		= 0x00020000
RAMLEN		= 0x8000
ROMLEN		= 0x18000
ERASERNG	= 128-223
BOARDFLAGS	=
endif

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
MONCOMPTR	= 0x00000502
RAMBASE		= 0x2000b000
ROMBASE		= 0x00038000
RAMLEN		= 0x8000
ROMLEN		= 0x18000
ERASERNG	= 56-70
BOARDFLAGS	=
endif



#########################################################################
#
#  Nothing below this point should have to be changed.
#

TARGET_IP	= $(shell echo $$TARGET_IP)
ENDIANNESS	= -D BYTE_ORDER=LITTLE_ENDIAN

APPNAME		= lwipapp
LWIPDIR		= ./lwip
NM			= $(TOOL_PREFIX)-nm
AR			= $(TOOL_PREFIX)-ar
LD			= $(TOOL_PREFIX)-ld
ASM			= $(TOOL_PREFIX)-as
CC			= $(TOOL_PREFIX)-gcc
STRIP		= $(TOOL_PREFIX)-strip
OBJCOPY		= $(TOOL_PREFIX)-objcopy
OBJDUMP		= $(TOOL_PREFIX)-objdump
UMONCOM		= ../../umon_main/target/common
INCPATHS	= -I . -I ./net -I $(LWIPDIR)/src/include/ipv4 \
			  -I $(LWIPDIR)/src/include -I $(LWIPDIR)/src/include/netif \
			  -I $(LWIPDIR)/src/netif/ppp -I $(UMONCOM)


TOOL_PREFIX = arm-none-eabi

CFLAGS		= -g -c -Wall -fno-builtin -fno-toplevel-reorder \
			  -mcpu=cortex-m3 -O0 -mthumb -ffunction-sections \
			  $(BOARDFLAGS) $(INCPATHS) $(ENDIANNESS)

BASE_OBJS=main.o console.o monlib.o

#
#########################################################################
#
NET_OBJS	= base64.o fs.o httpd.o monsrvr.o sha1.o \
			  umonlwip.o websocd.o

API_OBJS	= api_lib.o api_msg.o err.o netbuf.o netdb.o netifapi.o

CORE_OBJS	= dhcp.o dhcp.o dns.o init.o mem.o memp.o netif.o pbuf.o \
			  raw.o stats.o sys.o tcp.o tcp_in.o tcp_out.o udp.o

IPV4_OBJS	= autoip.o icmp.o igmp.o inet.o inet_chksum.o ip.o \
			  ip_addr.o ip_frag.o

SNMP_OBJS	= asn1_dec.o asn1_enc.o mib2.o mib_structs.o msg_in.o \
			  msg_out.o

NETIF_OBJS	= etharp.o

PPP_OBJS	= auth.o chap.o chpms.o fsm.o ipcp.o lcp.o magic.o md5.o \
			  pap.o ppp.o ppp_oe.o randm.o vj.o

LWIP_OBJS	= $(API_OBJS) $(CORE_OBJS) $(IPV4_OBJS) $(NETIF_OBJS)
OBJS		= $(BASE_OBJS) $(LWIP_OBJS) $(NET_OBJS) 

#####
#
# $(APPNAME):
# Top level target builds the application.
#
$(APPNAME): $(OBJS) makefile setup
	$(CC) -e Cstart -o $(APPNAME) -nostartfiles -Wl,-Tlink.ld -O0 \
	  -mcpu=cortex-m3 -mthumb -Wl,--gc-sections $(OBJS) 
	$(OBJDUMP) --source --disassemble $(APPNAME) >$(APPNAME).dis
	$(OBJCOPY) --output-target=binary --gap-fill 0xff $(APPNAME) $(APPNAME).bin

play:
	$(NM) --numeric-sort $(APPNAME) >$(APPNAME).sym
	$(NM) --print-size --size-sort $(APPNAME) | grep "^2" >$(APPNAME).symsz
	$(STRIP) $(APPNAME) 


#####
#
# Objects:
#
main.o: main.c 
	$(CC) $(CFLAGS) -o $@ -D MONCOMPTR=$(MONCOMPTR) \
		main.c

console.o: console.c 
	$(CC) $(CFLAGS) -o $@ console.c

misc.o: misc.c 
	$(CC) $(CFLAGS) -o $@ misc.c

monlib.o: monlib.c
	$(CC) $(CFLAGS) -o $@ monlib.c


# To rebuild depends, add -MM to CFLAGS, then run make.
include depends

#########################################################################
#
# Dependencies:
#
fs.o:	 ./net/fs.c
		 $(CC) $(CFLAGS) ./net/fs.c

websocd.o:	 ./net/websocd.c
		 $(CC) $(CFLAGS) ./net/websocd.c

httpd.o:	 ./net/httpd.c
		 $(CC) $(CFLAGS) ./net/httpd.c

httpget.o:	 ./net/httpget.c
		 $(CC) $(CFLAGS) ./net/httpget.c

telnetc.o:	 ./net/telnetc.c
		 $(CC) $(CFLAGS) ./net/telnetc.c

umonlwip.o:	 ./net/umonlwip.c
		 $(CC) $(CFLAGS) ./net/umonlwip.c

api_lib.o:	 $(LWIPDIR)/src/api/api_lib.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/api/api_lib.c

api_msg.o:	 $(LWIPDIR)/src/api/api_msg.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/api/api_msg.c

err.o:	 $(LWIPDIR)/src/api/err.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/api/err.c

netbuf.o:	 $(LWIPDIR)/src/api/netbuf.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/api/netbuf.c

netdb.o:	 $(LWIPDIR)/src/api/netdb.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/api/netdb.c

netifapi.o:	 $(LWIPDIR)/src/api/netifapi.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/api/netifapi.c

sockets.o:	 $(LWIPDIR)/src/api/sockets.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/api/sockets.c

tcpip.o:	 $(LWIPDIR)/src/api/tcpip.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/api/tcpip.c

dhcp.o:	 $(LWIPDIR)/src/core/dhcp.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/dhcp.c

dns.o:	 $(LWIPDIR)/src/core/dns.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/dns.c

init.o:	 $(LWIPDIR)/src/core/init.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/init.c

autoip.o:	 $(LWIPDIR)/src/core/ipv4/autoip.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/ipv4/autoip.c

icmp.o:	 $(LWIPDIR)/src/core/ipv4/icmp.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/ipv4/icmp.c

igmp.o:	 $(LWIPDIR)/src/core/ipv4/igmp.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/ipv4/igmp.c

inet.o:	 $(LWIPDIR)/src/core/ipv4/inet.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/ipv4/inet.c

inet_chksum.o:	 $(LWIPDIR)/src/core/ipv4/inet_chksum.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/ipv4/inet_chksum.c

ip.o:	 $(LWIPDIR)/src/core/ipv4/ip.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/ipv4/ip.c

ip_addr.o:	 $(LWIPDIR)/src/core/ipv4/ip_addr.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/ipv4/ip_addr.c

ip_frag.o:	 $(LWIPDIR)/src/core/ipv4/ip_frag.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/ipv4/ip_frag.c

mem.o:	 $(LWIPDIR)/src/core/mem.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/mem.c

memp.o:	 $(LWIPDIR)/src/core/memp.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/memp.c

netif.o:	 $(LWIPDIR)/src/core/netif.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/netif.c

pbuf.o:	 $(LWIPDIR)/src/core/pbuf.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/pbuf.c

raw.o:	 $(LWIPDIR)/src/core/raw.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/raw.c

asn1_dec.o:	 $(LWIPDIR)/src/core/snmp/asn1_dec.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/snmp/asn1_dec.c

asn1_enc.o:	 $(LWIPDIR)/src/core/snmp/asn1_enc.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/snmp/asn1_enc.c

mib2.o:	 $(LWIPDIR)/src/core/snmp/mib2.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/snmp/mib2.c

mib_structs.o:	 $(LWIPDIR)/src/core/snmp/mib_structs.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/snmp/mib_structs.c

msg_in.o:	 $(LWIPDIR)/src/core/snmp/msg_in.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/snmp/msg_in.c

msg_out.o:	 $(LWIPDIR)/src/core/snmp/msg_out.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/snmp/msg_out.c

stats.o:	 $(LWIPDIR)/src/core/stats.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/stats.c

sys.o:	 $(LWIPDIR)/src/core/sys.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/sys.c

tcp.o:	 $(LWIPDIR)/src/core/tcp.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/tcp.c

tcp_in.o:	 $(LWIPDIR)/src/core/tcp_in.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/tcp_in.c

tcp_out.o:	 $(LWIPDIR)/src/core/tcp_out.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/tcp_out.c

udp.o:	 $(LWIPDIR)/src/core/udp.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/core/udp.c

etharp.o:	 $(LWIPDIR)/src/netif/etharp.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/netif/etharp.c

ethernetif.o:	 $(LWIPDIR)/src/netif/ethernetif.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/netif/ethernetif.c

loopif.o:	 $(LWIPDIR)/src/netif/loopif.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/netif/loopif.c

auth.o:	 $(LWIPDIR)/src/netif/ppp/auth.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/netif/ppp/auth.c

chap.o:	 $(LWIPDIR)/src/netif/ppp/chap.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/netif/ppp/chap.c

chpms.o:	 $(LWIPDIR)/src/netif/ppp/chpms.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/netif/ppp/chpms.c

fsm.o:	 $(LWIPDIR)/src/netif/ppp/fsm.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/netif/ppp/fsm.c

ipcp.o:	 $(LWIPDIR)/src/netif/ppp/ipcp.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/netif/ppp/ipcp.c

lcp.o:	 $(LWIPDIR)/src/netif/ppp/lcp.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/netif/ppp/lcp.c

magic.o:	 $(LWIPDIR)/src/netif/ppp/magic.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/netif/ppp/magic.c

md5.o:	 $(LWIPDIR)/src/netif/ppp/md5.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/netif/ppp/md5.c

pap.o:	 $(LWIPDIR)/src/netif/ppp/pap.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/netif/ppp/pap.c

ppp.o:	 $(LWIPDIR)/src/netif/ppp/ppp.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/netif/ppp/ppp.c

ppp_oe.o:	 $(LWIPDIR)/src/netif/ppp/ppp_oe.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/netif/ppp/ppp_oe.c

randm.o:	 $(LWIPDIR)/src/netif/ppp/randm.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/netif/ppp/randm.c

vj.o:	 $(LWIPDIR)/src/netif/ppp/vj.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/netif/ppp/vj.c

slipif.o:	 $(LWIPDIR)/src/netif/slipif.c
		 $(CC) $(CFLAGS) $(LWIPDIR)/src/netif/slipif.c

sha1.o:	 ./net/sha1.c
		 $(CC) $(CFLAGS) ./net/sha1.c

base64.o:	 ./net/base64.c
		 $(CC) $(CFLAGS) ./net/base64.c

monsrvr.o:	 ./net/monsrvr.c
		 $(CC) $(CFLAGS) ./net/monsrvr.c


##########################################################################
#
# UTILITIES:
#
rebuild: 
	make clean
	make
	make cscope


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
	sed -e 's/ROMBASE/$(ROMBASE)/g' srvr.0 > srvr

#####
#
# clean:
# Remove all files created by this make.
#
clean:
	find . -name '*.o' -exec rm {} \;
	rm -f $(APPNAME) $(APPNAME).bin $(APPNAME).sym $(APPNAME).dis symtbl
	rm -f cscope.out tags cscope.files link.ld[1-3] link.ld srvr

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

#####
#
# dis:
# Build a disassembly of the image...
#
dis:
	$(OBJDUMP) --source --disassemble $(APPNAME) >$(APPNAME).dis

#####
#
# html:
# Install the web files...
html:
	moncmd -q -w3 $(TARGET_IP) "reset -x"
	(cd htmldir1; ./loadhtml $(TARGET_IP); cd -)
	ttftp $(TARGET_IP) put srvr srvr,e

#####
#
# cscope:
# Remove all files created by this make.
#
cscope:
	find . -name '*.[chS]' | grep -v ipv6 | grep -v ppp | grep -v snmp >cscope.files
	ctags --file-tags=yes -n -L cscope.files

