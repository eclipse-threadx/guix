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
/**   Button Management (radio_button)                                    */
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
/*    _gx_radio_button_create                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service creates a radio button widget.                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*    name                                  Name of button                */
/*    parent                                Parent widget control block   */
/*    text_id                               text resource id              */
/*    style                                 Style of button               */
/*    radio_button_id                       Application-defined ID of     */
/*                                            radio button                */
/*    size                                  Button size                   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_text_button_create                Create text button            */
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
UINT  _gx_radio_button_create(GX_RADIO_BUTTON *button, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                              GX_RESOURCE_ID text_id, ULONG style, USHORT radio_btton_id,
                              GX_CONST GX_RECTANGLE *size)
{

    style |= GX_STYLE_BUTTON_RADIO;

    /* Call the button create function.  */
    _gx_text_button_create((GX_TEXT_BUTTON *)button, name, GX_NULL, text_id, style, radio_btton_id, size);

    /* Populate the rest of button control block - overriding as necessary.  */
    button -> gx_widget_type = GX_TYPE_RADIO_BUTTON;
    button -> gx_widget_draw_function = (VOID (*)(GX_WIDGET *))_gx_radio_button_draw;
    button -> gx_radio_button_on_pixelmap_id = GX_PIXELMAP_RADIO_ON_ID;
    button -> gx_radio_button_off_pixelmap_id = GX_PIXELMAP_RADIO_OFF_ID;
    button -> gx_radio_button_on_disabled_pixelmap_id = GX_PIXELMAP_RADIO_ON_ID;
    button -> gx_radio_button_off_disabled_pixelmap_id = GX_PIXELMAP_RADIO_OFF_ID;

    /* Determine if a parent widget was provided.  */
    if (parent)
    {
        _gx_widget_link(parent, (GX_WIDGET *)button);
    }

    /* Return the status from button create.  */
    return(GX_SUCCESS);
}

