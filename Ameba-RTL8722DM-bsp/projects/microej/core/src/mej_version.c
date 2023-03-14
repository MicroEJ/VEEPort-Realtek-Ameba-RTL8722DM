/*
 * C
 *
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

/*
 * build_info.h is updated at each build
 * Including build_info.h should ensure rebuild of the source file
 */
#include "build_info.h"

#include "mej_log.h"

void mej_version_print_build_info(void) {
	MEJ_LOG_INFO("Build info:");
	MEJ_LOG_INFO("\t" "date: " __DATE__);
	MEJ_LOG_INFO("\t" "time: " __TIME__);

	MEJ_LOG_INFO("\t" "compiler: " RTL_FW_COMPILER);
	MEJ_LOG_INFO("\t" "compiler version: %d", __VER__);
}
