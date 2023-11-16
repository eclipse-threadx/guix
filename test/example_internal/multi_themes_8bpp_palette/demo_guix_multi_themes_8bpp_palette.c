/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "multi_themes_8bpp_palette_resources.h"
#include "multi_themes_8bpp_palette_specifications.h"

#define MAX_PATH_LENGTH 255

GX_WINDOW *pHelloScreen;

GX_WINDOW_ROOT    *root;

TX_BYTE_POOL       memory_pool;
#define            SCRATCHPAD_PIXELS 300 * 1024
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

/* Define prototypes.   */
VOID  start_guix(VOID);
extern UINT win32_graphics_driver_setup_8bit_palette (GX_DISPLAY *display);

UINT load_binres_theme(GX_UBYTE *pathname, INT theme_id);
UINT load_binary_resource_data_to_ram();

extern GX_STUDIO_DISPLAY_INFO multi_themes_8bpp_palette_display_table[];
extern GX_CONST GX_THEME  *main_display_theme_table[];
extern GX_CONST GX_STRING *main_display_language_table[];

GX_THEME *theme = GX_NULL;
GX_UBYTE ***language_table = GX_NULL;

GX_UBYTE *binres_root_address = GX_NULL;
GX_CHAR binres_pathname[] = "multi_themes_8bpp_palette_resources.bin";

VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////
void memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

//////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

//////////////////////////////////////////////////////////////////////////////////////
VOID tx_application_define(void *first_unused_memory)
{
    /* create byte pool*/
    tx_byte_pool_create(&memory_pool, "scratchpad", scratchpad,
        SCRATCHPAD_PIXELS * sizeof(GX_COLOR));


    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_system_memory_allocator_set(memory_allocate, memory_free);
    load_binary_resource_data_to_ram();

    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_8bit_palette,
        LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root);

    /* create the hello world screen */
    gx_studio_named_widget_create("simple_window", (GX_WIDGET *)root, (GX_WIDGET **)&pHelloScreen);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start GUIX thread */
    gx_system_start();
}

//////////////////////////////////////////////////////////////////////////////////////
UINT simple_window_event_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(widget, event_ptr);
        gx_window_client_scroll(&simple_window.simple_window_scroll_frame, 0, 1);
        break;

    case GX_SIGNAL(ID_THEME_1, GX_EVENT_RADIO_SELECT):
        gx_display_theme_install(multi_themes_8bpp_palette_display_table[MAIN_DISPLAY].display, main_display_theme_table[MAIN_DISPLAY_THEME_1]);
        break;

    case GX_SIGNAL(ID_THEME_2, GX_EVENT_RADIO_SELECT):
        gx_display_theme_install(multi_themes_8bpp_palette_display_table[MAIN_DISPLAY].display, main_display_theme_table[MAIN_DISPLAY_THEME_2]);
        break;

    case GX_SIGNAL(ID_THEME_3, GX_EVENT_RADIO_SELECT):
        gx_display_theme_install(multi_themes_8bpp_palette_display_table[MAIN_DISPLAY].display, main_display_theme_table[MAIN_DISPLAY_THEME_3]);
        break;

    case GX_SIGNAL(ID_THEME_4, GX_EVENT_RADIO_SELECT):
        load_binres_theme(binres_root_address, 0);
        break;

    case GX_SIGNAL(ID_THEME_5, GX_EVENT_RADIO_SELECT):
        load_binres_theme(binres_root_address, 1);
        break;

    case GX_SIGNAL(ID_ENGLISH, GX_EVENT_RADIO_SELECT):
        gx_display_language_table_set_ext(root->gx_window_root_canvas->gx_canvas_display, main_display_language_table, MAIN_DISPLAY_LANGUAGE_TABLE_SIZE, MAIN_DISPLAY_STRING_TABLE_SIZE);
        gx_system_active_language_set(LANGUAGE_ENGLISH);
        break;

    case GX_SIGNAL(ID_CHINESE, GX_EVENT_RADIO_SELECT):
        gx_display_language_table_set_ext(root->gx_window_root_canvas->gx_canvas_display, main_display_language_table, MAIN_DISPLAY_LANGUAGE_TABLE_SIZE, MAIN_DISPLAY_STRING_TABLE_SIZE);
        gx_system_active_language_set(LANGUAGE_CHINESE);
        break;

    case GX_SIGNAL(ID_JAPANESE, GX_EVENT_RADIO_SELECT):
        gx_system_active_language_set(LANGUAGE_JAPANESE);
        break;

    case GX_SIGNAL(ID_SPANISH, GX_EVENT_RADIO_SELECT):
        gx_system_active_language_set(LANGUAGE_SPANISH);
        break;
    }

    return gx_window_event_process(widget, event_ptr);
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

//////////////////////////////////////////////////////////////////////////////////////
UINT load_binary_resource_data_to_ram()
{
    FILE *p_file;
    size_t total_length;
    size_t result;
    char pathname[MAX_PATH_LENGTH + 1];
    int pathlen;

    pathlen = extract_path(__FILE__, pathname);
    memcpy(pathname + pathlen, binres_pathname, sizeof(binres_pathname));

    p_file = fopen(pathname, "rb");

    if (!p_file)
    {
        printf("Cannot open file: %s\n", pathname);
        return GX_FAILURE;
    }

    fseek(p_file, 0, SEEK_END);
    total_length = ftell(p_file);
    fseek(p_file, SEEK_SET, SEEK_SET);

    binres_root_address = memory_allocate(total_length);
    result = fread(binres_root_address, 1, total_length, p_file);

    fclose(p_file);

    if (result != total_length)
    {
        return GX_FAILURE;
    }

    return GX_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////
UINT load_binres_theme(GX_UBYTE *root_address, INT theme_id)
{
    UINT  status = GX_SUCCESS;

    if (theme)
    {
        memory_free(theme);
        theme = GX_NULL;
    }

    status = gx_binres_theme_load(root_address, theme_id, &theme);

    if (theme)
    {
        gx_display_theme_install(multi_themes_8bpp_palette_display_table[MAIN_DISPLAY].display, theme);
    }

    return status;
}
