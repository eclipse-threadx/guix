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
/**   Cursor Management (Cursor)                                          */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_text_input_cursor.h                              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX cursor management component,             */
/*    including all data types and external references.  It is assumed    */
/*    that gx_api.h and gx_port.h have already been included.             */
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

#ifndef GX_CURSOR_H
#define GX_CURSOR_H


/* Define cursor management function prototypes.  */

VOID _gx_text_input_cursor_draw(GX_TEXT_INPUT_CURSOR *cursor_input);
UINT _gx_text_input_cursor_dirty_rectangle_get(GX_TEXT_INPUT_CURSOR *cursor_ptr, GX_RECTANGLE *dirty_rect);
UINT _gx_text_input_cursor_width_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE width);
UINT _gx_text_input_cursor_height_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE height);
UINT _gx_text_input_cursor_blink_interval_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE blink_interval);


/* Define error checking shells for API services.  These are only referenced by the
   application.  */

UINT _gxe_text_input_cursor_width_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE width);
UINT _gxe_text_input_cursor_height_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE height);
UINT _gxe_text_input_cursor_blink_interval_set(GX_TEXT_INPUT_CURSOR *cursor_input, GX_UBYTE blink_interval);

#endif

