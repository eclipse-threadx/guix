#ifndef _IMAGE_READER_H_
#define _IMAGE_READER_H_

#include "png.h"

extern "C" {
#include "jpeglib.h"
#include "jerror.h"
#include "jmorecfg.h"
}


// mode flags kept in internal variable mMode
#define IMAGE_READER_MODE_DITHER        0x01

#define DITHER_SPECKLE_LIMIT 400
#define FS_SCALE 1024

enum IMAGE_TYPES {
    IMAGE_TYPE_UNKNOWN = 0,
    IMAGE_TYPE_PNG,
    IMAGE_TYPE_JPG,
    IMAGE_TYPE_GIF,
    IMAGE_TYPE_BMP,
    // KGM: Add support for tif, others?
};

class Pixel  {
    public:
        Pixel(void)
        {
            Red = Green = Blue = Alpha = 0;
        }

        Pixel(int Red, int Green, int Blue);
        Pixel(GX_COLOR color);

        BOOL operator == (const Pixel &Pix) const;
        BOOL operator != (const Pixel &Pix) const;

        int Red;
        int Green;
        int Blue;
        int Alpha;
};

typedef struct  {
    int Width;
    int Height;
    int BitsPerPix;
} image_info;


class image_reader
{
    public:
        image_reader();
        virtual ~image_reader();

        void DestroyImage();
        virtual BOOL CheckImageHasAlphaChannel(CString &path) {return FALSE;}
        virtual BOOL ReadImage(CString &path, int frame_id = -1) = 0;
        virtual BOOL ReadImage(unsigned char *data, int data_size) = 0;

        BOOL FinalizeImage(int width, int height);
        BOOL RleEncode(GX_PIXELMAP *pMap, BOOL targa_format = FALSE);

        GX_PIXELMAP *ResizeImage(GX_PIXELMAP *src, int dest_width, int dest_height);

        GX_PIXELMAP *GetPixelmap(int frame_id = 0);
        virtual INT GetDelayTime(int frame_id = 0) = 0;
        static INT GetFrameCount(CString& path);

        GX_COLOR *GetPalette() {return mpPalette;}
        int       GetPalSize() {return mPalSize;}

        static int GetImageType(CString &path);
        static int GetImageType(unsigned char *data, int data_size);
        static image_reader *CreateProperReader(int image_type);
        static image_reader *CreateProperReader(CString &path);
        static image_reader *CreateProperReader(unsigned char *data, int data_size);

        void SetSaveAlphaVal(BOOL Save);
        void DoCompress(BOOL docompress);
        void SetDither(BOOL bDither);
        BOOL SetOutputColorFormat(int output_format, int display_color_format);
        void SetPalette(GX_COLOR *pal, int size, int fixed);
        INT GetDisplayFormat() { return mDisplayFormat; }

        //void SetDisplayFormat(UINT Format);
        static int GetNearestPaletteColor(Pixel want_color, GX_COLOR *palette, int palsize);
        static void ConvertRGBToGray(GX_COLOR rgb_val, GX_UBYTE *gray_val);
        static void ConvertRGBToGray(GX_UBYTE red, GX_UBYTE green, GX_UBYTE blue, GX_UBYTE *gray_val);

        const GX_UBYTE *mpGetData;

    protected:

        virtual UCHAR *GetInputDataPtr(int row) = 0;

        void ComputeGrayThreshod(int width, int height);
        void ColorSpaceConvert(int width, int height, GX_PIXELMAP *map);
        void ColorSpaceConvertRow(int Width);

        int CountDuplicates(int Index, int Width);
        USHORT WriteRleCount(USHORT count, UCHAR *pPutCount, BOOL bRaw);
        GX_UBYTE *GetCountLocation();
        void RleEncodeRow(int Width);

        BOOL InitDither(int cols);
        void DitherOneRow(int Width);
        void DitherCleanup(void);

        void SetDefaultPixelReader(int input_bits_per_pix);
        int CalculateOutputStride(int ImgWidth);
    
        // Raw Data readers
        // This is a group of functions that read pixel data from the input
        // stream. Most of these input-data reader functions are provided
        // by the format-specific sub-class, but we provide the most simple
        // and common versions as part of the base class so that they can be
        // re-used by multiple decoders

        static void ReadPixel16(image_reader *reader, int Index, Pixel *pPut);
        static void ReadPixel24(image_reader *reader, int Index, Pixel *pPut);
        static void ReadPixel32(image_reader *reader, int Index, Pixel *pPut);

        // Pixel data writers- 
        // A group of functions for writing Pixelmap output data in the requested
        // format. Only supporting 16, 24, and 32 bit formats at first.
        // FIXME: Need to add 1bpp to 8bpp formats.

