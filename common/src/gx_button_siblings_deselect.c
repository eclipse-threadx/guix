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
/*    _gx_button_siblings_deselect                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function deselects all sibling buttons of the caller that      */
/*    have GX_STYLE_BUTTON_RADIO style.                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_button_deselect_handler]          Widget-provided deselect      */
/*                                            handler routine             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_button_select                                                   */
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
VOID  _gx_button_siblings_deselect(GX_BUTTON *button)
{
GX_WIDGET *parent;
GX_WIDGET *test;
GX_WIDGET *widget;
GX_BUTTON *sibling;

    widget = (GX_WIDGET *)button;
    parent = widget -> gx_widget_parent;
    test = parent -> gx_widget_first_child;

    while (test)
    {
        /* not the incoming widget? */
        if (test != widget)
        {
            /* is this widget a button? */
            if (test -> gx_widget_status & GX_STATUS_BUTTON_DERIVED)
            {
                sibling = (GX_BUTTON *)test;

                /* is this a pushed radio button?  */
                if ((test -> gx_widget_style & (GX_STYLE_BUTTON_RADIO | GX_STYLE_BUTTON_PUSHED)) ==
                    (GX_STYLE_BUTTON_RADIO | GX_STYLE_BUTTON_PUSHED))
                {
                    sibling -> gx_button_deselect_handler(test, GX_TRUE);
                }
            }
        }
        test = test -> gx_widget_next;
    }
}

