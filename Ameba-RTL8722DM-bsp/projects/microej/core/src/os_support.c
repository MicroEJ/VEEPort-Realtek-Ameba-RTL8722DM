/*
 * C
 *
 * Copyright 2015-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include "FreeRTOS.h"
#include "task.h"
#include "interrupts.h"

// -----------------------------------------------------------------------------
// Project functions
// -----------------------------------------------------------------------------

// See the header file for the function documentation
void OS_SUPPORT_disable_context_switching() {
	if (interrupt_is_in() == MICROEJ_TRUE) {
		taskENTER_CRITICAL_FROM_ISR();
		portSET_INTERRUPT_MASK_FROM_ISR();
	} else {
		taskENTER_CRITICAL();
	}
}

// See the header file for the function documentation
void OS_SUPPORT_enable_context_switching() {
	if (interrupt_is_in() == MICROEJ_TRUE) {
		taskEXIT_CRITICAL_FROM_ISR(0);
		portCLEAR_INTERRUPT_MASK_FROM_ISR( 0 );
	} else {
		taskEXIT_CRITICAL();
	}
}

// -----------------------------------------------------------------------------
// EOF
// -----------------------------------------------------------------------------

