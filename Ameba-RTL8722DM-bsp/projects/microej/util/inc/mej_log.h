/*
 * C
 *
 * Copyright 2019-2020 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

#if !defined __MEJ_LOG_H__
# define __MEJ_LOG_H__

#include <stdio.h>

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Macros and Defines
// -----------------------------------------------------------------------------

/*
 * @brief Print a message on the debug console
 */
# define MEJ_LOG_DBG_MSG(tag, fmt, ...)	printf("[" tag "]: %s: " fmt "\n", __func__, ##__VA_ARGS__)

/*
 * @brief Print an informative message on the debug console
 */
# define MEJ_LOG_INFO(fmt, ...)			MEJ_LOG_DBG_MSG("INFO", fmt, __VA_ARGS__);

/*
 * @brief Print a trace message on the debug console
 */
# define MEJ_LOG_TRACE(fmt, ...)		MEJ_LOG_DBG_MSG("TRACE", fmt, __VA_ARGS__);

/*
 * @brief Print a warning message on the debug console
 */
# define MEJ_LOG_WARNING(fmt, ...)		MEJ_LOG_DBG_MSG("WARNING", fmt, __VA_ARGS__);

/*
 * @brief Print an error message on the debug console
 */
# define MEJ_LOG_ERROR(fmt, ...)		MEJ_LOG_DBG_MSG("ERROR", fmt, __VA_ARGS__);

/*
 * @brief Print a critical message on the debug console and stop the current task execution
 */
# define MEJ_LOG_CRITICAL(fmt, ...)			\
	do {									\
		MEJ_LOG_ERROR(fmt, __VA_ARGS__);	\
        while (1);							\
	} while (0)

#endif // !defined __MEJ_LOG_H__

// -----------------------------------------------------------------------------
// EOF
// -----------------------------------------------------------------------------
