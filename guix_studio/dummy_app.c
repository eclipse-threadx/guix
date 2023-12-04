
#include "gx_api.h"

#define ID_NUMBER_TIMER         1

GX_WINDOW         demo_window_a;
GX_WINDOW         demo_window_b;
GX_WINDOW         demo_window_c;
GX_WINDOW         demo_window_d;
GX_WINDOW         window_a_frame;
GX_PROMPT         demo_prompt;
GX_PIXELMAP_PROMPT  demo_number;
GX_TEXT_BUTTON    demo_stop_button;
GX_TEXT_BUTTON    demo_go_button;
GX_SINGLE_LINE_TEXT_INPUT     text_input_ptr;
GX_SLIDER         demo_slider;
GX_ICON           demo_icon;
GX_SCROLLBAR      win_a_vertical_scrollbar;
GX_SCROLLBAR      win_a_horizontal_scrollbar;

GX_CHECKBOX       checkbox1;
GX_RADIO_BUTTON   radio1;
GX_RADIO_BUTTON   radio2;

GX_BOOL           flag = TRUE;

static void    my_timer_entry(ULONG);

#define WIN_A_COLOR 0x800000
#define WIN_B_COLOR 0x008000
#define WIN_C_COLOR 0x000080

TX_TIMER          number_timer;

INT NumVal = 0;
char numbuf[12];

/* Define GUIX strings and string IDs for this demo.  */

enum demo_string_ids
{
    SID_HELLO = 1,
    SID_ANOTHER,
    SID_STOP,
    SID_GO,
    SID_MOVABLE,
    SID_BLUE_WIN,
    SID_RED_WIN,
    STRING_TABLE_MAX
};

char *demo_strings[] = {
    NULL,
    "Hello GUIX",
    "Another example string",
    "Stop",
    "Go",
    "movable",
    "Blue Window",
    "Red Window"
};

extern GX_PIXELMAP cal_poppy_200_png_24xrgb_ca_pixelmap;
extern GX_PIXELMAP cal_poppy_320_png_24xrgb_ca_pixelmap;
extern GX_PIXELMAP Frog8_200_png_24xrgb_ca_pixelmap;
extern GX_PIXELMAP tfield_fill_small_png_24xrgb_ca_pixelmap;
extern GX_PIXELMAP tfield_left_small_png_24xrgb_ca_pixelmap;
extern GX_PIXELMAP tfield_right_small_png_24xrgb_ca_pixelmap;
extern GX_PIXELMAP threadx_png_24xrgb_ca_pixelmap;

extern GX_PIXELMAP radiobutton_on_pixelmap;
extern GX_PIXELMAP radiobutton_off_pixelmap;
extern GX_PIXELMAP checkbox_on_pixelmap;
extern GX_PIXELMAP checkbox_off_pixelmap;

GX_PIXELMAP *demo_pixelmap_table[] = {
    NULL,
    &radiobutton_on_pixelmap,
    &radiobutton_off_pixelmap,
    &checkbox_on_pixelmap,
    &checkbox_off_pixelmap,
    &cal_poppy_200_png_24xrgb_ca_pixelmap,
    &tfield_fill_small_png_24xrgb_ca_pixelmap,
    &tfield_left_small_png_24xrgb_ca_pixelmap,
    &tfield_right_small_png_24xrgb_ca_pixelmap,
    &threadx_png_24xrgb_ca_pixelmap
};

enum demo_pixelmaps {
    
    BID_WALLPAPER = GX_FIRST_USER_PIXELMAP,
    BID_PROMPT_FILL,
    BID_PROMPT_LEFT,
    BID_PROMPT_RIGHT,
    BID_THREADX,
    NUMBER_OF_PIXELMAPS
};

