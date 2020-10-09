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
/**   System Management (System)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_display.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_string_get                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the string for the specified resource ID.         */
/*    This function has been deprecated.                                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    string_id                             String resource ID            */
/*    return_string                         Pointer to return string      */
/*                                            pointer                     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_string_get                                              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    _gx_multi_line_text_view_create       Create multi line text view   */
/*                                            widget                      */
/*    _gx_multi_line_text_view_text_id_set  Assign text string to the     */
/*                                            multi line text view widget */
/*    _gx_prompt_text_get                   Get text string for the       */
/*                                            prompt widget               */
/*    _gx_text_button_text_get              Get text string for the       */
/*                                            button widget               */
/*    _gx_widget_text_id_draw               Drew the text onto the wdiget */
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
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT  _gx_system_string_get(GX_RESOURCE_ID string_id, GX_CONST GX_CHAR **return_string)
{
UINT status = GX_FAILURE;

    if (_gx_system_display_created_list)
    {
        status = _gx_display_string_get(_gx_system_display_created_list,
                                        string_id, return_string);
    }
    return status;
}
#endif

