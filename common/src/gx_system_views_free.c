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
/**   System Management (System)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_system_views_free                               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function returns a list of views to the free list              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    head                                  Pointer to head of list to    */
/*                                            free                        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_all_views_free                    Release all views             */
/*                                                                        */
/**************************************************************************/

VOID _gx_system_views_free(GX_VIEW *head)
{
GX_VIEW *tail = head;

    /* find the tail end of this window's viewport list */
    while (tail -> gx_view_next)
    {
        tail = tail -> gx_view_next;
    }
    /* link last viewport to head of free list */
    tail -> gx_view_next = _gx_system_free_views;

    /* free list start becomes the list */
    _gx_system_free_views = head;
}

