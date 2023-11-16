/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <string.h>
#include "gx_api.h"

#include "standalone_binres_load_8bpp_resources.h"
#include "standalone_binres_load_8bpp_specifications.h"

#define GUIX_POOL_SIZE (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION * 2)
#define MAX_PATH_LENGTH 255

/* Define variables.  */
GX_WINDOW_ROOT *root;
TX_BYTE_POOL memory_pool;
UCHAR guix_pool_memory[GUIX_POOL_SIZE];
GX_PIXELMAP pixelmap_folder_1[3];
GX_PIXELMAP pixelmap_fish;

VOID *root_address_pixelmap_folder_1 = GX_NULL;
VOID *root_address_pixelmap_fish = GX_NULL;

/* Define prototypes.   */
VOID  start_guix(VOID);
extern UINT win32_graphics_driver_setup_8bit_palette(GX_DISPLAY *display);
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

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_8bit_palette,
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

VOID get_absolute_name(GX_CHAR* filename, GX_CHAR *pathname)
{
    INT len = strnlen(__FILE__, MAX_PATH_LENGTH);

    memcpy(pathname, __FILE__, len + 1);

    GX_CHAR *ptr = strrchr(pathname, '\\');

    if (!ptr)
    {
        ptr = strrchr(pathname, '/');
    }
    
    ptr[1] = '\0';
    strcat(pathname, filename);
}

VOID *load_binary_resource_data_to_ram(GX_CHAR *filename)
{


    /* If generated resource is stored in a FAT filesystem, it must be
       loaded into memory before it can be used. This memory could be
       RAM or FLASH, but for the purpose of this example will simply
       read the file to RAM. */
    FILE* p_file;
    size_t total_length;
    UCHAR* address = GX_NULL;
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

    root_address_pixelmap_folder_1 = load_binary_resource_data_to_ram("pixelmap_folder_1.bin");
    root_address_pixelmap_fish = load_binary_resource_data_to_ram("pixelmap_fish.bin");

    for (INT index = 0; index < 3; index++)
    {
        status = gx_binres_pixelmap_load(root_address_pixelmap_folder_1, index, &pixelmap_folder_1[index]);

        if (status != GX_SUCCESS)
        {
            return status;
        }
    }

    status = gx_binres_pixelmap_load(root_address_pixelmap_fish, 0, &pixelmap_fish);

    return status;
}

VOID pixelmap_draw(GX_WINDOW* window)
{
    GX_VALUE xpos;
    GX_VALUE ypos;
    GX_RECTANGLE *size = &window->gx_widget_size;

    gx_window_background_draw(window);

    xpos = size->gx_rectangle_left;
    ypos = size->gx_rectangle_top;
    gx_canvas_pixelmap_draw(xpos, ypos, &pixelmap_folder_1[0]);

    xpos = size->gx_rectangle_right - (pixelmap_folder_1[1].gx_pixelmap_width);
    gx_canvas_pixelmap_draw(xpos, ypos, &pixelmap_folder_1[1]);

    xpos = size->gx_rectangle_left;
    ypos = size->gx_rectangle_bottom - (pixelmap_folder_1[2].gx_pixelmap_height);
    gx_canvas_pixelmap_draw(xpos, ypos, &pixelmap_folder_1[2]);

    xpos = size->gx_rectangle_right - (pixelmap_fish.gx_pixelmap_width);
    gx_canvas_pixelmap_draw(xpos, ypos, &pixelmap_fish);

    gx_widget_children_draw(window);
}