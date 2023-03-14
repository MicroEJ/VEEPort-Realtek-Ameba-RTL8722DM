/*
 * C
 *
 * Copyright 2021 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

/**
 * @file
 * @brief WATCHDOG TIMER REALTEK AMEBAD CCO implementation file.
 * @author MicroEJ Developer Team
 * @version 2.0.1
 * @date 5 August 2021
 */

/* Includes ------------------------------------------------------------------*/
#include <watchdog_timer_realtek_AmebaD_configuration.h>

#include "wdt_api.h"
#include "LLWATCHDOG_TIMER_configuration.h"
#include "LLWATCHDOG_TIMER_impl.h"
#include "watchdog_timer_helper.h"


#ifdef __cplusplus
	extern "C" {
#endif


/* Macros and Defines --------------------------------------------------------*/
/**
* Sanity check between the expected version of the configuration and the actual version of
* the configuration.
* If an error is raised here, it means that a new version of the CCO has been installed and
* the configuration watchdog_realtek_AmebaD.h must be updated based on the one provided
* by the new CCO version.
*/
#if WATCHDOG_TIMER_REALTEK_AMEBAD_CONFIGURATION_VERSION != 1
	#error "Version of the configuration file watchdog_timer_realtek_AmebaD_configuration.h is not compatible with this implementation."
#endif

#if WATCHDOG_TIMER_TIMEOUT_MS <= 0
#error "Illegal watchdog timeout value defined with the macro WATCHDOG_TIMER_TIMEOUT_MS. The timeout value must be positive."
#endif


int32_t LLWATCHDOG_TIMER_IMPL_init_action(void){
	watchdog_init(WATCHDOG_TIMER_TIMEOUT_MS);
#if WATCHDOG_TIMER_MODE == WATCHDOG_TIMER_INTERRUPT_MODE
	watchdog_irq_init((wdt_irq_handler)watchdog_timer_irq_handler, 0);
#endif
	return WATCHDOG_TIMER_OK;
}

int32_t LLWATCHDOG_TIMER_IMPL_start_action(void){
	watchdog_start();
	return WATCHDOG_TIMER_OK;
}

int32_t LLWATCHDOG_TIMER_IMPL_stop_action(void){
	watchdog_stop();
	return WATCHDOG_TIMER_OK;
}

int32_t LLWATCHDOG_TIMER_IMPL_refresh_action(void){
	watchdog_refresh();
	return WATCHDOG_TIMER_OK;
}

uint8_t LLWATCHDOG_TIMER_IMPL_isResetCause_action(void){
	if (BIT_BOOT_KM4WDG_RESET_HAPPEN & BOOT_Reason()){
		return WATCHDOG_TIMER_TRUE;
	}
	return WATCHDOG_TIMER_FALSE;
}

int32_t LLWATCHDOG_TIMER_IMPL_getWatchdogTimeoutMs_action(void){
	return WATCHDOG_TIMER_TIMEOUT_MS;
}

#ifdef __cplusplus
	}
#endif

