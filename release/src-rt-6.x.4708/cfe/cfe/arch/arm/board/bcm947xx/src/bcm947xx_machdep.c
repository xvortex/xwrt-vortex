/*
 * Broadcom Common Firmware Environment (CFE)
 * Architecure dependent initialization,
 * File: bcm947xx_machdep.c
 *
 * Copyright (C) 2012, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id$
 */

#include "cfe_error.h"

#include <typedefs.h>
#include <osl.h>
#include <bcmutils.h>
#include <hndsoc.h>
#include <siutils.h>
#include <sbchipc.h>
#include <bcmdevs.h>
#include <bcmnvram.h>
#include <hndcpu.h>
#include <hndchipc.h>
#include "bsp_priv.h"
#include <chipcommonb.h>

extern bool si_arm_setclock(si_t *sih, uint32 armclock, uint32 ddrclock, uint32 axiclock);
extern int cpu_turbo_mode;

#ifdef EA6900
/* define EA6900 GPIO */
#define	LOGO_LED1_GPIO	(1 << 6)	// GPIO 6
#define	LOGO_LED2_GPIO	(1 << 8)	// GPIO 8

#define	USB1_PWR_GPIO	(1 << 9)	// GPIO 9
#define	USB2_PWR_GPIO	(1 << 10)	// GPIO 10

#define RST_BTN_GPIO	(1 << 11)	// GPIO 11
#define WPS_BTN_GPIO	(1 << 7)	// GPIO 7
#endif

#ifdef R7000
/* define R7000 GPIO */
#define	PWR_WH_LED_GPIO	(1 << 2)	// GPIO 2
#define	PWR_OR_LED_GPIO	(1 << 3)	// GPIO 3
#define	WAN_OR_LED_GPIO	(1 << 8)	// GPIO 8
#define	WAN_WH_LED_GPIO	(1 << 9)	// GPIO 9
#define	WL_5G_LED_GPIO	(1 << 12)	// GPIO 12
#define	WL_2G_LED_GPIO	(1 << 13)	// GPIO 13
#define	WPS_LED_GPIO	(1 << 14)	// GPIO 14
#define	WIFI_LED_GPIO	(1 << 15)	// GPIO 15
#define	USB2_LED_GPIO	(1 << 17)	// GPIO 17
#define	USB3_LED_GPIO	(1 << 18)	// GPIO 18

#define	USB_PWR_GPIO	(1 << 0)	// GPIO 0

#define WPS_BTN_GPIO	(1 << 4)	// GPIO 4
#define WIFI_BTN_GPIO	(1 << 5)	// GPIO 5
#define RST_BTN_GPIO	(1 << 6)	// GPIO 6
#endif

#ifdef R6300V2
/* define R6300V2 GPIO */
#define	LOGO_LED_GPIO	(1 << 1)	// GPIO 1
#define	PWR_GR_LED_GPIO	(1 << 2)	// GPIO 2
#define	PWR_OR_LED_GPIO	(1 << 3)	// GPIO 3
#define	USB_LED_GPIO	(1 << 8)	// GPIO 8
#define	WAN_LED_GPIO	(1 << 10)	// GPIO 10
#define	WLAN_LED_GPIO	(1 << 11)	// GPIO 11

#define	USB_PWR_GPIO	(1 << 0)	// GPIO 0

#define WPS_BTN_GPIO	(1 << 4)	// GPIO 4
#define WIFI_BTN_GPIO	(1 << 5)	// GPIO 5
#define RST_BTN_GPIO	(1 << 6)	// GPIO 6
#endif

#ifdef WS880
/* define WS880 GPIO */
#define	WLAN_LED_GPIO	(1 << 0)	// GPIO 0
#define	LAN_LED_GPIO	(1 << 1)	// GPIO 1
#define	WPS_LED_GPIO	(1 << 6)	// GPIO 6
#define	INET_LED_GPIO	(1 << 12)	// GPIO 12
#define	USB_LED_GPIO	(1 << 14)	// GPIO 14

#define	USB_PWR_GPIO	(1 << 7)	// GPIO 7

#define RST_BTN_GPIO	(1 << 2)	// GPIO 2
#define WPS_BTN_GPIO	(1 << 3)	// GPIO 3
#define PWR_BTN_GPIO	(1 << 15)	// GPIO 15
#endif

