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
/**   Text Scroll Wheel Management (Scroll Wheel)                         */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_scroll_wheel.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_scroll_wheel_callback_set                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION (deprecated)                                              */
/*                                                                        */
/*    This function updates the pointer to the callback function          */
/*    used to determine the string to be displayed at a give wheel row    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Text scroll wheel control     */
/*                                            block                       */
/*    callback                              Scroll wheel text get callback*/
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
/*                                            added logic to delete       */
/*                                            dynamic bidi text,          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
UINT _gx_text_scroll_wheel_callback_set(GX_TEXT_SCROLL_WHEEL *wheel, GX_CONST GX_CHAR *(*callback)(GX_TEXT_SCROLL_WHEEL *, INT))
{

    /* Update event processing function pointer */
    wheel -> gx_text_scroll_wheel_text_get_deprecated = callback;
    wheel -> gx_text_scroll_wheel_text_get = GX_NULL;
    
#ifdef GX_DYNAMIC_BIDI_TEXT_SUPPORT
    _gx_text_scroll_wheel_dynamic_bidi_text_delete(wheel);
#endif  // GX_DYNAMIC_BIDI_TEXT_SUPPORT

    return(GX_SUCCESS);
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_text_scroll_wheel_callback_set_ext              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function updates the pointer to the callback function          */
/*    used to determine the string to be displayed at a give wheel row    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    wheel                                 Text scroll wheel control     */
/*                                            block                       */
/*    callback                              Scroll wheel text get callback*/
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
/*                                            added logic to delete       */
/*                                            dynamic bidi text,          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT _gx_text_scroll_wheel_callback_set_ext(GX_TEXT_SCROLL_WHEEL *wheel, UINT (*callback)(GX_TEXT_SCROLL_WHEEL *, INT, GX_STRING *))
{

    /* Update event processing function pointer */
    wheel -> gx_text_scroll_wheel_text_get = callback;
#if defined(GX_ENABLE_DEPRECATED_STRING_API)
    wheel -> gx_text_scroll_wheel_text_get_deprecated = GX_NULL;
#endif

#ifdef GX_DYNAMIC_BIDI_TEXT_SUPPORT
    _gx_text_scroll_wheel_dynamic_bidi_text_delete(wheel);
#endif  // GX_DYNAMIC_BIDI_TEXT_SUPPORT

    return(GX_SUCCESS);
}

