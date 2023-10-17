/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "two_displays_binres_MAIN_DISPLAY_resources.h"
#include "two_displays_binres_SECONDARY_resources.h"
#include "two_displays_binres_specifications.h"

#define GUIX_POOL_SIZE (MAIN_DISPLAY_X_RESOLUTION * MAIN_DISPLAY_Y_RESOLUTION)

/* Define variables.  */
GX_WINDOW_ROOT    *root1;
GX_WINDOW_ROOT    *root2;

TX_BYTE_POOL       memory_pool;
UCHAR              guix_pool_memory[GUIX_POOL_SIZE];
GX_THEME          *theme[2];
GX_STRING        **language_table[2];


/* Define prototypes.   */
VOID start_guix(VOID);
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);
VOID load_resource();

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID tx_application_define(void *first_unused_memory)
{
    /* Create the main demo thread.  */

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

VOID start_guix(void)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    /* Set memory alloc/free functions. */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    /* Create byte pool. */
    tx_byte_pool_create(&memory_pool, "guix_pool_memory", guix_pool_memory, GUIX_POOL_SIZE);

    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_565rgb, 
                                MAIN_DISPLAY_LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root1);

    gx_studio_display_configure(SECONDARY, win32_graphics_driver_setup_565rgb,
                                MAIN_DISPLAY_LANGUAGE_ENGLISH, SECONDARY_THEME_1, &root2);

    load_resource();

    /* create the windows screen */
    gx_studio_named_widget_create("MAIN_DISPLAY_main_window", (GX_WIDGET *) root1, GX_NULL);

    gx_studio_named_widget_create("SECONDARY_main_window", (GX_WIDGET *) root2, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root1);
    gx_widget_show(root2);

    /* start the GUIX thread */
    gx_system_start();
}

static GX_UBYTE *load_binary_resource_data_to_ram(UCHAR *binres_pathname)
{
    UCHAR* address = GX_NULL;

    /* If generated resource is stored in a FAT filesystem, it must be
       loaded into memory before it can be used. This memory could be
       RAM or FLASH, but for the purpose of this example will simply
       read the file to RAM. */
    FILE* p_file;
    size_t total_length;

    p_file = fopen(binres_pathname, "rb");

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

static UINT load_theme(GX_UBYTE* root_address, GX_DISPLAY *display, GX_THEME *theme)
{
    UINT  status = GX_SUCCESS;

    /* Load a theme from binary data memory. */
    status = gx_binres_theme_load(root_address,/* Address of binary resource data. */
                                  0,           /* Theme identification, 0, 1, 2: 1th, 2nd, 3rd theme in the binary resource data. */
                                  &theme);     /* Loaded theme. */

    if (status == GX_SUCCESS)
    {
        /* Install themes to specified display. */
        status = gx_display_theme_install(display, theme);
    }

    return status;
}

static UINT load_language_table(GX_UBYTE* root_address, GX_DISPLAY *display, GX_STRING **language_table)
{
    UINT  status = GX_SUCCESS;

    /* Load language table from binary data memory. */
    status = gx_binres_language_table_load_ext(root_address,     /* Address of binary resource data. */
                                               &language_table); /* Loaded language table that contains all languages in the specified binary resource data. */

    if (language_table)
    {
        /* Set language table. */
        status = gx_display_language_table_set_ext(display, (GX_CONST GX_STRING**)language_table, MAIN_DISPLAY_LANGUAGE_TABLE_SIZE, MAIN_DISPLAY_STRING_TABLE_SIZE);
    }
    return status;
}

VOID load_resource()
{
    GX_UBYTE *address;
    UINT status;

    address = load_binary_resource_data_to_ram("..//..//two_displays_binres_MAIN_DISPLAY_resources.bin");

    if (address)
    {
        status = load_theme(address, root1->gx_window_root_canvas->gx_canvas_display, theme[0]);

        if (status == GX_SUCCESS)
        {
            status = load_language_table(address, root1->gx_window_root_canvas->gx_canvas_display, language_table[0]);
        }
    }

    address = load_binary_resource_data_to_ram("..//..//two_displays_binres_SECONDARY_resources.bin");

    if (address)
    {
        status = load_theme(address, root2->gx_window_root_canvas->gx_canvas_display, theme[1]);

        if (status == GX_SUCCESS)
        {
            status = load_language_table(address, root2->gx_window_root_canvas->gx_canvas_display, language_table[1]);
        }
    }
}


