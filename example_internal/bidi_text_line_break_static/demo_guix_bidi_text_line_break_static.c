/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <string.h>
#include "tx_api.h"
#include "gx_api.h"

#include "bidi_text_line_break_static_resources.h"
#include "bidi_text_line_break_static_specifications.h"

#define MAX_PATH_LENGTH 255
#define GUIX_POOL_SIZE        (DISPLAY_1_X_RESOLUTION * DISPLAY_1_Y_RESOLUTION)

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];
GX_WINDOW_ROOT    *root;

UCHAR              guix_pool_memory[GUIX_POOL_SIZE];
TX_BYTE_POOL       memory_pool;
GX_THEME          *theme = GX_NULL;
GX_CHAR            binres_pathname[] = "bidi_text_line_break_static_resources.bin";

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID tx_application_define(void *first_unused_memory)
{
    /* Create the main demo thread.  */
    tx_thread_create(&demo_thread, "GUIX Demo Thread", demo_thread_entry, 
                     0,  demo_thread_stack, sizeof(demo_thread_stack), 
                     1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}

VOID *memory_allocate(ULONG size)
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

static UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length)
{
    UINT length = 0;

    if (input_string)
    {
        /* Traverse the string.  */
        for (length = 0; input_string[length]; length++)
        {
            /* Check if the string length is bigger than the max string length.  */
            if (length >= max_string_length)
            {
                break;
            }
        }
    }

    return length;
}

static int extract_path(char* pathname, char* path)
{
    int len = string_length_get(pathname, MAX_PATH_LENGTH) - 1;

    while (len >= 0)
    {
        if ((pathname[len] == '/') || pathname[len] == '\\')
        {
            memcpy(path, pathname, len + 1);
            len++;
            path[len] = '\0';
            break;
        }
        len--;
    }

    return len;
}

static VOID load_theme_2()
{
    UINT  status = GX_SUCCESS;
    UCHAR* address = GX_NULL;
    FILE *p_file;
    size_t total_length;
    char pathname[MAX_PATH_LENGTH + 1];
    int len;

    len = extract_path(__FILE__, pathname);

    memcpy(pathname + len, binres_pathname, sizeof(binres_pathname));

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

    if (theme)
    {
        memory_free(theme);
        theme = GX_NULL;
    }

    /* Load a theme from binary data memory. */
    status = gx_binres_theme_load(address,/* Address of binary resource data. */
        0,    /* Theme identification, 0, 1, 2: 1th, 2nd, 3rd theme in the binary resource data. */
        &theme);     /* Loaded theme. */

    if (status == GX_SUCCESS)
    {
        /* Install themes to specified display. */
        gx_display_theme_install(root->gx_window_root_canvas->gx_canvas_display, theme);
    }

    return status;
}

VOID  demo_thread_entry(ULONG thread_input)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    /* Create byte pool. */
    tx_byte_pool_create(&memory_pool, "guix_pool_memory", guix_pool_memory, GUIX_POOL_SIZE);

    /* Set memory alloc/free functions. */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb, 
                                0, 0, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET*)root, GX_NULL);

    /* create the second screen */
    gx_studio_named_widget_create("second_screen", GX_NULL, GX_NULL);

    //load_theme_2();

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}