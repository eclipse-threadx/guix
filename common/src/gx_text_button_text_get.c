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
#include "gx_system.h"
#include "gx_button.h"
#include "gx_widget.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_button_text_get                            PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION (deprecated)                                              */
/*                                                                        */
/*    This function assigns a string literal to the specified prompt      */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*    return_text                           String used in the button     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_string_get                 Get the string based on ID    */
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
/*                                                                        */
/**************************************************************************/
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_text_button_text_get(GX_TEXT_BUTTON *button, GX_CONST GX_CHAR **return_text)
{
UINT      status = GX_SUCCESS;
GX_STRING string;

    status = _gx_text_button_text_get_ext(button, &string);

    if (status == GX_SUCCESS)
    {
        *return_text = string.gx_string_ptr;
    }
    return status;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_button_text_get_ext                        PORTABLE C      */
/*                                                           6.0          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function assigns a string literal to the specified prompt      */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*    return_text                           String used in the button     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_string_get_ext             Get the string based on ID    */
/*    _gx_system_private_string_get         Get private string copy       */
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
/*                                                                        */
/**************************************************************************/
UINT _gx_text_button_text_get_ext(GX_TEXT_BUTTON *button, GX_STRING *return_text)
{
UINT status = GX_SUCCESS;

    if (button -> gx_text_button_text_id)
    {
        status = _gx_widget_string_get_ext((GX_WIDGET *)button, button -> gx_text_button_text_id, return_text);
    }
    else
    {
        _gx_system_private_string_get(&button -> gx_text_button_string, return_text, button -> gx_widget_style);
    }

    return(status);
}

