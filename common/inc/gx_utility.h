/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Utility (Utility)                                                   */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_utility.h                                        PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX utility component,                       */
/*    including all data types and external references.  It is assumed    */
/*    that gx_api.h and gx_port.h have already been included.             */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            added new prototype,        */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s), added    */
/*                                            bidi test reordering APIs,  */
/*                                            resulting in version 6.1.3  */
/*  06-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            rename RENESAS_DAVE2D       */
/*                                            support conditional,        */
/*                                            resulting in version 6.1.7  */
/*  01-31-2022     Ting Zhu                 Modified comment(s),          */
/*                                            added new bidi text         */
/*                                            reordering API,             */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/

#ifndef GX_UTILITY_H
#define GX_UTILITY_H

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
/* Define bidi character types. */
#define GX_BIDI_CHARACTER_TYPE_L     0x01 /* Left to Right */
#define GX_BIDI_CHARACTER_TYPE_R     0x02 /* Right to Left */
#define GX_BIDI_CHARACTER_TYPE_AL    0x03 /* Right to Left Arabic */
#define GX_BIDI_CHARACTER_TYPE_EN    0x04 /* European Number */
#define GX_BIDI_CHARACTER_TYPE_ES    0x05 /* European Number Separator */
#define GX_BIDI_CHARACTER_TYPE_ET    0x06 /* European Number Terminator */
#define GX_BIDI_CHARACTER_TYPE_AN    0x07 /* Arabic Number */
#define GX_BIDI_CHARACTER_TYPE_CS    0x08 /* Common Number Separator */
#define GX_BIDI_CHARACTER_TYPE_NSM   0x09 /* Nonspacing Mark */
#define GX_BIDI_CHARACTER_TYPE_BN    0x0A /* Boundary Neutral */
#define GX_BIDI_CHARACTER_TYPE_B     0x0B /* Paragraph Separator */
#define GX_BIDI_CHARACTER_TYPE_S     0x0C /* Segment Separator */
#define GX_BIDI_CHARACTER_TYPE_WS    0x0D /* Whitespace */
#define GX_BIDI_CHARACTER_TYPE_ON    0x0E /* Other Neutrals */
#define GX_BIDI_CHARACTER_TYPE_LRE   0x0F
#define GX_BIDI_CHARACTER_TYPE_LRO   0x10
#define GX_BIDI_CHARACTER_TYPE_RLE   0x11
#define GX_BIDI_CHARACTER_TYPE_RLO   0x12
#define GX_BIDI_CHARACTER_TYPE_PDF   0x13
#define GX_BIDI_CHARACTER_TYPE_LRI   0x14
#define GX_BIDI_CHARACTER_TYPE_RLI   0x15
#define GX_BIDI_CHARACTER_TYPE_FSI   0x16
#define GX_BIDI_CHARACTER_TYPE_PDI   0x17

#define GX_BIDI_CATEGORY_STRONG      0x01
#define GX_BIDI_CATEGORY_WEAK        0x02
#define GX_BIDI_CATEGORY_NEUTRAL     0x03

#define GX_BIDI_DIRECTION_FORMAT_LRE 0x202A /* Left to Right Embedding */
#define GX_BIDI_DIRECTION_FORMAT_LRO 0x202D /* Left to Right Override */
#define GX_BIDI_DIRECTION_FORMAT_RLE 0x202B /* Right to Left Embedding */
#define GX_BIDI_DIRECTION_FORMAT_RLO 0x202E /* Right to Left Override */
#define GX_BIDI_DIRECTION_FORMAT_PDF 0x202C /* Pop Directional Format */
#define GX_BIDI_DIRECTION_FORMAT_LRI 0x2066 /* Left to Right Isolate */
#define GX_BIDI_DIRECTION_FORMAT_RLI 0x2067 /* Right to Left Isolate */
#define GX_BIDI_DIRECTION_FORMAT_FSI 0x2068 /* First Strong Isolate */
#define GX_BIDI_DIRECTION_FORMAT_PDI 0x2069 /* Pop Directional Isolate */


#define GX_BIDI_OVERRIDE_STATUS_N    0x01 /* No override is currently active */
#define GX_BIDI_OVERRIDE_STATUS_R    0x02 /* Characters are to be reset to R */
#define GX_BIDI_OVERRIDE_STATUS_L    0x03 /* Characters are to be reset to L */

