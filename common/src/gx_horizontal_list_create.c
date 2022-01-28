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
#include "gx_scrollbar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_vetical_list_create                             PORTABLE C      */
/*                                                           6.1.10       */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service creates a horizontal list.                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    horizontal_list                     horizontal list widget control  */
/*                                          block                         */
/*    name                                Name of horizontal list         */
/*    parent                                Pointer to parent widget      */
/*    total_rows                            Total number of rows in       */
/*                                        horizontal list                 */
/*    callback                            Function called to create       */
/*                                          new widgets when the list     */
/*                                          is scrolled.                  */
/*    style                                 Style of scrollbar widget     */
/*    horizontal_list_id                    Application-defined ID of     */
/*                                        horizontal list                 */
/*    size                                Dimensions of horizontal list   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_window_create                     Create the underlying window  */
/*    _gx_widget_link                       Link the widget to its parent */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  01-31-2022     Ting Zhu                 Modified comment(s),          */
/*                                            initialized new horizontal  */
/*                                            list control block member,  */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
UINT  _gx_horizontal_list_create(GX_HORIZONTAL_LIST *horizontal_list,
                                 GX_CONST GX_CHAR *name,
                                 GX_WIDGET *parent, INT total_columns,
                                 VOID (*callback)(GX_HORIZONTAL_LIST *, GX_WIDGET *, INT),
                                 ULONG style, USHORT horizontal_list_id,
                                 GX_CONST GX_RECTANGLE *size)
{

    /* Call the widget create function.  */
    _gx_window_create((GX_WINDOW *)horizontal_list, name, GX_NULL, style, horizontal_list_id, size);

    horizontal_list -> gx_widget_type =       GX_TYPE_HORIZONTAL_LIST;
    horizontal_list -> gx_widget_status |=    GX_STATUS_NAV_PARENT;
    horizontal_list -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_horizontal_list_event_process;

    horizontal_list -> gx_horizontal_list_total_columns = total_columns;
    horizontal_list -> gx_horizontal_list_top_index = 0;
    horizontal_list -> gx_horizontal_list_pen_index = -1;
    horizontal_list -> gx_horizontal_list_child_width = 1;
    horizontal_list -> gx_horizontal_list_selected = 0;
    horizontal_list -> gx_horizontal_list_callback = callback;
    horizontal_list -> gx_horizontal_list_visible_columns = 0;
    horizontal_list -> gx_horizontal_list_child_count = 0;
    horizontal_list -> gx_window_scroll_info_get = _gx_horizontal_list_scroll_info_get;
    horizontal_list -> gx_horizontal_list_idle_child_list = GX_NULL;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)horizontal_list);
    }

    /* Return completion status.  */
    return(GX_SUCCESS);
}

