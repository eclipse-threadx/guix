import os
import sys
import argparse
import ctypes
import time
import test_utils

#dictionary of XLIFF versions: versions
xliff_versions = {
    'version_1.2':0,
    'version_2.0':1
}

#dictionary of string export format types
string_export_types = {
    'xliff':1,
    'csv':2
}

#dictionary of cpu types : values

cpu_types = {
    'generic':0,
    'synergy':1
    }
    
#define sort types of "test_add_widget" project
STRING_SORT_TYPE_NONE = "None"
STRING_SORT_TYPE_ID = "String ID"
STRING_SORT_TYPE_ENGLISH = "English"
STRING_SORT_TYPE_CHINESE = "Chinese"
STRING_SORT_TYPE_JAPANESE = "Japanese"
STRING_SORT_TYPE_REFERENCE_COUNT = "Reference Count"

    
#define border types
BORDER_TYPE_NONE     = "No Border"
BORDER_TYPE_THIN     = "Thin Border"
BORDER_TYPE_RAISED   = "Raised Border"
BORDER_TYPE_RECESSED = "Recessed Border"
BORDER_TYPE_THICK    = "Thick Border"

#define colors
COLOR_WHITE = "SELECTED_TEXT"
COLOR_BLACK = "TEXT"

#define pixelmaps
PIXELMAP_FISH = "FISH"
PIXELMAP_GAUGEMETER = "GAUGEMETER"
PIXELMAP_HISTORY = "I_HISTORY_LG"
PIXELMAP_NEEDLE = "NEEDLE"
PIXELMAP_ORANGE_BUTTON = "ORANGE_BUTTON"
PIXELMAP_SAVE_ICON = "SAVE_ICON"
PIXELMAP_FILL = "TFIELD_FILL_SMALL"
PIXELMAP_LEFT = "TFIELD_LEFT_SMALL"
PIXELMAP_RIGHT= "TFIELD_RIGHT_SMALL"
PIXELMAP_INDICATOR = "I_INDICATOR"
PIXELMAP_INDICATOR_HORIZONTAL = "I_INDICATOR_HORIZONTAL"
PIXELMAP_ORANGE_MIDDLE = "I_ORANGEFILL_MIDDLE"
PIXELMAP_ORANGE_MIDDLE_HORIZONTAL = "I_ORANGEFILL_MIDDLE_HORIZONTAL"
PIXELMAP_EMPTY_MIDDLE = "I_EMPTYFILL_MIDDLE"
PIXELMAP_EMPTY_MIDDLE_HORIZONTAL = "I_EMPTYFILL_MIDDLE_HORIZONTAL"

#define fonts
FONT_NOTOSANSHANS = "NotoSansHans"
FONT_VERA = "Vera"
FONT_VERABD = "VeraBd"
FONT_VERABI = "VeraBI"

#define menu status
MENU_ENABLED = 0
MENU_DISABLED = 1