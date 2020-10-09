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
#include "gx_utility.h"

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
static GX_CONST GX_BIDI_BRACKET_PAIR _gx_bidi_bracket_pair[] =
{
    {0x0028, 0x0029},    /* PARENTHESIS */
    {0x005B, 0x005D},    /* SQUARE BRACKET*/
    {0x007B, 0x007D},    /* CURLY BRACKET*/
    {0x0F3A, 0x0F3B},    /* TIBETAN MARK GUG RTAGS GYON*/
    {0x0F3C, 0x0F3D},    /* TIBETAN MARK ANG KHANG GYON*/
    {0x169B, 0x169C},    /* OGHAM FEATHER MARK*/
    {0x2045, 0x2046},    /* SQUARE BRACKET WITH QUILL*/
    {0x207D, 0x207E},    /* SUPERSCRIPT PARENTHESIS*/
    {0x208D, 0x208E},    /* SUBSCRIPT PARENTHESIS*/
    {0x2308, 0x2309},    /* CEILING*/
    {0x230A, 0x230B},    /* FLOOR*/
    {0x2329, 0x232A},    /* POINTING ANGLE BRACKET*/
    {0x2768, 0x2769},    /* MEDIUM PARENTHESIS ORNAMENT*/
    {0x276A, 0x276B},    /* MEDIUM FLATTENED PARENTHESIS ORNAMENT*/
    {0x276C, 0x276D},    /* MEDIUM - POINTING ANGLE BRACKET ORNAMENT*/
    {0x276E, 0x276F},    /* HEAVY - POINTING ANGLE QUOTATION MARK ORNAMENT*/
    {0x2770, 0x2771},    /* HEAVY - POINTING ANGLE BRACKET ORNAMENT*/
    {0x2772, 0x2773},    /* LIGHT TORTOISE SHELL BRACKET ORNAMENT*/
    {0x2774, 0x2775},    /* MEDIUM CURLY BRACKET ORNAMENT*/
    {0x27C5, 0x27C6},    /* LEFT S - SHAPED BAG DELIMITER*/
    {0x27E6, 0x27E7},    /* MATHEMATICAL WHITE SQUARE BRACKET*/
    {0x27E8, 0x27E9},    /* MATHEMATICAL ANGLE BRACKET*/
    {0x27EA, 0x27EB},    /* MATHEMATICAL DOUBLE ANGLE BRACKET*/
    {0x27EC, 0x27ED},    /* MATHEMATICAL WHITE TORTOISE SHELL BRACKET*/
    {0x27EE, 0x27EF},    /* MATHEMATICAL FLATTENED PARENTHESIS*/
    {0x2983, 0x2984},    /* WHITE CURLY BRACKET*/
    {0x2985, 0x2986},    /* WHITE PARENTHESIS*/
    {0x2987, 0x2988},    /* Z NOTATION IMAGE BRACKET*/
    {0x2989, 0x298A},    /* Z NOTATION BINDING BRACKET*/
    {0x298B, 0x298C},    /* SQUARE BRACKET WITH UNDERBAR*/
    {0x298D, 0x2990},    /* SQUARE BRACKET WITH TICK IN TOP CORNER*/
    {0x298F, 0x298E},    /* SQUARE BRACKET WITH TICK IN BOTTOM CORNER*/
    {0x2991, 0x2992},    /* ANGLE BRACKET WITH DOT*/
    {0x2993, 0x2994},    /* ARC LESS - THAN BRACKET*/
    {0x2995, 0x2996},    /* DOUBLE ARC GREATER - THAN BRACKET*/
    {0x2997, 0x2998},    /* BLACK TORTOISE SHELL BRACKET*/
    {0x29D8, 0x29D9},    /* WIGGLY FENCE*/
    {0x29DA, 0x29DB},    /* DOUBLE WIGGLY FENCE*/
    {0x29FC, 0x29FD},    /* POINTING CURVED ANGLE BRACKET*/
    {0x2E22, 0x2E23},    /* TOP HALF BRACKET*/
    {0x2E24, 0x2E25},    /* BOTTOM HALF BRACKET*/
    {0x2E26, 0x2E27},    /* SIDEWAYS U BRACKET*/
    {0x2E28, 0x2E29},    /* DOUBLE PARENTHESIS*/
    {0x3008, 0x3009},    /* ANGLE BRACKET*/
    {0x300A, 0x300B},    /* DOUBLE ANGLE BRACKET*/
    {0x300C, 0x300D},    /* CORNER BRACKET*/
    {0x300E, 0x300F},    /* WHITE CORNER BRACKET*/
    {0x3010, 0x3011},    /* BLACK LENTICULAR BRACKET*/
    {0x3014, 0x3015},    /* TORTOISE SHELL BRACKET*/
    {0x3016, 0x3017},    /* WHITE LENTICULAR BRACKET*/
    {0x3018, 0x3019},    /* WHITE TORTOISE SHELL BRACKET*/
    {0x301A, 0x301B},    /* WHITE SQUARE BRACKET*/
    {0xFE59, 0xFE5A},    /* SMALL PARENTHESIS*/
    {0xFE5B, 0xFE5C},    /* SMALL CURLY BRACKET*/
    {0xFE5D, 0xFE5E},    /* SMALL TORTOISE SHELL BRACKET*/
    {0xFF08, 0xFF09},    /* FULLWIDTH PARENTHESIS*/
    {0xFF3B, 0xFF3D},    /* FULLWIDTH SQUARE BRACKET*/
    {0xFF5B, 0xFF5D},    /* FULLWIDTH CURLY BRACKET*/
    {0xFF5F, 0xFF60},    /* FULLWIDTH WHITE PARENTHESIS*/
    {0xFF62, 0xFF63},    /* HALFWIDTH CORNER BRACKET*/
    {0, 0}
};

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_bracket_pair_get                   PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function finds the bracked pair that contain the specified     */
/*    code point.                                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    code                                  Code point whose bracket pair */
/*                                            is retrieved                */
/*    bracket_pair                          Retrieved bracket pair        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
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
UINT _gx_utility_bidi_bracket_pair_get(ULONG code, GX_BIDI_BRACKET_PAIR *bracket_pair)
{
GX_CONST GX_BIDI_BRACKET_PAIR *entry = _gx_bidi_bracket_pair;

    memset(bracket_pair, 0, sizeof(GX_BIDI_BRACKET_PAIR));

    while (entry -> gx_bidi_bracket_pair_open)
    {
        if (code == entry -> gx_bidi_bracket_pair_open ||
            code == entry -> gx_bidi_bracket_pair_close)
        {
            *bracket_pair = *entry;
            break;
        }
        entry++;
    }

    return GX_SUCCESS;
}
#endif
