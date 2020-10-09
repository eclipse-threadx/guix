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
#include "gx_system.h"

#if defined(GX_DYNAMIC_BIDI_TEXT_SUPPORT)
#if defined(GX_DYNAMIC_ARABIC_SHAPING_SUPPORT)
/* Define joining types. */
#define JOINING_TYPE_NONE        0x01
#define JOINING_TYPE_RIGHT       0x02
#define JOINING_TYPE_LEFT        0x03
#define JOINING_TYPE_DUAL        0x04
#define JOINING_TYPE_CAUSING     0x05
#define JOINING_TYPE_TRANSPARENT 0x06

#define ARABIC_FORM_ISOLATED     0x01
#define ARABIC_FORM_INITIAL      0x02
#define ARABIC_FORM_FINAL        0x03
#define ARABIC_FORM_MEDIAL       0x04

typedef struct ARABIC_SHAPING_STRUCT
{
    ULONG range_start;
    ULONG range_end;
    ULONG joining_type;
} ARABIC_SHAPING;

typedef struct ARABIC_FORM_STRUCT
{
    ULONG code_point;
    ULONG isolated;
    ULONG final;
    ULONG initial;
    ULONG medial;
} ARABIC_FORM;

typedef struct ARABIC_LIGATURE_STRICT
{
    ULONG alef;
    ULONG lam;
    ULONG ligature;
} ARABIC_LIGATURE;

typedef struct ARABIC_UNIT_STRUCT
{
    ULONG code_point;
    ULONG joining_type;
    ULONG shape_form;
} ARABIC_UNIT;

static ARABIC_SHAPING  arabic_shaping_table[] = {
    {0x0600, 0x060F, JOINING_TYPE_NONE},
    {0x0610, 0x061A, JOINING_TYPE_TRANSPARENT},
    {0x061B, 0x061B, JOINING_TYPE_NONE},
    {0x061C, 0x061C, JOINING_TYPE_TRANSPARENT},
    {0x061B, 0x061F, JOINING_TYPE_NONE},
    {0x0620, 0x0620, JOINING_TYPE_DUAL},
    {0x0621, 0x0621, JOINING_TYPE_NONE},
    {0x0622, 0x0625, JOINING_TYPE_RIGHT},
    {0x0626, 0x0626, JOINING_TYPE_DUAL},
    {0x0627, 0x0627, JOINING_TYPE_RIGHT},
    {0x0628, 0x0628, JOINING_TYPE_DUAL},
    {0x0629, 0x0629, JOINING_TYPE_RIGHT},
    {0x062A, 0x062E, JOINING_TYPE_DUAL},
    {0x062F, 0x0632, JOINING_TYPE_RIGHT},
    {0x0633, 0x063F, JOINING_TYPE_DUAL},
    {0x0640, 0x0640, JOINING_TYPE_CAUSING},
    {0x0641, 0x0647, JOINING_TYPE_DUAL},
    {0x0648, 0x0648, JOINING_TYPE_RIGHT},
    {0x0649, 0x064A, JOINING_TYPE_DUAL},
    {0x064B, 0x065F, JOINING_TYPE_TRANSPARENT},
    {0x0660, 0x066D, JOINING_TYPE_NONE},
    {0x066E, 0x066F, JOINING_TYPE_DUAL},
    {0x0670, 0x0670, JOINING_TYPE_TRANSPARENT},
    {0x0671, 0x0673, JOINING_TYPE_RIGHT},
    {0x0674, 0x0674, JOINING_TYPE_NONE},
    {0x0675, 0x0677, JOINING_TYPE_RIGHT},
    {0x0678, 0x0687, JOINING_TYPE_DUAL},
    {0x0688, 0x0699, JOINING_TYPE_RIGHT},
    {0x069A, 0x06BF, JOINING_TYPE_DUAL},
    {0x06C0, 0x06C0, JOINING_TYPE_RIGHT},
    {0x06C1, 0x06C2, JOINING_TYPE_DUAL},
    {0x06C3, 0x06CB, JOINING_TYPE_RIGHT},
    {0x06CC, 0x06CC, JOINING_TYPE_DUAL},
    {0x06CD, 0x06CD, JOINING_TYPE_RIGHT},
    {0x06CE, 0x06CE, JOINING_TYPE_DUAL},
    {0x06CF, 0x06CF, JOINING_TYPE_RIGHT},
    {0x06D0, 0x06D1, JOINING_TYPE_DUAL},
    {0x06D2, 0x06D3, JOINING_TYPE_RIGHT},
    {0x06D4, 0x06D4, JOINING_TYPE_NONE},
    {0x06D5, 0x06D5, JOINING_TYPE_RIGHT},
    {0x06D6, 0x06DC, JOINING_TYPE_TRANSPARENT},
    {0x06DD, 0x06DD, JOINING_TYPE_NONE},
    {0x06DF, 0x06E4, JOINING_TYPE_TRANSPARENT},
    {0x06E5, 0x06E6, JOINING_TYPE_NONE},
    {0x06E7, 0x06E8, JOINING_TYPE_TRANSPARENT},
    {0x06E9, 0x06E9, JOINING_TYPE_NONE},
    {0x06EA, 0x06ED, JOINING_TYPE_TRANSPARENT},
    {0x06EE, 0x06EF, JOINING_TYPE_RIGHT},
    {0x06FA, 0x06FC, JOINING_TYPE_DUAL},
    {0x06FF, 0x06FF, JOINING_TYPE_DUAL},
    {0, 0, 0}
};

