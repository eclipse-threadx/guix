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
/**   Prompt Management (Prompt)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_utility.h"
#include "gx_prompt.h"
#include "gx_system.h"
#include "gx_icon.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_icon_create                                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a bitmap prompt, which is a special type of   */
/*    widget.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    icon                                  Pointer to icon control block */
/*    name                                  Logical name of icon widget   */
/*    parent                                Pointer to the parent widget  */
/*    pixelmap_id                           Resource ID of pixelmap       */
/*    style                                 Style of icon                 */
/*    icon_id                               Application-definedID of icon */
/*    x                                     Starting x-coordinate position*/
/*    y                                     Starting y-coordinate position*/
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_define          Define the icon area          */
/*    _gx_widget_create                     Create the underlying widget  */
/*    _gx_widget_status_add                 Set widget status             */
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
UINT  _gx_icon_create(GX_ICON *icon, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                      GX_RESOURCE_ID pixelmap_id, ULONG style, USHORT icon_id,
                      GX_VALUE x, GX_VALUE y)
{

GX_RECTANGLE size;

    _gx_utility_rectangle_define(&size, x, y, (GX_VALUE)(x + 1), (GX_VALUE)(y + 1));

    /* Call the base prompt create function.  */
    _gx_widget_create((GX_WIDGET *)icon, name, GX_NULL, style, icon_id, &size);

    /* Populate the rest of icon control block - overriding as necessary.  */
    icon -> gx_widget_type = GX_TYPE_ICON;
    icon -> gx_icon_normal_pixelmap = pixelmap_id;
    icon -> gx_icon_selected_pixelmap = 0;
    icon -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_icon_event_process;
    icon -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_icon_draw;

    if (icon_id > 0)
    {
        _gx_widget_status_add((GX_WIDGET *)icon, GX_STATUS_SELECTABLE);
    }

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)icon);
    }

    return(GX_SUCCESS);
}
