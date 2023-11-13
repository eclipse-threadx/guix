/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_utility.h"

TEST_PARAM test_parameter = {
    "guix_utility_no_output", /* Test name */
    0, 0, 0, 0  /* Define the coordinates of the capture area.*/
};

static VOID      control_thread_entry(ULONG);

int main(int argc, char ** argv)
{
  tx_kernel_enter();
  return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
       on the line drawing example. These operations simulate 
       user input. */
    gx_validation_application_define(first_unused_memory);
    
    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Create a dedicated thread to perform various operations
       on the pixelmap drawing example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);
       
}

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"
#include "demo_guix_screenshot_to_bitmap_32bpp.c"

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */

GX_CONST GX_CHAR text_short[] = "short";
GX_CONST GX_CHAR text_long[] = "long string";

static VOID control_thread_entry(ULONG input)
{
UINT         status;
INT          failed_tests = 0;
GX_RECTANGLE rect;
GX_PIXELMAP map;
int         index;
GX_GRADIENT gradient[4];
GX_CHAR     get_value[2];
int         angle;
GX_PIXELMAP destination;
UINT        expect_status;
GX_POINT    point;
GX_STRING   string_1;
GX_STRING   string_2;

    /* Test invalid call to gx_utility_canvas_to_bmp. */
    for(index = 0; index < 3; index++)
    {
        switch(index)
        {
        case 0:
            root->gx_window_root_canvas->gx_canvas_display->gx_display_color_format = 0;
            expect_status = GX_NOT_SUPPORTED;
            break;

        case 1:
            root->gx_window_root_canvas->gx_canvas_display->gx_display_color_format = GX_COLOR_FORMAT_8BIT_PALETTE;
            root->gx_window_root_canvas->gx_canvas_display->gx_display_palette = (GX_COLOR *)&map;
            root->gx_window_root_canvas->gx_canvas_display->gx_display_palette_size = 0;
            expect_status = GX_FAILURE;
            break;

        case 2:
            root->gx_window_root_canvas->gx_canvas_display->gx_display_palette = GX_NULL;
            expect_status = GX_FAILURE;
            break;
        }

        gx_utility_rectangle_define(&rect, 0, 0, 100, 100);
        status = gx_utility_canvas_to_bmp(root->gx_window_root_canvas, &rect, write_data_callback);
        EXPECT_EQ(expect_status, status);
    }

    /* Test invalid call to _gx_utility_alphamap_create. */
    status = _gx_utility_alphamap_create(100, 0, &map);
    EXPECT_EQ(GX_FAILURE, status);

    /* Test gx_utility_pixelmap_resize. */
    memset(&map, 0, sizeof(GX_PIXELMAP));
    map.gx_pixelmap_width = 50;
    map.gx_pixelmap_height = 50;
    status = gx_utility_pixelmap_resize(&map, &destination, 100, 100);
    EXPECT_EQ(GX_NOT_SUPPORTED, status);

    /* Test gx_utility_pixelmap_rotate. */
    status = gx_utility_pixelmap_rotate(&map, 30, &destination, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_INVALID_FORMAT, status);

    map.gx_pixelmap_format = GX_COLOR_FORMAT_32ARGB;
    map.gx_pixelmap_flags = GX_PIXELMAP_COMPRESSED;
    status = gx_utility_pixelmap_rotate(&map, 30, &destination, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_INVALID_FORMAT, status);

    /* Test gx_utility_pixelmap_simple_rotate. */
    status = gx_utility_pixelmap_simple_rotate(&map, 30, &destination, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_INVALID_VALUE, status);

    status = gx_utility_pixelmap_simple_rotate(&map, 90, &destination, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_INVALID_FORMAT, status);

    map.gx_pixelmap_flags = 0;
    map.gx_pixelmap_format = 0;
    status = gx_utility_pixelmap_simple_rotate(&map, 90, &destination, GX_NULL, GX_NULL);
    EXPECT_EQ(GX_INVALID_FORMAT, status);

    /* Test _gx_utility_gradient_create. */
    status = _gx_utility_gradient_create(&gradient[0], 3, 1, GX_GRADIENT_TYPE_ALPHA|GX_GRADIENT_TYPE_MIRROR, 0, 255);
    EXPECT_EQ(GX_INVALID_SIZE, status);

    _gx_utility_gradient_create(&gradient[0], 3, 10, GX_GRADIENT_TYPE_ALPHA|GX_GRADIENT_TYPE_MIRROR, 0, 255);
    _gx_utility_gradient_create(&gradient[1], 3, 10, GX_GRADIENT_TYPE_ALPHA|GX_GRADIENT_TYPE_MIRROR, 0, 128);
    _gx_utility_gradient_create(&gradient[2], 4, 10, GX_GRADIENT_TYPE_ALPHA|GX_GRADIENT_TYPE_MIRROR, 0, 128);
    rotate_memory_free((void *)gradient[0].gx_gradient_pixelmap.gx_pixelmap_data);
    gradient[0].gx_gradient_pixelmap.gx_pixelmap_data = GX_NULL;
    _gx_utility_gradient_create(&gradient[3], 3, 12, GX_GRADIENT_TYPE_ALPHA|GX_GRADIENT_TYPE_MIRROR, 0, 128);

    if(gradient[3].gx_gradient_next != &gradient[2] ||
       gradient[2].gx_gradient_next != &gradient[1] ||
       gradient[1].gx_gradient_next != &gradient[0])
    {
        failed_tests++;
    }

    /* Test _gx_utility_gradient_delete. */
    gx_system_memory_allocator_set(GX_NULL, GX_NULL);
    _gx_utility_gradient_delete(&gradient[3]);

    /* Test gx_utility_ltoa. */
    gx_utility_ltoa(1111, get_value, 2);
    if(get_value[0] != '1' ||
       get_value[1] != '\0')
    {
        failed_tests++;
    }

    /* Test gx_utility_math_asin. */
    angle = gx_utility_math_asin(GX_FIXED_VAL_MAKE(300));
    EXPECT_EQ(0, angle);

    /* Test _gx_utility_string_length_check. */
    status = _gx_utility_string_length_check("test", GX_NULL, 4);
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_utility_circle_point_get(0, 0, 100, -90, &point);
    EXPECT_EQ(GX_SUCCESS, status);

    status = gx_utility_circle_point_get(0, 0, 100, 90, GX_NULL);
    EXPECT_EQ(GX_PTR_ERROR, status);

    status = gx_utility_circle_point_get(0, 0, 0, 90, &point);
    EXPECT_EQ(GX_INVALID_VALUE, status);

    string_1.gx_string_ptr = text_short;
    string_1.gx_string_length = sizeof(text_short) - 1;
    
    string_2.gx_string_ptr = text_long;
    string_2.gx_string_length = sizeof(text_long) - 1;

    status = _gx_utility_string_compare(&string_1, &string_2, string_2.gx_string_length);
    EXPECT_EQ(GX_FALSE, status);

    status = _gx_utility_string_compare(&string_2, &string_1, string_2.gx_string_length);
    EXPECT_EQ(GX_FALSE, status);

    if(point.gx_point_x != 0 || point.gx_point_y != 100)
    {
        failed_tests++;
    }

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