        static Pixel WritePixelMono(image_reader *reader, Pixel pPixel);
        static Pixel WritePixelMono_transparent(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel4bppgrayscale_transparent(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel4bppgrayscale(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel8bppPalette(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel8bppAlpha(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel8Bit_332_RGB(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel8Bit_332_Alpha(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel16Bit_555(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel16Bit_555_BGR(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel16Bit_555_Alpha(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel16Bit_565_Alpha(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel16Bit_565_BGR(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel16Bit_565_BGR_Alpha(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel16Bit_565_RGB(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel16Bit_4444_ARGB(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel16Bit_4444_ARGB_Alpha(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel16Bit_4444_BGRA(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel24Bit_BGRPacked(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel24Bit_RGBPacked(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel32Bit_ARGB(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel32Bit_565_RGB_Alpha(image_reader *reader, Pixel pPixel);
        static Pixel WritePixel32Bit_BGRA(image_reader *reader, Pixel pPixel);

 
        // Pixel data readers- 
        // A group of functions for reading Pixelmap output data back into
        // a Pixel structure. Only supporting 16, 24, and 32 bit formats at first.
        // FIXME: Need to add 1bpp to 8bpp formats.

        static Pixel ReadPixelMono(image_reader *reader, int PixIndex);
        static Pixel ReadPixelMonoTransparent(image_reader *reader, int PixIndex);
        static Pixel ReadPixel4bppgrayscale(image_reader *reader, int PixIndex);
        static Pixel ReadPixel4bppgrayscale_transparent(image_reader *reader, int PixIndex);
        static Pixel ReadPixel8bppPalette(image_reader *reader, int PixIndex);
        static Pixel ReadPixel8bppAlpha(image_reader *reader, int PixIndex);
        static Pixel ReadPixel8Bit_332_RGB(image_reader *reader, int PixIndex);
        static Pixel ReadPixel8Bit_332_Alpha(image_reader *reader, int PixIndex);
        static Pixel ReadPixel16Bit_555(image_reader *reader, int PixIndex);
        static Pixel ReadPixel16Bit_555_BGR(image_reader *reader, int PixIndex);
        static Pixel ReadPixel16Bit_555_Alpha(image_reader *reader, int PixIndex);
        static Pixel ReadPixel16Bit_565_Alpha(image_reader *reader, int PixIndex);
        static Pixel ReadPixel16Bit_565_BGR(image_reader *reader, int PixIndex);
        static Pixel ReadPixel16Bit_565_BGR_Alpha(image_reader *reader, int PixIndex);
        static Pixel ReadPixel16Bit_565_RGB(image_reader *reader, int PixIndex);
        static Pixel ReadPixel16Bit_4444_ARGB(image_reader *reader, int PixIndex);
        static Pixel ReadPixel16Bit_4444_BGRA(image_reader *reader, int PixIndex);
        static Pixel ReadPixel24Bit_BGRPacked(image_reader *reader, int PixIndex);
        static Pixel ReadPixel24Bit_RGBPacked(image_reader *reader, int PixIndex);
        static Pixel ReadPixel32Bit_ARGB(image_reader *reader, int PixIndex);
        static Pixel ReadPixel32Bit_BGRA(image_reader *reader, int PixIndex);

        // Functions for writing transparent (not alpha) values in each format
        static void WriteTransparentPixel_16(UCHAR *pPut, int PixIndex);
        static void WriteTransparentPixel_24_BGR(UCHAR *pPut, int PixIndex);
        static void WriteTransparentPixel_24_RGB(UCHAR *pPut, int PixIndex);
        static void WriteTransparentPixel_32BGRA(UCHAR *pPut, int PixIndex);
        static void WriteTransparentPixel_32ARGB(UCHAR *pPut, int PixIndex);

        void BitmapStretch(int src_width, int src_height,
            int dest_width, int dest_height, GX_PIXELMAP *src, GX_PIXELMAP *dest);

        void BitmapStretchStep(int dest_width,
            int source_width, int yCurrent, int yEnd,
            GX_PIXELMAP *src, GX_PIXELMAP *dest);

        // pointer to function to read data from incoming source
        void (*mpReadInputPixel)(image_reader *reader, int index, Pixel *put);

        // pointer to function to read our output data format, used
        // by the RLE compression routines.
        Pixel (*mpReadOutputPixel)(image_reader *reader, int);

        // pointer to function to write out output data format
        // 
        // The function must increment the data and aux ptr appropriately,
        // and return the actual value written (used for dithering error)

        Pixel (*mpWritePixel)(image_reader *, Pixel);

        // pointer to function to write transparent value
        void (*mpWriteTransparentPix)(UCHAR *, int);

    protected:
        BOOL mInputHasAlpha;    // does source have alpha channel?


    private:
        CArray<GX_PIXELMAP *> mPixelmapList;
        FILE *mFile;
        int *mpNextBlueErr;
        int *mpNextGreenErr;
        int *mpNextRedErr;
        int *mpThisBlueErr;
        int *mpThisGreenErr;
        int *mpThisRedErr;

        int mDataSize;
        int mOutputFormat;
        int mMode;

        GX_COLOR *mpPalette;
        int mPalSize;
        int mPalFixed;

        BOOL mWantAlpha;        // does caller want to keep it?
        BOOL mSaveAlpha;        // save or not save alpha in dest

        BOOL mDithering;
        BOOL mDitherDirection;
        BOOL mTargaFormat;
        UCHAR   mImageType;
        UINT    mDisplayFormat;

        GX_UBYTE *mpPutData;
        GX_UBYTE *mpPutAux;
        const GX_UBYTE *mpGetAux;
        BOOL  mSizeTesting;
        INT   mGrayThreshod;
        GX_UBYTE mMonoMask;
        GX_UBYTE mNibbleMask;
        BOOL mDoCompress;
};

// Define input_stream class for reading specified number of bytes from a memory buffer.
class input_stream: public IStream
{
public:
    input_stream(unsigned char *data, int data_size);
    ~input_stream() {};

    // IUnknown Methods
    IFACEMETHODIMP_(ULONG) AddRef() { return 0; }
    IFACEMETHODIMP_(ULONG) Release() { return 0; }
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppInterface) { return 0; }

    // ISequantialStream Methods
    IFACEMETHODIMP Read(void *pv, ULONG cb, ULONG *pcbRead);
    IFACEMETHODIMP Write(const void *pv, ULONG cb, ULONG *pcbWritten) { return 0; }

    // IStream Methods
    IFACEMETHODIMP Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition) { return 0; }
    IFACEMETHODIMP SetSize(ULARGE_INTEGER libNewSize) { return 0; }
    IFACEMETHODIMP CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten) { return 0; }
    IFACEMETHODIMP Commit(DWORD grfCommitFlags) { return 0; }
    IFACEMETHODIMP Revert(void) { return 0; }
    IFACEMETHODIMP LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) { return 0; }
    IFACEMETHODIMP UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) { return 0; }
    IFACEMETHODIMP Stat(STATSTG *pstatstg, DWORD grfStatFlag) { return 0; }
    IFACEMETHODIMP Clone(IStream **ppstm) { return 0; }

private:
    unsigned char* mpData;
    ULONG mDataSize;
    ULONG mReadSize;
};

class png_reader : public image_reader {
    public:
        png_reader();
        virtual ~png_reader();
        virtual BOOL ReadImage(CString &path, int frame_id = -1);
        virtual BOOL ReadImage(unsigned char *data, int data_size);
        virtual BOOL CheckImageHasAlphaChannel(CString &path);
        virtual INT GetDelayTime(int frame_id = 0) { return 0; }

    protected:
         BOOL ReadImage(IStream* input_stream);
         UCHAR *GetInputDataPtr(int row);

    private:        
        void ConfigureInternalFormat();

        static void ReadPixel1(image_reader *reader, int Index, Pixel *put);
        static void ReadPixel2(image_reader *reader, int Index, Pixel *put);
        static void ReadPixel4(image_reader *reader, int Index, Pixel *put);
        static void ReadPixel8(image_reader *reader, int Index, Pixel *put);

        static void ReadPixel24(image_reader *reader, int Index, Pixel *put);
        static void ReadPixel48(image_reader *reader, int Index, Pixel *put);
        static void ReadPixel64(image_reader *reader, int Index, Pixel *put);
        static void ReadPixel24Alpha(image_reader *reader, int Index, Pixel *put);
        static void ReadPixel8Alpha(image_reader *reader, int Index, Pixel *put);
        static void ReadPixel16To8(image_reader *reader, int Index, Pixel *put);
        static void ReadPixel16Alpha(image_reader *reader, int Index, Pixel *put);

        static void ReadDataFromIStream(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead);

        png_structp m_png;
        png_infop   m_info;
        UCHAR   **mpRowPointers;
};

class jpg_reader : public image_reader {
    public:
        jpg_reader();
        virtual ~jpg_reader();
        virtual BOOL ReadImage(CString &path, int frame_id = -1);
        virtual BOOL ReadImage(unsigned char* data, int data_size);
        virtual INT GetDelayTime(int frame_id = 0) { return 0; }

    protected:
         UCHAR *GetInputDataPtr(int row);

    private:        
        jpeg_decompress_struct mJpgInfo;
        void ConfigureInternalFormat();
        UCHAR   *mpRawData;
        ULONG   mRawRowStride;
};

class gif_reader : public png_reader {
public:
    gif_reader();
    virtual ~gif_reader();

    virtual BOOL ReadImage(CString& path, int frame_id = -1);
    virtual BOOL ReadImage(unsigned char* data, int data_size);
    virtual BOOL CheckImageHasAlphaChannel(CString& path);
    virtual INT GetDelayTime(int frame_id = 0);
    static INT GetFrameCount(CString& path);

private:
    int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
    CArray<UINT> mDelayTimeList;
};

#endif
