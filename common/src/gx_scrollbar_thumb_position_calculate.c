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
/**   Scrollbar Management (Scrollbar)                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_scrollbar_thumb_position_calculate              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Calculate and return the position of a GX_SCROLLBAR thumb button    */
/*    based scrollbar dimensions, current value, and visible range.       */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    scroll                                Widget control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_context_pixelmap_get              Retrieve pixelmap image       */
/*    _gx_widget_resize                     Resize a widget               */
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
VOID _gx_scrollbar_thumb_position_calculate(GX_SCROLLBAR *scroll)
{
INT  thumb_length = 0;
INT  travelsize;
INT  range;
INT  space;
INT  thumb_width;
INT  scroll_width;

GX_RECTANGLE thumbrect;
ULONG        style;
GX_PIXELMAP *thumb_pixelmap = NULL;

    /* pick up scrollbar style */
    style = scroll -> gx_widget_style;

    /* pick up the scroll width and thumb button width */

    thumb_width = scroll -> gx_scrollbar_appearance.gx_scroll_thumb_width;

    /* see if the thumb has a pixelmap */
    if (scroll -> gx_scrollbar_appearance.gx_scroll_thumb_pixelmap)
    {
        _gx_widget_pixelmap_get((GX_WIDGET *) scroll, scroll -> gx_scrollbar_appearance.gx_scroll_thumb_pixelmap, &thumb_pixelmap);
    }

    if (style & GX_SCROLLBAR_VERTICAL)
    {
        scroll_width = scroll -> gx_widget_size.gx_rectangle_right - scroll -> gx_widget_size.gx_rectangle_left + 1;

        travelsize = (scroll -> gx_widget_size.gx_rectangle_bottom -
                      scroll -> gx_widget_size.gx_rectangle_top) + 1;
    }
    else
    {
        scroll_width = scroll -> gx_widget_size.gx_rectangle_bottom - scroll -> gx_widget_size.gx_rectangle_top + 1;

        travelsize = (scroll -> gx_widget_size.gx_rectangle_right -
                      scroll -> gx_widget_size.gx_rectangle_left) + 1;
    }

    travelsize -= scroll -> gx_scrollbar_appearance.gx_scroll_thumb_travel_max +
                  scroll -> gx_scrollbar_appearance.gx_scroll_thumb_travel_min;

    range = scroll -> gx_scrollbar_info.gx_scroll_maximum - scroll -> gx_scrollbar_info.gx_scroll_minimum + 1;

    if (style & GX_SCROLLBAR_RELATIVE_THUMB)
    {
        if (scroll -> gx_scrollbar_info.gx_scroll_maximum !=
            scroll -> gx_scrollbar_info.gx_scroll_minimum)
        {
            thumb_length = travelsize * scroll -> gx_scrollbar_info.gx_scroll_visible;
            if (range)
            {
                thumb_length /= range;
            }
            if (thumb_length < thumb_width)
            {
                thumb_length = thumb_width;
            }
        }
    }
    else
    {
        if (thumb_pixelmap)
        {
            if (style & GX_SCROLLBAR_VERTICAL)
            {
                thumb_length = thumb_pixelmap -> gx_pixelmap_height;
                thumb_width = thumb_pixelmap -> gx_pixelmap_width;
            }
            else
            {
                thumb_length = thumb_pixelmap -> gx_pixelmap_width;
                thumb_width = thumb_pixelmap -> gx_pixelmap_height;
            }
        }
        else
        {
            /* just a square thumb button */
            thumb_length = thumb_width;
        }
    }
    space = (travelsize - thumb_length);
    space *= scroll -> gx_scrollbar_info.gx_scroll_value - scroll -> gx_scrollbar_info.gx_scroll_minimum;
    range -= scroll -> gx_scrollbar_info.gx_scroll_visible;

    if (range)
    {
        space = (space + (range >> 1)) / range;
    }


    if((scroll -> gx_scrollbar_info.gx_scroll_value > scroll -> gx_scrollbar_info.gx_scroll_minimum) &&
       (scroll -> gx_scrollbar_info.gx_scroll_value + scroll -> gx_scrollbar_info.gx_scroll_visible - 1 < scroll -> gx_scrollbar_info.gx_scroll_maximum))
    {
        if (space == 0)
        {
            space = 1;
        }
        else if (space == (travelsize - thumb_length))
        {
            space -= 1;
        }
    }

    thumbrect = scroll -> gx_widget_size;

    if (style & GX_SCROLLBAR_VERTICAL)
    {
        thumbrect.gx_rectangle_left = (GX_VALUE)(thumbrect.gx_rectangle_left + (scroll_width - thumb_width) / 2);
        thumbrect.gx_rectangle_right = (GX_VALUE) (thumbrect.gx_rectangle_left + thumb_width - 1);
        thumbrect.gx_rectangle_top = (GX_VALUE)(scroll -> gx_widget_size.gx_rectangle_top +
                                                scroll -> gx_scrollbar_appearance.gx_scroll_thumb_travel_min + space);
        thumbrect.gx_rectangle_bottom = (GX_VALUE)(thumbrect.gx_rectangle_top + thumb_length - 1);
    }
    else
    {
        thumbrect.gx_rectangle_top = (GX_VALUE)(thumbrect.gx_rectangle_top + (scroll_width - thumb_width) / 2);
        thumbrect.gx_rectangle_bottom = (GX_VALUE)(thumbrect.gx_rectangle_top + thumb_width - 1);

        thumbrect.gx_rectangle_left = (GX_VALUE)(scroll -> gx_widget_size.gx_rectangle_left +
                                                 scroll -> gx_scrollbar_appearance.gx_scroll_thumb_travel_min + space);
        thumbrect.gx_rectangle_right = (GX_VALUE)(thumbrect.gx_rectangle_left + thumb_length - 1);
    }

    _gx_widget_resize((GX_WIDGET *)&scroll -> gx_scrollbar_thumb, &thumbrect);
}

