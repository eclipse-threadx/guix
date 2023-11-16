/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <string.h>
#include "tx_api.h"
#include "gx_api.h"

#include "bidi_text_draw_32bpp_resources.h"
#include "bidi_text_draw_32bpp_specifications.h"

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
UINT bidi_character_test();
static GX_CHAR *text_buffer = GX_NULL;

GX_STRING **language_table = GX_NULL;
GX_UBYTE *binres_root_address = GX_NULL;
GX_CHAR binres_pathname[] = "bidi_text_draw_32bpp_resources.bin";

extern GX_CONST GX_STRING **display_1_language_table[];

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

void PopulateMLTexts(GX_CHAR *pathname)
{
    int file_size;
    int read_size;
    FILE *pFile = fopen(pathname, "r");
    GX_STRING text;

    if (text_buffer)
    {
        free(text_buffer);
        text_buffer = GX_NULL;
    }

    if (pFile)
    {
        fseek(pFile, 0, SEEK_END);
        file_size = ftell(pFile);
        text_buffer = (char *)malloc(file_size + 1);
        fseek(pFile, 0, SEEK_SET);
        read_size = fread(text_buffer, 1, file_size, pFile);

        text_buffer[read_size] = '\0';
        text.gx_string_ptr = text_buffer;
        text.gx_string_length = read_size;
        gx_multi_line_text_view_text_set_ext(&main_screen.main_screen_text_view, &text);
        fclose(pFile);
    }
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
    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb, 
                                0, 0, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *) root, GX_NULL);

    /* create the dynamic bidi screen */
    gx_studio_named_widget_create("dynamic_bidi_screen", GX_NULL, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
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
    int len = string_length_get(pathname, 255) - 1;

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
/*  binary mode resource: Load file into memory.                                    */
/*  srec mode resource: Load file into flash.                                       */
/************************************************************************************/
VOID *load_binary_resource_data_to_ram()
{
    UCHAR *address = GX_NULL;

    /* If generated resource is in binary mode,
    load binary file into memory first. */
    FILE *p_file;
    size_t total_length;
    char pathname[255];
    int  pathlen;

    pathlen = extract_path(__FILE__, pathname);

    memcpy(pathname + pathlen, binres_pathname, sizeof(binres_pathname));
    p_file = fopen(pathname, "rb");

    if (!p_file)
    {
        printf("Cannot open file: %s\n", pathname);
        return NULL;
    }

    fseek(p_file, 0, SEEK_END);
    total_length = ftell(p_file);
    fseek(p_file, SEEK_SET, SEEK_SET);

    address = memory_allocate(total_length);
    fread(address, 1, total_length, p_file);

    fclose(p_file);

    return address;
}

/************************************************************************************/
/*  Load language table.                                                            */
/************************************************************************************/
UINT load_language_table(GX_UBYTE *root_address)
{
    UINT  status = GX_SUCCESS;

    if (language_table)
    {
        memory_free(language_table);
        language_table = GX_NULL;
    }

    /* Load language table from binary data memory. */
    status = gx_binres_language_table_load_ext(root_address,     /* Address of binary resource data. */
        &language_table); /* Loaded language table that contains all languages in the specified binary resource data. */

    if (language_table)
    {
        /* Set language table. */
        gx_display_language_table_set_ext(root->gx_window_root_canvas->gx_canvas_display, (GX_CONST GX_STRING **)language_table, DISPLAY_1_LANGUAGE_TABLE_SIZE, DISPLAY_1_STRING_TABLE_SIZE);

        /* Set active language. */
        gx_system_active_language_set(LANGUAGE_HEBREW);
    }
    return status;
}

UINT main_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
    case GX_SIGNAL(ID_ENABLE_BIDI_TEXT, GX_EVENT_TOGGLE_ON):
        gx_system_bidi_text_enable();
        break;

    case GX_SIGNAL(ID_ENABLE_BIDI_TEXT, GX_EVENT_TOGGLE_OFF):
        gx_system_bidi_text_disable();
        break;
#endif

    case GX_SIGNAL(ID_HEWBREW_SONG, GX_EVENT_RADIO_SELECT):
        gx_multi_line_text_view_text_id_set(&main_screen.main_screen_text_view, GX_STRING_ID_HEWBREW_SONG);
        break;

    case GX_SIGNAL(ID_ENGLISH_TEXT, GX_EVENT_RADIO_SELECT):
        PopulateMLTexts("..//readme_guix_generic.txt");
        break;

    case GX_SIGNAL(ID_LANGUAGE_HEBREW, GX_EVENT_TOGGLE_ON):
        /* Set language to Hebrew. */
        gx_system_active_language_set(LANGUAGE_HEBREW);
        break;

    case GX_SIGNAL(ID_LANGUAGE_HEBREW, GX_EVENT_TOGGLE_OFF):
        /* Set language to English*/
        gx_system_active_language_set(LANGUAGE_ENGLISH);
        break;

    case GX_SIGNAL(ID_LOAD_BINARY, GX_EVENT_TOGGLE_ON):
        /* Load binary resource into memory buffer. */
        binres_root_address = load_binary_resource_data_to_ram();

        /* Load language table from binary resource. */
        load_language_table(binres_root_address);
        break;

    case GX_SIGNAL(ID_LOAD_BINARY, GX_EVENT_TOGGLE_OFF):
        /* Set language table. */
        gx_display_language_table_set_ext(root->gx_window_root_canvas->gx_canvas_display, (GX_CONST GX_STRING **)display_1_language_table, DISPLAY_1_LANGUAGE_TABLE_SIZE, DISPLAY_1_STRING_TABLE_SIZE);

        /* Set active language. */
        gx_system_active_language_set(LANGUAGE_ENGLISH);
        break;
    }

    return gx_window_event_process(window, event_ptr);
}

