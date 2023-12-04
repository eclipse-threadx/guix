
#include "studiox_includes.h"
#include "image_reader.h"
#include "png.h"

png_structp png_instance;
png_infop   png_info_ptr;

///////////////////////////////////////////////////////////////////////////////
input_stream::input_stream(unsigned char* data, int data_size)
{
    mpData = data;
    mDataSize = data_size;
    mReadSize = 0;
}

///////////////////////////////////////////////////////////////////////////////
IFACEMETHODIMP input_stream::Read(void *pv, ULONG cb, ULONG *pcbRead)
{
    if (mReadSize < mDataSize)
    {
        ULONG remain_size = mDataSize - mReadSize;

        if (cb <= remain_size)
        {
            memcpy((void *)pv, (void *)(mpData + mReadSize), cb); /* Use case of memcpy is verified. */
            mReadSize += cb;

            if (pcbRead)
            {
                *pcbRead = cb;
            }
        }
        else
        {
            memcpy((void *)pv, (void *)(mpData + mReadSize), remain_size); /* Use case of memcpy is verified. */
            mReadSize += remain_size;

            if (pcbRead)
            {
                *pcbRead = remain_size;
            }
        }
    }

    return 0;
}

png_reader::png_reader() : image_reader()
{
    mpRowPointers = NULL;
}

png_reader::~png_reader()
{
}

BOOL png_reader::CheckImageHasAlphaChannel(CString &path)
{
    BOOL has_alpha = FALSE;
    UCHAR signature[8];

    FILE *file = _tfopen(path.GetBuffer(), _T("rb"));

    if (!file)
    {
        return FALSE;
    }

    fread(signature, 1, 8, file);
    if (png_sig_cmp(signature, 0, 8))
    {
        fclose(file);
        return FALSE;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
        NULL, NULL, NULL);
    
    if (!png_ptr)
    {
        fclose(file);
        return FALSE;
    }

    png_infop info = png_create_info_struct(png_ptr);

    if (!info)
    {
        fclose(file);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return FALSE;
    }
    
    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info);

    int color_type, num_trans;

    //get information from the info
    if (png_get_IHDR(png_ptr, info, NULL, NULL, NULL, &color_type, NULL, NULL, NULL) == 0)
    {
        color_type = -1;
    }

    if (png_get_tRNS(png_ptr, info, NULL, &num_trans, NULL) == 0)
    {
        num_trans = 0;
    }

    switch (color_type)
    {
    case PNG_COLOR_TYPE_RGB_ALPHA:
    case PNG_COLOR_TYPE_GRAY_ALPHA:
        has_alpha = TRUE;
        break;

    case PNG_COLOR_TYPE_PALETTE:
    case PNG_COLOR_TYPE_GRAY:
    case PNG_COLOR_TYPE_RGB:

        if (num_trans > 0)
        {
            has_alpha = TRUE;
        }
        break;
    default:
        break;
    }

    png_destroy_read_struct(&png_ptr, &info, NULL);
    fclose(file);
    return has_alpha;
}

