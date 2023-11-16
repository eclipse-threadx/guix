/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "single_line_text_input_clip_32bpp_resources.h"
#include "single_line_text_input_clip_32bpp_specifications.h"

MAIN_WINDOW_CONTROL_BLOCK *pMainWin;
GX_WINDOW      *clip_1;
GX_WINDOW      *clip_2;
GX_SLIDER      *v_slider_1;
GX_SLIDER      *v_slider_2;
GX_SLIDER      *h_slider_1;
GX_SLIDER      *h_slider_2;

/* Define the ThreadX demo thread control block and stack.  */

TX_THREAD          demo_thread;
UCHAR              demo_thread_stack[4096];

GX_WINDOW_ROOT    *root;
INT                xshift;
INT                yshift;
INT                width;
INT                height;
INT                h_slider_value_1 = -1;
INT                h_slider_value_2 = -1;
INT                v_slider_value_1 = -1;
INT                v_slider_value_2 = -1;

GX_CHAR            test_buffer[] = "Abcdefghi3748383";
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


VOID  demo_thread_entry(ULONG thread_input)
{
    GX_STRING string;
        
    /* Initialize GUIX.  */
    gx_system_initialize();


    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_24xrgb, 
                                LANGUAGE_ENGLISH, DISPLAY_1_DEFAULT_THEME, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_window", (GX_WIDGET *) root, (GX_WIDGET **) &pMainWin);

    /* Insert string. */
    string.gx_string_ptr = test_buffer;
    string.gx_string_length = sizeof(test_buffer) - 1;
    gx_single_line_text_input_text_set_ext(&main_window.main_window_text_input_1bpp, &string);
    gx_single_line_text_input_text_set_ext(&main_window.main_window_text_input_4bpp, &string);
    gx_single_line_text_input_text_set_ext(&main_window.main_window_text_input_8bpp, &string);

    clip_1 = &pMainWin -> main_window_clip_1;
    clip_2 = &pMainWin -> main_window_clip_2;
    v_slider_1 = &pMainWin -> main_window_slider_1;
    v_slider_2 = &pMainWin -> main_window_slider_2;
    h_slider_1 = &pMainWin -> main_window_slider_3;
    h_slider_2 = &pMainWin -> main_window_slider_4;

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* let GUIX run */
    gx_system_start();
}

UINT main_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
UINT            status = 0;
GX_RECTANGLE    new_size;

    switch(myevent -> gx_event_type)
    {
    case GX_SIGNAL(ID_CLIP_HEIGHT, GX_EVENT_SLIDER_VALUE):
        height = myevent -> gx_event_payload.gx_event_longdata;
        new_size.gx_rectangle_left = clip_1 -> gx_widget_size.gx_rectangle_left;
        new_size.gx_rectangle_right = clip_1 -> gx_widget_size.gx_rectangle_right;
        if(v_slider_value_1 == -1)
        {
            v_slider_value_1 = v_slider_1 -> gx_slider_info.gx_slider_info_min_val;
        }
        if(height < v_slider_value_1)
        {
            new_size.gx_rectangle_top = clip_1 -> gx_widget_size.gx_rectangle_top + (v_slider_value_1 - height);
            new_size.gx_rectangle_bottom = clip_1 -> gx_widget_size.gx_rectangle_bottom - (v_slider_value_1 - height);
        }
        else if(height > v_slider_value_1)
        {
            new_size.gx_rectangle_top = clip_1 -> gx_widget_size.gx_rectangle_top - (height - v_slider_value_1);
            new_size.gx_rectangle_bottom = clip_1 -> gx_widget_size.gx_rectangle_bottom + (height - v_slider_value_1);
        }
        v_slider_value_1 = height;

        gx_widget_resize(clip_1, &new_size);
        break;

    case GX_SIGNAL(ID_VERTICAL_MOVE, GX_EVENT_SLIDER_VALUE):
        yshift = myevent -> gx_event_payload.gx_event_longdata;
        if(v_slider_value_2 == -1)
        {
            v_slider_value_2 = v_slider_2 -> gx_slider_info.gx_slider_info_max_val;
        }
        gx_widget_shift(clip_1, 0, v_slider_value_2 - yshift, GX_TRUE);
        v_slider_value_2 = yshift;
        break;

    case GX_SIGNAL(ID_CLIP_WIDTH, GX_EVENT_SLIDER_VALUE):
        width = myevent -> gx_event_payload.gx_event_longdata;
        new_size.gx_rectangle_top = clip_2 -> gx_widget_size.gx_rectangle_top;
        new_size.gx_rectangle_bottom = clip_2 -> gx_widget_size.gx_rectangle_bottom;
        if(h_slider_value_1 == -1)
        {
            h_slider_value_1 = h_slider_1 -> gx_slider_info.gx_slider_info_min_val;
        }
        if(width < h_slider_value_1)
        {
            new_size.gx_rectangle_left = clip_2 -> gx_widget_size.gx_rectangle_left + (h_slider_value_1 - width);
            new_size.gx_rectangle_right = clip_2 -> gx_widget_size.gx_rectangle_right - (h_slider_value_1 - width);
        }
        else if(width > h_slider_value_1)
        {
            new_size.gx_rectangle_left = clip_2 -> gx_widget_size.gx_rectangle_left - (width - h_slider_value_1);
            new_size.gx_rectangle_right = clip_2 -> gx_widget_size.gx_rectangle_right + (width - h_slider_value_1);
        }
        h_slider_value_1 = width;

        gx_widget_resize(clip_2, &new_size);
        break;

    case GX_SIGNAL(ID_HORIZONTAL_MOVE, GX_EVENT_SLIDER_VALUE):
        xshift = myevent -> gx_event_payload.gx_event_longdata;
        if(h_slider_value_2 == -1)
        {
            h_slider_value_2 = h_slider_2 -> gx_slider_info.gx_slider_info_min_val;
        }
        gx_widget_shift(clip_2, xshift - h_slider_value_2, 0, GX_TRUE);
        h_slider_value_2 = xshift;
        break;

    default:
        status = gx_window_event_process(window, myevent);
        break;
    }
    return status;
}
