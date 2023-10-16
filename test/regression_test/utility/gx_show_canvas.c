#include "windows.h"
#include "stdio.h"
#include "stdlib.h"
#include "gx_validation_strings.h"
#include "gx_validation_verify.h"

/* Define the Windows Bitmap Structure */
typedef struct GUIX_BMP_INFO_STRUCT
{
    BITMAPINFOHEADER bmiHeader;
    DWORD            bmiColors[256];
} GUIX_BMP_INFO;

GUIX_BMP_INFO bmp_info;
FILE *pFile = NULL;
FILE *goldenFile = NULL;
unsigned int*buffer = NULL;
unsigned int*golden_frame_buffer = NULL;
int total_frames = 0;
int height = 0;
int width = 0;
int data_width = 0;
HWND hwnd;
int index = 1;
int bits_per_pixel = 0;
int len;
int file_foramt = 0;

static void win32_16srgb_bitmap_header_create(int width, int height)
{

DWORD *putmask;
HDC   win_device;



    bmp_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmp_info.bmiHeader.biWidth = width;
    bmp_info.bmiHeader.biHeight = height;

    bmp_info.bmiHeader.biPlanes = 1;
    bmp_info.bmiHeader.biBitCount = 16;
    bmp_info.bmiHeader.biSizeImage = width * height * 2;
    bmp_info.bmiHeader.biClrUsed = 65535;
    bmp_info.bmiHeader.biClrImportant = 65535;
    bmp_info.bmiHeader.biCompression = BI_BITFIELDS;


    putmask = (DWORD *)&(bmp_info.bmiColors[0]);

    *putmask++ = 0x0000f800;
    *putmask++ = 0x000007e0;
    *putmask   = 0x0000001f;

}

static void win32_565bgr_bitmap_header_create(int width, int height)
{

    DWORD* putmask;
    HDC   win_device;



    bmp_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmp_info.bmiHeader.biWidth = width;
    bmp_info.bmiHeader.biHeight = height;

    bmp_info.bmiHeader.biPlanes = 1;
    bmp_info.bmiHeader.biBitCount = 16;
    bmp_info.bmiHeader.biSizeImage = width * height * 2;
    bmp_info.bmiHeader.biClrUsed = 65535;
    bmp_info.bmiHeader.biClrImportant = 65535;
    bmp_info.bmiHeader.biCompression = BI_BITFIELDS;


    putmask = (DWORD*)&(bmp_info.bmiColors[0]);

    *putmask++ = 0x0000001f;
    *putmask++ = 0x000007e0;
    *putmask = 0x0000f800;

}

static void win32_1555xrgb_bitmap_header_create(int width, int height)
{

    DWORD *putmask;
    HDC   win_device;



    bmp_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmp_info.bmiHeader.biWidth = width;
    bmp_info.bmiHeader.biHeight = height;

    bmp_info.bmiHeader.biPlanes = 1;
    bmp_info.bmiHeader.biBitCount = 16;
    bmp_info.bmiHeader.biSizeImage = width * height * 2;
    bmp_info.bmiHeader.biClrUsed = 65535;
    bmp_info.bmiHeader.biClrImportant = 65535;
    bmp_info.bmiHeader.biCompression = BI_BITFIELDS;


    putmask = (DWORD *)&(bmp_info.bmiColors[0]);

    *putmask++ = 0x00007c00;
    *putmask++ = 0x000003e0;
    *putmask = 0x0000001f;

}

static void win32_4444argb_bitmap_header_create(int width, int height)
{

    DWORD *putmask;
    HDC   win_device;



    bmp_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmp_info.bmiHeader.biWidth = width;
    bmp_info.bmiHeader.biHeight = height;

    bmp_info.bmiHeader.biPlanes = 1;
    bmp_info.bmiHeader.biBitCount = 16;
    bmp_info.bmiHeader.biSizeImage = width * height * 2;
    bmp_info.bmiHeader.biClrUsed = 65535;
    bmp_info.bmiHeader.biClrImportant = 65535;
    bmp_info.bmiHeader.biCompression = BI_BITFIELDS;


    putmask = (DWORD *)&(bmp_info.bmiColors[0]);

    *putmask++ = 0x00000f00;
    *putmask++ = 0x000000f0;
    *putmask = 0x0000000f;

}

