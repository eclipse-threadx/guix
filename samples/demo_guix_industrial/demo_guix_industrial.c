/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_industrial.h"

/* Define this to improve animation performance when running on target board with two layers enabled. */
/* #define USE_CANVAS_FOR_ANIMATION */

#define MEMORY_POOL_BUFFER_SIZE (MAIN_DISPLAY_X_RESOLUTION * MAIN_DISPLAY_Y_RESOLUTION * 8)

/* Define memory for memory pool. */
GX_UBYTE        memory_pool_buffer[MEMORY_POOL_BUFFER_SIZE];
TX_BYTE_POOL    memory_pool;
GX_WINDOW_ROOT *root;

USHORT toggle_animation_flag = ANIMATION_NONE;
GX_WINDOW *current_screen = (GX_WINDOW *)&main_screen;

#ifdef USE_CANVAS_FOR_ANIMATION
#define ANIMATION_WINDOW_WIDTH  640
#define ANIMATION_WINDOW_HEIGHT 327
#define ANIMATION_CANVAS_MEMORY_SIZE (ANIMATION_WINDOW_WIDTH * ANIMATION_WINDOW_HEIGHT * 4)

GX_UBYTE animation_canvas_memory[ANIMATION_CANVAS_MEMORY_SIZE];
GX_CANVAS animation_canvas;
GX_WINDOW_ROOT animation_root;
#endif

/* Define prototypes.   */
VOID  guix_setup(void);
extern UINT win32_graphics_driver_setup_24xrgb(GX_DISPLAY *display);
UINT root_win_event_process(GX_WINDOW *root, GX_EVENT *event_ptr);

/* Define GX_PIXELMAP type variable to receive pixelmap data decoded from background jpeg. */
GX_PIXELMAP main_screen_bg;

/******************************************************************************************/
/* Application entry.                                                                     */
/******************************************************************************************/
int main(int argc, char ** argv)
{
    tx_kernel_enter();
    return(0);
}

/******************************************************************************************/
/* Define memory allocator function.                                                      */
/******************************************************************************************/
VOID *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }
    return NULL;
}

/******************************************************************************************/
/* Define memory de-allocator function.                                                   */
/******************************************************************************************/
VOID memory_free(VOID *mem)
{
    tx_byte_release(mem);
}

/******************************************************************************************/
/* Define tx_application_define function.                                                 */
/******************************************************************************************/
VOID tx_application_define(void *first_unused_memory)
{
    guix_setup();
}

/******************************************************************************************/
/* Initiate and run GUIX.                                                                 */
/******************************************************************************************/
VOID  guix_setup()
{
#ifdef USE_CANVAS_FOR_ANIMATION
    GX_RECTANGLE size;
#endif

    /* Create byte pool. */
    tx_byte_pool_create(&memory_pool, "memory_pool_buffer", memory_pool_buffer, MEMORY_POOL_BUFFER_SIZE);

    /* Initialize GUIX.  */
    gx_system_initialize();

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    gx_studio_display_configure(MAIN_DISPLAY, win32_graphics_driver_setup_24xrgb,
        LANGUAGE_ENGLISH, MAIN_DISPLAY_THEME_1, &root);

    gx_widget_event_process_set(root, root_win_event_process);

    /* Create the main screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *)root, GX_NULL);

    /* Create sequence window. */
    gx_studio_named_widget_create("sequence_window", GX_NULL, GX_NULL);

    /* Create sequence number window. */
    gx_studio_named_widget_create("sequence_number", GX_NULL, GX_NULL);

    /* Create complete window. */
    gx_studio_named_widget_create("complete_window", GX_NULL, GX_NULL);

#ifdef USE_CANVAS_FOR_ANIMATION
    /* Define animation root window size.  */
    gx_utility_rectangle_define(&size, 0, 0, ANIMATION_WINDOW_WIDTH - 1, ANIMATION_WINDOW_HEIGHT - 1);

    /* create the root window used for window blend animation */
    gx_window_root_create(&animation_root, "animation root", &animation_canvas, GX_STYLE_ENABLED, GX_NULL, &size);

    /* Create new canvas for sequence screen fade in/out animation. */
    gx_canvas_create(&animation_canvas, GX_NULL,
                     root->gx_window_root_canvas->gx_canvas_display,
                     GX_CANVAS_SIMPLE,
                     ANIMATION_WINDOW_WIDTH, ANIMATION_WINDOW_HEIGHT,
                     (GX_COLOR *)animation_canvas_memory, ANIMATION_CANVAS_MEMORY_SIZE);
    gx_canvas_hardware_layer_bind(&animation_canvas, 2);
#endif
    
    /* Initialize "main_screen_by" varaible. */
    memset(&main_screen_bg, 0, sizeof(GX_PIXELMAP));

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* Let GUIX run */
    gx_system_start();
}

