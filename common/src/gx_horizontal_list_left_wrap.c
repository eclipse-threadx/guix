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
/**   Horizontal List (List)                                              */
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
/*    _gx_horizontal_list_left_wrap                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function scrolls up the horizontal list.                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    list                                  horizontal list widget control*/
/*                                            block                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_first_client_child_get            Get the first client child    */
/*    _gx_last_client_child_get             Get the last client child     */
/*    _gx_widget_front_move                 Move widget to the front      */
/*    _gx_widget_shift                      Shift a widget                */
/*    [gx_horizontal_list_callback]         Horizontal list callback      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_horizontal_list_scroll                                          */
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
VOID _gx_horizontal_list_left_wrap(GX_HORIZONTAL_LIST *list)
{
GX_WIDGET   *test;
GX_WIDGET   *check;
GX_RECTANGLE newpos;
GX_BOOL      moving = GX_TRUE;
INT          index;

    while (moving)
    {
        moving = GX_FALSE;

        if ((list -> gx_horizontal_list_top_index + list -> gx_horizontal_list_child_count < list -> gx_horizontal_list_total_columns) ||
            (list -> gx_widget_style & GX_STYLE_WRAP))
        {
            /* scrolling up. See if my top widget is already above my
               client area: */

            test = _gx_widget_first_client_child_get((GX_WIDGET *)list);
            if (test)
            {
                if (test -> gx_widget_size.gx_rectangle_right < list -> gx_widget_size.gx_rectangle_left)
                {
                    /* left widget is left of my client area, move it to the right: */

                    moving = GX_TRUE;
                    check = _gx_widget_last_client_child_get((GX_WIDGET *)list);

                    _gx_widget_detach(test);

                    newpos = test -> gx_widget_size;
                    _gx_utility_rectangle_shift(&newpos,
                                                (GX_VALUE)(check -> gx_widget_size.gx_rectangle_right -
                                                           test -> gx_widget_size.gx_rectangle_left + 1),
                                                0);
                    _gx_widget_resize(test, &newpos);

                    index = list -> gx_horizontal_list_top_index + list -> gx_horizontal_list_child_count;

                    /* Wrap index. */
                    if (index >= list -> gx_horizontal_list_total_columns)
                    {
                        index -= list -> gx_horizontal_list_total_columns;
                    }

                    list -> gx_horizontal_list_callback(list, test, index);
                    if (index == list -> gx_horizontal_list_selected)
                    {
                        test -> gx_widget_style |=  GX_STYLE_DRAW_SELECTED;
                    }
                    else
                    {
                        test -> gx_widget_style &= ~GX_STYLE_DRAW_SELECTED;
                    }
                    _gx_widget_attach((GX_WIDGET *)list, test);
                    list -> gx_horizontal_list_top_index++;

                    /* Wrap page index.  */
                    if (list -> gx_horizontal_list_top_index >= list -> gx_horizontal_list_total_columns)
                    {
                        list -> gx_horizontal_list_top_index -= list -> gx_horizontal_list_total_columns;
                    }
                }
            }
        }
    }
}

