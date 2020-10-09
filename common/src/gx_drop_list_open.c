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
/**   Drop List (List)                                                    */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_window.h"
#include "gx_system.h"
#include "gx_drop_list.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_drop_list_open                                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service opens a drop list.                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    drop_list                             Drop list control block       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_window_root_find                  Find the root window          */
/*    _gx_widget_resize                     Resize a widget               */
/*    _gx_vertical_list_children_position   Position the children for     */
/*                                          the vertical list             */
/*    _gx_widget_link                       Link the widget to its root   */
/*    _gx_system_focus_claim                Claim the input focus         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_drop_list_event_process                                         */
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
UINT _gx_drop_list_open(GX_DROP_LIST *drop_list)
{
GX_WINDOW_ROOT   *root;
GX_WIDGET        *child;
GX_RECTANGLE      size;
GX_VERTICAL_LIST *list = &drop_list -> gx_drop_list_popup.gx_popup_list_list;

    if (list -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        drop_list -> gx_drop_list_popup_open = GX_TRUE;
        return(GX_SUCCESS);
    }
    _gx_window_root_find((GX_WIDGET *)drop_list, &root);

    size = drop_list -> gx_widget_size;
    size.gx_rectangle_top = (GX_VALUE)(size.gx_rectangle_bottom + 1);
    size.gx_rectangle_bottom = (GX_VALUE)(size.gx_rectangle_top + drop_list -> gx_drop_list_open_height);
    _gx_widget_resize((GX_WIDGET *)list, &size);
    _gx_vertical_list_children_position(list);
    _gx_widget_link((GX_WIDGET *)drop_list -> gx_widget_parent, (GX_WIDGET *)list);

    _gx_vertical_list_selected_widget_get(list, &child);
    if (child)
    {
        child -> gx_widget_style |= GX_STYLE_DRAW_SELECTED;
    }

    drop_list -> gx_drop_list_popup_open = GX_TRUE;
    _gx_system_focus_claim((GX_WIDGET *)list);

    return(GX_SUCCESS);
}

