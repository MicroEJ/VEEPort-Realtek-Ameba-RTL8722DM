/*
 * C
 *
 * Copyright 2021 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

#ifndef WATCHDOG_TIMER_REALTEK_AMEBAD_CONF_H
#define WATCHDOG_TIMER_REALTEK_AMEBAD_CONF_H

/**
 * @file
 * @brief WATCHDOG TIMER REALTEK AMEBAD CCO configuration file.
 * @author MicroEJ Developer Team
 * @version 2.0.1
 * @date 5 August 2021
 */

#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

//#error "This header must be customized with platform specific configuration. Remove this #error when done. This file is not modified when a new version of the CCO is installed."

/**
 * @brief Compatibility sanity check value.
 * This define value is checked in the implementation to validate that the version of this configuration
 * is compatible with the implementation.
 *
 * This value must not be changed by the user of the CCO.
 * This value must be incremented by the implementor of the CCO when a configuration define is added, deleted or modified.
 */
#define WATCHDOG_TIMER_REALTEK_AMEBAD_CONFIGURATION_VERSION (1)

/**
 * @brief The timeout (in milliseconds) of the hardware watchdog timer.
 * @warning Choose a value according your MCU datasheet.
 */
#ifndef WATCHDOG_TIMER_TIMEOUT_MS
#define WATCHDOG_TIMER_TIMEOUT_MS 50000 // ms
#endif

/**
 *  Watchdog mode
 *  - WATCHDOG_TIMER_INTERRUPT_MODE: interrupt CPU when Watchdog timer is triggered.
 *  - WATCHDOG_TIMER_RESET_MODE: reset the hardware when Watchdog timer is triggered.
 *
 *  Don't modify WATCHDOG_TIMER_INTERRUPT_MODE and WATCHDOG_TIMER_RESET_MODE constants.
 */
#define WATCHDOG_TIMER_INTERRUPT_MODE 0
#define WATCHDOG_TIMER_RESET_MODE 1

/**
 * @brief Selects the watchdog mode.
 * Reset mode selected by default.
 */
#ifndef WATCHDOG_TIMER_MODE
#define WATCHDOG_TIMER_MODE WATCHDOG_TIMER_INTERRUPT_MODE
#endif


#if WATCHDOG_TIMER_MODE == WATCHDOG_TIMER_INTERRUPT_MODE
/**
 * @brief The watchdog timer IRQ handler.
 * Callback function called when the watchdog is triggered.
 * This function is called only when the watchdog is configured in interrupt mode.
 *
 * @param[in] id the parameter for the callback function
 *
 * @see watchdog_timer_irq_init(wdt_irq_handler handler, uint32_t id) function in wdt_api.h.
 *
 * @note IMPORTANT: you have to implement this function.
 *
 */
//#error "Since you use the watchdog timer in interrupt mode, you have to implement the callback function <code>void watchdog_timer_irq_handler(uint32_t id)</code> for watchdog timeout interrupt. Remove this #error when done."
void watchdog_timer_irq_handler(uint32_t id);

#endif // WATCHDOG_TIMER_MODE == WATCHDOG_TIMER_INTERRUPT_MODE


#ifdef __cplusplus
	}
#endif

#endif /* WATCHDOG_TIMER_REALTEK_AMEBAD_CONF_H */
