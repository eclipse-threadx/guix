/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_system_private_string_list_copy", /* Test name */
    209, 116, 380, 353  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the pixelmap
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

const char *test_string_list_1[12]=
{
"1234567890",
"2345678901",
"3456789012",
"4567890123",
"5678901234",
"6789012345",
"7890123456",
"8901234567",
"9012345678",
"0123456789",
"1123456789",
"1234567899"
};

const char *test_string_list_2[12]=
{
"1",
"2",
"3",
"4",
"5",
"6",
"7",
"8",
"9",
"0",
"1",
"2"
};

#define TEST_MEMORY_SIZE 100
GX_UBYTE test_memory[TEST_MEMORY_SIZE];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT                     frame_id = 1;
INT                     index;
GX_STRING_SCROLL_WHEEL *wheel;

    /* Create scroll wheel screen dynamically. */
    gx_studio_named_widget_create("scroll_wheel_screen", GX_NULL, (GX_WIDGET **)&pScrollWheelScreen);

        /* Search 3 wheel widget. */
    gx_widget_find(pScrollWheelScreen, ID_MONTH_WHEEL, GX_SEARCH_DEPTH_INFINITE, &wheel);

    gx_string_scroll_wheel_string_list_set(wheel, GX_NULL, 0);
    gx_widget_style_add(wheel, GX_STYLE_TEXT_COPY);
    gx_widget_style_remove(wheel, GX_STYLE_TEXT_SCROLL_WHEEL_ROUND);
    gx_string_scroll_wheel_string_list_set(wheel, test_string_list_2, 12);

    /* Toggle from button screen to scroll wheel screen. */
    ToggleScreen((GX_WINDOW *)pScrollWheelScreen, pButtonScreen);

    tx_byte_pool_delete(&rotate_pool);
    tx_byte_pool_create(&rotate_pool, "test_memory_pool", test_memory, TEST_MEMORY_SIZE);
  

    for(index = 0; index < 6; index++)
    {
        switch(index)
        {
        case 0:
            sprintf(test_comment, "set memory pool size to %d, add GX_STYLE_TEXT_COPY style", TEST_MEMORY_SIZE);
            break;

        case 1:
        case 2:
            gx_string_scroll_wheel_string_list_set(wheel, test_string_list_1, 12);
            sprintf(test_comment, "set long string list");
            break;

        case 3:
            gx_string_scroll_wheel_string_list_set(wheel, test_string_list_2, 12);
            sprintf(test_comment, "set short string list");
            break;

        case 4:
            gx_string_scroll_wheel_string_list_set(wheel, GX_NULL, 0);
            sprintf(test_comment, "set string list GX_NULL");
            break;

        case 5:
            gx_string_scroll_wheel_string_list_set(wheel, GX_NULL, 0);
            sprintf(test_comment, "set string list GX_NULL");
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
