/*
 * C
 *
 * Copyright 2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

#ifndef LLTOUCH_CAL_IMPL
#define LLTOUCH_CAL_IMPL

#include <stdint.h>
#include "intern/LLTOUCH_CAL_intern.h"

#ifdef __cplusplus
	extern "C" {
#endif

/**
 * @return <code>MICROEJ_TRUE</code> if the touch is calibrated, <code>MICROEJ_FALSE</code> otherwise.
 */
uint8_t LLTOUCH_CAL_IMPL_is_calibrated(void);

/**
 * Returns the number of calibration point to configure to calibrate the touch screen.
 * <p>
 * Every calibration point is identified by an ID starting from 0 to <code>LLTOUCH_CAL_IMPL_get_points_count()-1</code>.
 *
 * @return the number of calibration point.
 */
int32_t LLTOUCH_CAL_IMPL_get_points_count(void);

/**
 * Returns the display x coordinate of the calibration point identified by the given ID.
 *
 * @param id identifier of the calibration point.
 *
 * @return display x coordinate or -1 if id is invalid.
 */
int32_t LLTOUCH_CAL_IMPL_get_point_x(int32_t id);

/**
 * Returns the display y coordinate of the calibration point identified by the given ID.
 *
 * @param id identifier of the calibration point.
 *
 * @return display y coordinate or -1 if id is invalid.
 */
int32_t LLTOUCH_CAL_IMPL_get_point_y(int32_t id);

/**
 * Notifies the calibration system that a calibration point has been touched.
 *
 * @param id identifier of the calibration point.
 */
void LLTOUCH_CAL_IMPL_calibration_point_touched(int32_t id);

/**
 * Notifies the system that the calibration is fully done and can be stored.
 */
void LLTOUCH_CAL_IMPL_calibration_done(void);

/**
 * Clear calibration configuration. Successive call to <code>LLTOUCH_CAL_IMPL_is_calibrated()</code> will return <code>MICROEJ_FALSE</code>.
 */
void LLTOUCH_CAL_IMPL_clear_calibration(void);

#ifdef __cplusplus
	}
#endif

#endif
