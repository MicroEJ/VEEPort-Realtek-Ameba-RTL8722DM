/*
 * C
 *
 * Copyright 2014-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "x_core_benchmark.h"
#include "bsp_util.h"

#include "coremark.h"

/** public function default definitions */

/**
 * @brief
 *
 * @return
 */
bool X_CORE_BENCHMARK_run(void)
{
	/* Call the Coremark entry point */
	return  core_main()==0;
}
