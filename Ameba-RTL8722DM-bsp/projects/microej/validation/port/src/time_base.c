/*
 * C
 *
 * Copyright 2014-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "timer_api.h"

#include "../../framework/c/utils/inc/u_time_base.h"
#include "../../framework/c/utils/inc/bsp_util.h"
#include "../../framework/c/embunit/embunit/config.h"
/* External constant declarations */
extern gtimer_t PQT_TIMER;

/* External function definitions */
extern void PQT_Timer_init();

/* Private constant declarations */



/* private variable definitions */


/* public function definitions */

void UTIL_TIME_BASE_initialize(void)
{
    PQT_Timer_init();
}

int64_t UTIL_TIME_BASE_getTime(void)
{
	uint32_t tick_cnt = gtimer_read_tick(&PQT_TIMER);
	int64_t us_tick = tick_cnt * (1000000/32768);
	return us_tick;
}

/** private function definitions */