#define GX_BIDI_MAX_EXPLICIT_DEPTH   125
#define GX_BIDI_MAX_BRACKET_DEPTH    63

/* Define explicit entry structure. */
typedef struct GX_BIDI_EXPLICIT_ENTRY_STRUCT
{
    GX_UBYTE gx_bidi_explicit_level;
    GX_BOOL  gx_bidi_explicit_override_status;
    GX_BOOL  gx_bidi_explicit_isolate_status;
} GX_BIDI_EXPLICIT_ENTRY;

/* Define level run information structure. */
typedef struct GX_BIDI_LEVEL_RUN_STRUCT
{
    INT                              gx_bidi_level_run_start_index;
    INT                              gx_bidi_level_run_end_index;
    GX_UBYTE                         gx_bidi_level_run_level;
    struct GX_BIDI_LEVEL_RUN_STRUCT *gx_bidi_level_run_next;
} GX_BIDI_LEVEL_RUN;

/* Define isolate run sequence information structure. */
typedef struct GX_BIDI_ISOLATE_RUN_STRUCT
{
    INT                               *gx_bidi_isolate_run_index_list;
    INT                                gx_bidi_isolate_run_index_count;
    GX_UBYTE                           gx_bidi_isolate_run_sos;
    GX_UBYTE                           gx_bidi_isolate_run_eos;
    struct GX_BIDI_ISOLATE_RUN_STRUCT *gx_bidi_isolate_run_next;
} GX_BIDI_ISOLATE_RUN;

/* Define unicode information structure. */
typedef struct GX_BIDI_UNIT_STRUCT
{
    ULONG    gx_bidi_unit_code;
    GX_UBYTE gx_bidi_unit_level;
    GX_UBYTE gx_bidi_unit_type;
    GX_UBYTE gx_bidi_unit_org_type;
} GX_BIDI_UNIT;

/* Define a truture to keep parameters for a bunch of functions. */
typedef struct GX_BIDI_CONTEXT_STRUCT
{
    GX_BIDI_TEXT_INFO   *gx_bidi_context_input_info;
    UINT                 gx_bidi_context_processced_size;
    UINT                 gx_bidi_context_total_lines;
    GX_BIDI_UNIT        *gx_bidi_context_unit_list;
    INT                  gx_bidi_context_unit_count;
    INT                 *gx_bidi_context_line_index_cache;
    GX_BIDI_LEVEL_RUN   *gx_bidi_context_level_runs;
    GX_BIDI_ISOLATE_RUN *gx_bidi_context_isolate_runs;
    GX_UBYTE            *gx_bidi_context_buffer;
    UINT                 gx_bidi_context_buffer_size;
    UINT                 gx_bidi_context_buffer_index;
    UINT                 gx_bidi_context_bracket_pair_size;
    GX_UBYTE             gx_bidi_context_base_level;
    ULONG                gx_bidi_context_reordered_utf8_size;
} GX_BIDI_CONTEXT;

/* Define bidirectional character infomation structure. */
typedef struct GX_BIDI_CHARACTER_INFO_STRUCT
{
    ULONG    gx_bidi_character_info_range_start;
    ULONG    gx_bidi_character_info_range_end;
    GX_UBYTE gx_bidi_character_info_type;
} GX_BIDI_CHARACTER_INFO;

typedef struct GX_BIDI_BRACKET_PAIR_STRUCT
{
    ULONG gx_bidi_bracket_pair_open;
    ULONG gx_bidi_bracket_pair_close;
} GX_BIDI_BRACKET_PAIR;

#define GX_UTILITY_TEXT_DIRECTION_GET(direction, widget, canvas, display)                                                      \
            direction = GX_LANGUAGE_DIRECTION_LTR;                                                                             \
                                                                                                                               \
            if (_gx_widget_canvas_get((GX_WIDGET *)widget, &canvas) == GX_SUCCESS)                                             \
            {                                                                                                                  \
                display = canvas -> gx_canvas_display;                                                                         \
                                                                                                                               \
                if (display -> gx_display_language_direction_table)                                                            \
                {                                                                                                              \
                    direction = (GX_BYTE)display -> gx_display_language_direction_table[display -> gx_display_active_language];\
                }                                                                                                              \
            }
