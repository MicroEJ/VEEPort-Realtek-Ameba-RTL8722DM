/*
 * C
 *
 * Copyright 2014-2021 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

/*
 * Implementation for FreeRTOS
 */

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include "FreeRTOS.h"

#include "cpuload_conf.h"
#ifdef CPULOAD_ENABLED

#include "FreeRTOS.h"
#include "task.h"
#include "cpuload_impl.h"
#include "mej_math.h"

#if (configUSE_IDLE_HOOK == 0)
	#error configUSE_IDLE_HOOK must be defined in FreeRTOSConfig.h and equals to 1 when CPULOAD_ENABLED defined.
#endif
#endif	// CPULOAD_ENABLED

// -----------------------------------------------------------------------------
// Macros and Defines
// -----------------------------------------------------------------------------

#define CPULOAD_STACK_SIZE			( 512 )
#define CPULOAD_TASK_PRIORITY		( configTIMER_TASK_PRIORITY - 1 )
#define CPULOAD_TASK_STACK_SIZE		CPULOAD_STACK_SIZE/4

// -----------------------------------------------------------------------------
// Internal function definitions
// -----------------------------------------------------------------------------

#ifdef CPULOAD_ENABLED

/*
 * @brief Worker task that samples the CPU load
 *
 * @param[in] parameters: unused
 */
static void _cpuload_task(void * parameters);

#endif	// CPULOAD_ENABLED

// -----------------------------------------------------------------------------
// Project functions
// -----------------------------------------------------------------------------

#ifdef CPULOAD_ENABLED

// See the header file for the function documentation
int32_t cpuload_impl_start_idle_task(void) {
	return 0;	// nothing to do (no error)
}

// See the header file for the function documentation
int32_t cpuload_impl_start_task(void) {
	BaseType_t xReturn = xTaskCreate( _cpuload_task, "CPULOAD", CPULOAD_TASK_STACK_SIZE, NULL, CPULOAD_TASK_PRIORITY, NULL );
	return xReturn == pdPASS ? 0 : -1;
}

// See the header file for the function documentation
void cpuload_impl_sync_os_tick(void) {
	TickType_t os_tick = xTaskGetTickCount();
	while(os_tick == xTaskGetTickCount());
}

// See the header file for the function documentation
void cpuload_impl_sleep(uint32_t ms) {
	TickType_t ticks = ms / portTICK_PERIOD_MS;
	vTaskDelay(ticks ? ticks : 1);          /* Minimum delay = 1 tick */
}

// See the header file for the function documentation
void vApplicationIdleHook(void) {
	cpuload_idle();
}

/*
 * @brief Disable interrupts.
 *
 */
void cpuload_impl_disable_interrupts(void){
	__disable_irq();
}

/*
 * @brief Enable interrupts.
 *
 */
void cpuload_impl_enable_interrupts(void){
	__enable_irq();
}
#else

/*
 * If the CPU load function disabled and the FreeRTOS IDLE_HOOK is enabled
 * the vApplicationIdleHook must be defined
 */

# if configUSE_IDLE_HOOK != 0
// See the header file for the function documentation
void vApplicationIdleHook(void) {
}
# endif

#endif	// CPULOAD_ENABLED

// -----------------------------------------------------------------------------
// Internal functions
// -----------------------------------------------------------------------------

#ifdef CPULOAD_ENABLED
// See the section 'Internal function definitions' for the function documentation
static void _cpuload_task(void * parameters) {
	// launch cpuload job
	cpuload_task();
	// job end, cleanup resources
	vTaskDelete(xTaskGetCurrentTaskHandle());
}
#endif	// CPULOAD_ENABLED

// -----------------------------------------------------------------------------
// EOF
// -----------------------------------------------------------------------------