static void win32_24srgb_bitmap_header_create(int width, int height)
{

DWORD *putmask;
HDC   win_device;



    bmp_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmp_info.bmiHeader.biWidth = width;
    bmp_info.bmiHeader.biHeight = height;

    bmp_info.bmiHeader.biPlanes = 1;
    bmp_info.bmiHeader.biBitCount = 32;
    bmp_info.bmiHeader.biSizeImage = width * height * 4;
    bmp_info.bmiHeader.biClrUsed = 16777215;
    bmp_info.bmiHeader.biClrImportant = 16777215;
    bmp_info.bmiHeader.biCompression = BI_BITFIELDS;


    putmask = (DWORD *)&(bmp_info.bmiColors[0]);

    *putmask++ = 0x00ff0000;
    *putmask++ = 0x0000ff00;
    *putmask   = 0x000000ff;

}

static void win32_8bpp_bitmap_header_create(width, height)
{
    bmp_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmp_info.bmiHeader.biWidth = width;
    bmp_info.bmiHeader.biHeight = height;

    bmp_info.bmiHeader.biPlanes = 1;
    bmp_info.bmiHeader.biBitCount = 8;
    bmp_info.bmiHeader.biSizeImage = width * height;
    bmp_info.bmiHeader.biClrUsed = 255;
    bmp_info.bmiHeader.biClrImportant = 255;
    bmp_info.bmiHeader.biCompression = BI_RGB;
}

static void win32_332rgb_bitmap_header_create(width, height)
{
    ULONG                         color;
    INT                           r = 0;
    INT                           g = 0;
    INT                           b = 0;
    INT                           i = 0;
    INT                           red;
    INT                           green;
    INT                           blue;

    bmp_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmp_info.bmiHeader.biWidth = width;
    bmp_info.bmiHeader.biHeight = height;

    bmp_info.bmiHeader.biPlanes = 1;
    bmp_info.bmiHeader.biBitCount = 8;
    bmp_info.bmiHeader.biSizeImage = width * height;
    bmp_info.bmiHeader.biClrUsed = 0;
    bmp_info.bmiHeader.biClrImportant = 0;
    bmp_info.bmiHeader.biCompression = BI_RGB;

    for (r = 0; r < 8; r++)
    {
        red = r << 21;
        if (red & 0x200000) { red |= 0x1f0000; }
        for (g = 0; g < 8; g++)
        {
            green = g << 13;
            if (green & 0x2000) { green |= 0x1f00; }
            for (b = 0; b < 4; b++)
            {
                blue = b << 6;
                if (blue & 0x40) { blue |= 0x3f; }
                color = red | green | blue;
                bmp_info.bmiColors[i] = color;
                i++;
             }
        }
     }
}

static void win32_8bpp_palette_set()
{
    ULONG color = 0;
    int loop;
    int count = 0;
    char line[256];

    while(!count)
    {
        if(fgets(line, 100, pFile) != NULL);
        {
            if(strncmp(line, "PALETTE DATA SIZE: ", strlen("PALETTE DATA SIZE: ")) == 0)
                count = atoi(&line[strlen("PALETTE DATA SIZE: ")]);
        }
    }
    fgets(line, 100, pFile);

    bmp_info.bmiHeader.biClrUsed = count;
    bmp_info.bmiHeader.biClrImportant = count;

    for (loop = 0; loop < count; loop++)
    {
        fscanf(pFile, "%ul", &color);
        bmp_info.bmiColors[loop] = color;
    }
}

static ULONG grayscale_table[] =
{
    0xff000000,
    0xff111111,
    0xff222222,
    0xff333333,
    0xff444444,
    0xff555555,
    0xff666666,
    0xff777777,
    0xff888888,
    0xff999999,
    0xffaaaaaa,
    0xffbbbbbb,
    0xffcccccc,
    0xffdddddd,
    0xffeeeeee,
    0xffffffff
};

static void win32_4bpp_bitmap_header_create(width, height)
{
    bmp_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmp_info.bmiHeader.biWidth = width;
    bmp_info.bmiHeader.biHeight = height;

    bmp_info.bmiHeader.biPlanes = 1;
    bmp_info.bmiHeader.biBitCount = 4;
    bmp_info.bmiHeader.biSizeImage = width * height >> 1;
    bmp_info.bmiHeader.biClrUsed = 16;
    bmp_info.bmiHeader.biClrImportant = 16;
    bmp_info.bmiHeader.biCompression = BI_RGB;

    memcpy(bmp_info.bmiColors, grayscale_table, sizeof(grayscale_table));
}

