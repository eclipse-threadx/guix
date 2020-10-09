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
/**   Context Management (Context)                                        */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_display.h"
#include "gx_system.h"
#include "gx_context.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_context_string_get                              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION  (deprecated)                                             */
/*                                                                        */
/*    This service gets the string associated with the supplied           */
/*      resource ID.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    string_id                             String resource ID            */
/*    return_string                         Pointer to string             */
/*                                            destination pointer         */
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
/*    _gx_widget_text_id_draw                                             */
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

UINT  _gx_context_string_get(GX_RESOURCE_ID resource_id, GX_CONST GX_CHAR **return_string)
{
GX_DRAW_CONTEXT *context = _gx_system_current_draw_context;
GX_DISPLAY      *display;

    display = context -> gx_draw_context_display;
    return _gx_display_string_get(display, resource_id, return_string);
}

#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_context_string_get_ext                          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service gets the string associated with the supplied           */
/*      resource ID.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    string_id                             String resource ID            */
/*    return_string                         Pointer to string             */
/*                                            destination pointer         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_string_get_ext                                          */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_widget_text_id_draw                                             */
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
UINT  _gx_context_string_get_ext(GX_RESOURCE_ID resource_id, GX_STRING *return_string)
{
GX_DRAW_CONTEXT *context = _gx_system_current_draw_context;
GX_DISPLAY      *display;

    display = context -> gx_draw_context_display;
    return _gx_display_string_get_ext(display, resource_id, return_string);
}

