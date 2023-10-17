/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "text_draw_resources.h"
#include "text_draw_specifications.h"

/* Define prototypes.   */
VOID  start_guix(VOID);
extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);

GX_WINDOW_ROOT *root;

int main(int argc, char ** argv)
{
    tx_kernel_enter();
    return(0);
}

VOID tx_application_define(void *first_unused_memory)
{
    start_guix();
}

VOID  start_guix(VOID)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_565rgb,
        LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* create the main screen */
    gx_studio_named_widget_create("main_screen", (GX_WIDGET *)root, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start GUIX thread */
    gx_system_start();
}

VOID prompt_left_aligned_draw(GX_PROMPT *prompt)
{
    GX_STRING string;

    gx_widget_background_draw(prompt);

    gx_context_string_get_ext(prompt->gx_prompt_text_id, &string);

    gx_context_font_set(prompt->gx_prompt_font_id);

    gx_context_line_color_set(prompt->gx_prompt_normal_text_color);

    gx_canvas_aligned_text_draw(&string, &prompt->gx_widget_size, GX_STYLE_TEXT_LEFT);
}

VOID prompt_center_aligned_draw(GX_PROMPT *prompt)
{
    GX_STRING string;

    gx_widget_background_draw(prompt);

    gx_context_string_get_ext(prompt->gx_prompt_text_id, &string);

    gx_context_font_set(prompt->gx_prompt_font_id);

    gx_context_line_color_set(prompt->gx_prompt_normal_text_color);

    gx_canvas_aligned_text_draw(&string, &prompt->gx_widget_size, GX_STYLE_TEXT_CENTER);
}

VOID prompt_right_aligned_draw(GX_PROMPT *prompt)
{
    GX_STRING string;

    gx_widget_background_draw(prompt);

    gx_context_string_get_ext(prompt->gx_prompt_text_id, &string);

    gx_context_font_set(prompt->gx_prompt_font_id);

    gx_context_line_color_set(prompt->gx_prompt_normal_text_color);

    gx_canvas_aligned_text_draw(&string, &prompt->gx_widget_size, GX_STYLE_TEXT_RIGHT);
}
