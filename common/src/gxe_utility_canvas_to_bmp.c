/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Text Input Management (Single Line Text Input)                      */
/**                                                                       */
/**************************************************************************/

/* Include necessary system files.  */
#define GX_SOURCE_CODE

#include "gx_api.h"
#include "gx_utility.h"

/* Bring in externs for caller checking code.  */
GX_CALLER_CHECKING_EXTERNS
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_canvas_to_bmp                           PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Covert canvas memory data to bmp file.                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    canvas                                Canvas control block          */
/*    rectangle                             Rectangle specification       */
/*    write_data                            Write data callback function  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_canvas_to_bmp             Actual function which do this */
/*                                            convert.                    */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application                                                         */
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
UINT _gxe_utility_canvas_to_bmp(GX_CANVAS *canvas, GX_RECTANGLE *rect, UINT (*write_data)(GX_UBYTE *byte_data, UINT data_count))
{

    /* Check for appropriate caller.  */
    GX_INIT_AND_THREADS_CALLER_CHECKING

    if ((canvas == GX_NULL) ||
        (canvas -> gx_canvas_display == GX_NULL) ||
        (canvas -> gx_canvas_memory == NULL) ||
        (rect == GX_NULL) ||
        (write_data == GX_NULL))
    {
        return GX_PTR_ERROR;
    }
    
    return _gx_utility_canvas_to_bmp(canvas, rect, write_data);
}
