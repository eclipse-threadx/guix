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
/*    _gx_text_button_create                              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a text button, which is a special type of     */
/*    button (widget).                                                    */
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
/*                                            set new event process,      */
/*                                            added logic to init new     */
/*                                            structure member for        */
/*                                            dynamic bidi text support,  */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT  _gx_text_button_create(GX_TEXT_BUTTON *button, GX_CONST GX_CHAR *name,
                             GX_WIDGET *parent, GX_RESOURCE_ID text_id, ULONG style, USHORT Id,
                             GX_CONST GX_RECTANGLE *size)
{

    /* Call the prompt create function.  */
    _gx_button_create((GX_BUTTON *)button, name, GX_NULL, style, Id, size);

    /* Populate the rest of button control block - overriding as necessary.  */
    button -> gx_widget_type =                     GX_TYPE_TEXT_BUTTON;
    button -> gx_text_button_text_id =             text_id;
    button -> gx_text_button_string.gx_string_ptr =  GX_NULL;
    button -> gx_text_button_string.gx_string_length = 0;
    button -> gx_widget_draw_function =            (VOID (*)(GX_WIDGET *))_gx_text_button_draw;
    button -> gx_widget_event_process_function = (UINT(*)(GX_WIDGET*, GX_EVENT*))_gx_text_button_event_process;
    button -> gx_text_button_normal_text_color =   GX_COLOR_ID_BUTTON_TEXT;
    button -> gx_text_button_selected_text_color = GX_COLOR_ID_BUTTON_TEXT;
    button -> gx_text_button_disabled_text_color = GX_COLOR_ID_DISABLED_TEXT;
    button -> gx_widget_disabled_fill_color = GX_COLOR_ID_DISABLED_FILL;
    button -> gx_text_button_font_id =             GX_FONT_ID_BUTTON;

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
    button -> gx_text_button_bidi_resolved_text_info = GX_NULL;
#endif

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)button);
    }

    /* Return the status from button create.  */
    return(GX_SUCCESS);
}

