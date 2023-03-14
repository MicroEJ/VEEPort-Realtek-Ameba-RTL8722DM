/*
 * C
 *
 * Copyright 2021 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */
 
 
 /* Includes ------------------------------------------------------------------*/
#include "watchdog_timer_realtek_AmebaD_configuration.h"

#include <stdint.h>

#include "sys_api.h"

#include "LLMJVM.h"
#include "LLWATCHDOG_TIMER_impl.h"
#include "LLWATCHDOG_TIMER_configuration.h"
#include "watchdog_timer_helper.h"

#include "watchdog_timer_freertos.h"


#ifdef __cplusplus
	extern "C" {
#endif


/* Macros and Defines --------------------------------------------------------*/

/* Globals -------------------------------------------------------------------*/

/* Project functions----------------------------------------------------------*/

#if WATCHDOG_TIMER_MODE == WATCHDOG_TIMER_INTERRUPT_MODE
void watchdog_timer_irq_handler(uint32_t id){
	LLWATCHDOG_TIMER_IMPL_stop_action();
	LLWATCHDOG_TIMER_DEBUG_TRACE("Watchdog timer triggers! print RTOS task status and use LLMJVM_dump\n");

	watchdog_timer_freertos_print_tasks_state();
	LLMJVM_dump();

	LLWATCHDOG_TIMER_DEBUG_TRACE("Try to perform a system reset to return to a normal state.\n");
	sys_reset();
	for(;;){
		//wait until reset
	}
}
#endif


/* Internal function definitions ---------------------------------------------*/


#ifdef __cplusplus
	}
#endif