void
board_pinmux_init(si_t *sih)
{
	uint origidx;
	chipcommonbregs_t *chipcb;

	origidx = si_coreidx(sih);
	chipcb = si_setcore(sih, NS_CCB_CORE_ID, 0);
	if (chipcb != NULL) {
		/* Default select the mux pins for GPIO */
		W_REG(osh, &chipcb->cru_gpio_control0, 0x1fffff);
	}
	si_setcoreidx(sih, origidx);
#ifdef EA6900
	si_gpiocontrol(sih, LOGO_LED1_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, LOGO_LED2_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, USB1_PWR_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, USB2_PWR_GPIO, 0, GPIO_DRV_PRIORITY);

	si_gpioouten(sih, LOGO_LED1_GPIO, LOGO_LED1_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, LOGO_LED2_GPIO, LOGO_LED2_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, USB1_PWR_GPIO, USB1_PWR_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, USB2_PWR_GPIO, USB2_PWR_GPIO, GPIO_DRV_PRIORITY);

	si_gpioout(sih, LOGO_LED1_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpioout(sih, LOGO_LED2_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpioout(sih, USB1_PWR_GPIO, USB1_PWR_GPIO, GPIO_DRV_PRIORITY);
	si_gpioout(sih, USB2_PWR_GPIO, USB2_PWR_GPIO, GPIO_DRV_PRIORITY);
#endif
#ifdef R7000
	si_gpiocontrol(sih, PWR_WH_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, PWR_OR_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, WAN_OR_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, WAN_WH_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, WL_2G_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, WL_5G_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, WPS_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, WIFI_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, USB2_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, USB3_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, USB_PWR_GPIO, 0, GPIO_DRV_PRIORITY);

	si_gpioouten(sih, PWR_WH_LED_GPIO, PWR_WH_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, PWR_OR_LED_GPIO, PWR_OR_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, WAN_OR_LED_GPIO, WAN_OR_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, WAN_WH_LED_GPIO, WAN_WH_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, WL_2G_LED_GPIO, WL_2G_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, WL_5G_LED_GPIO, WL_5G_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, WPS_LED_GPIO, WPS_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, WIFI_LED_GPIO, WIFI_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, USB2_LED_GPIO, USB2_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, USB3_LED_GPIO, USB3_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, USB_PWR_GPIO, USB_PWR_GPIO, GPIO_DRV_PRIORITY);

	si_gpioout(sih, PWR_WH_LED_GPIO, PWR_WH_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioout(sih, PWR_OR_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpioout(sih, WAN_OR_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpioout(sih, WAN_WH_LED_GPIO, WAN_WH_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioout(sih, WL_2G_LED_GPIO, WL_2G_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioout(sih, WL_5G_LED_GPIO, WL_5G_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioout(sih, WPS_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpioout(sih, WIFI_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpioout(sih, USB2_LED_GPIO, USB2_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioout(sih, USB3_LED_GPIO, USB3_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioout(sih, USB_PWR_GPIO, USB_PWR_GPIO, GPIO_DRV_PRIORITY);
#endif
#ifdef R6300V2
	si_gpiocontrol(sih, LOGO_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, PWR_GR_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, PWR_OR_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, USB_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, WAN_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, WLAN_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, USB_PWR_GPIO, 0, GPIO_DRV_PRIORITY);

	si_gpioouten(sih, LOGO_LED_GPIO, LOGO_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, PWR_GR_LED_GPIO, PWR_GR_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, PWR_OR_LED_GPIO, PWR_OR_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, USB_LED_GPIO, USB_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, WAN_LED_GPIO, WAN_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, WLAN_LED_GPIO, WLAN_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, USB_PWR_GPIO, USB_PWR_GPIO, GPIO_DRV_PRIORITY);

	si_gpioout(sih, LOGO_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpioout(sih, PWR_GR_LED_GPIO, PWR_GR_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioout(sih, PWR_OR_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpioout(sih, USB_LED_GPIO, USB_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioout(sih, WAN_LED_GPIO, WAN_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioout(sih, WLAN_LED_GPIO, WLAN_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioout(sih, USB_PWR_GPIO, USB_PWR_GPIO, GPIO_DRV_PRIORITY);
#endif
#ifdef WS880
	si_gpiocontrol(sih, WLAN_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, LAN_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, WPS_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, INET_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, USB_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpiocontrol(sih, USB_PWR_GPIO, 0, GPIO_DRV_PRIORITY);
	
	si_gpioouten(sih, WLAN_LED_GPIO, WLAN_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, LAN_LED_GPIO, LAN_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, WPS_LED_GPIO, WPS_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, INET_LED_GPIO, INET_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, USB_LED_GPIO, USB_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioouten(sih, USB_PWR_GPIO, USB_PWR_GPIO, GPIO_DRV_PRIORITY);

	si_gpioout(sih, WLAN_LED_GPIO, WLAN_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioout(sih, LAN_LED_GPIO, LAN_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioout(sih, WPS_LED_GPIO, WPS_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioout(sih, INET_LED_GPIO, 0, GPIO_DRV_PRIORITY);
	si_gpioout(sih, USB_LED_GPIO, USB_LED_GPIO, GPIO_DRV_PRIORITY);
	si_gpioout(sih, USB_PWR_GPIO, USB_PWR_GPIO, GPIO_DRV_PRIORITY);
#endif
}

void
board_clock_init(si_t *sih)
{
	uint32 armclock = 0, ddrclock = 0, axiclock = 0;
	char *nvstr;
	char *end;

	if (cpu_turbo_mode)
	{
		printf("CPU Turbo Mode\n");
		nvstr = strdup("1000,533");
	}
	else
		nvstr = nvram_safe_get("clkfreq");

	/* ARM clock speed override */
	if (nvstr) {
		printf("clkfreq: %s\n", nvstr);
		armclock = bcm_strtoul(nvstr, &end, 0) * 1000000;

		if (cpu_turbo_mode)
			KFREE(nvstr);

		if (*end == ',') {
			nvstr = ++end;
			ddrclock = bcm_strtoul(nvstr, &end, 0) * 1000000;
			if (*end == ',') {
				nvstr = ++end;
				axiclock = bcm_strtoul(nvstr, &end, 0) * 1000000;
			}
		}
	}

	if (!armclock)
		armclock = 800 * 1000000;

	/* To accommodate old sdram_ncdl usage to store DDR clock;
	 * should be removed if sdram_ncdl is used for some other purpose.
	 */
	if ((nvstr = nvram_get("sdram_ncdl"))) {
		uint32 ncdl = bcm_strtoul(nvstr, NULL, 0);
		if (ncdl && (ncdl * 1000000) != ddrclock) {
			ddrclock = ncdl * 1000000;
		}
	}

	/* Set current ARM clock speed */
	si_arm_setclock(sih, armclock, ddrclock, axiclock);

	/* Update cfe_cpu_speed */
	board_cfe_cpu_speed_upd(sih);
}

void
mach_device_init(si_t *sih)
{
	uint32 armclock = 0, ddrclock = 0;
	char *nvstr;
	char *end;
	uint32 ncdl = 0;
	uint32 clock;

	/* ARM clock speed override */
	if ((nvstr = nvram_get("clkfreq"))) {
		armclock = bcm_strtoul(nvstr, &end, 0);
		if (*end == ',') {
			nvstr = ++end;
			ddrclock = bcm_strtoul(nvstr, &end, 0);
		}
	}

	/* To accommodate old sdram_ncdl usage to store DDR clock;
	 * should be removed if sdram_ncdl is used for some other purpose.
	 */
	clock = si_mem_clock(sih) / 1000000;
	printf("DDR Clock: %u MHz\n", clock);
	if ((nvstr = nvram_get("sdram_ncdl"))) {
		ncdl = bcm_strtoul(nvstr, NULL, 0);
		if (ncdl && ncdl != ddrclock) {
			printf("Warning: using legacy sdram_ncdl parameter to set DDR frequency. "
				"Equivalent setting in clkfreq=%u,*%u* will be ignored.\n",
				armclock, ddrclock);
			if (ncdl != 0 && ncdl != clock)
				printf("Warning: invalid DDR setting of %u MHz ignored. "
					"DDR frequency will be set to %u MHz.\n", ncdl, clock);
			goto out;
		}
	}

	if (ddrclock)
		printf("Info: DDR frequency set from clkfreq=%u,*%u*\n", armclock, ddrclock);
	if (ddrclock != clock)
		printf("Warning: invalid DDR setting of %u MHz ignored. "
			"DDR frequency will be set to %u MHz.\n", ddrclock, clock);

out:
	return;
}

void
board_power_init(si_t *sih)
{
        uint origidx;
        chipcregs_t *cc;
        int gpio;

        /* Drive gpio pin to HIGH to enable on-board switching regulator for COMA mode */
        origidx = si_coreidx(sih);
        if ((cc = (chipcregs_t *)si_setcore(sih, CC_CORE_ID, 0)) != NULL) {
                gpio = getgpiopin(NULL, "coma_swreg", GPIO_PIN_NOTDEFINED);
                if (gpio != GPIO_PIN_NOTDEFINED) {
                        gpio = 1 << gpio;
                        si_gpioout(sih, gpio, gpio, GPIO_DRV_PRIORITY);
                        si_gpioouten(sih, gpio, gpio, GPIO_DRV_PRIORITY);
                }
        }
        si_setcoreidx(sih, origidx);
}

void
board_cpu_init(si_t *sih)
{
	/* Initialize clocks and interrupts */
	si_arm_init(sih);
}

/* returns the ncdl value to be programmed into sdram_ncdl for calibration */
uint32
si_memc_get_ncdl(si_t *sih)
{
	return 0;
}

void
flash_memory_size_config(si_t *sih, void *probe)
{
}


void
board_cfe_cpu_speed_upd(si_t *sih)
{
	/* Update current CPU clock speed */
	if ((cfe_cpu_speed = si_cpu_clock(sih)) == 0) {
		cfe_cpu_speed = 800000000; /* 800 MHz */
	}
}
