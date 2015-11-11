extern void ledSet(int lednum, int state);
extern void ledCfg(void);

#define LED6_ON()	ledSet(6,0)
#define LED7_ON()	ledSet(7,0)
#define LED8_ON()	ledSet(8,0)
#define LED9_ON()	ledSet(9,0)

#define LED6_OFF()	ledSet(6,1)
#define LED7_OFF()	ledSet(7,1)
#define LED8_OFF()	ledSet(8,1)
#define LED9_OFF()	ledSet(9,1)

#define LED6_SET(a)	ledSet(6,a)
#define LED7_SET(a)	ledSet(7,a)
#define LED8_SET(a)	ledSet(8,a)
#define LED9_SET(a)	ledSet(9,a)


/* Ethernet link-up led: */
#define ENET_LINK_LED_ON()		LED6_SET(0)
#define ENET_LINK_LED_OFF()		LED6_SET(1)

/* General "I-am-alive" status led: */
#define STATLED_SET(a)			LED9_SET(a)
#define STATLED_ON()			LED9_ON()
#define STATLED_OFF()			LED9_OFF()
