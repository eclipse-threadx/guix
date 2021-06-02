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
/*    _gx_generic_scroll_wheel_down_wrap                  PORTABLE C      */
/*                                                           6.1.7        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Ting Zhu, Microsoft Corporation                                     */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function scrolls down the generic scroll wheel.                */
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
/*    _gx_widget_detach                     Detach a widget from its      */
/*                                            parent                      */
/*    _gx_utility_rectangle_shift           Shift a rectangle             */
/*    _gx_widget_resize                     Resize a widget               */
/*    _gx_widget_back_attach                Attach a widget to its parent */
/*    [gx_generic_scroll_wheel_callback]    Callback to create row item   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_generic_scroll_wheel_scroll       Generic scroll wheel scroll   */
/*                                            function                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  06-02-2021     Ting Zhu                 Initial Version 6.1.7         */
/*                                                                        */
/**************************************************************************/
VOID _gx_generic_scroll_wheel_down_wrap(GX_GENERIC_SCROLL_WHEEL *wheel)
{
GX_WIDGET   *test;
GX_WIDGET   *check;
GX_RECTANGLE newpos;

    while ((wheel -> gx_generic_scroll_wheel_top_index > 0) ||
           (wheel -> gx_widget_style & GX_STYLE_WRAP))
    {
        /* Scrolling down, see if my bottom widget can be moved to the top.  */
        test = _gx_widget_last_visible_client_child_get((GX_WIDGET *)wheel);

        if (test && (test -> gx_widget_size.gx_rectangle_top > wheel -> gx_widget_size.gx_rectangle_bottom))
        {

            /* Bottom widget is below my client area, move it to the top.  */
            wheel -> gx_generic_scroll_wheel_top_index--;

            /* Wrap index. */
            if (wheel -> gx_generic_scroll_wheel_top_index < 0)
            {
                wheel -> gx_generic_scroll_wheel_top_index = wheel -> gx_scroll_wheel_total_rows - 1;
            }

            check = _gx_widget_first_visible_client_child_get((GX_WIDGET *)wheel);

            if (check)
            {
                _gx_widget_detach(test);
                newpos = test -> gx_widget_size;
                _gx_utility_rectangle_shift(&newpos, 0, (GX_VALUE)(-(newpos.gx_rectangle_bottom - check -> gx_widget_size.gx_rectangle_top + 1)));
                _gx_widget_resize(test, &newpos);

                wheel -> gx_generic_scroll_wheel_callback(wheel, test, wheel -> gx_generic_scroll_wheel_top_index);
                _gx_widget_back_attach((GX_WIDGET *)wheel, test);
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