#endif

#define GX_UTILITY_MATH_UINT_ADD(augend, addend, result) \
    if ((UINT)-1 - (augend) < (addend))                  \
    {                                                    \
        return GX_MATH_OVERFLOW;                         \
    }                                                    \
    else                                                 \
    {                                                    \
        (result) = (augend) + (addend);                  \
    }

#define GX_UTILITY_MATH_UINT_MULT(multiplicand, multiplier, result) \
    if ((UINT)-1 / (multiplier) < (multiplicand))                   \
    {                                                               \
        return GX_MATH_OVERFLOW;                                    \
    }                                                               \
    else                                                            \
    {                                                               \
        (result) = (multiplicand) * (multiplier);                   \
    }

#define GX_UTILITY_MATH_USHORT_ADD(augend, addend, result) \
    if ((USHORT)-1 - (augend) < (addend))                  \
    {                                                      \
        return GX_MATH_OVERFLOW;                           \
    }                                                      \
    else                                                   \
    {                                                      \
        (result) = (USHORT)((augend) + (addend));          \
    }

#define GX_UTILITY_MATH_USHORT_MULT(multiplicand, multiplier, result) \
    if ((USHORT)-1 / (multiplier) < (multiplicand))                   \
    {                                                                 \
        return GX_MATH_OVERFLOW;                                      \
    }                                                                 \
    else                                                              \
    {                                                                 \
        result = (USHORT)((multiplicand) * (multiplier));             \
    }

#define GX_UTILITY_USHORT_CHECK(a)\
    if((a) > (USHORT)-1)          \
    {                             \
        return GX_MATH_OVERFLOW;  \
    }

/* Define utility component function prototypes.  */
UINT _gx_utility_1bpp_pixelmap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height);
UINT _gx_utility_1bpp_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT _gx_utility_1bpp_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT _gx_utility_4bpp_pixelmap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height);
UINT _gx_utility_4bpp_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT _gx_utility_4bpp_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT _gx_utility_8bpp_pixelmap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height);
UINT _gx_utility_8bit_alphamap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height);
UINT _gx_utility_8bpp_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT _gx_utility_8bpp_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT _gx_utility_8bit_alphamap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT _gx_utility_16bpp_pixelmap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height);
UINT _gx_utility_32argb_pixelmap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height);
UINT _gx_utility_32argb_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT _gx_utility_32argb_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT _gx_utility_332rgb_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT _gx_utility_332rgb_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT _gx_utility_565rgb_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT _gx_utility_565rgb_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT _gx_utility_1555xrgb_pixelmap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height);
UINT _gx_utility_1555xrgb_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT _gx_utility_4444argb_pixelmap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height);
UINT _gx_utility_4444argb_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT _gx_utility_4444argb_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT _gx_utility_alphamap_create(INT width, INT height, GX_PIXELMAP *map);
UINT _gx_utility_circle_point_get(INT xcenter, INT ycenter, UINT r, INT angle, GX_POINT *point);
UINT _gx_utility_easing_function_calculate(USHORT easing_function_style, INT start_val, INT end_val, INT t, INT d, INT *return_value);

VOID _gx_utility_glyph_1bpp_to_alphamap_draw(GX_PIXELMAP *map, INT xpos, INT ypos, GX_CONST GX_GLYPH *glyph);
VOID _gx_utility_glyph_4bpp_to_alphamap_draw(GX_PIXELMAP *map, INT xpos, INT ypos, GX_CONST GX_GLYPH *glyph);
VOID _gx_utility_glyph_8bpp_to_alphamap_draw(GX_PIXELMAP *map, INT xpos, INT ypos, GX_CONST GX_GLYPH *glyph);

#if defined(GX_RENESAS_DAVE2D_FONT_SUPPORT)
VOID _gx_utility_glyph_reversed_4bpp_to_alphamap_draw(GX_PIXELMAP *map, INT xpos, INT ypos, GX_CONST GX_GLYPH *glyph);
VOID _gx_utility_glyph_reversed_1bpp_to_alphamap_draw(GX_PIXELMAP *map, INT xpos, INT ypos, GX_CONST GX_GLYPH *glyph);
#endif

