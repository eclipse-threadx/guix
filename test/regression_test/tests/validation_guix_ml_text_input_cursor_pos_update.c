/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_system.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_ml_text_input_cursor_pos_update", /* Test name */
    30, 120, 245, 330  /* Define the coordinates of the capture area.
                         In this test, we only need to capture the multi 
                         line text input drawing area.  */
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
       on the multi line text input example. These operations simulate 
       user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Terminate the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual multi line text input routine. */
    gx_validation_application_define(first_unused_memory);

}


/* Replace the default graphics driver with the validation driver. */
#ifdef win32_graphics_driver_setup_565rgb  
#undef win32_graphics_driver_setup_565rgb  
#endif
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb

#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_ml_text_input_16bpp.c"


char test_comment[256];
char test_string_1[]="1\r2\r3\r4\r5\r6\r7\r8\r9\r10\r11\r12\r13\r14\r15\r16\r17\r18\r19\r20\r21\r22\r23\r24\r25\r26\r27\r28\r29\r30\r31\r32\r33\r34\r35\r36\r37\n\n\n";

char test_string_2[]="1\r2\r3\r4\r5\r6\r7\r8\r9\r10\r11\r12\r13\r14\r15\r16\r17\r18\r19\r20\r21\r22\r23\r24\r25\r26\r27\r28\r29\r30\r31\r32\r33\r34\r35\r36\r37\r\r\r";

char test_string_3[]="1\r2\r3\r4\r5\r6\r7\r88888888888888888899\r10\r11\r12\r13\r14\r15\r16\r17\r18\r19\r20\r21\r22\r23\r24\r25\r26\r27\r28\r29\r30\r31\r32\r33\r34\r35\r36\r37\r\r\r";

char test_string_4[]="1234567890123456ssA\r2\r3\r4\r5\r6\r7\r8\r9\r10\r11\r12\r13\r14\r15\r16\r17\r18\r19\r20\r21\r22\r23\r24\r25\r26\r27\r28\r29\r30\r31\r32\r33\r34\r35";

char test_string_5[]="1\n2\r3\r4\r5\r6\r7\r8\r9\r10\r11\r12\r13\r14\r15\r16\r17\r18\r19\r20\r21\r22\r23\r24\r25\r26\r27\r28\r29\r30\r31\r32\r33\r34\r35\r36\r37\n\n\n";

typedef struct TEST_STRUCT{
char *test_string;
int   click_x;
int   click_y;
int   insert_pos;
USHORT  key_value;
}TEST;

TEST test_list[]={
{test_string_1,   0,   0, 0, 0},
{test_string_2,   0,   0, 0, 0},
{test_string_3,  35, 298, 0, GX_KEY_DELETE},
{test_string_4, 211, 137, 0, 's'},
{test_string_5,  36, 154, 0, GX_KEY_DELETE},
{test_string_5,  36, 136, 0, GX_KEY_DELETE},
{test_string_5,   0,   0, 1000, 0},
{GX_NULL, 0, 0, 0, 0}
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int                       frame_id = 1;
int                       index;
GX_MULTI_LINE_TEXT_INPUT *text_input;
GX_EVENT                  my_event;
TEST                     *test = test_list;

    text_input = &((WINDOW_CONTROL_BLOCK *)pMainScreen)->window_ml_text_input;
    memset(&my_event, 0, sizeof(GX_EVENT));

    /* Remove the cursor blink flag. */
    gx_multi_line_text_input_style_remove(text_input, GX_STYLE_CURSOR_BLINK);

    gx_system_focus_claim((GX_WIDGET *)text_input);

    my_event.gx_event_display_handle = 1;

    while(test->test_string)
    {
        for(index = 0; index < 5; index++)
        {
            switch(index)
            {
            case 0:
                gx_multi_line_text_input_buffer_clear(text_input);
                gx_multi_line_text_input_text_set(text_input, test->test_string); 
                sprintf(test_comment, "set test string");
                break;

            case 1:
                my_event.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
                my_event.gx_event_payload.gx_event_intdata[1] = 1000;
                my_event.gx_event_payload.gx_event_intdata[0] = 0;
                my_event.gx_event_target = (GX_WIDGET *)text_input;
                gx_system_event_send(&my_event);
                sprintf(test_comment, "scroll up");
                break;
           
            case 2:
                if(test->click_x)
                {
                    my_event.gx_event_type = GX_EVENT_PEN_DOWN;
                    my_event.gx_event_payload.gx_event_pointdata.gx_point_x = test->click_x;
                    my_event.gx_event_payload.gx_event_pointdata.gx_point_y = test->click_y;
                    my_event.gx_event_target = (GX_WIDGET *)text_input;
                    gx_system_event_send(&my_event);
    
                    my_event.gx_event_type = GX_EVENT_PEN_UP;
                    gx_system_event_send(&my_event);
                    sprintf(test_comment, "click on (%d, %d)", test->click_x, test->click_y);
                }
                else
                {
                    my_event.gx_event_type = GX_EVENT_KEY_DOWN;
                    my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_RIGHT_ARROW;
                    gx_system_event_send(&my_event);
                    sprintf(test_comment, "right key down");
                }
                break;

            case 3:
                if(test->click_x)
                {
                    my_event.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
                    my_event.gx_event_payload.gx_event_intdata[1] = 0;
                    my_event.gx_event_payload.gx_event_intdata[0] = 1000;
                    my_event.gx_event_target = (GX_WIDGET *)text_input;
                    gx_system_event_send(&my_event);
                    sprintf(test_comment, "scroll down");

                }
                else
                {
                    if(test->insert_pos)
                    {
                        text_input->gx_multi_line_text_input_text_insert_position = test->insert_pos;
                        sprintf(test_comment, "set insert pos = %d, left key down", test->insert_pos);
                    }
                    else
                    {
                        sprintf(test_comment, "left key down");
                    }
                    my_event.gx_event_type = GX_EVENT_KEY_DOWN;
                    my_event.gx_event_payload.gx_event_ushortdata[0] = GX_KEY_LEFT_ARROW;
                    gx_system_event_send(&my_event);
                }
                break;

            case 4:
                if(test->click_x)
                {
                    if(test->key_value == GX_KEY_DELETE)
                    {
                        sprintf(test_comment, "delete key down");
                    }
                    else
                    {
                        sprintf(test_comment, "insert '%c'", (char)test->key_value);
                    }
                    my_event.gx_event_type = GX_EVENT_KEY_DOWN;
                    my_event.gx_event_payload.gx_event_ushortdata[0] = test->key_value;
                    gx_system_event_send(&my_event);
                }
                else
                {
                    continue;
                }
                break;
            }

            gx_validation_set_frame_id(frame_id++);
            gx_validation_set_frame_comment(test_comment);
            gx_validation_screen_refresh();
        }
        test++;
    }

    gx_validation_end();

    exit(0);
}
