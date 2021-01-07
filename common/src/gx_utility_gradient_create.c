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
/**   Utility (Utility)                                                   */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_canvas.h"
#include "gx_system.h"
#include "gx_utility.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_gradient_find                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Helper function for _gx_utility_gradient_create. This function      */
/*    searches the gradient list to find a match.                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    gradient                                   GX_GRADIENT pointer      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_utility_gradient_create                                         */
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
static GX_BOOL _gx_utility_gradient_find(GX_GRADIENT *gradient)
{
GX_GRADIENT *search = _gx_system_gradient_list;

    /* look for a matching gradient */
    while (search)
    {
        if (search -> gx_gradient_pixelmap.gx_pixelmap_data &&
            search -> gx_gradient_type == gradient -> gx_gradient_type &&
            search -> gx_gradient_alpha_start == gradient -> gx_gradient_alpha_start &&
            search -> gx_gradient_alpha_end == gradient -> gx_gradient_alpha_end &&
            search -> gx_gradient_pixelmap.gx_pixelmap_width == gradient -> gx_gradient_pixelmap.gx_pixelmap_width &&
            search -> gx_gradient_pixelmap.gx_pixelmap_height == gradient -> gx_gradient_pixelmap.gx_pixelmap_height)
        {
            /* found a match, just copy the pixelmap */
            gradient -> gx_gradient_pixelmap = search -> gx_gradient_pixelmap;
            return GX_TRUE;
        }
        search = search -> gx_gradient_next;
    }

    /* did not find a match, need to create a new gradient pixelmap */
    return GX_FALSE;
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_horizontal_alpha_gradient_create        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a horizontal alpha gradient pixelmap.         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    gradient                                   GX_GRADIENT structure    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_utility_gradient_create                                         */
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
static UINT _gx_utility_horizontal_alpha_gradient_create(GX_GRADIENT *gradient)
{
INT       column;
INT       row;
INT       height;
INT       width;
GX_UBYTE  dataval;
GX_UBYTE *data;
GX_UBYTE *data_start = (GX_UBYTE *)gradient -> gx_gradient_pixelmap.gx_pixelmap_data;

    height = gradient -> gx_gradient_pixelmap.gx_pixelmap_height;
    width = gradient -> gx_gradient_pixelmap.gx_pixelmap_width;

    if (gradient -> gx_gradient_type & GX_GRADIENT_TYPE_MIRROR)
    {
        for (column = 0; column < width / 2; column++)
        {
            dataval = (GX_UBYTE)(GX_FIXED_VAL_TO_INT(gradient -> gx_gradient_alpha_current));
            data = data_start;

            for (row = 0; row < height; row++)
            {
                *data = dataval;
                data += width;
            }
            data_start++;
            gradient -> gx_gradient_alpha_current += gradient -> gx_gradient_alpha_step;
        }
        for (; column < width; column++)
        {
            dataval = (GX_UBYTE)(GX_FIXED_VAL_TO_INT(gradient -> gx_gradient_alpha_current));
            data = data_start;

            for (row = 0; row < height; row++)
            {
                *data = dataval;
                data += width;
            }
            data_start++;
            gradient -> gx_gradient_alpha_current -= gradient -> gx_gradient_alpha_step;
        }
    }
    else
    {
        for (column = 0; column < width; column++)
        {
            dataval = (GX_UBYTE)(GX_FIXED_VAL_TO_INT(gradient -> gx_gradient_alpha_current));
            data = data_start;

            for (row = 0; row < height; row++)
            {
                *data = dataval;
                data += width;
            }
            data_start++;
            gradient -> gx_gradient_alpha_current += gradient -> gx_gradient_alpha_step;
        }
    }
    return GX_SUCCESS;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_vertical_alpha_gradient_create          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a vertical alpha gradient pixelmap.           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    gradient                                   GX_GRADIENT structure    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_utility_gradient_create                                         */
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
static UINT _gx_utility_vertical_alpha_gradient_create(GX_GRADIENT *gradient)
{
INT       row;
INT       height;
INT       width;
GX_UBYTE *data = (GX_UBYTE *)gradient -> gx_gradient_pixelmap.gx_pixelmap_data;

    height = gradient -> gx_gradient_pixelmap.gx_pixelmap_height;
    width = gradient -> gx_gradient_pixelmap.gx_pixelmap_width;

    if (gradient -> gx_gradient_type & GX_GRADIENT_TYPE_MIRROR)
    {
        for (row = 0; row < height / 2; row++)
        {
            memset(data, GX_FIXED_VAL_TO_INT(gradient -> gx_gradient_alpha_current), (size_t)width);
            data += width;
            gradient -> gx_gradient_alpha_current += gradient -> gx_gradient_alpha_step;
        }
        for (; row < height; row++)
        {
            memset(data, GX_FIXED_VAL_TO_INT(gradient -> gx_gradient_alpha_current), (size_t)width);
            data += width;
            gradient -> gx_gradient_alpha_current -= gradient -> gx_gradient_alpha_step;
        }
    }
    else
    {
        for (row = 0; row < height; row++)
        {
            memset(data, GX_FIXED_VAL_TO_INT(gradient -> gx_gradient_alpha_current), (size_t)width);
            data += width;
            gradient -> gx_gradient_alpha_current += gradient -> gx_gradient_alpha_step;
        }
    }

    return GX_SUCCESS;
}




/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_gradient_create                         PORTABLE C      */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a gradient pixelmap.                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    gradient                              Pointer to GX_GRADIENT        */
/*    width                                 Requested width               */
/*    height                                Requested height              */
/*    type                                  Gradient type                 */
/*    alpha_start                           Starting alpha value          */
/*    alpha_end                             Ending alpha value            */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_gradient_find                                            */
/*    _gx_utility_alphamap_create                                         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_text_scroll_wheel_gradient_create                               */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  12-31-2020     Kenneth Maxwell          Modified comment(s), removed  */
/*                                            errant comment,             */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
UINT _gx_utility_gradient_create(GX_GRADIENT *gradient, GX_VALUE width, GX_VALUE height,
                                 UCHAR type, GX_UBYTE alpha_start, GX_UBYTE alpha_end)
{
UINT status = GX_FAILURE;

    /* width and height must be >= 2 pixels */

    if (width < 2 || height < 2)
    {
        return GX_INVALID_SIZE;
    }

    /* only alpha gradient supported currently */
    if (!(type & GX_GRADIENT_TYPE_ALPHA))
    {
        return GX_NOT_SUPPORTED;
    }

    memset(&gradient -> gx_gradient_pixelmap, 0, sizeof(GX_PIXELMAP));
    gradient -> gx_gradient_pixelmap.gx_pixelmap_width = width;
    gradient -> gx_gradient_pixelmap.gx_pixelmap_height = height;
    gradient -> gx_gradient_type = type;
    gradient -> gx_gradient_alpha_start = alpha_start;
    gradient -> gx_gradient_alpha_end = alpha_end;
    gradient -> gx_gradient_previous = GX_NULL;
    gradient -> gx_gradient_next = GX_NULL;

    if (_gx_utility_gradient_find(gradient))
    {
        status = GX_SUCCESS;
    }
    else
    {
        gradient -> gx_gradient_alpha_current = GX_FIXED_VAL_MAKE(gradient -> gx_gradient_alpha_start);
        gradient -> gx_gradient_alpha_step = GX_FIXED_VAL_MAKE(gradient -> gx_gradient_alpha_end);
        gradient -> gx_gradient_alpha_step -= GX_FIXED_VAL_MAKE(gradient -> gx_gradient_alpha_start);

        status = _gx_utility_alphamap_create(width, height, &gradient -> gx_gradient_pixelmap);

        if (status == GX_SUCCESS)
        {
            if (gradient -> gx_gradient_type & GX_GRADIENT_TYPE_VERTICAL)
            {
                if (gradient -> gx_gradient_type & GX_GRADIENT_TYPE_MIRROR)
                {
                    gradient -> gx_gradient_alpha_step /= (height / 2);
                }
                else
                {
                    gradient -> gx_gradient_alpha_step /= height;
                }
                status = _gx_utility_vertical_alpha_gradient_create(gradient);
            }
            else
            {
                if (gradient -> gx_gradient_type & GX_GRADIENT_TYPE_MIRROR)
                {
                    gradient -> gx_gradient_alpha_step /= (width / 2);
                }
                else
                {
                    gradient -> gx_gradient_alpha_step /= width;
                }
                status = _gx_utility_horizontal_alpha_gradient_create(gradient);
            }
        }
    }

    if (status == GX_SUCCESS)
    {
        /* lock access to GUIX */
        GX_ENTER_CRITICAL

        /* insert gradient into global gradient list */
        gradient -> gx_gradient_next = _gx_system_gradient_list;

        if (_gx_system_gradient_list)
        {
            _gx_system_gradient_list -> gx_gradient_previous = gradient;
        }
        _gx_system_gradient_list = gradient;

        GX_EXIT_CRITICAL
    }

    return status;
}

