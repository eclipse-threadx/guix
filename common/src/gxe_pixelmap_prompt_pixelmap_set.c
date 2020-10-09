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
/**   Button Management (Button)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_pixelmap_prompt.h"
#include "gx_system.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gxe_pixelmap_prompt_pixelmap_set                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function checks for errors in the pixelmap prompt pixelmap     */
/*    set function call.                                                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    prompt                                Prompt control block          */
/*    normal_left_id                        Left pixelmap ID              */
/*    normal_fill_id                        Fill pixelmap ID              */
/*    normal_right_id                       Right pixelmap ID             */
/*    selected_left_id                      Left pixelmap ID              */
/*    selected_fill_id                      Fill pixelmap ID              */
/*    selected_right_id                     Right pixelmap ID             */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_pixelmap_prompt_pixelmap_set      Actual pixelmap prompt        */
/*                                            pixelmap set function       */
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
UINT  _gxe_pixelmap_prompt_pixelmap_set(GX_PIXELMAP_PROMPT *prompt,
                                        GX_RESOURCE_ID normal_left_id,
                                        GX_RESOURCE_ID normal_fill_id,
                                        GX_RESOURCE_ID normal_right_id,
                                        GX_RESOURCE_ID selected_left_id,
                                        GX_RESOURCE_ID selected_fill_id,
                                        GX_RESOURCE_ID selected_right_id)
{
UINT status;

    /* Check for invalid caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    if (!prompt)
    {
        return(GX_PTR_ERROR);
    }

    status = _gx_pixelmap_prompt_pixelmap_set(prompt,
                                              normal_left_id,
                                              normal_fill_id,
                                              normal_right_id,
                                              selected_left_id,
                                              selected_fill_id,
                                              selected_right_id);

    return(status);
}

