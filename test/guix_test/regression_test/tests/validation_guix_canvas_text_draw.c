/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_canvas_text_draw", /* Test name */
    0, 0, 639, 479  /* Define the coordinates of the capture area.
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
int                      frame_id = 1;
extern GX_CONST GX_FONT *Primary_theme_1_font_table[];

GX_FONT test_font = 
{
    GX_FONT_FORMAT_8BPP,                     /* format                         */
    0,                                       /* line pre-space                 */
    0,                                       /* line post-space                */ 
    29,                                      /* font data height               */
    22,                                      /* font baseline offset           */
    0x20,                                    /* first glyph within data page   */
    0x7e,                                    /* last glyph within data page    */
    {0},                                       /* pointer to glyph data        */
    GX_NULL                                  /* next font page                 */
};
GX_FONT *test_font_table[1] = {&test_font};

static void button_screen_draw_test(GX_WINDOW * window)
{
int               xpos, ypos;
GX_BRUSH         *brush;
GX_CONST GX_CHAR *draw_string = "test_string"; 

    xpos = window->gx_widget_size.gx_rectangle_left + 10;
    ypos = window->gx_widget_size.gx_rectangle_top + 10;
    
    gx_window_background_draw(window);
    
    switch(frame_id)
    {
    case 1:
        /* Do nothing to make it return GX_INVALID_FONT. */ 
        break;
        
    case 2:
        /*Set font and set an invalid font format to make */
        gx_context_font_set(0);
        break;
        
    case 3:
        /*Set font and set an invalid font format to make */
        test_font.gx_font_glyphs.gx_font_normal_glyphs = Primary_theme_1_font_table[GX_FONT_ID_VERABD]->gx_font_glyphs.gx_font_normal_glyphs;
        gx_context_font_set(0);
        gx_context_brush_get(&brush);
        brush->gx_brush_font->gx_font_format = GX_FONT_FORMAT_2BPP;
        break;
        
    case 4:
        /*Set font and set an invalid font format to make */
        gx_context_font_set(GX_FONT_ID_SYSTEM);
        gx_context_brush_width_set(0);
        gx_context_brush_style_set(GX_BRUSH_UNDERLINE);
        break;
        
    case 5:
        ypos = window->gx_widget_size.gx_rectangle_bottom - 15;
        gx_context_font_set(GX_FONT_ID_SYSTEM);
        gx_context_brush_width_set(10);
        gx_context_brush_style_set(GX_BRUSH_UNDERLINE);
        break;

    case 6:
        gx_context_font_set(GX_FONT_ID_SYSTEM);
        break;

    case 7:
        gx_context_font_set(GX_FONT_ID_SYSTEM);
        gx_context_brush_style_set(GX_BRUSH_UNDERLINE);
        ypos = window->gx_widget_size.gx_rectangle_bottom - 4;
        break;

    case 8:
        gx_context_font_set(GX_FONT_ID_SYSTEM);
        gx_context_brush_style_set(GX_BRUSH_UNDERLINE);
        ypos = window->gx_widget_size.gx_rectangle_top - 15;
        break;

    case 9:
        gx_context_font_set(GX_FONT_ID_SYSTEM);
        gx_context_brush_style_set(GX_BRUSH_UNDERLINE);
        gx_context_string_get(GX_STRING_ID_MULTI_LEVEL_ACCORDION_DESCRITION, &draw_string);
        break;
    }
    
    gx_context_line_color_set(GX_COLOR_ID_ORANGE);
    gx_canvas_text_draw(xpos, ypos, draw_string, strlen(draw_string));
}

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
    gx_widget_draw_set(pButtonScreen, button_screen_draw_test);
    for(frame_id = 1; frame_id <= 9; frame_id++)
    {
        switch(frame_id)
        {
            case 1:
                sprintf(test_comment, "Draw text without set font");
                break;
            
            case 2:
                gx_display_font_table_set(root->gx_window_root_canvas->gx_canvas_display, test_font_table, 1);
                sprintf(test_comment, "Set invalid font and call function gx_canvas_text_draw");
                break;
            
            case 3:
                sprintf(test_comment, "Set invalid font format and call function gx_canvas_text_draw");
                break;
            
            case 4:
		gx_display_font_table_set(root->gx_window_root_canvas->gx_canvas_display, (GX_FONT **)Primary_theme_1_font_table, 8);
                sprintf(test_comment, "Set invalid font width and GX_BRUSH_UNDERLINE style, then call function gx_canvas_text_draw");
                break;
            
            case 5:
                sprintf(test_comment, "Set font width and GX_BRUSH_UNDERLINE style, then call function gx_canvas_text_draw");
                break;
            
            case 6:
                sprintf(test_comment, "Just call function gx_canvas_text_draw to draw text with system font.");
                break;

            case 7:
            case 8:
                sprintf(test_comment, "Draw with underline.");
                break;

            case 9:
                sprintf(test_comment, "Test gx_context_string_get");
                break;         
        }
        gx_validation_set_frame_id(frame_id);
        gx_validation_set_frame_comment(test_comment);
        gx_validation_screen_refresh();
        
        tx_semaphore_get(selected_semaphore, TX_WAIT_FOREVER);
    }
    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}
