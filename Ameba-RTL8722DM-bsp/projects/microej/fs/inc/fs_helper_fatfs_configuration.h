/*
 * C
 *
 * Copyright 2021-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 *
 */

#ifndef  FS_HELPER_FATFS_CONFIGURATION_H
#define  FS_HELPER_FATFS_CONFIGURATION_H

/**
 * @file
 * @brief fatfs helper for LLFS.
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 06 October 2020
 */


#include "ff.h"
#include <fatfs_ext/inc/ff_driver.h>    
  
#if (_MAX_SS != 4096)
	#error set _MAX_SS to 4096 in ffconf.h to define maximum supported range of sector size for flash memory. See the description below the MACRO for details.
#endif
#if (_USE_MKFS != 1)
	#error define _USE_MKFS MACRO to 1 in ffconf.h for on-board flash memory to enable f_mkfs() which creates FATFS volume on Flash.
#endif
#include "flash_api.h"
#include <disk_if/inc/flash_fatfs.h>

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @brief Compatibility sanity check value.
 * This define value is checked in the implementation to validate that the version of this configuration
 * is compatible with the implementation.
 *
 * This value must not be changed by the user of the CCO.
 * This value must be incremented by the implementor of the CCO when a configuration define is added, deleted or modified.
 */
#define FS_FATFS_CONFIGURATION_VERSION (1)

/** @brief define the amount of file in private pool module */
#define FS_FATFS_MAX_NUMBER_OF_FILE_IN_POOL  2

/** @brief define the amount of dir in private pool module */
#define FS_FATFS_MAX_NUMBER_OF_DIR_IN_POOL  2

/**
 * @brief Initializes the fatfs.
 *
 * fs_configuration.h header file is provided by the fs-generic cco.
 */
void llfs_fatfs_init(void);

#ifdef __cplusplus
	}
#endif

#endif // FS_HELPER_FATFS_CONFIGURATION_H