/* widget id values used by this demo */
enum demo_widget_ids {
    BID_GO = 1,                 /* the GO button            */
    BID_STOP,                   /* the STOP button          */
    BID_MOVABLE,
    BID_BLUE_WIN,
    BID_RED_WIN,
    ID_SLIDER,
    ID_WINDOW_A,
    ID_WINDOW_B,
    ID_WINDOW_C,
    ID_WINDOW_D,
    ID_WINDOW_A_FRAME
};

/* application specific events used by this demo */
enum demo_events {
    DEMO_EVENT_START_COUNTER = GX_FIRST_APP_EVENT,     /* event sent to start numeric counter */
    DEMO_EVENT_STOP_COUNTER                            /* event sent to stop numeric counter  */
};

/* Define prototypes.   */

UINT  demo_win_a_event_handler(GX_WIDGET *me, GX_EVENT *myevent);
UINT  demo_window_b_event_handler(GX_WIDGET *me, GX_EVENT *myevent);
UINT  window_d_event_handler(GX_WIDGET *widget, GX_EVENT *myevent);

VOID  demo_window_c_draw(GX_WIDGET *me);
VOID  demo_window_a_draw(GX_WIDGET *me);

extern GX_WINDOW_ROOT *_gx_system_root_window_created_list;
static GX_SLIDER_INFO slider_info;

