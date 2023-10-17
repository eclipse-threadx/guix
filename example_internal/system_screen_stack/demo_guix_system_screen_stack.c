/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"
#include "system_screen_stack_resources.h"
#include "system_screen_stack_specifications.h"

/* Define the ThreadX demo thread control block and stack.  */

#define SYSTEM_SCREEN_STACK_SIZE  20
#define MEMORY_SIZE (MAIN_DISPLAY_X_RESOLUTION * MAIN_DISPLAY_Y_RESOLUTION)

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;
TX_BYTE_POOL       memory_pool;
GX_COLOR           memory_buffer[MEMORY_SIZE];
GX_WIDGET         *system_screen_stack_memory[SYSTEM_SCREEN_STACK_SIZE];

/* Define prototypes.   */
VOID  start_guix(VOID);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}


VOID tx_application_define(void *first_unused_memory)
{
    /* create byte pool for rotate to use */
    tx_byte_pool_create(&memory_pool, "memory_pool", memory_buffer, MEMORY_SIZE * sizeof(GX_COLOR));

    /* Create the main demo thread.  */
    start_guix();
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

VOID  start_guix(VOID)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root);

    /* create the top menu screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, GX_NULL);

    gx_studio_named_widget_create("bookshelf_screen", GX_NULL, GX_NULL);

    gx_studio_named_widget_create("book_screen", GX_NULL, GX_NULL);

    gx_studio_named_widget_create("language_screen", GX_NULL, GX_NULL);

    /* Create system screen stack. */
    gx_system_screen_stack_create(system_screen_stack_memory, sizeof(GX_WIDGET *)* SYSTEM_SCREEN_STACK_SIZE);

    /* Show the root window to make it and child screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}