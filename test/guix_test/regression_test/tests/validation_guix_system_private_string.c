/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_system_private_string", /* Test name */
    116, 139, 253, 170  /* Define the coordinates of the capture area.
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

char test_string[]="123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";

#define TEST_MEMORY_SIZE 100
GX_UBYTE test_memory[TEST_MEMORY_SIZE];

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
INT             frame_id = 1;
GX_TEXT_BUTTON *button;
INT             index;
GX_STRING       string;

    button = &button_screen.button_screen_text_button_1;
 
    tx_byte_pool_delete(&rotate_pool);
    tx_byte_pool_create(&rotate_pool, "test_memory_pool", test_memory, TEST_MEMORY_SIZE);
  
    gx_widget_hide(button);
    gx_text_button_text_set(button, GX_NULL);
    gx_widget_style_add(button, GX_STYLE_TEXT_COPY);
    gx_widget_show(button);
    gx_text_button_text_set(button, "text_button");

    for(index = 0; index < 9; index++)
    {
        switch(index)
        {
        case 0:
            sprintf(test_comment, "set memory pool size to %d, add GX_STYLE_TEXT_COPY style", TEST_MEMORY_SIZE);
            break;

        case 1:
        case 2:
            gx_text_button_text_set(button, test_string);
            sprintf(test_comment, "set %d bytes long string", sizeof(test_string));
            break;

        case 3:
            gx_text_button_text_set(button, "test");
            sprintf(test_comment, "set string \"test\"");
            break;

        case 4:
            gx_text_button_text_set(button, "testtest");
            sprintf(test_comment, "set string \"testtest\"");
            break;

        case 5:
            gx_text_button_text_set(button, GX_NULL);
            sprintf(test_comment, "set text GX_NULL");
            break;

        case 6:
            gx_text_button_text_id_set(button, 1);
            sprintf(test_comment, "set text id = 0");
            break;

        case 7:
            gx_text_button_text_set_ext(button, GX_NULL);
            sprintf(test_comment, "set text GX_NULL");
            break;

        case 8:
            string.gx_string_ptr = GX_NULL;
            string.gx_string_length = 0;
            gx_text_button_text_set_ext(button, &string);
            sprintf(test_comment, "set text GX_NULL");
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
