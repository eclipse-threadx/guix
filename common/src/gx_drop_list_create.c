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
#include "gx_system.h"
#include "gx_drop_list.h"
#include "gx_window.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_drop_list_create                                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service creates a drop list.                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    drop_list                             Drop list control block       */
/*    name                                  Name of drop list             */
/*    parent                                Pointer to parent widget      */
/*    total_rows                            Total number of rows in       */
/*                                            drop list                   */
/*    open_height                           Height of the vertical list   */
/*    callback                              function called to create     */
/*                                            new widgets when the list   */
/*                                            is scrolled.                */
/*    style                                 Style of drop list            */
/*    drop_list_id                          Application-defined ID of     */
/*                                            the drop list               */
/*    size                                  Dimensions of the drop list   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_create                     Create the underlying widget  */
/*    memset                                Clear the memory block        */
/*    _gx_vertical_list_create              Create the underlying         */
/*                                            vertical list               */
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
/*                                                                        */
/**************************************************************************/
UINT  _gx_drop_list_create(GX_DROP_LIST *drop_list, GX_CONST GX_CHAR *name,
                           GX_WIDGET *parent, INT total_rows, INT open_height,
                           VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT),
                           ULONG style, USHORT drop_list_id,
                           GX_CONST GX_RECTANGLE *size)
{
GX_VERTICAL_LIST *list;

    /* the drop list should not have enabled style */
    style &= ~GX_STYLE_ENABLED;

    /* Call the widget create function.  */
    _gx_widget_create((GX_WIDGET *)drop_list, name, GX_NULL, style, drop_list_id, size);

    drop_list -> gx_widget_type = GX_TYPE_DROP_LIST;
    drop_list -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_drop_list_event_process;
    drop_list -> gx_drop_list_open_height = open_height;
    drop_list -> gx_drop_list_pixelmap = 0;
    drop_list -> gx_drop_list_popup_open = GX_FALSE;
    drop_list -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_drop_list_draw;
    drop_list -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_drop_list_event_process;
    drop_list -> gx_widget_disabled_fill_color = drop_list -> gx_widget_normal_fill_color;

    /* create the drop-down list, which is a modified vertical list */
    list = &drop_list -> gx_drop_list_popup.gx_popup_list_list;

    _gx_vertical_list_create(list, name, GX_NULL, total_rows, callback, style, drop_list_id, size);
    list -> gx_widget_type = GX_TYPE_POPUP_LIST;
    list -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_popup_list_event_process;
    drop_list -> gx_drop_list_popup.gx_popup_list_owner = (GX_WIDGET *)drop_list;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)drop_list);
    }

    /* Return completion status.  */
    return(GX_SUCCESS);
}

