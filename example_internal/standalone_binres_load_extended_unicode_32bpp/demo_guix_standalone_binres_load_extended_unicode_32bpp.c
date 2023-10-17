/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <string.h>
#include "gx_api.h"

#include "standalone_binres_load_extended_unicode_32bpp_resources.h"
#include "standalone_binres_load_extended_unicode_32bpp_specifications.h"

#define GUIX_POOL_SIZE (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION * 8)
#define MAX_PATH_LENGTH 255
#define FONT_BUFFER_SIZE 4096

/* Define variables.  */
GX_WINDOW_ROOT* root;
TX_BYTE_POOL    memory_pool;
UCHAR           guix_pool_memory[GUIX_POOL_SIZE];

/* Define prototypes.   */
VOID  start_guix(VOID);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
VOID* load_binary_resource_data_to_ram(GX_CHAR* pathname);
UINT load_standalone_pixelmaps();


int main(int argc, char ** argv)
{
    tx_kernel_enter();
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{
    start_guix();
}

VOID* memory_allocate(ULONG size)
{
    VOID* memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }

    return NULL;
}

VOID memory_free(VOID* mem)
{
    tx_byte_release(mem);
}

VOID start_guix(VOID)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    /* Create byte pool. */
    tx_byte_pool_create(&memory_pool, "guix_pool_memory", guix_pool_memory, GUIX_POOL_SIZE);

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb,
        LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET*)root, GX_NULL);

    if (load_standalone_pixelmaps() != GX_SUCCESS)
    {
        return;
    }

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start GUIX thread */
    gx_system_start();
}

GX_PIXELMAP pixelmaps[2];
GX_UBYTE font_1bpp_buffer[FONT_BUFFER_SIZE];
GX_UBYTE font_4bpp_buffer[FONT_BUFFER_SIZE];
GX_UBYTE font_8bpp_buffer[FONT_BUFFER_SIZE];

VOID* root_address_pixelmaps = GX_NULL;
VOID* root_address_font_1bpp = GX_NULL;
VOID* root_address_font_4bpp = GX_NULL;
VOID* root_address_font_8bpp = GX_NULL;

VOID get_absolute_name(GX_CHAR* filename, GX_CHAR *pathname)
{
    INT len = strnlen(__FILE__, MAX_PATH_LENGTH);

    memcpy(pathname, __FILE__, len + 1);

    GX_CHAR* ptr = strrchr(pathname, '\\');

    if (!ptr)
    {
        ptr = strrchr(pathname, '/');
    }

    ptr[1] = '\0';
    strcat(pathname, filename);
}

VOID *load_binary_resource_data_to_ram(GX_CHAR *filename)
{
    UCHAR* address = GX_NULL;

    /* If generated resource is stored in a FAT filesystem, it must be
       loaded into memory before it can be used. This memory could be
       RAM or FLASH, but for the purpose of this example will simply
       read the file to RAM. */
    FILE* p_file;
    size_t total_length;
    GX_CHAR pathname[MAX_PATH_LENGTH + 1];

    get_absolute_name(filename, pathname);

    p_file = fopen(pathname, "rb");

    if (p_file)
    {
        fseek(p_file, 0, SEEK_END);
        total_length = ftell(p_file);
        fseek(p_file, SEEK_SET, SEEK_SET);

        address = memory_allocate(total_length);

        fread(address, 1, total_length, p_file);

        fclose(p_file);
    }

    return address;
}

UINT load_standalone_pixelmaps()
{
    UINT status;
    ULONG buffer_size;

    root_address_pixelmaps = load_binary_resource_data_to_ram("pixelmaps.bin");
    root_address_font_1bpp = load_binary_resource_data_to_ram("font_old_italic_1bpp.bin");
    root_address_font_4bpp = load_binary_resource_data_to_ram("font_old_italic_4bpp.bin");
    root_address_font_8bpp = load_binary_resource_data_to_ram("font_old_italic_8bpp.bin");

    for (INT index = 0; index < 2; index++)
    {
        status = gx_binres_pixelmap_load(root_address_pixelmaps, index, &pixelmaps[index]);

        if (status != GX_SUCCESS)
        {
            return status;
        }
    }

    buffer_size = FONT_BUFFER_SIZE;
    status = gx_binres_font_load(root_address_font_1bpp, 0, font_1bpp_buffer, &buffer_size);

    if (status != GX_SUCCESS)
    {
        return status;
    }

    status = gx_binres_font_load(root_address_font_4bpp, 0, font_4bpp_buffer, &buffer_size);

    if (status != GX_SUCCESS)
    {
        return status;
    }

    status = gx_binres_font_load(root_address_font_8bpp, 0, font_8bpp_buffer, &buffer_size);

    return status;
}

VOID text_draw(GX_PROMPT *prompt)
{
    GX_BRUSH* brush;
    GX_RECTANGLE* size = &prompt->gx_widget_size;
    GX_STRING string;

    gx_context_brush_get(&brush);
    gx_context_string_get_ext(prompt->gx_prompt_text_id, &string);

    switch (prompt->gx_widget_id)
    {
    case ID_TEXT_1BPP:
        brush->gx_brush_font = (GX_FONT*)font_1bpp_buffer;
        break;

    case ID_TEXT_4BPP:
        brush->gx_brush_font = (GX_FONT*)font_4bpp_buffer;
        break;

    case ID_TEXT_8BPP:
        brush->gx_brush_font = (GX_FONT*)font_1bpp_buffer;
        break;
    }

    gx_context_line_color_set(GX_COLOR_ID_TEXT);
    gx_canvas_aligned_text_draw(&string, size, GX_STYLE_TEXT_LEFT);
}

VOID pixelmap_draw(GX_WINDOW* window)
{
    GX_RECTANGLE *size = &window->gx_widget_size;

    gx_window_background_draw(window);

    switch (window->gx_widget_id)
    {
    case ID_APPLE:
        gx_canvas_pixelmap_draw(size->gx_rectangle_left, size->gx_rectangle_top, &pixelmaps[0]);
        break;

    case ID_FISH:
        gx_canvas_pixelmap_draw(size->gx_rectangle_left, size->gx_rectangle_top, &pixelmaps[1]);
        break;
    }

    gx_widget_children_draw(window);
}