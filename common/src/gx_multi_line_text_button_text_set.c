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
#include "gx_widget.h"
#include "gx_button.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_button_text_set                 PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION (Deprecated)                                              */
/*                                                                        */
/*    This function assigns text to a multi-line text button              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*    text                                  text pointer                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_text_button_text_set                                            */
/*    _gx_multi_line_text_button_line_pointers_set                        */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added return status check,  */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT  _gx_multi_line_text_button_text_set(GX_MULTI_LINE_TEXT_BUTTON *button, GX_CONST GX_CHAR *text)
{
UINT status;

    status = _gx_text_button_text_set((GX_TEXT_BUTTON *)button, text);

    if (status == GX_SUCCESS)
    {
        _gx_multi_line_text_button_line_pointers_set(button);
    }

    return status;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_multi_line_text_button_text_set_ext             PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION (Deprecated)                                              */
/*                                                                        */
/*    This function assigns text to a multi-line text button              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    button                                Button control block          */
/*    text                                  text pointer                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_text_button_text_set_ext                                        */
/*    _gx_multi_line_text_button_line_pointers_set                        */
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
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added return status check,  */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_multi_line_text_button_text_set_ext(GX_MULTI_LINE_TEXT_BUTTON *button, GX_CONST GX_STRING *text)
{
UINT status;

    status = _gx_text_button_text_set_ext((GX_TEXT_BUTTON *)button, text);

    if (status == GX_SUCCESS)
    {
        _gx_multi_line_text_button_line_pointers_set(button);
    }
    return status;
}
