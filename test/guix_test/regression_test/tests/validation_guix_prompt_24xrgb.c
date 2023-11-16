/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_prompt_24xrgb", /* Test name */
    51, 26, 369, 192  /* Define the coordinates of the capture area.
                      In this test, we only need to capture the prompt
                      drawing area.  */
};

static VOID      control_thread_entry(ULONG);
int main(int argc, char ** argv)
{
    /* Parse the command prompt argument. */
    gx_validation_setup(argc, argv);

    /* Start ThreadX system */
    tx_kernel_enter(); 
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{

    /* Create a dedicated thread to perform various operations
    on the prompt drawing example. These operations simulate 
    user input. */
    gx_validation_control_thread_create(control_thread_entry);

    /* Termiante the test if it runs for more than 100 ticks */
    /* This function is not implemented yet. */
    gx_validation_watchdog_create(100);

    /* Call the actual prompt routine. */
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

static int prompt_value[] = {ID_PROMPT_1, ID_PROMPT_3, ID_PROMPT_2, ID_PROMPT_4};

static char* text_value[] = {"text_1", "text_2"};

static int text_color[] = {GX_COLOR_ID_TEXT, GX_COLOR_ID_BLUE};

char test_comment[256];


static VOID control_thread_entry(ULONG input)
{
int prompt_index;
int text_index;
int text_color_index;
int frame_id = 1;
GX_PROMPT *prompt;

    ToggleScreen(pTextScreen, pButtonScreen);

    for(prompt_index = 0; prompt_index < (INT)(sizeof(prompt_value) / sizeof(int)); prompt_index++)
    {
        gx_widget_find(pTextScreen, prompt_value[prompt_index], 0, &prompt);

        if(prompt_value[prompt_index] == ID_PROMPT_1)
        {
            gx_widget_style_add((GX_WIDGET *)prompt, GX_STYLE_TEXT_COPY);
            gx_prompt_text_set(prompt, text_value[0]);
        }

        gx_prompt_font_set(prompt, GX_FONT_ID_PROMPT);
        gx_prompt_text_id_set(prompt, GX_STRING_ID_STRING_5);

        for(text_color_index = 0; text_color_index < (INT)(sizeof(text_color) / sizeof(int)); text_color_index++)
        {
            gx_prompt_text_color_set(prompt, text_color[text_color_index], text_color[text_color_index], text_color[text_color_index]);
            for(text_index = 0; text_index < 2; text_index++)
            {
                gx_prompt_text_set(prompt, text_value[text_index]);

                sprintf(test_comment, "%s ", text_value[text_index]);

                if(prompt_value[prompt_index] == ID_PROMPT_1)
                {
                    strcat(test_comment, "PROMPT_1 ");
                }
                else if(prompt_value[prompt_index] == ID_PROMPT_2)
                {
                    strcat(test_comment, "PROMPT_2 ");
                }
                else if(prompt_value[prompt_index] == ID_PROMPT_3)
                {
                    strcat(test_comment, "PROMPT_3 ");
                }
                else if(prompt_value[prompt_index] == ID_PROMPT_4)
                {
                    strcat(test_comment, "PROMPT_4 ");
                    if(text_index)
                    {
                        gx_widget_style_add((GX_WIDGET *)prompt, GX_STYLE_DRAW_SELECTED);
                        strcat(test_comment, "Added GX_STYLE_DRAW_SELECTED style ");
                        gx_pixelmap_prompt_pixelmap_set((GX_PIXELMAP_PROMPT *)prompt, GX_PIXELMAP_ID_TFIELD_LEFT_SMALL,
                            GX_PIXELMAP_ID_TFIELD_FILL_SMALL, GX_PIXELMAP_ID_TFIELD_RIGHT_SMALL,
                            GX_PIXELMAP_ID_I_ORANGEFILL_LEFT, GX_PIXELMAP_ID_FILL, GX_PIXELMAP_ID_I_EMPTYFILL_RIGHT);
                    }
                    else
                    {
                        gx_widget_style_remove((GX_WIDGET *)prompt, GX_STYLE_DRAW_SELECTED);
                        strcat(test_comment, "Removed GX_STYLE_DRAW_SELECTED style ");
                    }
                }
                if(text_color[text_color_index] == GX_COLOR_ID_TEXT)
                    strcat(test_comment, "GX_COLOR_ID_TEXT ");
                else
                    strcat(test_comment, "GX_COLOR_ID_BLUE ");

                gx_validation_set_frame_id(frame_id);
                gx_validation_set_frame_comment(test_comment);

                /* Force a screen refresh. */
                gx_validation_screen_refresh();

                /* Increment frame ID */
                frame_id ++;
            }
        }
    }

    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