static ARABIC_FORM     arabic_form_table_b[] = {
    /* code,  isolated, final, initial, medial */
    {0x0621, 0xFE80, 0xFE80, 0xFE80, 0xFE80},   /* HAMZA 0x0621*/
    {0x0622, 0xFE81, 0xFE82, 0xFE81, 0xFE82},   /* ALEF WITH MADDA ABOVE 0x622 */
    {0x0623, 0xFE83, 0xFE84, 0xFE83, 0xFE84},   /* ALEF WITH HAMZA ABOVE 0x0623*/
    {0x0624, 0xFE85, 0xFE86, 0xFE85, 0xFE86},   /* WAW WITH HAMZA ABOVE 0x0624 */
    {0x0625, 0xFE87, 0xFE88, 0xFE87, 0xFE88},   /* ALEF WITH HAMZA BELOW 0x0625*/
    {0x0626, 0xFE89, 0xFE8A, 0xFE8B, 0xFE8C},   /* YEH WITH HAMZA ABOVE 0x0626*/
    {0x0627, 0xFE8D, 0xFE8E, 0xFE8D, 0xFE8E},   /* ALEF 0x0627*/
    {0x0628, 0xFE8F, 0xFE90, 0xFE91, 0xFE92},   /* BEH 0x0628*/
    {0x0629, 0xFE93, 0xFE94, 0xFE93, 0xFE94},   /* TEH MARBUTA 0x0629*/
    {0x062A, 0xFE95, 0xFE96, 0xFE97, 0xFE98},   /* TEH 0x062A*/
    {0x062B, 0xFE99, 0xFE9A, 0xFE9B, 0xFE9C},   /* THEH 0x062B*/
    {0x062C, 0xFE9D, 0xFE9E, 0xFE9F, 0xFEA0},   /* JEEM 0x062C*/
    {0x062D, 0xFEA1, 0xFEA2, 0xFEA3, 0xFEA4},   /* HAH 0x062D*/
    {0x062E, 0xFEA5, 0xFEA6, 0xFEA7, 0xFEA8},   /* KHAH 0x062E*/
    {0x062F, 0xFEA9, 0xFEAA, 0xFEA9, 0xFEAA},   /* DAL 0x062F*/
    {0x0630, 0xFEAB, 0xFEAC, 0xFEAB, 0xFEAC},   /* THAL0x0630 */
    {0x0631, 0xFEAD, 0xFEAE, 0xFEAD, 0xFEAE},   /* RAA 0x0631*/
    {0x0632, 0xFEAF, 0xFEB0, 0xFEAF, 0xFEB0},   /* ZAIN 0x0632*/
    {0x0633, 0xFEB1, 0xFEB2, 0xFEB3, 0xFEB4},   /* SEEN 0x0633*/
    {0x0634, 0xFEB5, 0xFEB6, 0xFEB7, 0xFEB8},   /* SHEEN 0x0634*/
    {0x0635, 0xFEB9, 0xFEBA, 0xFEBB, 0xFEBC},   /* SAD 0x0635*/
    {0x0636, 0xFEBD, 0xFEBE, 0xFEBF, 0xFEC0},   /* DAD 0x0636*/
    {0x0637, 0xFEC1, 0xFEC2, 0xFEC3, 0xFEC4},   /* TAH 0x0637*/
    {0x0638, 0xFEC5, 0xFEC6, 0xFEC7, 0xFEC8},   /* ZAH 0x0638*/
    {0x0639, 0xFEC9, 0xFECA, 0xFECB, 0xFECC},   /* AIN 0x0639*/
    {0x063A, 0xFECD, 0xFECE, 0xFECF, 0xFED0},   /* GHAIN 0x063A*/
    {0x0641, 0xFED1, 0xFED2, 0xFED3, 0xFED4},   /* FAA 0x0641*/
    {0x0642, 0xFED5, 0xFED6, 0xFED7, 0xFED8},   /* QAF 0x0642*/
    {0x0643, 0xFED9, 0xFEDA, 0xFEDB, 0xFEDC},   /* KAF 0x0643*/
    {0x0644, 0xFEDD, 0xFEDE, 0xFEDF, 0xFEE0},   /* LAM 0x0644*/
    {0x0645, 0xFEE1, 0xFEE2, 0xFEE3, 0xFEE4},   /* MEEM 0x0645*/
    {0x0646, 0xFEE5, 0xFEE6, 0xFEE7, 0xFEE8},   /* NOON 0x0646*/
    {0x0647, 0xFEE9, 0xFEEA, 0xFEEB, 0xFEEC},   /* HEH 0x0647*/
    {0x0648, 0xFEED, 0xFEEE, 0xFEED, 0xFEEE},   /* WAW 0x0648*/
    {0x0649, 0xFEEF, 0xFEF0, 0xFBE8, 0xFBE9},   /* ALEF MAKSURA 0x0649*/
    {0x064A, 0xFEF1, 0xFEF2, 0xFEF3, 0xFEF4},   /* YEH 0x064A*/
    {0, 0, 0, 0, 0}
};

