
#include "studiox_includes.h"
#include "image_reader.h"
#include "png.h"

extern png_structp png_instance;
extern png_infop   png_info_ptr;

gif_reader::gif_reader() : png_reader()
{
}

gif_reader::~gif_reader()
{
}

BOOL gif_reader::CheckImageHasAlphaChannel(CString& path)
{
    return FALSE;
}

int gif_reader::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;  // Failure

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return -1;  // Failure

    GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j)
    {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}

///////////////////////////////////////////////////////////////////////////////
INT gif_reader::GetFrameCount(CString& path)
{
    Image img(path);

    UINT count = img.GetFrameDimensionsCount();
    if (count < 0)
    {
        return FALSE;
    }

    GUID* p_dimension_ids = new GUID[count];
    img.GetFrameDimensionsList(p_dimension_ids, count);
    int frame_count = img.GetFrameCount(&p_dimension_ids[0]);

    if (p_dimension_ids)
    {
        delete[]p_dimension_ids;
    }

    return frame_count;
}

///////////////////////////////////////////////////////////////////////////////
INT gif_reader::GetDelayTime(int frame_id)
{
    if (frame_id < mDelayTimeList.GetCount())
    {
        return mDelayTimeList.GetAt(frame_id);
    }
    else
    {
        return 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL gif_reader::ReadImage(CString& path, int frame_id)
{
    Image img(path);

    UINT count = img.GetFrameDimensionsCount();
    if (count < 0)
    {
        return FALSE;
    }

    GUID* p_dimension_ids = new GUID[count];
    img.GetFrameDimensionsList(p_dimension_ids, count);
    int frame_count = img.GetFrameCount(&p_dimension_ids[0]);

    CLSID pngClsid;
    GetEncoderClsid(L"image/png", &pngClsid);

    IStorage* pIStorage = NULL;
    IStream* pIStream = NULL;
    HRESULT hr;

    hr = CoInitialize(NULL);

    if (FAILED(hr))
    {
        return FALSE;
    }

    if (SUCCEEDED(hr))
    {
        // Create a compound file object, and get
        // a pointer to its IStorage interface.

        hr = StgCreateDocfile(
            NULL,
            STGM_READWRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE,
            0,
            &pIStorage);
    }

    if (SUCCEEDED(hr))
    {
        // Create a stream in the compound file.
        hr = pIStorage->CreateStream(
            L"StreamImage",
            STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
            0,
            0,
            &pIStream);
    }

    LARGE_INTEGER dlibMove = { 0 };

    BOOL result = 0;
    palette_info palinfo;

    palinfo.total_size = image_reader::GetPalSize();
    GX_COLOR *palette = image_reader::GetPalette();

    UINT buffer_size = img.GetPropertyItemSize(PropertyTagFrameDelay);
    PropertyItem* p_item = (PropertyItem*)(new byte[buffer_size]);
    img.GetPropertyItem(PropertyTagFrameDelay, buffer_size, p_item);

    mDelayTimeList.RemoveAll();
    int delay;
    int start_frame;
    int end_frame;
    BOOL copy_palette = FALSE;

    if (palette && (GetDisplayFormat() > GX_COLOR_FORMAT_8BIT_PALETTE))
    {
        copy_palette = TRUE;
    }

    if (frame_id >= 0 && frame_id < frame_count)
    {
        start_frame = frame_id;
        end_frame = frame_id;
    }
    else
    {
        start_frame = 0;
        end_frame = frame_count - 1;
    }

    for (int index = start_frame; index <= end_frame; index++)
    {
        delay = ((UINT*)p_item[0].value)[index] * 10;
        mDelayTimeList.Add(delay);

        if ((index != start_frame) && copy_palette)
        {
            palinfo.palette = new GX_COLOR[palinfo.total_size];
            memcpy_s(palinfo.palette, palinfo.total_size * sizeof(GX_COLOR), palette, (palinfo.total_size * sizeof(GX_COLOR)));
            SetPalette(palinfo.palette, palinfo.total_size, palinfo.total_size);
        }

        pIStream->Seek(dlibMove, STREAM_SEEK_SET, NULL);

        img.SelectActiveFrame(&p_dimension_ids[0], index);
        img.Save(pIStream, &pngClsid);

        pIStream->Seek(dlibMove, STREAM_SEEK_SET, NULL);

        result += png_reader::ReadImage(pIStream);
    }

    if (p_item)
    {
        delete p_item;
    }

    if (pIStream)
    {
        pIStream->Release();
    }

    if (pIStorage)
    {
        pIStorage->Release();
    }

    if (p_dimension_ids)
    {
        delete []p_dimension_ids;
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////
BOOL gif_reader::ReadImage(unsigned char* data, int data_size)
{
    // This route is not needed at present, return FALSE directly.
    return FALSE;
}