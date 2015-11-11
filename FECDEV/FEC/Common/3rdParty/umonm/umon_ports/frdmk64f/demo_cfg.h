/* Define APPSTACKSIZE if the application is to establish
 * its own stack, and use mon_appexit() at the time of
 * completion.
 * If APPSTACKSIZE is not defined, then the demo runs off
 * the stack of uMon and will return to the calling context
 * of uMon at the time of completion.
 */
#define APPSTACKSIZE 0x800