static ARABIC_FORM     arabic_form_table_a[] = {
    {0x0671, 0xFB50, 0xFB51, 0xFB50, 0xFB51}, /* Alef Wasla */
    {0x0679, 0xFB66, 0xFB67, 0xFB68, 0xFB89}, /* Tteh */
    {0x067A, 0xFB5E, 0xFB5F, 0xFB60, 0xFB61}, /* Tteheh */
    {0x067B, 0xFB52, 0xFB53, 0xFB54, 0xFB55}, /* Beeh */
    {0x067E, 0xFB56, 0xFB57, 0xFB58, 0xFB59}, /* Peh */
    {0x067F, 0xFB62, 0xFB63, 0xFB64, 0xFB65}, /* Teheh */
    {0x0680, 0xFB5A, 0xFB5B, 0xFB5C, 0xFB5D}, /* Beheh */
    {0x0683, 0xFB76, 0xFB77, 0xFB78, 0xFB79}, /* Nyeh */
    {0x0684, 0xFB72, 0xFB73, 0xFB74, 0xFB75}, /* Dyeh */
    {0x0686, 0xFB7A, 0xFB7B, 0xFB7C, 0xFB7D}, /* Tcheh */
    {0x0688, 0xFB88, 0xFB89, 0xFB88, 0xFB89}, /* Ddal */
    {0x0687, 0xFB7E, 0xFB7F, 0xFB80, 0xFB81}, /* Tcheheh */
    {0x068D, 0xFB82, 0xFB83, 0xFB82, 0xFB83}, /* Ddahal */
    {0x068E, 0xFB86, 0xFB87, 0xFB86, 0xFB87}, /* Dul */
    {0x0691, 0xFB8C, 0xFB8D, 0xFB8C, 0xFB8D}, /* Rreh */
    {0x0698, 0xFB8A, 0xFB8B, 0xFB8A, 0xFB8B}, /* Jeh */
    {0x06A4, 0xFB6A, 0xFB6B, 0xFB6C, 0xFB6D}, /* Veh */
    {0x06A6, 0xFB6E, 0xFB6F, 0xFB70, 0xFB71}, /* Peheh */
    {0x06A9, 0xFB8E, 0xFB8F, 0xFB90, 0xFB91}, /* Keheh */
    {0x06AD, 0xFBD3, 0xFBD4, 0xFBD5, 0xFBD6}, /* Ng */
    {0x06AF, 0xFB92, 0xFB93, 0xFB94, 0xFB95}, /* Gaf */
    {0x06B1, 0xFB9A, 0xFB9B, 0xFB9C, 0xFB9D}, /* Ngoeh */
    {0x06B3, 0xFB96, 0xFB97, 0xFB98, 0xFB99}, /* Gueh */
    {0x06BA, 0xFB9E, 0xFB9F, 0xFB9E, 0xFB9F}, /* Noon Ghunna */
    {0x06BB, 0xFBA0, 0xFBA1, 0xFBA2, 0xFBA3}, /* Rnoon */
    {0x06BE, 0xFBAA, 0xFBAB, 0xFBAC, 0xFBAD}, /* Heh Doachashmee */
    {0x06C1, 0xFBA6, 0xFBA7, 0xFBA8, 0xFBA9}, /* Heh Goal */
    {0x06C5, 0xFBE0, 0xFBE1, 0xFBE0, 0xFBE1}, /* Kirghiz Oe */
    {0x06C6, 0xFBD9, 0xFBDA, 0xFBD9, 0xFBDA}, /* Oe */
    {0x06C7, 0xFBD7, 0xFBD8, 0xFBD7, 0xFBD8}, /* U */
    {0x06C8, 0xFBDB, 0xFBDC, 0xFBDB, 0xFBDC}, /* Yu */
    {0x06C9, 0xFBE2, 0xFBE3, 0xFBE2, 0xFBE3}, /* Kirghiz Yu */
    {0x06CC, 0xFBFC, 0xFBFD, 0xFBFE, 0xFBFF}, /* Farsi Yeh */
    {0x06D0, 0xFBE4, 0xFBE5, 0xFBE6, 0xFBE7}, /* E */
    {0x06CB, 0xFBDE, 0xFBDF, 0xFBDE, 0xFBDF}, /* Ve */
    {0x06D2, 0xFBAE, 0xFBAF, 0xFBAE, 0xFBAF}, /* Yeh Barree */
    {0x06D3, 0xFBB0, 0xFBB1, 0xFBB0, 0xFBB1}  /* Yeh Barree with Hamza Above */
};

