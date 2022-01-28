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
/**   Vertical List (List)                                                */
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
/*    This service creates a vertical list.                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    vertical_list                         Vertical list control block   */
/*    name                                  Name of vertical list         */
/*    parent                                Pointer to parent widget      */
/*    total_rows                            Total number of rows in       */
/*                                            vertical list               */
/*    callback                              User-specified Callback       */
/*                                            function                    */
/*    style                                 Style of scrollbar widget     */
/*    vertical_list_id                      Application-defined ID of     */
/*                                            vertical list               */
/*    size                                  Dimensions of vertical list   */
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
/*                                            initialized new vertical    */
/*                                            list control block member,  */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
UINT  _gx_vertical_list_create(GX_VERTICAL_LIST *vertical_list,
                               GX_CONST GX_CHAR *name,
                               GX_WIDGET *parent, INT total_rows,
                               VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT),
                               ULONG style, USHORT vertical_list_id,
                               GX_CONST GX_RECTANGLE *size)
{

    /* Call the widget create function.  */
    _gx_window_create((GX_WINDOW *)vertical_list, name, GX_NULL, style, vertical_list_id, size);

    vertical_list -> gx_widget_type =       GX_TYPE_VERTICAL_LIST;
    vertical_list -> gx_widget_status |=    GX_STATUS_NAV_PARENT;
    vertical_list -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_vertical_list_event_process;

    vertical_list -> gx_vertical_list_total_rows = total_rows;
    vertical_list -> gx_vertical_list_top_index = 0;
    vertical_list -> gx_vertical_list_pen_index = -1;
    vertical_list -> gx_vertical_list_child_height = 1;
    vertical_list -> gx_vertical_list_selected = 0;
    vertical_list -> gx_vertical_list_callback = callback;
    vertical_list -> gx_vertical_list_visible_rows = 0;
    vertical_list -> gx_vertical_list_child_count = 0;
    vertical_list -> gx_window_scroll_info_get = (VOID (*)(struct GX_WINDOW_STRUCT *, ULONG, GX_SCROLL_INFO *))_gx_vertical_list_scroll_info_get;
    vertical_list -> gx_vertical_list_idle_child_list = GX_NULL;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link((GX_WIDGET *)parent, (GX_WIDGET *)vertical_list);
    }

    /* Return completion status.  */
    return(GX_SUCCESS);
}

