/* cmdtbl.c:
 *	This is the command table used by the monitor.
 *
 *	General notice:
 *	This code is part of a boot-monitor package developed as a generic base
 *	platform for embedded system designs.  As such, it is likely to be
 *	distributed to various projects beyond the control of the original
 *	author.  Please notify the author of any enhancements made or bugs found
 *	so that all may benefit from the changes.  In addition, notification back
 *	to the author will allow the new user to pick up changes that may have
 *	been made by other users after this version of the code was distributed.
 *
 *	Note1: the majority of this code was edited with 4-space tabs.
 *	Note2: as more and more contributions are accepted, the term "author"
 *		   is becoming a mis-representation of credit.
 *
 *	Original author:	Ed Sutter
 *	Email:				esutter@lucent.com
 *	Phone:				908-582-2351
 */
#include "config.h"
#include "tfs.h"
#include "tfsprivate.h"
#include "cli.h"
#include "genlib.h"
#include "xcmddcl.h"

#define	ULVLCMD  "ulvl"

#if INCLUDE_MEMCMDS
#define INCLUDE_PM 1
#define INCLUDE_DM 1
#define INCLUDE_FM 1
#define INCLUDE_CM 1
#define INCLUDE_SM 1
#define INCLUDE_MT 1
#endif

extern	int CliCmd_IPV6(int, char **);
extern	int	BinfoCmd(int, char **);
extern	int Call(int, char **);
extern	int	Cast(int, char **);
extern	int Cm(int, char **);
extern	int	Dm(int, char **);
extern	int Dhcp(int, char **);
extern	int	DnsCmd(int, char **);
extern	int Echo(int, char **);
extern	int	Edit(int, char **);
extern	int Ether(int, char **);
extern	int Exit(int, char **);
extern	int	FatfsCmd(int, char **);
extern	int	FbiCmd(int, char **);
extern	int	FlashCmd(int, char **);
extern	int Fm(int, char **);
extern	int	Gdb(int, char **);
extern	int Goto(int, char **);
extern	int Gosub(int, char **);
extern	int	Heap(int, char **);
extern	int	Help(int, char **);
extern	int Icmp(int, char **);
extern	int	Ide(int, char **);
extern	int	I2cCmd(int, char **);
extern	int	If(int, char **);
extern	int	Igmp(int, char **);
extern	int Mt(int, char **);
extern	int	MtraceCmd(int, char **);
extern	int Pm(int, char **);
extern	int	Read(int, char **);
extern	int	Reg(int, char **);
extern	int Reset(int, char **);
extern	int Return(int, char **);
extern	int SdCmd(int, char **);
extern	int	Set(int, char **);
extern	int Sleep(int, char **);
extern	int Sm(int, char **);
extern	int SpifCmd(int, char **);
extern	int	Strace(int, char **);
extern	int	StructCmd(int, char **);
extern	int	SyslogCmd(int, char **);
extern	int	Tfs(int, char **);
extern	int	Tftp(int, char **);
extern	int	TsiCmd(int, char **);
extern	int Ulvl(int, char **);
extern	int	Unzip(int, char **);
extern	int Version(int, char **);
extern	int	Xmodem(int, char **);

extern	char *CliHelp_IPV6[];
extern  char *BinfoHelp[];
extern	char *CallHelp[];
extern  char *CastHelp[];
extern	char *CmHelp[];
extern	char *DhcpHelp[];
extern  char *DmHelp[];
extern  char *DnsHelp[];
extern	char *EchoHelp[];
extern	char *EditHelp[];
extern	char *EtherHelp[];
extern	char *ExitHelp[];
extern	char *FatfsHelp[];
extern	char *FbiHelp[];
extern	char *FlashHelp[];
extern	char *FmHelp[];
extern  char *GdbHelp[];
extern	char *GosubHelp[];
extern	char *GotoHelp[];
extern	char *HelpHelp[];
extern  char *HeapHelp[];
extern	char *IcmpHelp[];
extern  char *IdeHelp[];
extern  char *I2cHelp[];
extern	char *IfHelp[];
extern  char *IgmpHelp[];
extern	char *MtHelp[];
extern  char *MtraceHelp[];
extern	char *PmHelp[];
extern	char *ReadHelp[];
extern	char *RegHelp[];
extern	char *ResetHelp[];
extern	char *ReturnHelp[];
extern	char *SdHelp[];
extern	char *SetHelp[];
extern	char *SleepHelp[];
extern	char *SmHelp[];
extern	char *SpifHelp[];
extern	char *StraceHelp[];
extern	char *StructHelp[];
extern  char *SyslogHelp[];
extern	char *TfsHelp[];
extern	char *TftpHelp[];
extern	char *TsiHelp[];
extern	char *UlvlHelp[];
extern  char *UnzipHelp[];
extern	char *VersionHelp[];
extern	char *XmodemHelp[];