static ARABIC_LIGATURE arabic_ligature_table[] =
{
    /*alef, lam, (lam-alef) */
    {0xFE82, 0xFEDF, 0xFEF5},   /* Arabic Ligature Lam with Alef with Madda Above Isolated Form */
    {0xFE82, 0xFEE0, 0xFEF6},   /* Arabic Ligature Lam with Alef with Madda Above Final Form */
    {0xFE84, 0xFEDF, 0xFEF7},   /* Arabic Ligature Lam with Alef with Hamza Above Isolated Form */
    {0xFE84, 0xFEE0, 0xFEF8},   /* Arabic Ligature Lam with Alef with Hamza Above Final Form */
    {0xFE88, 0xFEDF, 0xFEF9},   /* Arabic Ligature Lam with Alef with Hamza Below Isolated Form */
    {0xFE88, 0xFEE0, 0xFEFA},   /* Arabic Ligature Lam with Alef with Hamza Below Final Form */
    {0xFE8E, 0xFEDF, 0xFEFB},   /* Arabic Ligature Lam with Alef Isolated Form */
    {0xFE8E, 0xFEE0, 0xFEFC},   /* Arabic Ligature Lam with Alef Final Form */
    {0, 0, 0}
};

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_arabic_joining_type_get            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function returns join type of sepecified unicode.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    code_point                            Code point                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    joining type                                                        */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    _gx_utility_bidi_arabic_shaping                                     */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
static UINT _gx_utility_bidi_arabic_joining_type_get(ULONG code_point)
{
ARABIC_SHAPING *shaping = arabic_shaping_table;

    if (code_point >= 0x0600 && code_point <= 0x06FF)
    {
        /* Arabic. */
        while (shaping -> range_start)
        {
            if ((code_point >= shaping -> range_start) &&
                (code_point <= shaping -> range_end))
            {
                return shaping -> joining_type;
            }
            shaping++;
        }
    }

    return JOINING_TYPE_NONE;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_arabic_form_get                    PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function gets Arabic shape form.                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    code_point                            Code point                    */
/*    form_type                             Joining type                  */
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
/*    _gx_utility_bidi_arabic_shaping                                     */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
static ULONG _gx_utility_bidi_arabic_form_get(ULONG code_point, UINT form_type)
{
ARABIC_FORM *entry = GX_NULL;

    if ((code_point) >= 0x0621 && (code_point <= 0x064A))
    {
        entry = arabic_form_table_b;
    }
    else if (code_point >= 0x0671 && code_point <= 0x06D3)
    {
        entry = arabic_form_table_a;
    }

    if (entry)
    {
        while (entry -> code_point)
        {
            if (entry -> code_point == code_point)
            {
                switch (form_type)
                {
                case ARABIC_FORM_ISOLATED:
                    return entry -> isolated;

                case ARABIC_FORM_INITIAL:
                    return entry -> initial;

                case ARABIC_FORM_FINAL:
                    return entry -> final;

                case ARABIC_FORM_MEDIAL:
                    return entry -> medial;
                }
            }

            entry++;
        }
    }

    return code_point;
}
/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_arabic_ligature_get                PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function retrieves Arabic ligature.                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    alef                                  Code of Arabic letter alef    */
/*    lam                                   Code of Arabic letter lam     */
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
/*    _gx_utility_bidi_arabic_shaping                                     */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
static ULONG _gx_utility_bidi_arabic_ligature_get(ULONG alef, ULONG lam)
{
ARABIC_LIGATURE *entry = arabic_ligature_table;

    while (entry -> ligature)
    {
        if (entry -> alef == alef && entry -> lam == lam)
        {
            return entry -> ligature;
        }
        entry++;
    }

    return 0;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_right_joining_causing_test         PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function tests if a join type is right joining causing.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    joining_type                          Joining type                  */
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
/*    _gx_utility_bidi_arabic_shaping                                     */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
static GX_BOOL _gx_utility_bidi_right_joining_causing_test(UINT joining_type)
{
    switch (joining_type)
    {
    case JOINING_TYPE_DUAL:
    case JOINING_TYPE_LEFT:
    case JOINING_TYPE_CAUSING:
        return GX_TRUE;
    }

    return GX_FALSE;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_left_joining_causing_test          PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function tests if a joing type is left joining causing.        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    joinint_type                          Joining type                  */
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
/*    _gx_utility_bidi_arabic_shaping                                     */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
static GX_BOOL _gx_utility_bidi_left_joining_causing_test(UINT joining_type)
{
    switch (joining_type)
    {
    case JOINING_TYPE_DUAL:
    case JOINING_TYPE_RIGHT:
    case JOINING_TYPE_CAUSING:
        return GX_TRUE;
    }

    return GX_FALSE;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_arabic_test                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function tests is the specified strings contains Arabic.       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
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
/*    _gx_utility_bidi_arabic_shaping                                     */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
static GX_BOOL _gx_utility_bidi_arabic_test(GX_BIDI_CONTEXT *context)
{
GX_BIDI_UNIT *unit = context -> gx_bidi_context_unit_list;
INT           index;

    for (index = 0; index < context -> gx_bidi_context_unit_count; index++)
    {
        if ((unit -> gx_bidi_unit_code >= 0x0600) && (unit -> gx_bidi_unit_code <= 0x06FF))
        {
            return GX_TRUE;
        }

        unit++;
    }

    return GX_FALSE;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_utility_bidi_arabic_shaping                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function shapes Arabic glyphs according to logical order.      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Bidi information control block*/
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
/*    _gx_utility_bidi_paragraph_reorder                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  09-30-2020     Kenneth Maxwell          Initial Version 6.1           */
/*                                                                        */
/**************************************************************************/
UINT _gx_utility_bidi_arabic_shaping(GX_BIDI_CONTEXT *context)
{
ARABIC_UNIT  *arabic_unit;
ARABIC_UNIT  *pre_unit;
ARABIC_UNIT  *unit;
ARABIC_UNIT  *follow_unit;
ULONG         ligature;
INT           index;
GX_BIDI_UNIT *bidi_unit;
GX_BIDI_UNIT *pre_bidi_unit;
GX_UBYTE      utf8[6];
UINT          glyph_len;
INT           increment;
UINT          form_type = 0;
INT           buffer_size;

    /* Test Arabic glyph. */
    if (_gx_utility_bidi_arabic_test(context) == GX_FALSE)
    {
        /* Nothing to change. */
        return GX_SUCCESS;
    }

    buffer_size = (context -> gx_bidi_context_unit_count + 1) * (INT)sizeof(ARABIC_UNIT);

    /* Allocate memory for Arabic shaping. */
    arabic_unit = (ARABIC_UNIT *)_gx_system_memory_allocator((UINT)buffer_size);

    if (arabic_unit == GX_NULL)
    {
        return GX_SYSTEM_MEMORY_ERROR;
    }

    /* Initiate Arabic shaping information list. */
    for (index = 0; index < context -> gx_bidi_context_unit_count; index++)
    {
        arabic_unit[index].code_point = context -> gx_bidi_context_unit_list[index].gx_bidi_unit_code;
        arabic_unit[index].shape_form = arabic_unit[index].code_point;
        arabic_unit[index].joining_type = _gx_utility_bidi_arabic_joining_type_get(arabic_unit[index].code_point);
    }

    arabic_unit[index].code_point = 0;
    arabic_unit[index].joining_type = 0;

    pre_unit = NULL;
    unit = arabic_unit;
    bidi_unit = context -> gx_bidi_context_unit_list;

    /* Start shaping. */
    while (unit -> code_point)
    {
        increment = 1;

        if (unit -> joining_type == JOINING_TYPE_TRANSPARENT)
        {
            /* Rule 1: Transparent characters do not affect the joining behavior of base characters. */
            unit++;
        }
        else
        {
            follow_unit = unit + 1;

            /* Find next non-transparent character. */
            while (follow_unit -> code_point && follow_unit -> joining_type == JOINING_TYPE_TRANSPARENT)
            {
                increment++;
                follow_unit++;
            }

            if (unit -> joining_type == JOINING_TYPE_RIGHT && pre_unit && _gx_utility_bidi_right_joining_causing_test(pre_unit -> joining_type))
            {
                /* Rule 2: a right-joining character x that has a right joining-causing character on the right(preceding) will
                   adopt the form x-right. */
                form_type = ARABIC_FORM_FINAL;
            }
            else if (unit -> joining_type == JOINING_TYPE_LEFT && _gx_utility_bidi_left_joining_causing_test(follow_unit -> joining_type))
            {
                /* Rule 3: a left-joining character x that has a left joining-causing character on the left(following) will
                   adopt the form x-left. */
                form_type = ARABIC_FORM_INITIAL;
            }
            else if (unit -> joining_type == JOINING_TYPE_DUAL &&
                     pre_unit && _gx_utility_bidi_right_joining_causing_test(pre_unit -> joining_type) &&
                     _gx_utility_bidi_left_joining_causing_test(follow_unit -> joining_type))
            {
                /* Rule 4: a dual-joining character x that has a right joining-causing character on the right(preceding) and
                   a left join-causing character on the left(following) will adopt the form x-medial. */
                form_type = ARABIC_FORM_MEDIAL;
            }
            else if (unit -> joining_type == JOINING_TYPE_DUAL &&
                     pre_unit && _gx_utility_bidi_right_joining_causing_test(pre_unit -> joining_type) &&
                     !_gx_utility_bidi_left_joining_causing_test(follow_unit -> joining_type))
            {
                /* Rule 5: a dual-joining character x that has a right join-causing character on the right(preceding) and
                   no left join-causing character on the left(following) will adopt the form x-right. */
                form_type = ARABIC_FORM_FINAL;
            }
            else if (unit -> joining_type == JOINING_TYPE_DUAL &&
                     _gx_utility_bidi_left_joining_causing_test(follow_unit -> joining_type) &&
                     ((pre_unit == GX_NULL) || !_gx_utility_bidi_right_joining_causing_test(pre_unit -> joining_type)))
            {
                /* Rule 6: a dual-joining character x that has left joining-causing character on the left(following) and
                   no right join-causing character on the right(preceding) will adopt the form x-left. */
                form_type = ARABIC_FORM_INITIAL;
            }

            if (form_type)
            {
                unit -> shape_form = _gx_utility_bidi_arabic_form_get(unit -> code_point, form_type);
                form_type = 0;

                if (pre_unit)
                {
                    /* Any sequence with alef-right on the left(following) and lam-medial on the right(preceding) will form
                       the ligature (lam-alef)-right. */
                    ligature = _gx_utility_bidi_arabic_ligature_get(unit -> shape_form, pre_unit -> shape_form);
                    if (ligature)
                    {
                        pre_unit -> shape_form = ligature;
                        pre_bidi_unit -> gx_bidi_unit_code = ligature;
                        unit -> shape_form = 0;
                    }
                }

                bidi_unit -> gx_bidi_unit_code = unit -> shape_form;
            }

            pre_bidi_unit = bidi_unit;
            pre_unit = unit;
            unit = follow_unit;
        }

        bidi_unit += increment;
    }

    /* Calculate total utf8 size for shaped glyphs. */
    context -> gx_bidi_context_reordered_utf8_size = 0;
    bidi_unit = context -> gx_bidi_context_unit_list;
    for (index = 0; index < context -> gx_bidi_context_unit_count; index++)
    {
        if (bidi_unit -> gx_bidi_unit_code)
        {
            _gx_utility_unicode_to_utf8(bidi_unit -> gx_bidi_unit_code, utf8, &glyph_len);
            context -> gx_bidi_context_reordered_utf8_size += glyph_len;
        }

        bidi_unit++;
    }

    _gx_system_memory_free(arabic_unit);

    return GX_SUCCESS;
}
#endif
#endif

