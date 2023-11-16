/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"

#include "thai_glyph_shaping_runtime_resources.h"
#include "thai_glyph_shaping_runtime_specifications.h"

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;
INT                xshift;
INT                yshift;
INT                pre_vslider_value = 345;
INT                pre_hslider_value = 20;

TX_BYTE_POOL       mem_pool;

#define            SCRATCHPAD_PIXELS (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION * 2)
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
UINT normalization_test();

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