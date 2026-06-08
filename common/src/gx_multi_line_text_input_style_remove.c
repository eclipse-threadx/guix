/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 Eclipse ThreadX contributors
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 **************************************************************************/


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
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_input_style_remove              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function removes styles from a multi-line text input widget.   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_input                             Multi-lien text input widget */
/*                                             control block              */
/*    style                                  Styles to remove             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_timer_stop                 Stop an active GUIX timer     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/**************************************************************************/
UINT _gx_multi_line_text_input_style_remove(GX_MULTI_LINE_TEXT_INPUT *text_input, ULONG style)
{
UINT status;

    status = _gx_widget_style_remove((GX_WIDGET *)text_input, style);

    if ((text_input -> gx_widget_status & GX_STATUS_VISIBLE) &&
        (style & GX_STYLE_CURSOR_BLINK))
    {
        /* Stop timer. */
        _gx_system_timer_stop((GX_WIDGET *)text_input, ID_TEXT_INPUT_TIMER);

        text_input -> gx_widget_status |= GX_STATUS_CURSOR_DRAW;
    }

    if ((style & GX_STYLE_CURSOR_ALWAYS) &&
        (_gx_system_focus_owner != (GX_WIDGET *)text_input))
    {
        text_input -> gx_widget_status &= (ULONG)(~GX_STATUS_CURSOR_SHOW);
    }

    return status;
}

