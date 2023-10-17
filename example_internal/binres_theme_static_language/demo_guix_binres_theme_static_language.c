/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "binres_theme_static_language_resources.h"
#include "binres_theme_static_language_specifications.h"

#define GUIX_POOL_SIZE        (MAIN_DISPLAY_X_RESOLUTION * MAIN_DISPLAY_Y_RESOLUTION)
#define MAX_PATH_LENGTH 255

GX_WINDOW_ROOT               *root;
TX_BYTE_POOL                  memory_pool;
TX_THREAD                     demo_thread;
ULONG                         demo_thread_stack[1024];

/* pointers which will be initialized when the binary resources
   are installed
*/
GX_THEME   *theme = GX_NULL;
GX_UBYTE   *binres_root_address = GX_NULL;
GX_CHAR     binres_pathname[] = "binres_theme_static_language_resources.bin";

/* Define prototypes.   */
VOID demo_thread_entry(ULONG thread_input);

extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);
UCHAR guix_pool_memory[GUIX_POOL_SIZE];

/* local application function prototypes */

VOID *load_binary_resource_data_to_ram();
UINT load_theme(GX_UBYTE *root_address, INT theme_id);

/************************************************************************************/
/*  User defined memory allocate function.                                          */
/************************************************************************************/
VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }

    return NULL;
}

/************************************************************************************/
/*  User defined memory free function.                                              */
/************************************************************************************/
VOID memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

/************************************************************************************/
/*  Program entry.                                                                  */
/************************************************************************************/
int main(int argc, char ** argv)
{
    /* Enter the ThreadX kernel. */
    tx_kernel_enter();
    return(0);
}

/************************************************************************************/
/*  Define system initialization.                                                   */
/************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{
      tx_thread_create(&demo_thread, "Demo Thread", demo_thread_entry, 0,  
                       demo_thread_stack, sizeof(demo_thread_stack), 
                       1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}

VOID demo_thread_entry(ULONG thread_input)
{
      /* Initialize GUIX.  */
    gx_system_initialize();

    /* Set memory alloc/free functions. */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    /* Configure display. */
    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_565rgb,
        LANGUAGE_ENGLISH, 0, &root);

    /* Create byte pool. */
    tx_byte_pool_create(&memory_pool, "guix_pool_memory", guix_pool_memory, GUIX_POOL_SIZE);

    /* Load binary resource file into ram. */
    /* This suit the case that we generated a .bin format resource file. */
    binres_root_address = load_binary_resource_data_to_ram();

    /* Load theme table. */
    load_theme(binres_root_address, 0);

    /* Create the "simple_window" screen. */
    gx_studio_named_widget_create("simple_window", (GX_WIDGET *)root, GX_NULL);

    /* Show the root window to make "simple_window" screen visible.  */
    gx_widget_show(root);

    /* Start GUIX thread */
    gx_system_start();
}

/************************************************************************************/
/*  Override default event processing of "simple_window" to handle signals from     */
/*  child widgets.                                                                  */
/************************************************************************************/
UINT simple_window_event_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_THEME_1, GX_EVENT_RADIO_SELECT):
        /* Load first theme from generated binary resource data. */
        load_theme(binres_root_address, 0);
        break;

    case GX_SIGNAL(ID_THEME_2, GX_EVENT_RADIO_SELECT):
        /* Load second theme from generated binary resource data. */
        load_theme(binres_root_address, 1);
        break;

    case GX_SIGNAL(ID_ENGLISH, GX_EVENT_RADIO_SELECT):
        /* Set action language to English. */
        gx_system_active_language_set(LANGUAGE_ENGLISH);
        break;

    case GX_SIGNAL(ID_SPANISH, GX_EVENT_RADIO_SELECT):
        /* Set active language to Spanish. */
        gx_system_active_language_set(LANGUAGE_SPANISH);
        break;

    default:
        /* for all other events, allow the default event
           processor to run
        */
        return gx_window_event_process(widget, event_ptr);
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////////////////
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


/************************************************************************************/
/*  Load binary resource data into ram.                                             */
/*  This is an example of how an application might load a binary resource data
    file to RAM (or FLASH) from a local filesystem. In this case we are using generic ANSI
    file I/O to read the resource data file.

    Note that for a typical embedded target, this step is not required. For most
    targets, the binary resource data has been directly programmed to FLASH rather
    than saved in a filesytem.
*/
/************************************************************************************/
VOID *load_binary_resource_data_to_ram()
{
UCHAR *address = GX_NULL;

    /* If generated resource is stored in a FAT filesystem, it must be 
       loaded into memory before it can be used. This memory could be
       RAM or FLASH, but for the purpose of this example will simply 
       read the file to RAM. */
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

    return address;
}

/************************************************************************************/
/*  Load a theme.                                                                   */
/************************************************************************************/
UINT load_theme(GX_UBYTE *root_address, INT theme_id)
{
UINT  status = GX_SUCCESS;

    if (theme)
    {
        memory_free(theme);
        theme = GX_NULL;
    }

    /* Load a theme from binary data memory. */
    status = gx_binres_theme_load(root_address,/* Address of binary resource data. */
                                  theme_id,    /* Theme identification, 0, 1, 2: 1th, 2nd, 3rd theme in the binary resource data. */
                                  &theme);     /* Loaded theme. */

    if (status == GX_SUCCESS)
    {
        /* Install themes to specified display. */
        gx_display_theme_install(root->gx_window_root_canvas->gx_canvas_display, theme);
    }

    return status;
}
