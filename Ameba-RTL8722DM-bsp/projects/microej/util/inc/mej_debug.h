/*
 * C
 *
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

#if !defined __MEJ_DEBUG_H__
# define __MEJ_DEBUG_H__

#include <stdint.h>

// --------------------------------------------------------------------------------
// Macros and Defines
// --------------------------------------------------------------------------------

#define MEJ_DEBUG_ENABLED

#define MEJ_DEBUG_LVL(i)        ( 1 << (i) )

#define MEJ_DEBUG_DISPLAY_TRANSFER_LVL      MEJ_DEBUG_LVL(1)
#define MEJ_DEBUG_LLDISPLAY_LVL             MEJ_DEBUG_LVL(2)
#define MEJ_DEBUG_TOUCH_MANAGER_LVL         MEJ_DEBUG_LVL(3)

#if defined MEJ_DEBUG_ENABLED
#define MEJ_DEBUG(lvl, fmt, ...) \
	if (MEJ_DEBUG_ ## lvl ## _LVL & __mej_debug_lvls) { \
		printf("%s: " fmt "\n", __func__, ##__VA_ARGS__); \
	}

#define MEJ_DEBUG_ENABLE(lvl) __mej_debug_enable(MEJ_DEBUG_ ## lvl ## _LVL);
#define MEJ_DEBUG_DISABLE(lvl) __mej_debug_disable(MEJ_DEBUG_ ## lvl ## _LVL);

#else
#define MEJ_DEBUG(...)
#define MEJ_DEBUG_ENABLE(lvl)
#define MEJ_DEBUG_DISABLE(lvl)
#endif

// --------------------------------------------------------------------------------
// Globals
// --------------------------------------------------------------------------------
extern uint32_t __mej_debug_lvls;

// --------------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------------
void __mej_debug_enable(int level);
void __mej_debug_disable(int level);

#endif // !defined __MEJ_DEBUG_H__
