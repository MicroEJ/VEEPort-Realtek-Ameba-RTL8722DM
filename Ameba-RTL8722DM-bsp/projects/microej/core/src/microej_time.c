/*
 * C
 *
 * Copyright 2020-2022 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

/* Includes ------------------------------------------------------------------*/

#include "microej_time.h"
#include "microej.h"
#include "timer_api.h"
#include "device.h"
#include "ameba_soc.h"

#define GTIMER_CLK_HZ	(32768)	// clock frequency in Hertz
#define MAX_TIMER_PERIOD_US ((uint32_t)0xFFFFFFFF) // max timer period in us

/* Globals -------------------------------------------------------------------*/
// this value is updated at each interrupt
static volatile int64_t software_counter ;

/** Offset in ms from system time to application time */
static uint64_t microej_application_time_offset = 0;


/* Private functions ---------------------------------------------------------*/

static uint64_t COUNT_TO_USEC(uint32_t tick_cnt)
{	
    uint64_t us_tick;
    us_tick = (uint64_t)(((uint64_t)tick_cnt * 1000000)/GTIMER_CLK_HZ);
    return us_tick;
}

static int64_t timer_get_counter_value(void)
{
    int64_t tc = (int64_t)COUNT_TO_USEC(RTIM_GetCount(TIMx[TIMER2]));
    return tc ;
}

static int64_t time_hardware_timer_getTicks(void)
{
	return software_counter + timer_get_counter_value();
}


static void timer_timeout_handler(uint32_t id)
{
	//update the soft timer value
	software_counter += MAX_TIMER_PERIOD_US ;
	RTIM_INTClear(TIMx[TIMER2]);
}


/**
 *  An interrupt may occur between read of lp_time and hp_time,
 *  this interrupt may modify lp_time,
 *  so, after accessing hp_time, we must check if lp_time has not been modified.
 */
static int64_t time_hardware_timer_getAndCheckTicks(void)
{
    volatile int64_t software_counter_local;
    int64_t timer_counter_local;
    do {
        software_counter_local = software_counter;
        timer_counter_local = timer_get_counter_value();
    } while (software_counter_local != software_counter);

    return software_counter_local + timer_counter_local;
}




/* Public functions ----------------------------------------------------------*/

void microej_time_init(void)
{
	// initialize fields
	software_counter = 0;
	RTIM_TimeBaseInitTypeDef TIM_InitStruct;
	//initial a periodical gimer
	RTIM_TimeBaseStructInit(&TIM_InitStruct);
	TIM_InitStruct.TIM_Idx = TIMER2;
	TIM_InitStruct.TIM_Period = MAX_TIMER_PERIOD_US/1000000*GTIMER_CLK_HZ;
	TIM_InitStruct.TIM_UpdateEvent = ENABLE; /* UEV enable */
	TIM_InitStruct.TIM_UpdateSource = TIM_UpdateSource_Overflow;
	TIM_InitStruct.TIM_ARRProtection = ENABLE;
	RTIM_TimeBaseInit(TIMx[TIMER2], (&TIM_InitStruct), TIMx_irq[TIMER2], (IRQ_FUN) timer_timeout_handler, 0);
	RTIM_INTConfig(TIMx[TIMER2], TIM_IT_Update, ENABLE);
	RTIM_Cmd(TIMx[TIMER2], ENABLE);
}

int64_t microej_time_get_current_time(uint8_t is_platform_time)
{
	/*
	 * /!\
	 * is_platform_time == true when ej.bon.Util.platformTimeMillis
	 * is_platform_time == false when java.lang.System.currentTimeMillis
	 * /!\
	 */
	int64_t systemTime = time_hardware_timer_getAndCheckTicks() / 1000;
	if (is_platform_time == MICROEJ_TRUE) {
		return systemTime;
	} else {
		return systemTime + microej_application_time_offset;
	}

}

int64_t microej_time_get_time_nanos(void)
{
    return time_hardware_timer_getAndCheckTicks()*1000 ;
}


void microej_time_set_application_time(int64_t time)
{
	int64_t currentTime = (int64_t)microej_time_get_current_time(MICROEJ_TRUE);
	microej_application_time_offset = time - currentTime;
}

/*
 * Convert a time in milliseconds in ticks.
 * Result is round up to the next tick (ie. converting back the resulting ticks
 * to milliseconds results in a value greater than or equals to given time).
 * Return a saturated number of ticks or 0 if given time is negative.
 */
int64_t microej_time_time_to_tick(int64_t time){
	if (time < 0) {
			return 0;
	}

	int64_t mticks = time * (int64_t)configTICK_RATE_HZ; // milli-ticks

	// Check for no overflow
	if (mticks >= 0) {
		int64_t ticks64 = (mticks + 999LL) / 1000LL;

		// Check for no overflow
		if (ticks64 >= 0) {
			portTickType ticks = (portTickType)ticks64;

			// Check for no overflow
			if (ticks == ticks64) {
				return (int64_t)ticks;
			}
		}
	}

	// An overflow occurs: saturate the value to the max value for a portTickType
	return (int64_t)portMAX_DELAY;
}

