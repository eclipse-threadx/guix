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
/**   Sprite Management (Sprite)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"
#include "gx_context.h"
#include "gx_canvas.h"
#include "gx_widget.h"
#include "gx_sprite.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_sprite_draw                                     PORTABLE C      */
/*                                                           6.1.9        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service draws a sprite widget.                                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    sprite                                Pointer to sprite widget      */
/*                                            control block               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_pixelmap_get              Get the pixelmap in the       */
/*                                            current context             */
/*    _gx_canvas_pixelmap_blend             Blend pixelmap                */
/*    _gx_canvas_pixelmap_draw              Draw pixelmap                 */
/*    _gx_widget_children_draw              Draw the children of a widget */
/*                                                                        */
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
/*  10-15-2021     Ting Zhu                 Modified comment(s),          */
/*                                            supported alphamap draw,    */
/*                                            resulting in version 6.1.9  */
/*                                                                        */
/**************************************************************************/
VOID  _gx_sprite_draw(GX_SPRITE *sprite)
{
GX_SPRITE_FRAME *frame;
GX_PIXELMAP     *map;
GX_VALUE         xpos;
GX_VALUE         ypos;

    /* Draw sprite background.  */
    _gx_widget_background_draw((GX_WIDGET *)sprite);

    if (sprite -> gx_sprite_frame_list)
    {
        if (sprite -> gx_sprite_current_frame < sprite -> gx_sprite_frame_count)
        {
            frame = &sprite -> gx_sprite_frame_list[sprite -> gx_sprite_current_frame];
            if (frame -> gx_sprite_frame_pixelmap)
            {
                _gx_widget_context_fill_set((GX_WIDGET *)sprite);

                _gx_context_pixelmap_get(frame -> gx_sprite_frame_pixelmap, &map);
                if (map)
                {
                    xpos = (GX_VALUE)(sprite -> gx_widget_size.gx_rectangle_left + frame -> gx_sprite_frame_x_offset);
                    ypos = (GX_VALUE)(sprite -> gx_widget_size.gx_rectangle_top + frame -> gx_sprite_frame_y_offset);

                    if (frame -> gx_sprite_frame_alpha != GX_ALPHA_VALUE_OPAQUE)
                    {
                        _gx_canvas_pixelmap_blend(xpos, ypos, map, frame -> gx_sprite_frame_alpha);
                    }
                    else
                    {
                        _gx_canvas_pixelmap_draw(xpos, ypos, map);
                    }
                }
            }
        }
    }

    /* Draw children widgets of prompt widget.  */
    _gx_widget_children_draw((GX_WIDGET *)sprite);
}

