/*
 * C
 *
 * Copyright 2021 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

#ifndef LLUI_VECTORFONT_GRADIENT_IMPL
#define LLUI_VECTORFONT_GRADIENT_IMPL

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
 * @brief Use SNI library to declare its native methods.
 */
#include "sni.h"

// --------------------------------------------------------------------------------
// Native function name redefinition
// --------------------------------------------------------------------------------

#define LLVECTOR_FONT_IMPL_init_gradient Java_ej_vector_font_LinearGradientNatives_init
#define LLVECTOR_FONT_IMPL_gradient_translate Java_ej_vector_font_LinearGradientNatives_translate
#define LLVECTOR_FONT_IMPL_close_gradient Java_ej_vector_font_LinearGradientNatives_close

// --------------------------------------------------------------------------------
// Functions that must be implemented
// --------------------------------------------------------------------------------

/*
 * @brief Initializes a gradient object.
 *
 * The gradient feature is optional. If the gradient feature is not supported the
 * init function has to return 0. The gradient operations (translate and close) are no-op.
 * The drawing functions with gradient are then redirected to the draw without gradient.
 *
 * @param[in] start_x the x coordinate of the start of the linear gradient vector.
 * @param[in] start_y the y coordinate of the start of the linear gradient vector.
 * @param[in] end_x the x coordinate of the end of the linear gradient vector.
 * @param[in] end_y the y coordinate of the end of the linear gradient vector.
 * @param[in] fractions the array of fractions use as gradient stop points.
 * @param[in] colors the array of colors associated with each gradient stop points.
 *
 * @return the handle of the gradient object.
 */
jint LLVECTOR_FONT_IMPL_init_gradient(jint start_x, jint start_y, jint end_x, jint end_y, jint* fractions, jint* colors);

/*
 * @brief Translates a gradient object.
 *
 * @param[in] gradientHandle the handle of the gradient object.
 * @param[in] x the horizontal translation offset.
 * @param[in] y the vertical translation offset.
 *
 */
void LLVECTOR_FONT_IMPL_gradient_translate(jint gradientHandle, jint x, jint y);

/*
 * @brief Closes a gradient object.
 *
 * @param[in] gradientHandle the handle of the gradient object.
 *
 */
void LLVECTOR_FONT_IMPL_close_gradient(jint gradientHandle);


#ifdef __cplusplus
}
#endif
#endif
