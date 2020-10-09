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
/**   Widget Management (Widget)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_canvas.h"
#include "gx_utility.h"
#include "gx_widget.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_widget_child_clipping_update                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Update the clipping members of each child widget to prevent them    */
/*    from drawing outside their parent's area.                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    parent                                Pointer to parent widget      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_rectangle_overlap_detect  Detect overlapping widgets    */
/*    _gx_widget_child_clipping_update      Updating child clipping area  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*    GUIX application code                                               */
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
VOID  _gx_widget_child_clipping_update(GX_WIDGET *parent)
{
GX_WIDGET *child;
GX_WIDGET *win;

    child = parent -> gx_widget_first_child;

    while (child)
    {
        win = child -> gx_widget_parent;
        _gx_utility_rectangle_overlap_detect(&win -> gx_widget_clip,
                                             &child -> gx_widget_size, &child -> gx_widget_clip);

        if (win -> gx_widget_type >= GX_TYPE_WINDOW &&
            !(child -> gx_widget_status & GX_STATUS_NONCLIENT))
        {
            _gx_utility_rectangle_overlap_detect(&((GX_WINDOW *)win) -> gx_window_client,
                                                 &child -> gx_widget_clip, &child -> gx_widget_clip);
        }

        if (child -> gx_widget_first_child)
        {
            child = child -> gx_widget_first_child;
            continue;
        }

        while ((child -> gx_widget_next == GX_NULL) && (child != parent))
        {
            child = child -> gx_widget_parent;
        }

        if (child == parent)
        {
            break;
        }

        child = child -> gx_widget_next;
    }
}

