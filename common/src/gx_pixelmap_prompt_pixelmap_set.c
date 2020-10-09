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
/**   Prompt Management (Prompt)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_widget.h"
#include "gx_button.h"
#include "gx_system.h"
#include "gx_pixelmap_prompt.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_pixelmap_prompt_pixelmap_set                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function assigned the pixelmaps used to draw a pixelmap_prompt */
/*                                                                        */
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
/*    _gx_system_dirty_mark                 Mark the widget as dirty      */
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
UINT  _gx_pixelmap_prompt_pixelmap_set(GX_PIXELMAP_PROMPT *prompt,
                                       GX_RESOURCE_ID normal_left_id,
                                       GX_RESOURCE_ID normal_fill_id,
                                       GX_RESOURCE_ID normal_right_id,
                                       GX_RESOURCE_ID selected_left_id,
                                       GX_RESOURCE_ID selected_fill_id,
                                       GX_RESOURCE_ID selected_right_id)
{
    prompt -> gx_normal_left_pixelmap_id = normal_left_id;
    prompt -> gx_normal_fill_pixelmap_id = normal_fill_id;
    prompt -> gx_normal_right_pixelmap_id = normal_right_id;

    prompt -> gx_selected_left_pixelmap_id = selected_left_id;
    prompt -> gx_selected_fill_pixelmap_id = selected_fill_id;
    prompt -> gx_selected_right_pixelmap_id = selected_right_id;

    if (prompt -> gx_widget_status & GX_STATUS_VISIBLE)
    {
        /* Mark widget as needing to be re-painted.  */
        _gx_system_dirty_mark((GX_WIDGET *)prompt);
    }

    return(GX_SUCCESS);
}

