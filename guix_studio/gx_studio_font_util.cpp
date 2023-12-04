
#include "studiox_includes.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include "gx_studio_font_util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef struct {
    int ascent;
    int descent;
    int max_height;
} FONT_METRICS;

typedef struct {
    int glyph;
    int value;
} KERNING_INFO;

///////////////////////////////////////////////////////////////////////////////
int gx_studio_font_util_open(const char *font_file, GXS_FONT_LIBRARY_HANDLE **handle)
{
    FT_Error      error;
    GXS_FONT_LIBRARY_HANDLE *font_handle;

    if(font_file == NULL)
        return(GXS_FONT_UTIL_INVALID_FILENAME_PTR);

    font_handle = (GXS_FONT_LIBRARY_HANDLE*)malloc(sizeof(GXS_FONT_LIBRARY_HANDLE));
    if(font_handle == 0)
        return(GXS_FONT_UTIL_MALLOC_FAILURE);

    memset(font_handle, 0, sizeof(GXS_FONT_LIBRARY_HANDLE));

    error = FT_Init_FreeType(&(font_handle -> library));              /* initialize library */
    if(error)
    {
        free(font_handle);
        return(GXS_FONT_UTIL_FT_INIT_FREETYPE_ERROR);
    }
    
    error = FT_New_Face(font_handle -> library, font_file, 0, &(font_handle -> face) );/* create face object */
    if(error)
    {
        FT_Done_FreeType( font_handle -> library );
        free(font_handle);
        return(GXS_FONT_UTIL_CANNOT_OPEN_FONT_FILE);
    }    
    
    font_handle -> size = 0;
    font_handle -> initialized = 1;

    *handle = font_handle;

    return(GXS_FONT_UTIL_SUCCESS);
}

