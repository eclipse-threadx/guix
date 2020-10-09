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
/**   Scroll Management (Scrollbar)                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_scrollbar.h                                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX icon management component,               */
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

#ifndef GX_SCROLLBAR_H
#define GX_SCROLLBAR_H


/* Define scrollbar management function prototypes.  */

UINT _gx_horizontal_scrollbar_create(GX_SCROLLBAR *scrollbar, GX_CONST GX_CHAR *name, GX_WINDOW *parent,
                                     GX_SCROLLBAR_APPEARANCE *appearance, ULONG style);

UINT _gx_vertical_scrollbar_create(GX_SCROLLBAR *scrollbar, GX_CONST GX_CHAR *name, GX_WINDOW *parent,
                                   GX_SCROLLBAR_APPEARANCE *appearance, ULONG style);


VOID _gx_scrollbar_draw(GX_SCROLLBAR *scrollbar);
UINT _gx_scrollbar_event_process(GX_SCROLLBAR *scrollbar, GX_EVENT *event_ptr);
UINT _gx_scrollbar_limit_check(GX_SCROLLBAR *scrollbar);
VOID _gx_scrollbar_thumb_position_calculate(GX_SCROLLBAR *scroll);
UINT _gx_scrollbar_reset(GX_SCROLLBAR *scrollbar, GX_SCROLL_INFO *info);
VOID _gx_scrollbar_size_update(GX_SCROLLBAR *scroll);
VOID _gx_scrollbar_value_calculate(GX_SCROLLBAR *scroll, INT offset, INT size);
UINT _gx_scrollbar_value_set(GX_SCROLLBAR *scroll, INT value);

/* Define scroll thumb management function prototypes.  */

UINT _gx_scroll_thumb_create(GX_SCROLL_THUMB *scroll_thumb, GX_SCROLLBAR *parent, ULONG style);
VOID _gx_scroll_thumb_draw(GX_SCROLL_THUMB *scroll_thumb);
UINT _gx_scroll_thumb_event_process(GX_SCROLL_THUMB *scroll_thumb, GX_EVENT *event_ptr);
INT  _gx_scroll_thumb_shift_limit(GX_SCROLL_THUMB *thumb, INT shift);


/* Define error checking shells for API services.  These are only referenced by the
   application.  */

UINT _gxe_horizontal_scrollbar_create(GX_SCROLLBAR *scrollbar, GX_CONST GX_CHAR *name, GX_WINDOW *parent,
                                      GX_SCROLLBAR_APPEARANCE *appearance, ULONG style,
                                      UINT scrollbar_control_block_size);

UINT _gxe_vertical_scrollbar_create(GX_SCROLLBAR *scrollbar, GX_CONST GX_CHAR *name, GX_WINDOW *parent,
                                    GX_SCROLLBAR_APPEARANCE *appearance,
                                    ULONG style, UINT scrollbar_control_block_size);
UINT _gxe_scroll_thumb_event_process(GX_SCROLL_THUMB *scroll_thumb, GX_EVENT *event_ptr);
UINT _gxe_scroll_thumb_create(GX_SCROLL_THUMB *scroll_thumb, GX_SCROLLBAR *parent, ULONG style, UINT scroll_thumb_control_block_size);

UINT _gxe_scrollbar_event_process(GX_SCROLLBAR *scrollbar, GX_EVENT *event_ptr);
UINT _gxe_scrollbar_limit_check(GX_SCROLLBAR *scrollbar);
UINT _gxe_scrollbar_reset(GX_SCROLLBAR *scrollbar, GX_SCROLL_INFO *info);
UINT _gxe_scrollbar_value_set(GX_SCROLLBAR *scrollbar, INT value);
#endif

