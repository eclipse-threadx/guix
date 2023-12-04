#ifndef _GX_STUDIO_FONT_UTIL_H_
#define _GX_STUDIO_FONT_UTIL_H_


#define GXS_FONT_UTIL_SUCCESS                0
#define GXS_FONT_UTIL_INVALID_FILENAME_PTR   1
#define GXS_FONT_UTIL_FT_INIT_FREETYPE_ERROR 2 
#define GXS_FONT_UTIL_CANNOT_OPEN_FONT_FILE  3
#define GXS_FONT_UTIL_SET_CHAR_SIZE_ERROR    4
#define GXS_FONT_UTIL_MALLOC_FAILURE         5
#define GXS_FONT_UTIL_LIBRARY_UNINITIAZLIED  6

#define GXS_FONT_UTIL

typedef struct
{
    short          gx_glyph_ascent;
    short          gx_glyph_descent;
    unsigned char  gx_glyph_advance;
    char           gx_glyph_leading;
    int            width;
    int            height;
    int            pitch;
    unsigned char *gx_glyph_data; /* pointer to Glyph bitmap data.  
                                     The size of the bitmap is width * height */
} GXS_FONT_DATA;

typedef struct
{
    int           initialized;
    FT_Library    library;
    FT_Face       face;
    int           size;

} GXS_FONT_LIBRARY_HANDLE;


int gxs_font_util_open(char *font_file, int pt, int aa, GXS_FONT_LIBRARY_HANDLE *handle);
int gxs_font_util_get_glyph(GXS_FONT_LIBRARY_HANDLE handle, int glyph_index, int pt, int aa, GXS_FONT_DATA *font_data);
int gxs_font_util_close(GXS_FONT_LIBRARY_HANDLE handle);
void DeleteFont(GX_FONT *DelMe);

#endif


