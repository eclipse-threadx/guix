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
/**   Button Management (Button)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_button.h"
#include "gx_system.h"
#include "gx_utility.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_pixelmap_button_create                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a pixelmap button, which is a special type of */
/*    button.                                                             */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*    name                                  Name of button                */
/*    parent                                Parent widget control block   */
/*    normal_id                             Normal state pixelmap id      */
/*    selected_id                           Selected state pixelmap id    */
/*    disabled_id                           Disabled state pixelmap id    */
/*    style                                 Style of button               */
/*    pixelmap_button_id                    Application-defined ID of     */
/*                                            the pixelmap button         */
/*    size                                  Button size                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_button_create                     Create the underlying button  */
/*    _gx_pixelmap_transparent_detect       Detect whether or not a       */
/*                                            pixelmap is transparent     */
/*    _gx_widget_status_add                 Set the widget status flag    */
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
UINT  _gx_pixelmap_button_create(GX_PIXELMAP_BUTTON *button,
                                 GX_CONST GX_CHAR *name,
                                 GX_WIDGET *parent,
                                 GX_RESOURCE_ID normal_id,
                                 GX_RESOURCE_ID selected_id,
                                 GX_RESOURCE_ID disabled_id,
                                 ULONG style,
                                 USHORT pixelmap_button_id,
                                 GX_CONST GX_RECTANGLE *size)
{
    /* Call the button create function.  */
    _gx_button_create((GX_BUTTON *)button, name, GX_NULL, style, pixelmap_button_id, size);

    button -> gx_widget_type = GX_TYPE_PIXELMAP_BUTTON;
    button -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_pixelmap_button_draw;
    button -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_pixelmap_button_event_process;
    button -> gx_pixelmap_button_normal_id = normal_id;
    button -> gx_pixelmap_button_selected_id = selected_id;
    button -> gx_pixelmap_button_disabled_id = disabled_id;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)button);
    }

    return(GX_SUCCESS);
}

