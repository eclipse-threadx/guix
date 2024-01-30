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
/**   Numeric Pixelmap Prompt Management (Prompt)                         */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    gx_numeric_pixelmap_prompt.h                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the GUIX numeric pixelmap prompt management       */
/*    component, including all data types and external references.  It is */
/*    assumed that gx_api.h and gx_port.h have already been included.     */
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

#ifndef GX_NUMERIC_PIXELMAP_PROMPT_H
#define GX_NUMERIC_PIXELMAP_PROMPT_H


/* Define numeric pixelmap prompt management function prototypes.  */

UINT _gx_numeric_pixelmap_prompt_create(GX_NUMERIC_PIXELMAP_PROMPT *prompt,
                                        GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                        GX_RESOURCE_ID text_id, GX_RESOURCE_ID fill_id,
                                        ULONG style, USHORT pixelmap_prompt_id,
                                        GX_CONST GX_RECTANGLE *size);
VOID _gx_numeric_pixelmap_prompt_format(GX_NUMERIC_PIXELMAP_PROMPT *prompt, INT value);
UINT _gx_numeric_pixelmap_prompt_format_function_set(GX_NUMERIC_PIXELMAP_PROMPT *prompt, VOID (*format_func)(GX_NUMERIC_PIXELMAP_PROMPT *, INT));
UINT _gx_numeric_pixelmap_prompt_value_set(GX_NUMERIC_PIXELMAP_PROMPT *prompt, INT value);

/* Define error checking shells for API services.  These are only referenced by the
   application.  */

UINT _gxe_numeric_pixelmap_prompt_create(GX_NUMERIC_PIXELMAP_PROMPT *prompt,
                                         GX_CONST GX_CHAR *name, GX_WIDGET *parent,
                                         GX_RESOURCE_ID text_id, GX_RESOURCE_ID fill_id,
                                         ULONG style, USHORT pixelmap_prompt_id,
                                         GX_CONST GX_RECTANGLE *size,
                                         UINT control_block_size);
UINT _gxe_numeric_pixelmap_prompt_format_function_set(GX_NUMERIC_PIXELMAP_PROMPT *prompt, VOID (*format_func)(GX_NUMERIC_PIXELMAP_PROMPT *, INT));
UINT _gxe_numeric_pixelmap_prompt_value_set(GX_NUMERIC_PIXELMAP_PROMPT *prompt, INT value);
#endif

