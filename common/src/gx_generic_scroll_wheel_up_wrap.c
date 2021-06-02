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
/**   Scroll Wheel Management (Generic Scroll Wheel)                      */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_scroll_wheel.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_generic_scroll_wheel_up_wrap                    PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function scrolls up the generic scroll wheel.                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Generic scroll wheel control  */
/*                                            block                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_first_visible_client_child_get                           */
/*                                          Get the first visible client  */
/*    _gx_widget_last_visible_client_child_get                            */
/*                                          Get the last visible client   */
/*    _gx_utility_rectangle_shift           Move a rectangle              */
/*    [gx_generic_scroll_wheel_callback]     Callback to create row item  */
/*    _gx_widget_resize                     Resize a widget               */
/*    _gx_widget_attach                     Attach a widget to its parent */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_generic_scroll_wheel_scroll        Generic scroll wheel scroll  */
/*                                            function                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  06-02-2021     Ting Zhu                 Initial Version 6.1.7         */
/*                                                                        */
/**************************************************************************/
VOID _gx_generic_scroll_wheel_up_wrap(GX_GENERIC_SCROLL_WHEEL *wheel)
{
GX_WIDGET   *test;
GX_WIDGET   *check;
GX_RECTANGLE newpos;
INT          index;

    while ((wheel -> gx_generic_scroll_wheel_top_index + wheel -> gx_generic_scroll_wheel_child_count < wheel -> gx_scroll_wheel_total_rows) ||
           (wheel -> gx_widget_style & GX_STYLE_WRAP))
    {

        /* Scrolling up. See if my top widget is already above my client area.  */

        test = _gx_widget_first_visible_client_child_get((GX_WIDGET *)wheel);

        if (test && test -> gx_widget_size.gx_rectangle_bottom < wheel -> gx_widget_size.gx_rectangle_top)
        {

            /* Top widget is above my client area, move it to the bottom.  */
            check = _gx_widget_last_visible_client_child_get((GX_WIDGET *)wheel);

            if (check)
            {
                _gx_widget_detach(test);

                newpos = test -> gx_widget_size;
                _gx_utility_rectangle_shift(&newpos, 0, (GX_VALUE)(check -> gx_widget_size.gx_rectangle_bottom - newpos.gx_rectangle_top + 1));
                _gx_widget_resize(test, &newpos);

                index = wheel -> gx_generic_scroll_wheel_top_index + wheel -> gx_generic_scroll_wheel_child_count;

                /* Wrap index.  */
                if (index >= wheel -> gx_scroll_wheel_total_rows)
                {
                    index -= wheel -> gx_scroll_wheel_total_rows;
                }

                wheel -> gx_generic_scroll_wheel_callback(wheel, test, index);

                _gx_widget_attach((GX_WIDGET *)wheel, test);
                wheel -> gx_generic_scroll_wheel_top_index++;

                /* Wrap page index.  */
                if (wheel -> gx_generic_scroll_wheel_top_index >= wheel -> gx_scroll_wheel_total_rows)
                {
                    wheel -> gx_generic_scroll_wheel_top_index -= wheel -> gx_scroll_wheel_total_rows;
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
}