void dummy_app_create(void)
{
    GX_RECTANGLE size;
    GX_RECTANGLE child_size;
    GX_WINDOW_ROOT *root;

    root = _gx_system_root_window_created_list;

    /* Install the demo string table.  */
    gx_system_string_table_set(demo_strings, STRING_TABLE_MAX);

    /* Install the demo bitmap table. */
    gx_system_pixelmap_table_set(demo_pixelmap_table, NUMBER_OF_PIXELMAPS);

    /* Create a window frame that will do scrolling */
    gx_utility_rectangle_define(&size, 0, 0, 199, 199);
    gx_utility_rectangle_center(&size, &root -> gx_widget_size);
    gx_window_create(&window_a_frame, NULL, root, GX_STYLE_BORDER_THIN, ID_WINDOW_A_FRAME, &size);

    /* add a vertical scrollbar to the window */
    gx_vertical_scrollbar_create(&win_a_vertical_scrollbar, NULL, &window_a_frame, GX_NULL); 

    /* add a horizontal scrollbar to the window */
    gx_horizontal_scrollbar_create(&win_a_horizontal_scrollbar, NULL, &window_a_frame, GX_NULL); 

    /* Create a window that contains a few buttons */
    child_size = size;
    child_size.gx_rectangle_bottom += 200;
    child_size.gx_rectangle_right += 200;

    gx_window_create(&demo_window_a, "window a", &window_a_frame, GX_STYLE_BORDER_THIN, ID_WINDOW_A, &child_size);
//    gx_widget_fill_color_set(&demo_window_a, GX_CID_BLUE);
    gx_window_wallpaper_set(&demo_window_a, BID_WALLPAPER, TRUE);
    //gx_widget_status_remove(&demo_window_a, GX_STATUS_MOVABLE);

    /* provide an event handler for the button window */
    gx_widget_event_process_set(&demo_window_a, demo_win_a_event_handler);
    gx_widget_draw_set(&demo_window_a, demo_window_a_draw);
   
    /* create the slider */
    gx_utility_rectangle_define(&child_size, 0, 0, 99, 34);
    gx_utility_rectangle_center(&child_size, &demo_window_a.gx_widget_size);
   
    gx_utility_rectangle_shift(&child_size, 0, -68);

    memset(&slider_info, 0, sizeof(slider_info));
    slider_info.min_val = 64;
    slider_info.max_val = 255;
    slider_info.min_travel = 10;
    slider_info.max_travel = 10;
    slider_info.needle_height = 20;
    slider_info.needle_inset = 6;
    slider_info.increment = 5;

    gx_slider_create(&demo_slider, NULL, &demo_window_a,
        GX_STYLE_BORDER_THIN | GX_STYLE_SHOW_NEEDLE | GX_STYLE_SHOW_TICKMARKS,
                     ID_SLIDER, &child_size, 10, &slider_info);

    /* Create the Stop and Go buttons.  */

    gx_utility_rectangle_shift(&child_size, 0, 40);
    gx_text_button_create(&demo_go_button, "demo go button", &demo_window_a, SID_GO, 0, BID_GO, &child_size);

    gx_utility_rectangle_shift(&child_size, 0, 40);
    gx_text_button_create(&demo_stop_button, "demo stop button", &demo_window_a, SID_STOP, 0, BID_STOP, &child_size);
    
    gx_utility_rectangle_shift(&child_size, 0, 40);
    child_size.gx_rectangle_left = (199 - threadx_png_24xrgb_ca_pixelmap.gx_pixelmap_width) / 2;
    child_size.gx_rectangle_left += demo_window_a.gx_widget_size.gx_rectangle_left;
    gx_icon_create(&demo_icon, NULL, &demo_window_a, BID_THREADX, BID_THREADX, 0, child_size.gx_rectangle_left, child_size.gx_rectangle_top);

    /* default to 50% alpha value for slider */
    gx_slider_value_set(&demo_slider, 128);
    
    /* Create Window B on background canvas */
    gx_utility_rectangle_shift(&size, -210, -60);
    //gx_window_create(&demo_window_b, "demo win b", &demo_root_window, GX_STYLE_BORDER_THIN, ID_WINDOW_B, &size);
    gx_window_create(&demo_window_b, "demo win b", root, GX_STYLE_BORDER_NONE, ID_WINDOW_B, &size);
	gx_widget_event_process_set(&demo_window_b, demo_window_b_event_handler);	
    gx_widget_status_add(&demo_window_b, GX_STATUS_MOVABLE);

    /* assign a wallpaper to counter Window */
    gx_window_wallpaper_set(&demo_window_b, BID_WALLPAPER, TRUE);

    /* Create a prompt on demo counter Window */
    gx_utility_rectangle_define(&child_size, 0, 0, 179, 32);
    gx_utility_rectangle_center(&child_size, &demo_window_b.gx_widget_size);
    gx_prompt_create(&demo_prompt, "demo prompt", &demo_window_b, SID_HELLO, 
        GX_STYLE_BORDER_THIN | GX_STYLE_TEXT_CENTER, 0, &child_size);
	
    /* Create a bitmap-prompt on demo counter window */
	gx_utility_rectangle_shift(&child_size, 0, 50);
    child_size.gx_rectangle_bottom = child_size.gx_rectangle_top + tfield_fill_small_png_24xrgb_ca_pixelmap.gx_pixelmap_height - 1;
    gx_pixelmap_prompt_create(&demo_number, "demo number", &demo_window_b, 0,
        BID_PROMPT_LEFT, BID_PROMPT_FILL, BID_PROMPT_RIGHT,
        GX_STYLE_BORDER_THIN | GX_STYLE_TEXT_RIGHT, 0, &child_size);
    gx_widget_fill_color_set(&demo_number, GX_CID_WHITE, GX_CID_WHITE);

    /* Create demo draw Window C */
    gx_utility_rectangle_shift(&size, 420, 120);
    gx_window_create(&demo_window_c, "demo window c", root, GX_STYLE_BORDER_NONE, ID_WINDOW_C, &size);
    gx_widget_draw_set(&demo_window_c, demo_window_c_draw);
    gx_widget_status_add(&demo_window_c, GX_STATUS_MOVABLE);

/*  KGM this is broken, remove for now.
    gx_utility_rectangle_define(&size, 300, 50, 420, 80);
    gx_single_line_text_input_create(&text_input_ptr, "text input", &demo_root_window, size, 1, 
                         "zero one TWO Three for five",27, GX_ECHO, GX_SOLID_LINE, GX_VERTICAL_BLINKING_CURSOR, 0, 0);
*/
    /* create demo window d */
    gx_utility_rectangle_define(&size, 10, 300, 209, 499);
    gx_window_create(&demo_window_d, "demo window d", root, GX_STYLE_BORDER_NONE, ID_WINDOW_D, &size);
    gx_widget_event_process_set(&demo_window_d, window_d_event_handler);
    gx_widget_fill_color_set(&demo_window_d, GX_CID_BLUE, GX_CID_BLUE);

    size.gx_rectangle_top += 10;
    size.gx_rectangle_left += 10;
    size.gx_rectangle_right -= 10;
    size.gx_rectangle_bottom = size.gx_rectangle_top + 20;

    gx_checkbox_create(&checkbox1, "checkbox1", &demo_window_d, SID_MOVABLE,
        GX_STYLE_TEXT_LEFT | GX_STYLE_ENABLED | GX_STYLE_TRANSPARENT,
        BID_MOVABLE, &size);

    gx_text_button_color_set(&checkbox1, GX_CID_WHITE, GX_CID_WHITE);

    gx_utility_rectangle_shift(&size, 0, 30);
    gx_radio_button_create(&radio1, "blue window", &demo_window_d, SID_BLUE_WIN,
        GX_STYLE_TEXT_LEFT | GX_STYLE_ENABLED | GX_STYLE_BUTTON_PUSHED | GX_STYLE_TRANSPARENT,
        BID_BLUE_WIN, &size);
    gx_text_button_color_set(&radio1, GX_CID_WHITE, GX_CID_WHITE);

    gx_utility_rectangle_shift(&size, 0, 30);
    gx_radio_button_create(&radio2, "red window", &demo_window_d, SID_RED_WIN,
        GX_STYLE_TEXT_LEFT | GX_STYLE_ENABLED | GX_STYLE_TRANSPARENT,
        BID_RED_WIN, &size);
    gx_text_button_color_set(&radio2, GX_CID_WHITE, GX_CID_WHITE);

    /* put overlay a in front */
    gx_widget_front_move(&window_a_frame, NULL);

    /* Show the root window.  */
    _gx_widget_show((GX_WIDGET *) root);
}


