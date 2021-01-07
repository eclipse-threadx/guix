/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_smart_watch.h"

#define GAME_LIST_VISIBLE_ROWS 2

GX_RESOURCE_ID game_ids[] = {
    GX_PIXELMAP_ID_GAME_ONE,
    GX_PIXELMAP_ID_GAME_TWO,
    GX_PIXELMAP_ID_GAME_THREE,
    GX_PIXELMAP_ID_GAME_FOUR,
    GX_PIXELMAP_ID_GAME_ONE,
    GX_PIXELMAP_ID_GAME_TWO,
    GX_PIXELMAP_ID_GAME_THREE,
    GX_PIXELMAP_ID_GAME_FOUR
};

/* CITY list row structure. */
typedef struct GAME_LIST_ROW_STRUCT {
    GX_WIDGET          widget;
    GX_PIXELMAP_BUTTON game_1;
    GX_PIXELMAP_BUTTON game_2;
} GAME_LIST_ROW;

GAME_LIST_ROW game_list_row_memory[GAME_LIST_VISIBLE_ROWS + 1];

/*************************************************************************************/
VOID game_list_button_draw(GX_PIXELMAP_BUTTON *button)
{
    GX_PIXELMAP *map;
    gx_pixelmap_button_draw(button);

    if (button->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        gx_context_pixelmap_get(GX_PIXELMAP_ID_HALF_TRANSPARENT_COVER, &map);

        gx_canvas_pixelmap_draw(button->gx_widget_size.gx_rectangle_left, button->gx_widget_size.gx_rectangle_top, map);
    }
}

/*************************************************************************************/
VOID game_list_row_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
    GX_RECTANGLE childsize;
    GAME_LIST_ROW *row = (GAME_LIST_ROW *)widget;
    GX_BOOL result;

    gx_widget_created_test(&row->widget, &result);
    if (!result)
    {
        gx_utility_rectangle_define(&childsize, 0, 5, 256, 130);
        gx_widget_create(&row->widget, NULL, (GX_WIDGET *)list, GX_STYLE_TRANSPARENT, 0, &childsize);
        gx_utility_rectangle_define(&childsize, 10, 5, 132, 125);
        gx_pixelmap_button_create(&row->game_1, NULL, (GX_WIDGET *)&row->widget, GX_NULL, GX_NULL, GX_NULL,
            GX_STYLE_ENABLED, 0, &childsize);
        gx_widget_draw_set(&row->game_1, game_list_button_draw);
        gx_widget_fill_color_set(&row->game_1, GX_COLOR_ID_BLACK, GX_COLOR_ID_BLACK, GX_COLOR_ID_BLACK);

        gx_utility_rectangle_define(&childsize, 140, 5, 266, 125);
        gx_pixelmap_button_create(&row->game_2, NULL, (GX_WIDGET *)&row->widget, GX_NULL, GX_NULL, GX_NULL,
            GX_STYLE_ENABLED, 0, &childsize);
        gx_widget_draw_set(&row->game_2, game_list_button_draw);
        gx_widget_fill_color_set(&row->game_2, GX_COLOR_ID_BLACK, GX_COLOR_ID_BLACK, GX_COLOR_ID_BLACK);
    }

    gx_pixelmap_button_pixelmap_set(&row->game_1, game_ids[index << 1], game_ids[index << 1], GX_NULL);
    gx_pixelmap_button_pixelmap_set(&row->game_2, game_ids[(index << 1) + 1], game_ids[(index << 1) + 1], GX_NULL);
}

/*************************************************************************************/
VOID game_list_children_create(GX_VERTICAL_LIST *list)
{
    INT count;

    for (count = 0; count < GAME_LIST_VISIBLE_ROWS + 1; count++)
    {
        game_list_row_create(list, (GX_WIDGET *)&game_list_row_memory[count], count);
    }
}