/******************************************************************************************/
/* Decode "main_screen" background image.                                                 */
/******************************************************************************************/
static VOID decode_main_screen_jpeg()
{
    GX_IMAGE_READER reader;
    GX_PIXELMAP *map;

    /* get a pointer to the raw jpeg data */
    gx_context_pixelmap_get(GX_PIXELMAP_ID_BG_METAL_DARK, &map);

    /* create an image reader object */
    gx_image_reader_create(&reader, map->gx_pixelmap_data, map->gx_pixelmap_data_size, GX_COLOR_FORMAT_24XRGB, 0);

    /* decode and color space convert the jpeg to produce a GUIX compatible pixelmap image */
    gx_image_reader_start(&reader, &main_screen_bg);
}

/******************************************************************************************/
/* Override the default draw function of "main_screen".                                   */
/******************************************************************************************/
VOID main_screen_draw(GX_WINDOW *window)
{
    gx_window_background_draw(window);

    /* If this is the first time drawing, then we need to decompress the raw jpeg
       image we use to paint the screen background.  */
    if (!main_screen_bg.gx_pixelmap_data)
    {
        decode_main_screen_jpeg();
    }

    /* Unless something went wrong, the pixelmap data should now be populated in 
       GUIX pixelmap format.  */
    if (main_screen_bg.gx_pixelmap_data)
    {
        /* Draw background map. */
        gx_canvas_pixelmap_draw(window->gx_widget_size.gx_rectangle_left,
            window->gx_widget_size.gx_rectangle_top, &main_screen_bg);
    }

    gx_widget_children_draw(window);
}

/******************************************************************************************/
/* Override default event processing of root window to handle screen animation            */ 
/* complete events.                                                                       */
/******************************************************************************************/
UINT root_win_event_process(GX_WINDOW *root, GX_EVENT *event_ptr)
{
    switch(event_ptr->gx_event_type)
    {
    case GX_EVENT_ANIMATION_COMPLETE:
        switch (event_ptr->gx_event_sender)
        {
        case ANI_ID_SEQ_WIN_FADE_IN:
            current_screen = (GX_WINDOW *)&sequence_window;
            sequence_animation_start();
            toggle_animation_flag = ANIMATION_NONE;
            break;

        case ANI_ID_START_BTN_SLIDE_IN:
            current_screen = (GX_WINDOW *)&main_screen;
            toggle_animation_flag = ANIMATION_NONE;
            break;

        case ANI_ID_COMPLETE_WIN_FADE_IN:
            current_screen = (GX_WINDOW *)&complete_window;
            toggle_animation_flag = ANIMATION_NONE;
            complete_window_timer_start();
            break;
        }
        break;

    default:
        gx_window_event_process(root, event_ptr);
        break;
    }
    return GX_SUCCESS;
}

