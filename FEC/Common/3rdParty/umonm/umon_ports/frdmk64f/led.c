#include "config.h"
#if INCLUDE_LEDCMD
#include "cpuio.h"
#include "stddefs.h"
#include "timer.h"
#include "cli.h"
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "fsl_debug_uart.h"
#include "fsl_uart_hal.h"
#include "gpio_pins.h"
#include "device/MK64F12/MK64F12.h"
#include "device/MK64F12/MK64F12_wdog.h"
#include "device/fsl_device_registers.h"
#include "hal/pit/fsl_pit_hal.h"
#include "fsl_clock_manager.h"

#define GRNLED_PIN	26
#define BLULED_PIN	21
#define REDLED_PIN	22
#define GRNLED_PORT	HW_PORTE
#define BLULED_PORT	HW_PORTB
#define REDLED_PORT	HW_PORTB

void gpio_output_pin_init(const gpio_output_pin_user_config_t *outputPin)
{
    /* Get actual port and pin number.*/
    uint32_t gpioInstance = GPIO_EXTRACT_PORT(outputPin->pinName);
    uint32_t pin = GPIO_EXTRACT_PIN(outputPin->pinName);

    /* Un-gate port clock*/
    clock_manager_set_gate(kClockModulePORT, gpioInstance, true);

    /* Set current pin as digital output.*/
    gpio_hal_set_pin_direction(gpioInstance, pin, kGpioDigitalOutput);

    /* Configure GPIO output features. */
    gpio_hal_write_pin_output(gpioInstance, pin, outputPin->config.outputLogic);
    port_hal_configure_slew_rate(gpioInstance, pin, outputPin->config.slewRate);
    port_hal_configure_drive_strength(gpioInstance, pin, outputPin->config.driveStrength);
    #if FSL_FEATURE_PORT_HAS_OPEN_DRAIN
    port_hal_configure_open_drain(gpioInstance, pin, outputPin->config.isOpenDrainEnabled);
    #endif
}


void
ledSetup(void)
{
	gpio_output_pin_init(&ledPins[0]);
	gpio_output_pin_init(&ledPins[1]);
	gpio_output_pin_init(&ledPins[2]);

	/* LED is turned on when gpiobit is cleared... */
	gpio_hal_set_pin_output(REDLED_PORT,REDLED_PIN);	
	gpio_hal_set_pin_output(GRNLED_PORT,GRNLED_PIN);
	gpio_hal_clear_pin_output(BLULED_PORT,BLULED_PIN);
}


char *LedHelp[] = {
	"Configure RGB LED",
	"{R|r|G|g|B|b}",
	0,
};

#include "core_cm4.h"

int
LedCmd(int argc, char *argv[])
{
	if (argc < 2)
		return(CMD_FAILURE);

	if (strcmp(argv[1],"tick") == 0) {
		SysTick_Config(5000);
		while(1);
		return(CMD_SUCCESS);
	}

	if (strchr(argv[1],'R'))
		gpio_hal_clear_pin_output(REDLED_PORT, REDLED_PIN);
	if (strchr(argv[1],'r'))
		gpio_hal_set_pin_output(REDLED_PORT, REDLED_PIN);

	if (strchr(argv[1],'G'))
		gpio_hal_clear_pin_output(GRNLED_PORT, GRNLED_PIN);
	if (strchr(argv[1],'g'))
		gpio_hal_set_pin_output(GRNLED_PORT, GRNLED_PIN);

	if (strchr(argv[1],'B'))
		gpio_hal_clear_pin_output(BLULED_PORT, BLULED_PIN);
	if (strchr(argv[1],'b'))
		gpio_hal_set_pin_output(BLULED_PORT, BLULED_PIN);

	return(CMD_SUCCESS);
}
#endif
