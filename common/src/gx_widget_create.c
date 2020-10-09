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
/**   Widget Management (Widget)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_utility.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_create                                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a widget.                                     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    widget                                Widget control block          */
/*    name                                  Name of widget                */
/*    parent                                Parent widget control block   */
/*    style                                 Style of widget               */
/*    Id                                    Application-defined ID of the */
/*                                            the widget                  */
/*    size                                  Widget size                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_lock                       Lock access to GUIX           */
/*    _gx_widget_link                       Link a widget to parent       */
/*    _gx_system_unlock                     Release the protection        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX Internal Code                                                  */
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
UINT  _gx_widget_create(GX_WIDGET *widget, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                        ULONG style, USHORT Id, GX_CONST GX_RECTANGLE *size)
{

    /* lock access to GUIX */
    GX_ENTER_CRITICAL

    /* Setup the widget.  */
    widget -> gx_widget_type =                         GX_TYPE_WIDGET;

    widget -> gx_widget_style =                        style;
    widget -> gx_widget_size =                         *size;
    widget -> gx_widget_clip  =                        widget -> gx_widget_size;
    widget -> gx_widget_event_process_function =       _gx_widget_event_process;
    widget -> gx_widget_draw_function =                (VOID (*)(GX_WIDGET *))_gx_widget_draw;
    widget -> gx_widget_normal_fill_color =            GX_COLOR_ID_WIDGET_FILL;
    widget -> gx_widget_selected_fill_color =          GX_COLOR_ID_SELECTED_FILL;
    widget -> gx_widget_disabled_fill_color = GX_COLOR_ID_DISABLED_FILL;
    widget -> gx_widget_id =                           Id;

#if defined(GX_BRUSH_ALPHA_SUPPORT)
    widget -> gx_widget_alpha =                        0xff;
#endif

    if (style & GX_STYLE_DYNAMICALLY_ALLOCATED)
    {
        widget -> gx_widget_status = GX_STATUS_DYNAMICALLY_ALLOCATED;
    }
    else
    {
        widget -> gx_widget_status = 0;
    }

    if (widget -> gx_widget_style & GX_STYLE_ENABLED)
    {
        widget -> gx_widget_status |= (GX_STATUS_ACCEPTS_FOCUS | GX_STATUS_SELECTABLE);
    }

    if (style & GX_STYLE_TRANSPARENT)
    {
        widget -> gx_widget_status |= GX_STATUS_TRANSPARENT;
    }

    /* setup linked list */
    widget -> gx_widget_parent = GX_NULL;
    widget -> gx_widget_first_child = GX_NULL;
    widget -> gx_widget_last_child = GX_NULL;
    widget -> gx_widget_next = GX_NULL;
    widget -> gx_widget_previous = GX_NULL;
    widget -> gx_widget_nav_next = GX_NULL;
    widget -> gx_widget_nav_previous = GX_NULL;

    /* Save the widget name.  */
    widget -> gx_widget_name =  name;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, widget);
    }

    /* Release the protection.  */
    GX_EXIT_CRITICAL

    /* Return successful status.  */
    return(GX_SUCCESS);
}

