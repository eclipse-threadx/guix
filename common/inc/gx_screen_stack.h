/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 Eclipse ThreadX contributors
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

