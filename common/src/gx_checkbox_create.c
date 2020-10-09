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
/**   Button Management (checkbox)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_button.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_checkbox_create                                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a checkbox, which is a special type of        */
/*    button (widget).                                                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    checkbox                              Checkbox control block        */
/*    name                                  Name of checkbox              */
/*    parent                                Parent widget control block   */
/*    text_id                               Text resource id              */
/*    style                                 Style of checkbox             */
/*    checkbox_id                           Checkbox id                   */
/*    size                                  Checkbox size                 */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_text_button_create                Create the underlying button  */
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
/*                                            fixed compiler warnings,    */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT  _gx_checkbox_create(GX_CHECKBOX *checkbox, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                          GX_RESOURCE_ID text_id, ULONG style, USHORT checkbox_id,
                          GX_CONST GX_RECTANGLE *size)
{

    style |= GX_STYLE_BUTTON_TOGGLE;

    /* Call the prompt create function.  */
    _gx_text_button_create((GX_TEXT_BUTTON *)checkbox, name, GX_NULL, text_id, style, checkbox_id, size);

    /* Populate the rest of button control block - overriding as necessary.  */
    checkbox -> gx_widget_type = GX_TYPE_CHECKBOX;
    checkbox -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_checkbox_draw;
    checkbox -> gx_button_select_handler = (VOID (*)(GX_WIDGET *))(void (*)(void))_gx_checkbox_select;
    checkbox -> gx_widget_event_process_function = (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_checkbox_event_process;
    checkbox -> gx_checkbox_checked_pixelmap_id = GX_PIXELMAP_CHECKBOX_ON_ID;
    checkbox -> gx_checkbox_unchecked_pixelmap_id = GX_PIXELMAP_CHECKBOX_OFF_ID;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)checkbox);
    }

    /* Return the status from button create.  */
    return(GX_SUCCESS);
}

