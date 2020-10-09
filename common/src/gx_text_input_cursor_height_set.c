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
/**   Cursor Management (Cursor)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_text_input_cursor.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_input_cursor_height_set                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sets the height value of the cursor                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    cursor_input                          Cursor control block          */
/*    height                                Value to be set               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
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
UINT _gx_text_input_cursor_height_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE height)
{

    /* Set the height value.  */

    if (height > 0)
    {
        cursor_input -> gx_text_input_cursor_height = height;
        cursor_input -> gx_text_input_cursor_flags |= GX_CURSOR_USE_CUSTOM_HEIGHT;
    }
    else
    {
        cursor_input -> gx_text_input_cursor_flags &= (GX_UBYTE)(~GX_CURSOR_USE_CUSTOM_HEIGHT);
    }

    /* Return successful status.  */
    return(GX_SUCCESS);
}

