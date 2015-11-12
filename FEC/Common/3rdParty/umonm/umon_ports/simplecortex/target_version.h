/* target_version.h:
 * Initial version for all ports is zero.  As the TARGET_VERSION incrments
 * as a result of changes made to the target-specific code, this file should
 * be used as an informal log of those changes for easy reference by others.
 *
 * If you're reading this under the umon_ports/simplecortex directory
 * then realize that these version transitions are not necessarily
 * reflective of port-specific updates.  With this being the first port of
 * the new uMon-M, these version updates may simply reflect general
 * cleanups of the uMon-M build.
 * If you're reading this and its NOT under the umon_ports/simplecortex
 * directory; then chances are good that this file was copied from the
 * simplecortex port, but not cleaned up!
 *
 * Version transitions:
 *
 * 1->2:
 *	-xmodem now works with LPC1769 flash for files and raw address but
 *	 not yet for boot-reburn.
 *  -tftp srvr works with LPC1769 flash for files and raw address; haven't
 *	 tried the client yet.
 * 2->3:
 *  -removed 'arp' and 'history' commands.
 * 3->4:
 *  -fixed issue in tfsadd(): can't do the precompare when source is
 *	 in TFS space.
 *	-new simpler (not powersafe) defrag options.
 *	-added #if INCLUDE_EDITHELPINFO to edit.c
 *	-removed extravagant options from icmp command.
 */

#define TARGET_VERSION 4
