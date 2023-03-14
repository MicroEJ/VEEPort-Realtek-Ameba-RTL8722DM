/*
 * C
 *
 * Copyright 2019 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

#ifndef LLTOUCH_CAL_INTERN
#define LLTOUCH_CAL_INTERN

#ifdef __cplusplus
	extern "C" {
#endif

#define LLTOUCH_CAL_IMPL_is_calibrated Java_ej_microui_touch_TouchCalibration_isCalibrated
#define LLTOUCH_CAL_IMPL_get_points_count Java_ej_microui_touch_TouchCalibration_getPointsCount
#define LLTOUCH_CAL_IMPL_get_point_x Java_ej_microui_touch_TouchCalibration_getPointX
#define LLTOUCH_CAL_IMPL_get_point_y Java_ej_microui_touch_TouchCalibration_getPointY
#define LLTOUCH_CAL_IMPL_calibration_point_touched Java_ej_microui_touch_TouchCalibration_calibrationPointTouched
#define LLTOUCH_CAL_IMPL_calibration_done Java_ej_microui_touch_TouchCalibration_calibrationDone
#define LLTOUCH_CAL_IMPL_clear_calibration Java_ej_microui_touch_TouchCalibration_clearCalibration

#ifdef __cplusplus
	}
#endif

#endif