UINT _gx_utility_gradient_create(GX_GRADIENT *gradient, GX_VALUE width, GX_VALUE height, UCHAR type, GX_UBYTE start_alpha, GX_UBYTE end_alpha);
UINT _gx_utility_gradient_delete(GX_GRADIENT *gradient);
UINT _gx_utility_canvas_to_bmp(GX_CANVAS *canvas, GX_RECTANGLE *rect, UINT (*write_data)(GX_UBYTE *byte_data, UINT data_count));
UINT _gx_utility_ltoa(LONG value, GX_CHAR *return_buffer, UINT return_buffer_size);

#if defined(GUIX_5_4_0_COMPATIBILITY)
INT _gx_utility_math_acos_5_4_0(INT x);
INT _gx_utility_math_asin_5_4_0(INT x);
INT _gx_utility_math_cos_5_4_0(INT angle);
INT _gx_utility_math_sin_5_4_0(INT angle);
#endif

INT          _gx_utility_math_acos(GX_FIXED_VAL x);
INT          _gx_utility_math_asin(GX_FIXED_VAL x);
GX_FIXED_VAL _gx_utility_math_cos(GX_FIXED_VAL angle);
GX_FIXED_VAL _gx_utility_math_sin(GX_FIXED_VAL angle);
UINT         _gx_utility_math_sqrt(UINT n);

UINT         _gx_utility_pixelmap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height);
UINT         _gx_utility_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT         _gx_utility_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT         _gx_utility_rectangle_center(GX_RECTANGLE *rectangle, GX_RECTANGLE *within);
UINT         _gx_utility_rectangle_center_find(GX_RECTANGLE *rectangle, GX_POINT *return_center);
UINT         _gx_utility_rectangle_combine(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle);
GX_BOOL      _gx_utility_rectangle_compare(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle);
UINT         _gx_utility_rectangle_define(GX_RECTANGLE *rectangle, GX_VALUE left, GX_VALUE top, GX_VALUE right, GX_VALUE bottom);
GX_BOOL      _gx_utility_rectangle_inside_detect(GX_RECTANGLE *outer, GX_RECTANGLE *inner);
GX_BOOL      _gx_utility_rectangle_overlap_detect(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle, GX_RECTANGLE *return_overlap_area);
GX_BOOL      _gx_utility_rectangle_point_detect(GX_RECTANGLE *rectangle, GX_POINT point);
UINT         _gx_utility_rectangle_resize(GX_RECTANGLE *rectangle, GX_VALUE adjust);
UINT         _gx_utility_rectangle_shift(GX_RECTANGLE *rectangle, GX_VALUE x_shift, GX_VALUE y_shift);

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
#if defined(GX_DYNAMIC_ARABIC_SHAPING_SUPPORT)
UINT _gx_utility_bidi_arabic_shaping(GX_BIDI_CONTEXT *context);
#endif
UINT _gx_utility_bidi_bracket_pair_get(ULONG code, GX_BIDI_BRACKET_PAIR *bracket_pair);
UINT _gx_utility_bidi_character_type_get(ULONG code, GX_UBYTE *type);
UINT _gx_utility_bidi_mirroring_get(USHORT code, USHORT *mirror);
UINT _gx_utility_bidi_paragraph_reorder(GX_BIDI_TEXT_INFO *input_info, GX_BIDI_RESOLVED_TEXT_INFO **resolved_info_head);
UINT _gx_utility_bidi_paragraph_reorder_ext(GX_BIDI_TEXT_INFO *input_info, GX_BIDI_RESOLVED_TEXT_INFO **resolved_info_head);
UINT _gx_utility_bidi_resolved_text_info_delete(GX_BIDI_RESOLVED_TEXT_INFO **resolved_info_head);
#endif

#ifdef GX_THAI_GLYPH_SHAPING_SUPPORT
UINT    _gx_utility_thai_glyph_shaping(GX_CONST GX_STRING *string, GX_CHAR_CODE **returned_code_list, UINT *returned_code_count);
#endif

