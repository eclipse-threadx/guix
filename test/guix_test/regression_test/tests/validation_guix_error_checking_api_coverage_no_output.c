/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_animation.h"
#include "gx_canvas.h"
#include "gx_widget.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_scroll_wheel.h"
#include "gx_multi_line_text_view.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_api_coverage_test_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.*/
};

static GX_CHAR test_string[] = "test string";

int main(int argc, char ** argv)
{
    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

#if defined (GX_DISABLE_ERROR_CHECKING)
VOID tx_application_define(void *first_unused_memory)
{
    exit(0);
}
#else
static VOID      control_thread_entry(ULONG);
static UINT failed_tests;
extern volatile ULONG      _tx_thread_system_state;
extern ULONG             _tx_timer_created_count;
extern ULONG             _tx_queue_created_count;

UINT widget_event_process_function(GX_WIDGET *widget, GX_EVENT *event)
{
    return 0;
}

static USHORT display_driver_row_pitch_get(USHORT input)
{
    return 0;
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);
}

#include "gx_validation_wrapper.h"
#include "demo_guix_all_widgets.c"

extern UINT gx_validation_display_driver_setup_24xrgb(GX_DISPLAY *display);
extern VOID gx_validation_graphics_driver_cleanup_24xrgb(GX_DISPLAY *display);
static GX_CONST GX_CHAR *          callback_fun(GX_TEXT_SCROLL_WHEEL * param1, INT param2) {return GX_NULL;}
static VOID *                      memory_allocate(ULONG size){ GX_PARAMETER_NOT_USED(size); return GX_NULL;}
static VOID                        memory_free(VOID * ptr){GX_PARAMETER_NOT_USED(ptr);}
static VOID                        draw_func(GX_WIDGET * widget){}
static UINT                        event_process_func(GX_WIDGET *widget, GX_EVENT *event){ return 0;}
static VOID                        display_cleanup_func(GX_DISPLAY *display) { memset(display, 0, sizeof(GX_DISPLAY));}
static UINT                        write_data_callback(GX_UBYTE *byte_data, UINT data_count){return GX_SUCCESS;}

