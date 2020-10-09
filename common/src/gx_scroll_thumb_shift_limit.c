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
/*    _gx_scrollbar_thumb_shift_limit                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Limit the travel of a scrollbar thumb button so that it cannot      */
/*    shift outside parent's limits.                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    thumb                                 Thumb button control block    */
/*    shift                                 Requested shift amount        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Shift                                 Allowed shift amount          */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
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
INT _gx_scroll_thumb_shift_limit(GX_SCROLL_THUMB *thumb, INT shift)
{
INT           minlimit;
INT           maxlimit;
INT           shiftpos;
GX_WIDGET    *parent;
GX_SCROLLBAR *bar;
GX_RECTANGLE  parentsize;
ULONG         style;

    /* pick up thumb style */
    style = thumb -> gx_widget_style;

    /* pick up parent scrollbar */
    parent = thumb -> gx_widget_parent;
    parentsize = parent -> gx_widget_size;

    bar = (GX_SCROLLBAR *)parent;

    if (style & GX_SCROLLBAR_VERTICAL)
    {
        minlimit = parentsize.gx_rectangle_top + bar -> gx_scrollbar_appearance.gx_scroll_thumb_travel_min;
        maxlimit = parentsize.gx_rectangle_bottom - bar -> gx_scrollbar_appearance.gx_scroll_thumb_travel_max;

        if (shift > 0)
        {
            /* shifting down, don't allow down too far */
            shiftpos = thumb -> gx_widget_size.gx_rectangle_bottom + shift;
            if (shiftpos > maxlimit)
            {
                shift = maxlimit - thumb -> gx_widget_size.gx_rectangle_bottom;
            }
        }
        else
        {
            /* shifting up, don't allow shift up too far */
            shiftpos = thumb -> gx_widget_size.gx_rectangle_top + shift;
            if (shiftpos < minlimit)
            {
                shift = minlimit - thumb -> gx_widget_size.gx_rectangle_top;
            }
        }
    }
    else
    {
        minlimit = parentsize.gx_rectangle_left + bar -> gx_scrollbar_appearance.gx_scroll_thumb_travel_min;
        maxlimit = parentsize.gx_rectangle_right - bar -> gx_scrollbar_appearance.gx_scroll_thumb_travel_max;

        if (shift > 0)
        {
            /* shifting right, don't allow down too over */
            shiftpos = thumb -> gx_widget_size.gx_rectangle_right + shift;
            if (shiftpos > maxlimit)
            {
                shift = maxlimit - thumb -> gx_widget_size.gx_rectangle_right;
            }
        }
        else
        {
            /* shifting left, don't allow shift up too far */
            shiftpos = thumb -> gx_widget_size.gx_rectangle_left + shift;
            if (shiftpos < minlimit)
            {
                shift = minlimit - thumb -> gx_widget_size.gx_rectangle_left;
            }
        }
    }
    return shift;
}

