
#include <stdio.h>
#include <stdlib.h>
#include "gx_api.h"
#include "gx_display.h"
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <semaphore.h>
#include "gx_x11_binding.h"
#include <ctype.h>

/* FIXME: It's weird that include gx_display.h but setup functions still cannot be found.
   This must be have something to do the makefile setting. */
VOID _gx_display_driver_24xrgb_setup(GX_DISPLAY *display, VOID *aux_data,
                                     VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                             GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_565rgb_setup(GX_DISPLAY *display, VOID *aux_data,
                                     VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                             GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_4444argb_setup(GX_DISPLAY *display, VOID *aux_data,
                                       VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                               GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_8bit_palette_setup(GX_DISPLAY *display, VOID *aux_data,
                                           VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                                   GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_4bpp_grayscale_setup(GX_DISPLAY *display, VOID *aux_data,
                                             VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                                     GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_monochrome_setup(GX_DISPLAY *display, VOID *aux_data,
                                         VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                                 GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_332rgb_setup(GX_DISPLAY *display, VOID *aux_data,
                                     VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                             GX_RECTANGLE *dirty_area));

VOID _gx_display_driver_1555xrgb_setup(GX_DISPLAY *display, VOID *aux_data,
                                       VOID (*toggle_function)(struct GX_CANVAS_STRUCT *canvas,
                                                               GX_RECTANGLE *dirty_area));
/*---------------------------------------------------------------------------*/
// Globals
/*---------------------------------------------------------------------------*/
Display            *gx_x11_display;
Window              gx_x11_window;
int                 gx_x11_screen;
GC                  gx_x11_context;

XImage              *gx_x11_image = NULL;
XVisualInfo         gx_x11_visual;

char                *gx_canvas_memory; 

// The event mask. This is added to later based on hardware support.

int iEventMask = ExposureMask | StructureNotifyMask | SubstructureNotifyMask |
                 SubstructureRedirectMask;

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
GX_BOOL gx_x11_get_preferred_visual(int depth, int class_filter, XVisualInfo *pVisual)
{
    XVisualInfo viTemplate;
    XVisualInfo *pVIList;
    int iVIListNum;
    GX_BOOL RetVal = GX_FALSE;

    viTemplate.screen = gx_x11_screen;
    viTemplate.depth = depth;
    viTemplate.class = class_filter;

    pVIList = XGetVisualInfo(gx_x11_display,
        VisualScreenMask|VisualDepthMask|VisualClassMask,
        &viTemplate, &iVIListNum);

    if (pVIList)
    {
        debug("found preferred visual");
        // the X server supports the color depth and class asked for
        memcpy(pVisual, pVIList, sizeof(XVisualInfo)); /* Use case of memcpy is verified. */
        RetVal = GX_TRUE;
        XFree(pVIList);
    }
    else
    {
        debug("did not find preferred visual");
        // send back the default color depth visual
        viTemplate.depth = DefaultDepth(gx_x11_display, gx_x11_screen);
        pVIList = XGetVisualInfo(gx_x11_display, VisualDepthMask,
            &viTemplate, &iVIListNum);

        if (pVIList)
        {
            memcpy(pVisual, pVIList, sizeof(XVisualInfo)); /* Use case of memcpy is verified. */
            XFree(pVIList);
        }
    }

    return(RetVal);
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
GX_BOOL gx_allocate_x_canvas(GX_VALUE width, GX_VALUE height)
{
    if(gx_x11_image)
    {
        // This will also delete mpImageMem
        XDestroyImage(gx_x11_image);
    }

    if (!gx_x11_get_preferred_visual(24, TrueColor, &gx_x11_visual))
    {
        debug("Failed to get preferred visual");
        return GX_FALSE;
    }
    
    if((width > GX_MAX_DISPLAY_RESOLUTION) || (height > GX_MAX_DISPLAY_RESOLUTION))
    {
        debug("Invalid canvas size");
    }

    printf("depth: %d\n", gx_x11_visual.depth);
    printf("red_mask: %lu\n", gx_x11_visual.red_mask);
    printf("red_mask: %lu\n", gx_x11_visual.green_mask);
    printf("red_mask: %lu\n", gx_x11_visual.blue_mask);
    printf("bits_per_rgb: %d\n", gx_x11_visual.bits_per_rgb);

    UINT wHorzQuant = width * sizeof(GX_COLOR);
    
    // canvas width and height are limited to 8192, overflow cannot occur
    gx_canvas_memory = malloc(wHorzQuant * height);
    
    if(!gx_canvas_memory)
    {
        debug("could not allocate gx_canvas_memory");
        return GX_FALSE;
    }
    
    // the quantum will always be 32 since we fixed up the 
    // width and height before hand.
    gx_x11_image = XCreateImage(gx_x11_display,
            gx_x11_visual.visual, gx_x11_visual.depth, 
            ZPixmap, 0, gx_canvas_memory, width, height, 32, wHorzQuant);

    if(!gx_x11_image)
    {
        debug("Could not create gx_x11_image.\n");
        return GX_FALSE;
    }
    return GX_TRUE;
}

void block_fill(int top, int width, int height, unsigned long color)
{
    int row;
    int column;

    //GX_COLOR *put = (GX_COLOR *) gx_canvas_memory;
    UINT *put = (UINT *) gx_canvas_memory;
    put += width * top;

    for (row = 0; row < height; row++)
    {
        for (column = 0; column < width; column++)
        {
            *put++ = color;
        }
    }    
}

static void _gx_copy_canvas_to_buffer_24xrgb(char *dest, GX_CANVAS *canvas, GX_RECTANGLE *dirty_area)
{
GX_COLOR     *put_row;
GX_COLOR     *read_row;
GX_COLOR     *put;
GX_COLOR     *read;
int           width;
int           height;
int           x, y;

    width = canvas->gx_canvas_x_resolution;
    height = canvas->gx_canvas_y_resolution;
    /* Calculate dst start ptr*/
    put_row = (GX_COLOR *)dest;
    put_row += dirty_area->gx_rectangle_top * width;
    put_row += dirty_area->gx_rectangle_left;

    /* Calculate src start ptr*/
    read_row = (GX_COLOR *)canvas->gx_canvas_memory;
    read_row += dirty_area->gx_rectangle_top * width + dirty_area->gx_rectangle_left;

    for (y = dirty_area->gx_rectangle_top; y < dirty_area->gx_rectangle_bottom; y++)
    {
        put = put_row;
        read = read_row;
        for (x = dirty_area->gx_rectangle_left; x < dirty_area->gx_rectangle_right; x++)
        {
            *put++ = *read++;
        }
        put_row += width;
        read_row += width;
    }
}

static void _gx_copy_canvas_to_buffer_565rgb(char *dest, GX_CANVAS *canvas, GX_RECTANGLE *dirty_area)
{
GX_COLOR     *put;
USHORT       *read;
GX_COLOR     *put_row;
USHORT       *read_row;
int           width;
int           height;
int           x, y;
GX_UBYTE      red;
GX_UBYTE      green;
GX_UBYTE      blue;

    width = canvas->gx_canvas_x_resolution;
    height = canvas->gx_canvas_y_resolution;
    /* Calculate dst start ptr*/
    put_row = (GX_COLOR *)dest;
    put_row += dirty_area->gx_rectangle_top * width;
    put_row += dirty_area->gx_rectangle_left;

    /* Calculate src start ptr*/
    read_row = (USHORT *)canvas->gx_canvas_memory;
    read_row += dirty_area->gx_rectangle_top * width + dirty_area->gx_rectangle_left;

    for (y = dirty_area->gx_rectangle_top; y < dirty_area->gx_rectangle_bottom; y++)
    {
        put = put_row;
        read = read_row;
        for (x = dirty_area->gx_rectangle_left; x < dirty_area->gx_rectangle_right; x++)
        {
            red = REDVAL_16BPP(*read) << 3;
            if (red & 0x08) { red |= 0x07; }
            green = GREENVAL_16BPP(*read) << 2;
            if (green & 0x04) { green |= 0x03; }
            blue = BLUEVAL_16BPP(*read) << 3;
            if (blue & 0x08) { blue |= 0x07; }
            *put++ = ASSEMBLECOLOR_32BPP(red, green, blue);
            read++;
        }
        put_row += width;
        read_row += width;
    }

}

/* Need to be checked later. */
static void _gx_copy_canvas_to_buffer_1555xrgb(char *dest, GX_CANVAS *canvas, GX_RECTANGLE *dirty_area)
{
GX_COLOR     *put;
USHORT       *read;
GX_COLOR     *put_row;
USHORT       *read_row;
int           width;
int           height;
int           x, y;
GX_UBYTE      red;
GX_UBYTE      green;
GX_UBYTE      blue;

    width = canvas->gx_canvas_x_resolution;
    height = canvas->gx_canvas_y_resolution;
    /* Calculate dst start ptr*/
    put_row = (GX_COLOR *)dest;
    put_row += dirty_area->gx_rectangle_top * width;
    put_row += dirty_area->gx_rectangle_left;

    /* Calculate src start ptr*/
    read_row = (USHORT *)canvas->gx_canvas_memory;
    read_row += dirty_area->gx_rectangle_top * width + dirty_area->gx_rectangle_left;

    for (y = dirty_area->gx_rectangle_top; y < dirty_area->gx_rectangle_bottom; y++)
    {
        put = put_row;
        read = read_row;
        for (x = dirty_area->gx_rectangle_left; x < dirty_area->gx_rectangle_right; x++)
        {
            red = ((*read) & 0x7c00) >> 7;
            if (red & 0x08) { red |= 0x07; }
            green = ((*read) & 0x03e0) >> 2;
            if (green & 0x08) { green |= 0x03; }
            blue = ((*read) & 0x1f) << 3;
            if (blue & 0x08) { blue |= 0x07; }
            *put++ = ASSEMBLECOLOR_32BPP(red, green, blue);
            read++;
        }
        put_row += width;
        read_row += width;
    }
}


static void _gx_copy_canvas_to_buffer_4444argb(char *dest, GX_CANVAS *canvas, GX_RECTANGLE *dirty_area)
{
GX_COLOR     *put;
USHORT       *read;
GX_COLOR     *put_row;
USHORT       *read_row;
int           width;
int           height;
int           x, y;
GX_UBYTE      red;
GX_UBYTE      green;
GX_UBYTE      blue;

    width = canvas->gx_canvas_x_resolution;
    height = canvas->gx_canvas_y_resolution;
    /* Calculate dst start ptr*/
    put_row = (GX_COLOR *)dest;
    put_row += dirty_area->gx_rectangle_top * width;
    put_row += dirty_area->gx_rectangle_left;

    /* Calculate src start ptr*/
    read_row = (USHORT *)canvas->gx_canvas_memory;
    read_row += dirty_area->gx_rectangle_top * width + dirty_area->gx_rectangle_left;

    for (y = dirty_area->gx_rectangle_top; y < dirty_area->gx_rectangle_bottom; y++)
    {
        put = put_row;
        read = read_row;
        for (x = dirty_area->gx_rectangle_left; x < dirty_area->gx_rectangle_right; x++)
        {
            red = ((*read) & 0x0f00) >> 4;
            red |= red >> 4;
            green = (*read) & 0xf0;
            green |= green >> 4;
            blue = ((*read) & 0x0f);
            blue |= blue << 4;
            *put++ = ASSEMBLECOLOR_32BPP(red, green, blue);
            read++;
        }
        put_row += width;
        read_row += width;
    }
}

static void _gx_copy_canvas_to_buffer_332rgb(char *dest, GX_CANVAS *canvas, GX_RECTANGLE *dirty_area)
{
GX_COLOR     *put;
GX_UBYTE     *read;
GX_COLOR     *put_row;
GX_UBYTE     *read_row;
int           width;
int           height;
int           x, y;
GX_UBYTE      red;
GX_UBYTE      green;
GX_UBYTE      blue;

    width = canvas->gx_canvas_x_resolution;
    height = canvas->gx_canvas_y_resolution;
    /* Calculate dst start ptr*/
    put_row = (GX_COLOR *)dest;
    put_row += dirty_area->gx_rectangle_top * width;
    put_row += dirty_area->gx_rectangle_left;

    /* Calculate src start ptr*/
    read_row = (GX_UBYTE *)canvas->gx_canvas_memory;
    read_row += dirty_area->gx_rectangle_top * width + dirty_area->gx_rectangle_left;

    for (y = dirty_area->gx_rectangle_top; y < dirty_area->gx_rectangle_bottom; y++)
    {
        put = put_row;
        read = read_row;
        for (x = dirty_area->gx_rectangle_left; x < dirty_area->gx_rectangle_right; x++)
        {
            red = (*read) & 0xe0;
            if (red & 0x20) { red |= 0x1f; }
            green = ((*read) & 0x1c) << 3;
            if (green & 0x20) { green |= 0x1f; }
            blue = ((*read) & 0x03) << 6;
            if (blue & 0x40) { blue |= 0x3f; }
            *put++ = ASSEMBLECOLOR_32BPP(red, green, blue);
            read++;
        }
        put_row += width;
        read_row += width;
    }
}

static void _gx_copy_canvas_to_buffer_8bit_palette(char *dest, GX_CANVAS *canvas, GX_RECTANGLE *dirty_area)
{
GX_COLOR     *put;
GX_UBYTE     *read;
GX_COLOR     *put_row;
GX_UBYTE     *read_row;
int           width;
int           height;
int           x, y;
GX_COLOR     *palette_table;

    width = canvas->gx_canvas_x_resolution;
    height = canvas->gx_canvas_y_resolution;
    palette_table = canvas->gx_canvas_display->gx_display_palette;
    /* Calculate dst start ptr*/
    put_row = (GX_COLOR *)dest;
    put_row += dirty_area->gx_rectangle_top * width;
    put_row += dirty_area->gx_rectangle_left;

    /* Calculate src start ptr*/
    read_row = (GX_UBYTE *)canvas->gx_canvas_memory;
    read_row += dirty_area->gx_rectangle_top * width + dirty_area->gx_rectangle_left;

    for (y = dirty_area->gx_rectangle_top; y < dirty_area->gx_rectangle_bottom; y++)
    {
        put = put_row;
        read = read_row;
        for (x = dirty_area->gx_rectangle_left; x < dirty_area->gx_rectangle_right; x++)
        {
            *put++ = palette_table[*read++];
        }
        put_row += width;
        read_row += width;
    }
}

static void _gx_copy_canvas_to_buffer_4bit_grayscale(char *dest, GX_CANVAS *canvas, GX_RECTANGLE *dirty_area)
{
GX_COLOR     *put;
GX_UBYTE     *read;
GX_COLOR     *put_row;
GX_UBYTE     *read_row;
int           x, y;
GX_UBYTE      read_mask;
GX_UBYTE      read_pixel;
int           width;
int           height;

    width = canvas->gx_canvas_x_resolution;
    height = canvas->gx_canvas_y_resolution;
    /* Calculate dst start ptr*/
    put_row = (GX_COLOR *)dest;
    put_row += dirty_area->gx_rectangle_top * width;
    put_row += dirty_area->gx_rectangle_left;

    /* Calculate src start ptr*/
    read_row = (GX_UBYTE *)canvas->gx_canvas_memory;
    read_row += dirty_area->gx_rectangle_top * ((width + 1) >> 1);
    read_row += (dirty_area->gx_rectangle_left + 1 )>> 1;

    for (y = dirty_area->gx_rectangle_top; y < dirty_area->gx_rectangle_bottom; y++)
    {
        put = put_row;
        read = read_row;

        if (dirty_area->gx_rectangle_left & 1)
        {
            read_mask = 0x0f;
        }
        else
        {
            read_mask = 0xf0;
        }

        for (x = dirty_area->gx_rectangle_left; x < dirty_area->gx_rectangle_right; x++)
        {
            read_pixel = read_mask & (*read);

            if (read_mask == 0xf0)
            {
                read_pixel |= read_pixel >> 4;
                read_mask = 0x0f;
            }
            else
            {
                read_pixel |= read_pixel << 4;
                read_mask = 0xf0;
                read++;
            }
            *put++ = read_pixel | (read_pixel << 8) | (read_pixel << 16) | 0xff000000;
        }
        put_row += width;
        read_row += (width + 1) >> 1;
    }
}


static void _gx_copy_canvas_to_buffer_monochrome(char *dest, GX_CANVAS *canvas, GX_RECTANGLE *dirty_area)
{
GX_COLOR     *put;
GX_UBYTE     *read;
GX_COLOR     *put_row;
GX_UBYTE     *read_row;
int           x, y;
GX_UBYTE      read_mask;
int           width;
int           height;

    width = canvas->gx_canvas_x_resolution;
    height = canvas->gx_canvas_y_resolution;
    /* Calculate dst start ptr*/
    put_row = (GX_COLOR *)dest;
    put_row += dirty_area->gx_rectangle_top * width;
    put_row += dirty_area->gx_rectangle_left;

    /* Calculate src start ptr*/
    read_row = (GX_UBYTE *)canvas->gx_canvas_memory;
    read_row += dirty_area->gx_rectangle_top * ((width + 7) >> 3) ;
    read_row += (dirty_area->gx_rectangle_left + 7) >> 3;

    for (y = dirty_area->gx_rectangle_top; y < dirty_area->gx_rectangle_bottom; y++)
    {
        put = put_row;
        read = read_row;

        read_mask = 0x80 >> (dirty_area->gx_rectangle_left & 0x07);

        for (x = dirty_area->gx_rectangle_left; x < dirty_area->gx_rectangle_right; x++)
        {
            if (read_mask & (*read))
            {
                *put++ = 0xffffffff;
            }
            else
            {
                *put++ = 0;
            }

            read_mask >>= 1;
            if (read_mask == 0)
            {
                read_mask = 0x80;
                read++;
            }
        }
        read_row += (width + 7) >> 3;
        put_row += width;
    }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void gx_x11_buffer_toggle(struct GX_CANVAS_STRUCT *canvas, GX_RECTANGLE *dirty_area)
{
GX_VALUE            format;
GX_RECTANGLE        Limit;
GX_RECTANGLE        Copy;
int                 width;
int                 height;

    width = canvas->gx_canvas_x_resolution;
    height = canvas->gx_canvas_y_resolution;

    //debug("At gx_x11_buffer_toggle");
    gx_utility_rectangle_define(&Limit, 0, 0,
                                canvas -> gx_canvas_x_resolution - 1,
                                canvas -> gx_canvas_y_resolution - 1);
    gx_utility_rectangle_overlap_detect(&Limit, &canvas->gx_canvas_dirty_area, &Copy);


    if (canvas->gx_canvas_display)
    {
        format = canvas->gx_canvas_display->gx_display_color_format;
    }

    switch (format)
    {
    case GX_COLOR_FORMAT_565RGB:
        _gx_copy_canvas_to_buffer_565rgb(gx_canvas_memory, canvas, &Copy);
        break;

    case GX_COLOR_FORMAT_24XRGB:
    case GX_COLOR_FORMAT_32ARGB:
        _gx_copy_canvas_to_buffer_24xrgb(gx_canvas_memory, canvas, &Copy);
        break;

    case GX_COLOR_FORMAT_8BIT_PALETTE:
        _gx_copy_canvas_to_buffer_8bit_palette(gx_canvas_memory, canvas, &Copy);
        break;

    case GX_COLOR_FORMAT_4444ARGB:
        _gx_copy_canvas_to_buffer_4444argb(gx_canvas_memory, canvas, &Copy);
        break;

    case GX_COLOR_FORMAT_1555XRGB:
        _gx_copy_canvas_to_buffer_1555xrgb(gx_canvas_memory, canvas, &Copy);
        break;

    case GX_COLOR_FORMAT_8BIT_PACKED_PIXEL:
        _gx_copy_canvas_to_buffer_332rgb(gx_canvas_memory, canvas, &Copy);
        break;

    case GX_COLOR_FORMAT_4BIT_GRAY:
        _gx_copy_canvas_to_buffer_4bit_grayscale(gx_canvas_memory, canvas, &Copy);
        break;

    case GX_COLOR_FORMAT_MONOCHROME:
        _gx_copy_canvas_to_buffer_monochrome(gx_canvas_memory, canvas, &Copy);
        break;

    default:
        break;
    }


    // these lines were just for testing
    //block_fill(0, width, 20, 0xffffff);
    //block_fill(40, width, 20, 0xff0000);
    //block_fill(80, width, 20, 0x00ff00);
    //block_fill(120, width, 20, 0x0000ff);

    XPutImage(gx_x11_display, gx_x11_window, gx_x11_context, gx_x11_image,
        0, 0, 0, 0, width, height);
    
    XFlush(gx_x11_display);
}


static linux_display_driver_8bit_palette_set(GX_DISPLAY *display, GX_COLOR *palette, INT count)
{
    display->gx_display_palette = palette;
    display->gx_display_palette_size = count;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void _gx_x11_graphics_driver_setup(GX_DISPLAY *display)
{
    XGCValues tValues;    
    INT iX = 0;
    INT iY = 0;
    UINT wBorderWidth = 0;
    INT width = display->gx_display_width;
    INT height = display->gx_display_height;
    GX_RECTANGLE tWinRect;
 
    tWinRect.gx_rectangle_left = 0;
    tWinRect.gx_rectangle_top = 0;
    tWinRect.gx_rectangle_right = width - 1;
    tWinRect.gx_rectangle_bottom = height - 1;
 
    const char* pWindowName = "GUIX Development Window";
    const char* pIconName = "GXDevWin";
    char* gx_x11_displayName = NULL;
    XSizeHints tSizeHints;

    // Determine if the X server can support multi-threaded
    // operation
    //debug("Calling XInitThreads");

    Status tXStatus = XInitThreads();
    if(!tXStatus)
    {
        debug("\nThe X Server does not support threading. Bailing...\n");
        exit(-1);
    }
 
     // Connect to the X server
    if((gx_x11_display = XOpenDisplay(gx_x11_displayName)) == NULL)
    {
        // didn't connect
        debug("\nFailed to connect to the X server. Bailing...\n");
        exit(-1);
    }

    // Get the screen from the display
    gx_x11_screen = DefaultScreen(gx_x11_display);

    // Create the development frame window

    XVisualInfo xvi, *xviList;
    INT ixviNum;
    XSetWindowAttributes xswa;

    xvi.screen = gx_x11_screen;
    xvi.depth = DefaultDepth(gx_x11_display, gx_x11_screen);
    printf("xvi.depth: %d\n", xvi.depth);

    //xvi.depth = 24;
    //xvi.depth = 32;

    xviList = XGetVisualInfo(gx_x11_display, VisualScreenMask | VisualDepthMask,
            &xvi, &ixviNum);

    if (!xviList)
    {
        xvi.depth = DefaultDepth(gx_x11_display, gx_x11_screen);
        xviList = XGetVisualInfo(gx_x11_display, VisualDepthMask, &xvi, &ixviNum);
    }

    memcpy(&xvi, xviList, sizeof(XVisualInfo)); /* Use case of memcpy is verified. */
    XFree(xviList);

    xswa.border_pixel = BlackPixel(gx_x11_display, gx_x11_screen);
    xswa.background_pixel = BlackPixel(gx_x11_display, gx_x11_screen);

    gx_x11_window = XCreateWindow(gx_x11_display, RootWindow(gx_x11_display, gx_x11_screen),
            iX, iY, width, height, wBorderWidth, xvi.depth, CopyFromParent,
            xvi.visual, CWBorderPixel | CWBackPixel, &xswa);

    // Tell the window manager that we would like to participate
    // in WM_PROTOCOLS messaging

    Atom tAtomDelete = XInternAtom(gx_x11_display, "WM_DELETE_WINDOW", GX_FALSE);
    if(!XSetWMProtocols(gx_x11_display, gx_x11_window, &tAtomDelete, 1))
    {
        debug("\ngx_x11_start: Unable to set WM_DELETE_WINDOW procotol with the X server");
    }

    // Setup hints for the wm
 
    tSizeHints.flags = PSize | PMinSize | PMaxSize;

    // x, y, width and height are obsolete, but we will leave them
    // in for compatibility
    tSizeHints.x = iX;
    tSizeHints.y = iY;
    tSizeHints.width = width;
    tSizeHints.height = height;

    tSizeHints.min_width = width;
    tSizeHints.min_height = height;

    tSizeHints.max_width = width;
    tSizeHints.max_height = height;
 
    tSizeHints.base_width = width;
    tSizeHints.base_height = height;
 

    // Set the properties for the wm
    XmbSetWMProperties(gx_x11_display, gx_x11_window, pWindowName, pIconName, GX_NULL,
                    0, &tSizeHints, NULL, NULL);

    // We want to know about these events
 
    iEventMask |= ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
    iEventMask |= KeyPressMask | KeyReleaseMask;
    XSelectInput(gx_x11_display, gx_x11_window, iEventMask);

    gx_allocate_x_canvas(width, height);

    XMapWindow(gx_x11_display, gx_x11_window);

    gx_x11_context = XCreateGC(gx_x11_display, gx_x11_window, 0, &tValues);
    XSetForeground(gx_x11_display, gx_x11_context, WhitePixel(gx_x11_display, gx_x11_screen));
    XSetBackground(gx_x11_display, gx_x11_context, WhitePixel(gx_x11_display, gx_x11_screen));

    XSync(gx_x11_display, 0);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
UINT gx_x11_graphics_driver_setup_24xrgb(GX_DISPLAY *display) 
{
    _gx_display_driver_24xrgb_setup(display, GX_NULL, gx_x11_buffer_toggle);
    _gx_x11_graphics_driver_setup(display);

    return GX_SUCCESS;
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
UINT gx_x11_graphics_driver_setup_565rgb(GX_DISPLAY *display)
{
    _gx_display_driver_565rgb_setup(display, GX_NULL, gx_x11_buffer_toggle);
    _gx_x11_graphics_driver_setup(display);

    return GX_SUCCESS;
}
/*---------------------------------------------------------------------------*/
/* FIXME later
UINT gx_x11_graphics_driver_setup_1555rgb(GX_DISPLAY *display)
{
    _gx_display_driver_1555xrgb_setup(display, GX_NULL, gx_x11_buffer_toggle);
    _gx_x11_graphics_driver_setup(display);

    return GX_SUCCESS;
}
---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
UINT gx_x11_graphics_driver_setup_4444argb(GX_DISPLAY *display)
{
    _gx_display_driver_4444argb_setup(display, GX_NULL, gx_x11_buffer_toggle);
    _gx_x11_graphics_driver_setup(display);

    return GX_SUCCESS;
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
UINT gx_x11_graphics_driver_setup_8bit_palette(GX_DISPLAY *display)
{
    _gx_display_driver_8bit_palette_setup(display, GX_NULL, gx_x11_buffer_toggle);
    display -> gx_display_driver_palette_set = linux_display_driver_8bit_palette_set;

    _gx_x11_graphics_driver_setup(display);

    return GX_SUCCESS;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
UINT gx_x11_graphics_driver_setup_332rgb(GX_DISPLAY *display)
{
    _gx_display_driver_332rgb_setup(display, GX_NULL, gx_x11_buffer_toggle);
    _gx_x11_graphics_driver_setup(display);

    return GX_SUCCESS;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
UINT gx_x11_graphics_driver_setup_4bpp_grayscale(GX_DISPLAY *display)
{
    _gx_display_driver_4bpp_grayscale_setup(display, GX_NULL, gx_x11_buffer_toggle);
    _gx_x11_graphics_driver_setup(display);

    return GX_SUCCESS;
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
UINT gx_x11_graphics_driver_setup_monochrome(GX_DISPLAY *display)
{
    _gx_display_driver_monochrome_setup(display, GX_NULL, gx_x11_buffer_toggle);
    _gx_x11_graphics_driver_setup(display);

    return GX_SUCCESS;
}

