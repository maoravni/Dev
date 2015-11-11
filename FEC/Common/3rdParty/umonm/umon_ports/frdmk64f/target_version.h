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
 * 0 -> 1: Initial boot with debug uart port.
 * 1 -> 2: target_reset() uses watchdog, target_timer() interfaces with PIT.
 * 2 -> 3: Ethernet and TFS are atleast basically working (need more testing).
 * 3 -> 4: TFS moved to top of flash, demo app working.
 * 4 -> 5: Lot of rework to fit this in a 64K block.
 */
#define TARGET_VERSION 5
