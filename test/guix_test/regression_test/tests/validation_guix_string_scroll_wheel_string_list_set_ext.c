/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"
#include "gx_system.h"

TEST_PARAM test_parameter = {
    "guix_string_scroll_wheel_string_list_set_ext", /* Test name */
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

char test_comment[256];

GX_CONST GX_STRING string_list_1[] = {
{"Jan", 3},
{"Feb", 3},
{"Mar", 3},
{"Apr", 3},
{"May", 3},
{"Jun", 3},
{"Jul", 3},
{"Aug", 3},
{"Sep", 3},
{"Oct", 3},
{"Nov", 3},
{"Dec", 3}
};

GX_CONST GX_STRING string_list_2[] = {
{"one", sizeof("one") - 1},
{"two", sizeof("two") - 1},
{"three", sizeof("three") - 1},
{"four", sizeof("four") - 1},
{"five", sizeof("five") - 1},
{"six", sizeof("six") - 1},
{"seven", sizeof("seven") - 1},
{"eight", sizeof("eight") - 1},
{"nine", sizeof("nine") - 1},
{"ten", sizeof("ten") - 1},
{"eleven",sizeof("eleven") - 1},
{"twelve",sizeof("twelve") - 1},
{"thirteen", sizeof("thirteen") - 1},
{"fourteen", sizeof("fourteen") - 1},
{"fifteen", sizeof("fifteen") - 1},
{"sisteen", sizeof("sisteen") - 1},
{"seventeen", sizeof("seventeen") - 1},
{"eighteen", sizeof("eighteen") - 1},
{"nineteen", sizeof("nineteen") - 1},
{"twenty", sizeof("twenty") - 1}
};

GX_CONST GX_STRING string_list_3[] = {
{"Monday", sizeof("Monday") - 1},
{"Tuesday", sizeof("Tuesday") - 1},
{"Wednesday", sizeof("Wednesday") - 1},
{GX_NULL, 0},
{"Thursday", sizeof("Thursday") - 1},
{"Friday", sizeof("Friday") - 1},
{"Satday", sizeof("Satday") - 1},
{"Sunday", sizeof("Sunday") - 1}
};

GX_CONST GX_CHAR *old_string_list_1[] = {
"old string 1",
"old string 2",
"old string 3",
GX_NULL,
"old string 4"
};

GX_CHAR  invalid_string[GX_MAX_STRING_LENGTH + 1];
GX_CONST GX_CHAR *old_string_list_2[] = {
"row 1",
"row 2",
"row 3",
(GX_CONST GX_CHAR *)invalid_string
};

GX_RESOURCE_ID string_id_list[] = {
GX_STRING_ID_ITEM_1,
GX_STRING_ID_ITEM_2,
GX_STRING_ID_MENU_1,
GX_STRING_ID_MENU_3
};

TX_SEMAPHORE semaphore_memory;

static VOID control_thread_entry(ULONG input)
{
INT                     frame_id = 1;
GX_STRING_SCROLL_WHEEL *month_wheel;
INT                     index;

    /* Create scroll wheel screen dynamically. */
    gx_studio_named_widget_create("scroll_wheel_screen", GX_NULL, (GX_WIDGET **)&pScrollWheelScreen);

    /* Search month wheel widget. */
    gx_widget_find(pScrollWheelScreen, ID_MONTH_WHEEL, GX_SEARCH_DEPTH_INFINITE, &month_wheel);
    gx_widget_style_add((GX_WIDGET *)month_wheel, GX_STYLE_TEXT_COPY);
    gx_widget_style_remove(month_wheel, GX_STYLE_TEXT_SCROLL_WHEEL_ROUND);

    /* Toggle from button screen to scroll wheel screen. */
    ToggleScreen((GX_WINDOW *)pScrollWheelScreen, pButtonScreen);

    for(index = 0; index < 14; index++)
    {
        gx_widget_hide(month_wheel);
        switch(index)
        {
        case 0:
            sprintf(test_comment, "Create scroll wheel screen, add text copy style to month wheel");
            break;

        case 1:
            gx_string_scroll_wheel_string_list_set_ext(month_wheel, string_list_1, sizeof(string_list_1)/sizeof(GX_STRING));
            sprintf(test_comment, "Test text copy, set a string list that have shorter strings.");
            break;

        case 2:
            gx_string_scroll_wheel_string_list_set_ext(month_wheel, string_list_2, sizeof(string_list_2)/sizeof(GX_STRING));
            sprintf(test_comment, "Test text copy, set a string list that have more string count");
            break;

        case 3:
            gx_string_scroll_wheel_string_list_set_ext(month_wheel, string_list_3, sizeof(string_list_3)/sizeof(GX_STRING));
            sprintf(test_comment, "Test text copy, set a string list that have less string count");
            break;

        case 4:
            gx_string_scroll_wheel_string_list_set(month_wheel, old_string_list_1, sizeof(old_string_list_1)/sizeof(GX_CHAR *));
            sprintf(test_comment, "Test text copy, set string list with old API");
            break;

        case 5:
            gx_string_scroll_wheel_string_list_set_ext(month_wheel, string_list_3, sizeof(string_list_3)/sizeof(GX_STRING));
            sprintf(test_comment, "Test text copy, set string list with new API");
            break;

        case 6:
            gx_system_memory_allocator_set(GX_NULL, GX_NULL);
            gx_string_scroll_wheel_string_list_set_ext(month_wheel, string_list_2, sizeof(string_list_2)/sizeof(GX_STRING));
            sprintf(test_comment, "Test text copy, set memory allocator and free function pointer to GX_NULL, and set string list");
            break;

        case 7:
            gx_system_memory_allocator_set(rotate_memory_allocate, rotate_memory_free);
            gx_string_scroll_wheel_string_id_list_set(month_wheel, string_id_list, sizeof(string_id_list)/sizeof(INT));
            sprintf(test_comment, "Set memory allocator and free function and set string id list");
            break;

        case 8:
            memset(invalid_string, 'x', GX_MAX_STRING_LENGTH + 1);
            gx_string_scroll_wheel_string_list_set(month_wheel, old_string_list_2, sizeof(old_string_list_2)/sizeof(GX_CHAR *));
            sprintf(test_comment, "Set invalid string list with old API");
            break;

        case  9:
             gx_string_scroll_wheel_string_list_set_ext(month_wheel, GX_NULL, 0);
             sprintf(test_comment, "Set null pointer string list with extended API");
             break;

        case 10:
             gx_string_scroll_wheel_string_list_set_ext(month_wheel, string_list_2, sizeof(string_list_2)/sizeof(GX_STRING));
             sprintf(test_comment, "Set string list with extended aPI");
             break;

        case 11:
             gx_string_scroll_wheel_string_list_set_ext(month_wheel, GX_NULL, 0);
             sprintf(test_comment, "Set null pointer string list with extended API");
             break;

        case 12:
            gx_widget_style_remove(month_wheel, GX_STYLE_TEXT_COPY);
            memset(invalid_string, 'x', GX_MAX_STRING_LENGTH + 1);
            gx_string_scroll_wheel_string_list_set(month_wheel, old_string_list_2, sizeof(old_string_list_2)/sizeof(GX_CHAR *));
            sprintf(test_comment, "Remove text copy style, set invalid string list with old API");
            break;

        case 13:
            gx_string_scroll_wheel_string_list_set_ext(month_wheel, string_list_3, sizeof(string_list_3)/sizeof(GX_STRING));
            sprintf(test_comment, "Set string list with extended API");
            break;
        }
        gx_widget_show(month_wheel);
        gx_validation_set_frame_id(frame_id++);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}

