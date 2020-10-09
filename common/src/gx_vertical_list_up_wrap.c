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
/**   List Management (List)                                              */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_system.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_vertical_list_up_wrap                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function scrolls up the vertical list.                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    vertical_list                         Vertical list control block   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_first_client_child_get            Get the first client child    */
/*    _gx_last_client_child_get             Get the last client child     */
/*    _gx_utility_rectangle_shift           Move a rectangle              */
/*    [gx_vertical_list_callback]           Invoke the list callback      */
/*                                            routine                     */
/*    _gx_widget_resize                     Resize a widget               */
/*    _gx_widget_attach                     Attach a widget to its parent */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_vertical_list_scroll              Vertical list scroll function */
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
VOID _gx_vertical_list_up_wrap(GX_VERTICAL_LIST *list)
{
GX_WIDGET   *test;
GX_WIDGET   *check;
GX_BOOL      moving = GX_TRUE;
GX_RECTANGLE newpos;
INT          index;

    while (moving)
    {
        moving = GX_FALSE;

        if ((list -> gx_vertical_list_top_index + list -> gx_vertical_list_child_count < list -> gx_vertical_list_total_rows) ||
            (list -> gx_widget_style & GX_STYLE_WRAP))
        {
            /* scrolling up. See if my top widget is already above my
               client area: */

            test = _gx_widget_first_client_child_get((GX_WIDGET *)list);
            if (test)
            {
                if (test -> gx_widget_size.gx_rectangle_bottom < list -> gx_widget_size.gx_rectangle_top)
                {
                    /* top widget is above my client area, move it to the bottom: */
                    moving = GX_TRUE;
                    check = _gx_widget_last_client_child_get((GX_WIDGET *)list);

                    _gx_widget_detach(test);

                    newpos = test -> gx_widget_size;
                    _gx_utility_rectangle_shift(&newpos, 0, (GX_VALUE)(check -> gx_widget_size.gx_rectangle_bottom - newpos.gx_rectangle_top + 1));
                    _gx_widget_resize(test, &newpos);

                    index = list -> gx_vertical_list_top_index + list -> gx_vertical_list_child_count;

                    /* Wrap index. */
                    if (index >= list -> gx_vertical_list_total_rows)
                    {
                        index -= list -> gx_vertical_list_total_rows;
                    }

                    list -> gx_vertical_list_callback(list, test, index);
                    if (index == list -> gx_vertical_list_selected)
                    {
                        test -> gx_widget_style |=  GX_STYLE_DRAW_SELECTED;
                    }
                    else
                    {
                        test -> gx_widget_style &= ~GX_STYLE_DRAW_SELECTED;
                    }
                    _gx_widget_attach((GX_WIDGET *)list, test);
                    list -> gx_vertical_list_top_index++;

                    /* Wrao page index.  */
                    if (list -> gx_vertical_list_top_index >= list -> gx_vertical_list_total_rows)
                    {
                        list -> gx_vertical_list_top_index -= list -> gx_vertical_list_total_rows;
                    }
                }
            }
        }
    }
}