///////////////////////////////////////////////////////////////////////////////
int gx_studio_font_util_get_glyph(GXS_FONT_LIBRARY_HANDLE *font_handle, int font_index, int pt, int aa, GXS_FONT_DATA *font_data)
{
    FT_Glyph_Metrics *metrics;
    FT_GlyphSlot  slot;
    FT_Error      error;
    int           glyph_index;    

    if(font_handle -> initialized != 1)
        return(GXS_FONT_UTIL_LIBRARY_UNINITIAZLIED);
    
    /* set character size */
    if(font_handle -> size != pt)
    {
        error = FT_Set_Char_Size(font_handle -> face, pt * 64, 0, 72, 0);
        if(error)
        {
            return(GXS_FONT_UTIL_SET_CHAR_SIZE_ERROR);
        }
        font_handle -> size = pt;
    }
    
    slot = font_handle -> face -> glyph;
  
    glyph_index = FT_Get_Char_Index(font_handle -> face, font_index);
    error = FT_Load_Glyph(font_handle -> face, glyph_index, 0);
        
    if(font_handle -> face -> glyph -> format != FT_GLYPH_FORMAT_BITMAP)
    {
        if(aa)
            error = FT_Render_Glyph(font_handle -> face -> glyph, FT_RENDER_MODE_NORMAL);
        else
            error = FT_Render_Glyph(font_handle -> face -> glyph, FT_RENDER_MODE_MONO);
    }

    if (aa && slot->bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
    {
        error = FT_Load_Glyph(font_handle->face, glyph_index, FT_LOAD_NO_BITMAP | FT_LOAD_RENDER);
    }
    
    metrics = &(font_handle -> face -> glyph -> metrics);
    font_data -> width = slot -> bitmap.width;
    font_data -> height = slot -> bitmap.rows;
    font_data -> pitch = slot -> bitmap.pitch;

    font_data -> gx_glyph_ascent = (short) ((metrics -> horiBearingY) >> 6);
    font_data -> gx_glyph_descent = (short)((metrics->height - metrics->horiBearingY) >> 6);
    font_data -> gx_glyph_advance = (unsigned char) ((metrics -> horiAdvance) >> 6);
    font_data -> gx_glyph_leading = (char) ((metrics -> horiBearingX) >> 6);
    font_data -> gx_glyph_data = slot -> bitmap.buffer;
    return(GXS_FONT_UTIL_SUCCESS);
}

///////////////////////////////////////////////////////////////////////////////
int gx_studio_font_util_get_kerning_info(GXS_FONT_LIBRARY_HANDLE *font_handle, int font_index,
                                         int first_glyph_index, int last_glyph_index, GX_KERNING_GLYPH *kerning_glyph, FontCharMap *map)
{
    FT_Error      error;
    FT_Vector     kerning_vector;
    GX_UBYTE      left_glyph_index;
    GX_UBYTE      right_glyph_index;
    INT           index;
    CArray<KERNING_INFO> kerning_pairs;
    KERNING_INFO kerning_info;
    INT           table_size;
    GX_UBYTE     *left_glyph_ptr;
    INT           first_glyph = first_glyph_index;
    INT           last_glyph = last_glyph_index;

    if (font_handle->initialized != 1)
        return(GXS_FONT_UTIL_LIBRARY_UNINITIAZLIED);

    if (FT_HAS_KERNING(font_handle->face) == 0)
    {
        // The face object does not contain kerning data.
        return(GXS_FONT_UTIL_SUCCESS);
    }

    if (!map)
    {
        return(GXS_FONT_UTIL_SUCCESS);
    }

    kerning_pairs.SetSize(0, 1);
    if (first_glyph > 0xff)
    {
        return 0;
    }
    if (last_glyph > 0xff)
    {
        last_glyph = 0xff;
    }

    right_glyph_index = FT_Get_Char_Index(font_handle->face, font_index);

    if (!right_glyph_index)
    {
        // Undefined character code.
        return(GXS_FONT_UTIL_SUCCESS);
    }

    // Get kerning vector between two glyphs in the same face
    for (index = first_glyph; index <= last_glyph; index++)
    {
        if (!map->Test(index))
        {
            continue;
        }

        left_glyph_index = FT_Get_Char_Index(font_handle->face, index);

        if (!left_glyph_index)
        {
            continue;
        }

        memset(&kerning_vector, 0, sizeof(FT_Vector));
        error = FT_Get_Kerning(font_handle->face, left_glyph_index, right_glyph_index, FT_KERNING_DEFAULT, &kerning_vector);
        if ((error == 0) && (kerning_vector.x))
        {
            kerning_info.glyph = index;
            kerning_info.value = (kerning_vector.x >> 6);
            kerning_pairs.Add(kerning_info);
        }
    }

    if (kerning_pairs.GetCount())
    {
        // Calculate size of needed buffer. 1 byte size + 2 bytes for each kerning pair.
        table_size = 1 + kerning_pairs.GetCount() * (sizeof(GX_UBYTE) + sizeof(GX_CHAR));

        // Allocate memory to load kerning table.
        kerning_glyph->gx_kerning_table = new GX_UBYTE[table_size];
        memset(const_cast<GX_UBYTE *>(kerning_glyph->gx_kerning_table), 0, table_size);
        left_glyph_ptr = (GX_UBYTE *)(kerning_glyph->gx_kerning_table + 1);

        if (kerning_glyph->gx_kerning_table)
        {
            *(const_cast<GX_UBYTE *>(kerning_glyph->gx_kerning_table)) = kerning_pairs.GetCount();

            for (index = 0; index < kerning_pairs.GetCount(); index++)
            {
                kerning_info = kerning_pairs.GetAt(index);
                *left_glyph_ptr++ = (GX_UBYTE)kerning_info.glyph;
                *left_glyph_ptr++ = (GX_UBYTE)kerning_info.value;
            }

        }
    }
    
    return(GXS_FONT_UTIL_SUCCESS);
}

///////////////////////////////////////////////////////////////////////////////
void gx_studio_font_util_close(GXS_FONT_LIBRARY_HANDLE *font_handle)
{
    if(!font_handle)
        return;

    if(font_handle -> initialized == 1)
    {

        if(font_handle -> face)
            FT_Done_Face(font_handle -> face);
        if(font_handle -> library)
            FT_Done_FreeType(font_handle -> library);
    }

    free(font_handle);
    return;
}

///////////////////////////////////////////////////////////////////////////////
INT GetRowPitch(INT width, INT bits_per_pix)
{
    // Calcualte data size of glyph map
    INT pitch = width;

    switch (bits_per_pix)
    {
    case 1:
        pitch += 7;
        pitch /= 8;
        break;
    case 4:
        pitch ++;
        pitch /= 2;
        break;
    case 8:
        break;
    }

    return pitch;
}

///////////////////////////////////////////////////////////////////////////////
INT  GetFontBits(INT font_format)
{
    int font_bits = 8;

    switch (font_format & GX_FONT_FORMAT_BPP_MASK)
    {
    case GX_FONT_FORMAT_1BPP:
        font_bits = 1;
        break;

    case GX_FONT_FORMAT_2BPP:
        font_bits = 2;
        break;

    case GX_FONT_FORMAT_4BPP:
        font_bits = 4;
        break;

    case GX_FONT_FORMAT_8BPP:
        font_bits = 8;
        break;
    }

    return font_bits;
}

///////////////////////////////////////////////////////////////////////////////
BOOL IsFontBitsSupported(INT font_bits)
{
    switch (font_bits)
    {
    case 1:
    case 4:
    case 8:
        return TRUE;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
int DuplicatesCount(GX_UBYTE *data, int width, int index)
{
    int count = 1;
    GX_UBYTE pre;

    pre = data[index++];

    while (index < width)
    {
        if (pre == data[index])
        {
            pre = data[index];
            count++;
        }
        else
        {
            break;
        }

        index++;
    }

    return count;
}

///////////////////////////////////////////////////////////////////////////////
int RleEncode(GX_UBYTE *put_data, GX_UBYTE *get_data, int width, int height)
{
    int count = 0;
    int raw_count = 0;
    int put_index = 0;
    int row;
    int compressed_size = 0;

    for (row = 0; row < height; row++)
    {
        int index = 0;

        while (index < width)
        {

            count = DuplicatesCount(get_data, width, index);

            if (count >= 3)
            {
                if (raw_count)
                {
                    compressed_size += raw_count + 1;

                    if (put_data)
                    {
                        *put_data++ = (raw_count - 1) & 0x7f;

                        while (raw_count)
                        {
                            *put_data++ = get_data[index - raw_count];
                            raw_count--;
                        }
                    }
                    else
                    {
                        raw_count = 0;
                    }
                }

                if (count > 128)
                {
                    count = 128;
                }

                if (put_data)
                {
                    *put_data++ = (count - 1) | 0x80;
                    *put_data++ = get_data[index];
                }
                compressed_size += 2;
                index += count;
            }
            else
            {
                raw_count++;
                index++;

                if ((raw_count == 128) || (index == width))
                {
                    compressed_size += raw_count + 1;

                    if (put_data)
                    {
                        *put_data++ = (raw_count - 1) & 0x7f;

                        while (raw_count)
                        {
                            *put_data++ = get_data[index - raw_count];
                            raw_count--;
                        }
                    }
                    else
                    {
                        raw_count = 0;
                    }
                }
            }
        }

        get_data += width;
    }

    return compressed_size;
}

///////////////////////////////////////////////////////////////////////////////
void RleEncodeGlyphData(GX_COMPRESSED_GLYPH *glyph, int bits_per_pix)
{
    GX_UBYTE *pGet = (GX_UBYTE *)glyph->gx_glyph_map;

    int pitch = GetRowPitch(glyph->gx_glyph_width, bits_per_pix);
    int data_size = pitch * glyph->gx_glyph_height;

    int compressed_size = RleEncode(NULL, pGet, pitch, glyph->gx_glyph_height);

    if (compressed_size < data_size)
    {
        glyph->gx_glyph_map_size = compressed_size | 0x8000;
        UCHAR *pPut = new UCHAR[compressed_size];
        pGet = (GX_UBYTE *)glyph->gx_glyph_map;
        RleEncode((GX_UBYTE *)pPut, pGet, pitch, glyph->gx_glyph_height);

        delete glyph->gx_glyph_map;
        glyph->gx_glyph_map = pPut;
    }
    else
    {
        glyph->gx_glyph_map_size = data_size;
    }
}

///////////////////////////////////////////////////////////////////////////////
GX_UBYTE* RleDecodeGlyphData(GX_COMPRESSED_GLYPH* glyph, int bits_per_pix)
{
    int pitch = GetRowPitch(glyph->gx_glyph_width, bits_per_pix);
    int data_size = pitch * glyph->gx_glyph_height;

    if (!data_size)
    {
        return GX_NULL;
    }

    GX_UBYTE* put_data = new GX_UBYTE[data_size];
    GX_UBYTE* put_row = put_data;
    GX_UBYTE* put;
    GX_CONST GX_UBYTE* glyph_data = glyph->gx_glyph_map;

    int row, col;
    GX_UBYTE count;
    GX_UBYTE alpha;

    for (row = 0; row < glyph->gx_glyph_height; row++)
    {
        col = 0;
        put = put_row;

        while (col < pitch)
        {
            count = *glyph_data++;

            if (count & 0x80)
            {
                count = (count & 0x7f) + 1;
                alpha = *glyph_data++;

                while (count--)
                {
                    *put = alpha;

                    put++;
                    col++;
                }
            }
            else
            {
                count++;

                while (count--)
                {
                    alpha = *glyph_data++;
                    *put = alpha;
                    put++;
                    col++;
                }
            }
        }
        put_row += pitch;
    }

    return put_data;
}

///////////////////////////////////////////////////////////////////////////////
void CopyGlyphData(GX_GLYPH *put_glyph, GXS_FONT_DATA &glyph, int bits_per_pix, BOOL reversed_order)
{
    int bitmap_size;
    int row;
    int col;
    int num_bits;
    UCHAR *read_data;
    UCHAR *write_data;
    UCHAR val;
    UCHAR in_byte;
    UCHAR in_mask;
    UCHAR out_mask;

    put_glyph->gx_glyph_advance = glyph.gx_glyph_advance;
    put_glyph->gx_glyph_ascent = glyph.gx_glyph_ascent;
    put_glyph->gx_glyph_descent = glyph.gx_glyph_descent;
    put_glyph->gx_glyph_height = ToUByte(glyph.height);
    put_glyph->gx_glyph_width = ToUByte(glyph.width);
    put_glyph->gx_glyph_leading = glyph.gx_glyph_leading;

    bitmap_size = GetRowPitch(glyph.width, bits_per_pix) * glyph.height;

    if(bitmap_size == 0)
    {
        return;
    }

    put_glyph->gx_glyph_map = new UCHAR[bitmap_size];

    switch(bits_per_pix)
    {
    case 1:
        write_data = (UCHAR *) put_glyph->gx_glyph_map;

        for (row = 0; row < glyph.height; row++)
        {
            num_bits = glyph.width;
            read_data = glyph.gx_glyph_data + (row * glyph.pitch);
                
            while(num_bits)
            {
                val = *read_data;

                if (reversed_order)
                {
                    // swap the bit order:
                    in_byte = val;
                    val = 0;
                    in_mask = 0x80;
                    out_mask = 0x01;

                    while(in_mask)
                    {
                        if (in_byte & in_mask)
                        {
                            val |= out_mask;
                        }
                        in_mask >>= 1;
                        out_mask <<= 1;
                    } 
                }
                if (num_bits > 8)
                {
                    *write_data++ = val;
                    num_bits -= 8;
                    read_data++;
                }
                else
                {
                    *write_data++ = val;
                    num_bits = 0;
                }
            }
        }
        break;

    case 4:
        // convert 8 bit incoming data to 4 bit outgoing data

        read_data = glyph.gx_glyph_data;
        write_data = (UCHAR *) put_glyph->gx_glyph_map;

        for (row = 0; row < glyph.height; row++)
        {
            val = 0;
            for (col = 0; col < glyph.width; col++)
            {
                in_byte = *read_data++;

                if (col & 1)
                {
                    if (reversed_order)
                    {
                        val |= (in_byte & 0xf0);    // keep high nibble
                    }
                    else
                    {
                        val |= (in_byte >> 4);
                    }
                    *write_data++ = val;
                }
                else
                {
                    if (reversed_order)
                    {
                        val = in_byte >> 4;
                    }
                    else
                    {
                        val = in_byte & 0xf0;
                    }
                }
            }

            // check for odd width, if so write out last pixel
            if (glyph.width & 1)
            {
                *write_data++ = val;
            }
        }
        break;

    default:
        memcpy_s((UCHAR *) put_glyph->gx_glyph_map, bitmap_size, glyph.gx_glyph_data, bitmap_size);
        break;
    }
}


///////////////////////////////////////////////////////////////////////////////
void GetFontMetrics(GXS_FONT_LIBRARY_HANDLE *handle,
    int height, FONT_METRICS &metrics, int firstchar, int lastchar, int bits_per_pix, FontCharMap &map)
{
    int use_char;

    GXS_FONT_DATA glyph;
    GX_BOOL anti_alias = GX_TRUE;

    if (bits_per_pix == 1)
    {
        anti_alias = GX_FALSE;
    }

    for (use_char = firstchar; use_char <= lastchar; use_char++ )
    {
        if (!map.Test(use_char))
        {
            continue;
        }

        gx_studio_font_util_get_glyph(handle, use_char,
            height, anti_alias, &glyph);

        if(glyph.height > metrics.max_height)
        {
            metrics.max_height = glyph.height;
        }
        if(glyph.gx_glyph_ascent > metrics.ascent)
        {
            metrics.ascent = glyph.gx_glyph_ascent;
        }
        if(glyph.gx_glyph_descent > metrics.descent)
        {
            metrics.descent = glyph.gx_glyph_descent;
        }
    }

    if (metrics.max_height < (metrics.ascent + metrics.descent))
    {
        metrics.max_height = (metrics.ascent + metrics.descent);
    }
}


///////////////////////////////////////////////////////////////////////////////
GX_FONT *MakeFontPage(GXS_FONT_LIBRARY_HANDLE *handle,
    int height, FONT_METRICS &metrics, int firstchar, int lastchar, FontCharMap &map, res_info *info, int display)
{
    int use_char;
    int char_count;

    GXS_FONT_DATA glyph;
    GX_FONT *new_font;
    GX_GLYPH *glyph_array;
    GX_GLYPH *put_glyph;
    GX_COMPRESSED_GLYPH *compressed_glyph_array;
    GX_COMPRESSED_GLYPH *put_compressed_glyph;
    GX_BOOL anti_alias = GX_TRUE;
    INT     bits_per_pix = info->font_bits;
    /* Kerning glyph */
    GX_BOOL reversed_order = GX_FALSE;
    GX_KERNING_GLYPH *kerning_glyph_array;
    GX_KERNING_GLYPH *put_kerning_glyph;

    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return NULL;
    }

    char_count = lastchar - firstchar + 1;

    new_font = new GX_FONT;
    new_font->gx_font_first_glyph = firstchar;
    new_font->gx_font_last_glyph = lastchar;
    new_font->gx_font_next_page = NULL;

    if (!IsRenesasDave2D(GetOpenProject()))
    {
        /* Compressed mode is only supported by Renesas dave2d.  */
        info->compress = FALSE;
    }

    if (info->compress)
    {
        compressed_glyph_array = new GX_COMPRESSED_GLYPH[char_count];
        memset(compressed_glyph_array, 0, char_count * sizeof(GX_COMPRESSED_GLYPH));
        new_font->gx_font_glyphs.gx_font_compressed_glyphs = compressed_glyph_array;
    }
    else
    {
        if (info->font_kerning)
        {
            if (FT_HAS_KERNING(handle->face))
            {
                /* allocate memory for kerning glyph */
                kerning_glyph_array = new GX_KERNING_GLYPH[char_count];
                memset(kerning_glyph_array, 0, char_count * sizeof(GX_KERNING_GLYPH));
                new_font->gx_font_glyphs.gx_font_kerning_glyphs = kerning_glyph_array;

            }
            else
            {
                /* Set kerning flag to false as the font does not contain kerning information.  */
                info->font_kerning = FALSE;
                glyph_array = new GX_GLYPH[char_count];
                memset(glyph_array, 0, char_count * sizeof(GX_GLYPH));
                new_font->gx_font_glyphs.gx_font_normal_glyphs = glyph_array;
            }
        }
        else
        {
            glyph_array = new GX_GLYPH[char_count];
            memset(glyph_array, 0, char_count * sizeof(GX_GLYPH));
            new_font->gx_font_glyphs.gx_font_normal_glyphs = glyph_array;
        }
    }

    switch(bits_per_pix)
    {
    case 1:
        new_font->gx_font_format = GX_FONT_FORMAT_1BPP;
        anti_alias = GX_FALSE;

        if (IsDave2dFontFormat(project, display))
        {
            reversed_order = TRUE;
        }
        break;

    case 4:
        new_font->gx_font_format = GX_FONT_FORMAT_4BPP;

        if (IsDave2dFontFormat(project, display))
        {
            reversed_order = TRUE;
        }
        break;

    case 8:
        new_font->gx_font_format = GX_FONT_FORMAT_8BPP;
        break;
    }

    if (reversed_order)
    {
        new_font->gx_font_format |= GX_FONT_FORMAT_REVERSED_ORDER;
    }

    if (info->compress)
    {
        new_font->gx_font_format |= GX_FONT_FORMAT_COMPRESSED;
    }
    
    if (info->font_kerning)
    {
        new_font->gx_font_format |= GX_FONT_FORMAT_KERNING;
    }

    for (use_char = firstchar; use_char <= lastchar; use_char++)
    {
        if (!map.Test(use_char))
        {
            continue;
        }

        gx_studio_font_util_get_glyph(handle, use_char,
                                      height, anti_alias, &glyph);
        if (info->compress)
        {
            put_compressed_glyph = &(compressed_glyph_array)[use_char - firstchar];
            CopyGlyphData((GX_GLYPH *)put_compressed_glyph, glyph, bits_per_pix, reversed_order);

            //Compress glyph data
            RleEncodeGlyphData(put_compressed_glyph, bits_per_pix);
        }
        else
        {
            if (info->font_kerning)
            {
                put_kerning_glyph = &kerning_glyph_array[use_char - firstchar];
                CopyGlyphData((GX_GLYPH *)put_kerning_glyph, glyph, bits_per_pix, reversed_order);
                if (use_char <= 0xff)
                {
                    gx_studio_font_util_get_kerning_info(handle, use_char, firstchar, lastchar, put_kerning_glyph, &map);
                }
            }
            else
            {
                put_glyph = &glyph_array[use_char - firstchar];
                CopyGlyphData(put_glyph, glyph, bits_per_pix, reversed_order);
            }
        }
    }

    new_font->gx_font_prespace = 0;       /* Line spacing above, pixels               */
    new_font->gx_font_postspace = 0;
    new_font->gx_font_line_height = metrics.max_height;
    new_font->gx_font_baseline = metrics.ascent;

    return new_font;
}

///////////////////////////////////////////////////////////////////////////////
GX_FONT *MakeFont(res_info *info, int display, BOOL warn_on_error)
{
    font_page_info *pages = info->font_pages;
    int page_count;
    int max_char_count;
    int max_char;
    int height = info->font_height;
    int bits_per_pix = info->font_bits;
    FontCharMap char_map;
    FONT_METRICS metrics;

    int page;
    GXS_FONT_LIBRARY_HANDLE *handle;
    GX_FONT *head_page = NULL;
    GX_FONT *last_page = NULL;

    studiox_project* project = GetOpenProject();
    
    if(!project)
    {
    	return NULL;
    }

//    GotoProjectDirectory();
    CString abspath = MakeAbsolutePathname(info->pathinfo);
    if (gx_studio_font_util_open(CT2A(abspath.GetString()), &handle) != 0)
    {
        if (warn_on_error)
        {
            CString msg;
            msg.Format(_T("Font Name = %s\nFailed to open TrueType font file: %s"), info->name, abspath);
            ErrorMsg(msg);
        }
        return NULL;
    }

    if (info->font_support_extended_unicode)
    {
        // Use 21-bit character map
        char_map.SetMapSize(262144);
        page_count = NUM_FONT_CHAR_RANGES + NUM_FONT_EXTENDED_CHAR_RANGES;

        // extended unicode range is between [0x10000, 0x10ffff],
        // the total supported character count is 0x110000
        max_char_count = GX_MAX_GLYPH_CODE + 1;
        max_char = 0x10ffff;
    }
    else
    {
        page_count = NUM_FONT_CHAR_RANGES;
        max_char_count = 0xffff;
        max_char = 0xffff;
    }

    if (info->font_pages_count)
    {
        page_count = info->font_pages_count;
    }

    for (page = 0; page < page_count; page++)
    {
        // map character ranges the user has turned on
        if (pages[page].enabled && pages[page].first_char <= pages[page].last_char)
        {
            char_map.Set(pages[page].first_char, pages[page].last_char);
        }
    }

    if (info->font_charset_include_string_table)
    {
        // add characters used in string table
        FontCharMap *tmap = GetActiveCharacterMap(FALSE);
        if (tmap)
        {
            char_map.Overlay(tmap);
        }
    }

    if (char_map.Test(0xe00, 0xe7f))
    {
        // Detected thai glyph code
        if (char_map.Test(0x0e33))
        {
            // The vowel sara am will be split into the character nikhahit and sara aa if it is
            // appended to a consonant.
            char_map.Set(0x0e4d, 0x0e4d);
            char_map.Set(0x0e32, 0x0e32);
        }

        // add private user area for thai glyph shaping
        char_map.Set(0xf700, 0xf71a);
    }

    if (char_map.Test(0x600, 0x6ff))
    {
        // add arabic presentation forms-b for arabic shaping
        char_map.Set(0xfe70, 0xfeff);

        // add arabic presentation forms-a for persion shaping
        char_map.Set(0xfb50, 0xfbff);
    }

    if (char_map.IsEmpty())
    {
        gx_studio_font_util_close(handle);
        return NULL;
    }

    int first_char, last_char;

    metrics.ascent = metrics.descent = metrics.max_height = 0;

    // first get the font metrics
    for (first_char = 0; first_char <= max_char; first_char++)
    {
        if (char_map.Test(first_char, first_char))
        {
            GetFontMetrics(handle, height, metrics, first_char, first_char, bits_per_pix, char_map);
        }
    }

    // now render actual font data pages
    GX_FONT *newpage;

    for (first_char = 0; first_char <= max_char;)
    {
        last_char = first_char + 127;

        if (last_char > max_char)
        {
            last_char = max_char;
        }

        if (char_map.Test(first_char, last_char))
        {
            // Make font for characters that defined in character map but not in character ranges.
            newpage = MakeFontPage(handle, height, metrics, first_char, last_char, char_map, info, display);

            if (newpage)
            {
                if (!head_page)
                {
                    head_page = newpage;
                }
                if (last_page)
                {
                    last_page->gx_font_next_page = newpage;
                }
                last_page = newpage;
            }
        }

        first_char = last_char + 1;
    }

    gx_studio_font_util_close(handle);
    return head_page;
}

///////////////////////////////////////////////////////////////////////////////
BOOL InsertStringTableGlyphs(FontCharMap *char_map, res_info *info, int display)
{
    BOOL status = TRUE;
    BOOL found_page;
    BOOL anti_alias = FALSE;
    int height = info->font_height;
    int bits_per_pix = info->font_bits;
    DWORD char_val;
    DWORD maxval = char_map->GetMapSize() << 3;
    GXS_FONT_LIBRARY_HANDLE *handle;
    GXS_FONT_DATA glyph;
    GX_FONT *head_page = NULL;
    GX_FONT *last_page = NULL;

    if (!char_map)
    {
        return FALSE;
    }

    CString abspath = MakeAbsolutePathname(info->pathinfo);
    if (gx_studio_font_util_open(CT2A(abspath.GetString()), &handle) != 0)
    {
        return FALSE;
    }

    if (bits_per_pix > 1)
    {
        anti_alias = TRUE;
    }

    BOOL reversed_order = FALSE;

    switch (bits_per_pix)
    {
    case 1:
    case 4:
        if (IsDave2dFontFormat(GetOpenProject(), display))
        {
            reversed_order = TRUE;
        }
        break;
    }

    for (char_val = 0; char_val < maxval; char_val++)
    {
        if (char_map->Test(char_val))
        {
            GX_FONT *page = info->font;
            found_page = FALSE;

            while(page)
            {
                if (page->gx_font_first_glyph <= char_val &&
                    page->gx_font_last_glyph >= char_val)
                {
                    found_page = TRUE;

                    if (info->font_kerning)
                    {
                        GX_KERNING_GLYPH *put_glyph = (GX_KERNING_GLYPH *) &page->gx_font_glyphs.gx_font_kerning_glyphs[char_val - page->gx_font_first_glyph];

                        if ((!put_glyph->gx_glyph_map) && (put_glyph->gx_glyph_advance == 0))
                        {
                            // get the glyph data, insert into font

                            if (gx_studio_font_util_get_glyph(handle, char_val, height, anti_alias, &glyph) != GXS_FONT_UTIL_SUCCESS)
                            {
                                status = FALSE;
                                break;
                            }
                            CopyGlyphData((GX_GLYPH *)put_glyph, glyph, bits_per_pix, reversed_order);

                            if (put_glyph->gx_kerning_table)
                            {
                                delete[] put_glyph->gx_kerning_table;
                                put_glyph->gx_kerning_table = NULL;
                                if (char_val <= 0xff)
                                {
                                    gx_studio_font_util_get_kerning_info(handle, char_val, page->gx_font_first_glyph, page->gx_font_last_glyph, put_glyph, char_map);
                                }
                            }
                        }
                    }
                    else if (page->gx_font_format & GX_FONT_FORMAT_COMPRESSED)
                    {
                        GX_COMPRESSED_GLYPH *put_glyph = (GX_COMPRESSED_GLYPH *) &page->gx_font_glyphs.gx_font_compressed_glyphs[char_val - page->gx_font_first_glyph];

                        if ((!put_glyph->gx_glyph_map) && (put_glyph->gx_glyph_advance == 0))
                        {
                            // get the glyph data, insert into font

                            if (gx_studio_font_util_get_glyph(handle, char_val, height, anti_alias, &glyph) != GXS_FONT_UTIL_SUCCESS)
                            {
                                status = FALSE;
                                break;
                            }
                            CopyGlyphData((GX_GLYPH *)put_glyph, glyph, bits_per_pix, reversed_order);

                            //Compress glyph data
                            RleEncodeGlyphData(put_glyph, bits_per_pix);
                        }
                    }
                    else
                    {
                        GX_GLYPH *put_glyph = (GX_GLYPH *)&page->gx_font_glyphs.gx_font_normal_glyphs[char_val - page->gx_font_first_glyph];

                        if ((!put_glyph->gx_glyph_map) && (put_glyph->gx_glyph_advance == 0))
                        {
                            // get the glyph data, insert into font

                            if (gx_studio_font_util_get_glyph(handle, char_val, height, anti_alias, &glyph) != GXS_FONT_UTIL_SUCCESS)
                            {
                                status = FALSE;
                                break;
                            }
                            CopyGlyphData(put_glyph, glyph, bits_per_pix, reversed_order);
                        }
                    }
                    
                    break;
                }
                page = (GX_FONT *) page->gx_font_next_page;
            }

            if ((!found_page) || (status != TRUE))
            {
                status = FALSE;
                break;
            }
        }
    }
    
    gx_studio_font_util_close(handle);
    return status;
}


///////////////////////////////////////////////////////////////////////////////
void DestroyFont(GX_FONT *font)
{
    GX_GLYPH *glyph;
    GX_COMPRESSED_GLYPH *compressed_glyph;
    GX_KERNING_GLYPH  *kering_glyph;
    const GX_FONT *next_page = NULL;

    while (font)
    {
        next_page = font->gx_font_next_page;

        int num_chars = font->gx_font_last_glyph - font->gx_font_first_glyph + 1;

        if (font->gx_font_glyphs.gx_font_normal_glyphs)
        {
            glyph = (GX_GLYPH *) font->gx_font_glyphs.gx_font_normal_glyphs;

            for (int index = 0; index < num_chars; index++)
            {
                if (glyph->gx_glyph_map)
                {
                    delete [] glyph->gx_glyph_map;
                    glyph->gx_glyph_map = NULL;
                }

                if (font->gx_font_format & GX_FONT_FORMAT_COMPRESSED)
                {
                    compressed_glyph = (GX_COMPRESSED_GLYPH *)glyph;
                    compressed_glyph++;
                    glyph = (GX_GLYPH *)compressed_glyph;
                }
                else if (font->gx_font_format & GX_FONT_FORMAT_KERNING)
                {
                    kering_glyph = (GX_KERNING_GLYPH *)glyph;
                    if (kering_glyph->gx_kerning_table)
                    {
                        delete[] kering_glyph->gx_kerning_table;
                        kering_glyph->gx_kerning_table = NULL;
                    }
                    kering_glyph++;
                    glyph = (GX_GLYPH *)kering_glyph;
                }
                else
                {
                    glyph++;
                }
            }
            delete [] font->gx_font_glyphs.gx_font_normal_glyphs;
            font->gx_font_glyphs.gx_font_normal_glyphs = NULL;
        }
        delete font;
        font = (GX_FONT *) next_page;
    } 
}

///////////////////////////////////////////////////////////////////////////////
GX_CHAR_CODE FindFirstValidGlyph(const GX_FONT *page, GX_CHAR_CODE glyph_start)
{
    GX_CHAR_CODE index;
    GX_COMPRESSED_GLYPH *compressed_glyph;
    GX_KERNING_GLYPH *kerning_glyph;
    while(1)
    {
        if (glyph_start > page->gx_font_last_glyph)
        {
            break;
        }
        index = glyph_start - page->gx_font_first_glyph;

        if (page->gx_font_format & GX_FONT_FORMAT_COMPRESSED)
        {
            compressed_glyph = (GX_COMPRESSED_GLYPH *)page->gx_font_glyphs.gx_font_compressed_glyphs;
            if (compressed_glyph[index].gx_glyph_map != NULL ||
                compressed_glyph[index].gx_glyph_advance != 0)
            {
                break;
            }
        } 
        else if (page->gx_font_format & GX_FONT_FORMAT_KERNING)
        {
            kerning_glyph = (GX_KERNING_GLYPH *)page->gx_font_glyphs.gx_font_kerning_glyphs;
            if (kerning_glyph[index].gx_glyph_map != NULL ||
                kerning_glyph[index].gx_glyph_advance != 0)
            {
                break;
            }
        }
        else
        {
            if (page->gx_font_glyphs.gx_font_normal_glyphs[index].gx_glyph_map != NULL ||
                page->gx_font_glyphs.gx_font_normal_glyphs[index].gx_glyph_advance != 0)
            {
                break;
            }
        }
        glyph_start++;
    }
    return glyph_start;
}

#define MAX_FONT_DEAD_BLOCK 64

///////////////////////////////////////////////////////////////////////////////
GX_CHAR_CODE FindDeadGlyphBlock(const GX_FONT *page, GX_CHAR_CODE glyph_end)
{
    GX_CHAR_CODE dead_count = 0;
    GX_CHAR_CODE stop_point = glyph_end;
    GX_CHAR_CODE index;
    GX_CONST GX_COMPRESSED_GLYPH *compressed_glyph;
    GX_CONST GX_KERNING_GLYPH *kerning_glyph;

    index = stop_point;

    if (page->gx_font_format & GX_FONT_FORMAT_COMPRESSED)
    {
        compressed_glyph = page->gx_font_glyphs.gx_font_compressed_glyphs;

        while (index <= page->gx_font_last_glyph)
        {
            if (compressed_glyph[index - page->gx_font_first_glyph].gx_glyph_map == NULL &&
                compressed_glyph[index - page->gx_font_first_glyph].gx_glyph_advance == 0)
            {
                dead_count++;

                if (dead_count > MAX_FONT_DEAD_BLOCK)
                {
                    break;
                }
            }
            else
            {
                stop_point = index;
                dead_count = 0;
            }
            index++;
        }
    }
    else if (page->gx_font_format & GX_FONT_FORMAT_KERNING)
    {
        kerning_glyph = page->gx_font_glyphs.gx_font_kerning_glyphs;

        while (index <= page->gx_font_last_glyph)
        {
            if (kerning_glyph[index - page->gx_font_first_glyph].gx_glyph_map == NULL &&
                kerning_glyph[index - page->gx_font_first_glyph].gx_glyph_advance == 0)
            {
                dead_count++;

                if (dead_count > MAX_FONT_DEAD_BLOCK)
                {
                    break;
                }
            }
            else
            {
                stop_point = index;
                dead_count = 0;
            }
            index++;
        }
    }
    else
    {

        while (index <= page->gx_font_last_glyph)
        {
            if (page->gx_font_glyphs.gx_font_normal_glyphs[index - page->gx_font_first_glyph].gx_glyph_map == NULL &&
                page->gx_font_glyphs.gx_font_normal_glyphs[index - page->gx_font_first_glyph].gx_glyph_advance == 0)
            {
                dead_count++;

                if (dead_count > MAX_FONT_DEAD_BLOCK)
                {
                    break;
                }
            }
            else
            {
                stop_point = index;
                dead_count = 0;
            }
            index++;
        }
    }
    return stop_point;
}

///////////////////////////////////////////////////////////////////////////////
void CopyGlyphs(GX_FONT *dest, const GX_FONT *src)
{
    int char_count = dest->gx_font_last_glyph - dest->gx_font_first_glyph + 1;

    GX_GLYPH *glyph_array = new GX_GLYPH[char_count];
    memset(glyph_array, 0, char_count * sizeof(GX_GLYPH));
    dest->gx_font_glyphs.gx_font_normal_glyphs = glyph_array;

    GX_GLYPH *put = glyph_array;
    const GX_GLYPH *get = &src->gx_font_glyphs.gx_font_normal_glyphs[dest->gx_font_first_glyph - src->gx_font_first_glyph];

    int bitmap_size = 0;

    while(char_count--)
    {
        *put = *get;    
        put->gx_glyph_map = NULL;
        bitmap_size = GetRowPitch(put->gx_glyph_width, GetFontBits(dest->gx_font_format));
        bitmap_size *= put->gx_glyph_height;

        if(bitmap_size != 0)
        {
            put->gx_glyph_map = new UCHAR[bitmap_size];
            memcpy_s((GX_UBYTE *) put->gx_glyph_map, bitmap_size, get->gx_glyph_map, bitmap_size);
        }
        else
        {
            put->gx_glyph_map = GX_NULL;
        }

        put++;
        get++;
    }
}

///////////////////////////////////////////////////////////////////////////////
void CopyKerningGlyphs(GX_FONT *dest, const GX_FONT *src)
{
    int char_count = dest->gx_font_last_glyph - dest->gx_font_first_glyph + 1;
    GX_KERNING_GLYPH *glyph_array = new GX_KERNING_GLYPH[char_count];
    memset(glyph_array, 0, char_count * sizeof(GX_KERNING_GLYPH));
    dest->gx_font_glyphs.gx_font_kerning_glyphs = glyph_array;

    GX_KERNING_GLYPH *put = glyph_array;
    const GX_KERNING_GLYPH *get = &src->gx_font_glyphs.gx_font_kerning_glyphs[dest->gx_font_first_glyph - src->gx_font_first_glyph];

    int bitmap_size = 0;
    while (char_count--)
    {
        *put = *get;
        bitmap_size = GetRowPitch(put->gx_glyph_width, GetFontBits(dest->gx_font_format));
        bitmap_size *= put->gx_glyph_height;

        if (bitmap_size != 0)
        {
            put->gx_glyph_map = new UCHAR[bitmap_size];
            memcpy_s((GX_UBYTE *)put->gx_glyph_map, bitmap_size, get->gx_glyph_map, bitmap_size);
        }
        else
        {
            put->gx_glyph_map = GX_NULL;
        }
        put -> gx_kerning_table = GX_NULL;

        if (get -> gx_kerning_table)
        {
            /* Copy kerning table if there's one */
            INT table_size = *(get->gx_kerning_table);
            table_size = 1 + (table_size * (sizeof(GX_UBYTE) + sizeof(GX_CHAR)));
            put->gx_kerning_table = new GX_UBYTE[table_size];
            memcpy_s((GX_UBYTE *)put->gx_kerning_table, table_size, get->gx_kerning_table, table_size);
        }
        put++;
        get++;
    }
}

///////////////////////////////////////////////////////////////////////////////
void CopyCompressedGlyphs(GX_FONT *dest, const GX_FONT *src)
{
    int char_count = dest->gx_font_last_glyph - dest->gx_font_first_glyph + 1;

    GX_COMPRESSED_GLYPH *glyph_array = new GX_COMPRESSED_GLYPH[char_count];
    memset(glyph_array, 0, char_count * sizeof(GX_COMPRESSED_GLYPH));
    dest->gx_font_glyphs.gx_font_compressed_glyphs = glyph_array;

    GX_COMPRESSED_GLYPH *put = glyph_array;
    const GX_COMPRESSED_GLYPH *get = &src->gx_font_glyphs.gx_font_compressed_glyphs[dest->gx_font_first_glyph - src->gx_font_first_glyph];

    int bitmap_size = 0;

    while (char_count--)
    {
        *put = *get;
        bitmap_size = put->gx_glyph_map_size & 0x7fff;

        if (bitmap_size != 0)
        {
            put->gx_glyph_map = new UCHAR[bitmap_size];
            memcpy_s((GX_UBYTE *)put->gx_glyph_map, bitmap_size, get->gx_glyph_map, bitmap_size);
        }
        else
        {
            put->gx_glyph_map = GX_NULL;
        }

        put++;
        get++;
    }
}

///////////////////////////////////////////////////////////////////////////////
GX_FONT *optimize_font(GX_FONT *src_font)
{
    GX_FONT *head_page = NULL;
    GX_FONT *last_page = NULL;
    GX_FONT *current_page = NULL;
    const GX_FONT *src_page = src_font;

    while (src_page)
    {
        GX_CHAR_CODE first_glyph = src_page->gx_font_first_glyph;
        GX_CHAR_CODE last_glyph = src_page->gx_font_last_glyph;

        while(first_glyph <= src_page->gx_font_last_glyph)
        {
            // skip leading NULL glyphs
            first_glyph = FindFirstValidGlyph(src_page, first_glyph);

            if (first_glyph <= src_page->gx_font_last_glyph)
            {
                // stop if there is a large dead section
                last_glyph = FindDeadGlyphBlock(src_page, first_glyph);

                // make a font page for first to last:
                current_page = new GX_FONT;
                memset(current_page, 0, sizeof(GX_FONT));
                current_page->gx_font_baseline = src_page->gx_font_baseline;
                current_page->gx_font_first_glyph = first_glyph;
                current_page->gx_font_last_glyph = last_glyph;
                current_page->gx_font_format = src_page->gx_font_format;
                current_page->gx_font_line_height = src_page->gx_font_line_height;
                current_page->gx_font_postspace = src_page->gx_font_postspace;
                current_page->gx_font_prespace = src_page->gx_font_prespace;

                if (src_page->gx_font_format & GX_FONT_FORMAT_COMPRESSED)
                {
                    CopyCompressedGlyphs(current_page, src_page);
                }
                else if(src_page->gx_font_format & GX_FONT_FORMAT_KERNING)
                {
                    CopyKerningGlyphs(current_page, src_page);
                }
                else
                {
                    CopyGlyphs(current_page, src_page);
                }

                if (last_page)
                {
                    last_page->gx_font_next_page = current_page;
                }
                else
                {
                    head_page = current_page;
                }
                last_page = current_page;
                first_glyph = last_glyph + 1;
            }
        }
        src_page = src_page->gx_font_next_page;
    }
    return head_page;
}

///////////////////////////////////////////////////////////////////////////////
GX_FONT *MakeOptimizedFont(res_info *info, int display, BOOL warn_on_error)
{
    GX_FONT *optimized = GX_NULL;
    GX_FONT *new_font = MakeFont(info, display, warn_on_error);

    if (new_font)
    {
        optimized = optimize_font(new_font);
        DestroyFont(new_font);
    }
    return optimized;
}

///////////////////////////////////////////////////////////////////////////////
INT  GetFontStorage(res_info *info, studiox_project *project, int display)
{
    // return the size (in byte) of output font 

    const GX_FONT *font_page;
    const GX_FONT *head_page;
    long datasize = 0;
    long kerning_datasize = 0;
    GX_CHAR_CODE charval;
    GX_CHAR_CODE index;
    const GX_GLYPH *glyph;
    const GX_COMPRESSED_GLYPH *compressed_glyph;
    const GX_KERNING_GLYPH *kerning_glyph;
    GX_UBYTE Kerning_pairs_count;
    int pitch;

    /* Make the font again, with optimization  */
    head_page = MakeOptimizedFont(info, display);

    if (!head_page)
    {
        return datasize;
    }

    font_page = head_page;

    /* Calculate font size.  */
    while (font_page)
    {
        for (charval = font_page->gx_font_first_glyph; charval <= font_page->gx_font_last_glyph; charval++)
        {
            index = charval - font_page->gx_font_first_glyph;

            if (font_page->gx_font_format & GX_FONT_FORMAT_COMPRESSED)
            {
                compressed_glyph = &font_page->gx_font_glyphs.gx_font_compressed_glyphs[index];
                datasize += (compressed_glyph->gx_glyph_map_size & 0x7fff);
            }
            else if (font_page->gx_font_format & GX_FONT_FORMAT_KERNING)
            {
                kerning_glyph = &font_page->gx_font_glyphs.gx_font_kerning_glyphs[index];
                pitch = GetRowPitch(kerning_glyph->gx_glyph_width, info->font_bits);
                datasize += pitch * kerning_glyph->gx_glyph_height;
                if (kerning_glyph->gx_kerning_table)
                {
                    Kerning_pairs_count = *(kerning_glyph->gx_kerning_table);
                    /* Add the kerning table size. */
                    kerning_datasize += 1 + Kerning_pairs_count * (sizeof(GX_CHAR) + sizeof(GX_UBYTE));
                }
            }
            else
            {
                glyph = &font_page->gx_font_glyphs.gx_font_normal_glyphs[index];
                pitch = GetRowPitch(glyph->gx_glyph_width, info->font_bits);
                datasize += pitch * glyph->gx_glyph_height;
            }
        }

        font_page = font_page->gx_font_next_page;
    }
    DestroyFont((GX_FONT *)head_page);

    /* if kerning info size is bigger than 5 kB*/
    if (kerning_datasize > 5120)
    {
        if (kerning_datasize > datasize)
        {
            /* kerning info data size is bigger than the glyph size.
               Show notification dialog that the font size is so large. */
            CString message ("Generate kerning info of font ");
            message += info->name;
            message += " is selected and it will take lots of memory. Make sure it is wanted.";
            Notify(CW2A(message));
        }
    }

    return (datasize + kerning_datasize);
}


////////////////////////////////////////////////////////////////////////////////
INT GetPixelmapStorage(res_info *info)
{
    int storage = 0;

    if (info->raw)
    {
        CString path = MakeAbsolutePathname(info->pathinfo);
        FILE *file = _tfopen(path.GetBuffer(), _T("rb"));

        if (!file)
        {
            return 0;
        }

        fseek(file, 0, SEEK_END);
        storage = ftell(file);
        fclose(file);
    }
    else
    {
        GX_PIXELMAP *map;

        for (int index = 0; index < info->GetPixelmapFrameCount(); index++)
        {
            map = info->GetPixelmap(index);

            if (map)
            {
                storage += map->gx_pixelmap_data_size;

                if (map->gx_pixelmap_aux_data_size)
                {
                    if (info->output_color_format != GX_COLOR_FORMAT_8BIT_PALETTE ||
                        (info->palette_type == PALETTE_TYPE_PRIVATE && index == 0))
                    {
                        storage += map->gx_pixelmap_aux_data_size;
                    }
                }
            }
        }
    }

    return storage;
}