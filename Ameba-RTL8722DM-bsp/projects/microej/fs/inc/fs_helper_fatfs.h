/*
 * C
 *
 * Copyright 2020-2021 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 *
 */

#ifndef  FS_HELPER_FATFS_H
#define  FS_HELPER_FATFS_H

/**
 * @file
 * @brief fatfs helper for LLFS.
 * @author MicroEJ Developer Team
 * @version 1.0.0
 * @date 30 December 2022
 */

#ifdef __cplusplus
	extern "C" {
#endif

/* File attribute bits  */
#define	AM_RDO	0x01	/* Read only */
#define	AM_HID	0x02	/* Hidden */
#define	AM_SYS	0x04	/* System */
#define AM_DIR	0x10	/* Directory */
#define AM_ARC	0x20	/* Archive */


#ifdef __cplusplus
	}
#endif

#endif // FS_HELPER_FATFS_H