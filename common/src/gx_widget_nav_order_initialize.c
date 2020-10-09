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
/**   Widget Management (Widget)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_nav_order_initialize                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function configures next/previous navigation order.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Widget control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
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
VOID _gx_widget_nav_order_initialize(GX_WIDGET *widget)
{
GX_WIDGET *winner;
GX_WIDGET *child;
GX_WIDGET *first_stop = NULL;
GX_WIDGET *last_stop = NULL;
GX_BOOL    assign_default_focus = GX_TRUE;

    child = widget -> gx_widget_first_child;

    /* test to see if any child already has default focus status flag */
    while (child)
    {
        if (assign_default_focus)
        {
            if (child -> gx_widget_status & GX_STATUS_DEFAULT_FOCUS)
            {
                assign_default_focus = GX_FALSE;
            }
        }
        else
        {
            /* make sure only one child has default focus flag */
            child -> gx_widget_status &= ~GX_STATUS_DEFAULT_FOCUS;
        }

        child -> gx_widget_nav_next = GX_NULL;
        child -> gx_widget_nav_previous = GX_NULL;
        child = child -> gx_widget_next;
    }

    /* loop through child widgets looking for
       the top-left child and assigning the navigation order
       in left-to-right top-to-bottom order.
     */

    do
    {
        child = widget -> gx_widget_first_child;
        winner = GX_NULL;

        while (child)
        {
            if (child == last_stop || (child -> gx_widget_status & GX_STATUS_VISIBLE) == 0)
            {
                child = child -> gx_widget_next;
                continue;
            }

            if (!child -> gx_widget_nav_next)
            {
                if ((child -> gx_widget_status & GX_STATUS_ACCEPTS_FOCUS) &&
                    !(child -> gx_widget_status & GX_STATUS_NONCLIENT))
                {
                    if (winner)
                    {
                        if (child -> gx_widget_size.gx_rectangle_top < winner -> gx_widget_size.gx_rectangle_top)
                        {
                            winner = child;
                        }
                        else
                        {
                            if (child -> gx_widget_size.gx_rectangle_top == winner -> gx_widget_size.gx_rectangle_top &&
                                child -> gx_widget_size.gx_rectangle_left < winner -> gx_widget_size.gx_rectangle_left)
                            {
                                winner = child;
                            }
                        }
                    }
                    else
                    {
                        winner = child;
                    }
                }
            }

            child = child -> gx_widget_next;
        }

        if (winner)
        {
            if (!first_stop)
            {
                first_stop = winner;

                if (assign_default_focus)
                {
                    first_stop -> gx_widget_status |= GX_STATUS_DEFAULT_FOCUS;
                }
            }

            if (last_stop)
            {
                winner -> gx_widget_nav_previous = last_stop;
                last_stop -> gx_widget_nav_next = winner;
            }
            last_stop = winner;
        }
    } while (winner);

    /* loop the last in the order back to the first */
    if (last_stop)
    {
        last_stop -> gx_widget_nav_next = first_stop;
        first_stop -> gx_widget_nav_previous = last_stop;
    }
}

