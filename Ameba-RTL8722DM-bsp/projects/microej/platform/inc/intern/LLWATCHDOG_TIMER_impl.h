/*
 * C
 *
 * Copyright 2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */
#ifndef LLWATCHDOG_TIMER_IMPL_INTERN_H
#define LLWATCHDOG_TIMER_IMPL_INTERN_H
#ifdef __cplusplus
extern "C" {
#endif

#define LLWATCHDOG_TIMER_IMPL_init					Java_ej_hal_WatchdogTimer_nativeInit
#define LLWATCHDOG_TIMER_IMPL_start					Java_ej_hal_WatchdogTimer_nativeStart
#define LLWATCHDOG_TIMER_IMPL_stop					Java_ej_hal_WatchdogTimer_nativeStop
#define LLWATCHDOG_TIMER_IMPL_registerCheckpoint	Java_ej_hal_WatchdogTimer_nativeRegisterCheckpoint
#define LLWATCHDOG_TIMER_IMPL_unregisterCheckpoint	Java_ej_hal_WatchdogTimer_nativeUnregisterCheckpoint
#define LLWATCHDOG_TIMER_IMPL_checkpoint			Java_ej_hal_WatchdogTimer_nativeCheckpoint
#define LLWATCHDOG_TIMER_IMPL_isResetCause			Java_ej_hal_WatchdogTimer_nativeIsResetCause
#define LLWATCHDOG_TIMER_IMPL_getWatchdogTimeoutMs	Java_ej_hal_WatchdogTimer_nativeGetWatchdogTimeoutMs

#ifdef __cplusplus
}
#endif
#endif // LLWATCHDOG_TIMER_IMPL_INTERN_H
