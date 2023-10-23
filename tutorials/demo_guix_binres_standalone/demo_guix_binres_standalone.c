/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <string.h>
#include "gx_api.h"

#include "demo_guix_binres_standalone_resources.h"
#include "demo_guix_binres_standalone_specifications.h"

#define FONT_BUFFER_SIZE 5120
#define GUIX_POOL_SIZE (640*480)

/* Define variables.  */
GX_WINDOW_ROOT* root;
TX_THREAD       demo_thread;
ULONG           demo_thread_stack[1024];
TX_BYTE_POOL    memory_pool;
UCHAR           guix_pool_memory[GUIX_POOL_SIZE];
GX_PIXELMAP     pixelmap_red_apple;
GX_UBYTE        font_vera_buffer[FONT_BUFFER_SIZE];
VOID           *root_address_pixelmap_red_apple = GX_NULL;
VOID           *root_address_font_vera = GX_NULL;

/* Define prototypes.   */
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);
VOID demo_thread_entry(ULONG thread_input);
VOID *load_binary_resource_data_to_ram(GX_CHAR* pathname);
UINT load_standalone_resources();

/************************************************************************************/
/*  Program entry.                                                                  */
/************************************************************************************/
int main(int argc, char ** argv)
{
    tx_kernel_enter();
    return(0);
}

/************************************************************************************/
/*  User defined memory allocate function.                                          */
/************************************************************************************/
VOID* memory_allocate(ULONG size)
{
    VOID* memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }

    return NULL;
}

/************************************************************************************/
/*  User defined memory free function.                                              */
/************************************************************************************/
VOID memory_free(VOID* mem)
{
    tx_byte_release(mem);
}

/************************************************************************************/
/*  Define system initialization.                                                   */
/************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{
    tx_thread_create(&demo_thread, "Demo Thread", demo_thread_entry, 0,
        demo_thread_stack, sizeof(demo_thread_stack),
        GX_SYSTEM_THREAD_PRIORITY + 1,
        GX_SYSTEM_THREAD_PRIORITY + 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}

/************************************************************************************/
/*  Demo thread entry.                                                              */
/************************************************************************************/
VOID demo_thread_entry(ULONG thread_input)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    /* Create byte pool. */
    tx_byte_pool_create(&memory_pool, "guix_pool_memory", guix_pool_memory, GUIX_POOL_SIZE);

    /* Configure display.  */
    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_565rgb,
                                LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root);

    /* Create the main screen.  */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET*)root, GX_NULL);

    /* Load standalone resources.  */
    if (load_standalone_resources() != GX_SUCCESS)
    {
        return;
    }

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* Start GUIX thread.  */
    gx_system_start();
}

/************************************************************************************/
/*  Load binary resource data into ram.                                             */
/*  This is an example of how an application might load a binary resource data      */
/*  file to RAM (or FLASH) from a local filesystem. In this case we are using       */
/*  generic ANSI file I/O to read the resource data file.                           */
/*                                                                                  */
/*  Note that for a typical embedded target, this step is not required. For most    */
/*  targets, the binary resource data has been directly programmed to FLASH rather  */
/*  than saved in a filesytem.                                                      */
/************************************************************************************/
VOID *load_binary_resource_data_to_ram(GX_CHAR *pathname)
{
    UCHAR *address = GX_NULL;

    /* If generated resource is stored in a FAT filesystem, it must be
       loaded into memory before it can be used. This memory could be
       RAM or FLASH, but for the purpose of this example will simply
       read the file to RAM. */
    FILE *p_file;
    size_t total_length;

    p_file = fopen(pathname, "rb");

    if (p_file)
    {
        fseek(p_file, 0, SEEK_END);
        total_length = ftell(p_file);
        fseek(p_file, SEEK_SET, SEEK_SET);

        address = memory_allocate(total_length);

        if (address)
        {
            fread(address, 1, total_length, p_file);
        }

        fclose(p_file);
    }

    return address;
}

/************************************************************************************/
/*  Load font and pixelmap resources from standalone binary data.                   */
/************************************************************************************/
UINT load_standalone_resources()
{
    UINT status;
    ULONG buffer_size;

    /* Load binary file into ram.  */
    root_address_pixelmap_red_apple = load_binary_resource_data_to_ram("..\\..\\pixelmap_red_apple.bin");

    if (!root_address_pixelmap_red_apple)
    {
        return GX_FAILURE;
    }

    /* Load binary file into ram.  */
    root_address_font_vera = load_binary_resource_data_to_ram("..\\..\\font_vera.bin");

    if (!root_address_font_vera)
    {
        return GX_FAILURE;
    }

    /* Load a pixelmap from the standalone binary data.  */
    status = gx_binres_pixelmap_load(root_address_pixelmap_red_apple, 0, &pixelmap_red_apple);

    if (status != GX_SUCCESS)
    {
        return status;
    }

    buffer_size = FONT_BUFFER_SIZE;

    /* Load a font from the standalone binary data.  */
    status = gx_binres_font_load(root_address_font_vera, 0, font_vera_buffer, &buffer_size);

    if (status != GX_SUCCESS)
    {
        return status;
    }

    return status;
}

/************************************************************************************/
/*  Define the custom drawing function for the "pixelmap_window" widget.            */
/************************************************************************************/
VOID pixelmap_window_draw(GX_WINDOW *window)
{
    GX_RECTANGLE *client = &window->gx_window_client;;

    /* Draw the window's background.  */
    gx_window_background_draw(window);

    /* Draw a pixlemap.  */
    gx_canvas_pixelmap_draw(client->gx_rectangle_left, client->gx_rectangle_top, &pixelmap_red_apple);
}

/************************************************************************************/
/*  Define the custom drawing function for the "prompt" widget.                     */
/************************************************************************************/
VOID text_draw(GX_PROMPT *prompt)
{
    GX_BRUSH* brush;
    GX_RECTANGLE* size = &prompt->gx_widget_size;
    GX_STRING string;

    /* Draw the promt's background.  */
    gx_widget_background_draw(prompt);

    /* Retrieve the context brush.  */
    gx_context_brush_get(&brush);

    /* Set text font.  */
    brush->gx_brush_font = (GX_FONT*)font_vera_buffer;

    /* Set text color.  */
    gx_context_line_color_set(GX_COLOR_ID_TEXT);

    /* Get text.  */
    gx_context_string_get_ext(prompt->gx_prompt_text_id, &string);
    
    /* Draw text.  */
    gx_canvas_aligned_text_draw(&string, size, GX_STYLE_TEXT_CENTER);
}