/*
 * C
 *
 * Copyright 2013-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include "FreeRTOS.h"
#include "task.h"

#include "mej_log.h"

#include "monitor.h"

#if MONITOR_ENABLED == 1

// -----------------------------------------------------------------------------
// Macros and Defines
// -----------------------------------------------------------------------------

#define MONITOR_STACK_SIZE          ( 2048 )
#define MONITOR_TASK_PRIORITY       ( 15 )
#define MONITOR_TASK_STACK_SIZE     ( MONITOR_STACK_SIZE/4 )

// -----------------------------------------------------------------------------
// Internal function definitions
// -----------------------------------------------------------------------------

static void _monitor_task(void * pvParameters);

// -----------------------------------------------------------------------------
// Project functions
// -----------------------------------------------------------------------------

int32_t monitor_init(void) {
	cpuload_init();
	framerate_init(MONITOR_SCHEDULE_TIME);

	BaseType_t xReturn = xTaskCreate( _monitor_task, "MONITOR", MONITOR_TASK_STACK_SIZE, NULL, MONITOR_TASK_PRIORITY, NULL );
	return xReturn == pdPASS ? 0 : -1;
}

// -----------------------------------------------------------------------------
// Internal functions
// -----------------------------------------------------------------------------

// See the header file for the function documentation
static void _monitor_task(void * pvParameters) {
	TickType_t delay = MONITOR_SCHEDULE_TIME / portTICK_PERIOD_MS;

	while(1) {
		vTaskDelay(delay);

#ifdef CPULOAD_ENABLED
		uint32_t cpuload = cpuload_get();
		MEJ_LOG_INFO("cpuload: %d ", cpuload);
#endif

#if FRAMERATE_ENABLED == 1
		uint32_t fps = framerate_get();
		MEJ_LOG_INFO("fps: %d ", fps);
#endif
	}
}

// -----------------------------------------------------------------------------
// EOF
// -----------------------------------------------------------------------------

#endif // MONITOR_ENABLED == 1