/******************************************************************************************/
/* Override default event processing of "main_screen" to handle signals from my child     */
/* widgets.                                                                               */
/******************************************************************************************/
UINT main_screen_event_process(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_START, GX_EVENT_CLICKED):

        /* Toggle to sequence screen.  */
        screen_toggle_animation_start((GX_WINDOW *)&sequence_window, current_screen);
        break;

    case GX_SIGNAL(ID_ON_OFF, GX_EVENT_CLICKED):
        if (main_screen.main_screen_prompt_on_off.gx_prompt_text_id == GX_STRING_ID_TURN_ON)
        {
            /* Toggle to sequence screen.  */
            screen_toggle_animation_start((GX_WINDOW *)&sequence_window, current_screen);
        }
        else
        {
            /* Toggle to main screen.  */
            screen_toggle_animation_start((GX_WINDOW *)&main_screen, current_screen);
        }
        break;

    case GX_SIGNAL(ID_INSPECTING, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_ASSEMBLING, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_WELDING, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_PALLETIZING, GX_EVENT_CLICKED):
        if (current_screen->gx_widget_id == ID_SEQUENCE_WINDOW)
        {
            /* Pass event to sequence window.  */
            sequence_window.gx_widget_event_process_function((GX_WIDGET *)window, event_ptr);
        }
        else
        {
            GX_WIDGET *find;
            gx_widget_find(&main_screen, event_ptr->gx_event_sender, GX_SEARCH_DEPTH_INFINITE, &find);
            if (find)
            {
                button_indicator_attach((GX_PIXELMAP_BUTTON*)find);
            }
        }
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/******************************************************************************************/
/* Attach the indicator icon to the specified button.                                     */
/******************************************************************************************/
VOID button_indicator_attach(GX_PIXELMAP_BUTTON *button)
{
    GX_WIDGET *indicator = (GX_WIDGET *)&main_screen.main_screen_button_indicator_dot;
    INT x_shift;
    INT y_shift;

    if ((GX_WIDGET *)button != indicator->gx_widget_parent)
    {
        gx_widget_attach((GX_WIDGET *)button, indicator);
    }

    x_shift = button->gx_widget_size.gx_rectangle_right - 18 - indicator->gx_widget_size.gx_rectangle_right;
    y_shift = button->gx_widget_size.gx_rectangle_top + 16 - indicator->gx_widget_size.gx_rectangle_top;
    gx_widget_shift(indicator, x_shift, y_shift, GX_TRUE);
}

