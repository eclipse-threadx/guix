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
/**   Progress Bar Management (Radial Progress Bar)                       */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_canvas.h"
#include "gx_utility.h"
#include "gx_radial_progress_bar.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_progress_bar_background_draw             PORTABLE C      */
/*                                                           6.1.8        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service draws a radial progress bar background.                */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    progress_bar                          Radial Progress Bar control   */
/*                                            block                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_background_draw                                          */
/*    _gx_context_brush_get                                               */
/*    _gx_context_line_color_set                                          */
/*    _gx_context_brush_width_set                                         */
/*    _gx_canvas_circle_draw                                              */
/*    _gx_canvas_arc_draw                                                 */
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
/*  08-02-2021     Ting Zhu                 Modified comment(s),          */
/*                                            supported no background     */
/*                                            radial track mode,          */
/*                                            resulting in version 6.1.8  */
/*                                                                        */
/**************************************************************************/
VOID  _gx_radial_progress_bar_background_draw(GX_RADIAL_PROGRESS_BAR *progress_bar)
{
GX_RADIAL_PROGRESS_BAR_INFO *info;
GX_VALUE                     xcenter;
GX_VALUE                     ycenter;
GX_BRUSH                    *brush;
GX_RECTANGLE                *size;

#if defined(GX_BRUSH_ALPHA_SUPPORT)
GX_UBYTE     brush_alpha;
GX_RECTANGLE dirty;
GX_PIXELMAP  map;
VOID        *canvas_memory;
INT          canvas_size;
GX_DISPLAY  *display;
GX_CANVAS   *canvas;
GX_VALUE     old_xpos = 0;
GX_VALUE     old_ypos = 0;
#endif

    _gx_context_brush_get(&brush);

#if defined(GX_BRUSH_ALPHA_SUPPORT)
    brush_alpha = brush -> gx_brush_alpha;
    if (brush_alpha == 0)
    {
        return;
    }

    if (brush_alpha != GX_ALPHA_VALUE_OPAQUE)
    {
        size = &progress_bar -> gx_widget_size;
        map.gx_pixelmap_width = (GX_VALUE)(size -> gx_rectangle_right - size -> gx_rectangle_left + 1);
        map.gx_pixelmap_height = (GX_VALUE)(size -> gx_rectangle_bottom - size -> gx_rectangle_top + 1);

        display = _gx_system_current_draw_context -> gx_draw_context_display;

        if ((progress_bar -> gx_radial_progress_bar_canvas.gx_canvas_id != GX_CANVAS_ID) &&
            _gx_system_memory_allocator)
        {

            /* Pixelmap width and height is GX_VALUE type, overflow cannot occur. */
            canvas_size = display -> gx_display_driver_row_pitch_get((USHORT)map.gx_pixelmap_width);
            canvas_size *= map.gx_pixelmap_height;

            canvas_memory = _gx_system_memory_allocator((ULONG)canvas_size);

            if (canvas_memory)
            {
                _gx_canvas_create(&progress_bar -> gx_radial_progress_bar_canvas, "progress_bar_canvas",
                                  display, GX_CANVAS_SIMPLE, (UINT)map.gx_pixelmap_width,
                                  (UINT)map.gx_pixelmap_height, (GX_COLOR *)canvas_memory, (ULONG)canvas_size);
            }
        }

        if (progress_bar -> gx_radial_progress_bar_canvas.gx_canvas_id == GX_CANVAS_ID)
        {
            /* Remove resize notify temporary. */
            progress_bar -> gx_widget_status &= ~GX_STATUS_RESIZE_NOTIFY;

            old_xpos = size -> gx_rectangle_left;
            old_ypos = size -> gx_rectangle_top;

            if (old_xpos || old_ypos)
            {
                _gx_widget_shift((GX_WIDGET *)progress_bar, (GX_VALUE)(-old_xpos), (GX_VALUE)(-old_ypos), GX_FALSE);
            }

            canvas = _gx_system_current_draw_context -> gx_draw_context_canvas;
            dirty = canvas -> gx_canvas_dirty_area;
            _gx_utility_rectangle_shift(&dirty, (GX_VALUE)(-old_xpos), (GX_VALUE)(-old_ypos));
            _gx_utility_rectangle_overlap_detect(&dirty, &progress_bar -> gx_widget_size, &dirty);

            _gx_canvas_drawing_initiate(&progress_bar -> gx_radial_progress_bar_canvas, GX_NULL, &dirty);

            if (progress_bar -> gx_widget_status & GX_STATUS_TRANSPARENT)
            {
                xcenter = canvas -> gx_canvas_display_offset_x;
                ycenter = canvas -> gx_canvas_display_offset_y;
                _gx_canvas_offset_set(canvas, (GX_VALUE)(-old_xpos), (GX_VALUE)(-old_ypos));
                display -> gx_display_driver_canvas_copy(canvas, &progress_bar -> gx_radial_progress_bar_canvas);
                _gx_canvas_offset_set(canvas, xcenter, ycenter);
            }

            _gx_context_brush_get(&brush);
        }
    }

#endif

    /* Draw progress bar background.  */
    _gx_widget_background_draw((GX_WIDGET *)progress_bar);

    /* Draw circular track.  */
    info = &progress_bar -> gx_radial_progress_bar_info;

    size = &progress_bar -> gx_widget_size;

    xcenter = (GX_VALUE)(size -> gx_rectangle_right + size -> gx_rectangle_left);
    xcenter >>= 1;

    ycenter = (GX_VALUE)(size -> gx_rectangle_bottom + size -> gx_rectangle_top);
    ycenter >>= 1;

    brush -> gx_brush_style &= (ULONG)(~GX_BRUSH_SOLID_FILL);

    if (progress_bar -> gx_widget_style & GX_STYLE_RADIAL_PROGRESS_ALIAS)
    {
        brush -> gx_brush_style |= GX_BRUSH_ALIAS;
    }

    if (progress_bar -> gx_widget_style & GX_STYLE_RADIAL_PROGRESS_ROUND)
    {
        brush -> gx_brush_style |= GX_BRUSH_ROUND;
    }

    if (!(progress_bar -> gx_widget_style & GX_STYLE_RADIAL_PROGRESS_NO_BACKTRACK))
    {
        _gx_context_line_color_set(info -> gx_radial_progress_bar_info_normal_brush_color);
        _gx_context_brush_width_set((UINT)(info -> gx_radial_progress_bar_info_normal_brush_width));

        _gx_canvas_circle_draw(xcenter, ycenter, (UINT)(info -> gx_radial_progress_bar_info_radius));
    }

    if (info -> gx_radial_progress_bar_info_current_val != 0)
    {
        _gx_context_line_color_set(info -> gx_radial_progress_bar_info_selected_brush_color);
        brush -> gx_brush_width = info -> gx_radial_progress_bar_info_selected_brush_width;

        if (info -> gx_radial_progress_bar_info_current_val > 0)
        {
            _gx_canvas_arc_draw(xcenter, ycenter,
                                (UINT)(info -> gx_radial_progress_bar_info_radius),
                                info -> gx_radial_progress_bar_info_anchor_val,
                                info -> gx_radial_progress_bar_info_current_val + info -> gx_radial_progress_bar_info_anchor_val);
        }
        else
        {
            _gx_canvas_arc_draw(xcenter, ycenter,
                                (UINT)(info -> gx_radial_progress_bar_info_radius),
                                info -> gx_radial_progress_bar_info_current_val + info -> gx_radial_progress_bar_info_anchor_val,
                                info -> gx_radial_progress_bar_info_anchor_val);
        }
    }

#if defined(GX_BRUSH_ALPHA_SUPPORT)
    if ((brush_alpha != GX_ALPHA_VALUE_OPAQUE) &&
        (progress_bar -> gx_radial_progress_bar_canvas.gx_canvas_id == GX_CANVAS_ID))
    {
        if (old_xpos || old_ypos)
        {
            _gx_widget_shift((GX_WIDGET *)progress_bar, old_xpos, old_ypos, GX_FALSE);
        }

        _gx_canvas_pixelmap_get(&map);
        _gx_canvas_drawing_complete(&progress_bar -> gx_radial_progress_bar_canvas, GX_FALSE);
        _gx_canvas_pixelmap_blend(old_xpos, old_ypos, &map, brush_alpha);

        /* Added resize notification back. */
        progress_bar -> gx_widget_status |= GX_STATUS_RESIZE_NOTIFY;
    }
#endif
}

