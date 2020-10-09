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
/**   Rich Text View Management (Rich Text View)                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_rich_text_view.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_rich_text_view_font_set                         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service sets the font of a rich text view widget.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                             Rich text view widget         */
/*                                            control block               */
/*    fonts                                 Font list                     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_dirty_mark                 Mark a widget as dirty        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
UINT  _gx_rich_text_view_fonts_set(GX_RICH_TEXT_VIEW *text_view, GX_RICH_TEXT_FONTS *fonts)
{
    /* Update rich text view fonts. */
    text_view -> gx_rich_text_view_fonts = *fonts;

    /* Mark that we need to update text total height. */
    text_view -> gx_multi_line_text_view_line_index_old = GX_TRUE;

    if (text_view -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        /* Mark widget as dirty. */
        _gx_system_dirty_mark((GX_WIDGET *)text_view);
    }

    return(GX_SUCCESS);
}

