/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "template_resources.h"
#include "template_specifications.h"

/* Define the ThreadX demo thread control block and stack.  */

GX_WINDOW_ROOT    *root;

TX_BYTE_POOL       memory_pool;

#define            SCRATCHPAD_PIXELS (PRIMARY_X_RESOLUTION * PRIMARY_X_RESOLUTION)
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

extern GX_CONST GX_STUDIO_WIDGET child_template_screen_dynamic_child_template_define;

/* Define prototypes.   */
VOID start_guix(VOID);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
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
    /* Create the main demo thread.  */

    /* create byte pool for rotate to use */
    tx_byte_pool_create(&memory_pool, "scratchpad", scratchpad,
                        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));

    start_guix();
}

VOID start_guix(void)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    gx_studio_display_configure(PRIMARY, win32_graphics_driver_setup_24xrgb,
        LANGUAGE_ENGLISH, PRIMARY_THEME_1, &root);

    gx_studio_named_widget_create("template_screen", (GX_WIDGET *)root, GX_NULL);

    gx_studio_named_widget_create("child_template_screen", GX_NULL, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start the GUIX thread */
    gx_system_start();
}