///////////////////////////////////////////////////////////////////////////////
BOOL png_reader::ReadImage(CString &path, int frame_id)
{
    UCHAR signature[8];

    FILE *file = _tfopen(path.GetBuffer(), _T("rb"));

    if (!file)
    {
        return FALSE;
    }

    fread(signature, 1, 8, file);
    if (png_sig_cmp(signature, 0, 8))
    {
        fclose(file);
        return FALSE;
    }

    m_png = png_create_read_struct(PNG_LIBPNG_VER_STRING,
        NULL, NULL, NULL);
    
    if (!m_png)
    {
        fclose(file);
        return FALSE;
    }
    png_instance = m_png;

    m_info = png_create_info_struct(m_png);
    if (!m_info)
    {
        fclose(file);
        png_destroy_read_struct(&m_png, NULL, NULL);
        return FALSE;
    }
    
    png_init_io(m_png, file);
    png_set_sig_bytes(m_png, 8);
    png_read_png(m_png, m_info, PNG_TRANSFORM_IDENTITY, NULL);
    png_info_ptr = m_info;

    png_uint_32 width, height;

    // get information from the m_info
    if (png_get_IHDR(m_png, m_info, &width, &height, NULL, NULL, NULL, NULL, NULL) != 0)
    {
        mpRowPointers = png_get_rows(m_png, m_info);

        if (mpRowPointers)
        {
            ConfigureInternalFormat();
            DoCompress(FALSE);
            if (!FinalizeImage(width, height))
            {
                png_destroy_read_struct(&m_png, &m_info, NULL);
                fclose(file);
                return FALSE;
            }
        }
    }

    png_destroy_read_struct(&m_png, &m_info, NULL);
    fclose(file);
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL png_reader::ReadImage(IStream *input_stream)
{
    UCHAR signature[8];

    input_stream->Read(signature, 8, NULL);

    if (png_sig_cmp(signature, 0, 8))
    {
        return FALSE;
    }

    m_png = png_create_read_struct(PNG_LIBPNG_VER_STRING,
        NULL, NULL, NULL);

    if (!m_png)
    {
        return FALSE;
    }
    png_instance = m_png;

    m_info = png_create_info_struct(m_png);
    if (!m_info)
    {
        png_destroy_read_struct(&m_png, NULL, NULL);
        return FALSE;
    }

    // Set user-defined function for reading a PNG stream.
    png_set_read_fn(m_png, input_stream, ReadDataFromIStream);

    png_set_sig_bytes(m_png, 8);
    png_read_png(m_png, m_info, PNG_TRANSFORM_IDENTITY, NULL);
    png_info_ptr = m_info;

    png_uint_32 width, height;

    // get information from the m_info
    if (png_get_IHDR(m_png, m_info, &width, &height, NULL, NULL, NULL, NULL, NULL) != 0)
    {
        mpRowPointers = png_get_rows(m_png, m_info);

        if (mpRowPointers)
        {
            ConfigureInternalFormat();
            DoCompress(FALSE);
            if (!FinalizeImage(width, height))
            {
                png_destroy_read_struct(&m_png, &m_info, NULL);
                return FALSE;
            }
        }
    }

    png_destroy_read_struct(&m_png, &m_info, NULL);
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL png_reader::ReadImage(unsigned char* data, int data_size)
{
    input_stream inputstream(data, data_size);

    return ReadImage((IStream *)&inputstream);
}

///////////////////////////////////////////////////////////////////////////////
UCHAR *png_reader::GetInputDataPtr(int row)
{
    if (mpRowPointers)
    {
        return mpRowPointers[row];
    }
    return NULL;
}


// the data formats supported by the PNG decoder are not always
// formats that our image_reader understands, so we have to convert
// the odd formats to something we do understand

///////////////////////////////////////////////////////////////////////////////
void png_reader::ReadPixel48(image_reader *reader, int Index, Pixel *Pixel)
{
    const USHORT *pGet = (USHORT *)reader->mpGetData;
    pGet += Index * 3;
    Pixel->Red = *pGet++;
    Pixel->Green = *pGet++;
    Pixel->Blue = *pGet;

    int num_trans;
    png_color_16p trans_color;
    if (png_get_tRNS(png_instance, png_info_ptr, NULL, &num_trans, &trans_color) == 0)
    {
        num_trans = 0;
        trans_color = NULL;
    }

    if ((num_trans > 0) &&
        (trans_color->red == Pixel->Red) &&
        (trans_color->green == Pixel->Green) &&
        (trans_color->blue == Pixel->Blue))
    {
        //Transparent
        Pixel->Alpha = 0;
    }
    else
    {
        Pixel->Red &= 0xff;
        Pixel->Green &= 0xff;
        Pixel->Blue &= 0xff;
        Pixel->Alpha = 0xff;
    }
}

///////////////////////////////////////////////////////////////////////////////
void png_reader::ReadPixel1(image_reader *reader, int Index, Pixel *put)
{
    UCHAR pal_index;
    const GX_UBYTE *pGet = reader->mpGetData;

    put->Red = put->Green = put->Blue = put->Alpha = 0;

    pGet += (Index >> 3);
    pal_index = *pGet;
    pal_index >>= (7 - (Index & 0x07));
    pal_index &= 0x01;

    png_colorp palette;
    int num_palette, num_trans;
    png_bytep trans_alpha;
    png_color_16p trans_color;

    //get information from the png_ingo

    if (png_get_PLTE(png_instance, png_info_ptr, &palette, &num_palette) == 0)
    {
        palette = NULL;
        num_palette = 0;
    }

    if (png_get_tRNS(png_instance, png_info_ptr, &trans_alpha, &num_trans, &trans_color) == 0)
    {
        trans_alpha = NULL;
        num_trans = 0;
        trans_color = NULL;
    }


    if (palette)
    {
        //Palette
        if ((num_trans > 0) && (trans_alpha[0] == pal_index))
        {
            put->Alpha = 0;
        }
        else
        {
            if (pal_index < num_palette)
            {
                put->Red = palette[pal_index].red;
                put->Green = palette[pal_index].green;
                put->Blue = palette[pal_index].blue;
            }
            put->Alpha = 0xff;
        }
    }
    else
    {
        //Gray
        if ((num_trans > 0) &&  (trans_color->gray == pal_index))
        {
            put->Alpha = 0;
        }
        else
        {
            pal_index *= 255;
            put->Red = pal_index;
            put->Green = pal_index;
            put->Blue = pal_index;
            put->Alpha = 0xff;
        }
    }

}
///////////////////////////////////////////////////////////////////////////////
void png_reader::ReadPixel2(image_reader *reader, int Index, Pixel *put)
{
    UCHAR pal_index;
    const GX_UBYTE *pGet = reader->mpGetData;

    put->Red = put->Green = put->Blue = put->Alpha = 0;

    pGet += (Index >> 2);
    pal_index = *pGet;
    pal_index >>= (3 - (Index & 0x03)) * 2;
    pal_index &= 0x03;

    png_colorp palette;
    int num_palette, num_trans;
    png_bytep trans_alpha;
    png_color_16p trans_color;

    //get information from the png_info
    if (png_get_PLTE(png_instance, png_info_ptr, &palette, &num_palette) == 0)
    {
        palette = NULL;
        num_palette = 0;
    }

    if (png_get_tRNS(png_instance, png_info_ptr, &trans_alpha, &num_trans, &trans_color) == 0)
    {
        trans_alpha = NULL;
        num_trans = 0;
        trans_color = NULL;
    }

    if (palette)
    {
        //Palette
        if ((num_trans > 0) && (trans_alpha[0] == pal_index))
        {
            put->Alpha = 0;
        }
        else
        {
            if (pal_index < num_palette)
            {
                put->Red = palette[pal_index].red;
                put->Green = palette[pal_index].green;
                put->Blue = palette[pal_index].blue;
            }
            put->Alpha = 0xff;
        }
    }
    else
    {
        //Gray
        if ((num_trans > 0) && (trans_color->gray == pal_index))
        {
            put->Alpha = 0;
        }
        else
        {
            pal_index = pal_index * 255 / 3;
            put->Red = pal_index;
            put->Green = pal_index;
            put->Blue = pal_index;
            put->Alpha = 0xff;
        }
    }
}
///////////////////////////////////////////////////////////////////////////////
void png_reader::ReadPixel4(image_reader *reader, int Index, Pixel *put)
{
    UCHAR pal_index;
    const GX_UBYTE *pGet = reader->mpGetData;

    put->Red = put->Green = put->Blue = put->Alpha = 0;

    pGet += (Index >> 1);
    pal_index = *pGet;
    pal_index >>= (1 - (Index & 0x01)) * 4;
    pal_index &= 0x0f;

    png_colorp palette;
    int num_palette, num_trans;
    png_bytep trans_alpha;
    png_color_16p trans_color;

    // get information from the png_info
    if (png_get_PLTE(png_instance, png_info_ptr, &palette, &num_palette) == 0)
    {
        palette = NULL;
        num_palette = 0;
    }

    if (png_get_tRNS(png_instance, png_info_ptr, &trans_alpha, &num_trans, &trans_color) == 0)
    {
        trans_alpha = NULL;
        num_trans = 0;
        trans_color = NULL;
    }

    if (palette)
    {
        //Palette
        if ((num_trans > 0) && (trans_alpha[0] == pal_index))
        {
            put->Alpha = 0;
        }
        else
        {
            if (pal_index < num_palette)
            {
                put->Red = palette[pal_index].red;
                put->Green = palette[pal_index].green;
                put->Blue = palette[pal_index].blue;
            }
            put->Alpha = 0xff;
        }
    }
    else
    {
        //Gray

        if ((num_trans > 0) && (trans_color->gray == pal_index))
        {
            put->Alpha = 0;
        }
        else
        {
            pal_index = pal_index * 255 / 15;
            put->Red = pal_index;
            put->Green = pal_index;
            put->Blue = pal_index;
            put->Alpha = 0xff;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void png_reader::ReadPixel8(image_reader *reader, int Index, Pixel *put)
{
    UCHAR pal_index;
    const GX_UBYTE *pGet = reader->mpGetData;

    put->Red = put->Green = put->Blue = put->Alpha = 0;

    pGet += Index;
    pal_index = *pGet;

    int color_type, num_palette, num_trans;
    png_colorp palette;
    png_bytep trans_alpha;
    png_color_16p trans_color;

    //get information from the info
    if (png_get_IHDR(png_instance, png_info_ptr, NULL, NULL, NULL, &color_type, NULL, NULL, NULL) == 0)
    {
        color_type = -1;
    }

    
    if (png_get_PLTE(png_instance, png_info_ptr, &palette, &num_palette) == 0)
    {
        palette = NULL;
        num_palette = 0;
    }

    if (png_get_tRNS(png_instance, png_info_ptr, &trans_alpha, &num_trans, &trans_color) == 0)
    {
        trans_alpha = NULL;
        num_trans = 0;
        trans_color = NULL;
    }

    switch(color_type)
    {
    case 3:   // indexed color
        if (pal_index < num_palette)
        {
            put->Red = palette[pal_index].red;
            put->Green = palette[pal_index].green;    
            put->Blue = palette[pal_index].blue;
        }
        if (pal_index < num_trans)
        {
            put->Alpha = trans_alpha[pal_index];
        }
        else
        {
            put->Alpha = 0xff;
        }
        break;

    case 2: // TrueColor image data
        if (pal_index < num_palette)
        {
            put->Red = palette[pal_index].red;
            put->Green = palette[pal_index].green;    
            put->Blue = palette[pal_index].blue;
        }
        if ((num_trans > 0) && (trans_alpha[0] == pal_index))
        {
            put->Alpha = 0;
        }
        else
        {
            put->Alpha = 0xff;
        }
        break;
    
    case 0: // grayscale
        //Gray
        if ((num_trans > 0) && (trans_color->gray == pal_index))
        {
            put->Alpha = 0;
        }
        else
        {
            put->Red = pal_index;
            put->Green = pal_index;
            put->Blue = pal_index;
            put->Alpha = 0xff;
        }
        break;

    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
void png_reader::ReadPixel24(image_reader *reader, int Index, Pixel *put)
{
    const GX_UBYTE *pGet = reader->mpGetData;

    pGet += Index * 3;

    put->Red = *pGet++;
    put->Green = *pGet++;
    put->Blue = *pGet++;

    int num_trans;
    png_color_16p trans_color;

    if (png_get_tRNS(png_instance, png_info_ptr, NULL, &num_trans, &trans_color) == 0)
    {
        trans_color = NULL;
        num_trans = 0;
    }

    if ((num_trans > 0) &&
        (trans_color->red == put->Red) &&
        (trans_color->green == put->Green) &&
        (trans_color->blue == put->Blue))
    {
        //Transparent
        put->Alpha = 0;
    }
    else
    {
        put->Alpha = 0xff;
    }
}

///////////////////////////////////////////////////////////////////////////////
void png_reader::ReadPixel64(image_reader *reader, int Index, Pixel *put)
{
    const GX_UBYTE *pGet = reader->mpGetData;

    pGet += Index * 8;
    put->Red = *pGet;
    pGet += 2;
    put->Green = *pGet;
    pGet += 2;
    put->Blue =  *pGet;
    pGet += 2;
    put->Alpha = *pGet;
}

///////////////////////////////////////////////////////////////////////////////
void png_reader::ReadPixel24Alpha(image_reader *reader, int Index, Pixel *put)
{
    const GX_UBYTE *pGet = reader->mpGetData;

    pGet += Index * 4;
    put->Red =   *pGet++;
    put->Green = *pGet++;
    put->Blue =  *pGet++;
    put->Alpha = *pGet;
}


///////////////////////////////////////////////////////////////////////////////
void png_reader::ReadPixel8Alpha(image_reader *reader, int Index, Pixel *put)
{
    const GX_UBYTE *pGet = reader->mpGetData;

    pGet += Index * 2;
    put->Red = put->Green = put->Blue = *pGet++;
    put->Alpha = *pGet;
}

///////////////////////////////////////////////////////////////////////////////
void png_reader::ReadPixel16To8(image_reader *reader, int Index, Pixel *put)
{
    const USHORT *pGet = (USHORT *)reader->mpGetData;
    int gray;

    pGet += Index;   
    gray = *pGet;

    int num_trans;
    png_color_16p trans_color;

    if (png_get_tRNS(png_instance, png_info_ptr, NULL, &num_trans, &trans_color) == 0)
    {
        num_trans = 0;
        trans_color = NULL;
    }

    if ((num_trans > 0) && (trans_color->gray == gray))
    {
        //Transparent
        put->Alpha = 0;
    }
    else
    {
        put->Red = put->Green = put->Blue = (GX_UBYTE)gray;
        put->Alpha = 0xff;
    }
}

///////////////////////////////////////////////////////////////////////////////
void png_reader::ReadPixel16Alpha(image_reader *reader, int Index, Pixel *put)
{
    const GX_UBYTE *pGet = reader->mpGetData;

    pGet += (Index * 4);
    put->Red = put->Green = put->Blue = *pGet;
    pGet += 2;
    put->Alpha = *pGet;
}

///////////////////////////////////////////////////////////////////////////////
void png_reader::ReadDataFromIStream(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead)
{
    IStream *inputstream = (IStream *)png_get_io_ptr(png_ptr);

    if (inputstream)
    {
        inputstream->Read(outBytes, byteCountToRead, GX_NULL);
    }
}

///////////////////////////////////////////////////////////////////////////////
void png_reader::ConfigureInternalFormat()
{
    mInputHasAlpha = FALSE;

    int bit_depth, color_type, num_trans;

    //get information from the info
    if (png_get_IHDR(m_png, m_info, NULL, NULL, &bit_depth, &color_type, NULL, NULL, NULL) == 0)
    {
        bit_depth = 0;
        color_type = -1;
    }

    if (png_get_tRNS(m_png, m_info, NULL, &num_trans, NULL) == 0)
    {
        num_trans = 0;
    }

    switch(color_type)
    {
    case PNG_COLOR_TYPE_RGB:
        if (num_trans > 0)
        {
            mInputHasAlpha = TRUE;
        }

        if (bit_depth == 8)
        {
            // use standard raw reader provided by base class
            //SetDefaultPixelReader(24);
            mpReadInputPixel = &png_reader::ReadPixel24;
        }
        else 
        {
            // the input file has 16-bit channels. Use
            // my own reader
            mpReadInputPixel = &png_reader::ReadPixel48;
        }
        break;

    case PNG_COLOR_TYPE_RGB_ALPHA:
        mInputHasAlpha = TRUE;
        if (bit_depth == 8)
        {
            mpReadInputPixel = &png_reader::ReadPixel24Alpha;
        }
        else
        {
            // the input file has 16-bit color channels
            mpReadInputPixel = &png_reader::ReadPixel64;
        }
        break;

    case PNG_COLOR_TYPE_PALETTE:
    case PNG_COLOR_TYPE_GRAY:

        if (num_trans > 0)
        {
            mInputHasAlpha = TRUE;
        }
 
        switch(bit_depth)
        {
        case 1:
            mpReadInputPixel = &png_reader::ReadPixel1;
            break;

        case 2:
            mpReadInputPixel = &png_reader::ReadPixel2;
            break;

        case 4:
            mpReadInputPixel = &png_reader::ReadPixel4;
            break;

        case 8:
            mpReadInputPixel = &png_reader::ReadPixel8;
            break;

        case 16:
            mpReadInputPixel = &png_reader::ReadPixel16To8;
            break;
        }
        break;

    case  PNG_COLOR_TYPE_GRAY_ALPHA:
        mInputHasAlpha = TRUE;

        if (bit_depth == 8)
        {
            mpReadInputPixel = &png_reader::ReadPixel8Alpha;
        }
        else
        {
            mpReadInputPixel = &png_reader::ReadPixel16Alpha;
        }
        break;
    }
}
