
#include "studiox_includes.h"
#include "image_reader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TRANSPARENT_COLOR 0xff


/* How to use the image_reader class:

   image_reader *pReader = image_reader::CreateProperReader(path);

    if (pReader)
    {
        // this need to be set based on display properties
        pReader->SetOutputColorFormat(GX_COLOR_FORMAT_24BIT_XRGB);   
        pReader->SetSaveAlphaVal(TRUE);     // optional
        pReader->SetCompression(TRUE);      // optional

        if (pReader->ReadImage(path))
        {
            pmap = pReader->GetPixelmap();  // get back pixelmap
        }
        delete pReader;                     // cleanup
    }
*/

/* The Pixel class is a very simple class we use to pass color data around
   between the data readers and data writers. 
*/
///////////////////////////////////////////////////////////////////////////////
Pixel::Pixel(int red, int green, int blue)
{
    Red = red;
    Green = green;
    Blue = blue;
    Alpha = 0xff;
}

///////////////////////////////////////////////////////////////////////////////
Pixel::Pixel(GX_COLOR color)
{
    Red = (color >> 16) & 0xff;
    Green = (color >> 8) & 0xff;
    Blue = color & 0xff;
}
///////////////////////////////////////////////////////////////////////////////
BOOL Pixel::operator != (const Pixel &other) const
{
    if (Red == other.Red && Green == other.Green &&
        Blue == other.Blue && Alpha == other.Alpha)
    {
        return FALSE;
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL Pixel::operator == (const Pixel &other) const
{
    if (Red == other.Red && Green == other.Green &&
        Blue == other.Blue && Alpha == other.Alpha)
    {
        return TRUE;
    }
    return FALSE;
}

/* 
    Image reader base class. This class defines functionality of storing pixels to 
    memory in each of the supported formats. The base class also takes care of
    initializing the GX_PIXELMAP structure, color space remapping,
    RLE encoding, and Dithering.

    The individual PNG, JPG, etc decoders should be derived from this base. They 
    have to provide these functions:
    1) ReadImage: The main function to decode whatever type of image it is an place
        the data in RAM.

    2) GetInputDataPtr: Function that returns address of a specified scanline in the 
        source data.

    3) mpReadInputPixel: This is a function pointer that must be initialized by the 
        sub-class, once the sub-class has identified the input data format.
       Default pixel readers are provided by the base class for 16 bpp 565, 24 bpp packed,
       and 32 bpp argb formats.

    4) The sub-class should call the function FinalizeImage after it has decoded the source
       data into RAM. FinalizeImage will do the color space conversion and/or RLE
       compression.
*/

///////////////////////////////////////////////////////////////////////////////
image_reader::image_reader()
{
    mFile = NULL;
   
    mMode = 0;

    mDithering = FALSE;
    mWantAlpha = FALSE;
    mInputHasAlpha = FALSE;
    mSaveAlpha = FALSE;
    mDoCompress = FALSE;
    mTargaFormat = FALSE;

    mpReadInputPixel = NULL;
    mpReadOutputPixel = NULL;
    mpWritePixel = NULL;
    mpWriteTransparentPix = NULL;
    
    mpPalette = NULL;
    mPalSize = 0;
    mPalFixed = 0;
    mDisplayFormat = 0;
}

///////////////////////////////////////////////////////////////////////////////
image_reader::~image_reader()
{
}

// CreateProperReader: This function is called by external code to
// create an image_reader sub-class that is correct for the image type.
// i.e. the caller doesn't have to figure what type of image it is,
// just call this function and it will do it for you and return
// an image_reader instance.
///////////////////////////////////////////////////////////////////////////////
image_reader *image_reader::CreateProperReader(CString &path)
{
    return CreateProperReader(GetImageType(path));
}

///////////////////////////////////////////////////////////////////////////////
image_reader *image_reader ::CreateProperReader(unsigned char* data, int data_size)
{
    return CreateProperReader(GetImageType(data, data_size));
}

///////////////////////////////////////////////////////////////////////////////
image_reader* image_reader::CreateProperReader(int image_type)
{
    image_reader* pReader = NULL;

    switch (image_type)
    {
    case IMAGE_TYPE_PNG:
        pReader = new png_reader();
        break;

    case IMAGE_TYPE_JPG:
        pReader = new jpg_reader();
        break;

    case IMAGE_TYPE_GIF:
        pReader = new gif_reader();
        break;

    default:
        break;
    }
    return pReader;
}

// can be called by app code if image is not needed. Cleans 
// up image storage.
///////////////////////////////////////////////////////////////////////////////
void image_reader::DestroyImage(void)
{
    GX_PIXELMAP *map;

    for(int index = 0; index < mPixelmapList.GetCount(); index++)
    {
        map = mPixelmapList.GetAt(index);

        if (map->gx_pixelmap_data)
        {
            delete [] map->gx_pixelmap_data;
        }
    
        if (map->gx_pixelmap_aux_data)
        {
            delete [] map->gx_pixelmap_aux_data;
        }

        delete map;
    }
}

///////////////////////////////////////////////////////////////////////////////
GX_PIXELMAP *image_reader::ResizeImage(GX_PIXELMAP *src, int dest_width, int dest_height)
{
    if (!mPixelmapList.GetCount() || dest_width <= 0 || dest_height <= 0)
    {
        return NULL;
    }
    if (src->gx_pixelmap_flags & GX_PIXELMAP_COMPRESSED)
    {
        // can't resize a compressed image. just return:
        return NULL;
    }

    SetOutputColorFormat(src->gx_pixelmap_format, mDisplayFormat);
    int output_stride = CalculateOutputStride(dest_width);

    // Initialize Pixelmap structure and
    // create a buffer to hold the correctly formatted data:

    int new_data_size = output_stride * dest_height;
    UCHAR *pData = new UCHAR[new_data_size];
    memset(pData, 0, (output_stride * dest_height));
    GX_PIXELMAP *newmap = new GX_PIXELMAP;
    memset(newmap, 0, sizeof(GX_PIXELMAP));

    newmap->gx_pixelmap_data = pData;
    newmap->gx_pixelmap_height = dest_height;
    newmap->gx_pixelmap_width = dest_width;
    newmap->gx_pixelmap_flags = src->gx_pixelmap_flags;
    newmap->gx_pixelmap_format = src->gx_pixelmap_format;
    newmap->gx_pixelmap_data_size = new_data_size;
    newmap->gx_pixelmap_aux_data_size = 0;  
    newmap->gx_pixelmap_aux_data = NULL;
    newmap->gx_pixelmap_transparent_color = src->gx_pixelmap_transparent_color;

    // check to see if we need an aux-data
    switch(mOutputFormat)
    {
    case GX_COLOR_FORMAT_MONOCHROME:
    case GX_COLOR_FORMAT_MONOCHROME_INVERTED:
    case GX_COLOR_FORMAT_2BIT_GRAY:
    case GX_COLOR_FORMAT_2BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_4BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_4BIT_VGA:
    case GX_COLOR_FORMAT_8BIT_GRAY:
    case GX_COLOR_FORMAT_8BIT_GRAY_INVERTED:
        break;


    case GX_COLOR_FORMAT_4BIT_GRAY:
        if (src->gx_pixelmap_aux_data &&
            src->gx_pixelmap_aux_data_size)
        {
            // need a seperate transparent mask value channel:
            int aux_data_size = ((dest_width  + 7) >> 3)* dest_height;
            newmap->gx_pixelmap_aux_data = new UCHAR[aux_data_size];
            newmap->gx_pixelmap_aux_data_size = aux_data_size;
        }
        break;

    case GX_COLOR_FORMAT_8BIT_PALETTE:
        if (src->gx_pixelmap_aux_data &&
            src->gx_pixelmap_aux_data_size)
        {
            // create a copy of the palette:
            newmap->gx_pixelmap_aux_data = new GX_UBYTE[src->gx_pixelmap_aux_data_size];
            memcpy_s((void *) (newmap->gx_pixelmap_aux_data), src->gx_pixelmap_aux_data_size, src->gx_pixelmap_aux_data, src->gx_pixelmap_aux_data_size);
            newmap->gx_pixelmap_aux_data_size = src->gx_pixelmap_aux_data_size;
        }
        break;

    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
    case GX_COLOR_FORMAT_5551BGRX:
    case GX_COLOR_FORMAT_1555XRGB:
    case GX_COLOR_FORMAT_565RGB:
    case GX_COLOR_FORMAT_565BGR:
    case GX_COLOR_FORMAT_4444ARGB:
    case GX_COLOR_FORMAT_4444BGRA:
        if (mSaveAlpha)
        {
            // need a seperate alpha value channel:
            int aux_data_size = dest_width * dest_height;
            newmap->gx_pixelmap_aux_data = new UCHAR[aux_data_size];
            newmap->gx_pixelmap_aux_data_size = aux_data_size;
        }
        break;

    case GX_COLOR_FORMAT_24RGB:
    case GX_COLOR_FORMAT_24BGR:
    case GX_COLOR_FORMAT_24XRGB:
    case GX_COLOR_FORMAT_24BGRX:
    case GX_COLOR_FORMAT_32ARGB:
    case GX_COLOR_FORMAT_32RGBA:
    case GX_COLOR_FORMAT_32ABGR:
    case GX_COLOR_FORMAT_32BGRA:
        break;
    }

    BitmapStretch(src->gx_pixelmap_width, src->gx_pixelmap_height,
        dest_width, dest_height, src, newmap);

    return newmap;
}

///////////////////////////////////////////////////////////////////////////////
void image_reader::BitmapStretch(int src_width, int src_height,
    int dest_width, int dest_height, GX_PIXELMAP *src, GX_PIXELMAP *dest)
{
    //
    // This algorithm is an implementation of Fast Bitmap Stretching
    // published by Tomas Moller in Graphics Gems III, p 4-7, code
    // listing on page 411-413 and also found here:
    //
    // http://tog.acm.org/resources/GraphicsGems/gemsiii/fastBitmap.c
    //

    int dx, dy, e, d, dx2;
    int yIn = 0;
    int yOut = 0;
    dx = dest_height;
    dy = src_height;
    e = (dy << 1) - dx;
    dx2 = dx << 1;
    dy <<= 1;

    for (d = 0; d < dx; d++)
    {
        BitmapStretchStep(dest_width, src_width, yIn, yOut, src, dest);

        while(e >= 0 && dx2)
        {
            yIn++;
            e -= dx2;
        }

        yOut++;
        e += dy;
    }
}

///////////////////////////////////////////////////////////////////////////////
void image_reader::BitmapStretchStep(int dest_width,
    int source_width, int yIn, int yOut,
    GX_PIXELMAP *src, GX_PIXELMAP *dest)
{
    // Note:
    //
    // This function is an implementation of Fast Bitmap Stretching
    // published by Tomas Moller in Graphics Gems III, p 4-7, code
    // listing on page 411-413 and also found here:
    //
    // http://tog.acm.org/resources/GraphicsGems/gemsiii/fastBitmap.c
    //

    int dx, dy, e, d, dx2;
    int xIn = 0;
    int xOut = 0;
    Pixel val;
    dx = dest_width;
    dy = source_width;
    e = (dy << 1) - dx;
    dx2 = dx << 1;
    dy <<= 1;
    mpGetData = src->gx_pixelmap_data;
    int stride = CalculateOutputStride(source_width);
    mpGetData += (yIn * stride);

    stride = CalculateOutputStride(dest_width);
    mpPutData = (GX_UBYTE *) dest->gx_pixelmap_data;
    mpPutData += (yOut * stride);

    if (src->gx_pixelmap_aux_data != GX_NULL)
    {
        int SrcAuxStride = source_width;
        if (src->gx_pixelmap_format == GX_COLOR_FORMAT_4BIT_GRAY)
        {
            /* 4bpp use 1bit to mask transparent. */
            SrcAuxStride = (source_width + 7) >> 3;
        }
        mpGetAux = src->gx_pixelmap_aux_data;
        mpGetAux += yIn * SrcAuxStride;

        int DestAuxStride = dest_width;
        if (dest->gx_pixelmap_format == GX_COLOR_FORMAT_4BIT_GRAY)
        {
            /* 4bpp use 1bit to mask transparent. */
            DestAuxStride = (dest_width + 7) >> 3;
            this->mNibbleMask = 0xf0;
            this->mMonoMask = 0x80;
        }
        mpPutAux = (UCHAR *)dest->gx_pixelmap_aux_data;
        mpPutAux += yOut * DestAuxStride;
    }

    if (dest->gx_pixelmap_format == GX_COLOR_FORMAT_MONOCHROME ||
        dest->gx_pixelmap_format == GX_COLOR_FORMAT_MONOCHROME_INVERTED)
    {
        this->mMonoMask = 0x80;
    }

    for (d = 0; d < dx; d++)
    {
        val = mpReadOutputPixel(this, xIn);
        mpWritePixel(this, val);

        while(e >= 0 && dx2)
        {
            xIn++;
            e -= dx2;
        }

        xOut++;
        e += dy;
    }

    /* Make data byte-aligned one row for 4bpp fprmat.*/
    if (dest->gx_pixelmap_format == GX_COLOR_FORMAT_4BIT_GRAY)
    {
        if (this->mNibbleMask != 0xf0)
        {
            this->mNibbleMask = 0xf0;
            mpPutData++;
        }
        if (this->mMonoMask != 0x80)
        {
            this->mpPutAux++;
            this->mMonoMask = 0x80;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// called by app code to retrieve pixelmap pointer
///////////////////////////////////////////////////////////////////////////////
GX_PIXELMAP *image_reader::GetPixelmap(int frame_id)
{
    if (frame_id < mPixelmapList.GetCount())
    {
        return mPixelmapList.GetAt(frame_id);
    }
    else
    {
        return NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
int image_reader::GetFrameCount(CString& path)
{
    int frame_count = 1;

    switch (GetImageType(path))
    {
    case IMAGE_TYPE_GIF:
        frame_count = gif_reader::GetFrameCount(path);
        break;

    default:
        break;
    }

    return frame_count;
}
///////////////////////////////////////////////////////////////////////////////
// called by app code or by CreateProperReader to determine
// image type. Note that this used the image data header, not
// the filename extension, to determine image type.
///////////////////////////////////////////////////////////////////////////////
int image_reader::GetImageType(CString &path)
{
    UCHAR ImageType = IMAGE_TYPE_UNKNOWN;
    UCHAR Buffer[8];

    // examine the first few bytes of data to determine the image type:
    FILE *file = _tfopen(path.GetBuffer(), _T("rb"));

    if (file)
    {
        fread(Buffer, 1, 8, file);
        ImageType = GetImageType(Buffer, 8);
        fclose(file);
    } 
    return ImageType;
}

///////////////////////////////////////////////////////////////////////////////
int image_reader::GetImageType(unsigned char *data, int data_size)
{
    UCHAR ImageType = IMAGE_TYPE_UNKNOWN;
    UCHAR Buffer[8];

    if (data_size < sizeof(Buffer))
    {
        return ImageType;
    }

    // examine the first few bytes of data to determine the image type:
    memcpy((void *)Buffer, (void *)data, sizeof(Buffer)); /* Use case of memcpy is verified. */

    if (Buffer[0] == 'B')
    {
        ImageType = IMAGE_TYPE_BMP;
    }
    else
    {
        if (Buffer[0] == 'G')
        {
            ImageType = IMAGE_TYPE_GIF;
        }
        else
        {
            if (Buffer[1] == 'P')
            {
                ImageType = IMAGE_TYPE_PNG;
            }
            else
            {
                if (Buffer[6] == 'J')
                {
                    ImageType = IMAGE_TYPE_JPG;
                }
                else
                {
                    if (Buffer[1] == 0xd8 && Buffer[0] == 0xff && (Buffer[2] == 0xff))
                    {
                        ImageType = IMAGE_TYPE_JPG;
                    }
                }
            }
        }
    }

    return ImageType;
}

///////////////////////////////////////////////////////////////////////////////
// called by app if it wants to keep alpha values
void image_reader::SetSaveAlphaVal(BOOL save)
{
    mWantAlpha = save;

    if (save && mInputHasAlpha)
    {
        mSaveAlpha = TRUE;
    }
}

///////////////////////////////////////////////////////////////////////////////
void image_reader::DoCompress(BOOL docompress)
{
    if (docompress)
    {
        mDoCompress = TRUE;
    }
}


///////////////////////////////////////////////////////////////////////////////
void image_reader::SetPalette(GX_COLOR *pal, int size, int fixed)
{
    mpPalette = pal;
    mPalSize = size;
    mPalFixed = fixed;
}


///////////////////////////////////////////////////////////////////////////////
// Called by sub-class when raw input data is ready to go. This
// function does color space mapping and compression.
///////////////////////////////////////////////////////////////////////////////
BOOL image_reader::FinalizeImage(int width, int height)
{
    // initialize output color format and pixel writer function
    // this has to be done now, after the image has been
    // read, so that we know if we are saving alpha values
    // or not.

    if (!SetOutputColorFormat(mOutputFormat, mDisplayFormat))
    {
        return FALSE;
    }

    int output_stride = CalculateOutputStride(width);

    // Initialize Pixelmap structure and
    // create a buffer to hold the correctly formatted data:

    UCHAR *pPut = new UCHAR[output_stride * height];
    GX_PIXELMAP *map = new GX_PIXELMAP;

    memset(map, 0, sizeof(GX_PIXELMAP));

    map->gx_pixelmap_data = pPut;
    map->gx_pixelmap_height = height;
    map->gx_pixelmap_width = width;
    map->gx_pixelmap_aux_data_size = 0;  
    map->gx_pixelmap_aux_data = NULL;
    mSizeTesting = FALSE;

    // allocate aux data, do color format conversion, dithering if requested

    switch(mOutputFormat)
    {
    case GX_COLOR_FORMAT_MONOCHROME_INVERTED:
    case GX_COLOR_FORMAT_2BIT_GRAY:
    case GX_COLOR_FORMAT_2BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_4BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_4BIT_VGA:
    case GX_COLOR_FORMAT_8BIT_GRAY:
    case GX_COLOR_FORMAT_8BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_8BIT_PALETTE:
        mSaveAlpha = FALSE;
        mWantAlpha = FALSE;

        if (mInputHasAlpha)
        {
            map->gx_pixelmap_flags |= GX_PIXELMAP_TRANSPARENT;
            map->gx_pixelmap_transparent_color = TRANSPARENT_COLOR;
        }
        break;

    case GX_COLOR_FORMAT_MONOCHROME:
        mSaveAlpha = FALSE;
        mWantAlpha = FALSE;

        if (mInputHasAlpha)
        {
            map->gx_pixelmap_flags |= GX_PIXELMAP_TRANSPARENT;
        }
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
        mSaveAlpha = FALSE;
        mWantAlpha = FALSE;
        if (mInputHasAlpha)
        {
            map->gx_pixelmap_flags |= GX_PIXELMAP_TRANSPARENT;
            int aux_data_size = ((width + 7) >> 3) * height;
            map->gx_pixelmap_aux_data = new UCHAR[aux_data_size];
            map->gx_pixelmap_aux_data_size = aux_data_size;
        }
        break;

    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
    case GX_COLOR_FORMAT_5551BGRX:
    case GX_COLOR_FORMAT_1555XRGB:
    case GX_COLOR_FORMAT_565RGB:
    case GX_COLOR_FORMAT_565BGR:
        if (mSaveAlpha) // && !do_compress)
        {
            // need a seperate alpha value channel:
            int aux_data_size = width * height;
            map->gx_pixelmap_aux_data = new UCHAR[aux_data_size];
            map->gx_pixelmap_aux_data_size = aux_data_size;
        }
        break;

    case GX_COLOR_FORMAT_4444ARGB:
    case GX_COLOR_FORMAT_4444BGRA:
        break;

    case GX_COLOR_FORMAT_24RGB:
    case GX_COLOR_FORMAT_24BGR:
    case GX_COLOR_FORMAT_24XRGB:
    case GX_COLOR_FORMAT_24BGRX:
    case GX_COLOR_FORMAT_32ARGB:
    case GX_COLOR_FORMAT_32RGBA:
    case GX_COLOR_FORMAT_32ABGR:
    case GX_COLOR_FORMAT_32BGRA:
        // don't try to dither if 24 bpp or higher
        if (!(mDisplayFormat == GX_COLOR_FORMAT_565RGB) || (!mSaveAlpha))
        {
            mMode &= ~IMAGE_READER_MODE_DITHER;
        }
        break;
    }

    ColorSpaceConvert(width, height, map);

    if (mSaveAlpha)
    {
        map->gx_pixelmap_flags |= GX_PIXELMAP_ALPHA;
    }
    map->gx_pixelmap_format = mOutputFormat;

    mPixelmapList.Add(map);
    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// called internally as part of RLE encoding
int image_reader::CountDuplicates(int Index, int Width)
{
    int duplicates = 1;
    Pixel val1;
    Pixel val2;

    val1 = mpReadOutputPixel(this, Index);
    Index++;

    while(Index < Width)
    {
        val2 = mpReadOutputPixel(this, Index);
        if (val1 == val2)
        {
            duplicates++;
        }
        else
        {
            break;
        }

        Index++;
    }

    return(duplicates);
}

///////////////////////////////////////////////////////////////////////////////
void image_reader::ComputeGrayThreshod(int width, int height)
{
    // This function computes a global threshod lies in range 0 ~ 255) that used 
    // to convert an gray image to binary image 
    mGrayThreshod = 0;
    int count = 0;

    Pixel val;
    GX_UBYTE gray;

    for (int row = 0; row < height; row++)
    {
        mpGetData = GetInputDataPtr(row);

        for (int col = 0; col < width; col++)
        {
            mpReadInputPixel(this, col, &val);

            if (val.Alpha > 128)
            {
                ConvertRGBToGray(val.Red, val.Green, val.Blue, (GX_UBYTE *)&gray);
                mGrayThreshod += gray;
                count++;
            }
        }
    }

    if (count)
    {
        mGrayThreshod /= count;
    }

    if (mGrayThreshod == 0)
    {
        //All pixels are black
        mGrayThreshod = 255;
    }
    else if (mGrayThreshod == 255)
    {
        mGrayThreshod = 0;
    }

    mMonoMask = 0x80;
}

///////////////////////////////////////////////////////////////////////////////
void image_reader::ColorSpaceConvert(int width, int height, GX_PIXELMAP *map)
{
    mpPutData = (UCHAR *) map->gx_pixelmap_data;
    mpPutAux = (UCHAR *) map->gx_pixelmap_aux_data;
       
    if (mInputHasAlpha && mWantAlpha)
    {
        mSaveAlpha = TRUE;
    }

    if (mMode & IMAGE_READER_MODE_DITHER)
    {
        InitDither(width);
    }

    if (mOutputFormat == GX_COLOR_FORMAT_MONOCHROME)
    {
        // Compute gray shreshold that can be used to generate
        // binary image
        ComputeGrayThreshod(width, height);
        mMonoMask = 0x80;
    }
    else if (mOutputFormat == GX_COLOR_FORMAT_4BIT_GRAY)
    {
        /* Use mNibbleMask to mask the wirte bits of 4bit */
        mNibbleMask = 0xf0;
        mMonoMask = 0x80;
    }

    for (int row = 0; row < height; row++)
    {
        mpGetData = GetInputDataPtr(row);

        if (mMode & IMAGE_READER_MODE_DITHER)
        {
            DitherOneRow(width);
        }
        else
        {
            ColorSpaceConvertRow(width);
        }

        if ((mOutputFormat == GX_COLOR_FORMAT_MONOCHROME) && (mMonoMask != 0x80))
        {
            while (mMonoMask)
            {
                /* Set bit to make the output always be same. */
                *mpPutData &= ~mMonoMask;
                mMonoMask >>= 1;
            }
            mpPutData++;
            mMonoMask = 0x80;
        }
        else if (mOutputFormat == GX_COLOR_FORMAT_4BIT_GRAY)
        {
            if (mNibbleMask != 0xf0)
            {
                mpPutData++;
                mNibbleMask = 0xf0;
            }

            if (mMonoMask != 0x80)
            {
                mpPutAux++;
                mMonoMask = 0x80;
            }
        }
    }

    map->gx_pixelmap_data_size = (mpPutData - map->gx_pixelmap_data);
    map->gx_pixelmap_aux_data_size = (mpPutAux - map->gx_pixelmap_aux_data);
    map->gx_pixelmap_format = mOutputFormat;

    if (map->gx_pixelmap_format == GX_COLOR_FORMAT_8BIT_PALETTE)
    {
        if (mDisplayFormat > GX_COLOR_FORMAT_8BIT_PALETTE)
        {
            map->gx_pixelmap_aux_data = (GX_UBYTE *) GetPalette();
            map->gx_pixelmap_aux_data_size = GetPalSize() * sizeof(GX_COLOR);
        }
    }
    
    if (mMode & IMAGE_READER_MODE_DITHER)
    {
        DitherCleanup();
    }
}

///////////////////////////////////////////////////////////////////////////////
void image_reader::ColorSpaceConvertRow(int Width)
{
    Pixel val;
    int PixIndex;

    for (PixIndex = 0; PixIndex < Width; PixIndex++)
    {
        mpReadInputPixel(this, PixIndex, &val);
        mpWritePixel(this, val);
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL image_reader::RleEncode(GX_PIXELMAP *pMap, BOOL targa_format)
{
    BOOL status = FALSE;
    int Row;
    int OriginalSize;
    int datasize;
    int auxsize;
    int DataWidth;
    int DataHeight;

    int RowDataSize = 0;
    int RowAuxSize = 0;
    int CompressedDataSize = 0;
    int CompressedAuxSize = 0;

    mpGetData = pMap->gx_pixelmap_data;
    mpGetAux = pMap->gx_pixelmap_aux_data;

    mSizeTesting = TRUE;
    mTargaFormat = targa_format;

    DataWidth = pMap->gx_pixelmap_width;
    DataHeight = pMap->gx_pixelmap_height;
    int rowRows = DataHeight;
    int output_stride = CalculateOutputStride(pMap->gx_pixelmap_width);
    int auxstride = DataWidth;

    if (mOutputFormat == GX_COLOR_FORMAT_4BIT_GRAY)
    {
        /* Use mMonoMask to mask the wirte bits of 4bit(not trans-info) for 4bpp driver. */
        mNibbleMask = 0xf0;
        if (mInputHasAlpha)
        {
            auxstride = (auxstride + 7) >> 3;
        }
    }
    else if (mOutputFormat == GX_COLOR_FORMAT_MONOCHROME)
    {
        if (mInputHasAlpha)
        {
            auxstride = (auxstride + 3) >> 2;
        }
        else
        {
            auxstride = (auxstride + 7) >> 3;
        }
    }


    // first, compute size of compresses data so that we can
    // test to see if it is smaller
    mpPutData = NULL;
    mpPutAux = NULL;

    for (Row = 0; Row < DataHeight; Row++)
    {
        mpGetData = pMap->gx_pixelmap_data + (Row * output_stride);
        mpGetAux = pMap->gx_pixelmap_aux_data;

        if (mpGetAux)
        {
            mpGetAux += (Row * auxstride);
        }
        datasize = auxsize = 0;

        RleEncodeRow(DataWidth);
    }

    CompressedDataSize = (ULONG) mpPutData;
    CompressedAuxSize = (ULONG) mpPutAux;

    if (mOutputFormat == GX_COLOR_FORMAT_4BIT_GRAY)
    {
        /* For 4bit driver, mNibbleMask equals 0x0f means there's a pixel is stored,
        but it hasn't been contain by datasize. See parameter mpPutData in function WritePixel4bppgrayscale for more info. */
        if ((!mInputHasAlpha) && (mNibbleMask == 0x0f))
        {
            CompressedDataSize++;
        }
    }

    mSizeTesting = FALSE;
    OriginalSize = pMap->gx_pixelmap_data_size;
    OriginalSize += pMap->gx_pixelmap_aux_data_size;

    if ((CompressedDataSize + CompressedAuxSize) >= OriginalSize)
    {
        // compression does not improve size, just return
        mTargaFormat = FALSE;
        //Clear flag mDoCompress.
        mDoCompress = FALSE;
        return FALSE;
    }

    mpPutData = new UCHAR[CompressedDataSize];
    memset(mpPutData, 0x0, CompressedDataSize);
    GX_UBYTE *pNewData = mpPutData;

    if (CompressedAuxSize)
    {
        mpPutAux = new UCHAR[CompressedAuxSize];
    }
    GX_UBYTE *pNewAux = mpPutAux;

    if (mOutputFormat == GX_COLOR_FORMAT_4BIT_GRAY)
    {
        /* Use mMonoMask to mask the wirte bits of 4bit(not trans-info) for 4bpp driver. */
        mNibbleMask = 0xf0;
    }

    for (Row = 0; Row < DataHeight; Row++)
    {   
        mpGetData = pMap->gx_pixelmap_data + (Row * output_stride);
        mpGetAux = NULL;

        if (pMap->gx_pixelmap_aux_data)
        {
            mpGetAux = pMap->gx_pixelmap_aux_data + (Row * auxstride);
        }
        RleEncodeRow(DataWidth);
    }

    if (pMap->gx_pixelmap_data)
    {
        delete [] pMap->gx_pixelmap_data;
    }
    pMap->gx_pixelmap_data = pNewData;
    pMap->gx_pixelmap_data_size = CompressedDataSize;

    if (pMap->gx_pixelmap_format != GX_COLOR_FORMAT_8BIT_PALETTE)
    {
        if (pMap->gx_pixelmap_aux_data)
        {
            delete[] pMap->gx_pixelmap_aux_data;
        }

        pMap->gx_pixelmap_aux_data = pNewAux;
        pMap->gx_pixelmap_aux_data_size = CompressedAuxSize;
    }

    pMap->gx_pixelmap_flags |= GX_PIXELMAP_COMPRESSED;

    if (mTargaFormat)
    {
        pMap->gx_pixelmap_flags |= GX_PIXELMAP_TARGA;
        mTargaFormat = FALSE;
    }
    //Clear flag mDoCompress.
    mDoCompress = FALSE;
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
GX_UBYTE *image_reader::GetCountLocation()
{
    GX_UBYTE *pPutByteCount = NULL;

    if (mTargaFormat)
    {
        pPutByteCount = mpPutData++;
    }
    else
    {
        switch(mOutputFormat)
        {
        case GX_COLOR_FORMAT_MONOCHROME:
        case GX_COLOR_FORMAT_MONOCHROME_INVERTED:
            pPutByteCount = mpPutData;
            break;

        case GX_COLOR_FORMAT_2BIT_GRAY:
        case GX_COLOR_FORMAT_2BIT_GRAY_INVERTED:
        case GX_COLOR_FORMAT_4BIT_GRAY_INVERTED:
        case GX_COLOR_FORMAT_4BIT_VGA:
            // FIXME: These formats are not done yet
            break;

        case GX_COLOR_FORMAT_4BIT_GRAY:
            // counts go in the aux data array:
            pPutByteCount = mpPutAux++;
            break;

        case GX_COLOR_FORMAT_8BIT_GRAY:
        case GX_COLOR_FORMAT_8BIT_GRAY_INVERTED:
        case GX_COLOR_FORMAT_8BIT_PALETTE:
        case GX_COLOR_FORMAT_8BIT_ALPHAMAP:
            pPutByteCount = mpPutData++;
            break;

        case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
            if (mSaveAlpha)
            {
                // two byte count in the main data stream
                pPutByteCount = mpPutData;
                mpPutData += 2;
            }
            else
            {
                // 1 byte count in main data stream
                // keep the data pointer quad aligned
                pPutByteCount = mpPutData++;
            }
            break;

        case GX_COLOR_FORMAT_5551BGRX:
        case GX_COLOR_FORMAT_1555XRGB:
        case GX_COLOR_FORMAT_565RGB:
        case GX_COLOR_FORMAT_565BGR:
            if (mSaveAlpha)
            {
                // 1 byte count in main data stream
                // keep the data pointer quad aligned
                pPutByteCount = mpPutData;
            }
            else
            {
                // two byte count in the main data stream
                pPutByteCount = mpPutData;
                mpPutData += 2;
            }
            break;

        case GX_COLOR_FORMAT_4444ARGB:
        case GX_COLOR_FORMAT_4444BGRA:
            // two byte count in the main data stream
            pPutByteCount = mpPutData;
            mpPutData += 2;
            break;

        case GX_COLOR_FORMAT_24RGB:
        case GX_COLOR_FORMAT_24BGR:
            // one byte count in the main data stream, but must be
            // ULONG aligned

            while(((ULONG) mpPutData) & 0x03)
            {
                mpPutData++;
            }
            pPutByteCount = mpPutData++;
            break;

        case GX_COLOR_FORMAT_24XRGB:
        case GX_COLOR_FORMAT_24BGRX:
        case GX_COLOR_FORMAT_32ARGB:
        case GX_COLOR_FORMAT_32BGRA:
            // counts go in the aux data array:
            pPutByteCount = mpPutAux++;
            break;
        }
    }
    return pPutByteCount;
}

///////////////////////////////////////////////////////////////////////////////
USHORT image_reader::WriteRleCount(USHORT count, UCHAR *pPutCount, BOOL repeat)
{
    USHORT *pPutWordCount;
    count -= 1;

    if (mTargaFormat)
    {
        if (count > 127)
        {
            count = 127;
        }
        if (repeat)
        {
            count |= 0x80;
        }
        if (!mSizeTesting)
        {
            *pPutCount = (UCHAR) count;
        }
        return ((count & 0x7f) + 1);
    }

    switch(mOutputFormat)
    {
    case GX_COLOR_FORMAT_5551BGRX:
    case GX_COLOR_FORMAT_1555XRGB:
    case GX_COLOR_FORMAT_565RGB:
    case GX_COLOR_FORMAT_565BGR:
        if (mSaveAlpha)
        {
            if (count > 127)
            {
                count = 127;
            }
            if (repeat)
            {
                count |= 0x80;
            }
            if (!mSizeTesting)
            {
                *pPutCount = (UCHAR) count;
            }
            return ((count & 0x7f) + 1);
        }
        else
        {
            // 16 bit compressed with no alpha uses a two-byte count
            if (repeat)
            {
                count |= 0x8000;
            }
        
            pPutWordCount = (USHORT *) pPutCount;
            if (!mSizeTesting)
            {
                *pPutWordCount = count;
            }
            return ((count & 0x7fff) + 1);
        }
        break;

    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
        if (mSaveAlpha)
        {
            if (repeat)
            {
                count |= 0x8000;
            }

            pPutWordCount = (USHORT *)pPutCount;
            if (!mSizeTesting)
            {
                *pPutWordCount = count;
            }
            return ((count & 0x7fff) + 1);

        }
        else
        {
            // 8 bit compressed with no alpha uses a one-byte count
            if (count > 127)
            {
                count = 127;
            }
            if (repeat)
            {
                count |= 0x80;
            }
            if (!mSizeTesting)
            {
                *pPutCount = (UCHAR)count;
            }
            return ((count & 0x7f) + 1);
        }
        break;
        break;

    case GX_COLOR_FORMAT_4444ARGB:
    case GX_COLOR_FORMAT_4444BGRA:
        if (repeat)
        {
            count |= 0x8000;
        }

        pPutWordCount = (USHORT *)pPutCount;
        if (!mSizeTesting)
        {
            *pPutWordCount = count;
        }
        return ((count & 0x7fff) + 1);
        break;

    case GX_COLOR_FORMAT_MONOCHROME:
        if (mInputHasAlpha)
        {
            if (count > 0x1f)
            {
                count = 0x1f;
            }
            if (repeat)
            {
                count |= 0x20;
            }
            if (!mSizeTesting)
            {
                *pPutCount |= (GX_UBYTE)(count << 2);
            }

            return ((count & 0x1f) + 1);
        }
        else
        {
            if (count > 0x3f)
            {
                count = 0x3f;
            }
            if (repeat)
            {
                count |= 0x40;
            }
            if (!mSizeTesting)
            {
                *pPutCount |= (GX_UBYTE)(count << 1);
            }

            return ((count & 0x3f) + 1);
        }
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
    default:
        // everything else uses a 1-byte count
        if (count > 127)
        {
            count = 127;
        }
        if (repeat)
        {
            count |= 0x80;
        }
        if (!mSizeTesting)
        {
            *pPutCount = (UCHAR) count;
        }
        return ((count & 0x7f) + 1);
    }
}

///////////////////////////////////////////////////////////////////////////////
void image_reader::RleEncodeRow(int Width)
{
    int PixIndex = 0;
    Pixel val;
    int count;
    int RawCount = 0;
    UCHAR *pPutByteCount = NULL;

    while(PixIndex < Width)
    {
        count = CountDuplicates(PixIndex, Width);

        if (count >= 3)     // have 3 or more duplicates?
        {
            if (RawCount)   // previous raw count?
            {
                // save the raw count to the location previously save address
                WriteRleCount(RawCount, pPutByteCount, FALSE);
            }
            // the previous raw count is now saved.
            // Now write
            // out the repeat count and save one value
            pPutByteCount = GetCountLocation();
            count = WriteRleCount(count, pPutByteCount, TRUE);

            val = mpReadOutputPixel(this, PixIndex);
            mpWritePixel(this, val);
            PixIndex += count;
            RawCount = 0;
        }
        else
        {
            if (!RawCount)
            {
                // save spot to write our raw count
                pPutByteCount = GetCountLocation();
            }
                
            RawCount++;
            val = mpReadOutputPixel(this, PixIndex);
            mpWritePixel(this, val);
            PixIndex++;

            // check for being forced to write out the raw count value
            // because we are at the end of row or our count exceeds
            // our max
            if (RawCount == 128 || PixIndex == Width)
            {
                WriteRleCount(RawCount, pPutByteCount, FALSE);
                RawCount = 0;
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// Note: 
// The dithering code is based on the Independent Jpeg Group
// libjpg code. It's called the Floyd Steinburg dithering
// algorithm, and you can find implementations all over the place,
// including here:
/*
// http://en.literateprograms.org/Floyd-Steinberg_dithering_%28C%29
*/
// It's been moved here so that all the color space
// conversions can use this dithering if they want to.
//
// The dithering algorithm simply keeps track of the cumulative error
// in the converted colors, and adds this error to the next requested
// color. This eventually causes us to choose a different color, so that
// sum of the colors matches the requested colors sum over time.
//
// The dithering algorithm works left-to-right on one pass, then right
// to left on the next pass to help prevent vertical lines caused by switching
// colors at the same point on row after row.
///////////////////////////////////////////////////////////////////////////////
void image_reader::DitherOneRow(int Width)
{
    int Index = 0;
    Pixel OutPixel;
    Pixel WantPixel;
    int Col;
    int LimitCol;
    int SpeckleLimit = DITHER_SPECKLE_LIMIT;

    memset(mpNextRedErr, 0, sizeof(int) * (Width + 2));
    memset(mpNextGreenErr, 0, sizeof(int) * (Width + 2));
    memset(mpNextBlueErr, 0, sizeof(int) * (Width + 2));

    if (mDitherDirection)
    {
        Col = 0;
        LimitCol = Width;
    }
    else
    {
        Col = Width - 1;
        LimitCol = -1;
        Index = Width - 1;
    }

    do 
    {
        mpReadInputPixel(this, Index, &OutPixel);

        OutPixel.Red +=  mpThisRedErr[Col + 1] / FS_SCALE ;
        OutPixel.Green += mpThisGreenErr[Col + 1] / FS_SCALE;
        OutPixel.Blue  += mpThisBlueErr[Col + 1] / FS_SCALE;

        if (OutPixel.Red < 0)
        {
            OutPixel.Red = 0;
        }
        else
        {
            if (OutPixel.Red > 255)
            {
                OutPixel.Red = 255;
            }
        }

        if (OutPixel.Green < 0)
        {
            OutPixel.Green = 0;
        }
        else
        {
            if (OutPixel.Green > 255)
            {
                OutPixel.Green = 255;
            }
        }

        if (OutPixel.Blue < 0)
        {
            OutPixel.Blue = 0;
        }
        else
        {
            if (OutPixel.Blue > 255)
            {
                OutPixel.Blue = 255;
            }
        }

        WantPixel = OutPixel;
        Pixel PixelWritten = mpWritePixel(this, OutPixel);
        int err;

        if (mDitherDirection)
        {
            err = (WantPixel.Red - PixelWritten.Red) * FS_SCALE;
                
            if (abs(err) > SpeckleLimit)
            {
                mpThisRedErr[Col + 2] += ( err * 7 ) / 16;
                mpNextRedErr[Col    ] += ( err * 3 ) / 16;
                mpNextRedErr[Col + 1] += ( err * 5 ) / 16;
                mpNextRedErr[Col + 2] += ( err     ) / 16;
            }
            else
            {
                mpThisRedErr[Col + 2] = 0;
                mpNextRedErr[Col    ] = 0;
                mpNextRedErr[Col + 1] = 0;
                mpNextRedErr[Col + 2] = 0;
            }
                
            err = (WantPixel.Green - PixelWritten.Green) * FS_SCALE;

            if (abs(err) > SpeckleLimit)
            {
                mpThisGreenErr[Col + 2] += ( err * 7 ) / 16;
                mpNextGreenErr[Col    ] += ( err * 3 ) / 16;
                mpNextGreenErr[Col + 1] += ( err * 5 ) / 16;
                mpNextGreenErr[Col + 2] += ( err     ) / 16;
            }
            else
            {
                mpThisGreenErr[Col + 2] = 0;
                mpNextGreenErr[Col    ] = 0;
                mpNextGreenErr[Col + 1] = 0;
                mpNextGreenErr[Col + 2] = 0;
            }

            err = (WantPixel.Blue - PixelWritten.Blue) * FS_SCALE;

            if (abs(err) > SpeckleLimit)
            {
                mpThisBlueErr[Col + 2] += ( err * 7 ) / 16;
                mpNextBlueErr[Col    ] += ( err * 3 ) / 16;
                mpNextBlueErr[Col + 1] += ( err * 5 ) / 16;
                mpNextBlueErr[Col + 2] += ( err     ) / 16;
            }
            else
            {
                mpThisBlueErr[Col + 2] = 0;
                mpNextBlueErr[Col    ] = 0;
                mpNextBlueErr[Col + 1] = 0;
                mpNextBlueErr[Col + 2] = 0;
            }
        }
        else
        {
            err = (WantPixel.Red - PixelWritten.Red) * FS_SCALE;

            if (abs(err) > SpeckleLimit)
            {
                mpThisRedErr[Col    ] += ( err * 7 ) / 16;
                mpNextRedErr[Col + 2] += ( err * 3 ) / 16;
                mpNextRedErr[Col + 1] += ( err * 5 ) / 16;
                mpNextRedErr[Col    ] += ( err     ) / 16;
            }
            else
            {
                mpThisRedErr[Col    ] = 0;
                mpNextRedErr[Col + 2] = 0;
                mpNextRedErr[Col + 1] = 0;
                mpNextRedErr[Col    ] = 0;
            }

            err = (WantPixel.Green - PixelWritten.Green) * FS_SCALE;

            if (abs(err) > SpeckleLimit)
            {
                mpThisGreenErr[Col    ] += ( err * 7 ) / 16;
                mpNextGreenErr[Col + 2] += ( err * 3 ) / 16;
                mpNextGreenErr[Col + 1] += ( err * 5 ) / 16;
                mpNextGreenErr[Col    ] += ( err     ) / 16;
            }
            else
            {
                mpThisGreenErr[Col    ] = 0;
                mpNextGreenErr[Col + 2] = 0;
                mpNextGreenErr[Col + 1] = 0;
                mpNextGreenErr[Col    ] = 0;
            }
                
            err = (WantPixel.Blue - PixelWritten.Blue) * FS_SCALE;

            if (abs(err) > SpeckleLimit)
            {
                mpThisBlueErr[Col    ] += ( err * 7 ) / 16;
                mpNextBlueErr[Col + 2] += ( err * 3 ) / 16;
                mpNextBlueErr[Col + 1] += ( err * 5 ) / 16;
                mpNextBlueErr[Col    ] += ( err     ) / 16;
            }
            else
            {
                mpThisBlueErr[Col    ] = 0;
                mpNextBlueErr[Col + 2] = 0;
                mpNextBlueErr[Col + 1] = 0;
                mpNextBlueErr[Col    ] = 0;
            }
        }

        if (mDitherDirection)
        {
            Col++;
            Index++;
        }
        else
        {
            Col--;
            Index--;
        }
    } while ( Col != LimitCol);

    int *temperr = mpThisRedErr;
    mpThisRedErr = mpNextRedErr;
    mpNextRedErr = temperr;
    temperr = mpThisGreenErr;
    mpThisGreenErr = mpNextGreenErr;
    mpNextGreenErr = temperr;
    temperr = mpThisBlueErr;
    mpThisBlueErr = mpNextBlueErr;
    mpNextBlueErr = temperr;
    //mDitherDirection = !mDitherDirection;
}

///////////////////////////////////////////////////////////////////////////////
void image_reader::DitherCleanup(void)
{
    delete mpThisRedErr;
    delete mpNextRedErr;
    delete mpThisGreenErr;
    delete mpNextGreenErr;
    delete mpThisBlueErr;
    delete mpNextBlueErr;
    mDithering = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// allocate space for keeping track of dithering error values
BOOL image_reader::InitDither(int cols)
{
    mpThisRedErr = new int[cols + 2];
    mpNextRedErr = new int[cols + 2];
    mpThisGreenErr = new int[cols + 2];
    mpNextGreenErr = new int[cols + 2];
    mpThisBlueErr = new int[cols + 2];
    mpNextBlueErr = new int[cols + 2];

    memset(mpThisRedErr, 0, sizeof(int) * (cols + 2));
    memset(mpThisGreenErr, 0, sizeof(int) * (cols + 2));
    memset(mpThisBlueErr, 0, sizeof(int) * (cols + 2));
    mDitherDirection = 1;
    mDithering = TRUE;
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Some very basic pixel readers that might be re-used by several
// codecs so they are included in base image_reader class. Most of the
// readers are provided by the sub-class, since they are specific to
// that file type's data formats.

void image_reader::ReadPixel16(image_reader *reader, int Index,  Pixel *Pixel)
{
    USHORT *pVal = (USHORT *) reader->mpGetData;
    pVal += Index;
    USHORT RawVal = *pVal;

    Pixel->Red = (RawVal >> 8) & 0xf8;
    Pixel->Green = (RawVal >> 3) & 0xfc;
    Pixel->Blue = (RawVal << 3) & 0xf8;
    Pixel->Alpha = 0xff;
}

///////////////////////////////////////////////////////////////////////////////
void image_reader::ReadPixel24(image_reader *reader, int Index, Pixel *Pixel)
{
    UCHAR *pGet = (UCHAR *) reader->mpGetData;
    pGet += Index * 3;
    Pixel->Red = *pGet++;
    Pixel->Green = *pGet++;
    Pixel->Blue = *pGet;
    Pixel->Alpha = 0xff;
}

///////////////////////////////////////////////////////////////////////////////
void image_reader::ReadPixel32(image_reader *reader, int Index, Pixel *Pixel)
{
    const GX_UBYTE *pGet = reader->mpGetData;
    pGet += Index * 4;
    Pixel->Alpha = *pGet++;
    Pixel->Red = *pGet++;
    Pixel->Green = *pGet++;
    Pixel->Blue = *pGet;
}

///////////////////////////////////////////////////////////////////////////////
void image_reader::SetDefaultPixelReader(int bits_per_pix)
{
    switch(bits_per_pix)
    {
    case 15:
    case 16:
        mpReadInputPixel = &image_reader::ReadPixel16;
        break;

    case 24:
        mpReadInputPixel = &image_reader::ReadPixel24;
        break;

    case 32:
        mpReadInputPixel = &image_reader::ReadPixel32;
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL image_reader::SetOutputColorFormat(int output_format, int display_format)
{
    mOutputFormat = output_format;
    mDisplayFormat = display_format;

    if (mInputHasAlpha && mWantAlpha)
    {
        mSaveAlpha = TRUE;
    }

    switch(output_format)
    {
    case GX_COLOR_FORMAT_MONOCHROME:
    case GX_COLOR_FORMAT_MONOCHROME_INVERTED:
        if (mInputHasAlpha)
        {
            mpReadOutputPixel = &image_reader::ReadPixelMonoTransparent;
            mpWritePixel = &image_reader::WritePixelMono_transparent;
        }
        else
        {
            mpReadOutputPixel = &image_reader::ReadPixelMono;
            mpWritePixel = &image_reader::WritePixelMono;
        }
        break;

    case GX_COLOR_FORMAT_2BIT_GRAY:
    case GX_COLOR_FORMAT_2BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_4BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_4BIT_VGA:
        // not currently supported
        return FALSE;
    
    case GX_COLOR_FORMAT_8BIT_GRAY:
    case GX_COLOR_FORMAT_8BIT_GRAY_INVERTED:
        // not currently supported
        return FALSE;

    case GX_COLOR_FORMAT_4BIT_GRAY:
        if (mInputHasAlpha)
        {
            mpWritePixel = &image_reader::WritePixel4bppgrayscale_transparent;
            mpReadOutputPixel = &image_reader::ReadPixel4bppgrayscale_transparent;
        }
        else
        {
            mpWritePixel = &image_reader::WritePixel4bppgrayscale;
            mpReadOutputPixel = &image_reader::ReadPixel4bppgrayscale;
        }
        break;

    case GX_COLOR_FORMAT_8BIT_PALETTE:
        mpWritePixel = &image_reader::WritePixel8bppPalette;
        mpReadOutputPixel = &image_reader::ReadPixel8bppPalette;
        break;

    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
        if (mSaveAlpha)
        {
            /*Not support alpha yet.*/
            mpWritePixel = &image_reader::WritePixel8Bit_332_Alpha;
            mpReadOutputPixel = &image_reader::ReadPixel8Bit_332_Alpha;
        }
        else
        {
            mpWritePixel = &image_reader::WritePixel8Bit_332_RGB;
            mpReadOutputPixel = &image_reader::ReadPixel8Bit_332_RGB;
        }
        break;

    case GX_COLOR_FORMAT_8BIT_ALPHAMAP:
        mpWritePixel = &image_reader::WritePixel8bppAlpha;
        mpReadOutputPixel = &image_reader::ReadPixel8bppAlpha;
        break;

    case GX_COLOR_FORMAT_5551BGRX:
        mpWritePixel = &image_reader::WritePixel16Bit_555_BGR;
        mpReadOutputPixel = &image_reader::ReadPixel16Bit_555_BGR;
        break;

    case GX_COLOR_FORMAT_1555XRGB:
        if (mSaveAlpha)
        {
            mpWritePixel = &image_reader::WritePixel16Bit_555_Alpha;
            mpReadOutputPixel = &image_reader::ReadPixel16Bit_555_Alpha;
        }
        else
        {
            mpWritePixel = &image_reader::WritePixel16Bit_555;
            mpReadOutputPixel = &image_reader::ReadPixel16Bit_555;
        }
        break;

    case GX_COLOR_FORMAT_565RGB:
        if (mSaveAlpha)
        {
            mpWritePixel = &image_reader::WritePixel16Bit_565_Alpha;
            mpReadOutputPixel = &image_reader::ReadPixel16Bit_565_Alpha;
        }
        else
        {
            mpWritePixel = &image_reader::WritePixel16Bit_565_RGB;
            mpReadOutputPixel = &image_reader::ReadPixel16Bit_565_RGB;
        }
        break;

    case GX_COLOR_FORMAT_565BGR:
        if (mSaveAlpha)
        {
            //mpWritePixel = &image_reader::WritePixel16Bit_565_Alpha;
            //mpReadOutputPixel = &image_reader::ReadPixel16Bit_565_Alpha;
            mpWritePixel = &image_reader::WritePixel16Bit_565_BGR_Alpha;
            mpReadOutputPixel = &image_reader::ReadPixel16Bit_565_BGR_Alpha;
        }
        else
        {
            //mpWritePixel = &image_reader::WritePixel16Bit_565_RGB;
            //mpReadOutputPixel = &image_reader::ReadPixel16Bit_565_RGB;

            mpWritePixel = &image_reader::WritePixel16Bit_565_BGR;
            mpReadOutputPixel = &image_reader::ReadPixel16Bit_565_BGR;
        }
        break;
    
  
    case GX_COLOR_FORMAT_4444ARGB:
        if (mSaveAlpha)
        {
            mpWritePixel = &image_reader::WritePixel16Bit_4444_ARGB_Alpha;
        }
        else
        {
            mpWritePixel = &image_reader::WritePixel16Bit_4444_ARGB;
        }
        mpReadOutputPixel = &image_reader::ReadPixel16Bit_4444_ARGB;
        break;

    case GX_COLOR_FORMAT_4444BGRA:
        // FIXME: 4444 format is not done
        mpWritePixel = &image_reader::WritePixel16Bit_4444_BGRA;
        mpReadOutputPixel = &image_reader::ReadPixel16Bit_4444_BGRA;
        break;

    case GX_COLOR_FORMAT_24RGB:
        if (mSaveAlpha)
        {
            mOutputFormat = GX_COLOR_FORMAT_32ARGB;
            mpWritePixel = &image_reader::WritePixel32Bit_ARGB;
            mpReadOutputPixel = &image_reader::ReadPixel32Bit_ARGB;
        }
        else
        {
            mpWritePixel = &image_reader::WritePixel24Bit_RGBPacked;
            mpReadOutputPixel = &image_reader::ReadPixel24Bit_RGBPacked;
        }
        break;

    case GX_COLOR_FORMAT_24BGR:
        if (mSaveAlpha)
        {
            mOutputFormat = GX_COLOR_FORMAT_32BGRA;
            mpWritePixel = &image_reader::WritePixel32Bit_BGRA;
            mpReadOutputPixel = &image_reader::ReadPixel32Bit_BGRA;
        }
        else
        {
            mpWritePixel = &image_reader::WritePixel24Bit_BGRPacked;
            mpReadOutputPixel = &image_reader::ReadPixel24Bit_BGRPacked;
        }
        break;

    case GX_COLOR_FORMAT_24XRGB:
    case GX_COLOR_FORMAT_32ARGB:
        if ((mDisplayFormat == GX_COLOR_FORMAT_565RGB) && mSaveAlpha)
        {
            mpWritePixel = &image_reader::WritePixel32Bit_565_RGB_Alpha;
        }
        else
        {
            mpWritePixel = &image_reader::WritePixel32Bit_ARGB;
        }
        mpReadOutputPixel = &image_reader::ReadPixel32Bit_ARGB;

        break;

    case GX_COLOR_FORMAT_24BGRX:
    case GX_COLOR_FORMAT_32BGRA:
        mpWritePixel = &image_reader::WritePixel32Bit_BGRA;
        mpReadOutputPixel = &image_reader::ReadPixel32Bit_BGRA;
        break;

    default:
        // undefined format
        return FALSE;
    }
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
#if 0
void image_reader::SetCompression(BOOL bCompress)
{
    if (bCompress)
    {
        mMode |= IMAGE_READER_MODE_COMPRESS;
    }
    else
    {
        mMode &= ~IMAGE_READER_MODE_COMPRESS;
    }
}
#endif

///////////////////////////////////////////////////////////////////////////////
void image_reader::SetDither(BOOL bDither)
{
    if (bDither)
    {
        mMode |= IMAGE_READER_MODE_DITHER;
    }
    else
    {
        mMode &= ~IMAGE_READER_MODE_DITHER;
    }
}

///////////////////////////////////////////////////////////////////////////////
//void image_reader::SetDisplayFormat(UINT Format)
//{
//    mDisplayFormat = Format;
//}

///////////////////////////////////////////////////////////////////////////////
int image_reader::CalculateOutputStride(int width)
{
    int OutputStride = width;

    switch(mOutputFormat)
    {
    case GX_COLOR_FORMAT_MONOCHROME:
    case GX_COLOR_FORMAT_MONOCHROME_INVERTED:
        if (mInputHasAlpha)
        {
            // 1-bit transparency, 1-bit color
            width += 3;
            OutputStride = width / 4;
        }
        else
        {
            width += 7;
            OutputStride = width / 8;
        }
        break;

    case GX_COLOR_FORMAT_2BIT_GRAY:
    case GX_COLOR_FORMAT_2BIT_GRAY_INVERTED:
        width += 3;
        OutputStride = width / 4;
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
    case GX_COLOR_FORMAT_4BIT_GRAY_INVERTED:
    case GX_COLOR_FORMAT_4BIT_VGA:
        width++;
        OutputStride = width / 2;
        break;

    case GX_COLOR_FORMAT_8BIT_GRAY:
    case GX_COLOR_FORMAT_8BIT_PALETTE:
        break;

    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
        OutputStride = width;
        break;

    case GX_COLOR_FORMAT_5551BGRX:
    case GX_COLOR_FORMAT_1555XRGB:
    case GX_COLOR_FORMAT_565RGB:
    case GX_COLOR_FORMAT_565BGR:
    case GX_COLOR_FORMAT_4444ARGB:
    case GX_COLOR_FORMAT_4444BGRA:
        OutputStride = width * 2;
        break;

    case GX_COLOR_FORMAT_24RGB:
    case GX_COLOR_FORMAT_24BGR:
        OutputStride = width * 3;
        break;

    case GX_COLOR_FORMAT_24XRGB:
    case GX_COLOR_FORMAT_24BGRX:
    case GX_COLOR_FORMAT_32ARGB:
    case GX_COLOR_FORMAT_32BGRA:
        OutputStride = width * 4;
        break;
   
    default:
        OutputStride = width;  // default to 1-byte per pixel
        break;
    }
    return OutputStride;
}

///////////////////////////////////////////////////////////////////////////////
void image_reader::ConvertRGBToGray(GX_UBYTE red, GX_UBYTE green, GX_UBYTE blue, GX_UBYTE *gray_val)
{
    *gray_val = (GX_UBYTE)(((INT)red * 299 + (INT)green * 587 + (INT)blue * 114) / 1000);
}

///////////////////////////////////////////////////////////////////////////////
void image_reader::ConvertRGBToGray(GX_COLOR rgb_val, GX_UBYTE *gray_val)
{
    GX_UBYTE red = (GX_UBYTE)((rgb_val & 0xff0000) >> 16);
    GX_UBYTE green = (GX_UBYTE)((rgb_val & 0xff00) >> 8);
    GX_UBYTE blue = (GX_UBYTE)(rgb_val & 0xff);

    ConvertRGBToGray(red, green, blue, gray_val);
    
}

///////////////////////////////////////////////////////////////////////////////
int image_reader::GetNearestPaletteColor(Pixel want_color, GX_COLOR *palette, int palsize)
{
    ULONG nearest_dist = 0x7fffffff;
    ULONG dist;
    int red_dist;
    int green_dist;
    int blue_dist;

    int pal_index;
    int nearest_index = 0;

    if (palette)
    {
        for (pal_index = 0; pal_index < palsize; pal_index++)
        {
            if (pal_index != TRANSPARENT_COLOR)
            {
                Pixel pal_color(palette[pal_index]);

                red_dist = want_color.Red - pal_color.Red;
                green_dist = want_color.Green - pal_color.Green;
                blue_dist = want_color.Blue - pal_color.Blue;

                dist = (red_dist * red_dist) + (green_dist * green_dist) + (blue_dist * blue_dist);
                if (dist < nearest_dist)
                {
                    if (dist == 0)
                    {
                        return pal_index;
                    }
                    nearest_dist = dist;
                    nearest_index = pal_index;
                }
            }
        }
    }
    return nearest_index;
}

///////////////////////////////////////////////////////////////////////////////
// pixel data writers. One function for each supported output format 
///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixelMono_transparent(image_reader *reader, Pixel write)
{
    Pixel written;
    UCHAR *put = reader->mpPutData;
    GX_UBYTE gray;
    GX_UBYTE transMask;

    if (reader->mDoCompress)
    {
        /* [1-bit color][1-bit transparency] */
        /* Doing RLE here. */
        if (!reader->mSizeTesting)
        {
            if (write.Alpha < 128)
            {
                /* Set bits to 0 to mask transparent */
                *put &= 0xfc;
            }
            else
            {
                *put |= 0x01;
                ConvertRGBToGray(write.Red, write.Green, write.Blue, &gray);
                if (gray > reader->mGrayThreshod)
                {
                    *put |= 0x02;
                }
                else
                {
                    *put &= 0xfd;
                }
            }
        }
        reader->mpPutData++;
    }
    else
    {
        transMask = reader->mMonoMask >> 1;
        if (!reader->mSizeTesting)
        {
            if (write.Alpha < 128)
            {
                //transparency
                (*put) &= ~transMask;
                //Set bit to make sure the output will always be same.
                (*put) &= ~(reader->mMonoMask);
                written = write;
            }
            else
            {
                (*put) |= transMask;

                ConvertRGBToGray(write.Red, write.Green, write.Blue, &gray);

                if (gray > reader->mGrayThreshod)
                {
                    written.Red = written.Green = written.Blue = 255;
                    (*put) |= reader->mMonoMask;
                }
                else
                {
                    written.Red = written.Green = written.Blue = 0;
                    (*put) &= ~reader->mMonoMask;
                }
            }
        }
        reader->mMonoMask >>= 2;
        
        if (!reader->mMonoMask)
        {
            reader->mpPutData++;
            reader->mMonoMask = 0x80;
        }
    }

    return written;
}
///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixelMono(image_reader *reader, Pixel write)
{
    Pixel written;
    UCHAR *put = reader->mpPutData;
    GX_UBYTE gray;
    GX_UBYTE transMask = reader->mMonoMask >> 1;

    if (reader->mDoCompress)
    {
        /* Doing RLE here. */
        if (!reader->mSizeTesting)
        {
            ConvertRGBToGray(write.Red, write.Green, write.Blue, &gray);
            if (gray > reader->mGrayThreshod)
            {
                *put |= 0x01;
            }
            else
            {
                *put &= 0xfe;
            }
        }
        reader->mpPutData++;
    }
    else
    {
        if (!reader->mSizeTesting)
        {
            ConvertRGBToGray(write.Red, write.Green, write.Blue, &gray);
            if (gray > reader->mGrayThreshod)
            {
                written.Red = written.Green = written.Blue = 255;
                (*put) |= reader->mMonoMask;
            }
            else
            {
                written.Red = written.Green = written.Blue = 0;
                (*put) &= ~reader->mMonoMask;
            }
        }
        written.Alpha = 0xff;

        reader->mMonoMask >>= 1;

        if (!reader->mMonoMask)
        {
            reader->mpPutData++;
            reader->mMonoMask = 0x80;
        }
    }

    return written;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel4bppgrayscale_transparent(image_reader *reader, Pixel write)
{
    Pixel written;
    UCHAR *put = reader->mpPutData;
    UCHAR *putaux = reader->mpPutAux;
    GX_UBYTE gray;

    if (!reader->mSizeTesting)
    {
        ConvertRGBToGray(write.Red, write.Green, write.Blue, &gray);
        gray >>= 4;
        written.Red = written.Green = written.Blue = gray | (gray << 4);

        if (reader->mDoCompress)
        {
            if (write.Alpha < 128)
            {
                *put = TRANSPARENT_COLOR;
            }
            else
            {
                *put = gray;
            }
        }
        else
        {
            *put &= (GX_UBYTE)(~reader->mNibbleMask);
            if (reader->mNibbleMask & 0xf0)
            {
                *put = gray << 4;
            }
            else
            {
                *put |= gray;
            }

            if (reader->mMonoMask == 0x80)
            {
                /* Set bits to 0 in case that pixelmap data will be initialized differetly for different enviroment. */
                *putaux = 0;
            }

            if (write.Alpha > 128)
            {
                *putaux &= (GX_UBYTE)(~reader->mMonoMask);
            }
            else
            {
                *putaux |= reader->mMonoMask;
            }
        }
    }


    if (reader->mDoCompress)
    {
        reader->mpPutData++;
    }
    else
    {
        reader->mNibbleMask >>= 4;
        if (!reader->mNibbleMask)
        {
            reader->mpPutData++;
            reader->mNibbleMask = 0xf0;
        }

        reader->mMonoMask >>= 1;
        if (reader->mMonoMask == 0)
        {
            reader->mMonoMask = 0x80;
            reader->mpPutAux++;
        }
    }

    return written;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel4bppgrayscale(image_reader *reader, Pixel write)
{
    Pixel written;
    UCHAR *put = reader->mpPutData;
    GX_UBYTE gray;

    if (!reader->mSizeTesting)
    {
        ConvertRGBToGray(write.Red , write.Green, write.Blue, &gray);
        gray >>= 4;
        written.Red = written.Green = written.Blue = gray | (gray << 4);

        *put &= (GX_UBYTE)(~reader->mNibbleMask);
        if (reader->mNibbleMask & 0xf0)
        {
            *put = gray << 4;
        }
        else
        {
            *put |= gray;
        }
    }

    reader->mNibbleMask >>= 4;
    if (!reader->mNibbleMask)
    {
        reader->mpPutData++;
        reader->mNibbleMask = 0xf0;
    }

    return written;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel8bppPalette(image_reader *reader, Pixel write)
{
    Pixel written;
    int PalIndex;
    UCHAR *PutIndex = reader->mpPutData;
    reader->mpPutData++;;

    if (!reader->mSizeTesting)
    {
        GX_COLOR *palette = reader->GetPalette();

        if (write.Alpha < 128)
        {
            *PutIndex = TRANSPARENT_COLOR;
            written = write;
        }
        else
        {
            if (palette)
            {
                PalIndex = GetNearestPaletteColor(write, palette, reader->GetPalSize());

                *PutIndex = PalIndex;
                Pixel PalColor(palette[PalIndex]);
                written = PalColor;
            }
            else
            {
                *PutIndex = 0;
                written.Red = written.Green = written.Blue = written.Alpha = 0;
            }
        }
    }
    return written;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel8bppAlpha(image_reader *reader, Pixel write)
{
    UCHAR *pData = reader->mpPutData++;
    if (!reader->mSizeTesting)
    {
        *pData = (UCHAR) write.Alpha;
    }
    return write;
}
///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel8Bit_332_RGB(image_reader *reader, Pixel write)
{
GX_UBYTE HiColor;
GX_UBYTE *pPutHColor;
Pixel written;

    pPutHColor = reader->mpPutData;
    reader->mpPutData ++;
    written = write;
    written.Red &= 0xe0;
    written.Green &= 0xe0;
    written.Blue &= 0xc0;

    if (!reader->mSizeTesting)
    {
        HiColor = written.Red;
        HiColor |= (written.Green >> 3);
        HiColor |= written.Blue >> 6;
        *pPutHColor = HiColor;
    }
    return written;
}
///////////////////////////////////////////////////////////////////////////////
//haven't support now.

Pixel image_reader::WritePixel8Bit_332_Alpha(image_reader *reader, Pixel write)
{
GX_UBYTE    HiColor;
GX_UBYTE   *pPutHColor;
USHORT     *pPut_A_Hcolor;
Pixel       written;

    written = write;
    written.Red &= 0xe0;
    written.Green &= 0xe0;
    written.Blue &= 0xc0;


    if (!reader->mSizeTesting)
    {

        HiColor = written.Red;
        HiColor |= (written.Green >> 3);
        HiColor |= written.Blue >> 6;

        if (reader->mpPutAux)
        {
            pPutHColor = (GX_UBYTE *)reader->mpPutData;
            reader->mpPutData ++;
            *pPutHColor = HiColor;

            // save alpha in seperate array
            *reader->mpPutAux = write.Alpha;
            reader->mpPutAux++;
        }
        else
        {
            pPut_A_Hcolor = (USHORT *)reader->mpPutData;
            *pPut_A_Hcolor = ((USHORT)written.Alpha << 8 | HiColor);

            reader->mpPutData += 2;
        }
    }
    else
    {
        reader->mpPutData += 2;
    }
    return written;
}


///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel16Bit_555(image_reader *reader, Pixel write)
{
    USHORT HiColor;
    USHORT *pPutHColor;
    Pixel written;

    pPutHColor = (USHORT *) reader->mpPutData;
    reader->mpPutData += 2;
    written = write;
    written.Red &= 0xf8;
    written.Green &= 0xf8;
    written.Blue &= 0xf8;

    if (!reader->mSizeTesting)
    {
        HiColor =  (USHORT) written.Red;
        HiColor <<= 7;
        HiColor |= ((USHORT) written.Green << 2);
        HiColor |= (written.Blue  >> 3);
        *pPutHColor = HiColor;
    }
    return written;
}    

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel16Bit_555_BGR(image_reader *reader, Pixel write)
{
    USHORT HiColor;
    USHORT *pPutHColor;
    Pixel written;

    pPutHColor = (USHORT *) reader->mpPutData;
    reader->mpPutData += 2;
    written = write;
    written.Red &= 0xf8;
    written.Green &= 0xf8;
    written.Blue &= 0xf8;

    if (!reader->mSizeTesting)
    {
        HiColor =  (USHORT) written.Blue;
        HiColor <<= 7;
        HiColor |= ((USHORT) written.Green << 2);
        HiColor |= written.Red  >> 3;
        *pPutHColor = HiColor;
    }
    return written;
}    


///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel16Bit_555_Alpha(image_reader *reader, Pixel write)
{
    USHORT HiColor;
    USHORT *pPutHColor;
    Pixel written;

    written = write;
    written.Red &= 0xf8;
    written.Green &= 0xf8;
    written.Blue &= 0xf8;

    if (!reader->mSizeTesting)
    {
        HiColor =  (USHORT) written.Red;
        HiColor <<= 7;
        HiColor |= ((USHORT) written.Green << 2);
        HiColor |= (written.Blue  >> 3);

        // if we have a seperate alpha channel, then it means we are not
        // writing RLE counts, so the alpha goes into it's own array
        if (reader->mpPutAux)
        {
            pPutHColor = (USHORT *)reader->mpPutData;
            reader->mpPutData += 2;
            *pPutHColor = HiColor;

            // save alpha in seperate array
            *reader->mpPutAux = write.Alpha;
            reader->mpPutAux++;
        }
        else
        {
            // here if the alpha channel is NULL. The count, alpha, and pixel value
            // all go into the same data word.
            UCHAR *pPutAlpha = reader->mpPutData;
            pPutHColor = (USHORT *)reader->mpPutData;
            pPutAlpha++;    // skip the count byte
            pPutHColor++;   // skip to lower half-word

            *pPutAlpha = write.Alpha;
            *pPutHColor = HiColor;

            reader->mpPutData += 4;        // four bytes total for count, alpha, and pixel value
        }
    }
    else
    {
        reader->mpPutData += 4;        // four bytes total for count, alpha, and pixel value
    }

    return written;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel16Bit_565_RGB(image_reader *reader, Pixel write)
{
    USHORT HiColor;
    USHORT *pPutHColor;
    Pixel written;

    pPutHColor = (USHORT *) reader->mpPutData;
    reader->mpPutData += 2;
    written = write;
    written.Red &= 0xf8;
    written.Green &= 0xfc;
    written.Blue &= 0xf8;

    if (!reader->mSizeTesting)
    {
        HiColor =  (USHORT) written.Red;
        HiColor <<= 8;
        HiColor |= ((USHORT) written.Green << 3);
        HiColor |= written.Blue  >> 3;
        *pPutHColor = HiColor;
    }
    return written;
}


///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel16Bit_565_Alpha(image_reader *reader, Pixel write)
{
    USHORT HiColor;
    USHORT *pPutHColor;
    Pixel written;

    written = write;
    written.Red &= 0xf8;
    written.Green &= 0xfc;
    written.Blue &= 0xf8;

    if (!reader->mSizeTesting)
    {
        HiColor =  (USHORT) written.Red;
        HiColor <<= 8;
        HiColor |= ((USHORT) written.Green << 3);
        HiColor |= written.Blue  >> 3;

        // if we have a seperate alpha channel, then it means we are not
        // writing RLE counts, so the alpha goes into it's own array
        if (reader->mpPutAux)
        {
            pPutHColor = (USHORT *) reader->mpPutData;
            reader->mpPutData += 2;
            *pPutHColor = HiColor;

            // save alpha in seperate array
            *reader->mpPutAux = write.Alpha;
            reader->mpPutAux++;
        }
        else
        {
            // here if the alpha channel is NULL. The count, alpha, and pixel value
            // all go into the same data word.
            UCHAR *pPutAlpha = reader->mpPutData;
            pPutHColor = (USHORT *) reader->mpPutData;
            pPutAlpha++;    // skip the count byte
            pPutHColor++;   // skip to lower half-word

            *pPutAlpha = write.Alpha;
            *pPutHColor = HiColor;
        
            reader->mpPutData += 4;        // four bytes total for count, alpha, and pixel value
        }
    }
    else
    {
        reader->mpPutData += 4;        // four bytes total for count, alpha, and pixel value
    }
    return written;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel16Bit_565_BGR(image_reader *reader, Pixel write)
{
    USHORT HiColor;
    USHORT *pPutHColor;
    Pixel written;

    pPutHColor = (USHORT *) reader->mpPutData;
    reader->mpPutData += 2;
    written = write;
    written.Red &= 0xf8;
    written.Green &= 0xfc;
    written.Blue &= 0xf8;
    if (!reader->mSizeTesting)
    {
        HiColor =  (USHORT) written.Blue;
        HiColor <<= 8;
        HiColor |= ((USHORT) written.Green << 3);
        HiColor |= written.Red  >> 3;
        *pPutHColor = HiColor;
    }
    return written;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel16Bit_4444_ARGB_Alpha(image_reader *reader, Pixel write)
{
    USHORT HiColor;
    USHORT *pPutHColor;
    Pixel written;

    pPutHColor = (USHORT *)reader->mpPutData;
    reader->mpPutData += 2;
    written = write;
    written.Red &= 0xf0;
    written.Green &= 0xf0;
    written.Blue &= 0xf0;
    written.Alpha &= 0xf0;

    if (!reader->mSizeTesting)
    {
        HiColor = ((USHORT)written.Blue >> 4);
        HiColor |= ((USHORT)written.Green);
        HiColor |= ((USHORT)written.Red << 4);
        HiColor |= ((USHORT)written.Alpha << 8);
        *pPutHColor = HiColor;
    }

    return written;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel16Bit_4444_ARGB(image_reader *reader, Pixel write)
{
    USHORT HiColor;
    USHORT *pPutHColor;
    Pixel written;

    pPutHColor = (USHORT *)reader->mpPutData;
    reader->mpPutData += 2;
    written = write;
    written.Red &= 0xf0;
    written.Green &= 0xf0;
    written.Blue &= 0xf0;
    written.Alpha &= 0xf0;

    if (!reader->mSizeTesting)
    {
        HiColor = ((USHORT)written.Blue >> 4);
        HiColor |= ((USHORT)written.Green);
        HiColor |= ((USHORT)written.Red << 4);
        HiColor |= 0xf000;
        *pPutHColor = HiColor;
    }

    return written;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel16Bit_4444_BGRA(image_reader *reader, Pixel write)
{
    USHORT HiColor;
    USHORT *pPutHColor;
    Pixel written;

    pPutHColor = (USHORT *) reader->mpPutData;
    reader->mpPutData += 2;
    written = write;
    written.Red &= 0xf0;
    written.Green &= 0xf0;
    written.Blue &= 0xf0;
    written.Alpha &= 0xf0;

    if (!reader->mSizeTesting)
    {
        HiColor = (USHORT)(written.Alpha >> 4);
        HiColor |= ((USHORT)written.Red);
        HiColor |= ((USHORT)written.Green << 4);
        HiColor |= ((USHORT)written.Blue << 8);
        *pPutHColor = HiColor;
    }
    return written;
}

///////////////////////////////////////////////////////////////////////////////
// KGM not used, we do bgr order when writing resources
Pixel image_reader::WritePixel16Bit_565_BGR_Alpha(image_reader *reader, Pixel write)
{
    USHORT HiColor;
    USHORT *pPutHColor;
    Pixel written;

    written = write;
    written.Red &= 0xf8;
    written.Green &= 0xfc;
    written.Blue &= 0xf8;

    if (!reader->mSizeTesting)
    {
        HiColor =  (USHORT) written.Blue;
        HiColor <<= 8;
        HiColor |= ((USHORT) written.Green << 3);
        HiColor |= written.Red  >> 3;

        // if we have a seperate alpha channel, then it means we are not
        // writing RLE counts, so the alpha goes into it's own array
        if (reader->mpPutAux)
        {
            pPutHColor = (USHORT *) reader->mpPutData;
            reader->mpPutData += 2;
            *pPutHColor = HiColor;

            // save alpha in seperate array
            *reader->mpPutAux = write.Alpha;
            reader->mpPutAux += 1;
        }
        else
        {
            // here if the alpha channel is NULL. The count, alpha, and pixel value
            // all go into the same data word.
            UCHAR *pPutAlpha = reader->mpPutData;
            pPutHColor = (USHORT *) reader->mpPutData;
            pPutAlpha++;    // skip the count byte
            pPutHColor++;   // skip to lower half-word

            *pPutAlpha = write.Alpha;
            *pPutHColor = HiColor;
        
            reader->mpPutData += 4;        // four bytes total for count, alpha, and pixel value
        }
    }
    else
    {
        reader->mpPutData += 4;        // four bytes total for count, alpha, and pixel value
    }

    return written;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel24Bit_RGBPacked(image_reader *reader, Pixel write)
{
    if (!reader->mSizeTesting)
    {
        UCHAR *pPut = reader->mpPutData;

        *pPut++ = write.Red;
        *pPut++ = write.Green;
        *pPut++ = write.Blue;
    }
    reader->mpPutData += 3;
    return write;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel24Bit_BGRPacked(image_reader *reader, Pixel write)
{
    if (!reader->mSizeTesting)
    {
        UCHAR *pPut = reader->mpPutData;

        *pPut++ =  write.Blue;
        *pPut++ =  write.Green;
        *pPut++ =  write.Red;
    }
    reader->mpPutData += 3;
    return write;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel32Bit_BGRA(image_reader *reader, Pixel write)
{
    if (!reader->mSizeTesting)
    {
        UCHAR *pPut = reader->mpPutData;

        *pPut++ =  write.Alpha;
        *pPut++ =  write.Red;
        *pPut++ =  write.Green;
        *pPut = write.Blue;
    }
    reader->mpPutData += 4;
    return write;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel32Bit_ARGB(image_reader *reader, Pixel write)
{
    if (!reader->mSizeTesting)
    {
        UCHAR *pPut = reader->mpPutData;
        *pPut++ =  write.Blue;
        *pPut++ =  write.Green;
        *pPut++ =  write.Red;
        *pPut = write.Alpha;
    }
    reader->mpPutData += 4;
    return write;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::WritePixel32Bit_565_RGB_Alpha(image_reader *reader, Pixel pPixel)
{
    Pixel written;

    written = pPixel;
    written.Red &= 0xf8;
    written.Green &= 0xfc;
    written.Blue &= 0xf8;

    if (!reader->mSizeTesting)
    {
        UCHAR *pPut = reader->mpPutData;
        *pPut++ = written.Blue;
        *pPut++ = written.Green;
        *pPut++ = written.Red;
        *pPut = written.Alpha;
    }
    reader->mpPutData += 4;

    return written;
}

///////////////////////////////////////////////////////////////////////////////
// Pixel data readers. Read specified format data back into Pixel.  
///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixelMono(image_reader *reader, int PixIndex)
{
    Pixel val;
    const GX_UBYTE *pLine = reader->mpGetData;
    GX_UBYTE color;

    color = *(pLine + (PixIndex >> 3));

    if (color & (0x80 >> (PixIndex & 0x07)))
    {
        val.Red = val.Green = val.Blue = 0xff;
    }
    else
    {
         val.Red = val.Green = val.Blue = 0;
    }
    val.Alpha = 0xff;

    return val;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixelMonoTransparent(image_reader *reader, int PixIndex)
{
    Pixel val;
    const GX_UBYTE *pLine = reader->mpGetData;
    GX_UBYTE color;

    color = *(pLine + (PixIndex >> 2));

    /* [1-bit color][1-bit transparency] */
    if (color & (0x40 >> ((PixIndex & 0x03) << 1)))
    {
        if (color & (0x80 >> ((PixIndex & 0x03) << 1)))
        {
            val.Red = val.Green = val.Blue = 0xff;
        }
        else
        {
            val.Red = val.Green = val.Blue = 0;
        }

        val.Alpha = 0xff;
    }
    else
    {
        val.Alpha = val.Red = val.Green = val.Blue = 0;
    }

    return val;

}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixel4bppgrayscale(image_reader *reader, int PixIndex)
{
    Pixel val;
    const UCHAR *pLine = reader->mpGetData;
    GX_UBYTE color;

    color = *(pLine + (PixIndex >> 1));
    if (PixIndex & 1)
    {
        color &= 0x0f;
    }
    else
    {
        color &= 0xf0;
        color >>= 4;
    }

    color |= color << 4;
    val.Red = val.Green = val.Blue = color;
    val.Alpha = 0xff;

    return val;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixel4bppgrayscale_transparent(image_reader *reader, int PixIndex)
{
    Pixel val;
    const UCHAR *pLine = reader->mpGetData;
    const UCHAR *pLineAux = reader->mpGetAux;
    INT AuxIndex = PixIndex >> 3;
    INT ColorIndex = PixIndex >> 1;
    GX_UBYTE color = *(pLine + ColorIndex);
    GX_UBYTE TransMask = *(pLineAux + AuxIndex);
    /* transparent mask is just 1 bit, and from left to right. So we need move it to the low */
    TransMask = (GX_UBYTE)(TransMask >> (7 - (PixIndex & 0x07)));

    if (TransMask & 1)
    {
        val.Alpha = val.Red = val.Green = val.Blue = 0;
    }
    else
    {
        if (PixIndex & 1)
        {
            color &= 0xf;
        }
        else
        {
            color &= 0xf0;
            color >>= 4;
        }
        color |= color << 4;
        val.Red = val.Green = val.Blue = color;
        val.Alpha = 0xff;
    }

    return val;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixel8bppPalette(image_reader *reader, int PixIndex)
{
    int pal_index;
    Pixel val;
    GX_COLOR *palette = reader->GetPalette();
    const UCHAR *pLine = reader->mpGetData;

    if (palette)
    {
        pLine += PixIndex;
        pal_index = *pLine;

        if (pal_index == TRANSPARENT_COLOR)
        {
            val.Alpha = val.Red = val.Green = val.Blue = 0;
        }
        else
        {
            if (pal_index < reader->GetPalSize())
            {
                ULONG color_val = palette[pal_index];
                val.Red =   (color_val >> 16) & 0xff;
                val.Green = (color_val >> 8) & 0xff;
                val.Blue =  color_val & 0xff;
                val.Alpha = 0xff;
            }
        }
    }
    return val;
}

Pixel image_reader::ReadPixel8bppAlpha(image_reader *reader, int PixIndex)
{
Pixel val;
    const UCHAR *pLine = reader->mpGetData;

    val.Red = 0;
    val.Green = 0;
    val.Blue = 0;
    val.Alpha = *(pLine + PixIndex);
    return val;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixel8Bit_332_RGB(image_reader *reader, int PixIndex)
{
    GX_UBYTE HiColor;
    GX_UBYTE *pGetColor;
    Pixel val;

    pGetColor = (GX_UBYTE *)reader->mpGetData;
    pGetColor += PixIndex;
    HiColor = *pGetColor;

    val.Red = (HiColor & 0xe0);
    val.Green = ((HiColor & 0x1c) << 3);
    val.Blue = (HiColor & 0x3) << 6;
    val.Alpha = 0xff;
    return val;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixel8Bit_332_Alpha(image_reader *reader, int PixIndex)
{
GX_UBYTE   HiColor;
GX_UBYTE  *pGetColor;
Pixel      val;
const GX_UBYTE *pLine = reader->mpGetData;

    pGetColor = (GX_UBYTE *)pLine;
    pGetColor += PixIndex;

    HiColor = *pGetColor++;
    val.Red = HiColor & 0xe0;
    val.Green = (HiColor & 0x1c) << 3;
    val.Blue = (HiColor & 0x03) << 6;

    const UCHAR *pAux = reader->mpGetAux;
    pAux += PixIndex;
    val.Alpha = *pAux;
    return val;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixel16Bit_555(image_reader *reader, int PixIndex)
{
    USHORT HiColor;
    USHORT *pGetColor;
    Pixel val;

    const UCHAR *pLine = reader->mpGetData;
    pGetColor = (USHORT *) pLine;
    pGetColor += PixIndex;
    HiColor = *pGetColor;

    val.Red = ((HiColor & 0x7c00) >> 7);
    val.Green = ((HiColor & 0x03e0) >> 2);
    val.Blue = ((HiColor & 0x1f) << 3);
    val.Alpha = 0xff;
    return val;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixel16Bit_555_BGR(image_reader *reader, int PixIndex)
{
    USHORT HiColor;
    USHORT *pGetColor;
    Pixel val;
    const UCHAR *pLine = reader->mpGetData;

    pGetColor = (USHORT *) pLine;
    pGetColor += PixIndex;
    HiColor = *pGetColor;

    val.Blue = ((HiColor & 0x7c00) >> 7);
    val.Green = ((HiColor & 0x03e0) >> 2);
    val.Red = (HiColor & 0x1f);
    val.Alpha = 0xff;
    return val;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixel16Bit_555_Alpha(image_reader *reader, int PixIndex)
{
    USHORT HiColor;
    USHORT *pGetColor;
    Pixel val;
    const UCHAR *pLine = reader->mpGetData;

    pGetColor = (USHORT *) pLine;
    pGetColor += PixIndex;
    HiColor = *pGetColor;

    val.Red = ((HiColor & 0x7c00) >> 7);
    val.Green = ((HiColor & 0x03e0) >> 2);
    val.Blue = ((HiColor & 0x1f) << 3);

    const UCHAR *pAux = reader->mpGetAux;
    pAux += PixIndex;
    val.Alpha = *pAux;
    return val;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixel16Bit_565_RGB(image_reader *reader, int PixIndex)
{
    USHORT HiColor;
    USHORT *pGetColor;
    Pixel val;

    pGetColor = (USHORT *) reader->mpGetData;
    pGetColor += PixIndex;
    HiColor = *pGetColor;

    val.Red = ((HiColor & 0xf800) >> 8);
    val.Green = ((HiColor & 0x07e0) >> 3);
    val.Blue = (HiColor & 0x1f) << 3;
    val.Alpha = 0xff;
    return val;
}


///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixel16Bit_565_Alpha(image_reader *reader, int PixIndex)
{
    USHORT HiColor;
    USHORT *pGetColor;
    Pixel val;
    const GX_UBYTE *pLine = reader->mpGetData;

    pGetColor = (USHORT *) pLine;
    pGetColor += PixIndex;
    HiColor = *pGetColor;

    val.Red = ((HiColor & 0xf800) >> 8);
    val.Green = ((HiColor & 0x07e0) >> 3);
    val.Blue = (HiColor & 0x1f) << 3;

    const UCHAR *pAux  = reader->mpGetAux;
    pAux += PixIndex;
    val.Alpha = *pAux;
    return val;
}


///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixel16Bit_565_BGR(image_reader *reader, int PixIndex)
{
    USHORT HiColor;
    USHORT *pGetColor;
    Pixel val;
    const UCHAR *pLine = reader->mpGetData;

    pGetColor = (USHORT *) pLine;
    pGetColor += PixIndex;
    HiColor = *pGetColor;

    val.Blue = ((HiColor & 0xf800) >> 8);
    val.Green = ((HiColor & 0x07e0) >> 3);
    val.Red = (HiColor & 0x1f) << 3;
    val.Alpha = 0xff;
    return val;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixel16Bit_565_BGR_Alpha(image_reader *reader, int PixIndex)
{
    USHORT HiColor;
    USHORT *pGetColor;
    Pixel val;
    const GX_UBYTE *pLine = reader->mpGetData;

    pGetColor = (USHORT *) pLine;
    pGetColor += PixIndex;
    HiColor = *pGetColor;

    val.Blue = ((HiColor & 0xf800) >> 8);
    val.Green = ((HiColor & 0x07e0) >> 3);
    val.Red = (HiColor & 0x1f) << 3;

    const UCHAR *pAux = reader->mpGetAux;
    pAux += PixIndex;
    val.Alpha = *pAux;
    return val;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixel16Bit_4444_ARGB(image_reader *reader, int PixIndex)
{
    USHORT HiColor;
    USHORT *pGetColor;
    Pixel val;
    const UCHAR *pLine = reader->mpGetData;

    pGetColor = (USHORT *)pLine;
    pGetColor += PixIndex;
    HiColor = *pGetColor;

    val.Alpha = ((HiColor & 0xf000) >> 8);
    val.Red = ((HiColor & 0xf00) >> 4);
    val.Green = (HiColor & 0xf0);
    val.Blue = (HiColor & 0xf) << 4;

    return val;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixel16Bit_4444_BGRA(image_reader *reader, int PixIndex)
{
    USHORT HiColor;
    USHORT *pGetColor;
    Pixel val;
    const UCHAR *pLine = reader->mpGetData;

    pGetColor = (USHORT *)pLine;
    pGetColor += PixIndex;
    HiColor = *pGetColor;

    val.Blue = ((HiColor & 0xf000) >> 8);
    val.Green = ((HiColor & 0xf00) >> 4);
    val.Red = (HiColor & 0xf0);
    val.Alpha = (HiColor & 0xf) << 4;

    return val;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixel24Bit_BGRPacked(image_reader *reader, int PixIndex)
{
    Pixel val;
    const UCHAR *pLine = reader->mpGetData;

    pLine += PixIndex * 3;
    val.Blue = *pLine++;
    val.Green = *pLine++;
    val.Red = *pLine++;
    val.Alpha = 0xff;
    return val;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixel24Bit_RGBPacked(image_reader *reader, int PixIndex)
{
    Pixel val;
    const UCHAR *pLine = reader->mpGetData;

    pLine += PixIndex * 3;
    val.Red = *pLine++;
    val.Green = *pLine++;
    val.Blue = *pLine++;
    val.Alpha = 0xff;
    return val;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixel32Bit_ARGB(image_reader *reader, int PixIndex)
{
    Pixel val;
    const UCHAR *pLine = reader->mpGetData;

    pLine += PixIndex * 4;
    val.Blue = *pLine++;
    val.Green = *pLine++;
    val.Red = *pLine++;
    val.Alpha = *pLine;
    return val;
}

///////////////////////////////////////////////////////////////////////////////
Pixel image_reader::ReadPixel32Bit_BGRA(image_reader *reader,  int PixIndex)
{
    Pixel val;
    const UCHAR *pLine = reader->mpGetData;

    pLine += PixIndex * 4;
    val.Alpha = *pLine++;
    val.Red = *pLine++;
    val.Green = *pLine++;
    val.Blue = *pLine;
    return val;
}

///////////////////////////////////////////////////////////////////////////////
// Transparent data writers. Not being used currently 
///////////////////////////////////////////////////////////////////////////////
void image_reader::WriteTransparentPixel_16(UCHAR *pPut, int PixIndex)
{
    USHORT *pWrite = (USHORT *) pPut;
    pWrite += PixIndex;
    *pWrite = 1;
}

///////////////////////////////////////////////////////////////////////////////
void image_reader::WriteTransparentPixel_24_BGR(UCHAR *pPut, int PixIndex)
{
    pPut += PixIndex * 3;
    *pPut++ = 1;
    *pPut++ = 0;
    *pPut++ = 0;
}

///////////////////////////////////////////////////////////////////////////////
void image_reader::WriteTransparentPixel_24_RGB(UCHAR *pPut, int PixIndex)
{
    pPut += PixIndex * 3;
    *pPut++ = 0;
    *pPut++ = 0;
    *pPut++ = 1;
}

///////////////////////////////////////////////////////////////////////////////
void image_reader::WriteTransparentPixel_32BGRA(UCHAR *pPut, int PixIndex)
{
    ULONG *pData = (ULONG *) pPut; 
    pData += PixIndex;
    *pData = 0x0;
}

///////////////////////////////////////////////////////////////////////////////
void image_reader::WriteTransparentPixel_32ARGB(UCHAR *pPut, int PixIndex)
{
    ULONG *pData = (ULONG *) pPut; 
    pData += PixIndex;
    *pData = 0x0;
}



