/*
 * C
 *
 * Copyright 2020-2021 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 * 
 */

#ifndef LLUI_VECTORFONT_IMPL
#define LLUI_VECTORFONT_IMPL

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

#define LLVECTOR_FONT_IMPL_load_font Java_ej_vector_font_VectorFontNatives_loadTrueTypeFont
#define LLVECTOR_FONT_IMPL_char_width Java_ej_vector_font_VectorFontNatives_charWidth
#define LLVECTOR_FONT_IMPL_string_width Java_ej_vector_font_VectorFontNatives_stringWidth
#define LLVECTOR_FONT_IMPL_get_baseline_position Java_ej_vector_font_VectorFontNatives_getBaselinePosition
#define LLVECTOR_FONT_IMPL_get_glyph_height Java_ej_vector_font_VectorFontNatives_getGlyphHeight
#define LLVECTOR_FONT_IMPL_get_glyph_width Java_ej_vector_font_VectorFontNatives_getGlyphWidth

// --------------------------------------------------------------------------------
// Functions that must be implemented
// --------------------------------------------------------------------------------

/*
 * @brief Initializes the lowlevel font library.
 *
 */
void LLVECTOR_FONT_init();

/*
 * @brief Loads a font in memory.
 *
 * The font file must be provided to the lowlevel font library.
 *
 * @param[in] font_name the font file name.
 *
 * @return the reference handle of the font.
 */
jint LLVECTOR_FONT_IMPL_load_font(char* font_name);

/*
 * @brief Gets the width in pixels of the given character with this font for the given height.
 *
 * The width is the horizontal distance between the character origin and the next character origin.
 *
 * @param[in] faceHandle the font reference handle.
 * @param[in] size the height of the font in pixels.
 * @param[in] character the character to measure.
 *
 * @return the width in pixel of the given character.
 */
jint LLVECTOR_FONT_IMPL_char_width(jint faceHandle, jfloat size, jchar character);

/*
 * @brief Gets the width in pixels of the given character string with this font for the given height.
 *
 * The width is the horizontal distance that would be occupied if the string was drawn using this font
 * and this size.
 *
 * @param[in] faceHandle the font reference handle.
 * @param[in] size the height of the font in pixels.
 * @param[in] character the character to measure.
 *
 * @return the width in pixel of the given string.
 */
jint LLVECTOR_FONT_IMPL_string_width(jint faceHandle, jfloat size, jchar* text);

/*
 * @brief Gets the vertical distance in pixels between the top of this font and its baseline for
 * the given height.
 *
 * @param[in] faceHandle the font reference handle.
 * @param[in] size the height of the font in pixels.
 *
 * @return the distance in pixel from top to the baseline position.
 */
jint LLVECTOR_FONT_IMPL_get_baseline_position(jint faceHandle, jfloat size);

/*
 * @brief Gets the height in pixels of the given character's glyph for the given font size.
 *
 * @param[in] faceHandle the font reference handle.
 * @param[in] size the height of the font in pixels.
 * @param[in] c the character to measure.
 *
 * @return the height in pixel of the character glyph.
 */
jint LLVECTOR_FONT_IMPL_get_glyph_height(jint faceHandle, jfloat size, jchar c);

/*
 * @brief Gets the width in pixels of the given character's glyph for the given font size.
 *
 * @param[in] faceHandle the font reference handle.
 * @param[in] size the height of the font in pixels.
 * @param[in] c the character to measure.
 *
 * @return the width in pixel of the character glyph.
 */
jint LLVECTOR_FONT_IMPL_get_glyph_width(jint faceHandle, jfloat size, jchar c);

#ifdef __cplusplus
}
#endif
#endif
