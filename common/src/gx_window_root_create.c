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
/**   Window Management (Window)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_window.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_window_root_create                              PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a root window, which is a special type of     */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    root_window                           Root Window control block     */
/*    name                                  Name of window                */
/*    canvas                                Canvas this root window       */
/*                                            belongs to                  */
/*    style                                 Style of window               */
/*    Id                                    User-specified root window ID */
/*    size                                  Window size                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memset                                Set control block to zero     */
/*    _gx_window_create                     Create the underlying window  */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            updated the link logic,     */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_window_root_create(GX_WINDOW_ROOT *root_window, GX_CONST GX_CHAR *name,
                             GX_CANVAS *canvas, ULONG style, USHORT Id,
                             GX_CONST GX_RECTANGLE *size)
{

    /* Clear the root window control block.  */
    memset(root_window, 0, sizeof(GX_WINDOW_ROOT));

    /* Call the standard window create API.  */
    _gx_window_create((GX_WINDOW *)root_window, name, GX_NULL, style, Id, size);

    /* Populate the rest of root window control block - overriding as necessary.  */
    root_window -> gx_widget_normal_fill_color = GX_COLOR_ID_CANVAS;
    root_window -> gx_widget_selected_fill_color = GX_COLOR_ID_CANVAS;
    root_window -> gx_widget_disabled_fill_color = GX_COLOR_ID_CANVAS;
    root_window -> gx_widget_type = GX_TYPE_ROOT_WINDOW;
    root_window -> gx_window_root_canvas = canvas;
    root_window -> gx_window_root_views_changed = GX_TRUE;
    root_window -> gx_widget_status &= ~GX_STATUS_MOVABLE;
    root_window -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_window_root_event_process;

    GX_ENTER_CRITICAL

    if (_gx_system_root_window_created_list)
    {
        /* link this root to the one previously created */
        root_window -> gx_widget_next = (GX_WIDGET *)_gx_system_root_window_created_list;

        _gx_system_root_window_created_list -> gx_widget_previous = (GX_WIDGET *)root_window;
    }

    /* Update the root window pointer  */
    _gx_system_root_window_created_list = root_window;

    GX_EXIT_CRITICAL

    /* Return the status from window create.  */
    return(GX_SUCCESS);
}

