/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "tx_api.h"
#include "gx_api.h"
#include "gx_validation_utility.h"

TEST_PARAM test_parameter = {
    "guix_lines_565rgb", /* Test name */
    20, 65, 350, 400  /* Define the coordinates of the capture area.
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

#ifdef win32_graphics_driver_setup_565rgb  
#undef win32_graphics_driver_setup_565rgb  
#endif
/* Replace the default graphics driver with the validation driver. */
#define win32_graphics_driver_setup_565rgb  gx_validation_graphics_driver_setup_565rgb


#ifdef WIN32
#undef WIN32
#endif

#include "gx_validation_wrapper.h"

#include "demo_guix_all_widgets_16bpp.c"

extern int lines_screen_line_angle;
extern GX_UBYTE lines_screen_brush_alpha;
extern int lines_screen_line_length;
extern GX_BOOL        lines_screen_anti_aliased ;
extern GX_BOOL        lines_screen_rounded ;
extern GX_RESOURCE_ID lines_screen_line_color;
extern GX_UBYTE       lines_screen_brush_alpha ;
extern int            line_x1;
extern int            line_x2;
extern int            line_y1;
extern int            line_y2;
extern GX_WINDOW     *pLineWin;
extern int            lines_screen_line_width;

char test_comment[256];

#define LINE_CENTER_X_OFFSET 159
#define LINE_CENTER_Y_OFFSET 159
#define LINE_LENGTH 100

static VOID test_calculate_line_ends(VOID)
{
    GX_FIXED_VAL scaled_angle;
    INT x_dist;
    INT y_dist;

    INT x_center = pLineWin->gx_widget_size.gx_rectangle_left + LINE_CENTER_X_OFFSET;
    INT y_center = pLineWin->gx_widget_size.gx_rectangle_top + LINE_CENTER_Y_OFFSET;

    scaled_angle = GX_FIXED_VAL_MAKE(lines_screen_line_angle);
    x_dist = gx_utility_math_cos(scaled_angle) * LINE_LENGTH;
    y_dist = gx_utility_math_sin(scaled_angle) * LINE_LENGTH;
    x_dist = GX_FIXED_VAL_TO_INT(x_dist);
    y_dist = GX_FIXED_VAL_TO_INT(y_dist);

    line_x1 = x_center - x_dist;
    line_x2 = x_center + x_dist;
    line_y1 = y_center - y_dist;
    line_y2 = y_center + y_dist;
}

static VOID test_line_win_draw(GX_WINDOW *window)
{
ULONG brush_style = 0;
GX_BRUSH *brush;
GX_RECTANGLE *size;

    gx_window_draw((GX_WINDOW*) window);

    gx_context_brush_get(&brush);
    brush -> gx_brush_alpha = lines_screen_brush_alpha;

    if (lines_screen_anti_aliased)
    {
        brush_style |= GX_BRUSH_ALIAS;
    }
    if (lines_screen_rounded)
    {
        brush_style |= GX_BRUSH_ROUND;
    }
    gx_context_brush_define(lines_screen_line_color, window ->gx_widget_normal_fill_color, brush_style);
    gx_context_brush_width_set(lines_screen_line_width);

    /* Draw pattern lines. */
    size = &window->gx_widget_size;

    gx_context_brush_pattern_set(0xf8fcfeff);
    gx_canvas_line_draw(size->gx_rectangle_left + 5, size->gx_rectangle_top + 5, size->gx_rectangle_left + 5, size->gx_rectangle_bottom - 5);
    gx_context_brush_pattern_set(0xf0f0f0f0);
    gx_canvas_line_draw(size->gx_rectangle_left + 5, size->gx_rectangle_top + 5, size->gx_rectangle_right - 5, size->gx_rectangle_top + 5);
    gx_context_brush_pattern_set(0xaaaaaaaa);
    gx_canvas_line_draw(size->gx_rectangle_left + 5, size->gx_rectangle_bottom - 5, size->gx_rectangle_right - 5, size->gx_rectangle_bottom - 5);
    gx_context_brush_pattern_set(0x33333333);
    gx_canvas_line_draw(size->gx_rectangle_right - 5, size->gx_rectangle_top + 5, size->gx_rectangle_right - 5, size->gx_rectangle_bottom - 5);

    gx_context_brush_pattern_set(0);
    test_calculate_line_ends();
    gx_canvas_line_draw(line_x1, line_y1, line_x2, line_y2);
}

static GX_RECTANGLE widget_size[] = {
    {25, 71, 344, 390},
    {25, 71, 190, 336},
    {25, 71, 170, 316},
    {175, 221, 344, 390},
    {195, 241, 344, 390}
};

/* This thread simulates user input.  Its priority is lower
   than the GUIX thread, so that GUIX finishes an operation 
   before this thread is able to issue the next command. */
static VOID control_thread_entry(ULONG input)
{
int frame_id = 1;
int index;

    gx_widget_draw_set((GX_WIDGET *)pLineWin, (void(*)(GX_WIDGET *))test_line_win_draw);
    ToggleScreen((GX_WINDOW *)&lines_screen, (GX_WINDOW *)&button_screen);

    for(index = 0; index < (int)(sizeof(widget_size)/sizeof(GX_RECTANGLE)); index++)
    {
        gx_widget_resize((GX_WIDGET *)pLineWin, &widget_size[index]);
        for(lines_screen_brush_alpha = 255; lines_screen_brush_alpha > 50; lines_screen_brush_alpha -= 100)
        {
            for(lines_screen_line_length = 100; lines_screen_line_length < 300; lines_screen_line_length += 100)
            {
                for(lines_screen_line_width = 0; lines_screen_line_width < 3; lines_screen_line_width += 1)
                {
                    for(lines_screen_line_angle = 0; lines_screen_line_angle < 360; lines_screen_line_angle+=30)
                    {
                        /* Inform the validation system 
                           (1) Frame ID, which identifies a specific test configuration;
                           (2) Start recording frame on the next toggle operation.
                        */
                        gx_validation_set_frame_id(frame_id);

                        sprintf(test_comment, "brush alpha = %d, line length = %d, angle = %d, line width: %d, window size: (%d, %d, %d, %d) ", 
                                               lines_screen_brush_alpha, lines_screen_line_length, lines_screen_line_angle, lines_screen_line_width,
                                               widget_size[index].gx_rectangle_left,widget_size[index].gx_rectangle_top, widget_size[index].gx_rectangle_right, widget_size[index].gx_rectangle_bottom);

                        gx_validation_set_frame_comment(test_comment);

                        /* Mark the window "dirty" */
                        gx_system_dirty_mark(pLineWin);

                        /* Force a screen refresh. */
                        gx_validation_screen_refresh();

                        /* Increment frame ID */
                        frame_id ++;
                    }
                }
            }
        
        }
    }    
    /* Signal the end of the test case.  Verify the output. */
    gx_validation_end();

    exit(0);
}





