/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "kerning_glyph_draw_32bpp_screen_resources.h"
#include "kerning_glyph_draw_32bpp_screen_specifications.h"

#define MAX_PATH_LENGTH 255

GX_CHAR binres_pathname[] = "kerning_glyph_draw_32bpp_screen_resources.bin";

WINDOW_CONTROL_BLOCK *pMainWin;
GX_WINDOW    *pHorizontalClip;
GX_WINDOW    *pVerticalClip;
GX_SLIDER    *pHorizontalSlider;
GX_SLIDER    *pVerticalSlider;
GX_SINGLE_LINE_TEXT_INPUT *pVSliderInput;
GX_SINGLE_LINE_TEXT_INPUT *pHSliderInput;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;
INT                xshift;
INT                yshift;
INT                pre_vslider_value = 345;
INT                pre_hslider_value = 20;
TX_BYTE_POOL       memory_pool;
extern GX_STUDIO_DISPLAY_INFO kerning_glyph_draw_32bpp_screen_display_table[1];
extern GX_CONST GX_THEME *display_1_theme_table[1];
/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
UINT load_binary_resource_data_to_ram();
UINT load_binres_theme(GX_UBYTE *root_address, INT theme_id);
GX_UBYTE *binres_root_address = GX_NULL;
GX_THEME *theme = GX_NULL;

void string_to_int(char *str, int len, int *value);

#define            SCRATCHPAD_PIXELS 300 * 1024
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];

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

//////////////////////////////////////////////////////////////////////////////////////
void memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

VOID tx_application_define(void *first_unused_memory)
{
    /* create byte pool*/
    tx_byte_pool_create(&memory_pool, "scratchpad", scratchpad,
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
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    load_binary_resource_data_to_ram();

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb, 
                                0, 0, &root);

    /* create the main screen */
    gx_studio_named_widget_create("window", (GX_WIDGET *) root, (GX_WIDGET **) &pMainWin);

    pHorizontalClip = &pMainWin -> window_horizontal_clip;
    pVerticalClip = &pMainWin -> window_vertical_clip;
    pHorizontalSlider = &pMainWin -> window_horizontal_slider;
    pVerticalSlider = &pMainWin -> window_vertical_slider;

    pVSliderInput = &pMainWin -> window_text_input_vslider_val;
    pHSliderInput = &pMainWin -> window_text_input_hslider_val;


    /* Initialize single line text input. */
    strcpy(pVSliderInput->gx_single_line_text_input_buffer, "20");
    pVSliderInput -> gx_single_line_text_input_string_size = 2;

    strcpy(pHSliderInput->gx_single_line_text_input_buffer, "20");
    pHSliderInput -> gx_single_line_text_input_string_size = 2;

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

UINT main_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
UINT status = 0;
INT value;
    
    switch(myevent -> gx_event_type)
    {
    case GX_SIGNAL(ID_VERTICAL_SLIDER, GX_EVENT_SLIDER_VALUE):
        yshift = myevent -> gx_event_payload.gx_event_longdata;
        yshift = pre_vslider_value - yshift;
        pre_vslider_value = myevent -> gx_event_payload.gx_event_longdata;
        gx_widget_shift(pHorizontalClip, 0, yshift, GX_TRUE);
        break;

    case GX_SIGNAL(ID_HORIZONTAL_SLIDER, GX_EVENT_SLIDER_VALUE):
        xshift = myevent -> gx_event_payload.gx_event_longdata;
        xshift -= pre_hslider_value;
        pre_hslider_value = myevent -> gx_event_payload.gx_event_longdata;
        gx_widget_shift(pVerticalClip, xshift, 0, GX_TRUE);
        break;

    case GX_SIGNAL(ID_VSLIDER_INPUT, GX_EVENT_TEXT_EDITED):
        string_to_int(pVSliderInput -> gx_single_line_text_input_buffer, pVSliderInput -> gx_single_line_text_input_string_size, &value);
        if((value >= 20) && (value <= 345))
        {
            value = 345 - (value - 20);
            gx_slider_value_set(pVerticalSlider, &pVerticalSlider->gx_slider_info, value);
        }
        break;

    case GX_SIGNAL(ID_HSLIDER_INPUT, GX_EVENT_TEXT_EDITED):
        string_to_int(pHSliderInput -> gx_single_line_text_input_buffer, pHSliderInput -> gx_single_line_text_input_string_size, &value);
        if((value >= 20) && (value <= 570))
        {
            gx_slider_value_set(pHorizontalSlider, &pHorizontalSlider->gx_slider_info, value);
        }
        break;

    case GX_SIGNAL(ID_RADIO_BUTTON_THEME1, GX_EVENT_RADIO_SELECT):
        gx_display_theme_install(kerning_glyph_draw_32bpp_screen_display_table[DISPLAY_1].display, display_1_theme_table[DISPLAY_1_THEME_1]);
        break;

    case GX_SIGNAL(ID_RADIO_BUTTON_THEME2, GX_EVENT_RADIO_SELECT):
        load_binres_theme(binres_root_address, 0);
        break;
    default:
        status = gx_window_event_process(window, myevent);
        break;
    }
    return status;
}

void string_to_int(char *str, int len, int *value)
{
int i;
    
    *value = 0;

    for(i = 0; i < len; i++)
    {
        *value = *value * 10;
        *value += str[i] - '0';
    }
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
    int  pathlen;

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
        gx_display_theme_install(kerning_glyph_draw_32bpp_screen_display_table[DISPLAY_1].display, theme);
    }

    return status;
}