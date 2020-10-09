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


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_button_create                                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a vector button, which is a special type of   */
/*    prompt (widget).                                                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Pointer to button control     */
/*                                            block                       */
/*    name                                  Logical name of button        */
/*    parent                                Pointer to parent widget      */
/*                                            of button                   */
/*    style                                 Button stuyle                 */
/*    button_id                             Application-defined ID of     */
/*                                             the button                 */
/*    size                                  Size of the button            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_create                     Create the underlying prompt  */
/*    _gx_widget_status_add                 Set the widget status         */
/*    _gx_widget_link                       Link the widget to its parent */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    _gx_horizontal_scrollbar_create                                     */
/*    _gx_icon_button_create                                              */
/*    _gx_pixelmap_button_create                                          */
/*    _gx_scroll_thumb_create                                             */
/*    _gx_text_button_create                                              */
/*    _gx_vertical_scrollbar_create                                       */
/*    _gx_scroll_thumb_create                                             */
/*    _gx_text_button_create                                              */
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
UINT  _gx_button_create(GX_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                        ULONG style, USHORT button_id, GX_CONST GX_RECTANGLE *size)
{

    /* Call the widget create function.  */
    _gx_widget_create((GX_WIDGET *)button, name, GX_NULL, style, button_id, size);

    /* Check for completion status on the prompt create.  */
    /* Yes, the prompt widget was created successfully.  Populate the rest of
       button control block - overriding as necessary.  */
    button -> gx_widget_type = GX_TYPE_BUTTON;

    _gx_widget_status_add((GX_WIDGET *)button, GX_STATUS_BUTTON_DERIVED);

    button -> gx_button_select_handler = (VOID (*)(GX_WIDGET *))(void (*)(void))_gx_button_select;
    button -> gx_button_deselect_handler = (VOID (*)(GX_WIDGET *, GX_BOOL))(void (*)(void))_gx_button_deselect;
    button -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_button_draw;
    button -> gx_widget_event_process_function =  (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_button_event_process;
    button -> gx_widget_normal_fill_color =       GX_COLOR_ID_BUTTON_LOWER;
    button -> gx_widget_selected_fill_color =     GX_COLOR_ID_BUTTON_UPPER;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)button);
    }

    /* Return the completion status code */
    return(GX_SUCCESS);
}