UINT window_d_event_handler(GX_WIDGET *widget, GX_EVENT *myevent)
{
    switch(myevent ->gx_event_type)
    {
    case GX_SIGNAL(BID_MOVABLE, GX_EVENT_TOGGLE_ON):
        gx_widget_status_add(&demo_window_d, GX_STATUS_MOVABLE);
        break;

    case GX_SIGNAL(BID_MOVABLE, GX_EVENT_TOGGLE_OFF):
        gx_widget_status_remove(&demo_window_d, GX_STATUS_MOVABLE);
        break;

    case GX_SIGNAL(BID_BLUE_WIN, GX_EVENT_RADIO_SELECT):
        gx_widget_fill_color_set(widget, GX_CID_BLUE, GX_CID_BLUE);
        break;

    case GX_SIGNAL(BID_RED_WIN, GX_EVENT_RADIO_SELECT):
        gx_widget_fill_color_set(widget, GX_CID_RED, GX_CID_RED);
        break;

    default:
        return gx_window_event_process(widget, myevent);
    }
    return 0;
}

UINT demo_win_a_event_handler(GX_WIDGET *widget, GX_EVENT *myevent)
{
GX_EVENT newevent;
UINT status = 0;

    switch(myevent -> gx_event_type)
    {
    case GX_EVENT_SLIDER_VALUE:
        break;

    case GX_SIGNAL(BID_GO, GX_EVENT_CLICKED):
        newevent.gx_event_sender = ID_WINDOW_A;
        newevent.gx_event_target = ID_WINDOW_B;
        newevent.gx_event_type   = DEMO_EVENT_START_COUNTER;
        gx_system_event_send(&newevent);
        break;

    case GX_SIGNAL(BID_STOP, GX_EVENT_CLICKED):
        newevent.gx_event_sender = ID_WINDOW_A;
        newevent.gx_event_target = ID_WINDOW_B;
        newevent.gx_event_type   = DEMO_EVENT_STOP_COUNTER;
        gx_system_event_send(&newevent);
        break;

    default:
        status = gx_window_event_process(widget, myevent);
        break;
    }
    return status;
}