VOID numeric_prompt_format(GX_NUMERIC_PROMPT *prompt, INT value)
{
    GX_STRING string;
    INT len;

    gx_widget_string_get_ext(prompt, GX_STRING_ID_STRING_8BPP, &string);
    gx_utility_ltoa(value, prompt->gx_numeric_prompt_buffer, GX_NUMERIC_PROMPT_BUFFER_SIZE);
    len = strnlen(prompt->gx_numeric_prompt_buffer, GX_NUMERIC_PROMPT_BUFFER_SIZE - 1);
    strncpy(prompt->gx_numeric_prompt_buffer + len, string.gx_string_ptr, string.gx_string_length);
}

VOID numeric_pixelmap_prompt_format(GX_NUMERIC_PIXELMAP_PROMPT *pixelmap_prompt, INT value)
{
    GX_STRING string;
    INT len;

    gx_widget_string_get_ext(pixelmap_prompt, GX_STRING_ID_STRING_8BPP, &string);
    gx_utility_ltoa(value, pixelmap_prompt->gx_numeric_pixelmap_prompt_buffer, GX_NUMERIC_PROMPT_BUFFER_SIZE);
    len = strnlen(pixelmap_prompt->gx_numeric_pixelmap_prompt_buffer, GX_NUMERIC_PROMPT_BUFFER_SIZE - 1);
    strncpy(pixelmap_prompt->gx_numeric_pixelmap_prompt_buffer + len, string.gx_string_ptr, string.gx_string_length);
}

UINT numeric_scroll_row_format(GX_NUMERIC_SCROLL_WHEEL *wheel, INT row, GX_STRING *string)
{
    GX_STRING get_string;

    if (wheel->gx_numeric_scroll_wheel_end_val <= 10)
    {
        gx_widget_string_get_ext(wheel, GX_STRING_ID_ONE_LINE_TEXT, &get_string);
    }
    else
    {
        gx_widget_string_get_ext(wheel, GX_STRING_ID_ONE_LINE_TEXT_2, &get_string);
    }

    *string = get_string;
    return GX_SUCCESS;
}