static ULONG color_table[] =
{
    0x00000000,
    0x00ffffff
};

static void win32_1bpp_bitmap_header_create(width, height)
{
    bmp_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmp_info.bmiHeader.biWidth = width;
    bmp_info.bmiHeader.biHeight = height;

    bmp_info.bmiHeader.biPlanes = 1;
    bmp_info.bmiHeader.biBitCount = 1;
    bmp_info.bmiHeader.biSizeImage = (width * height) >> 3;
    bmp_info.bmiHeader.biClrUsed = 2;
    bmp_info.bmiHeader.biClrImportant = 2;
    bmp_info.bmiHeader.biCompression = BI_RGB;

    memcpy(bmp_info.bmiColors, color_table, sizeof(color_table));
}

LRESULT CALLBACK WndProc(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
PAINTSTRUCT ps;

    switch(message)
    {
    case WM_TIMER:
        return 0;

    case WM_PAINT:
        BeginPaint(hwnd, &ps);
        show_frame();
        EndPaint(hwnd, &ps);

        return 0;

    case WM_KEYDOWN:
        if(wParam == 0x26) /* Upper arrow key */
        {
            if(index > 1)
            {
                index--;
                show_frame();
            }
        }
        else if(wParam == 0x28) /* Down arrow key */
        {
            if(index < total_frames)
            {
                index++;
                show_frame();
            }
        }

        else if((wParam == 'q') || (wParam == 'Q'))
        {
            PostQuitMessage(0);
            return 0;
        }
        break;



    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

static HWND win32_window_create(void)
{
HINSTANCE Instance = GetModuleHandle(NULL);
RECT      TargetSize;
DWORD     WinStyle;
HWND      win32_hwnd = 0;
WNDCLASS  wndclass;

    memset(&wndclass, 0, sizeof(wndclass));

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = Instance;

    wndclass.hCursor = NULL;
    wndclass.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = TEXT("GUIX");

    RegisterClass(&wndclass);

    TargetSize.left = 0;
    TargetSize.top = 0;
    TargetSize.right = bmp_info.bmiHeader.biWidth;
    TargetSize.bottom = bmp_info.bmiHeader.biHeight;

    WinStyle = WS_CAPTION | WS_SIZEBOX | WS_VISIBLE | WS_MINIMIZEBOX |
        WS_MAXIMIZEBOX | WS_SYSMENU;


    /* This function adjusts the size of the Windows window so that the client
    size is the size of the display, taking into account borders
    and titles and such. We want our window client are to be exactly
    the simulatedd display size.
    */
    AdjustWindowRect(&TargetSize, WinStyle, FALSE);
    win32_hwnd = CreateWindow(TEXT("GUIX"), TEXT("GUIX"),
        WinStyle, 20, 20,
        TargetSize.right - TargetSize.left + 1,
        TargetSize.bottom - TargetSize.top + 1, NULL, NULL,
        Instance, NULL);
    SetCursor(LoadCursor(NULL, IDC_ARROW));

    return win32_hwnd;
}

int device_created = 0;

char comment_buffer[256];
char golden_frame_comment[256];

static find_differences(unsigned int *buffer, unsigned int *golden_buffer, int buffer_size, int frame_ID)
{

unsigned int *data = buffer;
unsigned int *gdata = golden_buffer;
int pos;
int print_frame_id = 0;

    for (pos = 0; pos < buffer_size; pos++)
    {
        if (*data != *gdata)
        {
            if (print_frame_id == 0)
            {
                printf("Frame %d is different\n", frame_ID);
                print_frame_id = 1;
            }
            *data = ~(*data);
        }
        data++;
        gdata++;
    }
}


int show_frame(void)
{
HDC win_device;
int frame_number = -1;
int golden_frame_number = -1;
int file_format;
int ret = 0;
int i;
char *color_format;

    fseek(pFile, 0, SEEK_SET);
    if(goldenFile)
        fseek(goldenFile, 0, SEEK_SET);


    if(device_created == 0)
    {
        memset(&bmp_info, 0, sizeof(GUIX_BMP_INFO));

        ret = read_file_header(pFile, &file_format, &total_frames, &color_format, &bits_per_pixel, &width, &height);

        if(ret == -1)
        {
            cleanup(pFile, NULL);
            if(goldenFile)
                cleanup(goldenFile, NULL);
            exit(0);
        }

        if(bits_per_pixel == 16 && ((width & 1) != 0))
        {
            data_width = width + 1;
        }
        else if(bits_per_pixel == 8 && (width % 4))
        {
            data_width = width + 4 - (width % 4);
        }
        else if ((bits_per_pixel == 4) && (width % 8))
        {
            data_width = width + 8 - (width % 8);
        }
        else
        {
            data_width = width;
        }
        buffer = (unsigned int*)malloc(data_width * height * bits_per_pixel >> 3);
        if(goldenFile)
            golden_frame_buffer = (unsigned int*)malloc(data_width * height * bits_per_pixel >> 3);

        if (!strcmp(color_format, COLOR_FORMAT_24XRGB))
        {
            win32_24srgb_bitmap_header_create(width, height);
        }
        else if (!strcmp(color_format, COLOR_FORMAT_565RGB))
        {
            win32_16srgb_bitmap_header_create(width, height);
        }
        else if (!strcmp(color_format, COLOR_FORMAT_565BGR))
        {
            win32_565bgr_bitmap_header_create(width, height);
        }
        else if (!strcmp(color_format, COLOR_FORMAT_1555XRGB))
        {
            win32_1555xrgb_bitmap_header_create(width, height);
        }
        else if (!strcmp(color_format, COLOR_FORMAT_4444ARGB))
        {
            win32_4444argb_bitmap_header_create(width, height);
        }
        else if (!strcmp(color_format, COLOR_FORMAT_8BPP))
        {
            win32_8bpp_bitmap_header_create(width, height);
            win32_8bpp_palette_set();
        }
        else if(!strcmp(color_format, COLOR_FORMAT_332RGB))
        {
            win32_332rgb_bitmap_header_create(width, height);
        }
        else if(!strcmp(color_format, COLOR_FORMAT_4BPP))
        {
            win32_4bpp_bitmap_header_create(width, height);
        }
        else if (!strcmp(color_format, COLOR_FORMAT_1BPP))
        {
            win32_1bpp_bitmap_header_create(width, height);
        }

        hwnd = win32_window_create();

        device_created = 1;
    }

    for(i = 1; i < index; i++)
    {
        find_frame_data(pFile);
        fseek(pFile, (bits_per_pixel  * data_width * height >> 3), SEEK_CUR);
        if(goldenFile)
        {
            find_frame_data(goldenFile);
            fseek(goldenFile, (bits_per_pixel * data_width * height >> 3), SEEK_CUR);
        }
    }
    get_frame_id(pFile, &frame_number);
    get_frame_comment(pFile, comment_buffer, sizeof(comment_buffer));
    fread(buffer, 1, (bits_per_pixel * data_width * height) >> 3, pFile);
    if(goldenFile)
    {
        get_frame_id(goldenFile, &golden_frame_number);
        get_frame_comment(goldenFile, golden_frame_comment, sizeof(golden_frame_comment));
        fread(golden_frame_buffer, 1, (bits_per_pixel * data_width * height ) >> 3, goldenFile);

        find_differences(buffer, golden_frame_buffer, (data_width * height * bits_per_pixel) >> 5, index);
    }
    if(strlen(comment_buffer))
    {
        printf("frame number %d %s\n", frame_number, comment_buffer);
    }
    else
        printf("frame number %d\n", frame_number);

    win_device = GetDC(hwnd);
    SetMapMode(win_device, MM_TEXT);

    StretchDIBits(win_device, 0, 0, width, height,
                  0, height, width, -height, 
                  buffer,
                  (BITMAPINFO *)&bmp_info,
                  DIB_RGB_COLORS,
                  SRCCOPY);

    ReleaseDC(hwnd, win_device);
}

int main(int argc, char **argv)
{

MSG Msg;
char *golden_file_name = NULL;

    if(argc == 3)
    {
        if((argv[2][0] <= '9') && (argv[2][0] >= '0'))
            index = atoi(argv[2]);
        else
            golden_file_name = argv[2];
    }
    if(argc == 4)
    {
        index = atoi(argv[2]);
        golden_file_name = argv[3];
    }

    if(golden_file_name)
        goldenFile = fopen(golden_file_name, "rb");

    pFile = fopen(argv[1], "rb");

    if(pFile != NULL)
    {
        show_frame(index);
    }


    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    cleanup(pFile, buffer);
    if(goldenFile)
        cleanup(goldenFile, golden_frame_buffer);

    exit(0);
}




