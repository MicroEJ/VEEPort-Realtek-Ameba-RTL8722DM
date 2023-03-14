/*
 * C
 *
 * Copyright 2021 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

#ifndef LLUI_VECTORFONT_PAINTER_IMPL
#define LLUI_VECTORFONT_PAINTER_IMPL

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
 * @brief Use SNI library to declare its native methods.
 */
#include "sni.h"

/*
 * @brief Needed for MICROUI_GraphicsContext
 */
#include "LLUI_DISPLAY.h"

// --------------------------------------------------------------------------------
// Native function name redefinition
// --------------------------------------------------------------------------------

#define LLVECTOR_FONT_IMPL_draw_char Java_ej_vector_font_VectorFontPainterNatives_drawChar
#define LLVECTOR_FONT_IMPL_draw_string Java_ej_vector_font_VectorFontPainterNatives_drawString
#define LLVECTOR_FONT_IMPL_draw_string_arc Java_ej_vector_font_VectorFontPainterNatives_drawStringArc

// --------------------------------------------------------------------------------
// Functions that must be implemented
// --------------------------------------------------------------------------------

/*
 * @brief Draws a character with the given font and given size.
 *
 * Before drawing the character outline will be transformed with the given
 * matrix object.
 *
 * The top/left position of the character is the origin of the transformation.
 *
 * The character are filled with the current GraphicsContext color or
 * with the given gradient object.
 *
 * If the gradient object handle is null, the opacity is the given alpha.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] faceHandle the font reference handle.
 * @param[in] size the height of the font in pixels.
 * @param[in] x horizontal translation offset.
 * @param[in] y vertical translation offset.
 * @param[in] matrix_data the array of the transformation matrix parameters.
 * @param[in] alpha the opacity level to apply to the character.
 * @param[in] gradientHandle the handle of the gradient object.
 *
 */
void LLVECTOR_FONT_IMPL_draw_char(MICROUI_GraphicsContext* gc, jint faceHandle, jfloat size, jchar character, jint x, jint y, jfloat* matrix_data, int alpha, jint gradientHandle);

/*
 * @brief Draws a string with the given font and given size.
 *
 * The top/left position of the first drawn character is placed at
 * coordinate x/y.
 *
 * The characters are filled with the current GraphicsContext color or
 * with the given gradient object.
 *
 * A subset of the given string can be drawn using offset and lenght.
 *
 * If the gradient object handle is null, the opacity is the given alpha.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] faceHandle the font reference handle.
 * @param[in] size the height of the font in pixels.
 * @param[in] text the array of characters to draw.
 * @param[in] offset the offset in text of the first character to draw.
 * @param[in] length the number of characters to draw.
 * @param[in] x the horizontal coordinate of the top/left of the first drawn character.
 * @param[in] y the vertical coordinate of the top/left of the first drawn character.
 * @param[in] alpha the opacity level to apply to the character.
 * @param[in] gradientHandle the handle of the gradient object.
 *
 */
void LLVECTOR_FONT_IMPL_draw_string(MICROUI_GraphicsContext* gc, jint faceHandle, jfloat size, jchar* text, jint offset, jint length, jint x, jint y, jint alpha, jint gradientHandle);

/*
 * @brief Draws a string around a circle with the given font and given size.
 *
 * If out is false, the string will be drawn inside the circle (the top/left point of the
 * characters is on the circle outline) starting at the given angle following clockwise
 * direction.
 *
 * If out is true, the string will be drawn inside the circle (the bottom/left point of the
 * characters is on the circle outline) starting at the given angle following counter-clockwise
 * direction.
 *
 * The characters are filled with the current GraphicsContext color or
 * with the given gradient object.
 *
 * A subset of the given string can be drawn using offset and lenght.
 *
 * If the gradient object handle is null, the opacity is the given alpha.
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] faceHandle the font reference handle.
 * @param[in] size the height of the font in pixels.
 * @param[in] text the array of characters to draw.
 * @param[in] offset the offset in text of the first character to draw.
 * @param[in] length the number of characters to draw.
 * @param[in] x the horizontal coordinate of the circle top-left point.
 * @param[in] y the vertical coordinate of the circle top-left point.
 * @param[in] diameter the diameter of the circle.
 * @param[in] angle_degreesthe angle in degrees at which the string starts.
 * @param[in] alpha the opacity level to apply to the character.
 * @param[in] gradientHandle the handle of the gradient object.
 * @param[in] out tells whether the text should be written using the circle outline as bottom of the string.
 *
 */
void LLVECTOR_FONT_IMPL_draw_string_arc(MICROUI_GraphicsContext* gc, jint faceHandle, jfloat size, jchar* text, jint offset, jint length, jint x, jint y, jint diameter, jfloat angle_degrees, jint alpha, jint gradientHandle, bool out);


#ifdef __cplusplus
}
#endif
#endif