/******************************************************************************************/
/* Start an animation to toggle between screens.                                          */
/******************************************************************************************/
VOID screen_toggle_animation_start(GX_WINDOW *show, GX_WINDOW *hide)
{
    GX_EVENT  my_event;
#ifdef USE_CANVAS_FOR_ANIMATION
    GX_ANIMATION *animation;
    GX_ANIMATION_INFO info;
#endif

    if (toggle_animation_flag != ANIMATION_NONE)
    {
        return;
    }

    toggle_animation_flag = ANIMATION_ACTIVE;

    memset(&my_event, 0, sizeof(GX_EVENT));
    my_event.gx_event_target = (GX_WIDGET *)show;

    switch (show->gx_widget_id)
    {
    case ID_MAIN_SCREEN:

        /* Set button status to "Turn On". */
        gx_prompt_text_id_set(&main_screen.main_screen_prompt_on_off, GX_STRING_ID_TURN_ON);

        /* Detach sequence number prompt. */
        gx_widget_detach(&sequence_number);

        /* Fade in start window. */
        my_event.gx_event_type = USER_EVENT_START_WIN_FADE_IN;
        gx_system_event_send(&my_event);
        break;

    case ID_SEQUENCE_WINDOW:

        /* Attach sequence number prompt.  */
        gx_widget_attach((GX_WIDGET*)&main_screen, &sequence_number);

        /* Initialize sequence window.  */
        sequence_window_init();

        /* Fade in sequence window. */
#ifdef USE_CANVAS_FOR_ANIMATION
        gx_system_animation_get(&animation);
        gx_animation_canvas_define(animation, &animation_canvas);
        memset(&info, 0, sizeof(GX_ANIMATION_INFO));
        info.gx_animation_end_alpha = 255;
        info.gx_animation_start_alpha = 0;
        info.gx_animation_frame_interval = 1;
        info.gx_animation_steps = 20;
        info.gx_animation_parent = (GX_WIDGET*)&main_screen;
        info.gx_animation_target = (GX_WIDGET*)&sequence_window;
        info.gx_animation_start_position.gx_point_x = 0;
        info.gx_animation_start_position.gx_point_y = 70;
        info.gx_animation_end_position.gx_point_x = info.gx_animation_start_position.gx_point_x;
        info.gx_animation_end_position.gx_point_y = info.gx_animation_start_position.gx_point_y;
        info.gx_animation_id = ANI_ID_SEQ_WIN_FADE_IN;

        gx_animation_start(animation, &info);
#else
        my_event.gx_event_type = USER_EVENT_SEQ_WIN_FADE_IN;
        gx_system_event_send(&my_event);
#endif
        break;

    case ID_COMPLETE_WINDOW:

        /* Detach button window. */
        gx_widget_detach((GX_WIDGET*)&main_screen.main_screen_button_window);

        /* Initialize complete window.  */
        complete_window_init();

        /* Fade in complete window. */
        my_event.gx_event_type = USER_EVENT_COMPLETE_WIN_FADE_IN;
        gx_system_event_send(&my_event);
        break;

    default:
        toggle_animation_flag = ANIMATION_NONE;
        return;
    }

    my_event.gx_event_target = (GX_WIDGET*)hide;

    switch(hide->gx_widget_id)
    {
    case ID_MAIN_SCREEN:

        /* Set button status to "Shut Off". */
        gx_prompt_text_id_set(&main_screen.main_screen_prompt_on_off, GX_STRING_ID_TURN_OFF);

        /* Fade out main screen. */
        my_event.gx_event_type = USER_EVENT_START_WIN_FADE_OUT;
        gx_system_event_send(&my_event);
        break;

    case ID_SEQUENCE_WINDOW:

        /* Stop animations in sequence window. */
        sequence_animation_stop();

        /* Fade out sequence screen. */
#ifdef USE_CANVAS_FOR_ANIMATION
        gx_system_animation_get(&animation);
        gx_animation_canvas_define(animation, &animation_canvas);
        memset(&info, 0, sizeof(GX_ANIMATION_INFO));
        info.gx_animation_end_alpha = 0;
        info.gx_animation_start_alpha = 255;
        info.gx_animation_frame_interval = 1;
        info.gx_animation_steps = 20;
        info.gx_animation_parent = (GX_WIDGET *)&main_screen;
        info.gx_animation_target = (GX_WIDGET*)&sequence_window;
        info.gx_animation_start_position.gx_point_x = 0;
        info.gx_animation_start_position.gx_point_y = 70;
        info.gx_animation_end_position.gx_point_x = info.gx_animation_start_position.gx_point_x;
        info.gx_animation_end_position.gx_point_y = info.gx_animation_start_position.gx_point_y;
        info.gx_animation_id = ANI_ID_SEQ_WIN_FADE_OUT;
        info.gx_animation_style = GX_ANIMATION_DETACH;

        gx_animation_start(animation, &info);
#else
        my_event.gx_event_type = USER_EVENT_SEQ_WIN_FADE_OUT;
        gx_system_event_send(&my_event);
#endif
        break;

    case ID_COMPLETE_WINDOW:

        /* Attach button window. */
        gx_widget_attach((GX_WIDGET *)&main_screen, (GX_WIDGET *)&main_screen.main_screen_button_window);

        /* Stop complete window timer.  */
        complete_window_timer_stop();

        /* Fade out complete screen. */
        my_event.gx_event_type = USER_EVENT_COMPLETE_WIN_FADE_OUT;
        gx_system_event_send(&my_event);
        break;

    default:
        return;
    }
}

/******************************************************************************************/
/* Define custom mode button draw function.                                               */
/******************************************************************************************/
VOID mode_button_draw(GX_PIXELMAP_BUTTON *button)
{
    if (button->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        gx_widget_style_add(button->gx_widget_first_child, GX_STYLE_DRAW_SELECTED);
    }
    else
    {
        gx_widget_style_remove(button->gx_widget_first_child, GX_STYLE_DRAW_SELECTED);
    }

    gx_pixelmap_button_draw(button);
}

/******************************************************************************************/
/* Calculate string length.                                                               */
/******************************************************************************************/
UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length)
{
    UINT length = 0;

    if (input_string)
    {
        /* Traverse the string.  */
        for (length = 0; input_string[length]; length++)
        {
            /* Check if the string length is bigger than the max string length.  */
            if (length >= max_string_length)
            {
                break;
            }
        }
    }

    return length;
}
