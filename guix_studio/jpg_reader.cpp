
#include "studiox_includes.h"
#include "image_reader.h"

extern "C" {
#include "jpeglib.h"
//#include "jerror.h"
//#include "jmorecfg.h"
}

struct my_error_mgr 
{
	struct jpeg_error_mgr pub;	/* "public" fields */
	jmp_buf setjmp_buffer;	/* for return to caller */
};
typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void) my_error_exit (j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr) cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp(myerr->setjmp_buffer, 1);
}

jpg_reader::jpg_reader() : image_reader()
{
    mpRawData = NULL;
    memset(&mJpgInfo, 0, sizeof(jpeg_decompress_struct));
}

jpg_reader::~jpg_reader()
{
    if (mpRawData)
    {
        delete [] mpRawData;
    }
}

BOOL jpg_reader::ReadImage(CString &path, int frame_id)
{
	struct my_error_mgr jerr;

    FILE *file = _tfopen(path.GetBuffer(), _T("rb"));

    if (!file)
    {
        return FALSE;
    }

	mJpgInfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

    jpeg_create_decompress(&mJpgInfo);

    if (setjmp(jerr.setjmp_buffer))
    {
        jpeg_destroy_decompress(&mJpgInfo);
        fclose(file);
        return FALSE;
    }

    jpeg_stdio_src(&mJpgInfo, file);
    
    if (jpeg_read_header(&mJpgInfo, TRUE) != JPEG_HEADER_OK)
    {
        fclose(file);
        jpeg_destroy_decompress(&mJpgInfo);
        return FALSE;
    }

    mJpgInfo.out_color_space = JCS_RGB;
    mJpgInfo.do_fancy_upsampling = FALSE;
    jpeg_calc_output_dimensions(&mJpgInfo);

    if (!jpeg_start_decompress(&mJpgInfo))
    {
        fclose(file);
        jpeg_destroy_decompress(&mJpgInfo);
        return FALSE;
    }

    // for now assume all JPG files are 24-bit rgb, and fail if it is not:

    if (mJpgInfo.output_components != 3)
    {
        jpeg_finish_decompress(&mJpgInfo);
        jpeg_destroy_decompress(&mJpgInfo);
        fclose(file);
        return FALSE;
    }
    SetDefaultPixelReader(24);

    // test for 32-bit overflow before we attempt to allocate memory for
    // jpg input file:

    long long testval;

    mRawRowStride = mJpgInfo.output_width * mJpgInfo.output_components;

    testval = (long long) mRawRowStride * (long long) mJpgInfo.output_height;
    if (testval & 0xffffffff00000000)
    {
        jpeg_destroy_decompress(&mJpgInfo);
        fclose(file);
        return FALSE;
    }

    // no size overflow, continue

    mpRawData = new UCHAR[mRawRowStride * mJpgInfo.output_height];

    if (!mpRawData)
    {
        jpeg_destroy_decompress(&mJpgInfo);
        fclose(file);
        return FALSE;
    }
    memset(mpRawData, 0,  mRawRowStride * mJpgInfo.output_height);

    while (mJpgInfo.output_scanline < mJpgInfo.output_height) 
    {
        UCHAR *pPut = mpRawData;
        pPut += mJpgInfo.output_scanline * mRawRowStride;
        jpeg_read_scanlines(&mJpgInfo, &pPut, 1);
    }

    jpeg_finish_decompress(&mJpgInfo);
    jpeg_destroy_decompress(&mJpgInfo);

    DoCompress(FALSE);
    if (FinalizeImage(mJpgInfo.output_width, mJpgInfo.output_height))
    {
        fclose(file);
        return TRUE;
    }
    else
    {
        fclose(file);
        return FALSE;
    }
}

BOOL jpg_reader::ReadImage(unsigned char* data, int data_size)
{
    // This route is not needed at present, return FALSE directly.
    return FALSE;
}

UCHAR *jpg_reader::GetInputDataPtr(int row)
{
    if (mpRawData)
    {
        UCHAR *pPut = mpRawData;
        pPut += row * mRawRowStride;
        return pPut;
    }
    return NULL;
}