VOID counter_update(VOID)
{
    /* change the displayed number */
    NumVal++;
    gx_utility_ltoa(NumVal, numbuf, 10);
    gx_prompt_text_set(&demo_number, numbuf);
}

UINT demo_window_b_event_handler(GX_WIDGET *widget, GX_EVENT *myevent)
{
    UINT status = 0;

    switch(myevent->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_widget_event_process(widget, myevent);
        counter_update();
        return GX_SUCCESS;

    case GX_EVENT_HIDE:
        gx_widget_event_process(widget, myevent);
        gx_system_timer_stop(widget, 0);
        return GX_SUCCESS;

    case GX_EVENT_TIMER:
        if (myevent->gx_event_payload.gx_event_timer_id == ID_NUMBER_TIMER)
        {
            counter_update();
        }
        break;

    case DEMO_EVENT_START_COUNTER: 
        gx_system_timer_start(widget, ID_NUMBER_TIMER, 1, 1);
        break;

    case DEMO_EVENT_STOP_COUNTER:
        gx_system_timer_stop(widget, 0);
        break;

    default:
        status = gx_window_event_process(widget, myevent);
        break;
    }
    return status;
}

UINT demo_text_input_event_handler(GX_WIDGET *widget, GX_EVENT *myevent)
{
UINT status = 0;

return status;
}

VOID demo_window_a_draw(GX_WIDGET *widget)
{
    GX_WINDOW *win = (GX_WINDOW *) widget;

    gx_window_draw(widget);
    gx_context_brush_define(GX_CID_WHITE, GX_CID_WHITE, 0);
    gx_context_brush_width_set(1);
    gx_screen_rectangle_draw(&win ->gx_window_client);
}

VOID demo_window_c_draw(GX_WIDGET *widget)
{
INT xpos;
INT ypos;

    /* do normal background drawing first.  */
    gx_window_draw(widget);
    gx_context_brush_define(GX_CID_WHITE, GX_CID_WHITE, GX_BRUSH_ALIAS);
    gx_context_brush_width_set(1);


    //gx_screen_line_draw(widget ->gx_widget_size.gx_rectangle_left + 10, widget ->gx_widget_size.gx_rectangle_top + 10,
    //        widget ->gx_widget_size.gx_rectangle_left + 20, widget ->gx_widget_size.gx_rectangle_top + 20);
    for (xpos = widget ->gx_widget_size.gx_rectangle_left + 10,
         ypos = widget ->gx_widget_size.gx_rectangle_top + 10;
         xpos <= widget ->gx_widget_size.gx_rectangle_right - 10,
         ypos <= widget ->gx_widget_size.gx_rectangle_bottom - 10;
         xpos += 10, ypos += 10)
    {
        /* Draw line from top-middle to lower-right.  */
        gx_screen_line_draw(xpos, widget ->gx_widget_size.gx_rectangle_top + 10,
            widget ->gx_widget_size.gx_rectangle_right - 10, ypos);
    }

    gx_context_line_color_set(GX_CID_RED);

    for (ypos = widget ->gx_widget_size.gx_rectangle_top + 10,
         xpos = widget ->gx_widget_size.gx_rectangle_left + 10;
         ypos <= widget ->gx_widget_size.gx_rectangle_bottom - 10,
         xpos <= widget ->gx_widget_size.gx_rectangle_right - 10;
         xpos += 10, ypos += 10)
    {
        /* Draw line from top-middle to lower-right.  */
        gx_screen_line_draw(widget ->gx_widget_size.gx_rectangle_left + 10, ypos,
            xpos, widget ->gx_widget_size.gx_rectangle_bottom - 10);
    }
}