static VOID control_thread_entry(ULONG input)
{
GX_ACCORDION_MENU           accordion_menu;
GX_ANIMATION                animation;
GX_ANIMATION                animation1;
GX_ANIMATION                animation2;
GX_ANIMATION               *animation_ptr;
GX_ANIMATION_INFO           animation_info;
GX_LINE_CHART_INFO          line_chart_info;

GX_BUTTON                   button;
GX_CHECKBOX                 checkbox;
GX_CIRCULAR_GAUGE           circular_gauge;
GX_CIRCULAR_GAUGE_INFO      circular_gauge_info;
GX_DROP_LIST                drop_list;
GX_HORIZONTAL_LIST          horizontal_list;
GX_ICON_BUTTON              icon_button;
GX_ICON                     icon;
GX_IMAGE_READER             image_reader;
GX_LINE_CHART               line_chart;
GX_MENU                     menu;
GX_MULTI_LINE_TEXT_BUTTON   multi_line_text_button;
GX_MULTI_LINE_TEXT_INPUT    multi_line_text_input;
GX_MULTI_LINE_TEXT_VIEW     multi_line_text_view;
GX_NUMERIC_PIXELMAP_PROMPT  numeric_pixelmap_prompt;
GX_NUMERIC_PROMPT           numeric_prompt;
GX_NUMERIC_SCROLL_WHEEL     numeric_scroll_wheel;
GX_PIXELMAP_BUTTON          pixelmap_button;
GX_PIXELMAP_PROMPT          pixelmap_prompt;
GX_PIXELMAP_SLIDER          pixelmap_slider;
GX_SLIDER_INFO              slider_info;
GX_PIXELMAP_SLIDER_INFO     pixelmap_slider_info;
GX_PROGRESS_BAR             progress_bar;
GX_PROGRESS_BAR_INFO        progress_bar_info;
GX_PROMPT                   prompt;
GX_RADIAL_PROGRESS_BAR      radial_progress_bar;
GX_RADIAL_PROGRESS_BAR_INFO radial_progress_bar_info;
GX_RADIAL_SLIDER            radial_slider;
GX_RADIAL_SLIDER_INFO      *radial_slider_info;
GX_RADIO_BUTTON             radio_button;
GX_SCREEN_STACK_CONTROL     screen_stack_control;
GX_SCROLL_THUMB             scroll_thumb;
GX_SCROLLBAR                scrollbar;
GX_SCROLLBAR_APPEARANCE     scrollbar_appearance;
GX_SCROLL_INFO              scroll_info;
GX_SCROLL_WHEEL             scroll_wheel;
GX_SINGLE_LINE_TEXT_INPUT   single_line_text_input;
GX_SLIDER                   slider;
GX_SPRITE                   sprite;
GX_STRING_SCROLL_WHEEL      string_scroll_wheel;
GX_TEXT_BUTTON              text_button;
GX_TEXT_INPUT_CURSOR        text_input_cursor;
GX_TEXT_SCROLL_WHEEL        text_scroll_wheel;
GX_GRADIENT                 gradient;
GX_TREE_VIEW                tree_view;
GX_VERTICAL_LIST            vertical_list;
GX_WINDOW                   window;
GX_WINDOW_ROOT              window_root;

GX_BRUSH *                  brush;
GX_CANVAS                   canvas;
GX_DISPLAY                  display;
GX_DISPLAY                  default_display;
GX_DISPLAY                  display_1;
GX_DISPLAY                  display_2;
GX_DISPLAY_LAYER_SERVICES   display_layer_service;
GX_DRAW_CONTEXT             default_context;
GX_EVENT                    my_event;
GX_FONT                     font;
GX_PEN_CONFIGURATION        pen_config;
GX_PIXELMAP                 pixelmap;
GX_PIXELMAP                 destination_pixelmap;
GX_PIXELMAP                *return_pixelmap;
GX_POINT                    point;
GX_RECTANGLE                size, rec1, rec2;
GX_THEME *                  returned_theme = GX_NULL;
GX_TIMER *                  timer, gtimer;
GX_UBYTE ***                returned_language_table = GX_NULL;
GX_CHAR **                  langugage_table = GX_NULL;
GX_WIDGET                   parent;
GX_WIDGET                   widget;
GX_WIDGET *                 widget_ptr;
GX_COLOR                    color;
GX_COLOR                    canvas_memory[1];
GX_UBYTE *                  byte_data = (GX_UBYTE *)canvas_memory;
GX_RESOURCE_ID              ulong_data;
UINT                        status;
GX_CHAR                     buffer[32], *buffer_ptr;
GX_CHAR_CODE                glyph_value;
GX_UBYTE                    return_utf8_str[6];
UINT                        return_utf8_size, character_count;
GX_MULTI_LINE_TEXT_INFO     text_info; 
GX_POINT                    cursor_pos;
GX_STRING                   string;

    gx_system_initialize();     /* Initialize GUIX.  */
    gx_display_create(&default_display, "linux_display", gx_validation_display_driver_setup_24xrgb, 0, 0); //create display

    /* accordion_menu */
    printf("accordion_menu\n");
    memset(&accordion_menu, 0, sizeof(GX_ACCORDION_MENU));
    /* accordion menu create*/
    status = gx_accordion_menu_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL); // first parameter GX_ACCORDION_MENU * is null;
    EXPECT_EQ(GX_PTR_ERROR, status);

    accordion_menu.gx_widget_type = GX_TYPE_ACCORDION_MENU;
    status = gx_accordion_menu_create(&accordion_menu, GX_NULL, GX_NULL, GX_NULL, GX_NULL, &size); // gx_widget_type of accordion_menu is not 0; should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    accordion_menu.gx_widget_type = 0;
    status = _gxe_accordion_menu_create(&accordion_menu, GX_NULL, GX_NULL, GX_NULL, GX_NULL, &size, 0); // control block size is 0; should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    status = gx_accordion_menu_create(&accordion_menu, GX_NULL, GX_NULL, GX_NULL, GX_NULL, &size); // should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    gx_accordion_menu_event_process(0, 0); // event_process
    status = gx_accordion_menu_event_process(&accordion_menu, 0); // event_process
    EXPECT_EQ(GX_PTR_ERROR, status);

    accordion_menu.gx_widget_parent = 0;
    my_event.gx_event_type = 0;
    status = gx_accordion_menu_event_process(&accordion_menu, &my_event); // event_process
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_accordion_menu_position(0);
    EXPECT_EQ(GX_PTR_ERROR, status);

    /* animation */
    printf("animation\n");
    memset(&animation, 0, sizeof(GX_ANIMATION));
    memset(&canvas, 0, sizeof(GX_CANVAS));
    status = gx_animation_canvas_define(0, 0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_animation_canvas_define(&animation, 0); // canvas == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_animation_canvas_define(&animation, &canvas); // should return GX_PTR_ERROR because display is null
    EXPECT_EQ(GX_PTR_ERROR, status);

    canvas.gx_canvas_display = &default_display;
    canvas.gx_canvas_x_resolution = 1;
    canvas.gx_canvas_y_resolution = 1;
    canvas.gx_canvas_memory_size = 0;
    status = gx_animation_canvas_define(&animation, &canvas); // should return GX_INVALID_MEMORY_SIZE
    EXPECT_EQ(GX_INVALID_MEMORY_SIZE, status);
    
    status = gx_animation_create(0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    animation.gx_animation_status = GX_ANIMATION_IDLE;
    status = gx_animation_create(&animation); // should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    status = gx_animation_drag_disable(0, 0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_animation_drag_disable(&animation, 0); // widget == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    animation.gx_animation_original_event_process_function = 0;
    status = gx_animation_drag_disable(&animation, &widget); // gx_animation_original_event_process_function == GX_NULL
    EXPECT_EQ(GX_INVALID_ANIMATION, status);
    
    status = gx_animation_drag_enable(0, 0, 0); // animation == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_animation_drag_enable(&animation, 0, 0); // widget == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_animation_drag_enable(&animation, &widget, 0); // info == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    animation.gx_animation_original_event_process_function = (UINT (* )(struct GX_WIDGET_STRUCT *, GX_EVENT *))0x04;
    status = gx_animation_drag_enable(&animation, &widget, &animation_info); // animation -> gx_animation_original_event_process_function != GX_NULL
    EXPECT_EQ(GX_INVALID_ANIMATION, status);

    memset(&animation, 0, sizeof(animation));
    memset(&animation_info, 0, sizeof(GX_ANIMATION_INFO));
    animation_info.gx_animation_style |= GX_ANIMATION_EASING_FUNC_MASK;
    status = gx_animation_drag_enable(&animation, &widget, &animation_info); // animation_info -> gx_animation_style & GX_ANIMATION_EASING_FUNC_MASK
    EXPECT_EQ(GX_INVALID_VALUE, status);

    animation_info.gx_animation_steps = 3; 
    animation_info.gx_animation_frame_interval = 0; 
    status = gx_animation_drag_enable(&animation, &widget, &animation_info); // animation_info -> gx_animation_style & GX_ANIMATION_EASING_FUNC_MASK
    EXPECT_EQ(GX_INVALID_VALUE, status);

    memset(&animation_info, 0, sizeof(GX_ANIMATION_INFO));
    animation_info.gx_animation_frame_interval = 0; 
    status = gx_animation_drag_enable(&animation, &widget, &animation_info); // should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    animation_info.gx_animation_frame_interval = 1;
    status = gx_animation_drag_enable(&animation, &widget, &animation_info); // should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_animation_landing_speed_set(0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_animation_landing_speed_set(&animation, 0); // should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    status = gx_animation_landing_speed_set(&animation, 1); // should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_animation_start(0, 0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_animation_start(&animation, 0); // info == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&animation_info, 0, sizeof(GX_ANIMATION_INFO));
    animation_info.gx_animation_steps = 1;
    status = gx_animation_start(&animation, &animation_info); // should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    animation_info.gx_animation_steps = 2;
    animation_info.gx_animation_frame_interval = 0;
    status = gx_animation_start(&animation, &animation_info); // gx_animation_frame_interval < 1
    EXPECT_EQ(GX_INVALID_VALUE, status);

    animation_info.gx_animation_steps = 2;
    animation_info.gx_animation_frame_interval = 1;
    animation_info.gx_animation_start_alpha = 0;
    animation_info.gx_animation_end_alpha = 0;
    status = gx_animation_start(&animation, &animation_info); // should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    animation_info.gx_animation_end_alpha = 1;
    animation_info.gx_animation_start_alpha = 1;
    animation_info.gx_animation_target  = 0;
    status = gx_animation_start(&animation, &animation_info); // should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(&widget, 0, sizeof(GX_WIDGET));
    animation_info.gx_animation_target = &widget;
    widget.gx_widget_type = 0;
    status = gx_animation_start(&animation, &animation_info); // should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    widget.gx_widget_type = GX_TYPE_WINDOW;
    animation.gx_animation_status = GX_ANIMATION_IDLE + 1;
    status = gx_animation_start(&animation, &animation_info); // should return GX_INVALID_STATUS
    EXPECT_EQ(GX_INVALID_STATUS, status);
    
    gx_animation_stop(0); // should return GX_PTR_ERROR
    animation.gx_animation_status = GX_ANIMATION_ACTIVE + 1;
    status = gx_animation_stop(&animation); // should return GX_INVALID_STATUS
    EXPECT_EQ(GX_INVALID_STATUS, status);

    memset(&animation, 0, sizeof(GX_ANIMATION));
    animation.gx_animation_status = GX_ANIMATION_ACTIVE;
    _gx_system_animation_list = &animation;
    status = gx_animation_stop(&animation); // should return GX_INVALID_STATUS
    EXPECT_EQ(GX_SUCCESS, status);

    memset(&animation, 0, sizeof(GX_ANIMATION));
    memset(&animation1, 0, sizeof(GX_ANIMATION));
    memset(&animation2, 0, sizeof(GX_ANIMATION));
    animation.gx_animation_status = GX_ANIMATION_ACTIVE;
    _gx_system_animation_list = &animation2;
    animation2.gx_animation_next = &animation1;
    animation1.gx_animation_next = &animation;
    status = gx_animation_stop(&animation); // should return GX_INVALID_STATUS
    EXPECT_EQ(GX_SUCCESS, status);

    /* binary */
    printf("binary\n");
    status = gx_binres_language_table_load(0, &returned_language_table); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_binres_language_table_load((GX_UBYTE *)0x60000, 0); // should return GX_PTR_ERROR;
    EXPECT_EQ(GX_PTR_ERROR, status);

    _gx_system_memory_allocator = memory_allocate;
    _gx_system_memory_free = 0;
    status = gx_binres_language_table_load((GX_UBYTE *)0x60000, &returned_language_table); // _gx_system_memory_free == GX_NULL
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    _gx_system_memory_free = memory_free;
    _gx_system_memory_allocator = 0;
    status = gx_binres_language_table_load((GX_UBYTE *)0x60000, &returned_language_table); // _gx_system_memory_allocate == GX_NULL
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    gx_binres_theme_load(0, 0, 0); // should return GX_PTR_ERROR
    status = gx_binres_theme_load((GX_UBYTE *)buffer, 0, 0); // returned_theme is NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_binres_theme_load((GX_UBYTE *)buffer, -1, &returned_theme); // theme_id < 0
    EXPECT_EQ(GX_INVALID_VALUE, status);

    _gx_system_memory_allocator = 0;
    status = gx_binres_theme_load((GX_UBYTE *)buffer, 0, &returned_theme); // _gx_system_memory_allocator == GX_NULL
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    _gx_system_memory_allocator = memory_allocate;
    _gx_system_memory_free = 0;
    status = gx_binres_theme_load((GX_UBYTE *)buffer, 0, &returned_theme); // _gx_system_memory_free == GX_NULL
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    _gx_system_memory_free = memory_free;
    _gx_system_memory_allocator = 0;

    status = gx_binres_pixelmap_load(GX_NULL, 0, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_binres_pixelmap_load(0x80000, 0, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_binres_font_load(GX_NULL, 0, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_binres_font_load(0x80000, 0, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_binres_font_load(0x80000, 0, (GX_UBYTE *)buffer, 0);
    EXPECT_EQ(GX_PTR_ERROR, status);

    /* brush */
    printf("brush\n");
    status = gx_brush_default(0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_brush_default(&default_context.gx_draw_context_brush); // should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_brush_define(0, 0, 0, 0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_brush_define(&default_context.gx_draw_context_brush, 0, 0, 0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_SUCCESS, status);

    /* button */
    printf("button\n");
    status = gx_button_create(0, 0, 0, 0, 0, 0); // button is NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_button_create(&button, 0, 0, 0, 0, 0); // size is NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_button_create(&button, 0, 0, 0, 0, &size, 0); // should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    button.gx_widget_type = GX_TYPE_BUTTON;
    status = gx_button_create(&button, 0, 0, 0, 0, &size); // should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    memset(&button, 0, sizeof(GX_BUTTON));
    memset(&parent, 0, sizeof(GX_WIDGET));
    status = gx_button_deselect(&button, GX_TRUE); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    button.gx_widget_parent = &parent;
    status = gx_button_deselect(&button, GX_TRUE); // should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    parent.gx_widget_type = GX_TYPE_WIDGET;
    button.gx_widget_type = 0;
    status = gx_button_deselect(&button, GX_TRUE); // should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    button.gx_widget_type = GX_TYPE_WIDGET;
    status = gx_button_deselect(&button, GX_TRUE); // should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_button_event_process(0, 0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_button_event_process(&button, 0); // event_ptr is GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&button, 0, sizeof(GX_BUTTON));
    button.gx_widget_parent = 0;
    my_event.gx_event_type = 0;
    status = gx_button_event_process(&button, &my_event); // should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    memset(&button, 0, sizeof(GX_BUTTON));
    status = gx_button_select(&button); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    button.gx_widget_parent = &parent; // should return GX_INVALID_WIDGET
    button.gx_widget_type = 0;
    status = gx_button_select(&button); // should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    parent.gx_widget_type = 0;
    button.gx_widget_type = GX_TYPE_BUTTON;
    status = gx_button_select(&button); // should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    parent.gx_widget_type = GX_TYPE_BUTTON;
    button.gx_widget_style = GX_STYLE_BUTTON_PUSHED;
    status = gx_button_select(&button); // should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    /* canvas */
    printf("canvas\n");
    status = gx_canvas_alpha_set(0, 0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&canvas, 0, sizeof(GX_CANVAS));
    status = gx_canvas_alpha_set(&canvas, 0); // should return GX_INVALID_CANVAS
    EXPECT_EQ(GX_INVALID_CANVAS, status);
    
    status = gx_canvas_arc_draw(0, 0, 0, 0, 0); // should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    status = gx_canvas_arc_draw(0, 0, 1, 0, 0); // should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    status = gx_canvas_block_move(0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    _gx_system_current_draw_context = 0;
    status = gx_canvas_block_move(&size, 0, 0, 0); //should return GX_FAILURE
    EXPECT_EQ(GX_FAILURE, status);

    status = gx_canvas_circle_draw(0, 0, 0); // should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    status = gx_canvas_circle_draw(0, 0, 1); // should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    status = gx_canvas_create(0, 0, 0, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_canvas_create(&canvas, 0, 0, 0, 0, 0, 0, 0); // display == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&canvas, 0, sizeof(GX_CANVAS));
    status = _gxe_canvas_create(&canvas, 0, &default_display, 0, 0, 0, 0, 0, 0); //should return GX_INVALID_CANVAS_SIZE
    EXPECT_EQ(GX_INVALID_CANVAS_SIZE, status);

    canvas.gx_canvas_display = &default_display;
    status = gx_canvas_create(&canvas, 0, &default_display, 0, 0, 0, 0, 0); //should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    canvas.gx_canvas_display = 0;
    status = gx_canvas_create(&canvas, 0, &default_display, (~((UINT)(GX_CANVAS_SIMPLE | GX_CANVAS_MANAGED | GX_CANVAS_VISIBLE | GX_CANVAS_MODIFIED | GX_CANVAS_COMPOSITE))),
                              0, 0, 0, 0); //should return GX_INVALID_TYPE
    EXPECT_EQ(GX_INVALID_TYPE, status);

    memset(&canvas, 0, sizeof(GX_CANVAS));
    status = gx_canvas_create(&canvas, "canvas", &default_display, GX_CANVAS_SIMPLE, 10, 10, &color, 4); //should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);        
    
    gx_canvas_delete(0); //should return GX_PTR_ERROR
    canvas.gx_canvas_id = GX_CANVAS_ID + 1;
    gx_canvas_delete(&canvas); //should return GX_INVALID_CANVAS
    
    memset(&canvas, 0, sizeof(GX_CANVAS));
    status = gx_canvas_drawing_complete(&canvas, 0);// canvas -> gx_canvas_display is NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    canvas.gx_canvas_display = &display;
    status = gx_canvas_drawing_complete(&canvas, 0);// canvas -> gx_canvas_memory is NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_canvas_drawing_initiate(&canvas, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    canvas.gx_canvas_id = GX_CANVAS_ID + 1;
    status = gx_canvas_drawing_initiate(&canvas, 0, &size); //should return GX_INVALID_CANVAS
    EXPECT_EQ(GX_INVALID_CANVAS, status);

    status = gx_canvas_ellipse_draw(0, 0, 0, 1); // should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    status = gx_canvas_ellipse_draw(0, 0, 1, 1); // should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    status = gx_canvas_hardware_layer_bind(0, 1);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&canvas, 0, sizeof(GX_CANVAS));
    canvas.gx_canvas_id = GX_CANVAS_ID + 1;
    status = gx_canvas_hardware_layer_bind(&canvas, 1);//should return GX_INVALID_CANVAS
    EXPECT_EQ(GX_INVALID_CANVAS, status);

    canvas.gx_canvas_id = GX_CANVAS_ID;
    status = gx_canvas_hardware_layer_bind(&canvas, 1);//should return GX_INVALID_DISPLAY
    EXPECT_EQ(GX_INVALID_DISPLAY, status);

    memset(&display, 0, sizeof(GX_DISPLAY));
    canvas.gx_canvas_display = &display;
    status = gx_canvas_hardware_layer_bind(&canvas, 0);//should return GX_NOT_SUPPORTED
    EXPECT_EQ(GX_NOT_SUPPORTED, status);
    
    status = gx_canvas_hide(0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    canvas.gx_canvas_id = GX_CANVAS_ID + 1;
    status = gx_canvas_hide(&canvas);//should return GX_INVALID_CANVAS
    EXPECT_EQ(GX_INVALID_CANVAS, status);

    canvas.gx_canvas_id = GX_CANVAS_ID;
    canvas.gx_canvas_hardware_layer = 1;
    canvas.gx_canvas_display = &display;
    display.gx_display_layer_services = &display_layer_service;
    display_layer_service.gx_display_layer_hide = GX_NULL;
    status = gx_canvas_hide(&canvas);
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_canvas_line_draw(0,0,0,0);//should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);

    _gx_system_current_draw_context = &default_context;
    memset(&default_context, 0, sizeof(GX_DRAW_CONTEXT));
    status = gx_canvas_line_draw(0,0,0,0);//should return GX_INVALID_WIDTH
    EXPECT_EQ(GX_SUCCESS, status);
   
    /* gx_canvas_memory_define. */ 
    status = gx_canvas_memory_define(GX_NULL, GX_NULL, 0);
    EXPECT_EQ(GX_PTR_ERROR, status);

    canvas.gx_canvas_id = 0;
    status = gx_canvas_memory_define(&canvas, GX_NULL, 0);
    EXPECT_EQ(GX_INVALID_CANVAS, status);

    status = gx_canvas_offset_set(0, 0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&canvas, 0, sizeof(GX_CANVAS));
    canvas.gx_canvas_display = &display;
    canvas.gx_canvas_id = GX_CANVAS_ID + 1;
    canvas.gx_canvas_memory = canvas_memory;
    status = gx_canvas_offset_set(&canvas, 0, 0);//should return GX_INVALID_CANVAS
    EXPECT_EQ(GX_INVALID_CANVAS, status);

    canvas.gx_canvas_display = 0;
    status = gx_canvas_offset_set(&canvas, 0, 0);// gx_canvas_display == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    canvas.gx_canvas_display = &display;
    canvas.gx_canvas_memory = 0;
    status = gx_canvas_offset_set(&canvas, 0, 0);// gx_canvas_memory == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    canvas.gx_canvas_id = GX_CANVAS_ID;
    canvas.gx_canvas_display = &display;
    canvas.gx_canvas_memory = canvas_memory;
    canvas.gx_canvas_hardware_layer = 1;
    display.gx_display_layer_services = &display_layer_service;
    display_layer_service.gx_display_layer_offset_set = 0;
    canvas.gx_canvas_status &= ~GX_CANVAS_MANAGED;
    status = gx_canvas_offset_set(&canvas, 0, 0);
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_canvas_pie_draw(0, 0, 0, 0, 0); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    _gx_system_current_draw_context = GX_NULL;
    status = gx_canvas_pie_draw(0, 0, 10, 0, 0); //should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    status = gx_canvas_pixel_draw(point); //should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);

    memset(&point, 0, sizeof(GX_POINT));
    memset(&default_context, 0, sizeof(GX_DRAW_CONTEXT));
    memset(&display, 0, sizeof(GX_DISPLAY));
    _gx_system_current_draw_context = &default_context;
    default_context.gx_draw_context_display = &display;
    status = gx_canvas_pixel_draw(point); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_canvas_pixelmap_blend(0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    _gx_system_current_draw_context = 0;
    status = gx_canvas_pixelmap_blend(0, 0, &pixelmap, 0); //should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);

    _gx_system_current_draw_context = &default_context;
    status = gx_canvas_pixelmap_blend(0, 0, &pixelmap, 0); //should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_canvas_pixelmap_draw(0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    _gx_system_current_draw_context = 0;
    status = gx_canvas_pixelmap_draw(0, 0, &pixelmap); //should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);

    _gx_system_current_draw_context = &default_context;
    status = gx_canvas_pixelmap_draw(0, 0, &pixelmap); //should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_canvas_pixelmap_get(0); //should return GX_PTR_ERROR   
    EXPECT_EQ(GX_PTR_ERROR, status);

    _gx_system_current_draw_context = &default_context;
    default_context.gx_draw_context_canvas = &canvas;
    default_context.gx_draw_context_display = &display;
    status = gx_canvas_pixelmap_get(&pixelmap); //should return GX_PTR_ERROR   
    EXPECT_EQ(GX_SUCCESS, status);
        
    status = gx_canvas_pixelmap_rotate(0, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    _gx_system_current_draw_context = 0;
    status = gx_canvas_pixelmap_rotate(0, 0, &pixelmap, 0, 0, 0); //should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);

    _gx_system_current_draw_context = &default_context;
    status = gx_canvas_pixelmap_rotate(0, 0, &pixelmap, 0, 0, 0); //should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_canvas_pixelmap_tile(0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_canvas_pixelmap_tile(&size, 0); //should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_PTR_ERROR, status);

    _gx_system_current_draw_context = 0;
    status = gx_canvas_pixelmap_tile(&size, &pixelmap); //should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);

    _gx_system_current_draw_context = &default_context;
    size.gx_rectangle_bottom = size.gx_rectangle_right = 0; 
    size.gx_rectangle_left = size.gx_rectangle_right + 1;
    size.gx_rectangle_top = size.gx_rectangle_bottom + 1;
    status = gx_canvas_pixelmap_tile(&size, &pixelmap); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    size.gx_rectangle_left = size.gx_rectangle_right = 0;
    size.gx_rectangle_top = size.gx_rectangle_bottom + 1;
    status = gx_canvas_pixelmap_tile(&size, &pixelmap); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    memset(&size, 0, sizeof(GX_RECTANGLE));
    memset(&default_context.gx_draw_context_dirty, 0, sizeof(GX_RECTANGLE));
    _gx_system_current_draw_context = &default_context;
    memset(&pixelmap, 0, sizeof(GX_PIXELMAP));
    status = gx_canvas_pixelmap_tile(&size, &pixelmap); //should return GX_SUCCESS
    EXPECT_EQ(GX_FAILURE, status);

    memset(&pixelmap, 0, sizeof(GX_PIXELMAP));
    pixelmap.gx_pixelmap_width = 1;
    pixelmap.gx_pixelmap_height = 0;
    status = gx_canvas_pixelmap_tile(&size, &pixelmap);
    EXPECT_EQ(GX_FAILURE, status);
    
    status = gx_canvas_polygon_draw(0, 0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    _gx_system_current_draw_context = GX_NULL;
    status = gx_canvas_polygon_draw(&point, 0); // should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);

    status = gx_canvas_rectangle_draw(0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    _gx_system_current_draw_context = GX_NULL;
    status = gx_canvas_rectangle_draw(&size); //should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    status = gx_canvas_rotated_text_draw(0, 0, 0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_canvas_rotated_text_draw("rotate text", 0, 0, 0);// _gx_system_memory_allocator is GX_NULL
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    _gx_system_memory_allocator = memory_allocate;
    _gx_system_memory_free = 0;
    status = gx_canvas_rotated_text_draw("rotate text", 0, 0, 0);// _gx_system_memory_allocator is GX_NULL
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    _gx_system_memory_allocator = 0;
    
    status = gx_canvas_shift(0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    canvas.gx_canvas_display = 0;
    status = gx_canvas_shift(&canvas, 0, 0);
    EXPECT_EQ(GX_PTR_ERROR, status);

    canvas.gx_canvas_display = &default_display;
    canvas.gx_canvas_memory = 0;
    status = gx_canvas_shift(&canvas, 0, 0);
    EXPECT_EQ(GX_PTR_ERROR, status);

    canvas.gx_canvas_id = GX_CANVAS_ID + 1;
    canvas.gx_canvas_display = &display;
    canvas.gx_canvas_memory = (VOID *)buffer;
    status = gx_canvas_shift(&canvas, 0, 0); //should return GX_INVALID_CANVAS
    EXPECT_EQ(GX_INVALID_CANVAS, status);
    
    gx_canvas_show(0); //should return GX_PTR_ERROR
    canvas.gx_canvas_id = GX_CANVAS_ID + 1;
    status = gx_canvas_show(&canvas); //should return GX_INVALID_CANVAS
    EXPECT_EQ(GX_INVALID_CANVAS, status);
    
    memset(&canvas, 0, sizeof(GX_CANVAS));
    canvas.gx_canvas_id = GX_CANVAS_ID;
    canvas.gx_canvas_hardware_layer = -1;
    status = gx_canvas_show(&canvas);
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_canvas_hide(&canvas);
    EXPECT_EQ(GX_SUCCESS, status);

    canvas.gx_canvas_id = GX_CANVAS_ID;
    canvas.gx_canvas_hardware_layer = 1;
    canvas.gx_canvas_display = &display;
    display.gx_display_layer_services = &display_layer_service;
    display_layer_service.gx_display_layer_hide = GX_NULL;
    status = gx_canvas_show(&canvas);
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_canvas_text_draw(0, 0, 0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    _gx_system_current_draw_context = GX_NULL;
    status = gx_canvas_text_draw(0, 0, "text", 0);//should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    /* checkbox */
    printf("checkbox\n");
    memset(&checkbox, 0, sizeof(GX_CHECKBOX));
    gx_checkbox_create(0, "checkbox", 0, 0, 0, 0, 0); // should return GX_PTR_ERROR
    status = gx_checkbox_create(&checkbox, "checkbox", 0, 0, 0, 0, 0); // size is NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_checkbox_create(&checkbox, "checkbox", 0, 0, 0, 0, &size, 0); // should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    checkbox.gx_widget_type = GX_TYPE_CHECKBOX;
    status = gx_checkbox_create(&checkbox, "checkbox", 0, 0, 0, 0, &size); // should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    memset(&checkbox, 0, sizeof(GX_CHECKBOX));
    status = gx_checkbox_create(&checkbox, "checkbox", 0, 0, 0, 0, &size); // parent is NULL
    EXPECT_EQ(GX_SUCCESS, status);
    
    gx_checkbox_event_process(&checkbox, 0); // should return GX_PTR_ERROR
    checkbox.gx_widget_parent = &parent;
    checkbox.gx_widget_type = 0;
    status = gx_checkbox_event_process(&checkbox, &my_event); // should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    checkbox.gx_widget_type = GX_TYPE_CHECKBOX;
    parent.gx_widget_type = 0;
    status = gx_checkbox_event_process(&checkbox, &my_event); // should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    parent.gx_widget_type = GX_TYPE_WIDGET;
    my_event.gx_event_type = 0;
    gx_widget_event_process_set(&parent, event_process_func);
    gx_checkbox_event_process(&checkbox, &my_event); // should return GX_INVALID_WIDGET
    checkbox.gx_widget_parent = &parent;
    status = gx_checkbox_event_process(&checkbox, 0); // event_ptr is NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    gx_checkbox_pixelmap_set(0, 0, 0, 0, 0); // should return GX_PTR_ERROR
    
    memset(&checkbox, 0, sizeof(GX_CHECKBOX));
    status = gx_checkbox_select(&checkbox);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    checkbox.gx_widget_parent = &parent;
    parent.gx_widget_type = 0;
    status = gx_checkbox_select(&checkbox);// should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    parent.gx_widget_type = GX_TYPE_WIDGET;
    checkbox.gx_widget_type = 0;
    status = gx_checkbox_select(&checkbox);// should return GX_INVALID_WIDGET    
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    checkbox.gx_widget_parent = &parent;
    checkbox.gx_widget_type = GX_TYPE_CHECKBOX;
    status = gx_checkbox_select(&checkbox);// should return GX_SUCCESS        
    EXPECT_EQ(GX_SUCCESS, status);

    /* circular gauge */
    printf("circular gauge\n");
    memset(&circular_gauge, 0, sizeof(GX_CIRCULAR_GAUGE));
    status = gx_circular_gauge_angle_get(&circular_gauge, 0);//// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_circular_gauge_angle_get(&circular_gauge, (INT *)&color);//// should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_circular_gauge_angle_set(0, 10);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    memset(&circular_gauge, 0, sizeof(GX_CIRCULAR_GAUGE));
    status = gx_circular_gauge_animation_set(0, 10, 10); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_circular_gauge_animation_set(&circular_gauge, 10, 0); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    status = gx_circular_gauge_animation_set(&circular_gauge, 0, 0); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_circular_gauge_create(0, 0, 0, 0, 0, 0, 0, 0, 0); // circular_gauge is NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_circular_gauge_create(&circular_gauge, 0, 0, 0, 0, 0, 0, 0, 0); // circular_gauge_info is NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_circular_gauge_create(&circular_gauge, 0, 0, &circular_gauge_info, 0, 0, 0, 0, 0, 0); // should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    circular_gauge.gx_widget_type = GX_TYPE_CIRCULAR_GAUGE;
    status = gx_circular_gauge_create(&circular_gauge, 0, 0, &circular_gauge_info, 0, 0, 0, 0, 0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    memset(&circular_gauge, 0, sizeof(circular_gauge));
    status = gx_circular_gauge_create(&circular_gauge, 0, 0, &circular_gauge_info, 0, 0, 0, 0, 0); // parent is NULL
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_circular_gauge_event_process(&circular_gauge, 0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    my_event.gx_event_type = 0;
    status = gx_circular_gauge_event_process(&circular_gauge, &my_event); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_SUCCESS, status);
    
    /* context brush */
    printf("context brush\n");
    status = gx_context_brush_default(0);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_context_brush_default(&default_context);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_SUCCESS, status);

    _gx_system_current_draw_context = GX_NULL;
    status = gx_context_brush_define(0, 0, 0);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_INVALID_CONTEXT, status);

    status = gx_context_brush_get(0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    _gx_system_current_draw_context = &default_context;
    default_context.gx_draw_context_display = &default_display;
    default_display.gx_display_color_table_size = 0;
    status = gx_context_color_get(1, &color); // should return GX_INVALID_RESOURCE_ID
    EXPECT_EQ(GX_INVALID_RESOURCE_ID, status);
    
    _gx_system_current_draw_context = &default_context;
    memset(&default_context, 0, sizeof(GX_DRAW_CONTEXT));
    status = gx_context_color_get(1, &color);//should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_DISPLAY, status);

    _gx_system_current_draw_context = &default_context;
    default_context.gx_draw_context_display = &display;
    display.gx_display_color_table_size = 4;
    display.gx_display_color_table = 0;
    status = gx_context_color_get(1, &color);
    EXPECT_EQ(GX_INVALID_RESOURCE_ID, status);
    
    _gx_system_current_draw_context = GX_NULL; // following functions need the _gx_system_current_draw_context to be NULL
    // start
    status = gx_context_brush_get(&brush); // should return GX_INVALID_CONTEXT   
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    status = gx_context_brush_pattern_set(0); // should return GX_INVALID_CONTEXT   
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    status = gx_context_brush_set(0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_context_brush_set(&default_context.gx_draw_context_brush); //should return GX_INVALID_CONTEXT   
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    status = gx_context_brush_style_set(0);//should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    status = gx_context_brush_width_set(0);//should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    status = gx_context_color_get(1, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_context_color_get(1, &color);//should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    status = gx_context_fill_color_set(0);//should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    status = gx_context_font_get(1, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_context_font_get(1, &default_context.gx_draw_context_brush.gx_brush_font);//should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);

    status = gx_context_font_set(0);//should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    status = gx_context_line_color_set(1);//should return GX_INVALID_CONTEXT 
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    status = gx_context_pixelmap_get(1, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_context_pixelmap_get(1, &default_context.gx_draw_context_brush.gx_brush_pixelmap);//should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    status = gx_context_pixelmap_set(1);//should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    status = gx_context_raw_brush_define(1, 1, 1);//should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    status = gx_context_raw_fill_color_set(1); //should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    
    status = gx_context_raw_line_color_set(1); //should return GX_INVALID_CONTEXT
    EXPECT_EQ(GX_INVALID_CONTEXT, status);
    //end
    
    _gx_system_current_draw_context = &default_context;
    default_context.gx_draw_context_display = &display;
    display.gx_display_pixelmap_table_size = 0;
    status = gx_context_pixelmap_get(1, &return_pixelmap);
    EXPECT_EQ(GX_INVALID_RESOURCE_ID, status);
    
    /* display */
    printf("display\n");    
    status = gx_display_active_language_set(0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&display, 0, sizeof(GX_DISPLAY));
    status = gx_display_active_language_set(&display, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
    display.gx_display_language_table_deprecated = (GX_CONST GX_CHAR ***)&langugage_table;
    display.gx_display_language_table_size = 0;
    status = gx_display_active_language_set(&display, 1); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);
#endif
    
    status = gx_display_color_set(0, 1, 1);//should return GX_INVALID_DISPLAY
    EXPECT_EQ(GX_INVALID_DISPLAY, status);

    memset(&default_display, 0, sizeof(GX_DISPLAY));
    status = gx_display_color_set(&default_display, 1024, 1);//should return GX_INVALID_RESOURCE_ID
    EXPECT_EQ(GX_INVALID_RESOURCE_ID, status);
    
    status = gx_display_color_table_set(0, 0, 2); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_display_color_table_set(0, &color, 1); // should return GX_INVALID_DISPLAY
    EXPECT_EQ(GX_INVALID_DISPLAY, status);
    
    status = gx_display_create(0, 0, 0, 0, 0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_display_create(&display, "linux_display", gx_validation_display_driver_setup_24xrgb, 0, 0, 0); // Should return GX_INVALID_SIZE 
    EXPECT_EQ(GX_INVALID_SIZE, status);
    
    /* display delete coverage */
    status = gx_display_delete(0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_display_delete(&display, 0);// display_driver_cleanup == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_display_create(&display, "display_1", 0, 0, 0); // display_driver_setup is NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    gx_display_create(&display, "display_1", gx_validation_display_driver_setup_24xrgb, 0, 0); //create display
    gx_display_create(&display_1, "display_1", gx_validation_display_driver_setup_24xrgb, 0, 0); //create display
    gx_display_create(&display_2, "display_2", gx_validation_display_driver_setup_24xrgb, 0, 0); //create display
    status = gx_display_delete(&display, display_cleanup_func);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    gx_display_delete(&display, display_cleanup_func); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_display_delete(&display_1, display_cleanup_func); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_display_delete(&display_2, display_cleanup_func); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_display_font_table_set(0, 0, 1);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_display_language_table_get(0, 0, 0, 0); // _gx_system_display_created_list == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    _gx_system_display_created_list = &default_display;
    status = gx_display_language_table_get(0, 0, 0, 0); // display == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_display_language_table_get(&default_display, (GX_CHAR ****)&default_display.gx_display_language_table, &default_display.gx_display_language_table_size, &default_display.gx_display_string_table_size);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    _gx_system_display_created_list = &default_display;
    status = gx_display_language_table_get(&default_display, (GX_CHAR ****)&default_display.gx_display_language_table, &default_display.gx_display_language_table_size, &default_display.gx_display_string_table_size);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_display_language_table_set(0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_display_language_table_set(&display, 0, 0, 0); // number_of_strings == 0
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_display_language_table_set(&display, 0, 0, 1); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_display_pixelmap_table_set(0, 0, 1);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
   
    status = gx_display_string_table_get(0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&display, 0, sizeof(GX_DISPLAY));
    status = gx_display_string_table_get(&display, 0, 0, 0); //should return GX_NOT_FOUND
    EXPECT_EQ(GX_NOT_FOUND, status);

#if defined(GX_ENABLE_DEPRECATED_STRING_API)
    display.gx_display_language_table_deprecated = (GX_CONST GX_CHAR ***)&langugage_table;
    display.gx_display_language_table_size = 1;
    status = gx_display_string_table_get(&display, 0, &langugage_table, (UINT *)&color); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
#endif
    
    status = gx_display_theme_install(0, 0);//should return GX_INVALID_DISPLAY   
    EXPECT_EQ(GX_INVALID_DISPLAY, status);

    status = gx_display_theme_install(&display, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    printf("drop list\n");
    status = gx_drop_list_close(0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    drop_list.gx_drop_list_popup_open = 0;
    status = gx_drop_list_close(&drop_list);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_drop_list_create(&drop_list, 0, 0, 0, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_drop_list_create(&drop_list, 0, 0, 0, 0, 0, 0, 0, &size, 0); //should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    drop_list.gx_widget_type = GX_TYPE_DROP_LIST;
    status = gx_drop_list_create(&drop_list, 0, 0, 0, 0, 0, 0, 0, &size); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    memset(&drop_list, 0, sizeof(drop_list));
    status = gx_drop_list_create(&drop_list, 0, 0, 0, 0, 0, 0, 0, &size); // no parent
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_drop_list_event_process(&drop_list, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    drop_list.gx_widget_parent = 0;
    my_event.gx_event_type = 0;
    status = gx_drop_list_event_process(&drop_list, &my_event); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_drop_list_open(0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    drop_list.gx_drop_list_popup.gx_popup_list_list.gx_widget_status = GX_STATUS_VISIBLE;
    status = gx_drop_list_open(&drop_list);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_drop_list_pixelmap_set(0, 1);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_drop_list_popup_get(0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_drop_list_popup_get(&drop_list, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    /* horizontal list */
    printf("horizontal list\n");
    status = gx_horizontal_list_children_position(0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&horizontal_list, 0, sizeof(GX_HORIZONTAL_LIST));
    status = gx_horizontal_list_children_position(&horizontal_list); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_SUCCESS, status);
    
    memset(&horizontal_list, 0, sizeof(GX_HORIZONTAL_LIST));
    status = gx_horizontal_list_create(&horizontal_list, 0, 0, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_horizontal_list_create(&horizontal_list, 0, 0, 0, 0, 0, 0, &size, 0); //should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    horizontal_list.gx_widget_type = GX_TYPE_HORIZONTAL_LIST;
    status = gx_horizontal_list_create(&horizontal_list, 0, 0, 0, 0, 0, 0, &size); //should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    horizontal_list.gx_widget_type = 0;
    status = gx_horizontal_list_create(&horizontal_list, 0, 0, 0, 0, 0, 0, &size); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    memset(&horizontal_list, 0, sizeof(GX_HORIZONTAL_LIST));
    status = gx_horizontal_list_create(&horizontal_list, 0, 0, 1, 0, 0, 0, &size); // parent is NULL
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_horizontal_list_event_process(0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_horizontal_list_event_process(&horizontal_list, 0); // event_ptr == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    horizontal_list.gx_widget_parent = 0;
    my_event.gx_event_type = 0;
    status = gx_horizontal_list_event_process(&horizontal_list, &my_event); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_horizontal_list_page_index_set(0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_horizontal_list_page_index_set(&horizontal_list, -1); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);
    
    status = gx_horizontal_list_selected_index_get(0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_horizontal_list_selected_index_get(&horizontal_list, (INT *)&color); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_horizontal_list_selected_index_get((VOID *)0x01, 0); // return_list_entry == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_horizontal_list_selected_set(0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    horizontal_list.gx_widget_type = 0;
    status = gx_horizontal_list_selected_set(&horizontal_list, 0); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_horizontal_list_selected_widget_get(0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_horizontal_list_selected_widget_get((VOID *)0x01, 0); // return_list_entry == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_horizontal_list_total_columns_set(0, 10); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_horizontal_list_total_columns_set(&horizontal_list, -10); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);
    
    memset(&scrollbar, 0, sizeof(GX_SCROLLBAR));
    status = gx_horizontal_scrollbar_create(0, 0, 0, 0, 0);//should return GX_PTR_ERROR  
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_horizontal_scrollbar_create(&scrollbar, 0, 0, 0, 0, 0);//should return GX_INVALID_SIZE  
    EXPECT_EQ(GX_INVALID_SIZE, status);

    scrollbar.gx_widget_type = GX_TYPE_HORIZONTAL_SCROLL;
    status = gx_horizontal_scrollbar_create(&scrollbar, 0, 0, 0, 0);//should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);
    
    /* icon */
    printf("icon button\n");
    memset(&icon_button, 0, sizeof(GX_ICON_BUTTON));
    status = gx_icon_button_create(&icon_button, 0, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR  
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_icon_button_create(&icon_button, 0, 0, 0, 0, 0, &size, 0);//should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    icon_button.gx_widget_type = GX_TYPE_ICON_BUTTON;
    status = gx_icon_button_create(&icon_button, 0, 0, 0, 0, 0, &size);//should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    memset(&icon_button, 0, sizeof(GX_ICON_BUTTON));
    status = gx_icon_button_create(&icon_button, 0, 0, 0, 0, 0, &size);// parent is NULL
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_icon_button_pixelmap_set(0, 1);//should return GX_PTR_ERROR  
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    /* icon */
    printf("icon\n");
    memset(&icon, 0, sizeof(GX_ICON));
    status = gx_icon_create(0, 0, 0, 0, 0, 0, 0, 0); // should return GX_PTR_ERROR  
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_icon_create(&icon, 0, 0, 0, 0, 0, 0, 0, 0); // should return GX_INVALID_SIZE  
    EXPECT_EQ(GX_INVALID_SIZE, status);

    icon.gx_widget_type = GX_TYPE_ICON;
    status = gx_icon_create(&icon, 0, 0, 0, 0, 0, 0, 0); // should return GX_PTR_ERROR  
    EXPECT_EQ(GX_ALREADY_CREATED, status);
    
    status = gx_icon_event_process(&icon, 0); //should return GX_PTR_ERROR  
    EXPECT_EQ(GX_PTR_ERROR, status);

    icon.gx_widget_parent = 0;
    my_event.gx_event_type = 0;
    status = gx_icon_event_process(&icon, &my_event); //should return GX_PTR_ERROR  
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_icon_pixelmap_set(0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    /* image reader */
    printf("image reader\n");
    status = gx_image_reader_create(&image_reader, 0, 0, 0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_image_reader_create(&image_reader, (GX_CONST GX_UBYTE *)byte_data, 0, 0, 0);//should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);
    
    status = gx_image_reader_palette_set(&image_reader, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_image_reader_palette_set(&image_reader, canvas_memory, 0);//should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);
    
    status = gx_image_reader_start(&image_reader, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    /* line chart */
    printf("line chart\n");
    memset(&line_chart, 0, sizeof(GX_LINE_CHART));
    status = gx_line_chart_create(&line_chart, 0, 0, 0, 0, 0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_line_chart_create(&line_chart, 0, 0, 0, 0, 0, &size, 0);//should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    line_chart.gx_widget_type = GX_TYPE_LINE_CHART;
    status = gx_line_chart_create(&line_chart, 0, 0, 0, 0, 0, &size);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    memset(&line_chart, 0, sizeof(GX_LINE_CHART));
    status = gx_line_chart_create(&line_chart, 0, 0, &line_chart_info, 0, 0, &size);// parent is NULL
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_line_chart_update(0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    gx_line_chart_y_scale_calculate(0, 0); //should return GX_PTR_ERROR
    status = gx_line_chart_y_scale_calculate(&line_chart, 0); // return_value == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    /* menu */
    printf("menu\n");
    memset(&menu, 0, sizeof(GX_MENU));
    status = gx_menu_create(&menu, 0, 0, 0, 0, 0, 0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_menu_create(&menu, 0, 0, 0, 0, 0, 0, &size, 0);//should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    menu.gx_widget_type = GX_TYPE_MENU;
    status = gx_menu_create(&menu, 0, 0, 0, 0, 0, 0, &size);//should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    memset(&menu, 0, sizeof(GX_MENU));
    status = gx_menu_create(&menu, 0, 0, 0, 0, 0, 0, &size);// parent is NULL
    EXPECT_EQ(GX_SUCCESS, status);
    
    gx_menu_insert(0, 0); // should return GX_PTR_ERROR
    status = gx_menu_insert(&menu, 0); // widget == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    gx_menu_remove(0, 0); // should return GX_PTR_ERROR
    status = gx_menu_remove(&menu, 0); // widget == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&menu, 0, sizeof(GX_MENU));
    memset(&widget, 0, sizeof(GX_WIDGET));
    status = gx_menu_remove(&menu, &widget); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_menu_text_offset_set(0, 1, 2);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    /* multi line text button */
    printf("multi line text button\n");
    memset(&multi_line_text_button, 0, sizeof(GX_MULTI_LINE_TEXT_BUTTON));
    status = gx_multi_line_text_button_create(&multi_line_text_button, 0, 0, 0, 0, 0, 0);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_multi_line_text_button_create(&multi_line_text_button, 0, 0, 0, 0, 0, &size, 0);// should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    multi_line_text_button.gx_widget_type = GX_TYPE_MULTI_LINE_TEXT_BUTTON;
    status = gx_multi_line_text_button_create(&multi_line_text_button, 0, 0, 0, 0, 0, &size);// should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    memset(&multi_line_text_button, 0, sizeof(GX_MULTI_LINE_TEXT_BUTTON));
    status = gx_multi_line_text_button_create(&multi_line_text_button, 0, 0, 0, 0, 0, &size);// parent is NULL
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_multi_line_text_button_event_process(&multi_line_text_button, 0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    my_event.gx_event_type = 0;
    status = gx_multi_line_text_button_event_process(&multi_line_text_button, &my_event); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_multi_line_text_button_text_id_set(0, 1); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_multi_line_text_button_text_set(0, 0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&multi_line_text_button, 0, sizeof(GX_MULTI_LINE_TEXT_BUTTON));
    multi_line_text_button.gx_widget_style = GX_STYLE_TEXT_COPY;
    _gx_system_memory_allocator = GX_NULL;
    status = gx_multi_line_text_button_text_set(&multi_line_text_button, 0); // should return GX_SYSTEM_MEMORY_ERROR
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);
    
    /* multi line text input */
    memset(&multi_line_text_input, 0, sizeof(GX_MULTI_LINE_TEXT_INPUT));

    status = gx_multi_line_text_input_backspace(&multi_line_text_input);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_multi_line_text_input_buffer_clear(0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_multi_line_text_input_char_insert(&multi_line_text_input, GX_NULL, 0);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_multi_line_text_input_char_insert(&multi_line_text_input, byte_data, 0);
    EXPECT_EQ(GX_INVALID_VALUE, status);

    status = gx_multi_line_text_input_char_insert(&multi_line_text_input, byte_data, 1);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_multi_line_text_input_buffer_get(0, 0, 0, 0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_multi_line_text_input_create(&multi_line_text_input, 0, 0, (GX_CHAR *)byte_data, 0, 0, 0, 0);// size == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_multi_line_text_input_create(&multi_line_text_input, 0, 0, (GX_CHAR *)byte_data, 0, 0, 0, &size, 0);// should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    parent.gx_widget_type = 0;
    status = gx_multi_line_text_input_create(&multi_line_text_input, 0, &parent, (GX_CHAR *)byte_data, 0, 0, 0, &size);// should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    multi_line_text_input.gx_widget_type = GX_TYPE_MULTI_LINE_TEXT_INPUT;
    status = gx_multi_line_text_input_create(&multi_line_text_input, 0, 0, (GX_CHAR *)byte_data, 0, 0, 0, &size);// should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    status = gx_multi_line_text_input_cursor_pos_get(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_multi_line_text_input_cursor_pos_get(&multi_line_text_input, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_multi_line_text_input_cursor_pos_get(&multi_line_text_input, &cursor_pos);
    EXPECT_EQ(GX_SUCCESS, status);
    EXPECT_EQ(multi_line_text_input.gx_multi_line_text_input_cursor_instance.gx_text_input_cursor_pos.gx_point_x, cursor_pos.gx_point_x);
    EXPECT_EQ(multi_line_text_input.gx_multi_line_text_input_cursor_instance.gx_text_input_cursor_pos.gx_point_y, cursor_pos.gx_point_y);

    multi_line_text_input.gx_widget_type = 0;
    status = gx_multi_line_text_input_cursor_pos_get(&multi_line_text_input, &cursor_pos);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_multi_line_text_input_delete(&multi_line_text_input);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_multi_line_text_input_down_arrow(&multi_line_text_input);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_multi_line_text_input_end(&multi_line_text_input);
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_multi_line_text_input_event_process(0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_multi_line_text_input_event_process(&multi_line_text_input, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_multi_line_text_input_event_process(&multi_line_text_input, &my_event);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_multi_line_text_input_fill_color_set(0, 1, 2, 3, 4);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_multi_line_text_input_fill_color_set(&multi_line_text_input, 1, 2, 3, 4);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_SUCCESS, 0);

    status = gx_multi_line_text_input_home(&multi_line_text_input);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_multi_line_text_input_left_arrow(&multi_line_text_input);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_multi_line_text_input_right_arrow(&multi_line_text_input);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_multi_line_text_input_style_add(0, 1);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_multi_line_text_input_style_remove(0, 1);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_multi_line_text_input_style_set(0, 1);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_multi_line_text_input_style_set(&multi_line_text_input, 0); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_multi_line_text_input_text_color_set(0, 1, 2, 3, 4);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_multi_line_text_input_text_select(0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_multi_line_text_input_text_select(&multi_line_text_input, 0, 0); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    multi_line_text_input.gx_widget_type = GX_TYPE_MULTI_LINE_TEXT_INPUT;
    status = gx_multi_line_text_input_text_select(&multi_line_text_input, 0, 0); // start_index >= string_byte_count
    EXPECT_EQ(GX_INVALID_VALUE, status);

    multi_line_text_input.gx_multi_line_text_view_text.gx_string_length = 1;
    status = gx_multi_line_text_input_text_select(&multi_line_text_input, 0, 2); // end_index >= string_byte_count
    EXPECT_EQ(GX_INVALID_VALUE, status);
    
    status = gx_multi_line_text_input_text_set(0, (GX_CONST GX_CHAR *)byte_data);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    multi_line_text_input.gx_widget_type = 0;
    status = gx_multi_line_text_input_up_arrow(&multi_line_text_input);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    /* multi line text view */
    memset(&multi_line_text_view, 0, sizeof(GX_MULTI_LINE_TEXT_VIEW));
    status = gx_multi_line_text_view_create(&multi_line_text_view, 0, 0, 0, 0, 0, 0);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_multi_line_text_view_create(&multi_line_text_view, 0, 0, 0, 0, 0, &size, 0);// should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    multi_line_text_view.gx_widget_type = GX_TYPE_MULTI_LINE_TEXT_VIEW;
    status = gx_multi_line_text_view_create(&multi_line_text_view, 0, 0, 0, 0, 0, &size);// should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    status = gx_multi_line_text_view_event_process(&multi_line_text_view, 0);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    multi_line_text_view.gx_widget_type = 0;
    status = gx_multi_line_text_view_event_process(&multi_line_text_view, &my_event);// should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    multi_line_text_view.gx_widget_type = GX_TYPE_MULTI_LINE_TEXT_VIEW;
    my_event.gx_event_type = 0;
    status = gx_multi_line_text_view_event_process(&multi_line_text_view, &my_event);// should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_multi_line_text_view_font_set(0, 1);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_multi_line_text_view_line_space_set(0, 1);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_multi_line_text_view_scroll_info_get(&multi_line_text_view, 0, 0);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_multi_line_text_view_scroll_info_get(&multi_line_text_view, 0, &scroll_info);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_FAILURE, status);

    status = gx_multi_line_text_view_text_color_set(0, 1, 2, 3);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_multi_line_text_view_text_id_set(0, 1);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_multi_line_text_view_text_set(0, (GX_CONST GX_CHAR *)byte_data);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    multi_line_text_view.gx_widget_style = GX_STYLE_TEXT_COPY;
    _gx_system_memory_allocator = GX_NULL;
    status = gx_multi_line_text_view_text_set(&multi_line_text_view, (GX_CONST GX_CHAR *)byte_data);// should return GX_SYSTEM_MEMORY_ERROR
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);
        
    status = gx_multi_line_text_view_whitespace_set(0, 1);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    /* numeric pixelmap prompt */
    printf("numeric pixelmap prompt\n");
    memset(&numeric_pixelmap_prompt, 0, sizeof(GX_NUMERIC_PIXELMAP_PROMPT));
    status = gx_numeric_pixelmap_prompt_create(&numeric_pixelmap_prompt, 0, 0, 0, 0, 0, 0, 0);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_numeric_pixelmap_prompt_create(&numeric_pixelmap_prompt, 0, 0, 0, 0, 0, 0, &size, 0);// should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    numeric_pixelmap_prompt.gx_widget_type = GX_TYPE_NUMERIC_PIXELMAP_PROMPT;
    status = gx_numeric_pixelmap_prompt_create(&numeric_pixelmap_prompt, 0, 0, 0, 0, 0, 0, &size);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    memset(&numeric_pixelmap_prompt, 0, sizeof(GX_NUMERIC_PIXELMAP_PROMPT));
    status = gx_numeric_pixelmap_prompt_create(&numeric_pixelmap_prompt, 0, 0, 0, 0, 0, 0, &size);// parent is NULL
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_numeric_pixelmap_prompt_format_function_set(&numeric_pixelmap_prompt, 0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_numeric_pixelmap_prompt_value_set(0, 0);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    /* numeric prompt */
    printf("numeric prompt\n");
    memset(&numeric_prompt, 0, sizeof(GX_NUMERIC_PROMPT));
    status = gx_numeric_prompt_create(&numeric_prompt, 0, 0, 0, 0, 0, 0);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_numeric_prompt_create(&numeric_prompt, 0, 0, 0, 0, 0, &size, 0);// should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    numeric_prompt.gx_widget_type = GX_TYPE_NUMERIC_PROMPT;
    status = gx_numeric_prompt_create(&numeric_prompt, 0, 0, 0, 0, 0, &size);// should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    memset(&numeric_prompt, 0, sizeof(GX_NUMERIC_PROMPT));
    status = gx_numeric_prompt_create(&numeric_prompt, 0, 0, 0, 0, 0, &size);// parent is NULL
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_numeric_prompt_format_function_set(&numeric_prompt, 0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_numeric_prompt_value_set(0, 0);// should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    /* numeric scroll wheel */
    printf("numeric scroll wheel\n");
    memset(&numeric_scroll_wheel, 0, sizeof(GX_NUMERIC_SCROLL_WHEEL));
    status = gx_numeric_scroll_wheel_create(&numeric_scroll_wheel, 0, 0, 0, 0, 0, 0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_numeric_scroll_wheel_create(&numeric_scroll_wheel, 0, 0, 0, 0, 0, 0, &size, 0);//should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    numeric_scroll_wheel.gx_widget_type = GX_TYPE_NUMERIC_SCROLL_WHEEL;
    status = gx_numeric_scroll_wheel_create(&numeric_scroll_wheel, 0, 0, 0, 0, 0, 0, &size);//should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    memset(&numeric_scroll_wheel, 0, sizeof(GX_NUMERIC_SCROLL_WHEEL));
    status = gx_numeric_scroll_wheel_create(&numeric_scroll_wheel, 0, 0, 0, 0, 0, 0, &size);// parent is NULL
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_numeric_scroll_wheel_range_set(0, 1, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    /* pixelmap button */
    printf("pixelmap button\n");
    status = gx_pixelmap_button_create(&pixelmap_button, 0, 0, 0, 0, 0, 0, 0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&pixelmap_button, 0, sizeof(GX_PIXELMAP_BUTTON));
    status = _gxe_pixelmap_button_create(&pixelmap_button, 0, 0, 0, 0, 0, 0, 0,&size, 0);//should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    pixelmap_button.gx_widget_type = GX_TYPE_PIXELMAP_BUTTON;
    status = gx_pixelmap_button_create(&pixelmap_button, 0, 0, 0, 0, 0, 0, 0, &size);//should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    memset(&pixelmap_button, 0, sizeof(GX_PIXELMAP_BUTTON));
    status = gx_pixelmap_button_create(&pixelmap_button, 0, 0, 0, 0, 0, 0, 0, &size);// parent is NULL
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_pixelmap_button_event_process(&pixelmap_button, 0); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    pixelmap_button.gx_widget_type = 0;
    status = gx_pixelmap_button_event_process(&pixelmap_button, &my_event); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_SUCCESS, status);

    pixelmap_button.gx_widget_type = GX_TYPE_PIXELMAP_BUTTON;
    my_event.gx_event_type = 0;
    status = gx_pixelmap_button_event_process(&pixelmap_button, &my_event); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_pixelmap_button_pixelmap_set(&pixelmap_button, 0, 0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_SUCCESS, status);
    
    /* pixelmap prompt */
    printf("pixelmap prompt\n");
    memset(&pixelmap_prompt, 0, sizeof(GX_PIXELMAP_PROMPT));
    status = gx_pixelmap_prompt_create(&pixelmap_prompt, 0, 0, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_pixelmap_prompt_create(&pixelmap_prompt, 0, 0, 0, 0, 0, 0, &size, 0); //should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    pixelmap_prompt.gx_widget_type = GX_TYPE_PIXELMAP_PROMPT;
    status = gx_pixelmap_prompt_create(&pixelmap_prompt, 0, 0, 0, 0, 0, 0, &size); //should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);
    
    status = gx_pixelmap_prompt_pixelmap_set(0, 1, 2, 3, 4, 5, 6); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    /* pixelmap slider */
    printf("pixelmap slider\n");
    memset(&pixelmap_slider, 0, sizeof(GX_PIXELMAP_SLIDER));
    status = gx_pixelmap_slider_create(&pixelmap_slider, 0, 0, &slider_info, 0, 0, 0, &size); // should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_pixelmap_slider_create(&pixelmap_slider, 0, 0, &slider_info, 0, 0, 0, 0); // size == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_pixelmap_slider_create(&pixelmap_slider, 0, 0, 0, 0, 0, 0, &size); // info == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_pixelmap_slider_create(&pixelmap_slider, 0, 0, &slider_info, &pixelmap_slider_info, 0, 0, &size, 0); // should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    pixelmap_slider.gx_widget_type = GX_TYPE_PIXELMAP_SLIDER;
    status = gx_pixelmap_slider_create(&pixelmap_slider, 0, 0, &slider_info, &pixelmap_slider_info, 0, 0, &size); // should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    memset(&pixelmap_slider, 0, sizeof(GX_PIXELMAP_SLIDER));
    status = gx_pixelmap_slider_create(&pixelmap_slider, 0, 0, &slider_info, &pixelmap_slider_info, 0, 0, &size); // parent is NULL
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_pixelmap_slider_event_process(&pixelmap_slider, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    pixelmap_slider.gx_widget_type = 0;
    status = gx_pixelmap_slider_event_process(&pixelmap_slider, &my_event); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    pixelmap_slider.gx_widget_type = GX_TYPE_PIXELMAP_SLIDER;
    my_event.gx_event_type = 0;
    status = gx_pixelmap_slider_event_process(&pixelmap_slider, &my_event); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_pixelmap_slider_pixelmap_set(&pixelmap_slider, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    /* progress bar */
    printf("progress bar\n");
    memset(&progress_bar, 0, sizeof(GX_PROGRESS_BAR));
    status = gx_progress_bar_create(&progress_bar, 0, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_progress_bar_create(&progress_bar, 0, 0, &progress_bar_info, 0, 0, &size, 0); //should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    status = gx_progress_bar_create(&progress_bar, 0, 0, &progress_bar_info, 0, 0, &size); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_SUCCESS, status);

    progress_bar.gx_widget_type = GX_TYPE_PROGRESS_BAR;
    status = gx_progress_bar_create(&progress_bar, 0, &parent, &progress_bar_info, 0, 0, &size); //should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    progress_bar.gx_widget_type = 0;
    parent.gx_widget_type = 0;
    status = gx_progress_bar_create(&progress_bar, 0, &parent, &progress_bar_info, 0, 0, &size); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_progress_bar_create(&progress_bar, 0, 0, &progress_bar_info, 0, 0, &size); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_widget_delete(&progress_bar);
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_progress_bar_create(&progress_bar, 0, 0, 0, 0, 0, &size); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_progress_bar_event_process(&progress_bar, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    progress_bar.gx_widget_type = GX_TYPE_PROGRESS_BAR;
    my_event.gx_event_type = 0;
    memset(&progress_bar, 0, sizeof(GX_PROGRESS_BAR));
    status = gx_progress_bar_event_process(&progress_bar, &my_event); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_progress_bar_font_set(0, 1);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_progress_bar_info_set(0, &progress_bar_info); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_progress_bar_info_set(&progress_bar, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    progress_bar.gx_widget_status = 0;
    status = gx_progress_bar_info_set(&progress_bar, &progress_bar_info); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_progress_bar_pixelmap_set(0, 1);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_progress_bar_range_set(0, 10, 1);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_progress_bar_text_color_set(0, 1, 2, 3);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    progress_bar.gx_widget_type = 0;
    status = gx_progress_bar_text_color_set(&progress_bar, 1, 2, 3);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_progress_bar_value_set(0, 1);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    /* prompt */
    printf("prompt\n");
    memset(&prompt, 0, sizeof(GX_PROMPT));
    status = gx_prompt_create(&prompt, 0, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_prompt_create(&prompt, 0, 0, 0, 0, 0, &size, 0); //should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    prompt.gx_widget_type = GX_TYPE_PROMPT;
    status = gx_prompt_create(&prompt, 0, 0, 0, 0, 0, &size); //should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);
    
    status = gx_prompt_font_set(0, 1);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    prompt.gx_widget_type = 0;
    status = gx_prompt_font_set(&prompt, 0);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    prompt.gx_widget_type = GX_TYPE_PROMPT;
    status = gx_prompt_font_set(&prompt, 0);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_prompt_text_color_set(0, 1, 2, 3); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    prompt.gx_widget_type = 0;
    status = gx_prompt_text_color_set(&prompt, 1, 2, 3); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    prompt.gx_widget_type = GX_TYPE_PROMPT;
    status = gx_prompt_text_color_set(&prompt, 1, 2, 3); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
   
#ifdef GX_ENABLE_DEPRECATED_STRING_API 
    status = gx_prompt_text_get(0, (GX_CONST GX_CHAR **)&byte_data);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    prompt.gx_widget_type = 0;
    status = gx_prompt_text_get(&prompt, (GX_CONST GX_CHAR **)&byte_data);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(&prompt, 0, sizeof(GX_PROMPT));
    prompt.gx_widget_type = GX_TYPE_PROMPT;
    status = gx_prompt_text_get(&prompt, (GX_CONST GX_CHAR **)&byte_data);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
#endif
    status = gx_prompt_text_get_ext(0, &string);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&prompt, 0, sizeof(GX_PROMPT));
    status = gx_prompt_text_get_ext(&prompt, &string);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    gx_utility_rectangle_define(&size, 0, 0, 100, 100);
    gx_prompt_create(&prompt, "test_prompt", GX_NULL, 0, 0, 1024, &size);
    string.gx_string_ptr = test_string;
    string.gx_string_length = sizeof(test_string) - 1;
    gx_prompt_text_set_ext(&prompt, &string);

    memset(&string, 0, sizeof(GX_STRING));
    status = gx_prompt_text_get_ext(&prompt, &string);//should return GX_SUCCESSI
    EXPECT_EQ(GX_SUCCESS, status);

    if(strcmp(string.gx_string_ptr, test_string) != 0)
    {
        printf("\nERROR! File: %s Line: %d\n", __FILE__, __LINE__); \
        printf("Retrieved prompt text is not correct");

        failed_tests++;
    }

    status = gx_prompt_text_id_set(0, 1); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    _gx_system_memory_free = 0;
    prompt.gx_widget_style = GX_STYLE_TEXT_COPY;
    prompt.gx_prompt_string.gx_string_ptr = (GX_CONST GX_CHAR *)&color;
    status = gx_prompt_text_id_set(&prompt, 1); //should return GX_SYSTEM_MEMORY_ERROR
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    _gx_system_memory_free = memory_free;
    status = gx_prompt_text_id_set(&prompt, 1);
    EXPECT_EQ(GX_SUCCESS, status);

    prompt.gx_widget_style = GX_STYLE_TEXT_COPY;
    prompt.gx_prompt_string.gx_string_ptr = 0;
    prompt.gx_prompt_string.gx_string_length = 0;
    status = gx_prompt_text_id_set(&prompt, 1);
    EXPECT_EQ(GX_SUCCESS, status);

    prompt.gx_widget_style = 0;
    prompt.gx_prompt_string.gx_string_ptr = buffer;
    prompt.gx_prompt_string.gx_string_length = 1;
    status = gx_prompt_text_id_set(&prompt, 1);
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_prompt_text_set(0,  (GX_CONST GX_CHAR *)byte_data);//should return GX_PTR_ERROR 
    EXPECT_EQ(GX_PTR_ERROR, status);

    prompt.gx_widget_style = GX_STYLE_TEXT_COPY;
    _gx_system_memory_allocator = 0;
    status = gx_prompt_text_set(&prompt,  (GX_CONST GX_CHAR *)byte_data);//should return GX_SYSTEM_MEMORY_ERROR
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);
    
    /* radial progress bar */
    printf("radial progress bar\n");
    status = gx_radial_progress_bar_anchor_set(0, 1);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&radial_progress_bar, 0, sizeof(GX_RADIAL_PROGRESS_BAR));
    status = gx_radial_progress_bar_anchor_set(&radial_progress_bar, 1);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_radial_progress_bar_create(&radial_progress_bar, 0, 0, 0, 0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_radial_progress_bar_create(&radial_progress_bar, 0, 0, &radial_progress_bar_info, 0, 0, 0);//should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    parent.gx_widget_type = 0;
    status = gx_radial_progress_bar_create(&radial_progress_bar, 0, &parent, &radial_progress_bar_info, 0, 0);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    radial_progress_bar.gx_widget_type = GX_TYPE_RADIAL_PROGRESS_BAR;
    status = gx_radial_progress_bar_create(&radial_progress_bar, 0, 0, &radial_progress_bar_info, 0, 0);//should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    memset(&radial_progress_bar, 0, sizeof(GX_RADIAL_PROGRESS_BAR));
    status = gx_radial_progress_bar_create(&radial_progress_bar, 0, 0, &radial_progress_bar_info, 0, 0);// parent is NULL
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_radial_progress_bar_event_process(&radial_progress_bar, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&my_event, 0, sizeof(GX_EVENT));
    radial_progress_bar.gx_widget_type = 0;
    status = gx_radial_progress_bar_event_process(&radial_progress_bar, &my_event); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(&radial_progress_bar, 0, sizeof(GX_RADIAL_PROGRESS_BAR));
    radial_progress_bar.gx_widget_type = GX_TYPE_RADIAL_PROGRESS_BAR;
    status = gx_radial_progress_bar_event_process(&radial_progress_bar, &my_event); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_radial_progress_bar_font_set(0, 1);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    radial_progress_bar.gx_widget_type = 0;
    status = gx_radial_progress_bar_font_set(&radial_progress_bar, 1);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_radial_progress_bar_info_set(&radial_progress_bar, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    radial_progress_bar.gx_widget_type = 0;
    status = gx_radial_progress_bar_info_set(&radial_progress_bar, &radial_progress_bar_info); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_radial_progress_bar_text_color_set(0, 1, 2, 3);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    radial_progress_bar.gx_widget_type = 0;
    status = gx_radial_progress_bar_text_color_set(&radial_progress_bar, 1, 2, 3);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_radial_progress_bar_value_set(0, 1);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    radial_progress_bar.gx_widget_type = 0;
    status = gx_radial_progress_bar_value_set(&radial_progress_bar, 1);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    /* radial slider */
    printf("radial slider\n");
    memset(&radial_slider, 0, sizeof(GX_RADIAL_SLIDER));
    status = gx_radial_slider_anchor_angles_set(0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_radial_slider_anchor_angles_set(&radial_slider, (GX_VALUE *)&color, 0); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    status = gx_radial_slider_anchor_angles_set(&radial_slider, GX_NULL, 0); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    radial_slider.gx_widget_type = GX_TYPE_RADIAL_SLIDER;
    status = gx_radial_slider_anchor_angles_set(&radial_slider, GX_NULL, 0); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
   
    memset(&radial_slider, 0, sizeof(GX_RADIAL_SLIDER));   
    status = gx_radial_slider_angle_set(0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_radial_slider_angle_set(&radial_slider, 0); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    radial_slider.gx_widget_type = GX_TYPE_RADIAL_SLIDER;
    status = gx_radial_slider_angle_set(&radial_slider, 0); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    memset(&radial_slider, 0, sizeof(GX_RADIAL_SLIDER));   
    status = gx_radial_slider_animation_set(0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_radial_slider_animation_set(&radial_slider, 0, 0, 0, 0); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    radial_slider.gx_widget_type = GX_TYPE_RADIAL_SLIDER;
    status = gx_radial_slider_animation_set(&radial_slider, 0, 0, 0, 0); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    memset(&radial_slider, 0, sizeof(GX_RADIAL_SLIDER));
    status = gx_radial_slider_animation_start(GX_NULL, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_radial_slider_animation_start(&radial_slider, 0); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    radial_slider.gx_widget_type = GX_TYPE_RADIAL_SLIDER;
    status = gx_radial_slider_animation_start(&radial_slider, 0); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    memset(&radial_slider, 0, sizeof(GX_RADIAL_SLIDER));
    status = gx_radial_slider_create(GX_NULL, GX_NULL, GX_NULL, GX_NULL, 0, 0, GX_NULL); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_radial_slider_create(&radial_slider, GX_NULL, GX_NULL, GX_NULL, 0, 0, GX_NULL); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_radial_slider_create(&radial_slider, GX_NULL, GX_NULL, GX_NULL, 0, 0, &size); // info is GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_radial_slider_create(&radial_slider, GX_NULL, GX_NULL, &radial_slider.gx_radial_slider_info, 0, 0, GX_NULL); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    radial_slider.gx_widget_type = GX_TYPE_RADIAL_SLIDER;
    status = gx_radial_slider_create(&radial_slider, GX_NULL, GX_NULL, &radial_slider.gx_radial_slider_info, 0, 0, &size); //should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    radial_slider.gx_widget_type = 0;
    status = _gxe_radial_slider_create(&radial_slider, GX_NULL, GX_NULL, &radial_slider.gx_radial_slider_info, 0, 0, &size, 0); //should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    parent.gx_widget_type = 0;
    status = gx_radial_slider_create(&radial_slider, GX_NULL, &parent, &radial_slider.gx_radial_slider_info, 0, 0, &size); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    parent.gx_widget_type = GX_TYPE_WIDGET;
    status = gx_radial_slider_create(&radial_slider, GX_NULL, &parent, &radial_slider.gx_radial_slider_info, 0, 0, &size); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    radial_slider.gx_widget_type = 0;
    status = gx_radial_slider_create(&radial_slider, GX_NULL, GX_NULL, &radial_slider.gx_radial_slider_info, 0, 0, &size); // parent is GX_NULL
    EXPECT_EQ(GX_SUCCESS, status);
    
    memset(&radial_slider, 0, sizeof(GX_RADIAL_SLIDER));
    status = gx_radial_slider_event_process(GX_NULL, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_radial_slider_event_process(&radial_slider, GX_NULL); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    radial_slider.gx_widget_type = 0;
    status = gx_radial_slider_event_process(&radial_slider, &my_event); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(&my_event, 0, sizeof(GX_EVENT));
    radial_slider.gx_widget_type = GX_TYPE_RADIAL_SLIDER;
    status = gx_radial_slider_event_process(&radial_slider, &my_event); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    memset(&radial_slider, 0, sizeof(GX_RADIAL_SLIDER));
    status = gx_radial_slider_info_get(GX_NULL, GX_NULL); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_radial_slider_info_get(&radial_slider, GX_NULL); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    radial_slider.gx_widget_type = 0;
    status = gx_radial_slider_info_get(&radial_slider, &radial_slider_info); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    radial_slider.gx_widget_type = GX_TYPE_RADIAL_SLIDER;
    status = gx_radial_slider_info_get(&radial_slider, &radial_slider_info); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    memset(&radial_slider, 0, sizeof(GX_RADIAL_SLIDER));
    status = gx_radial_slider_info_set(GX_NULL, GX_NULL);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_radial_slider_info_set(&radial_slider, GX_NULL);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    radial_slider.gx_widget_type = 0;
    status = gx_radial_slider_info_set(&radial_slider, &radial_slider.gx_radial_slider_info);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    radial_slider.gx_widget_type = GX_TYPE_RADIAL_SLIDER;
    status = gx_radial_slider_info_set(&radial_slider, &radial_slider.gx_radial_slider_info);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    memset(&radial_slider, 0, sizeof(GX_RADIAL_SLIDER));
    status = gx_radial_slider_pixelmap_set(GX_NULL, 0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    radial_slider.gx_widget_type = 0;
    status = gx_radial_slider_pixelmap_set(&radial_slider, 0, 0);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    radial_slider.gx_widget_type = GX_TYPE_RADIAL_SLIDER;
    status = gx_radial_slider_pixelmap_set(&radial_slider, 0, 0);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    /* radio button */
    printf("radio button\n");
    memset(&radio_button, 0, sizeof(GX_RADIO_BUTTON));
    status = gx_radio_button_create(&radio_button, 0, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    radio_button.gx_widget_type = GX_TYPE_RADIO_BUTTON;
    status = gx_radio_button_create(&radio_button, 0, 0, 0, 0, 0, &size); //should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    radio_button.gx_widget_type = 0;
    status = _gxe_radio_button_create(&radio_button, 0, 0, 0, 0, 0, &size, 0); //should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    parent.gx_widget_type = 0;
    status = gx_radio_button_create(&radio_button, 0, &parent, 0, 0, 0, &size); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_radio_button_create(&radio_button, 0, 0, 0, 0, 0, &size); // parent is NULL
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_radio_button_pixelmap_set(0, 1, 2, 3, 4); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    radio_button.gx_widget_type = 0;
    status = gx_radio_button_pixelmap_set(&radio_button, 1, 2, 3, 4); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    /* screen stack */
    printf("screen stack\n");
    memset(&screen_stack_control, 0, sizeof(GX_SCREEN_STACK_CONTROL));
    status = gx_screen_stack_create(&screen_stack_control, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_screen_stack_create(&screen_stack_control, (GX_WIDGET **)&byte_data, 0); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);
    
    status = gx_screen_stack_pop(0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_screen_stack_push(&screen_stack_control, &widget, 0);// control is NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_screen_stack_push(&screen_stack_control, 0, 0);// screen is NULL
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_screen_stack_reset(0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    /* scroll thumb */
    printf("scroll thumb\n");
    memset(&scroll_thumb, 0, sizeof(GX_SCROLL_THUMB));
    status = gx_scroll_thumb_create(&scroll_thumb, 0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_SUCCESS, status);

    scroll_thumb.gx_widget_type = GX_TYPE_BUTTON;
    status = gx_scroll_thumb_create(&scroll_thumb, 0, 0);//should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    scroll_thumb.gx_widget_type = 0;
    status = _gxe_scroll_thumb_create(&scroll_thumb, 0, 0, 0);//should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    scrollbar.gx_widget_type = 0;
    status = gx_scroll_thumb_create(&scroll_thumb, &scrollbar, 0);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    scrollbar.gx_widget_type = GX_TYPE_WIDGET;
    status = gx_scroll_thumb_create(&scroll_thumb, &scrollbar, 0);
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_scroll_thumb_event_process(&scroll_thumb, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    scroll_thumb.gx_widget_type = 0;
    status = gx_scroll_thumb_event_process(&scroll_thumb, &my_event);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(&scroll_thumb, 0, sizeof(GX_SCROLL_THUMB));
    scroll_thumb.gx_widget_type = GX_TYPE_BUTTON;
    status = gx_scroll_thumb_event_process(&scroll_thumb, &my_event);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    /* scrollbar */
    printf("scrollbar\n");
    memset(&scrollbar, 0, sizeof(GX_SCROLLBAR));
    status = gx_scrollbar_event_process(&scrollbar, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    scrollbar.gx_widget_type = 0;
    status = gx_scrollbar_event_process(&scrollbar, &my_event);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    scrollbar.gx_widget_type = GX_TYPE_VERTICAL_SCROLL;
    status = gx_scrollbar_event_process(&scrollbar, &my_event);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_scrollbar_limit_check(0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    scrollbar.gx_widget_type = 0;
    status = gx_scrollbar_limit_check(&scrollbar);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    scrollbar.gx_widget_type = GX_TYPE_VERTICAL_SCROLL;
    status = gx_scrollbar_limit_check(&scrollbar);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_scrollbar_reset(0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    scrollbar.gx_widget_type = 0;
    status = gx_scrollbar_reset(&scrollbar, 0);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    scrollbar.gx_widget_type = GX_TYPE_WIDGET;
    memset(&scroll_info, 0, sizeof(GX_SCROLL_INFO));
    scroll_info.gx_scroll_minimum = scroll_info.gx_scroll_value + 1;
    status = gx_scrollbar_reset(&scrollbar, &scroll_info);//should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    memset(&scrollbar, 0, sizeof(GX_SCROLLBAR));
    status = gx_scrollbar_reset(&scrollbar, 0);//should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    scrollbar.gx_widget_type = GX_TYPE_HORIZONTAL_SCROLL;
    status = gx_scrollbar_reset(&scrollbar, 0);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    scroll_info.gx_scroll_maximum = 1;
    scroll_info.gx_scroll_value = 2;
    status = gx_scrollbar_reset(&scrollbar, &scroll_info);// value > maximum
    EXPECT_EQ(GX_INVALID_VALUE, status);
    
    /* scroll wheel */
    printf("scroll wheel\n");
    memset(&scroll_wheel, 0, sizeof(GX_SCROLL_WHEEL));
    status = gx_scroll_wheel_create(&scroll_wheel, 0, 0, 0, 0, 0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_scroll_wheel_create(&scroll_wheel, 0, 0, 0, 0, 0, &size, 0);//should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    scroll_wheel.gx_widget_type = GX_TYPE_SCROLL_WHEEL;
    status = gx_scroll_wheel_create(&scroll_wheel, 0, 0, 0, 0, 0, &size);//should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    scroll_wheel.gx_widget_type = 0;
    parent.gx_widget_type = 0;
    status = gx_scroll_wheel_create(&scroll_wheel, 0, &parent, 0, 0, 0, &size);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_scroll_wheel_create(&scroll_wheel, 0, 0, 0, 0, 0, &size);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    memset(&scroll_wheel, 0, sizeof(GX_SCROLL_WHEEL));
    parent.gx_widget_type = GX_TYPE_WIDGET;
    status = gx_scroll_wheel_create(&scroll_wheel, 0, &parent, 0, 0, 0, &size);
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_scroll_wheel_event_process(&scroll_wheel, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    scroll_wheel.gx_widget_type = 0;
    status = gx_scroll_wheel_event_process(&scroll_wheel, &my_event);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(&scroll_wheel, 0, sizeof(GX_SCROLL_WHEEL));
    scroll_wheel.gx_widget_type = GX_TYPE_SCROLL_WHEEL;
    status = gx_scroll_wheel_event_process(&scroll_wheel, &my_event);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_scroll_wheel_gradient_alpha_set(0, 1, 2);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    scroll_wheel.gx_widget_type = 0;
    status = gx_scroll_wheel_gradient_alpha_set(&scroll_wheel, 1, 2);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_scroll_wheel_row_height_set(0, 1);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    scroll_wheel.gx_widget_type = 0;
    status = gx_scroll_wheel_row_height_set(&scroll_wheel, 2);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    scroll_wheel.gx_widget_type = GX_TYPE_SCROLL_WHEEL;
    status = gx_scroll_wheel_row_height_set(&scroll_wheel, 0);//should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);
    
    status = gx_scroll_wheel_selected_background_set(0, 1);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    scroll_wheel.gx_widget_type = 0;
    status = gx_scroll_wheel_selected_background_set(&scroll_wheel, 1);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_scroll_wheel_selected_get(&scroll_wheel, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    scroll_wheel.gx_widget_type = 0;
    status = gx_scroll_wheel_selected_get(&scroll_wheel, (INT *)&color); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    scroll_wheel.gx_widget_type = GX_TYPE_SCROLL_WHEEL;
    status = gx_scroll_wheel_selected_get(&scroll_wheel, (INT *)&color); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_scroll_wheel_selected_set(0, 1);
    EXPECT_EQ(GX_PTR_ERROR, status);

    scroll_wheel.gx_widget_type = 0;
    status = gx_scroll_wheel_selected_set(&scroll_wheel, 1); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_scroll_wheel_speed_set(0, 1, 2, 3, 4); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    scroll_wheel.gx_widget_type = 0;
    status = gx_scroll_wheel_speed_set(&scroll_wheel, 1, 2, 3, 4); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    scroll_wheel.gx_widget_type = GX_TYPE_SCROLL_WHEEL;
    status = gx_scroll_wheel_speed_set(&scroll_wheel, 1, 2, 3, 0); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);
    
    status = gx_scroll_wheel_total_rows_set(0, 1); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    scroll_wheel.gx_widget_type = 0;
    status = gx_scroll_wheel_total_rows_set(&scroll_wheel, 1); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    scroll_wheel.gx_widget_type = GX_TYPE_SCROLL_WHEEL;
    status = gx_scroll_wheel_total_rows_set(&scroll_wheel, -1); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);
    
    /* single line text input */
    printf("single line text input\n");
    status = gx_single_line_text_input_backspace(0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&single_line_text_input, 0, sizeof(GX_SINGLE_LINE_TEXT_INPUT));
    status = gx_single_line_text_input_backspace(&single_line_text_input);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(&single_line_text_input, 0, sizeof(GX_TYPE_SINGLE_LINE_TEXT_INPUT));
    single_line_text_input.gx_widget_type = GX_TYPE_SINGLE_LINE_TEXT_INPUT;
    status = gx_single_line_text_input_backspace(&single_line_text_input);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_single_line_text_input_buffer_clear(0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    single_line_text_input.gx_widget_type = 0;
    status = gx_single_line_text_input_buffer_clear(&single_line_text_input);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_single_line_text_input_buffer_get(0, 0, 0, 0);   //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    single_line_text_input.gx_widget_type = 0;
    status = gx_single_line_text_input_buffer_get(&single_line_text_input, 0, 0, 0);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_single_line_text_input_character_delete(0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    single_line_text_input.gx_widget_type = 0;
    status = gx_single_line_text_input_character_delete(&single_line_text_input);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(&single_line_text_input, 0, sizeof(GX_TYPE_SINGLE_LINE_TEXT_INPUT));
    single_line_text_input.gx_widget_type = GX_TYPE_SINGLE_LINE_TEXT_INPUT;
    status = gx_single_line_text_input_character_delete(&single_line_text_input);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_single_line_text_input_character_insert(0, 0, 0);   //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    single_line_text_input.gx_widget_type = 0;
    status = gx_single_line_text_input_character_insert(&single_line_text_input, 0, 0);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    color = 0;
    status = gx_single_line_text_input_create(&single_line_text_input, 0, 0, (GX_CHAR *)&color, 0, 0, 0, 0);    //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_single_line_text_input_create(&single_line_text_input, 0, 0, (GX_CHAR *)&color, 0, 0, 0, &size, 0); //should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    single_line_text_input.gx_widget_type = GX_TYPE_SINGLE_LINE_TEXT_INPUT;
    status = gx_single_line_text_input_create(&single_line_text_input, 0, 0, (GX_CHAR *)&color, 0, 0, 0, &size);    //should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    single_line_text_input.gx_widget_type = 0;
    parent.gx_widget_type = 0;
    status = gx_single_line_text_input_create(&single_line_text_input, 0, &parent, (GX_CHAR *)&color, 0, 0, 0, &size);    //should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    single_line_text_input.gx_widget_type = 0;
    status = gx_single_line_text_input_draw_position_get(&single_line_text_input, (GX_VALUE *)&color, (GX_VALUE *)&color);
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    single_line_text_input.gx_widget_type = GX_TYPE_SINGLE_LINE_TEXT_INPUT;
    _gx_system_current_draw_context = &default_context;
    default_context.gx_draw_context_brush.gx_brush_font = GX_NULL;
    status = gx_single_line_text_input_draw_position_get(&single_line_text_input, (GX_VALUE *)&color, (GX_VALUE *)&color);
    EXPECT_EQ(GX_FAILURE, status);

    status = gx_single_line_text_input_draw_position_get(&single_line_text_input, GX_NULL, (GX_VALUE *)&color); // xpos == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_single_line_text_input_draw_position_get(&single_line_text_input, (GX_VALUE *)&color, GX_NULL); // ypos == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_single_line_text_input_end(0);   //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    single_line_text_input.gx_widget_type = 0;
    status = gx_single_line_text_input_end(&single_line_text_input);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(&single_line_text_input, 0, sizeof(GX_TYPE_SINGLE_LINE_TEXT_INPUT));
    single_line_text_input.gx_widget_type = GX_TYPE_SINGLE_LINE_TEXT_INPUT;
    status = gx_single_line_text_input_end(&single_line_text_input);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_single_line_text_input_event_process(&single_line_text_input, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    single_line_text_input.gx_widget_type = 0;
    status = gx_single_line_text_input_event_process(&single_line_text_input, &my_event);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    single_line_text_input.gx_widget_type = GX_TYPE_SINGLE_LINE_TEXT_INPUT;
    my_event.gx_event_type = 0;
    single_line_text_input.gx_widget_parent = 0;
    status = gx_single_line_text_input_event_process(&single_line_text_input, &my_event);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_single_line_text_input_fill_color_set(0, 0, 0, 0, 0);   //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    single_line_text_input.gx_widget_type = 0;
    status = gx_single_line_text_input_fill_color_set(&single_line_text_input, 0, 0, 0, 0);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_single_line_text_input_home(0);   //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    single_line_text_input.gx_widget_type = 0;
    status = gx_single_line_text_input_home(&single_line_text_input);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(&single_line_text_input, 0, sizeof(GX_TYPE_SINGLE_LINE_TEXT_INPUT));
    single_line_text_input.gx_widget_type = GX_TYPE_SINGLE_LINE_TEXT_INPUT;
    status = gx_single_line_text_input_home(&single_line_text_input);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_single_line_text_input_left_arrow(0);   //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    single_line_text_input.gx_widget_type = 0;
    status = gx_single_line_text_input_left_arrow(&single_line_text_input);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(&single_line_text_input, 0, sizeof(GX_TYPE_SINGLE_LINE_TEXT_INPUT));
    single_line_text_input.gx_widget_type = GX_TYPE_SINGLE_LINE_TEXT_INPUT;
    status = gx_single_line_text_input_left_arrow(&single_line_text_input);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_single_line_text_input_position_get(0, 0);   //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    single_line_text_input.gx_widget_type = 0;
    status = gx_single_line_text_input_position_get(&single_line_text_input, 0);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(&single_line_text_input, 0, sizeof(GX_TYPE_SINGLE_LINE_TEXT_INPUT));
    single_line_text_input.gx_widget_type = GX_TYPE_SINGLE_LINE_TEXT_INPUT;
    status = gx_single_line_text_input_position_get(&single_line_text_input, 0);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_single_line_text_input_right_arrow(0);   //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    single_line_text_input.gx_widget_type = 0;
    status = gx_single_line_text_input_right_arrow(&single_line_text_input);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(&single_line_text_input, 0, sizeof(GX_TYPE_SINGLE_LINE_TEXT_INPUT));
    single_line_text_input.gx_widget_type = GX_TYPE_SINGLE_LINE_TEXT_INPUT;
    status = gx_single_line_text_input_right_arrow(&single_line_text_input);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_single_line_text_input_style_add(0, 0);   //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    single_line_text_input.gx_widget_type = 0;
    status = gx_single_line_text_input_style_add(&single_line_text_input, 0);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_single_line_text_input_style_remove(0, 0);   //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    single_line_text_input.gx_widget_type = 0;
    status = gx_single_line_text_input_style_remove(&single_line_text_input, 0);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_single_line_text_input_style_set(0, 0);   //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    single_line_text_input.gx_widget_type = 0;
    status = gx_single_line_text_input_style_set(&single_line_text_input, 0);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    single_line_text_input.gx_widget_status = 0;
    single_line_text_input.gx_widget_type = GX_TYPE_SINGLE_LINE_TEXT_INPUT;
    status = gx_single_line_text_input_style_set(&single_line_text_input, 0);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_single_line_text_input_text_color_set(0, 0, 0, 0, 0);   //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    single_line_text_input.gx_widget_type = 0;
    status = gx_single_line_text_input_text_color_set(&single_line_text_input, 0, 0, 0, 0);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_single_line_text_input_text_select(0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    single_line_text_input.gx_widget_type = 0;
    status = gx_single_line_text_input_text_select(&single_line_text_input, 0, 0); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    single_line_text_input.gx_widget_type = GX_TYPE_SINGLE_LINE_TEXT_INPUT;
    single_line_text_input.gx_single_line_text_input_string_size = 0;
    status = gx_single_line_text_input_text_select(&single_line_text_input, 0, 0); // start_index >= input_string_size
    EXPECT_EQ(GX_INVALID_VALUE, status);

    single_line_text_input.gx_single_line_text_input_string_size = 16;
    status = gx_single_line_text_input_text_select(&single_line_text_input, 0, 17); // end_index >= input_string_size
    EXPECT_EQ(GX_INVALID_VALUE, status);
    
    status = gx_single_line_text_input_text_set(0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    single_line_text_input.gx_widget_type = 0;
    status = gx_single_line_text_input_text_set(&single_line_text_input, 0); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    /* slider */
    printf("slider\n");
    memset(&slider, 0, sizeof(GX_SLIDER));
    status = gx_slider_create(&slider, 0, 0, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status =_gxe_slider_create(&slider, 0, 0, 0, &slider_info, 0, 0, &size, 0); //should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    slider.gx_widget_type = GX_TYPE_SLIDER;
    status = gx_slider_create(&slider, 0, 0, 0, &slider_info, 0, 0, &size); //should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    slider.gx_widget_type = 0;
    parent.gx_widget_type = 0;
    status = gx_slider_create(&slider, 0, &parent, 0, &slider_info, 0, 0, &size); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_slider_create(&slider, 0, &parent, 0, 0, 0, 0, &size); // slider_info == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_slider_event_process(&slider, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    slider.gx_widget_type = 0;
    status = gx_slider_event_process(&slider, &my_event); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    slider.gx_widget_type = GX_TYPE_SLIDER;
    my_event.gx_event_type = 0;
    slider.gx_widget_parent = 0;
    status = gx_slider_event_process(&slider, &my_event); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_slider_info_set(&slider, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    slider.gx_widget_type = 0;
    status = gx_slider_info_set(&slider, &slider_info);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_slider_needle_position_get(&slider, &slider_info, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_slider_needle_position_get(&slider, 0, 0); // slider_info == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    slider.gx_widget_type = 0;
    status = gx_slider_needle_position_get(&slider, &slider_info, &size); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    slider.gx_widget_type = GX_TYPE_SLIDER;
    slider_info.gx_slider_info_min_val = slider_info.gx_slider_info_current_val + 1;
    status = gx_slider_needle_position_get(&slider, &slider_info, &size); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    slider_info.gx_slider_info_current_val = slider_info.gx_slider_info_min_val + 1;
    slider_info.gx_slider_info_max_val = slider_info.gx_slider_info_current_val + 1;
    status = gx_slider_needle_position_get(&slider, &slider_info, &size); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    slider_info.gx_slider_info_min_val = 0;
    slider_info.gx_slider_info_current_val = 2;
    slider_info.gx_slider_info_max_val = 1;
    status = gx_slider_needle_position_get(&slider, &slider_info, &size); // cur_val > max_val
    EXPECT_EQ(GX_INVALID_VALUE, status);

    slider_info.gx_slider_info_min_val = 1;
    slider_info.gx_slider_info_current_val = 1;
    slider_info.gx_slider_info_max_val = 1;
    status = gx_slider_needle_position_get(&slider, &slider_info, &size); // min_val == max_val
    EXPECT_EQ(GX_INVALID_VALUE, status);
    
    status = gx_slider_travel_get(0, 0, 0, 0); // slider is NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_slider_travel_get(&slider, 0, 0, 0); // info is NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_slider_travel_get(&slider, &slider_info, 0, 0); // return_min_travel is NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_slider_travel_get(&slider, &slider_info, (INT *)&color, 0); // return_max_travel is NULL
    EXPECT_EQ(GX_PTR_ERROR, status);

    slider.gx_widget_type = 0;
    status = gx_slider_travel_get(&slider, &slider_info, (INT *)&color, (INT *)&color); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    slider.gx_widget_type = GX_TYPE_SLIDER;
    slider_info.gx_slider_info_min_val = 1;
    slider_info.gx_slider_info_current_val = 0;    
    status = gx_slider_travel_get(&slider, &slider_info, (INT *)&color, (INT *)&color); // gx_slider_info_current_val < min_val
    EXPECT_EQ(GX_INVALID_VALUE, status);

    slider_info.gx_slider_info_current_val = 3;    
    slider_info.gx_slider_info_max_val = 2;    
    status = gx_slider_travel_get(&slider, &slider_info, (INT *)&color, (INT *)&color); // gx_slider_info_current_val > max_val
    EXPECT_EQ(GX_INVALID_VALUE, status);

    slider_info.gx_slider_info_min_val = 2;
    slider_info.gx_slider_info_current_val = 2;    
    slider_info.gx_slider_info_max_val = 2;    
    status = gx_slider_travel_get(&slider, &slider_info, (INT *)&color, (INT *)&color); // gx_slider_info_min_val = max_val
    EXPECT_EQ(GX_INVALID_VALUE, status);

    status = gx_slider_value_calculate(&slider, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    slider.gx_widget_type = 0;
    status = gx_slider_value_calculate(&slider, &slider_info, 0); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    slider.gx_widget_type = GX_TYPE_SLIDER;
    slider_info.gx_slider_info_min_val = slider_info.gx_slider_info_current_val + 1;    
    status = gx_slider_value_calculate(&slider, &slider_info, 0); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    slider_info.gx_slider_info_min_val = 0;    
    slider_info.gx_slider_info_current_val = slider_info.gx_slider_info_max_val + 1;   
    status = gx_slider_value_calculate(&slider, &slider_info, 0); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    slider_info.gx_slider_info_min_val = 10;    
    slider_info.gx_slider_info_max_val = 0;
    slider_info.gx_slider_info_current_val = 5;
    status = gx_slider_value_calculate(&slider, &slider_info, 0); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    slider_info.gx_slider_info_max_val = 10;
    slider_info.gx_slider_info_min_val = 0;    
    slider_info.gx_slider_info_current_val = 5;
    status = gx_slider_value_calculate(&slider, &slider_info, 5); //should return GX_INVALID_VALUE    
    EXPECT_EQ(GX_SUCCESS, status);

    slider_info.gx_slider_info_max_val = 5;
    slider_info.gx_slider_info_min_val = 5;    
    slider_info.gx_slider_info_current_val = 5;
    status = gx_slider_value_calculate(&slider, &slider_info, 5); // min_val == max_val
    EXPECT_EQ(GX_INVALID_VALUE, status);

    status = gx_slider_value_set(&slider, 0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    slider.gx_widget_type = 0;
    status = gx_slider_value_set(&slider, &slider_info, 0); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    /* sprite */
    printf("sprite\n");
    memset(&sprite, 0, sizeof(GX_SPRITE));
    status = gx_sprite_create(&sprite, 0, 0, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_sprite_create(&sprite, 0, 0, 0, 0, 0, 0, &size, 0); //should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    sprite.gx_widget_type = GX_TYPE_SPRITE;
    status = gx_sprite_create(&sprite, 0, 0, 0, 0, 0, 0, &size); //should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    parent.gx_widget_type = 0;
    sprite.gx_widget_type = 0;
    status = gx_sprite_create(&sprite, 0, &parent, 0, 0, 0, 0, &size); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(&sprite, 0, sizeof(GX_SPRITE));
    status = gx_sprite_create(&sprite, 0, 0, 0, 0, 0, 0, &size); // parent is NULL
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_sprite_current_frame_set(0, 1);  //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    sprite.gx_widget_type = 0;
    status = gx_sprite_current_frame_set(&sprite, 1);  //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(&sprite, 0, sizeof(GX_SPRITE));
    sprite.gx_widget_type = GX_TYPE_SPRITE;
    status = gx_sprite_current_frame_set(&sprite, 1);  //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_sprite_frame_list_set(0, 0, 0);  //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    sprite.gx_widget_type = 0;
    status = gx_sprite_frame_list_set(&sprite, 0, 0);  //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(&sprite, 0, sizeof(GX_SPRITE));
    sprite.gx_widget_type = GX_TYPE_SPRITE;
    status = gx_sprite_frame_list_set(&sprite, 0, 0);  //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_sprite_start(0, 0);  //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    sprite.gx_widget_type = 0;
    status = gx_sprite_start(&sprite, 0);  //should return GX_INVALID_WIDGET    
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(&sprite, 0, sizeof(GX_SPRITE));
    sprite.gx_widget_type = GX_TYPE_SPRITE;
    status = gx_sprite_start(&sprite, 0);  //should return GX_FAILURE   
    EXPECT_EQ(GX_FAILURE, status);
  
    status = gx_sprite_stop(0);  //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    sprite.gx_widget_type = 0;
    status = gx_sprite_stop(&sprite);  //should return GX_INVALID_WIDGET    
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    sprite.gx_sprite_run_state = 1; //GX_SPRITE_IDLE
    sprite.gx_widget_type = GX_TYPE_SPRITE;
    status = gx_sprite_stop(&sprite);  //should return GX_SUCCESS    
    EXPECT_EQ(GX_SUCCESS, status);
    
    /* string scroll wheel */
    printf("string scroll wheel\n");
    memset(&string_scroll_wheel, 0, sizeof(GX_STRING_SCROLL_WHEEL));
    status = gx_string_scroll_wheel_create(&string_scroll_wheel, 0, 0, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_string_scroll_wheel_create(&string_scroll_wheel, 0, 0, 0, 0, 0, 0, &size, 0); //should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    string_scroll_wheel.gx_widget_type = GX_TYPE_STRING_SCROLL_WHEEL;
    status = gx_string_scroll_wheel_create(&string_scroll_wheel, 0, 0, 0, 0, 0, 0, &size); //should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    string_scroll_wheel.gx_widget_type = 0;
    parent.gx_widget_type = 0;
    status = gx_string_scroll_wheel_create(&string_scroll_wheel, 0, &parent, 0, 0, 0, 0, &size); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_string_scroll_wheel_create(&string_scroll_wheel, 0, 0, 0, 0, 0, 0, &size); // parent is NULL
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_string_scroll_wheel_string_list_set(&string_scroll_wheel, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_SUCCESS, status);

    string_scroll_wheel.gx_widget_type = 0;
    status = gx_string_scroll_wheel_string_list_set(&string_scroll_wheel, (GX_CONST GX_CHAR **)&byte_data, 0); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    string_scroll_wheel.gx_widget_type = GX_TYPE_STRING_SCROLL_WHEEL;
    status = gx_string_scroll_wheel_string_list_set(&string_scroll_wheel, (GX_CONST GX_CHAR **)&byte_data, -1); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    status = gx_string_scroll_wheel_string_id_list_set(&string_scroll_wheel, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    string_scroll_wheel.gx_widget_type = 0;
    status = gx_string_scroll_wheel_string_id_list_set(&string_scroll_wheel, (GX_CONST GX_RESOURCE_ID *)&color, 0); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    string_scroll_wheel.gx_widget_type = GX_TYPE_STRING_SCROLL_WHEEL;
    status = gx_string_scroll_wheel_string_id_list_set(&string_scroll_wheel, (GX_CONST GX_RESOURCE_ID *)&color, -1); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    /* system */
    printf("system\n");
    default_display.gx_display_language_table_size = 0;
    status = gx_system_active_language_set(0);//should return GX_INVALID_VALUE
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_system_animation_get(0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    _gx_system_animation_free_list = 0;
    status = gx_system_animation_get(&animation_ptr); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_OUT_OF_ANIMATIONS, status);
    
    status = gx_system_animation_free(0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    animation.gx_animation_system_allocated = 0;
    status = gx_system_animation_free(&animation); //should return GX_INVALID_ANIMATION
    EXPECT_EQ(GX_INVALID_ANIMATION, status);

    animation.gx_animation_system_allocated = 1;
    animation.gx_animation_status = GX_ANIMATION_IDLE + 1;
    status = gx_system_animation_free(&animation); //should return GX_INVALID_ANIMATION
    EXPECT_EQ(GX_INVALID_ANIMATION, status);

    animation.gx_animation_system_allocated = 1;
    animation.gx_animation_status = GX_ANIMATION_IDLE;
    status = gx_system_animation_free(&animation); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_system_canvas_refresh();//should return GX_INVALID_CANVAS
    EXPECT_EQ(GX_INVALID_CANVAS, status);
    
    status = gx_system_dirty_mark(0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    widget.gx_widget_type = 0;
    status = gx_system_dirty_mark(&widget);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_system_dirty_partial_add(&widget, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    widget.gx_widget_type = 0;
    status = gx_system_dirty_partial_add(&widget, &size);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    widget.gx_widget_type = GX_TYPE_WIDGET;
    size.gx_rectangle_left = size.gx_rectangle_right + 1;
    status = gx_system_dirty_partial_add(&widget, &size);//should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    memset(&widget, 0, sizeof(GX_WIDGET));
    memset(&size, 0, sizeof(GX_RECTANGLE));
    size.gx_rectangle_right = 1;
    size.gx_rectangle_bottom = 1;
    widget.gx_widget_size.gx_rectangle_right = 1;
    widget.gx_widget_size.gx_rectangle_bottom = 1;
    widget.gx_widget_type = GX_TYPE_WIDGET;
    widget.gx_widget_status |= GX_STATUS_VISIBLE;
    status = gx_system_dirty_partial_add(&widget, &size);//should return GX_INVALID_SIZE
    EXPECT_EQ(GX_PTR_ERROR, status);

    size.gx_rectangle_left = 0;
    size.gx_rectangle_right = 1;
    size.gx_rectangle_top = 1;
    size.gx_rectangle_bottom = 0;
    status = gx_system_dirty_partial_add(&widget, &size);// top > bottom
    EXPECT_EQ(GX_INVALID_SIZE, status);

    size.gx_rectangle_left = 2;
    size.gx_rectangle_right = 4;
    size.gx_rectangle_top = 2;
    size.gx_rectangle_bottom = 4;
    widget.gx_widget_size.gx_rectangle_left = 4;
    widget.gx_widget_size.gx_rectangle_right = 2;
    status = gx_system_dirty_partial_add(&widget, &size); // rectangle_overlap_detect failed
    EXPECT_EQ(GX_INVALID_SIZE, status);
    
    status = gx_system_draw_context_get(0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_system_draw_context_get(&_gx_system_current_draw_context); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_system_event_fold(0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_system_event_send(0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_system_focus_claim(0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&widget, 0, sizeof(GX_WIDGET));
    widget.gx_widget_type = 0;
    status = gx_system_focus_claim(&widget); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_system_language_table_get(0, 0, 0);
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_system_language_table_set(0, 0, 1);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    status = gx_system_memory_allocator_set(0, 0);
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_system_pen_configure(0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_system_pen_configure(&pen_config); //should return GX_SUCCESS    
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_system_scroll_appearance_get(0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_system_scroll_appearance_get(0, &scrollbar_appearance);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_system_scroll_appearance_set(0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    //gx_system_start(); //should return GX_PTR_ERROR
  
    status = gx_system_string_table_get(1, 0, 0);
    EXPECT_EQ(GX_NOT_FOUND, status);
    
    memset(&font, 0, sizeof(GX_FONT));
    status = gx_system_string_width_get(&font, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_system_string_width_get(&font, "string", 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_system_string_width_get(&font, "string", 0,(GX_VALUE *)&color); //should return GX_INVALID_FONT
    EXPECT_EQ(GX_INVALID_FONT, status);

    status = gx_system_string_width_get(&font, "string", GX_STRLEN("string") + 10, (GX_VALUE *)&color); //should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_FONT, status);

    memset(&timer, 0, sizeof(GX_TIMER));
    _gx_system_free_timer_list = 0;
    status = gx_system_timer_start(0, 0, 0, 0); //should return GX_OUT_OF_TIMERS;
    EXPECT_EQ(GX_OUT_OF_TIMERS, status);

    _gx_system_free_timer_list = (GX_TIMER *)&timer;
    status = gx_system_timer_start(0, 0, 0, 0); //should return GX_PTR_ERROR;
    EXPECT_EQ(GX_PTR_ERROR, status);

    widget.gx_widget_type = 0;
    status = gx_system_timer_start(&widget, 0, 0, 0); //should return GX_INVALID_WIDGET;
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    widget.gx_widget_type = GX_TYPE_WIDGET;
    status = gx_system_timer_start(&widget, 0, 0, 0); //should return GX_INVALID_VALUE;
    EXPECT_EQ(GX_INVALID_VALUE, status);
    
    status = gx_system_timer_stop(0, 0); //should return GX_PTR_ERROR;
    EXPECT_EQ(GX_PTR_ERROR, status);

    widget.gx_widget_type = 0;
    status = gx_system_timer_stop(&widget, 0); //should return GX_INVALID_WIDGET;
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    _gx_system_free_timer_list = 0;
    status = gx_system_timer_stop(0, 1); //should return GX_NOT_FOUND;
    EXPECT_EQ(GX_NOT_FOUND, status);

    memset(&timer, 0, sizeof(GX_TIMER));
    _gx_system_free_timer_list = (GX_TIMER *)&timer;
    status = gx_system_timer_stop(0, 0); //should return GX_PTR_ERROR;
    EXPECT_EQ(GX_PTR_ERROR, status);

    _gx_system_active_timer_list = &gtimer;
    gtimer.gx_timer_next = 0;
    gtimer.gx_timer_id = 0xff;
    status = gx_system_timer_stop(0, 1);
    EXPECT_EQ(GX_NOT_FOUND, status);

    _gx_system_active_timer_list = 0;
    EXPECT_EQ(GX_NOT_FOUND, status);

    status = gx_system_version_string_get(0);//should return GX_PTR_ERROR;
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_system_version_string_get((GX_CHAR **)&byte_data);//should return GX_SUCCESS;
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_system_widget_find(0, 0, 0); //should return GX_PTR_ERROR;
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_system_widget_find(0, 0, &widget.gx_widget_parent); //should return GX_NOT_FOUND;
    EXPECT_EQ(GX_NOT_FOUND, status);
    
    /* text button */
    printf("text button\n");
    memset(&text_button, 0, sizeof(GX_TEXT_BUTTON));
    status = gx_text_button_create(&text_button, 0, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_text_button_create(&text_button, 0, 0, 0, 0, 0, &size, 0); //should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    text_button.gx_widget_type = GX_TYPE_TEXT_BUTTON;
    status = gx_text_button_create(&text_button, 0, 0, 0, 0, 0, &size); //should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    text_button.gx_widget_type = 0;
    parent.gx_widget_type = 0;
    status = gx_text_button_create(&text_button, 0, &parent, 0, 0, 0, &size); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_text_button_create(&text_button, 0, 0, 0, 0, 0, &size); // parent is NULL
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_text_button_font_set(0, 1);  //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    text_button.gx_widget_type = 0;
    status = gx_text_button_font_set(&text_button, 1);  //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_text_button_text_color_set(0, 1, 0, 0);  //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    text_button.gx_widget_type = 0;
    status = gx_text_button_text_color_set(&text_button, 1, 0, 0);  //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_text_button_text_get(&text_button, 0);  //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    text_button.gx_widget_type = 0;
    status = gx_text_button_text_get(&text_button, ( GX_CONST GX_CHAR **)&byte_data);  //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    text_button.gx_widget_type = GX_TYPE_TEXT_BUTTON;
    status = gx_text_button_text_get(&text_button, ( GX_CONST GX_CHAR **)&byte_data);  //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_text_button_text_id_set(0, 1);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    text_button.gx_widget_type = 0;
    status = gx_text_button_text_id_set(&text_button, 1);  //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    text_button.gx_widget_type = GX_TYPE_TEXT_BUTTON;
    text_button.gx_widget_style = 0;
    text_button.gx_widget_status = 0;
    status = gx_text_button_text_id_set(&text_button, 0);  //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_text_button_text_set(0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    text_button.gx_widget_type = 0;
    status = gx_text_button_text_set(&text_button, ( GX_CONST GX_CHAR *)byte_data);  //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    text_button.gx_widget_type = GX_TYPE_TEXT_BUTTON;
    text_button.gx_widget_style = GX_STYLE_TEXT_COPY;
    _gx_system_memory_allocator = 0;
    status = gx_text_button_text_set(&text_button, ( GX_CONST GX_CHAR *)byte_data);  //should return GX_SYSTEM_MEMORY_ERROR
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    text_button.gx_widget_style = 0;
    text_button.gx_widget_status = 0;
    status = gx_text_button_text_set(&text_button, ( GX_CONST GX_CHAR *)byte_data);  //should return GX_SYSTEM_MEMORY_ERROR
    EXPECT_EQ(GX_SUCCESS, status);
    
    /* text input cursor */
    printf("text input cursor\n");
    status = gx_text_input_cursor_blink_interval_set(0, 1); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&text_input_cursor, 0, sizeof(GX_TEXT_INPUT_CURSOR));
    status = gx_text_input_cursor_blink_interval_set(&text_input_cursor, 0);//should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    status = gx_text_input_cursor_blink_interval_set(&text_input_cursor, 1);//should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_text_input_cursor_height_set(0, 1); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_text_input_cursor_height_set(&text_input_cursor, 0);//should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    status = gx_text_input_cursor_width_set(0, 1); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_text_input_cursor_width_set(&text_input_cursor, 0);//should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    /* text scroll wheel */
    printf("text scroll wheel\n");
    memset(&text_scroll_wheel, 0 ,sizeof(GX_TEXT_SCROLL_WHEEL));
    status = gx_text_scroll_wheel_callback_set(&text_scroll_wheel, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    text_scroll_wheel.gx_widget_type = 0;
    status = gx_text_scroll_wheel_callback_set(&text_scroll_wheel, callback_fun);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_text_scroll_wheel_font_set(0, 1, 2); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    text_scroll_wheel.gx_widget_type = 0;
    status = gx_text_scroll_wheel_font_set(&text_scroll_wheel, 1, 2); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_text_scroll_wheel_text_color_set(0, 1, 2, 3); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    text_scroll_wheel.gx_widget_type = 0;
    status = gx_text_scroll_wheel_text_color_set(&text_scroll_wheel, 1, 2, 3); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_text_scroll_wheel_create(&text_scroll_wheel, 0, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = _gxe_text_scroll_wheel_create(&text_scroll_wheel, 0, 0, 0, 0, 0, &size, 0); //should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    text_scroll_wheel.gx_widget_type = GX_TYPE_TEXT_SCROLL_WHEEL;
    status = gx_text_scroll_wheel_create(&text_scroll_wheel, 0, 0, 0, 0, 0, &size); //should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    text_scroll_wheel.gx_widget_type = 0;
    parent.gx_widget_type = 0;
    status = gx_text_scroll_wheel_create(&text_scroll_wheel, 0, &parent, 0, 0, 0, &size); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    status = gx_text_scroll_wheel_create(&text_scroll_wheel, 0, 0, 0, 0, 0, &size); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);

    memset(&text_scroll_wheel, 0, sizeof(text_scroll_wheel));
    parent.gx_widget_type = GX_TYPE_TEXT_SCROLL_WHEEL;
    status = gx_text_scroll_wheel_create(&text_scroll_wheel, 0, &parent, 0, 0, 0, &size);
    EXPECT_EQ(GX_SUCCESS, status);

    /* utility gradient */
    printf("utility gradient\n");
    status = gx_utility_gradient_create(0, 1, 2, 3, 4, 5); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_utility_gradient_create(&gradient, 0, 2, 3, 4, 5); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    status = gx_utility_gradient_create(&gradient, 1, 0, 3, 4, 5); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    status = gx_utility_gradient_create(&gradient, 1, 2, 0, 4, 5); //should return GX_INVALID_TYPE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    status = gx_utility_gradient_create(&gradient, 1, 1, 1, 4, 5); //should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    status = gx_utility_gradient_create(&gradient, GX_MAX_PIXELMAP_RESOLUTION + 1, 3, GX_GRADIENT_TYPE_ALPHA, 0, 255);
    EXPECT_EQ(GX_INVALID_WIDTH, status);

    status = gx_utility_gradient_create(&gradient, 3, GX_MAX_PIXELMAP_RESOLUTION + 1, GX_GRADIENT_TYPE_ALPHA, 0, 255);
    EXPECT_EQ(GX_INVALID_HEIGHT, status);
 
    _gx_system_memory_allocator = GX_NULL;
    status = gx_utility_gradient_create(&gradient, 2, 2, GX_GRADIENT_TYPE_ALPHA, 255, 255);//should return GX_FAILURE;
    EXPECT_EQ(GX_FAILURE, status);

    _gx_system_memory_allocator = memory_allocate;
    status = gx_utility_gradient_create(&gradient, 2, 2, GX_GRADIENT_TYPE_ALPHA, 255, 255);//should return GX_FAILURE;
    EXPECT_EQ(GX_FAILURE, status);
    
    status = gx_utility_gradient_delete(0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    memset(&gradient.gx_gradient_pixelmap, 0, sizeof(GX_PIXELMAP));
    status = gx_utility_gradient_delete(&gradient);
    EXPECT_EQ(GX_SUCCESS, status);
    
    /* tree view */
    memset(&tree_view, 0, sizeof(GX_TREE_VIEW));
    status = gx_tree_view_create(&tree_view, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    tree_view.gx_widget_type = GX_TYPE_TREE_VIEW;
    status = gx_tree_view_create(&tree_view, 0, 0, 0, 0, &size); //should return GX_ALREADY_CREATED
    EXPECT_EQ(GX_ALREADY_CREATED, status);

    tree_view.gx_widget_type = 0;
    status = _gxe_tree_view_create(&tree_view, 0, 0, 0, 0, &size, 0); //should return GX_INVALID_SIZE
    EXPECT_EQ(GX_INVALID_SIZE, status);

    parent.gx_widget_type = 0;
    gx_tree_view_create(&tree_view, 0, &parent, 0, 0, &size); //should return GX_INVALID_WIDGET
    status = gx_tree_view_create(&tree_view, 0, 0, 0, 0, &size); // parent is NULL
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_tree_view_event_process(&tree_view, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    tree_view.gx_widget_type = 0;
    status = gx_tree_view_event_process(&tree_view, &my_event);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    tree_view.gx_widget_type = GX_TYPE_TREE_VIEW;
    my_event.gx_event_type = 0;
    tree_view.gx_widget_parent = 0;
    status = gx_tree_view_event_process(&tree_view, &my_event);//should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_SUCCESS, status);
    
    status = gx_tree_view_indentation_set(0, 1); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    tree_view.gx_widget_type = 0;
    status = gx_tree_view_indentation_set(&tree_view, 1); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_tree_view_position(0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    tree_view.gx_widget_type = 0;
    status = gx_tree_view_position(&tree_view); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    memset(&tree_view, 0, sizeof(GX_TREE_VIEW));
    tree_view.gx_widget_type = GX_TYPE_TREE_VIEW;
    status = gx_tree_view_position(&tree_view); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_SUCCESS, status);

    
    status = gx_tree_view_root_line_color_set(0, 1); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    tree_view.gx_widget_type = 0;
    status = gx_tree_view_root_line_color_set(&tree_view, 1); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_tree_view_root_pixelmap_set(0, 1, 2); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    tree_view.gx_widget_type = 0;
    status = gx_tree_view_root_pixelmap_set(&tree_view, 2, 1); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    status = gx_tree_view_selected_get(&tree_view, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    tree_view.gx_widget_type = 0;
    status = gx_tree_view_selected_get(&tree_view, &widget.gx_widget_parent); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    tree_view.gx_widget_type = GX_TYPE_TREE_VIEW;
    status = gx_tree_view_selected_get(&tree_view, &widget.gx_widget_parent); //should return GX_SUCCESS
    EXPECT_EQ(GX_SUCCESS, status);
        
    status = gx_tree_view_selected_set(&tree_view, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    tree_view.gx_widget_type = 0;
    status = gx_tree_view_selected_set(&tree_view, &widget); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    /* utility */
    printf("utility\n");
    gx_utility_canvas_to_bmp(0, 0, 0);
    memset(&canvas, 0, sizeof(GX_CANVAS));
    canvas.gx_canvas_display = &default_display;
    gx_utility_canvas_to_bmp(&canvas, 0, 0);
    canvas.gx_canvas_memory = &color;
    gx_utility_canvas_to_bmp(&canvas, 0, 0);
    gx_utility_canvas_to_bmp(&canvas, &size, 0);
    memset(&size, 0, sizeof(GX_RECTANGLE));
    gx_utility_canvas_to_bmp(&canvas, &size, write_data_callback);
    canvas.gx_canvas_display = 0;
    status = gx_utility_canvas_to_bmp(&canvas, &size, write_data_callback);
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    gx_utility_ltoa(0, 0, 0); //should return GX_PTR_ERROR
    gx_utility_ltoa(0, (GX_CHAR *)&color, 0); //should return GX_INVALID_SIZE
    
    gx_utility_math_acos(0);
    gx_utility_math_asin(0);
    gx_utility_math_cos(0);
    gx_utility_math_sin(0);   
    gx_utility_math_sqrt(0);
    
    if (gx_utility_math_asin(GX_FIXED_VAL_MAKE(1)) != 90)
    {
        printf("Guix Test:   guix_api_coverage_test...........................................................Failed!\n");
    }
    if(gx_utility_math_asin(-1 * GX_FIXED_VAL_MAKE(1)) != -90)
    {
        printf("Guix Test:   guix_api_coverage_test...........................................................Failed!\n");
    }
    
    gx_system_memory_allocator_set(memory_allocate, memory_free);
    pixelmap.gx_pixelmap_flags = 0;
    pixelmap.gx_pixelmap_width = GX_MAX_PIXELMAP_RESOLUTION + 1;
    pixelmap.gx_pixelmap_height = 10;
    status = gx_utility_pixelmap_resize(&pixelmap, &destination_pixelmap, 10, 10);
    EXPECT_EQ(GX_INVALID_WIDTH, status);

    pixelmap.gx_pixelmap_width = 10;
    pixelmap.gx_pixelmap_height = GX_MAX_PIXELMAP_RESOLUTION + 1;
    status = gx_utility_pixelmap_resize(&pixelmap, &destination_pixelmap, 10, 10);
    EXPECT_EQ(GX_INVALID_HEIGHT, status);

    status = gx_utility_pixelmap_resize(&pixelmap, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_utility_pixelmap_resize(&pixelmap, &pixelmap, 0, 0); // width <= 0
    EXPECT_EQ(GX_INVALID_VALUE, status);

    pixelmap.gx_pixelmap_flags = GX_PIXELMAP_COMPRESSED;
    status = gx_utility_pixelmap_resize(&pixelmap, &pixelmap, 0, 1); // height <= 0
    EXPECT_EQ(GX_NOT_SUPPORTED, status);

    status = gx_utility_pixelmap_resize(&pixelmap, &pixelmap, 1, 1); //should return GX_NOT_SUPPORTED
    EXPECT_EQ(GX_NOT_SUPPORTED, status);

    pixelmap.gx_pixelmap_flags = 0;
    gx_system_memory_allocator_set(GX_NULL, memory_free);
    status = gx_utility_pixelmap_resize(&pixelmap, &pixelmap, 1, 1); //should return GX_SYSTEM_MEMORY_ERROR
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    gx_system_memory_allocator_set(memory_allocate, GX_NULL);
    status = gx_utility_pixelmap_resize(&pixelmap, &pixelmap, 1, 1); // memory_free is NULL
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);
   
    gx_system_memory_allocator_set(memory_allocate, memory_free);
    pixelmap.gx_pixelmap_flags = 0;
    pixelmap.gx_pixelmap_width = GX_MAX_PIXELMAP_RESOLUTION + 1;
    pixelmap.gx_pixelmap_height = 10;
    status = gx_utility_pixelmap_rotate(&pixelmap, 30, &destination_pixelmap, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_INVALID_WIDTH, status);

    pixelmap.gx_pixelmap_width = 10;
    pixelmap.gx_pixelmap_height = GX_MAX_PIXELMAP_RESOLUTION + 1;
    status = gx_utility_pixelmap_rotate(&pixelmap, 30, &destination_pixelmap, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_INVALID_HEIGHT, status);

    status = gx_utility_pixelmap_rotate(&pixelmap, 0, 0, 0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    pixelmap.gx_pixelmap_width = 10;
    pixelmap.gx_pixelmap_height = 10;
    status = gx_utility_pixelmap_rotate(&pixelmap, 0, &pixelmap, 0, 0);//should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    gx_system_memory_allocator_set(GX_NULL, memory_free);
    status = gx_utility_pixelmap_rotate(&pixelmap, 90, &pixelmap, 0, 0);//should return GX_SYSTEM_MEMORY_ERROR
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    gx_system_memory_allocator_set(memory_allocate, GX_NULL);
    status = gx_utility_pixelmap_rotate(&pixelmap, 90, &pixelmap, 0, 0);//should return GX_SYSTEM_MEMORY_ERROR
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    gx_system_memory_allocator_set(memory_allocate, memory_free);
    status = gx_utility_pixelmap_rotate(&pixelmap, 90, 0, 0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    pixelmap.gx_pixelmap_flags = GX_PIXELMAP_COMPRESSED;
    status = gx_utility_pixelmap_rotate(&pixelmap, 91, &pixelmap, 0, 0);//should return GX_INVALID_FORMAT
    EXPECT_EQ(GX_INVALID_FORMAT, status);

    pixelmap.gx_pixelmap_flags = 0;
    pixelmap.gx_pixelmap_format = GX_COLOR_FORMAT_8BIT_ALPHAMAP + 1;
    status = gx_utility_pixelmap_rotate(&pixelmap, 91, &pixelmap, 0, 0);//should return GX_INVALID_FORMAT
    EXPECT_EQ(GX_INVALID_FORMAT, status);

    /* gx_utility_pixelmap_simple_rotate. */
    status = gx_utility_pixelmap_simple_rotate(0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_utility_pixelmap_simple_rotate(&pixelmap, 0, 0, 0, 0); //should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_utility_pixelmap_simple_rotate(&pixelmap, 0, &pixelmap, 0, 0); //should return GX_INVALID_VALUE
    EXPECT_EQ(GX_INVALID_VALUE, status);

    gx_system_memory_allocator_set(GX_NULL, memory_free);
    status = gx_utility_pixelmap_simple_rotate(&pixelmap, 90, &pixelmap, 0, 0); //should return GX_SYSTEM_MEMORY_ERROR
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    gx_system_memory_allocator_set(memory_allocate, GX_NULL);
    status = gx_utility_pixelmap_simple_rotate(&pixelmap, 90, &pixelmap, 0, 0); //should return GX_SYSTEM_MEMORY_ERROR
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);

    gx_system_memory_allocator_set(memory_allocate, memory_free);
    pixelmap.gx_pixelmap_flags = GX_PIXELMAP_COMPRESSED;
    status = gx_utility_pixelmap_simple_rotate(&pixelmap, 90, &pixelmap, 0, 0); //should return GX_INVALID_FORMAT
    EXPECT_EQ(GX_INVALID_FORMAT, status);

    pixelmap.gx_pixelmap_flags = 0;
    pixelmap.gx_pixelmap_format = GX_COLOR_FORMAT_8BIT_ALPHAMAP + 1;
    status = gx_utility_pixelmap_simple_rotate(&pixelmap, 90, &pixelmap, 0, 0); //should return GX_INVALID_FORMAT
    EXPECT_EQ(GX_INVALID_FORMAT, status);

    /* gx_utility_rectangle_center. */
    printf("utility rectangle center\n");
    gx_utility_rectangle_center(&size, 0); //should return GX_PTR_ERROR
    size.gx_rectangle_left = size.gx_rectangle_right + 1;
    gx_utility_rectangle_center(&size, &size); //should return GX_INVALID_SIZE
    size.gx_rectangle_left = size.gx_rectangle_right;
    size.gx_rectangle_top = size.gx_rectangle_bottom + 1;
    gx_utility_rectangle_center(&size, &size); //should return GX_INVALID_SIZE
    size.gx_rectangle_left = size.gx_rectangle_right;
    size.gx_rectangle_top = size.gx_rectangle_bottom;
    widget.gx_widget_size.gx_rectangle_left = widget.gx_widget_size.gx_rectangle_right + 1;
    gx_utility_rectangle_center(&size, &widget.gx_widget_size); //should return GX_INVALID_SIZE
    widget.gx_widget_size.gx_rectangle_left = widget.gx_widget_size.gx_rectangle_right;
    widget.gx_widget_size.gx_rectangle_top = widget.gx_widget_size.gx_rectangle_bottom + 1;
    gx_utility_rectangle_center(&size, &widget.gx_widget_size); //should return GX_INVALID_SIZE
    widget.gx_widget_size.gx_rectangle_top = widget.gx_widget_size.gx_rectangle_bottom;
    memset(&size, 0, sizeof(GX_RECTANGLE));
    gx_utility_rectangle_center(&size, &size); //should return GX_SUCCESS
    
    gx_utility_rectangle_center_find(&size, 0);//should return GX_PTR_ERROR
    size.gx_rectangle_left = size.gx_rectangle_right + 1;
    gx_utility_rectangle_center_find(&size, &point);//should return GX_INVALID_SIZE
    size.gx_rectangle_left = size.gx_rectangle_right;
    size.gx_rectangle_top = size.gx_rectangle_bottom + 1;
    gx_utility_rectangle_center_find(&size, &point);//should return GX_INVALID_SIZE
    memset(&size, 0, sizeof(GX_RECTANGLE));
    gx_utility_rectangle_center_find(&size, &point);//should return GX_SUCCESS
    
    gx_utility_rectangle_combine(&size, 0);//should return GX_PTR_ERROR
    gx_utility_rectangle_combine(&size, &size);//should return GX_SUCCESS
    
    gx_utility_rectangle_compare(&size, 0);//should return GX_PTR_ERROR
    size.gx_rectangle_left = size.gx_rectangle_right + 1;
    gx_utility_rectangle_compare(&size, &size);//should return GX_INVALID_SIZE
    memset(&size, 0, sizeof(GX_RECTANGLE));
    gx_utility_rectangle_compare(&size, &size);//should return GX_TRUE
    size.gx_rectangle_left = 1;
    size.gx_rectangle_right = 0;
    rec1.gx_rectangle_left = 0;
    rec1.gx_rectangle_right = 1;
    rec1.gx_rectangle_top = 1;
    rec1.gx_rectangle_bottom = 0;
    rec2.gx_rectangle_left = 0;
    rec2.gx_rectangle_right = 1;
    rec2.gx_rectangle_top = 0;
    rec2.gx_rectangle_bottom = 1;
    status = gx_utility_rectangle_compare(&rec1, &rec2);// first rectangle top > bottom
    EXPECT_EQ(GX_FALSE, status);
    status = gx_utility_rectangle_compare(&rec2, &size);// second rectangle left > right
    EXPECT_EQ(GX_FALSE, status);
    status = gx_utility_rectangle_compare(&rec2, &rec1);// second rectangle top > bottom
    EXPECT_EQ(GX_FALSE, status);
    
    gx_utility_rectangle_define(0, 1, 2, 3, 4);//should return GX_PTR_ERROR
    
    gx_utility_rectangle_overlap_detect(&size, 0, 0);//should return GX_PTR_ERROR
    size.gx_rectangle_left = size.gx_rectangle_right + 1;
    gx_utility_rectangle_overlap_detect(&size, &size, 0);//should return GX_INVALID_SIZE
    size.gx_rectangle_left = 1;
    size.gx_rectangle_right = 0;
    rec1.gx_rectangle_left = 0;
    rec1.gx_rectangle_right = 1;
    rec1.gx_rectangle_top = 1;
    rec1.gx_rectangle_bottom = 0;
    rec2.gx_rectangle_left = 0;
    rec2.gx_rectangle_right = 1;
    rec2.gx_rectangle_top = 0;
    rec2.gx_rectangle_bottom = 1;
    status = gx_utility_rectangle_overlap_detect(&rec1, &rec2, 0);// first rectangle top > bottom
    EXPECT_EQ(GX_FALSE, status);
    status = gx_utility_rectangle_overlap_detect(&rec2, &size, 0);// second rectangle left > right
    EXPECT_EQ(GX_FALSE, status);
    status = gx_utility_rectangle_overlap_detect(&rec2, &rec1, 0);// second rectangle top > bottom
    EXPECT_EQ(GX_FALSE, status);
    
    gx_utility_rectangle_point_detect(0, point);//should return GX_FALSE
    size.gx_rectangle_left = size.gx_rectangle_right + 1;
    gx_utility_rectangle_point_detect(&size, point);//should return GX_FALSE
    size.gx_rectangle_left = size.gx_rectangle_right;
    size.gx_rectangle_top = size.gx_rectangle_bottom + 1;
    gx_utility_rectangle_point_detect(&size, point);//should return GX_FALSE
    size.gx_rectangle_left = size.gx_rectangle_right = 0;
    size.gx_rectangle_bottom = size.gx_rectangle_top  = 0;
    gx_utility_rectangle_point_detect(&size, point);//should return GX_TRUE
    
    gx_utility_rectangle_resize(0, 1);//should return GX_PTR_ERROR
    
    gx_utility_rectangle_shift(0, 1, 2);//should return GX_PTR_ERROR
    
    gx_utility_string_to_alphamap((GX_CONST GX_CHAR *)byte_data, &font, 0); //should return GX_PTR_ERROR
    _gx_system_memory_allocator = 0;
    _gx_system_memory_free = 0;
    gx_utility_string_to_alphamap("text", &font, &pixelmap); //should return GX_SYSTEM_MEMORY_ERROR
    status = gx_utility_string_to_alphamap("text", GX_NULL, &pixelmap); // font == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);
    status = gx_utility_string_to_alphamap("text", &font, GX_NULL); // textmap == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);
    _gx_system_memory_allocator = (VOID *(*)(ULONG))0x04;
    _gx_system_memory_free = 0;
    status = gx_utility_string_to_alphamap("text", &font, &pixelmap); // _gx_system_memory_free == 0
    EXPECT_EQ(GX_SYSTEM_MEMORY_ERROR, status);
    
    /* vertical list */
    printf("vertical list\n");
    memset(&vertical_list, 0, sizeof(GX_VERTICAL_LIST));
    gx_vertical_list_children_position(0); // should return GX_PTR_ERROR
    vertical_list.gx_widget_type = 0;
    gx_vertical_list_children_position(&vertical_list);//should return GX_INVALID_WIDGET
    vertical_list.gx_widget_type = GX_TYPE_VERTICAL_LIST;
    gx_vertical_list_children_position(&vertical_list);//should return GX_SUCCESS
   
    gx_vertical_list_create(&vertical_list, 0, 0, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    _gxe_vertical_list_create(&vertical_list, 0, 0, 0, 0, 0, 0, &size, 0); //should return GX_INVALID_SIZE
    vertical_list.gx_widget_type = GX_TYPE_VERTICAL_LIST;
    gx_vertical_list_create(&vertical_list, 0, 0, 0, 0, 0, 0, &size); //should return GX_ALREADY_CREATED
    vertical_list.gx_widget_type = 0;
    gx_vertical_list_create(&vertical_list, 0, 0, 0, 0, 0, 0, &size); //should return GX_INVALID_VALUE
    parent.gx_widget_type = 0;
    gx_vertical_list_create(&vertical_list, 0, &parent, 1, 0, 0, 0, &size); //should return GX_INVALID_WIDGET
    status = gx_vertical_list_create(&vertical_list, 0, 0, 1, 0, 0, 0, &size); // parent == GX_NULL
    EXPECT_EQ(GX_SUCCESS, status);
    
    gx_vertical_list_event_process(&vertical_list, 0); //should return GX_PTR_ERROR
    vertical_list.gx_widget_type = 0;
    gx_vertical_list_event_process(&vertical_list, &my_event); //should return GX_INVALID_WIDGET
    memset(&vertical_list, 0, sizeof(GX_VERTICAL_LIST));
    memset(&my_event, 0, sizeof(GX_EVENT));
    vertical_list.gx_widget_type = GX_TYPE_VERTICAL_LIST;
    gx_vertical_list_event_process(&vertical_list, &my_event); //should return GX_SUCCESS
    
    gx_vertical_list_page_index_set(0, 1); //should return GX_PTR_ERROR
    vertical_list.gx_widget_type = 0;
    gx_vertical_list_page_index_set(&vertical_list, 1); //should return GX_INVALID_WIDGET
    vertical_list.gx_widget_type = GX_TYPE_VERTICAL_LIST;
    vertical_list.gx_vertical_list_total_rows = 0;
    gx_vertical_list_page_index_set(&vertical_list, 1); //should return GX_INVALID_VALUE
    status = gx_vertical_list_page_index_set(&vertical_list, -1); // index < 0
    EXPECT_EQ(GX_INVALID_VALUE, status);
    
    gx_vertical_list_selected_index_get(&vertical_list, 0); //should return GX_PTR_ERROR
    vertical_list.gx_widget_type = 0;
    gx_vertical_list_selected_index_get(&vertical_list, (INT *)&color); //should return GX_INVALID_WIDGET
    vertical_list.gx_widget_type = GX_TYPE_VERTICAL_LIST;
    gx_vertical_list_selected_index_get(&vertical_list, (INT *)&color); //should return GX_SUCCESS
    
    gx_vertical_list_selected_set(0, 1); //should return GX_PTR_ERROR
    vertical_list.gx_widget_type = 0;
    gx_vertical_list_selected_set(&vertical_list, 1); //should return GX_INVALID_WIDGET
    
    gx_vertical_list_selected_widget_get(&vertical_list, 0); //should return GX_PTR_ERROR
    vertical_list.gx_widget_type = 0;
    gx_vertical_list_selected_widget_get(&vertical_list, &vertical_list.gx_widget_parent); //should return GX_INVALID_WIDGET
    
    gx_vertical_list_total_rows_set(0, 1); //should return GX_PTR_ERROR
    vertical_list.gx_widget_type = 0;
    gx_vertical_list_total_rows_set(&vertical_list, 1); //should return GX_INVALID_WIDGET
    vertical_list.gx_widget_type = GX_TYPE_VERTICAL_LIST;
    gx_vertical_list_total_rows_set(&vertical_list, -1); //should return GX_INVALID_VALUE
    
    gx_vertical_scrollbar_create(&scrollbar, 0, 0, 0, 0);//should return GX_PTR_ERROR
    _gxe_vertical_scrollbar_create(&scrollbar, 0, (GX_WINDOW *)&parent, 0, 0, 0);//should return GX_INVALID_SIZE
    scrollbar.gx_widget_type = GX_TYPE_VERTICAL_SCROLL;
    gx_vertical_scrollbar_create(&scrollbar, 0, &parent, 0, 0); //should return GX_ALREADY_CREATED
    scrollbar.gx_widget_type = 0;
    parent.gx_widget_type = 0;
    gx_vertical_scrollbar_create(&scrollbar, 0, &parent, 0, 0); //should return GX_INVALID_WIDGET
    
    /* widget */
    printf("widget\n");
    _gx_system_memory_allocator = 0;
    gx_widget_allocate(&widget.gx_widget_parent, 0); //should return GX_SYSTEM_MEMORY_ERROR
    _gx_system_memory_allocator = memory_allocate;
    gx_widget_allocate(0, 0); //should return GX_PTR_ERROR
    gx_widget_allocate(&widget.gx_widget_parent, 0); //should return GX_INVALID_MEMORY_SIZE
    gx_widget_allocate(&widget.gx_widget_parent, 1); //should return GX_INVALID_MEMORY_SIZE
    
    gx_widget_attach(&parent, 0); //should return GX_PTR_ERROR
    parent.gx_widget_type = GX_TYPE_WIDGET;
    widget.gx_widget_type = 0;
    gx_widget_attach(&parent, &widget); //should return GX_INVALID_WIDGET
    parent.gx_widget_type = 0;
    status = gx_widget_attach(&parent, &widget); // parent.gx_widget_type == 0
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    gx_widget_back_attach(&parent, 0); //should return GX_PTR_ERROR
    parent.gx_widget_type = GX_TYPE_WIDGET;
    widget.gx_widget_type = 0;
    gx_widget_back_attach(&parent, &widget); //should return GX_INVALID_WIDGET
    parent.gx_widget_type = GX_TYPE_WIDGET;
    widget.gx_widget_type = GX_TYPE_WIDGET;
    gx_widget_back_attach(&parent, &widget); //should return GX_SUCCESS
    parent.gx_widget_type = 0;
    status = gx_widget_back_attach(&parent, &widget); // parent -> gx_widget_type == 0
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    gx_widget_back_move(0, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_back_move(&widget, 0); //should return GX_INVALID_WIDGET
    widget.gx_widget_type = GX_TYPE_WIDGET;
    widget.gx_widget_parent = 0;
    gx_widget_back_move(&widget, 0); //should return GX_PTR_ERROR
    widget.gx_widget_parent = &parent;
    parent.gx_widget_first_child = &widget;
    gx_widget_back_move(&widget, 0); //should return GX_NO_CHANGE
    
    gx_widget_block_move(0, 0, 1, 2); //should return GX_PTR_ERROR
    status = gx_widget_block_move(&widget, 0, 1, 2); // block == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);
    widget.gx_widget_type = 0;
    gx_widget_block_move(&widget, &size, 1, 2); //should return GX_INVALID_WIDGET
    memset(&size, 0, sizeof(GX_RECTANGLE));
    size.gx_rectangle_left = size.gx_rectangle_right + 1;
    size.gx_rectangle_top = size.gx_rectangle_bottom + 1;
    widget.gx_widget_type = GX_TYPE_WIDGET;
    gx_widget_block_move(&widget, &size, 1, 2); //should return GX_INVALID_VALUE
    size.gx_rectangle_left = 0;
    size.gx_rectangle_right = 1;
    size.gx_rectangle_top = size.gx_rectangle_bottom + 1;
    status = gx_widget_block_move(&widget, &size, 1, 2); // top > bottom
    EXPECT_EQ(GX_INVALID_VALUE, status);
    widget.gx_widget_type = GX_TYPE_WIDGET;
    size.gx_rectangle_right = size.gx_rectangle_left + 1;
    size.gx_rectangle_bottom = size.gx_rectangle_top + 1;
    _gx_system_current_draw_context = 0;
    gx_widget_block_move(&widget, &size, 1, 2); //should return GX_INVALID_CANVAS
    widget.gx_widget_status |= GX_STATUS_TRANSPARENT;
    gx_widget_block_move(&widget, &size, 1, 2); //should return GX_PTR_ERROR

    
    gx_widget_border_style_set(0, 1); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_border_style_set(&widget, 1);//should return GX_INVALID_WIDGET
    
    gx_widget_border_width_get(&widget, 0); //should return GX_INVALID_WIDGET
    widget.gx_widget_type = 0;
    gx_widget_border_width_get(&widget, (GX_VALUE *) &color); //should return GX_INVALID_WIDGET
    widget.gx_widget_type = GX_TYPE_WIDGET;
    widget.gx_widget_style = 0;
    gx_widget_border_width_get(&widget, (GX_VALUE *) &color); //should return GX_SUCCESS
    
    gx_widget_canvas_get(&widget, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_canvas_get(&widget, &default_context.gx_draw_context_canvas); //should return GX_INVALID_WIDGET
    _gx_system_current_draw_context = 0;
    memset(&widget, 0, sizeof(GX_WIDGET));
    widget.gx_widget_type = GX_TYPE_WIDGET;
    gx_widget_canvas_get(&widget, &default_context.gx_draw_context_canvas); //should return GX_SUCCESS
    
    gx_widget_child_detect(&parent, &widget, 0); //should return GX_PTR_ERROR
    status = gx_widget_child_detect(&parent, 0, 0); //child == GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);
    parent.gx_widget_type = 0;
    gx_widget_child_detect(&parent, &widget, (GX_BOOL *)&color); //should return GX_INVALID_WIDGET
    parent.gx_widget_type = GX_TYPE_WIDGET;
    widget.gx_widget_type = GX_TYPE_WIDGET;
    widget.gx_widget_parent = 0;
    gx_widget_child_detect(&parent, &widget, (GX_BOOL *)&color); //should return GX_SUCCESS
    parent.gx_widget_type = GX_TYPE_WIDGET;
    widget.gx_widget_type = 0;
    status = gx_widget_child_detect(&parent, &widget, (GX_BOOL *)&color); //should return GX_SUCCESS
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    
    gx_widget_color_get(&widget, 0, 0); //should return GX_PTR_ERROR
    status = gx_widget_color_get(0, 0, &color); // widget == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);
    widget.gx_widget_type = 0;
    gx_widget_color_get(&widget, 0, &color); //should return GX_INVALID_WIDGET
    widget.gx_widget_status = 0;
    widget.gx_widget_type = GX_TYPE_WIDGET;
    gx_widget_color_get(&widget, 0, &color); //should return GX_SUCCESS
    
    gx_widget_client_get(&widget, 0, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_client_get(&widget, 0, &size); //should return GX_INVALID_WIDGET
    memset(&widget, 0, sizeof(GX_WIDGET));
    widget.gx_widget_type = GX_TYPE_WIDGET;
    status = gx_widget_client_get(&widget, 10, &size); // border_width > widget_width / 2
    EXPECT_EQ(GX_INVALID_VALUE, status);
    widget.gx_widget_size.gx_rectangle_right = 32;
    widget.gx_widget_size.gx_rectangle_left = 0;
    status = gx_widget_client_get(&widget, 10, &size); // border_width > widget_height / 2
    EXPECT_EQ(GX_INVALID_VALUE, status);
    widget.gx_widget_type = GX_TYPE_WIDGET;
    gx_widget_client_get(&widget, 0, &size); //should return GX_SUCCESS
    
    gx_widget_create(&widget, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = GX_TYPE_WIDGET;
    gx_widget_create(&widget, 0, 0, 0, 0, &size); //should return GX_ALREADY_CREATED
    widget.gx_widget_type = 0;
    parent.gx_widget_type = 0;
    gx_widget_create(&widget, 0, &parent, 0, 0, &size); //should return GX_INVALID_WIDGET
    _gxe_widget_create(&widget, 0, 0, 0, 0, &size, 0); //should return GX_INVALID_SIZE
    memset(&size, 0, sizeof(GX_RECTANGLE));
    widget.gx_widget_type = 0;
    gx_widget_create(&widget, 0, 0, 0, 0, &size); //should return GX_PTR_ERROR
    
    gx_widget_created_test(&widget, 0); //should return GX_PTR_ERROR
    status = gx_widget_created_test(0, 0); // widget == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);
    
    gx_widget_delete(0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_delete(&widget); //should return GX_INVALID_WIDGET
    widget.gx_widget_type = GX_TYPE_WIDGET;
    _gx_system_memory_free = 0;
    widget.gx_widget_style = GX_STYLE_TEXT_COPY | GX_STATUS_DYNAMICALLY_ALLOCATED;
    gx_widget_delete(&widget); //should return GX_SYSTEM_MEMORY_ERROR
    
    gx_widget_detach(&widget); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_detach(&widget); //should return GX_INVALID_WIDGET
    
    gx_widget_draw_set(&widget, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_draw_set(&widget, draw_func); //should return GX_INVALID_WIDGET
    widget.gx_widget_type = GX_TYPE_WIDGET;
    gx_widget_draw_set(&widget, draw_func); //should return GX_SUCCESS
    
    gx_widget_event_generate(0, 0, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_event_generate(&widget, 0, 0); //should return GX_INVALID_WIDGET
    
    gx_widget_event_process(&widget, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    widget.gx_widget_parent = 0;
    gx_widget_event_process(&widget, &my_event); //should return GX_INVALID_WIDGET
    
    gx_widget_event_process_set(&widget, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_event_process_set(&widget, event_process_func); //should return GX_INVALID_WIDGET
    widget.gx_widget_type = GX_TYPE_WIDGET;
    gx_widget_event_process_set(&widget, event_process_func); //should return GX_SUCCESS
    
    gx_widget_event_to_parent(&widget, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_event_to_parent(&widget, &my_event); //should return GX_INVALID_WIDGET
    widget.gx_widget_parent = 0;
    widget.gx_widget_type = GX_TYPE_WIDGET;
    gx_widget_event_to_parent(&widget, &my_event); //should return GX_SUCCESS
    
    gx_widget_fill_color_set(0, 0, 0, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_fill_color_set(&widget, 0, 0, 0); //should return GX_INVALID_WIDGET
    
    gx_widget_find(&parent, 0, 0, 0 );//should return GX_PTR_ERROR
    parent.gx_widget_type = 0;
    gx_widget_find(&parent, 0, 0, &parent.gx_widget_first_child);//should return GX_INVALID_WIDGET
    
    gx_widget_focus_next(0); //shoud return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_focus_next(&widget);//should return GX_INVALID_WIDGET
    widget.gx_widget_type = GX_TYPE_WIDGET;
    widget.gx_widget_nav_next = 0;
    gx_widget_focus_next(&widget);//should return GX_INVALID_WIDGET

    gx_widget_focus_previous(0); //shoud return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_focus_previous(&widget);//should return GX_INVALID_WIDGET
    widget.gx_widget_type = GX_TYPE_WIDGET;
    gx_widget_focus_previous(&widget);//should return GX_FAILURE
    
    gx_widget_font_get(&widget, 0,0); //should return GX_PTR_ERROR
    status = gx_widget_font_get(0, 0, &default_context.gx_draw_context_brush.gx_brush_font); // widget == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);
    widget.gx_widget_type = 0;
    gx_widget_font_get(&widget, 0, &default_context.gx_draw_context_brush.gx_brush_font); //should return GX_INVALID_WIDGET
    
    gx_widget_free(0); //should return GX_PTR_ERROR
    _gx_system_memory_free = 0;
    gx_widget_free(&widget);//should return GX_SYSTEM_MEMORY_ERROR
    _gx_system_memory_free = memory_free;
    gx_widget_free(&widget);
    
    gx_widget_front_move(0, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_front_move(&widget, 0); //should return GX_INVALID_WIDGET
    widget.gx_widget_type = GX_TYPE_WIDGET;
    widget.gx_widget_parent = 0;
    gx_widget_front_move(&widget, 0); //should return GX_PTR_ERROR
    
    gx_widget_height_get(&widget, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_height_get(&widget, (GX_VALUE *)&color); //should return GX_INVALID_WIDGET
    widget.gx_widget_type = GX_TYPE_WIDGET;
    gx_widget_height_get(&widget, (GX_VALUE *)&color); //should return GX_SUCCESS
    
    gx_widget_hide(0); //shoud return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_hide(&widget);//should return GX_INVALID_WIDGET
    
    gx_widget_pixelmap_get(&widget, 0, 0); //should return GX_PTR_ERROR
    status = gx_widget_pixelmap_get(0, 0, &default_context.gx_draw_context_brush.gx_brush_pixelmap); // widget == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);
    widget.gx_widget_type = 0;
    gx_widget_pixelmap_get(&widget, 0, &default_context.gx_draw_context_brush.gx_brush_pixelmap); //should return GX_INVALID_WIDGET
    
    gx_widget_resize(&widget, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_resize(&widget, &size);//should return GX_INVALID_WIDGET

    gx_widget_shift(0, 0, 0, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_shift(&widget, 0, 0, 0);//should return GX_INVALID_WIDGET

    gx_widget_show(0); //shoud return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_show(&widget);//should return GX_INVALID_WIDGET
    widget.gx_widget_type = GX_TYPE_WIDGET;
    widget.gx_widget_parent = 0;
    gx_widget_show(&widget);//should return GX_INVALID_WIDGET
    widget.gx_widget_parent = &parent;
    parent.gx_widget_status = 0;
    gx_widget_show(&widget);//should return GX_INVALID_WIDGET

    gx_widget_status_add(0, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_status_add(&widget, 0);//should return GX_INVALID_WIDGET
    
    gx_widget_status_get(&widget, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_status_get(&widget, &ulong_data);//should return GX_INVALID_WIDGET
    widget.gx_widget_type = GX_TYPE_WIDGET;
    gx_widget_status_get(&widget, &ulong_data);//should return GX_SUCCESS
    
    gx_widget_status_remove(0, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_status_remove(&widget, 0);//should return GX_INVALID_WIDGET
    
    gx_widget_status_test(&widget, 0, 0);//should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_status_test(&widget, 0, (GX_BOOL *)&ulong_data);//should return GX_INVALID_WIDGET
    widget.gx_widget_type = GX_TYPE_WIDGET;
    gx_widget_status_test(&widget, 0, (GX_BOOL *)&ulong_data);//should return GX_SUCCESS
    widget.gx_widget_status = 1;
    gx_widget_status_test(&widget, widget.gx_widget_status, (GX_BOOL *)&ulong_data);//should return GX_SUCCESS
    
    gx_widget_style_add(0, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_style_add(&widget, 0); //should return GX_INVALID_WIDGET    
    
    gx_widget_style_get(&widget, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_style_get(&widget, &ulong_data); //should return GX_INVALID_WIDGET
    widget.gx_widget_type = GX_TYPE_WIDGET;
    gx_widget_style_get(&widget, &ulong_data); //should return GX_SUCCESS
    
    gx_widget_style_remove(0, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_style_remove(&widget, 0); //should return GX_INVALID_WIDGET
    
    gx_widget_style_set(0, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_style_set(&widget, 0); //should return GX_INVALID_WIDGET
   
#ifdef GX_ENABLED_DEPRECATED_STRING_API 
    gx_widget_text_blend(0, 0, 0, 0, 0, 0, 0);//should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_text_blend(&widget, 0, 0, 0, 0, 0, 0); //should return GX_INVALID_WIDGET
    widget.gx_widget_type = GX_TYPE_WIDGET;
    _gx_system_current_draw_context = &default_context;
    memset(&default_context, 0, sizeof(GX_DRAW_CONTEXT));
    default_context.gx_draw_context_display = &display;
    memset(&display, 0, sizeof(GX_DISPLAY));
    gx_widget_text_blend(&widget, 0, 0, 0, 0, 0, 0); //should return GX_SUCCESS
    gx_widget_text_blend(&widget, 0, 0, "widget_text_blend", 0, 0, 0); //should return GX_SUCCESS
#endif
    status = gx_widget_text_blend_ext(0, 0, 0, 0, 0, 0, 0);//should return GX_PTR_ERROR
    EXPECT_EQ(GX_PTR_ERROR, status);

    widget.gx_widget_type = 0;
    status = gx_widget_text_blend_ext(&widget, 0, 0, 0, 0, 0, 0); //should return GX_INVALID_WIDGET
    EXPECT_EQ(GX_INVALID_WIDGET, status);

    widget.gx_widget_type = GX_TYPE_WIDGET;
    string.gx_string_ptr = "test";
    string.gx_string_length = strlen(string.gx_string_ptr) + 1;
    status = gx_widget_text_blend_ext(&widget, 0, 0, &string, 0, 0, 0);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);

    string.gx_string_length = strlen(string.gx_string_ptr) - 1;
    status = gx_widget_text_blend_ext(&widget, 0, 0, &string, 0, 0, 0);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);

    string.gx_string_ptr = GX_NULL;
    status = gx_widget_text_blend_ext(&widget, 0, 0, &string, 0, 0, 0);
    EXPECT_EQ(GX_INVALID_STRING_LENGTH, status);

    gx_widget_type_find(&parent, 0, 0); //should return GX_PTR_ERROR
    parent.gx_widget_type = 0;
    gx_widget_type_find(&parent, 0, &parent.gx_widget_first_child); //should return GX_INVALID_WIDGET
    parent.gx_widget_type = GX_TYPE_WINDOW;
    gx_widget_type_find(&parent, 0, &widget_ptr); //should return GX_INVALID_WIDGET
    
    gx_widget_width_get(&widget, 0); //should return GX_PTR_ERROR
    widget.gx_widget_type = 0;
    gx_widget_width_get(&widget, (GX_VALUE *)&color);//should return GX_INVALID_WIDGET
    
    /* window */
    printf("window\n");
    memset(&window, 0, sizeof(GX_WINDOW));
    gx_window_client_height_get(&window, 0); //should return GX_PTR_ERROR
    window.gx_widget_type = 0;
    gx_window_client_height_get(&window, (GX_VALUE *)&color);//should return GX_INVALID_WIDGET
    window.gx_widget_type = GX_TYPE_WINDOW;
    gx_window_client_height_get(&window, (GX_VALUE *)&color);//should return GX_INVALID_WIDGET
    
    gx_window_client_scroll(0, 0, 0); //should return GX_PTR_ERROR
    window.gx_widget_type = 0;
    gx_window_client_scroll(&window, 0, 0); //should return GX_INVALID_WIDGET
    window.gx_widget_type = GX_TYPE_WINDOW;
    memset(&scrollbar, 0, sizeof(GX_SCROLLBAR));
    scrollbar.gx_widget_type = GX_TYPE_VERTICAL_SCROLL;
    scrollbar.gx_widget_status = GX_STATUS_NONCLIENT;
    window.gx_widget_first_child = (GX_WIDGET *)&scrollbar;
    gx_window_client_scroll(&window, 1, 1); //should return GX_INVALID_VALUE
    scrollbar.gx_scrollbar_info.gx_scroll_maximum = 100;
    scrollbar.gx_scrollbar_info.gx_scroll_minimum  = 50;
    scrollbar.gx_scrollbar_info.gx_scroll_value  = 0;
    gx_window_client_scroll(&window, 1, 1); //should return GX_INVALID_VALUE

    memset(&scrollbar, 0, sizeof(GX_SCROLLBAR));
    scrollbar.gx_widget_type = GX_TYPE_HORIZONTAL_SCROLL;
    scrollbar.gx_widget_status = GX_STATUS_NONCLIENT;
    window.gx_widget_first_child = (GX_WIDGET *)&scrollbar;
    gx_window_client_scroll(&window, 1, 1); //should return GX_INVALID_VALUE
    scrollbar.gx_scrollbar_info.gx_scroll_maximum = 100;
    scrollbar.gx_scrollbar_info.gx_scroll_minimum  = 50;
    scrollbar.gx_scrollbar_info.gx_scroll_value  = 0;
    gx_window_client_scroll(&window, 1, 1); //should return GX_INVALID_VALUE
    memset(&scrollbar, 0, sizeof(GX_SCROLLBAR));
    memset(&window, 0, sizeof(GX_WINDOW));
    window.gx_widget_type = GX_TYPE_WINDOW;
    gx_window_client_scroll(&window, -1, -1); //should return GX_SUCCESS
    
    gx_window_client_width_get(&window, 0); //should return GX_PTR_ERROR
    window.gx_widget_type = 0;
    gx_window_client_width_get(&window, (GX_VALUE *)&color);//should return GX_INVALID_WIDGET
    window.gx_widget_type = GX_TYPE_WINDOW;
    gx_window_client_width_get(&window, (GX_VALUE *)&color);//should return GX_INVALID_WIDGET

    gx_window_close(0); //should return GX_PTR_ERROR
    window.gx_widget_type = 0;
    gx_window_close(&window);//should return GX_INVALID_WIDGET

    gx_window_create(&window, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    window.gx_widget_type = GX_TYPE_WINDOW;
    gx_window_create(&window, 0, 0, 0, 0, &size); //should return GX_ALREADY_CREATED
    window.gx_widget_type = 0;
    _gxe_window_create(&window, 0, 0, 0, 0, &size, 0); //should return GX_INVALID_SIZE
    parent.gx_widget_type = 0;
    gx_window_create(&window, 0, &parent, 0, 0, &size); //should return GX_INVALID_WIDGET
    
    gx_window_event_process(&window, 0); //should return GX_PTR_ERROR
    window.gx_widget_type = 0;
    window.gx_widget_parent = 0;
    gx_window_event_process(&window, &my_event); //should return GX_INVALID_WIDGET
    
    gx_window_execute(0, 0); //should return GX_PTR_ERROR
    window.gx_widget_type = 0;
    gx_window_execute(&window, 0); //should return GX_INVALID_WIDGET
    
    /* window root */
    printf("window root\n");
    memset(&window_root, 0, sizeof(GX_WINDOW_ROOT));
    gx_window_root_create(&window_root, 0, 0, 0, 0, 0); //should return GX_PTR_ERROR
    gx_window_root_create(&window_root, 0, 0, 0, 0, &size); //should return GX_PTR_ERROR
    _gxe_window_root_create(&window_root, 0, &canvas, 0, 0, &size, 0); //should return GX_INVALID_SIZE
    window_root.gx_widget_type = GX_TYPE_ROOT_WINDOW;
    gx_window_root_create(&window_root, 0, &canvas, 0, 0, &size); //should return GX_ALREADY_CREATED
    
    gx_window_root_delete(0); //should return GX_PTR_ERROR
    window_root.gx_widget_type = 0;
    gx_window_root_delete(&window_root); //should return GX_INVALID_WIDGET
    window_root.gx_widget_type = GX_TYPE_ROOT_WINDOW;
    window_root.gx_widget_style = GX_STYLE_TEXT_COPY | GX_STATUS_DYNAMICALLY_ALLOCATED;
    _gx_system_memory_free = 0;
    gx_window_root_delete(&window_root); //should return GX_SYSTEM_MEMORY_ERROR
    _gx_system_memory_free = memory_free;
    window_root.gx_widget_event_process_function = widget_event_process_function;
    status = gx_window_root_delete(&window_root);
    EXPECT_EQ(GX_SUCCESS, status);

    gx_window_root_event_process(&window_root, 0); //should return GX_PTR_ERROR
    my_event.gx_event_type = 0;
    window_root.gx_widget_parent = 0;
    gx_window_root_event_process(&window_root, &my_event); //should return GX_PTR_ERROR
    
    gx_window_root_find(&widget, (GX_WINDOW_ROOT **)&widget.gx_widget_parent);//should return GX_INVALID_WIDGET
    gx_window_root_find(&widget, 0); //should return GX_PTR_ERROR
    
    gx_window_scroll_info_get(&window, 0, 0); //should return GX_PTR_ERROR
    window.gx_widget_type = 0;
    gx_window_scroll_info_get(&window, 0, &scroll_info); //should return GX_INVALID_WIDGET
    window.gx_widget_type = GX_TYPE_WINDOW;
    gx_window_scroll_info_get(&window, 0, &scroll_info); //should return GX_INVALID_TYPE
    status = gx_window_scroll_info_get(&window, GX_SCROLLBAR_HORIZONTAL, &scroll_info);
    EXPECT_EQ(GX_SUCCESS, status);
    status = gx_window_scroll_info_get(&window, GX_SCROLLBAR_VERTICAL, &scroll_info);
    EXPECT_EQ(GX_SUCCESS, status);
    
    gx_window_scrollbar_find(&window, 0, 0); //should return GX_PTR_ERROR
    window.gx_widget_type = 0;
    gx_window_scrollbar_find(&window, 0, (GX_SCROLLBAR **)&window.gx_widget_first_child); //should return GX_INVALID_WIDGET
    window.gx_widget_type = GX_TYPE_WINDOW;
    gx_window_scrollbar_find(&window, 0, (GX_SCROLLBAR **)&window.gx_widget_first_child); //should return GX_INVALID_TYPE
    
    gx_window_wallpaper_get(&window, 0);//should return GX_PTR_ERROR
    window.gx_widget_type = 0;
    gx_window_wallpaper_get(&window, &ulong_data); //should return GX_INVALID_WIDGET
    window.gx_widget_type = GX_TYPE_WINDOW;
    gx_window_wallpaper_get(&window, &ulong_data); //should return GX_SUCCESS
    
    gx_window_wallpaper_set(0, 0, 0);//should return GX_PTR_ERROR
    window.gx_widget_type = 0;
    gx_window_wallpaper_set(&window, 0, 0);//should return GX_INVALID_WIDGET
   
#ifdef GX_ENABLE_DEPRECATED_STRING_API 
    status = gx_widget_string_get(GX_NULL, 0, GX_NULL); // return_string == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);
    status = gx_widget_string_get(GX_NULL, 0, (VOID *)0x01); // widget == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);
    widget.gx_widget_type = 0;
    status = gx_widget_string_get(&widget, 0, (VOID *)0x01); // gx_widget_type == 0
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    widget.gx_widget_type = 1;
    gx_widget_status_remove(&widget, GX_STATUS_VISIBLE);
    status = gx_widget_string_get(&widget, 0, (GX_CONST GX_CHAR **)&buffer); // widget is invisible
    EXPECT_EQ(GX_INVALID_CANVAS, status);
    gx_widget_status_add(&widget, GX_STATUS_VISIBLE);
    gx_widget_string_get(&widget, 0, (GX_CONST GX_CHAR **)&buffer);
#endif
    status = gx_widget_string_get_ext(GX_NULL, 0, GX_NULL); // return_string == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);
    status = gx_widget_string_get_ext(GX_NULL, 0, (VOID *)0x01); // widget == GX_NULL
    EXPECT_EQ(GX_PTR_ERROR, status);
    widget.gx_widget_type = 0;
    status = gx_widget_string_get_ext(&widget, 0, (VOID *)0x01); // gx_widget_type == 0
    EXPECT_EQ(GX_INVALID_WIDGET, status);
    widget.gx_widget_type = 1;
    gx_widget_status_remove(&widget, GX_STATUS_VISIBLE);
    status = gx_widget_string_get_ext(&widget, 0, &string); // widget is invisible
    EXPECT_EQ(GX_INVALID_CANVAS, status);

#ifdef GX_UTF8_SUPPORT

    /* Tests for unicode_to_utf8 */

    /* unicode < 0x80 */
    _gx_utility_unicode_to_utf8(0x0, return_utf8_str, &return_utf8_size);

    /* unicode < 0x800 */
    _gx_utility_unicode_to_utf8(0x81, return_utf8_str, &return_utf8_size);

    /* unicode < 0x10000 */
    _gx_utility_unicode_to_utf8(0x801, return_utf8_str, &return_utf8_size);

    _gx_utility_unicode_to_utf8(0x8001, return_utf8_str, &return_utf8_size);

    /* unicode < 0x200000 */
    _gx_utility_unicode_to_utf8(0x10001, return_utf8_str, &return_utf8_size);

    _gx_utility_unicode_to_utf8(0x100001, return_utf8_str, &return_utf8_size);

    /* unicode < 0x4000000 */
    _gx_utility_unicode_to_utf8(0x200001, return_utf8_str, &return_utf8_size);

    _gx_utility_unicode_to_utf8(0x2000001, return_utf8_str, &return_utf8_size);

    /* unicode > 0x4000000 */
    _gx_utility_unicode_to_utf8(0x4000001, return_utf8_str, &return_utf8_size);

    buffer_ptr = buffer;
    buffer[0] = (CHAR)0xef;
    buffer[1] = (CHAR)0xbb;
    buffer[2] = (CHAR)0xbf;
    string.gx_string_ptr = buffer_ptr;
    string.gx_string_length = 3;
    _gx_utility_utf8_string_character_get(&string, GX_NULL, GX_NULL);

    buffer_ptr = buffer;
    buffer[0] = (CHAR)0xef;
    buffer[1] = (CHAR)0xbb;
    buffer[2] = (CHAR)0x0;
    string.gx_string_ptr = buffer_ptr;
    string.gx_string_length = 3;
    _gx_utility_utf8_string_character_get(&string, GX_NULL, GX_NULL);

    memset(buffer, 0, 4);
    buffer_ptr = buffer;
    buffer[0] = (CHAR)0xf0;
    string.gx_string_ptr = buffer_ptr;
    string.gx_string_length = 4;
    _gx_utility_utf8_string_character_get(&string, GX_NULL, GX_NULL);

    memset(buffer, 0, 4);
    buffer_ptr = buffer;
    buffer[0] = (CHAR)0xe0;
    string.gx_string_ptr = buffer_ptr;
    string.gx_string_length = 4;
    _gx_utility_utf8_string_character_get(&string, GX_NULL, GX_NULL);

    memset(buffer, 0, 4);
    buffer_ptr = buffer;
    buffer[0] = (CHAR)0xa0;
    string.gx_string_ptr = buffer_ptr;
    string.gx_string_length = 4;
    _gx_utility_utf8_string_character_get(&string, GX_NULL, GX_NULL);

    memset(buffer, 0, 4);
    buffer_ptr = buffer;
    buffer[0] = (CHAR)0xb0;
    string.gx_string_ptr = buffer_ptr;
    string.gx_string_length = 4;
    _gx_utility_utf8_string_character_get(&string, GX_NULL, GX_NULL);

    memset(buffer, 0, 4);
    buffer_ptr = buffer;
    buffer[0] = (CHAR)0x80;
    string.gx_string_ptr = buffer_ptr;
    string.gx_string_length = 4;
    _gx_utility_utf8_string_character_get(&string, GX_NULL, GX_NULL);

    memset(buffer, 0, 4);
    buffer_ptr = buffer;
    buffer[0] = (CHAR)0x40;
    string.gx_string_ptr = buffer_ptr;
    string.gx_string_length = 4;
    _gx_utility_utf8_string_character_get(&string, GX_NULL, GX_NULL);

    memset(buffer, 0, 4);
    buffer_ptr = buffer;
    buffer[0] = (CHAR)0x20;
    string.gx_string_ptr = buffer_ptr;
    string.gx_string_length = 4;
    _gx_utility_utf8_string_character_get(&string, GX_NULL, GX_NULL);

    memset(buffer, 0, 5);
    buffer_ptr = buffer;
    buffer[0] = (CHAR)0xf8;
    string.gx_string_ptr = buffer_ptr;
    string.gx_string_length = 5;
    _gx_utility_utf8_string_character_get(&string, GX_NULL, GX_NULL);

    memset(buffer, 0, 6);
    buffer_ptr = buffer;
    buffer[0] = (CHAR)0xfc;
    string.gx_string_ptr = buffer_ptr;
    string.gx_string_length = 6;
    _gx_utility_utf8_string_character_get(&string, GX_NULL, GX_NULL);

    memset(buffer, 0, 6);
    buffer_ptr = buffer;
    buffer[0] = (CHAR)0xff;
    string.gx_string_ptr = buffer_ptr;
    string.gx_string_length = 6;
    _gx_utility_utf8_string_character_get(&string, GX_NULL, GX_NULL);

    memset(buffer, 0, 6);
    buffer_ptr = buffer;
    buffer[0] = (CHAR)0xff;
    string.gx_string_ptr = buffer_ptr;
    string.gx_string_length = 6;
    _gx_utility_utf8_string_character_get(&string, &glyph_value, &character_count);

    memset(buffer, 0, 6);
    buffer_ptr = buffer;
    buffer[0] = (CHAR)0xff;
    string.gx_string_ptr = buffer_ptr;
    string.gx_string_length = 6;
    _gx_utility_utf8_string_character_get(&string, &glyph_value, &character_count);

    /* bytes < bytes_count */
    memset(buffer, 0, 5);
    buffer_ptr = buffer;
    buffer[0] = (CHAR)0xfc;
    string.gx_string_ptr = buffer_ptr;
    string.gx_string_length = 5;
    _gx_utility_utf8_string_character_get(&string, GX_NULL, GX_NULL);

    /* bytes < bytes_count */
    memset(buffer, 0, 5);
    buffer_ptr = buffer;
    buffer[0] = (CHAR)0xfc;
    string.gx_string_ptr = buffer_ptr;
    string.gx_string_length = 5;
    _gx_utility_utf8_string_character_get(&string, &glyph_value, &character_count);

    /* bytes < bytes_count */
    memset(buffer, 0, 5);
    buffer_ptr = buffer;
    buffer[0] = (CHAR)0xc0;
    buffer[1] = (CHAR)0x1;
    string.gx_string_ptr = buffer_ptr;
    string.gx_string_length = 5;
    _gx_utility_utf8_string_character_get(&string, &glyph_value, &character_count);

    string.gx_string_ptr = buffer_ptr;
    string.gx_string_length = 0;
    status = _gx_utility_utf8_string_character_get(&string, &glyph_value, &character_count);
    EXPECT_EQ(GX_INVALID_VALUE, status);

    display.gx_display_driver_row_pitch_get = display_driver_row_pitch_get;
    status = gx_canvas_create(&canvas, "aaa", &display, 0, 0, 0, 0, 0);
    EXPECT_EQ(GX_SUCCESS, status);

    gx_widget_status_remove(&widget, GX_STATUS_VISIBLE);
    _gx_multi_line_text_view_display_info_get((GX_MULTI_LINE_TEXT_VIEW *)&widget, 0, 0, &text_info, 0);

    _gx_system_active_timer_list = 0;
    _gx_system_animation_list = 0;
    _gx_system_timer_expiration(0); // no timers

    status = _gx_system_timer_start(0, 0, 0, 0); // owner is NULL
    EXPECT_EQ(GX_PTR_ERROR, status);
    _gx_system_free_timer_list = 0;
    status = _gx_system_timer_start(&widget, 0, 0, 0); // Unavailable to own a timer.
    EXPECT_EQ(GX_OUT_OF_TIMERS, status);

    _gx_system_root_window_created_list = &window_root;
    window_root.gx_widget_status |= GX_STATUS_VISIBLE;
    window_root.gx_widget_next = 0;
    my_event.gx_event_display_handle = 0;
    window_root.gx_window_root_canvas = &canvas;
    canvas.gx_canvas_display = &display;
    display.gx_display_driver_data = buffer;
    _gx_system_top_root_find(&my_event);

    _tx_thread_system_state = 1;
    tx_mutex_get(&_gx_system_protect, TX_WAIT_FOREVER);
    _gx_system_lock();
    tx_mutex_put(&_gx_system_protect);
    _tx_thread_system_state = 0;

    _gx_system_thread.tx_thread_state = 1;
    _gx_system_thread.tx_thread_delayed_suspend = 0;
    status = _gx_system_start();
    EXPECT_EQ(GX_SYSTEM_ERROR, status);

    parent.gx_widget_last_child = &widget;
    widget.gx_widget_previous = 0;
    gx_widget_status_add(&widget, GX_STATUS_VISIBLE);
    _gx_system_top_widget_find(&parent, point, 0);

    string_scroll_wheel.gx_string_scroll_wheel_string_list = 0;
    string_scroll_wheel.gx_scroll_wheel_total_rows = 2;
    _gx_string_scroll_wheel_text_get(&string_scroll_wheel, 4, &string);

    /* VERTICAL_SCROLL is found. */
    window.gx_widget_type = 1;
    window.gx_widget_first_child = (GX_WIDGET *)&scrollbar;
    scrollbar.gx_widget_type = GX_TYPE_VERTICAL_SCROLL;
    scrollbar.gx_widget_next = 0;
    scrollbar.gx_scrollbar_info.gx_scroll_maximum = 5;
    scrollbar.gx_scrollbar_info.gx_scroll_minimum = 2;
    scrollbar.gx_scrollbar_info.gx_scroll_value = 0;
    gx_widget_status_add(&scrollbar, GX_STATUS_NONCLIENT);
    status = gx_window_client_scroll(&window, 3, 3);
    EXPECT_EQ(GX_SUCCESS, status);

    /* HORIZONTAL_SCROLL is found. */
    scrollbar.gx_widget_type = GX_TYPE_HORIZONTAL_SCROLL;
    status = gx_window_client_scroll(&window, 3, 3);
    EXPECT_EQ(GX_SUCCESS, status);

#endif /* GX_UTF8_SUPPORT */

    // should we test draw functions here? Because draw function will not return error.
    /* draw functions
    gx_display_create(&display, "linux_display", gx_validation_display_driver_setup_24xrgb, 0, 0); //create display
    
    // draw functions
    gx_accordion_menu_draw(&accordion_menu); // draw
    gx_button_background_draw(&button);
    gx_button_draw(&button);
    gx_checkbox_draw(&checkbox);
    gx_circular_gauge_background_draw(&circular_gauge);
    gx_circular_gauge_draw(&circular_gauge);
    gx_icon_background_draw(&icon);
    gx_icon_button_draw(&icon_button);
    gx_icon_draw(&icon);
    gx_line_chart_axis_draw(&line_chart);
    gx_line_chart_data_draw(&line_chart);
    gx_line_chart_draw(&line_chart);
    gx_menu_draw(&menu);
    gx_menu_text_draw(&menu);
    gx_multi_line_text_button_draw(&multi_line_text_button);
    gx_multi_line_text_button_text_draw(&multi_line_text_button);
    gx_multi_line_text_view_draw(&multi_line_text_view);
    gx_pixelmap_button_draw(&pixelmap_button);
    gx_pixelmap_prompt_draw(&pixelmap_prompt);
    gx_pixelmap_slider_draw(&pixelmap_slider);
    gx_progress_bar_background_draw(&progress_bar);
    gx_progress_bar_draw(&progress_bar);
    gx_progress_bar_text_draw(&progress_bar);
    gx_prompt_draw(&prompt);
    gx_prompt_text_draw(&prompt);
    gx_radial_progress_bar_background_draw(&radial_progress_bar);
    gx_radial_progress_bar_draw(&radial_progress_bar);
    gx_radial_progress_bar_text_draw(&radial_progress_bar);
    gx_radio_button_draw(&radio_button);
    gx_scroll_thumb_draw(&scroll_thumb);
    gx_scrollbar_draw(&scrollbar);
    gx_single_line_text_input_draw(&single_line_text_input);
    gx_slider_draw(&slider);
    gx_slider_needle_draw(&slider);
    gx_slider_tickmarks_draw(&slider);
    gx_text_button_draw(&text_button);
    gx_text_button_text_draw(&text_button);
    gx_text_scroll_wheel_draw(&text_scroll_wheel);
    gx_tree_view_draw(&tree_view);
    gx_widget_background_draw(&widget);
    gx_widget_border_draw(&widget, 0, 0, 0, 0);
    gx_widget_children_draw(&widget);
    gx_widget_draw(&widget);
    gx_widget_text_draw(&widget, 0, 0, 0, 0, 0);
    gx_widget_text_id_draw(&widget, 0, 0, 0, 0, 0);
    gx_window_background_draw(&window);
    gx_window_draw(&window);
    
    
    gx_canvas_delete()
    gx_display_delete(&display, gx_validation_graphics_driver_cleanup_24xrgb);//delete display

    */
    gx_display_delete(&default_display, gx_validation_graphics_driver_cleanup_24xrgb);//delete default display

    if(failed_tests == 0) 
    {
        gx_validation_print_test_result(TEST_SUCCESS);
        exit(0);
    }
    else 
    {
        gx_validation_print_test_result(TEST_FAIL);
        exit(1);
    }
}
#endif


