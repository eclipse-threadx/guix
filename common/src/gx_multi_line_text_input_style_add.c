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
/**   Multi Line Text Input Managment (Multi Line Text Input)             */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_multi_line_text_input.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_style_add                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function adds styles to a multi-line text input widget.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                             Multi-line text input widget */
/*                                             control block              */
/*    style                                  styles to add                */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_timer_start                Allocate a free timer and     */
/*                                          activates it                  */
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
UINT _gx_multi_line_text_input_style_add(GX_MULTI_LINE_TEXT_INPUT *text_input, ULONG style)
{
UINT     status;
GX_VALUE blink_interval;

    status = _gx_widget_style_add((GX_WIDGET *)text_input, style);

    if (text_input -> gx_widget_style & GX_STYLE_CURSOR_ALWAYS)
    {
        text_input -> gx_widget_status |= (GX_STATUS_CURSOR_SHOW | GX_STATUS_CURSOR_DRAW);

        if ((text_input -> gx_widget_status & GX_STATUS_VISIBLE) &&
            (text_input -> gx_widget_style & GX_STYLE_CURSOR_BLINK))
        {
            blink_interval = text_input -> gx_multi_line_text_input_cursor_instance.gx_text_input_cursor_blink_interval;

            /* Start the timer.  */
            _gx_system_timer_start((GX_WIDGET *)text_input, ID_TEXT_INPUT_TIMER, (UINT)blink_interval, (UINT)blink_interval);
        }
    }

    return status;
}

