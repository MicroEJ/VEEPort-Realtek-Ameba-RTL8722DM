/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*            (c) 1995 - 2019 SEGGER Microcontroller GmbH             *
*       Copyright 2017-2020 MicroEJ Corp. All rights reserved.                                                       *
*
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
*       SEGGER SystemView * Real-time application analysis           *
*                                                                    *
**********************************************************************
*                                                                    *
* All rights reserved.                                               *
*                                                                    *
* SEGGER strongly recommends to not make any changes                 *
* to or modify the source code of this software in order to stay     *
* compatible with the SystemView and RTT protocol, and J-Link.       *
*                                                                    *
* Redistribution and use in source and binary forms, with or         *
* without modification, are permitted provided that the following    *
* condition is met:                                                  *
*                                                                    *
* o Redistributions of source code must retain the above copyright   *
*   notice, this condition and the following disclaimer.             *
*                                                                    *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND             *
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,        *
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF           *
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
* DISCLAIMED. IN NO EVENT SHALL SEGGER Microcontroller BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR           *
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT  *
* OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;    *
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF      *
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT          *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE  *
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH   *
* DAMAGE.                                                            *
*                                                                    *
**********************************************************************
*                                                                    *
*       SystemView version: V3.12                                    *
*                                                                    *
**********************************************************************
-------------------------- END-OF-HEADER -----------------------------

File    : SEGGER_SYSVIEW_Config_MicroEJ_FreeRTOS.c
Purpose : Setup configuration of SystemView with MicroEJ and FreeRTOS.
*/
#include "SEGGER_SYSVIEW.h"
#include "FreeRTOS.h"
#include "LLMJVM_MONITOR_SYSVIEW.h"
#include "LLTRACE_SYSVIEW_configuration.h"
#include "timer_api.h"

extern const SEGGER_SYSVIEW_OS_API SYSVIEW_X_OS_TraceAPI;

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/
// The application name to be displayed in SystemViewer
#define SYSVIEW_APP_NAME        "MicroEJ Application"

// The target device name
#define SYSVIEW_DEVICE_NAME     "Cortex-M33"

// Frequency of the timestamp. Must match SEGGER_SYSVIEW_GET_TIMESTAMP in SEGGER_SYSVIEW_Conf.h
#define SYSVIEW_TIMESTAMP_FREQ  (configCPU_CLOCK_HZ)

// System Frequency. SystemcoreClock is used in most CMSIS compatible projects.
#define SYSVIEW_CPU_FREQ        configCPU_CLOCK_HZ

// The lowest RAM address used for IDs (pointers)
#define SYSVIEW_RAM_BASE        (0x10005000)

/* Registers used in SEGGER_SYSVIEW_X_GetTimestamp function to calculate time needed by SystemView*/
#define SCB_ICSR  (*(volatile U32*) (0xE000ED04uL)) // Interrupt Control State Register
#define SCB_ICSR_PENDSTSET_MASK     (1UL << 26)     // SysTick pending bit
#define SYST_RVR  (*(volatile U32*) (0xE000E014uL)) // SysTick Reload Value Register
#define SYST_CVR  (*(volatile U32*) (0xE000E018uL)) // SysTick Current Value Register

/********************************************************************* 
*
*       _cbSendSystemDesc()
*
*  Function description
*    Sends SystemView description strings.
*/
static void _cbSendSystemDesc(void) {
	SEGGER_SYSVIEW_SendSysDesc("N="SYSVIEW_APP_NAME",D="SYSVIEW_DEVICE_NAME",O=MicroEJ");
	SEGGER_SYSVIEW_SendSysDesc("N="SYSVIEW_APP_NAME",D="SYSVIEW_DEVICE_NAME",O=FreeRTOS");
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

SEGGER_SYSVIEW_OS_API SYSVIEW_MICROEJ_X_OS_TraceAPI;

static void SYSVIEW_MICROEJ_X_OS_SendTaskList(void){
  SYSVIEW_X_OS_TraceAPI.pfSendTaskList();
  LLMJVM_MONITOR_SYSTEMVIEW_send_task_list();
}

void SEGGER_SYSVIEW_Conf(void) {
  SYSVIEW_MICROEJ_X_OS_TraceAPI.pfGetTime = SYSVIEW_X_OS_TraceAPI.pfGetTime;
  SYSVIEW_MICROEJ_X_OS_TraceAPI.pfSendTaskList = SYSVIEW_MICROEJ_X_OS_SendTaskList;

  SEGGER_SYSVIEW_Init(SYSVIEW_TIMESTAMP_FREQ, SYSVIEW_CPU_FREQ,
                      &SYSVIEW_MICROEJ_X_OS_TraceAPI, _cbSendSystemDesc);
  SEGGER_SYSVIEW_SetRAMBase(SYSVIEW_RAM_BASE);
}

/*********************************************************************
*
*       SEGGER_SYSVIEW_X_GetTimestamp()
*
* Function description
*   Returns the current timestamp in ticks using the system tick
*   count and the SysTick counter.
*   All parameters of the SysTick have to be known and are set via
*   configuration defines on top of the file.
*
* Return value
*   The current timestamp.
*
* Additional information
*   SEGGER_SYSVIEW_X_GetTimestamp is always called when interrupts are
*   disabled. Therefore locking here is not required.
*/
extern unsigned int SEGGER_SYSVIEW_TickCnt;
U32 SEGGER_SYSVIEW_X_GetTimestamp(void) {
#if defined ENABLE_SYSTEM_VIEW
  uint32_t TickCount;
  uint32_t Cycles;
  uint32_t CyclesPerTick;
  //
  // Get the cycles of the current system tick.
  // SysTick is down-counting, subtract the current value from the number of cycles per tick.
  //
  CyclesPerTick = SYST_RVR + 1;
  Cycles = (CyclesPerTick - SYST_CVR);
  //
  // Get the system tick count.
  //
  TickCount = SEGGER_SYSVIEW_TickCnt;
  //
  // If a SysTick interrupt is pending increment the TickCount
  //
  if ((SCB_ICSR & SCB_ICSR_PENDSTSET_MASK) != 0) {
    TickCount++;
  }
  Cycles += TickCount * CyclesPerTick;

  return Cycles;
#endif
}

/*************************** End of file ****************************/
