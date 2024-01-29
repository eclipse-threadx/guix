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
/**   Screen Stack Management (Screen Stack)                              */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_screen_stack.h                                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX screen stack component, including all    */
/*    data types and external references.  It is assumed that gx_api.h    */
/*    and gx_port.h have already been included.                           */
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

#ifndef GX_SCREEN_STACK_H
#define GX_SCREEN_STACK_H

/* Define screen stack management function prototypes.  */
UINT _gx_screen_stack_create(GX_SCREEN_STACK_CONTROL *control, GX_WIDGET **memory, INT size);
UINT _gx_screen_stack_push(GX_SCREEN_STACK_CONTROL *control, GX_WIDGET *screen, GX_WIDGET *new_screen);
UINT _gx_screen_stack_pop(GX_SCREEN_STACK_CONTROL *control);
UINT _gx_screen_stack_reset(GX_SCREEN_STACK_CONTROL *control);

/* error checking versions of the screen stack API functions */
UINT _gxe_screen_stack_create(GX_SCREEN_STACK_CONTROL *control, GX_WIDGET **memory, INT size);
UINT _gxe_screen_stack_push(GX_SCREEN_STACK_CONTROL *control, GX_WIDGET *screen, GX_WIDGET *new_screen);
UINT _gxe_screen_stack_pop(GX_SCREEN_STACK_CONTROL *control);
UINT _gxe_screen_stack_reset(GX_SCREEN_STACK_CONTROL *control);

#endif

