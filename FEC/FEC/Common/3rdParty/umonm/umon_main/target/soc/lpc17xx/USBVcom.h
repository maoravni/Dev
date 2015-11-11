
#define INT_IN_EP		0x81
#define BULK_OUT_EP		0x05
#define BULK_IN_EP		0x82

#define MAX_PACKET_SIZE	64

#define LE_WORD(x)		((x)&0xFF),((x)>>8)

// CDC definitions
#define CS_INTERFACE			0x24
#define CS_ENDPOINT				0x25

#define	SET_LINE_CODING			0x20
#define	GET_LINE_CODING			0x21
#define	SET_CONTROL_LINE_STATE	0x22

void BulkOut(U8 bEP, U8 bEPStatus);
void BulkIn(U8 bEP, U8 bEPStatus);
BOOL HandleClassRequest(TSetupPacket *pSetup, int *piLen, U8 **ppbData);
void VCOM_init(void);
int VCOM_putchar(int c);
void VCOM_putstring (char datasend[]);
int VCOM_getchar(void);
int VCOM_GetString(char *data);
char* VCOM_RecieveString(void);
int VCOM_Available(void);
void USB_init (void);
void USB_IRQHandler(void);
void USBFrameHandler(U16 wFrame);
void enable_USB_interrupts(void);
void enable_USB_interrupts(void);
