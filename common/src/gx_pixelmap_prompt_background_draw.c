/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Pixelmap Prompt Management (Prompt)                                 */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_canvas.h"
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_prompt.h"
#include "gx_pixelmap_prompt.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_pixelmap_prompt_background_draw                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws the background of a pixelmap prompt widget.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    prompt                                Pixelmap prompt control block */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_background_draw            Draw the widget's background  */
/*    _gx_context_pixelmap_get              Retrieve pixelmap image       */
/*    _gx_canvas_pixelmap_draw              Draw a pixelmap               */
/*    _gx_canvas_pixelmap_tile              Tile a pixelmap               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
VOID  _gx_pixelmap_prompt_background_draw(GX_PIXELMAP_PROMPT *prompt)
{
GX_WIDGET     *widget = (GX_WIDGET *)prompt;
GX_RECTANGLE   fill_rect = widget -> gx_widget_size;
GX_PIXELMAP   *map;
GX_RESOURCE_ID fill_id;
GX_RESOURCE_ID left_id;
GX_RESOURCE_ID right_id;


    fill_id = prompt -> gx_normal_fill_pixelmap_id;
    left_id = prompt -> gx_normal_left_pixelmap_id;
    right_id = prompt -> gx_normal_right_pixelmap_id;

    if (prompt -> gx_widget_style & GX_STYLE_DRAW_SELECTED)
    {
        if (prompt -> gx_selected_fill_pixelmap_id)
        {
            fill_id = prompt -> gx_selected_fill_pixelmap_id;
        }
        if (prompt -> gx_selected_left_pixelmap_id)
        {
            left_id = prompt -> gx_selected_left_pixelmap_id;
        }
        if (prompt -> gx_selected_right_pixelmap_id)
        {
            right_id = prompt -> gx_selected_right_pixelmap_id;
        }
    }


    /* draw the background */
    if (fill_id == 0 &&
        !(prompt -> gx_widget_style & GX_STYLE_TRANSPARENT))
    {
        _gx_widget_background_draw(widget);
    }

    /* draw the left-end pixelmap if one was provided */
    if (left_id)
    {
        _gx_context_pixelmap_get(left_id, &map);
        if (map)
        {
            _gx_widget_context_fill_set(widget);
            _gx_canvas_pixelmap_draw(fill_rect.gx_rectangle_left,
                                     fill_rect.gx_rectangle_top, map);
            fill_rect.gx_rectangle_left = (GX_VALUE)(fill_rect.gx_rectangle_left + map -> gx_pixelmap_width);
        }
    }

    /* draw the right end pixelmap, if one was provided */
    if (right_id)
    {
        _gx_context_pixelmap_get(right_id, &map);
        if (map)
        {
            _gx_widget_context_fill_set(widget);
            _gx_canvas_pixelmap_draw((GX_VALUE)(fill_rect.gx_rectangle_right - map -> gx_pixelmap_width + 1),
                                     fill_rect.gx_rectangle_top, map);
            fill_rect.gx_rectangle_right = (GX_VALUE)(fill_rect.gx_rectangle_right - map -> gx_pixelmap_width);
        }
    }

    /* now fill the remaining area with the fill pixelmap */
    if (fill_id)
    {
        _gx_context_pixelmap_get(fill_id, &map);
        if (map)
        {
            _gx_canvas_pixelmap_tile(&fill_rect, map);
        }
    }
}

