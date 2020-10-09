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
/**   Text Button Management (Button)                                     */
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
/*    _gx_multi_line_text_button_create                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a multi-line text button, which is a special  */
/*    type of button (widget).                                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*    name                                  Name of button                */
/*    parent                                Parent widget control block   */
/*    text_id                               text resource id              */
/*    style                                 Style of button               */
/*    size                                  Button size                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memset                                Set control block to zero     */
/*    _gx_button_create                     Create the underlying button  */
/*    _gx_widget_link                       Link the button to its parent */
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
UINT  _gx_multi_line_text_button_create(GX_MULTI_LINE_TEXT_BUTTON *button, GX_CONST GX_CHAR *name,
                                        GX_WIDGET *parent, GX_RESOURCE_ID text_id, ULONG style, USHORT Id,
                                        GX_CONST GX_RECTANGLE *size)
{

INT  line_index;

    /* Call the prompt create function.  */
    _gx_text_button_create((GX_TEXT_BUTTON *)button, name, GX_NULL, text_id, style, Id, size);

    /* Populate the rest of button control block - overriding as necessary.  */
    button -> gx_widget_type =                     GX_TYPE_MULTI_LINE_TEXT_BUTTON;
    button -> gx_widget_draw_function =            (VOID (*)(GX_WIDGET *))_gx_multi_line_text_button_draw;
    button -> gx_widget_event_process_function =   (UINT (*)(GX_WIDGET *, GX_EVENT *))_gx_multi_line_text_button_event_process;
    button -> gx_multi_line_text_button_line_count = 0;

    for (line_index = 0; line_index < GX_MULTI_LINE_TEXT_BUTTON_MAX_LINES; line_index++)
    {
        button -> gx_multi_line_text_button_lines[line_index].gx_string_ptr = GX_NULL;
        button -> gx_multi_line_text_button_lines[line_index].gx_string_length = 0;
    }

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)button);
    }

    /* Return the status from button create.  */
    return(GX_SUCCESS);
}

