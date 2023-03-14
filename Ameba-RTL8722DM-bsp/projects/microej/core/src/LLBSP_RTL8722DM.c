/*
 * C
 *
 * Copyright 2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

/* Includes ------------------------------------------------------------------*/

#include "LLBSP_impl.h"
#include "microej.h"
#include <stdio.h>

extern void* SOAR_START;
extern void* SOAR_END;

/* Defines ------------------------------------------------------------------*/
/**
 * Memory settings on Ameba RTL8722DM
 */
#define MEM_SRAM_START (0x10000000)
#define MEM_SRAM_END   (0x1007FFFF)
#define MEM_PSRAM_START (0x02000000)
#define MEM_PSRAM_END   (0x023FFFFF)

/*******************************************************************************
 * Function Name: LLBSP_IMPL_isInReadOnlyMemory
 * Description  : checks if the given pointer is in a read only memory or not.
 *******************************************************************************/
uint8_t LLBSP_IMPL_isInReadOnlyMemory(void* ptr)
{
	if(
	 ((uint32_t)ptr >= MEM_SRAM_START && (uint32_t)ptr < MEM_SRAM_END) ||
      ((uint32_t)ptr >= MEM_PSRAM_START && (uint32_t)ptr < MEM_PSRAM_END)
	)
		return MICROEJ_FALSE;
	else
		return MICROEJ_TRUE;
}

/*******************************************************************************
 * Function Name: LLBSP_IMPL_putchar
 * Description  : puts the character to stdout stream.
 *******************************************************************************/
void LLBSP_IMPL_putchar(int32_t c)
{
	putchar(c);
}

/* End of File */
