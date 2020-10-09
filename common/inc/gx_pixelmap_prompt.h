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
/**   Prompt Management (pixelmap prompt)                                 */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_pixelmap_prompt.h                                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX bitmap prompt management component,      */
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

#ifndef GX_PIXELMAP_PROMPT_H
#define GX_PIXELMAP_PROMPT_H

/* Define prompt management function prototypes.  */

VOID _gx_pixelmap_prompt_background_draw(GX_PIXELMAP_PROMPT *prompt);
UINT _gx_pixelmap_prompt_create(GX_PIXELMAP_PROMPT *prompt, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                GX_RESOURCE_ID text_id, GX_RESOURCE_ID fill_id,
                                ULONG style, USHORT pixelmap_prompt_id,
                                GX_CONST GX_RECTANGLE *size);
VOID _gx_pixelmap_prompt_draw(GX_PIXELMAP_PROMPT *prompt);
UINT _gx_pixelmap_prompt_pixelmap_set(GX_PIXELMAP_PROMPT *prompt,
                                      GX_RESOURCE_ID normal_left_id,
                                      GX_RESOURCE_ID normal_fill_id,
                                      GX_RESOURCE_ID normal_right_id,
                                      GX_RESOURCE_ID selected_left_id,
                                      GX_RESOURCE_ID selected_fill_id,
                                      GX_RESOURCE_ID selected_right_id);


UINT _gxe_pixelmap_prompt_create(GX_PIXELMAP_PROMPT *prompt, GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                 GX_RESOURCE_ID text_id, GX_RESOURCE_ID fill_id,
                                 ULONG style, USHORT pixelmap_prompt_id,
                                 GX_CONST GX_RECTANGLE *size, UINT pixelmap_prompt_control_block_size);
UINT _gxe_pixelmap_prompt_pixelmap_set(GX_PIXELMAP_PROMPT *prompt,
                                       GX_RESOURCE_ID normal_left_id,
                                       GX_RESOURCE_ID normal_fill_id,
                                       GX_RESOURCE_ID normal_right_id,
                                       GX_RESOURCE_ID selected_left_id,
                                       GX_RESOURCE_ID selected_fill_id,
                                       GX_RESOURCE_ID selected_right_id);
#endif

