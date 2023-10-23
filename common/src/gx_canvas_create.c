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
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_canvas.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_canvas_create                                   PORTABLE C      */
/*                                                           6.3.0        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a canvas associated with the specified        */
/*    display.                                                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*    name                                  Name of canvas                */
/*    display                               Display control block         */
/*    type                                  Type of canvas                */
/*    width                                 Width of canvas               */
/*    height                                Height of canvas              */
/*    memory_area                           Memory area of canvas with    */
/*                                            each pixel of GX_COLOR      */
/*    memory_size                           Size of canvas memory area    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memset                                Set control block and canvas  */
/*                                            memory to zero              */
/*    _gx_utility_rectangle_define          Define a rectangle            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    _gx_animation_canvas_define                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1.3  */
/*  10-31-2023     Ting Zhu                 Modified comment(s),          */
/*                                            added initilization to the  */
/*                                            canvas members,             */
/*                                            resulting in version 6.3.0  */
/*                                                                        */
/**************************************************************************/
UINT  _gx_canvas_create(GX_CANVAS *canvas, GX_CONST GX_CHAR *name, GX_DISPLAY *display,
                        UINT type, UINT width, UINT height, GX_COLOR *memory_area, ULONG memory_size)
{
    /* Clear the canvas control block.  */
    memset(canvas, 0, sizeof(GX_CANVAS));

    /* Setup the canvas.  */
    canvas -> gx_canvas_display =           display;
    canvas -> gx_canvas_memory =            memory_area;
    canvas -> gx_canvas_memory_size =       memory_size;
    canvas -> gx_canvas_alpha =             GX_ALPHA_VALUE_OPAQUE;
    canvas -> gx_canvas_display_offset_x =  0;
    canvas -> gx_canvas_display_offset_y =  0;
    canvas -> gx_canvas_draw_count =        0;
    canvas -> gx_canvas_draw_nesting =      0;
    canvas -> gx_canvas_dirty_count =       0;
    canvas -> gx_canvas_status =            type;
    canvas -> gx_canvas_x_resolution =      (GX_VALUE)width;
    canvas -> gx_canvas_y_resolution =      (GX_VALUE)height;
#ifdef GX_ENABLE_CANVAS_PARTIAL_FRAME_BUFFER
    canvas -> gx_canvas_memory_width =      (GX_VALUE)width;
    canvas -> gx_canvas_memory_height =     (GX_VALUE)height;
#endif
    canvas -> gx_canvas_hardware_layer =    (GX_BYTE)-1;

    /* Determine if there is a memory area.  */
    if (memory_area)
    {
        /* Yes, clear it!  */
        memset(memory_area, 0, memory_size);
    }

    /* Setup the dirty area.  */
    _gx_utility_rectangle_define(&canvas -> gx_canvas_dirty_area, 0, 0, -1, -1);

    /* Now link the canvas control block on the list of created canvases.  */

    /* Load the canvas ID field in the canvas control block.  */
    canvas -> gx_canvas_id =  GX_CANVAS_ID;

    /* Save the canvas name.  */
    canvas -> gx_canvas_name =  name;

    /* If running on Win32, create padded memory, only if needed */
#ifdef GX_TARGET_WIN32
    _win32_compatible_canvas_memory_allocate(canvas);
#endif

    /* initialize previous and next pointers */
    canvas -> gx_canvas_created_previous =  GX_NULL;
    canvas -> gx_canvas_created_next = GX_NULL;

    /* lock access to gx_system */

    GX_ENTER_CRITICAL

    /* Place the canvas on the list of created canvass.  First,
       check for an empty list.  */
    _gx_system_canvas_created_count++;

    if (_gx_system_canvas_created_count > 1)
    {
        /* Place the new canvas in the list.  */
        _gx_system_canvas_created_list -> gx_canvas_created_previous = canvas;
        canvas -> gx_canvas_created_next = _gx_system_canvas_created_list;
    }

    /* point start of list at this new canvas */
    _gx_system_canvas_created_list = canvas;

    /* unlock gx_system.  */
    GX_EXIT_CRITICAL

    /* Return successful status.  */
    return(GX_SUCCESS);
}

#ifdef GX_TARGET_WIN32
VOID _win32_compatible_canvas_memory_allocate(GX_CANVAS *canvas)
{
USHORT      rotation;
INT         padded_width;
USHORT      row_byte_width;
INT         color_format = GX_COLOR_FORMAT_565RGB;
GX_DISPLAY *display = canvas -> gx_canvas_display;

    /* Windows bitmaps must be padded to an even multiple of 4 bytes in width.
       When the GUIX canvas does not meet this requirement, we create a padded canvas memory
       so that we can pass the padded canvas memory off to Windows for display as a bitmap.
       This happens often when running at sub-byte color depth, but can occur at any color depth
       if the display resolution is very odd. */

    padded_width = canvas -> gx_canvas_x_resolution;

    if (display == GX_NULL)
    {
        return;
    }

    rotation = display -> gx_display_rotation_angle;

    padded_width = canvas -> gx_canvas_x_resolution;

    row_byte_width = display -> gx_display_driver_row_pitch_get(padded_width);

    while (row_byte_width % 4)
    {
        padded_width++;
        row_byte_width = display -> gx_display_driver_row_pitch_get(padded_width);
    }

    if ((padded_width != canvas -> gx_canvas_x_resolution) || rotation)
    {
        /* We are forced to create a padded buffer to hold Win32 compatible canvas memory. */
        canvas -> gx_canvas_padded_memory = (GX_COLOR *)malloc(row_byte_width * canvas -> gx_canvas_y_resolution);
    }
}

#endif

