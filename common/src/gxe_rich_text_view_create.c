/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
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
#include "gx_system.h"
#include "gx_window.h"
#include "gx_rich_text_view.h"

GX_CALLER_CHECKING_EXTERNS

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_multi_line_text_view_create                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the rich text view create        */
/*    function.                                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    text_view                             Rich text view widget control */
/*                                            block                       */
/*    name                                  Name of the widget            */
/*    parent                                Pointer to parent widget      */
/*    text_id                               Resource ID of the text string*/
/*    fonts                                 Font list                     */
/*    style                                 Style of text view widget     */
/*    id                                    Application-defined ID of text*/
/*                                            view                        */
/*    size                                  Dimension of the widget       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_rich_text_view_create             Actual rich text view create  */
/*                                            function                    */
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
UINT    _gxe_rich_text_view_create(GX_RICH_TEXT_VIEW *rich_view,
                                   GX_CONST GX_CHAR *name,
                                   GX_WIDGET *parent,
                                   GX_RESOURCE_ID text_id,
                                   GX_RICH_TEXT_FONTS *fonts,
                                   ULONG style,
                                   USHORT id,
                                   GX_CONST GX_RECTANGLE *size,
                                   UINT control_block_size)
{
UINT status;

    /* Check for invalid caller of this function.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    /* Check for invalid pointer.  */
    if ((rich_view == GX_NULL) || (size == GX_NULL) || (fonts == GX_NULL))
    {
        return(GX_PTR_ERROR);
    }

    /* Check for invalid control block size. */
    if (control_block_size != sizeof(GX_RICH_TEXT_VIEW))
    {
        return(GX_INVALID_SIZE);
    }

    /* Check for widget already created.  */
    if (rich_view -> gx_widget_type != 0)
    {
        return(GX_ALREADY_CREATED);
    }

    /* Call actual multi line text input create function.  */
    status = _gx_rich_text_view_create(rich_view, name, parent, text_id, fonts, style, id, size);

    /* Return the error status from window create.  */
    return(status);
}

