/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"

#include "thai_glyph_shaping_resources.h"
#include "thai_glyph_shaping_specifications.h"

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;
TX_BYTE_POOL       mem_pool;

GX_THEME          *theme = GX_NULL;
GX_STRING        **language_table = GX_NULL;
GX_UBYTE          *binres_root_address = GX_NULL;
extern GX_CONST GX_STRING *display_1_language_table[];

#define            SCRATCHPAD_PIXELS (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION * 2)
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
UINT load_language_table();

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&mem_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }

    return NULL;
}

VOID memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* create byte pool for rotate to use */
    tx_byte_pool_create(&mem_pool, "scratchpad", scratchpad,
        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));

    /* Create the main demo thread.  */
    tx_thread_create(&demo_thread, "GUIX Demo Thread", demo_thread_entry, 
                     0,  demo_thread_stack, sizeof(demo_thread_stack), 
                     1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}

VOID  demo_thread_entry(ULONG thread_input)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
    gx_system_bidi_text_enable();
#endif

#if defined(GX_THAI_GLYPH_SHAPING_SUPPORT)
    gx_system_text_render_style_set(GX_TEXT_RENDER_THAI_GLYPH_SHAPING);
#endif

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb, 
                                0, 0, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

UINT main_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_SWITCH_LANGUAGE, GX_EVENT_TOGGLE_OFF):
        if (!language_table)
        {
            load_language_table("..//thai_string_res.bin");
        }

        if (language_table)
        {
            /* Set language table. */
            gx_display_language_table_set_ext(root->gx_window_root_canvas->gx_canvas_display, (GX_CONST GX_STRING **)language_table, 1, DISPLAY_1_STRING_TABLE_SIZE);

            /* Set active language. */
            gx_system_active_language_set(0);
        }
        break;

    case GX_SIGNAL(ID_SWITCH_LANGUAGE, GX_EVENT_TOGGLE_ON):
        gx_display_language_table_set_ext(root->gx_window_root_canvas->gx_canvas_display, display_1_language_table, DISPLAY_1_LANGUAGE_TABLE_SIZE, DISPLAY_1_STRING_TABLE_SIZE);
        gx_system_active_language_set(LANGUAGE_ENGLISH);
        break;
    }

    return gx_window_event_process(window, event_ptr);
}

UINT load_language_table(char *pathname)
{
    /* If generated resource is stored in a FAT filesystem, it must be
    loaded into memory before it can be used. This memory could be
    RAM or FLASH, but for the purpose of this example will simply
    read the file to RAM. */
    FILE *p_file;
    size_t total_length;
    UINT  status = GX_FAILURE;

    p_file = fopen(pathname, "rb");

    if (p_file)
    {
        fseek(p_file, 0, SEEK_END);
        total_length = ftell(p_file);
        fseek(p_file, SEEK_SET, SEEK_SET);

        binres_root_address = memory_allocate(total_length);
        fread(binres_root_address, 1, total_length, p_file);

        fclose(p_file);

        /* Load language table from binary data memory. */
        status = gx_binres_language_table_load_ext(binres_root_address,     /* Address of binary resource data. */
            &language_table); /* Loaded language table that contains all languages in the specified binary resource data. */
    }

    return status;
}
