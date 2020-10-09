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
/**   Canvas Management (Canvas)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_canvas.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_pixelmap_get                             PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a GX_PIXELMAP structure pointing to the       */
/*    canvas data.                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    pixelmap                              Pointer to destination for    */
/*                                            the retrieved pixlemap      */
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
UINT  _gx_canvas_pixelmap_get(GX_PIXELMAP *pixelmap)
{
GX_DRAW_CONTEXT *context;
GX_DISPLAY      *display;
GX_CANVAS       *canvas;

    /* pick up the current drawing context */
    context = _gx_system_current_draw_context;
    if (context == GX_NULL)
    {
        return GX_INVALID_CONTEXT;
    }

    /* pick up curren canvas. */
    canvas = context -> gx_draw_context_canvas;
    if (canvas == GX_NULL)
    {
        return GX_INVALID_CANVAS;
    }

    /* pick up current display driver */
    display = context -> gx_draw_context_display;
    if (display == GX_NULL)
    {
        return GX_INVALID_DISPLAY;
    }

    memset(pixelmap, 0, sizeof(GX_PIXELMAP));

    pixelmap -> gx_pixelmap_width = canvas -> gx_canvas_x_resolution;
    pixelmap -> gx_pixelmap_height = canvas -> gx_canvas_y_resolution;
    pixelmap -> gx_pixelmap_format = (GX_UBYTE)display -> gx_display_color_format;
    pixelmap -> gx_pixelmap_data = (GX_UBYTE *)canvas -> gx_canvas_memory;
    pixelmap -> gx_pixelmap_data_size = canvas -> gx_canvas_memory_size;
    pixelmap -> gx_pixelmap_version_major = 1;
    pixelmap -> gx_pixelmap_version_minor = 0;

    /* Return successful completion.  */
    return(GX_SUCCESS);
}

