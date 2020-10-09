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
#include "gx_widget.h"
#include "gx_window.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_window_create                                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a window, which is a special type of          */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    window                                Pointer to window control     */
/*                                           block                        */
/*    name                                  Logical name of window        */
/*    parent                                Pointer to parent window      */
/*    style                                 Window Style                  */
/*    window_id                             Application-defined ID of the */
/*                                            window                      */
/*    size                                  Size of the window            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_create                     Create the underlying widget  */
/*    _gx_widget_border_width_get           Get the widget border width   */
/*    _gx_widget_client_get                 Get the widget client         */
/*    _gx_widget_link                       Link the widget to the parent */
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
/*                                            fixed compiler warnings,    */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT  _gx_window_create(GX_WINDOW *window, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                        ULONG style, USHORT window_id, GX_CONST GX_RECTANGLE *size)
{

GX_VALUE border_width;

    /* Call the widget create function.  */
    _gx_widget_create((GX_WIDGET *)window, name, GX_NULL, style, window_id, size);

    /* Populate the rest of window control block - overriding as necessary.  */
    window -> gx_widget_type = GX_TYPE_WINDOW;
    window -> gx_widget_status |= GX_STATUS_ACCEPTS_FOCUS;
    _gx_widget_border_width_get((GX_WIDGET *)window, &border_width);
    _gx_widget_client_get((GX_WIDGET *)window,
                          border_width,
                          &window -> gx_window_client);

    window -> gx_window_views =                   GX_NULL;
    window -> gx_widget_normal_fill_color =       GX_COLOR_ID_WINDOW_FILL;
    window -> gx_widget_selected_fill_color =     GX_COLOR_ID_WINDOW_FILL;
    window -> gx_window_wallpaper   =             0;
    window -> gx_widget_type =                    GX_TYPE_WINDOW;
    window -> gx_window_move_mode =               0;
    window -> gx_window_move_start.gx_point_x =   0;
    window -> gx_window_move_start.gx_point_y =   0;
    window -> gx_widget_draw_function =           (VOID (*)(GX_WIDGET *))_gx_window_draw;
    window -> gx_widget_event_process_function =  (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_window_event_process;
    window -> gx_window_scroll_info_get =         (VOID (*)(GX_WINDOW *, ULONG, GX_SCROLL_INFO *))(void (*)(void))_gx_window_scroll_info_get;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)window);
    }

    /* Return completion status code. */
    return(GX_SUCCESS);
}

