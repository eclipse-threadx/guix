/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "synergy_glyph_draw_8bpp_screen_resources.h"
#include "synergy_glyph_draw_8bpp_screen_specifications.h"

WINDOW_CONTROL_BLOCK *pMainWin;
GX_WINDOW    *pHorizontalClip;
GX_WINDOW    *pVerticalClip;
GX_SLIDER    *pHorizontalSlider;
GX_SLIDER    *pVerticalSlider;
GX_SINGLE_LINE_TEXT_INPUT *pVSliderInput;
GX_SINGLE_LINE_TEXT_INPUT *pHSliderInput;
GX_SINGLE_LINE_TEXT_INPUT *pVClipInput;
GX_SINGLE_LINE_TEXT_INPUT *pHClipInput;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;
INT                xshift;
INT                yshift;
INT                pre_vslider_value = 345;
INT                pre_hslider_value = 20;

/* Define prototypes.   */
VOID  demo_thread_entry(ULONG thread_input);
extern UINT win32_dave2d_graphics_driver_setup_8bit_palette(GX_DISPLAY *display);

void string_to_int(char *str, int len, int *value);

int main(int argc, char ** argv)
{
    tx_kernel_enter();
    return(0);
}


VOID tx_application_define(void *first_unused_memory)
{

    /* Create the main demo thread.  */
    tx_thread_create(&demo_thread, "GUIX Demo Thread", demo_thread_entry,
        0, demo_thread_stack, sizeof(demo_thread_stack),
        1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}

VOID  demo_thread_entry(ULONG thread_input)
{
    GX_STRING string;

    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(DISPLAY_1, win32_dave2d_graphics_driver_setup_8bit_palette,
        0, 0, &root);

    /* create the main screen */
    gx_studio_named_widget_create("window", (GX_WIDGET *)root, (GX_WIDGET **)&pMainWin);

    pHorizontalClip = &pMainWin->window_horizontal_clip;
    pVerticalClip = &pMainWin->window_vertical_clip;
    pHorizontalSlider = &pMainWin->window_horizontal_slider;
    pVerticalSlider = &pMainWin->window_vertical_slider;

    pVSliderInput = &pMainWin->window_text_input_vslider_val;
    pHSliderInput = &pMainWin->window_text_input_hslider_val;
    pVClipInput = &pMainWin->window_vclip_width;
    pHClipInput = &pMainWin->window_hclip_width;

    /* Initialize single line text input. */
    string.gx_string_ptr = "20";
    string.gx_string_length = 2;

    gx_single_line_text_input_text_set_ext(pVSliderInput, &string);
    gx_single_line_text_input_text_set_ext(pHSliderInput, &string);
    gx_single_line_text_input_text_set_ext(pVClipInput, &string);
    gx_single_line_text_input_text_set_ext(pHClipInput, &string);


    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

UINT main_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
    UINT status = 0;
    INT value;
    GX_RECTANGLE size;

    switch (myevent->gx_event_type)
    {
    case GX_SIGNAL(ID_VERTICAL_SLIDER, GX_EVENT_SLIDER_VALUE):
        yshift = myevent->gx_event_payload.gx_event_longdata;
        yshift = pre_vslider_value - yshift;
        pre_vslider_value = myevent->gx_event_payload.gx_event_longdata;

        gx_widget_shift(pHorizontalClip, 0, yshift, GX_TRUE);
        break;

    case GX_SIGNAL(ID_HORIZONTAL_SLIDER, GX_EVENT_SLIDER_VALUE):
        xshift = myevent->gx_event_payload.gx_event_longdata;
        xshift -= pre_hslider_value;
        pre_hslider_value = myevent->gx_event_payload.gx_event_longdata;

        gx_widget_shift(pVerticalClip, xshift, 0, GX_TRUE);
        break;

    case GX_SIGNAL(ID_VSLIDER_INPUT, GX_EVENT_TEXT_EDITED):
        string_to_int(pVSliderInput->gx_single_line_text_input_buffer, pVSliderInput->gx_single_line_text_input_string_size, &value);
        if ((value >= 20) && (value <= 345))
        {
            value = 345 - (value - 20);
            gx_slider_value_set(pVerticalSlider, &pVerticalSlider->gx_slider_info, value);
        }
        break;

    case GX_SIGNAL(ID_HSLIDER_INPUT, GX_EVENT_TEXT_EDITED):
        string_to_int(pHSliderInput->gx_single_line_text_input_buffer, pHSliderInput->gx_single_line_text_input_string_size, &value);
        if ((value >= 20) && (value <= 570))
        {
            gx_slider_value_set(pHorizontalSlider, &pHorizontalSlider->gx_slider_info, value);
        }
        break;

    case GX_SIGNAL(ID_VCLIP_INPUT, GX_EVENT_TEXT_EDITED):
        string_to_int(pVClipInput->gx_single_line_text_input_buffer, pVClipInput->gx_single_line_text_input_string_size, &value);
        if ((value >= 5) && (value <= 30))
        {
            size = pVerticalClip->gx_widget_size;
            size.gx_rectangle_right = size.gx_rectangle_left + value;
            gx_widget_resize(pVerticalClip, &size);

            size.gx_rectangle_right = size.gx_rectangle_left + value / 2;
            gx_widget_resize(pVerticalClip->gx_widget_first_child, &size);
        }
        break;

    case GX_SIGNAL(ID_HCLIP_INPUT, GX_EVENT_TEXT_EDITED):
        string_to_int(pHClipInput->gx_single_line_text_input_buffer, pHClipInput->gx_single_line_text_input_string_size, &value);
        if ((value >= 5) && (value <= 30))
        {
            size = pHorizontalClip->gx_widget_size;
            size.gx_rectangle_bottom = size.gx_rectangle_top + value;
            gx_widget_resize(pHorizontalClip, &size);

            size.gx_rectangle_bottom = size.gx_rectangle_top + value / 2;
            gx_widget_resize(pHorizontalClip->gx_widget_first_child, &size);
        }
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

    for (i = 0; i < len; i++)
    {
        *value = *value * 10;
        *value += str[i] - '0';
    }
}