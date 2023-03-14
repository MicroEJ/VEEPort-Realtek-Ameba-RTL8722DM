/*
 * C
 *
 * Copyright 2014-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "x_ram_checks.h"
#include "bsp_util.h"

extern void* _java_heap_start;
extern void* _java_heap_end;

#define __ZONE_SIZE		(uint32_t)((((uint32_t)&_java_heap_end) - ((uint32_t)&_java_heap_start)) / 2)
#define __DST_ZONE_START	((uint32_t) (&_java_heap_start))
#define __DST_ZONE_END		(__DST_ZONE_START + __ZONE_SIZE)
#define __SRC_ZONE_START	(__DST_ZONE_END)
#define __SRC_ZONE_END		(uint32_t)(&_java_heap_end)

static X_RAM_CHECKS_zone_t __src_zone;

static X_RAM_CHECKS_zone_t __dst_zone;

static inline X_RAM_CHECKS_zone_t* __init_zone(X_RAM_CHECKS_zone_t* zone, uint8_t isSrcZone) {
    if(isSrcZone){
        zone->start_address = __SRC_ZONE_START;
	zone->end_address = __SRC_ZONE_END;
    }else{
        zone->start_address = __DST_ZONE_START;
	zone->end_address = __DST_ZONE_END;   
    }
    return zone;
}
/* public function default definitions */

/**
 *
 * @brief  this function provide a definition array of  memory zones to test with 32-bit accesses.
 *
 * @return array of X_RAM_CHECKS_zone_t
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get32bitZones(void)
{
	return __init_zone(&__dst_zone, 0);
}

/**
 * @brief this function provide a definition array of  memory zones to test with 16-bit accesses.
 *
 * @return array of X_RAM_CHECKS_zone_t
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get16bitZones(void)
{
	return __init_zone(&__dst_zone, 0);
}

/**
 * @brief
 *
 * @return array of X_RAM_CHECKS_zone_t
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get8bitZones(void)
{
	return __init_zone(&__dst_zone, 0);
}

/**
 * @brief
 *
 * @return number of zones to test
 */
uint8_t X_RAM_CHECKS_get32bitZoneNumber(void)
{
	return 1;
}

/**
 * @brief
 *
 * @return
 */
uint8_t X_RAM_CHECKS_get16bitZoneNumber(void)
{
	return 1;
}

/**
 * @brief
 *
 * @return
 */
uint8_t X_RAM_CHECKS_get8bitZoneNumber(void)
{
	return 1;
}

/**
 * @brief
 *
 * @return
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get32bitSourceZone(void)
{
	return __init_zone(&__src_zone, 1);
}

/**
 * @brief
 *
 * @return
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get16bitSourceZone(void)
{
	return __init_zone(&__src_zone, 1);
}

/**
 * @brief
 *
 * @return
 */
X_RAM_CHECKS_zone_t* X_RAM_CHECKS_get8bitSourceZone(void)
{
	return __init_zone(&__src_zone, 1);
}
