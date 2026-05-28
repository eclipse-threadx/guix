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
/**   Progress Bar Management (Radial Progress Bar)                       */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_context.h"
#include "gx_widget.h"
#include "gx_canvas.h"
#include "gx_utility.h"
#include "gx_radial_progress_bar.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_radial_progress_bar_draw                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service draws a radial progress bar.                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    progress_bar                          Radial Progress Bar control   */
/*                                            block                       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_radial_progress_bar_background_draw                             */
/*    _gx_radial_progress_bar_text_draw                                   */
/*    _gx_widget_children_draw                                            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/**************************************************************************/
VOID  _gx_radial_progress_bar_draw(GX_RADIAL_PROGRESS_BAR *progress_bar)
{

    /* Draw progress bar background.  */
    _gx_radial_progress_bar_background_draw(progress_bar);

    /* Draw progress bar text. */
    if (progress_bar -> gx_widget_style & GX_STYLE_PROGRESS_TEXT_DRAW)
    {
        _gx_radial_progress_bar_text_draw(progress_bar);
    }

    /* Draw children widgets of progress bar widget.  */
    _gx_widget_children_draw((GX_WIDGET *)progress_bar);
}

