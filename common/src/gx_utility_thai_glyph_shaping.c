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
#include "gx_system.h"
#include "gx_utility.h"

/* Define thai glyph class types. */
#ifdef GX_THAI_GLYPH_SHAPING_SUPPORT
#define GX_THAI_GLYPH_CLASS_BASE      0x01
#define GX_THAI_GLYPH_CLASS_BASE_DESC 0x02
#define GX_THAI_GLYPH_CLASS_BASE_ASC  0x03
#define GX_THAI_GLYPH_CLASS_TOP       0x04
#define GX_THAI_GLYPH_CLASS_LOWER     0x05
#define GX_THAI_GLYPH_CLASS_UPPER     0x06
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_thai_utf8_to_unicode                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function converts utf8 string to unicode and split sara am to  */
/*    nikhahit(0x0e4d) and sara aa(0x0e32).                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    string                                Pointer to string             */
/*    font                                  Font for text drawing         */
/*    textmap                               Pointer to pixemap structure  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_utf8_string_character_get Retrieve code value of utf8   */
/*                                           string                       */
/*    _gx_system_memory_allocator           Application defined memory    */
/*                                           allocator.                   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_utility_thai_glyph_shaping                                      */
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
#ifdef GX_THAI_GLYPH_SHAPING_SUPPORT
static UINT _gx_utility_thai_utf8_to_unicode(GX_CONST GX_STRING *string, GX_CHAR_CODE **returned_code_list, UINT *returned_code_count)
{
GX_CHAR_CODE  code_point;
UINT          ret;
GX_CHAR_CODE *code_list;
UINT          code_count = 0;
GX_STRING     string_copy = *string;
GX_BOOL       is_thai_string = GX_FALSE;

    while (string_copy.gx_string_length > 0)
    {
        ret = _gx_utility_utf8_string_character_get(&string_copy, &code_point, GX_NULL);
        if ((ret != GX_SUCCESS) || (code_point == 0))
        {
            return GX_FAILURE;
        }

        code_count++;

        if ((!is_thai_string) && (code_point >= 0xe00) && (code_point <= 0xe7f))
        {
            is_thai_string = GX_TRUE;
        }

        if (code_point == 0xe33)
        {
            /* The vowel sara am will be split into the character nikhahit and sara aa if it is
               appended to a consonant. */
            code_count++;
        }
    }

    if (!is_thai_string)
    {
        return GX_NO_CHANGE;
    }

    code_list = (GX_CHAR_CODE *)_gx_system_memory_allocator(sizeof(GX_CHAR_CODE) * code_count);
    if (!code_list)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    string_copy = *string;
    code_count = 0;

    while (string_copy.gx_string_length > 0)
    {
        _gx_utility_utf8_string_character_get(&string_copy, &code_point, GX_NULL);

        if (code_point == 0xe33)
        {
            code_list[code_count++] = 0xe4d;
            code_list[code_count++] = 0xe32;
        }
        else
        {
            code_list[code_count++] = code_point;
        }
    }

    *returned_code_list = code_list;
    *returned_code_count = code_count;

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_thai_glyph_class_type_get               PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function retrieves thai glyph class for specified unicode.     */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    code                                  Specified unicode             */
/*    return_type                           Retrieved thai glyph class    */
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
/*    _gx_utility_thai_glyph_shaping                                      */
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
#ifdef GX_THAI_GLYPH_SHAPING_SUPPORT
static UINT _gx_utility_thai_glyph_class_type_get(ULONG code, GX_UBYTE *return_type)
{
GX_UBYTE type;

    if (((code >= 0xe01) && (code <= 0xe2f)) ||
        (code == 0xe30) ||
        (code == 0xe40) ||
        (code == 0xe41))
    {
        if ((code == 0xe0e) || (code == 0xe0f))
        {
            type = GX_THAI_GLYPH_CLASS_BASE_DESC;
        }
        else if ((code == 0xe1b) || (code == 0xe1d) || (code == 0xe1f) || (code == 0xe2c))
        {
            type = GX_THAI_GLYPH_CLASS_BASE_ASC;
        }
        else
        {
            type = GX_THAI_GLYPH_CLASS_BASE;
        }
    }
    else if ((code >= 0xe48) && (code <= 0xe4c))
    {
        type = GX_THAI_GLYPH_CLASS_TOP;
    }
    else if ((code >= 0xe38) && (code <= 0xe3a))
    {
        type = GX_THAI_GLYPH_CLASS_LOWER;
    }
    else if ((code == 0xe31) || ((code >= 0xe34) && (code <= 0xe37)) ||
             (code == 0xe47) || (code == 0xe4d))
    {
        type = GX_THAI_GLYPH_CLASS_UPPER;
    }
    else
    {
        type = 0;
    }

    *return_type = type;

    return GX_SUCCESS;
}
#endif

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_thai_glyph_shaping                      PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function adjusts thai glyph position by substitution           */
/*    technique with extra glyph sets that defined in Private Use Area.   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    string                                Thai string for shaping       */
/*    count                                 String size in bytes          */
/*    returned_code_list                    Adjusted unicode list         */
/*    returned_code_count                   Adjusted unicode count        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_thai_utf8_to_unicode      Convert utf8 string to unicode*/
/*    _gx_utility_thai_glyph_class_type_get Retrieve thai glyph class     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Software                                                */
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
#ifdef GX_THAI_GLYPH_SHAPING_SUPPORT
UINT _gx_utility_thai_glyph_shaping(GX_CONST GX_STRING *string, GX_CHAR_CODE **returned_code_list, UINT *returned_code_count)
{
/****************************************************/
/*                  Extra Glyph Sets                */
/* F700:     uni0E10.descless    (base.descless)    */
/* F701~04:  uni0E34~37.left     (upper.left)       */
/* F705~09:  uni0E48~4C.lowleft  (top.lowleft)      */
/* F70A~0E:  uni0E48~4C.low      (top.low)          */
/* F70F:     uni0E0D.descless    (base.descless)    */
/* F710~12:  uni0E31,4D,47.left  (upper.left)       */
/* F713~17:  uni0E48~4C.left     (top.left)         */
/* F718~1A:  uni0E38~3A.low      (lower.low)        */
/****************************************************/
GX_CHAR_CODE *code_list;
UINT          code_count;
UINT          status;
UINT          index;
GX_UBYTE      type;
GX_UBYTE      pp_type = 0;
GX_UBYTE      p_type = 0;
GX_UBYTE      f_type;
GX_UBYTE      t;
GX_BOOL       f_nikhahit;

    /* Convert utf8 string to unicode string. */
    status = _gx_utility_thai_utf8_to_unicode(string, &code_list, &code_count);

    if (status == GX_SUCCESS)
    {
        _gx_utility_thai_glyph_class_type_get(code_list[0], &type);

        for (index = 0; index < code_count; index++)
        {
            if (index < code_count - 1)
            {
                _gx_utility_thai_glyph_class_type_get(code_list[index + 1], &f_type);
            }
            else
            {
                f_type = 0;
            }

            if ((type == GX_THAI_GLYPH_CLASS_TOP) && index)
            {
                /* base + top --> base + top.low. */
                /* base + lower + top -> base + lower + top.low. */
                /* base.asc + top --> base.asc + top.lowleft. */
                /* base.asc + lower + top --> base.asc + lower + top.lowleft. */
                if (p_type == GX_THAI_GLYPH_CLASS_LOWER)
                {
                    t = pp_type;
                }
                else
                {
                    t = p_type;
                }

                if ((t == GX_THAI_GLYPH_CLASS_BASE) ||
                    (t == GX_THAI_GLYPH_CLASS_BASE_ASC) ||
                    (t == GX_THAI_GLYPH_CLASS_BASE_DESC))
                {
                    if ((index < code_count - 1) && (code_list[index + 1] == 0xe4d))
                    {
                        f_nikhahit = GX_TRUE;
                    }
                    else
                    {
                        f_nikhahit = GX_FALSE;
                    }

                    if (t == GX_THAI_GLYPH_CLASS_BASE_ASC)
                    {
                        if (f_nikhahit)
                        {
                            /* base.asc + top + sara am --> base.asc + nikhahit.left + top.left + sara aa. */
                            code_list[index + 1] = (GX_CHAR_CODE)(code_list[index] - 0xe48 + 0xf713);
                            code_list[index] = 0xf711;
                            code_list[index + 2] = 0xe32;
                            index += 2;
                            pp_type = 0;
                            p_type = 0;
                            type = 0;
                            continue;
                        }
                        else
                        {
                            /* base.asc + top --> base.asc + top.lowerleft. */
                            code_list[index] = (GX_CHAR_CODE)(code_list[index] - 0xe48 + 0xf705);
                        }
                    }
                    else
                    {
                        if (!f_nikhahit)
                        {
                            /* base + top --> base + top.low */
                            code_list[index] = (GX_CHAR_CODE)(code_list[index] - 0xe48 + 0xf70a);
                        }
                    }
                }

                if ((pp_type == GX_THAI_GLYPH_CLASS_BASE_ASC) && (p_type == GX_THAI_GLYPH_CLASS_UPPER))
                {
                    /* base.asc + upper + top --> base.asc + upper + top.left. */
                    code_list[index] = (GX_CHAR_CODE)(code_list[index] - 0xe48 + 0xf713);
                }
            }
            else if ((p_type == GX_THAI_GLYPH_CLASS_BASE_ASC) && (type == GX_THAI_GLYPH_CLASS_UPPER))
            {
                /* base.asc + upper --> base.asc + upper.left. */
                switch (code_list[index])
                {
                case 0xe31:
                    code_list[index] = 0xf710;
                    break;
                case 0xe34:
                case 0xe35:
                case 0xe36:
                case 0xe37:
                    code_list[index] = (GX_CHAR_CODE)(code_list[index] - 0xe34 + 0xf701);
                    break;
                case 0xe4d:
                    code_list[index] = 0xf711;
                    break;
                case 0xe47:
                    code_list[index] = 0xf712;
                    break;
                }
            }
            else if ((p_type == GX_THAI_GLYPH_CLASS_BASE_DESC) && (type == GX_THAI_GLYPH_CLASS_LOWER))
            {
                /* base.desc + lower --> base.desc + lower.low. */
                code_list[index] = (GX_CHAR_CODE)(code_list[index] - 0xe38 + 0xf718);
            }
            else if ((code_list[index] == 0xe0d) && (f_type == GX_THAI_GLYPH_CLASS_LOWER))
            {
                /* yong ying drop its lower part if combined with a lower vowel. */
                /* base.desclike + lower --> base.descless. */
                code_list[index] = 0xf70f;
            }
            else if ((code_list[index] == 0xe10) && (f_type == GX_THAI_GLYPH_CLASS_LOWER))
            {
                /* tho than drop its lower part if combined with a lower vowel. */
                /* base.desclike + lower --- base.descless. */
                code_list[index] = 0xf700;
            }

            pp_type = p_type;
            p_type = type;
            type = f_type;
        }

        *returned_code_list = code_list;
        *returned_code_count = code_count;
    }
    else
    {
        *returned_code_list = GX_NULL;
        *returned_code_count = 0;
    }

    return status;
}
#endif

