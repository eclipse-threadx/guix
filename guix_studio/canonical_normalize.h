#pragma once

typedef struct GX_CANONICAL_DECOMPOSE_MAP_STRUCT
{
    ULONG gx_canonical_decompose_map_code;
    ULONG gx_canonical_decompose_map[2];
}GX_CANONICAL_DECOMPOSE_MAP;

typedef struct GX_CANONICAL_COMBINING_CLASS_STRUCT
{
    ULONG    gx_canonical_combining_class_code;
    GX_UBYTE gx_canonical_combining_class;
}GX_CANONICAL_COMBINING_CLASS;

typedef struct GX_UNICODE_RANGE_STRUCT
{
    ULONG gx_unicode_range_start;
    ULONG gx_unicode_range_end;
}GX_UNICODE_RANGE;

typedef struct GX_CANONICAL_UNIT_STRUCT
{
    ULONG    gx_canonical_unit_code_point;
    GX_UBYTE gx_canonical_unit_combining_class;
} GX_CANONICAL_UNIT;

typedef struct GX_CANONICAL_CONTEXT_STRUCT
{
    ULONG             *gx_canonical_context_code_list;
    UINT               gx_canonical_context_code_count;
    GX_CANONICAL_UNIT *gx_canonical_context_unit_list;
    UINT               gx_canonical_context_unit_count;
} GX_CANONICAL_CONTEXT;

#define GX_NORMALIZATION_FORM_NO    0 /* The code point cannot occur in the normalization form. */
#define GX_NORMALIZATION_FORM_YES   1 /* The code point is a starter and can occur in the normalization form. */
#define GX_NORMALIZATION_FORM_MAYBE 2

UINT gx_studio_canonical_normalize(GX_CONST GX_STRING *string, GX_STRING *normalised_string);