UINT    _gx_utility_string_length_check(GX_CONST GX_CHAR *string, UINT *length, UINT max_string_length);
GX_BOOL _gx_utility_string_compare(GX_CONST GX_STRING *string_1, GX_CONST GX_STRING *string_2, UINT count);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT    _gx_utility_string_to_alphamap(GX_CONST GX_CHAR *text, GX_CONST GX_FONT *font, GX_PIXELMAP *textmap);
#endif
UINT    _gx_utility_string_to_alphamap_ext(GX_CONST GX_STRING *text, GX_CONST GX_FONT *font, GX_PIXELMAP *textmap);
VOID    _gx_utility_string_to_alphamap_draw(GX_CONST GX_STRING *text, GX_CONST GX_FONT *font, GX_PIXELMAP *map);

#ifdef GX_UTF8_SUPPORT
UINT    _gx_utility_unicode_to_utf8(ULONG unicode, GX_UBYTE *return_utf8_str, UINT *return_utf8_size);
UINT    _gx_utility_utf8_string_character_get(GX_STRING *utf8_str, GX_CHAR_CODE *glyph_value, UINT *glyph_len);
UINT    _gx_utility_utf8_string_backward_character_length_get(GX_STRING *utf8_str, INT start_index, UINT *glyph_len);
#endif /* GX_UTF8_SUPPORT */

/* Define error checking shells for API services.  These are only referenced by the
   application.  */
UINT    _gxe_utility_canvas_to_bmp(GX_CANVAS *canvas, GX_RECTANGLE *rect, UINT (*write_data)(GX_UBYTE *byte_data, UINT data_count));
UINT    _gxe_utility_circle_point_get(INT xcenter, INT ycenter, UINT r, INT angle, GX_POINT *point);
UINT    _gxe_utility_gradient_create(GX_GRADIENT *gradient, GX_VALUE width, GX_VALUE height, UCHAR type, GX_UBYTE start_alpha, GX_UBYTE end_alpha);
UINT    _gxe_utility_gradient_delete(GX_GRADIENT *gradient);
UINT    _gxe_utility_ltoa(LONG value, GX_CHAR *return_buffer, UINT return_buffer_size);
UINT    _gxe_utility_pixelmap_resize(GX_PIXELMAP *src, GX_PIXELMAP *destination, INT width, INT height);
UINT    _gxe_utility_pixelmap_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT    _gxe_utility_pixelmap_simple_rotate(GX_PIXELMAP *src, INT angle, GX_PIXELMAP *destination, INT *rot_cx, INT *rot_cy);
UINT    _gxe_utility_rectangle_center(GX_RECTANGLE *rectangle, GX_RECTANGLE *within);
UINT    _gxe_utility_rectangle_center_find(GX_RECTANGLE *rectangle, GX_POINT *return_center);
UINT    _gxe_utility_rectangle_combine(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle);
GX_BOOL _gxe_utility_rectangle_compare(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle);
UINT    _gxe_utility_rectangle_define(GX_RECTANGLE *rectangle, GX_VALUE left, GX_VALUE top, GX_VALUE right, GX_VALUE bottom);
UINT    _gxe_utility_rectangle_resize(GX_RECTANGLE *rectangle, GX_VALUE adjust);
GX_BOOL _gxe_utility_rectangle_overlap_detect(GX_RECTANGLE *first_rectangle, GX_RECTANGLE *second_rectangle, GX_RECTANGLE *return_overlap_area);
GX_BOOL _gxe_utility_rectangle_point_detect(GX_RECTANGLE *rectangle, GX_POINT point);
UINT    _gxe_utility_rectangle_shift(GX_RECTANGLE *rectangle, GX_VALUE x_shift, GX_VALUE y_shift);
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT    _gxe_utility_string_to_alphamap(GX_CONST GX_CHAR *text, GX_CONST GX_FONT *font, GX_PIXELMAP *textmap);
#endif
UINT    _gxe_utility_string_to_alphamap_ext(GX_CONST GX_STRING *text, GX_CONST GX_FONT *font, GX_PIXELMAP *textmap);
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
UINT    _gxe_utility_bidi_paragraph_reorder(GX_BIDI_TEXT_INFO *input_info, GX_BIDI_RESOLVED_TEXT_INFO **resolved_info_head);
UINT    _gxe_utility_bidi_paragraph_reorder_ext(GX_BIDI_TEXT_INFO *input_info, GX_BIDI_RESOLVED_TEXT_INFO **resolved_info_head);
UINT    _gxe_utility_bidi_resolved_text_info_delete(GX_BIDI_RESOLVED_TEXT_INFO **resolved_info_head);
#endif
#endif