struct monCommand cmdlist[] = {
#if INCLUDE_BOARDINFO
	{ "brdinfo",	BinfoCmd,	BinfoHelp,		0 },
#endif
#if INCLUDE_CALLCMD
	{ "call",		Call,		CallHelp,		CMDFLAG_NOMONRC },
#endif
#if INCLUDE_CM
	{ "cm",			Cm,			CmHelp,			0 },
#endif
#if INCLUDE_DHCPBOOT
	{ "dhcp",		Dhcp,		DhcpHelp,		CMDFLAG_NOMONRC },
#endif
#if INCLUDE_DM
	{ "dm",			Dm,			DmHelp,			0 },
#endif
#if INCLUDE_DNS
	{ "dns",		DnsCmd,		DnsHelp,		0 },
#endif
#if INCLUDE_TFSSCRIPT
	{ "echo",		Echo,		EchoHelp,		0 },
#endif
#if INCLUDE_EDIT
	{ "edit",		Edit,		EditHelp,		0 },
#endif
#if INCLUDE_ETHERNET & INCLUDE_ETHERCMD
	{ "ether",		Ether,		EtherHelp,		CMDFLAG_NOMONRC },
#endif
#if INCLUDE_TFSSCRIPT
	{ "exit",		Exit,		ExitHelp,		0 },
#endif
#if INCLUDE_FATFS
	{ "fatfs",		FatfsCmd,	FatfsHelp,		0 },
#endif
#if INCLUDE_FBI
	{ "fbi",		FbiCmd,		FbiHelp,		0 },
#endif
#if INCLUDE_FLASH
	{ "flash",		FlashCmd,	FlashHelp,		0 },
#endif
#if INCLUDE_FM
	{ "fm",			Fm,			FmHelp,			0 },
#endif

#if INCLUDE_GDB
	{ "gdb",		Gdb,		GdbHelp,		CMDFLAG_NOMONRC },
#endif

#if INCLUDE_TFSSCRIPT
	{ "gosub",		Gosub,		GosubHelp,		0 },
	{ "goto",		Goto,		GotoHelp,		0 },
#endif

#if INCLUDE_MALLOC
#if INCLUDE_HEAPCMD
	{ "heap",		Heap,		HeapHelp,		0 },
#endif
#endif

	{ "help",		Help,		HelpHelp,		0 },

#if INCLUDE_I2C
	{ "i2c",		I2cCmd,		I2cHelp,		0 },
#endif

#if INCLUDE_ICMP & INCLUDE_ICMPCMD
	{ "icmp",		Icmp,		IcmpHelp,		CMDFLAG_NOMONRC },
#endif

#if INCLUDE_IDE
	{ "ide",		Ide,		IdeHelp,		CMDFLAG_NOMONRC },
#endif

#if INCLUDE_IGMP
	{ "igmp",		Igmp,		IgmpHelp,		CMDFLAG_NOMONRC },
#endif

#if INCLUDE_TFSSCRIPT
	{ "if",			If,			IfHelp,			0 },
#endif

#if INCLUDE_IPV6
	{ "ipv6",		CliCmd_IPV6,		CliHelp_IPV6,		CMDFLAG_NOMONRC },
#endif

#if INCLUDE_MT
	{ "mt",			Mt,			MtHelp,			0 },
#endif

#if INCLUDE_PM
	{ "pm",			Pm,			PmHelp,			0 },
#endif

#if INCLUDE_TFSSCRIPT
#if INCLUDE_READ
	{ "read",		Read,		ReadHelp,		0 },
#endif
#endif

#if INCLUDE_RESETCMD
	{ "reset",		Reset,		ResetHelp,		0 },
#endif

#if INCLUDE_TFSSCRIPT
	{ "return",		Return,		ReturnHelp,		0 },
#endif
#if INCLUDE_SD
	{ "sd",			SdCmd,		SdHelp,			0 },
#endif

	{ "set",		Set,		SetHelp,		0 },

#if INCLUDE_TFSSCRIPT
	{ "sleep",		Sleep,		SleepHelp,		0 },
#endif

#if INCLUDE_SM
	{ "sm",			Sm,			SmHelp,			0 },
#endif

#if INCLUDE_SPIF
	{ "spif",		SpifCmd,	SpifHelp,		0 },
#endif

#if INCLUDE_SYSLOG
	{ "syslog",		SyslogCmd,	SyslogHelp,		CMDFLAG_NOMONRC },
#endif

#if INCLUDE_TFTP
	{ "tftp",		Tftp,		TftpHelp,		CMDFLAG_NOMONRC },
#endif

#if INCLUDE_TFSCLI
	{ "tfs",		Tfs,		TfsHelp,		0 },
#endif

#if INCLUDE_TSI
	{ "tsi",		TsiCmd,		TsiHelp,		0 },
#endif

#if INCLUDE_UNZIP
	{ "unzip",		Unzip,		UnzipHelp,		0 },
#endif

#if INCLUDE_XMODEM
	{ "xmodem",		Xmodem,		XmodemHelp,		0 },
#endif

#if INCLUDE_VERSIONCMD
	{ "version",	Version,	VersionHelp,	0 },
#endif

#include "xcmdtbl.h"				/* For non-generic commands that are */
									/* specific to a particular target.  */
	{ 0,0,0,0 },
};
