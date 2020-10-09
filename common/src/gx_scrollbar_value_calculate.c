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
/*    _gx_scrollbar_value_calculate                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Calculate new scrollbar value give thumb button offset and size     */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    scroll                                Scrollbar control block       */
/*    offset                                Thumb button offset           */
/*    size                                  Thumb button height           */
/*                                            (vertical) or width         */
/*                                            (horizontal)                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_widget_event_process_function]    Parent widget event process   */
/*                                            function                    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
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
VOID _gx_scrollbar_value_calculate(GX_SCROLLBAR *scroll, INT offset, INT size)
{
GX_EVENT   newevent;
INT        newval;
INT        oldval;
INT        travel;
UINT       style;

    /* calculate range of allowed values */
    newval = (scroll -> gx_scrollbar_info.gx_scroll_maximum -
              scroll -> gx_scrollbar_info.gx_scroll_minimum) -
              scroll -> gx_scrollbar_info.gx_scroll_visible + 1;

    /* scale this by distance thumb has traveled from minimum */
    style = scroll -> gx_widget_style;

    if (style & GX_SCROLLBAR_VERTICAL)
    {
        newval *= offset - (scroll -> gx_widget_size.gx_rectangle_top +
                            scroll -> gx_scrollbar_appearance.gx_scroll_thumb_travel_min);

        /* calculate total travel allowed */
        travel = (scroll -> gx_widget_size.gx_rectangle_bottom -
                  scroll -> gx_widget_size.gx_rectangle_top) + 1;
    }
    else
    {
        newval *= offset - (scroll -> gx_widget_size.gx_rectangle_left +
                            scroll -> gx_scrollbar_appearance.gx_scroll_thumb_travel_min);

        /* calculate total travel allowed */
        travel = (scroll -> gx_widget_size.gx_rectangle_right -
                  scroll -> gx_widget_size.gx_rectangle_left) + 1;
    }

    travel -= scroll -> gx_scrollbar_appearance.gx_scroll_thumb_travel_max +
              scroll -> gx_scrollbar_appearance.gx_scroll_thumb_travel_min;

    travel -= size;

    /* scale the value using linear interpolation */
    if (travel)
    {
        newval = (newval + (travel >> 1)) / travel;
    }

    /* offset the value based on minimum */
    newval += scroll -> gx_scrollbar_info.gx_scroll_minimum;

    /* make sure we stay within desired range */
    oldval = scroll -> gx_scrollbar_info.gx_scroll_value;
    scroll -> gx_scrollbar_info.gx_scroll_value = newval;
    _gx_scrollbar_limit_check(scroll);

    /* if the value has changed, send a scroll event to my parent */
    if (scroll -> gx_scrollbar_info.gx_scroll_value != oldval)
    {
        /* pass the old value in data[1] */
        newevent.gx_event_payload.gx_event_intdata[1] = oldval;
        newevent.gx_event_payload.gx_event_intdata[0] = scroll -> gx_scrollbar_info.gx_scroll_value;

        if (style & GX_SCROLLBAR_VERTICAL)
        {
            newevent.gx_event_sender = GX_ID_VERTICAL_SCROLL;
            newevent.gx_event_type = GX_EVENT_VERTICAL_SCROLL;
        }
        else
        {
            newevent.gx_event_sender = GX_ID_HORIZONTAL_SCROLL;
            newevent.gx_event_type = GX_EVENT_HORIZONTAL_SCROLL;
        }

        /* send event to my parent window */
        newevent.gx_event_target = scroll -> gx_widget_parent;
        _gx_system_event_send(&newevent);
    }
}

