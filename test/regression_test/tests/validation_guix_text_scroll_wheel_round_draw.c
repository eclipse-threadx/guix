/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_text_scroll_wheel_round_draw", /* Test name */
    100, 117, 525, 336  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the line
                         drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command line argument. */
    gx_validation_setup(argc, argv);

    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual line example routine. */
    gx_validation_application_define(first_unused_memory);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_24xrgb  
#undef win32_graphics_driver_setup_24xrgb  
#endif
#define win32_graphics_driver_setup_24xrgb  gx_validation_graphics_driver_setup_24xrgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets.c"

char test_comment[255];

GX_UBYTE chinese_character[] = {0xe4, 0xb8, 0xad};
GX_CONST GX_CHAR *string_list[]={
"January",
"February",
"",
"",
(GX_CHAR *)chinese_character,
"May",
"June"
};

#define TEST_MEMORY_SIZE 3000
GX_COLOR test_memory[TEST_MEMORY_SIZE];
extern TX_BYTE_POOL       rotate_pool;

static GX_CONST GX_FONT test_font = 
{
    0x04,                                    /* format                         */
    0,                                       /* line pre-space                 */
    0,                                       /* line post-space                */ 
    0,                                       /* font data height               */
    13,                                      /* font baseline offset           */
    0x20,                                    /* first glyph within data page   */
    0x19,                                    /* last glyph within data page    */
    {GX_NULL},                               /* pointer to glyph data          */
    GX_NULL                                  /* next font page                 */
};

extern GX_FONT _gx_system_font_8bpp;
GX_CONST GX_FONT *test_font_table[]=
{
    &_gx_system_font_8bpp,
    &_gx_system_font_8bpp,
    &_gx_system_font_8bpp,
    &_gx_system_font_8bpp,
    &test_font
};

static VOID control_thread_entry(ULONG input)
{
INT                      frame_id = 1;
GX_STRING_SCROLL_WHEEL  *month_wheel;
INT                      index;
GX_RECTANGLE             size;

    /* Create scroll wheel screen dynamically. */
    gx_studio_named_widget_create("scroll_wheel_screen", GX_NULL, (GX_WIDGET **)&pScrollWheelScreen);

    gx_widget_find(pScrollWheelScreen, ID_MONTH_WHEEL, GX_SEARCH_DEPTH_INFINITE, &month_wheel);

    tx_byte_pool_delete(&rotate_pool);
    tx_byte_pool_create(&rotate_pool, "memory_pool", test_memory, TEST_MEMORY_SIZE * sizeof(GX_COLOR));
    gx_scroll_wheel_selected_background_set(month_wheel, 0);
    gx_scroll_wheel_gradient_alpha_set(month_wheel, 0, 0);

    /* Toggle from button screen to scroll wheel screen. */
    ToggleScreen((GX_WINDOW *)pScrollWheelScreen, pButtonScreen);

    for(index = 0; index < 9; index++)
    {
        switch(index)
        {
        case 0:
            gx_widget_hide(month_wheel);
            size = month_wheel->gx_widget_size;
            size.gx_rectangle_bottom = size.gx_rectangle_top + 57;
            gx_widget_resize(month_wheel, &size);
            gx_widget_show(month_wheel);
            sprintf(test_comment, "resize to make widget height = 58");
            break;

        case 1:   
            gx_utility_rectangle_define(&size, 211, 164, 380, 274);
            gx_widget_resize(month_wheel, &size);
            gx_string_scroll_wheel_string_list_set(month_wheel, string_list, 7);
            sprintf(test_comment, "set new string list and resize");
            break;

        case 2:
            gx_system_memory_allocator_set(GX_NULL, GX_NULL);
            sprintf(test_comment, "set memory allocator/free null");
            break;

        case 3:
            gx_text_scroll_wheel_font_set(month_wheel, 1000, 1000);
            sprintf(test_comment, "set invalid font id");
            break;

        case 4:
            gx_system_memory_allocator_set(rotate_memory_allocate, rotate_memory_free);
            gx_display_font_table_set(root->gx_window_root_canvas->gx_canvas_display, (GX_FONT **)test_font_table, 5);
            gx_text_scroll_wheel_font_set(month_wheel, 4, 4);
            sprintf(test_comment, "reset font table, with font height = 0");
            break;

        case 5:
            month_wheel->gx_scroll_wheel_row_height = 0;
            sprintf(test_comment, "set row height = 0");
            break;

        case 6:
            gx_widget_style_remove(month_wheel, GX_STYLE_TEXT_SCROLL_WHEEL_ROUND);
            sprintf(test_comment, "removed round style");
            break;

        case 7:
            gx_widget_hide(month_wheel);
            month_wheel->gx_scroll_wheel_row_height = 45;
            gx_widget_border_style_set(month_wheel, GX_STYLE_BORDER_THICK);
            gx_widget_style_add(month_wheel, GX_STYLE_TEXT_SCROLL_WHEEL_ROUND);
            gx_utility_rectangle_define(&size, 210, 117, 214, 352);
            gx_widget_resize(month_wheel, &size);
            gx_widget_show(month_wheel);
            sprintf(test_comment, "add round style, set thick border, resize to make client width = 0");
            break;

        case 8:
            gx_widget_hide(month_wheel);
            gx_utility_rectangle_define(&size, 210, 117, 320, 120);
            gx_widget_resize(month_wheel, &size);
            gx_widget_show(month_wheel);
            sprintf(test_comment, "resize to make client height = 0");
            break;

        }

        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}

