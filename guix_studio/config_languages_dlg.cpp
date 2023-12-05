// config_languages_dlg.cpp : implementation file
//

#include "studiox_includes.h"
#include "config_languages_dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ISO 639-1 three-letter language codes
// http://en.wikipedia.org/wiki/List_of_ISO_639-1_codes
// http://www.loc.gov/standards/iso639-2/php/code_list.php

typedef struct {
    char *name;
    char *symbol;
} language_record;

language_record language_list[] = {
    {"Abkhaz",      "ab"},
    {"Afar",        "aa"},
    {"Afrikaans",   "af"},
    {"Akan",        "ak"},
    {"Albanian",    "sq"},
    {"Amharic",     "am"},
    {"Arabic",      "ar"},
    {"Aragonese",   "an"},
    {"Armenian",    "hy"},
    {"Assamese",    "as"}, 
    {"Avaric",      "av"},
    {"Avestan",     "ae"},
    {"Aymara",      "ay"},
    {"Azerbaijani", "az"},
    {"Bamanankan",  "bm"},
    {"Bangla",      "bn"},
    {"Bashkir",     "ba"},
    {"Basque",      "eu"},
    {"Belarusian",  "be"},
    {"Bihari",      "bh"},
    {"Bislama",     "bi"},
    {"Bosnian",     "bs"},
    {"Breton",      "br"},
    {"Bulgarian",   "bg"},
    {"Burmese",     "my"},
    {"Catalan",     "ca"},
    {"Chamorro",    "ch"},
    {"Chechen",     "ce"},
    {"Chichewa",    "ny"},
    {"Chinese",     "zh"},
    {"Church Slavic", "cu"},
    {"Chuvash",     "cv"},
    {"Cornish",     "kw"},
    {"Corsican",    "co"},
    {"Cree",        "cr"},
    {"Croatian",    "hr"},
    {"Czech",       "cs"},
    {"Danish",      "da"},
    {"Divehi",      "dv"},
    {"Dutch",       "nl"},
    {"Dzongkha",    "dz"},
    {"English",     "en"},
    {"Esperanto",   "eo"},
    {"Estonian",    "et"},
    {"Ewe",         "ee"},
    {"Faroese",     "fo"},
    {"Fijian",      "fj"},
    {"Finnish",     "fi"},
    {"French",      "fr"},
    {"Frisian",     "fy"},
    {"Fulah",       "ff"},
    {"Galician",    "gl"},
    {"Ganda",       "lg"},
    {"Georgian",    "ka"},
    {"German",      "de"},
    {"Greek",       "el"},
    {"Guarani",     "gn"},
    {"Gujarati",    "gu"},
    {"Haitian",     "ht"},
    {"Hausa",       "ha"},
    {"Hebrew",      "he"},
    {"Herero",      "hz"},
    {"Hindi",       "hi"},
    {"Hiri Motu",   "ho"},
    {"Hungarian",   "hu"},
    {"Icelandic",   "is"},
    {"Ido",         "io"},
    {"Igbo",        "ig"},
    {"Indonesian",  "id"},
    {"Interlingue", "ie"},
    {"Interlingua", "ia"},
    {"Inuktitut",   "iu"},
    {"Inupiaq",     "ik"},
    {"Irish",       "ga"},
	{"Italian",     "it"},
    {"Japanese",    "ja"},
    {"Javanese",    "jv"},
    {"Kalaallisut", "kl"},
    {"Kannada",     "kn"},
    {"Kanuri",      "kr"},
    {"Kashmiri",    "ks"},
    {"Kazakh",      "kk"},
    {"Khmer",       "km"},
    {"Kikuyu",      "ki"},
    {"Kinyarwanda", "rw"},
    {"Kirghiz",     "ky"},
    {"Komi",        "kv"},
    {"Kongo",       "kg"},
    {"Korean",      "ko"},
    {"Kuanyama",    "kj"},
    {"Kurdish",     "ku"},
    {"Lao",         "lo"},
    {"Latin",       "la"},
    {"Latvian",     "lv"},
    {"Limburgan",   "li"},
    {"Lingala",     "ln"},
    {"Lithuanian",  "lt"},
    {"Luba-Katanga", "lu"},
    {"Luxembourgish", "lb"},
    {"Macedonian",  "mk"},
    {"Malagasy",    "mg"},
    {"Malay",       "ms"},
    {"Malayalam",   "ml"},
    {"Maltese",     "mt"},
    {"Manx",        "gv"},
    {"Maori",       "mi"},
    {"Marathi",     "mr"},
    {"Marshallese", "mh"},
    {"Mongolian",   "mn"},
    {"Nauru",       "na"},
    {"Navajo",      "nv"},
    {"Ndebele",     "nd"},
    {"Ndonga",      "ng"},
    {"Nepali",      "ne"},
    {"Norwegian",   "no"},
    {"Occitan",     "oc"},
    {"Odia",        "or"},
    {"Ojibwa",      "oj"},
    {"Oromo",       "om"},
    {"Ossetian",    "os"},
    {"Pali",        "pi"},
    {"Panjabi",     "pa"},
    {"Persian",     "fa"},
    {"Polish",      "pl"},
    {"Portuguese",  "pt"},
    {"Pushto",      "ps"},
    {"Quechua",     "qu"},
    {"Romanian",    "ro"},
    {"Romansh",     "rm"},
    {"Rundi",       "rn"},
    {"Russian",     "ru"},
    {"Samoan",      "sm"},
    {"Sango",       "sg"},
    {"Sanskrit",    "sa"},
    {"Sardinian",   "sc"},
    {"Scottish Gaelic", "gd"},
    {"Serbian",     "sr"},
    {"Shona",       "sn"},
    {"Sindhi",      "sd"},
    {"Sinhala",     "si"},
    {"Slovak",      "sk"},
    {"Slovenian",   "sl"},
    {"Somali",      "so"},
    {"Sotho",       "st"},
    {"Spanish",     "es"},
    {"Sundanese",   "su"},
    {"Swahili",     "sw"},
    {"Swati",       "ss"},
    {"Swedish",     "sv"},
    {"Tagalog",     "tl"},
    {"Tahitian",    "ty"},
    {"Tajik",       "tg"},
    {"Tamil",       "ta"}, 
    {"Tatar",       "tt"},
    {"Telugu",      "te"},
    {"Thai",        "th"},
    {"Tibetan",     "bo"},
    {"Tigrinya",    "ti"},
    {"Tonga",       "to"},
    {"Tswana",      "tn"},
    {"Tsonga",      "ts"},
    {"Turkmen",     "tk"},
    {"Turkish",     "tr"},	
    {"Twi",         "tw"}, 
    {"Uighur",      "ug"},
    {"Ukrainian",   "uk"},
    {"Urdu",        "ur"},
    {"Uzbek",       "uz"},
    {"Venda",       "ve"},
    {"Vietnamese",  "vi"},
    {"Volap?",     "vo"},
    {"Walloon",     "wa"},
    {"Welsh",       "cy"},
    {"Wolof",       "wo"},
    {"Xhosa",       "xh"},
    {"Yi",          "ii"},
    {"Yiddish",     "yi"},
    {"Yoruba",      "yo"},
    {"Zhuang",      "za"},
    {"Zulu",        "zu"},
    {NULL, NULL}
};

typedef struct name_convert_map_struct {
    char *old_name;
    char *new_name;
}name_convert_map;

name_convert_map language_name_convert_list[] = {
    {"Bambara", "Bamanankan"},
    {"Bengali", "Bangla"},
    {"Gaelic", "Scottish Gaelic"},
    {"Oriya", "Odia"},
    {"Sichuan Yi", "Yi"},
    {NULL, NULL}
};

enum config_languages_dlg_test_commands{
    TEST_ADD_LANGUAGE = 1,
    TEST_DELETE_LANGEUGE,
    TEST_SELECT_LANGUAGE_INDEX,
    TEST_SELECT_LANGUAGE_ID,
    TEST_CHECK_SUPPORT_BIDI_TEXT,
    TEST_CHECK_REORDER_BIDI_TEXT,
    TEST_CHECK_SUPPORT_THAI_SHAPING,
    TEST_CHECK_GEN_ADJUSTED_THAI,
    TEST_SAVE_LANGUAGE_CONFIGURE,
    TEST_CANCEL_LANGUAGE_CONFIGURE
};

// config_languages_dlg dialog

#define LANGUAGE_LIST_SIZE ((sizeof(language_list) / sizeof(language_record)) - 1)

IMPLEMENT_DYNAMIC(config_languages_dlg, express_dialog)

///////////////////////////////////////////////////////////////////////////////
config_languages_dlg::config_languages_dlg(CWnd* pParent /*=NULL*/)
    : express_dialog(config_languages_dlg::IDD, pParent)
{
    IconId = IDB_CONFIGURE_LANGUAGES;
    SetTitleText("Configure Languages");
    studiox_project *project = GetOpenProject();

    if (project)
    {
        mNumLanguages = project->mHeader.num_languages;
        mStartLanguages = mNumLanguages;

        if (mNumLanguages <= 0)
        {
            mNumLanguages = 1;
        }
        mLanguages.SetSize(mNumLanguages, 1);

        for (int index = 0; index < mNumLanguages; index++)
        {
            mLanguages[index].name = project->mHeader.languages[index].name;
            mLanguages[index].support_bidi_text = project->mHeader.languages[index].support_bidi_text;
            mLanguages[index].gen_reordered_bidi_text = project->mHeader.languages[index].gen_reordered_bidi_text;

            mLanguages[index].support_thai_glyph_shaping = project->mHeader.languages[index].support_thai_glyph_shaping;
            mLanguages[index].gen_adjusted_thai_string = project->mHeader.languages[index].gen_adjusted_thai_string;

            mLanguages[index].statically_defined = project->mHeader.languages[index].statically_defined;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
config_languages_dlg::~config_languages_dlg()
{
}

///////////////////////////////////////////////////////////////////////////////
CString config_languages_dlg::BuildCombinedLanguageName(char *name, char *symbol)
{
    CString combined;
    CString tname = CA2T(name);
    CString tsymbol = CA2T(symbol);

    combined.Format(_T("%s [%s]"), tname, tsymbol);
    return combined;
}

///////////////////////////////////////////////////////////////////////////////
CString config_languages_dlg::BuildCombinedLanguageName(int index)
{
    if (language_list[index].name)
    {
        return BuildCombinedLanguageName(language_list[index].name,
                    language_list[index].symbol);
    }
    return CString("");
}

///////////////////////////////////////////////////////////////////////////////
int config_languages_dlg::GetLanguageNameIndex(CString &findname)
{
    int index = 0;
    CString combined;

    while(language_list[index].name)
    {
        if (language_list[index].name == findname)
        {
            return index;
        }
        combined = BuildCombinedLanguageName(language_list[index].name, language_list[index].symbol);
        if (combined == findname)
        {
            return index;
        }
        index++;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
int config_languages_dlg::GetLanguageIdIndex(CString &findid)
{
    int index = 0;
    CString combined;

    while(language_list[index].name)
    {
        if (language_list[index].symbol == findid)
        {
            return index;
        }
        index++;
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
CString config_languages_dlg::GetLanguageId(CString &findname)
{
    int index = 0;

    while(language_list[index].name)
    {
        if (language_list[index].name == findname)
        {
            return CString(language_list[index].symbol);
        }
        index++;
    }
    return CString("en");
}

///////////////////////////////////////////////////////////////////////////////
CString config_languages_dlg::GetLanguageId(int index)
{
    if (index < LANGUAGE_LIST_SIZE)
    {
        return CString(language_list[index].symbol);
    }
    return CString("");
}

///////////////////////////////////////////////////////////////////////////////
CString config_languages_dlg::GetLanguageName(int index)
{
    if (index < LANGUAGE_LIST_SIZE)
    {
        return CString(language_list[index].name);
    }
    return CString("");
}

///////////////////////////////////////////////////////////////////////////////
int config_languages_dlg::GetStringTableIndexFromLanguageIndex(studiox_project *project, int seek_language)
{
    int table_index = 0;

    for (table_index = 0; table_index < project->mHeader.num_languages; table_index++)
    {
        int language_index = GetLanguageNameIndex(project->mHeader.languages[table_index].name);

        if (language_index == seek_language)
        {
            return table_index;
        }
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////
CString config_languages_dlg::ConvertOldLanguageName(CString &old_language_name)
{
    name_convert_map *list = language_name_convert_list;

    while (list->old_name)
    {
        if (list->old_name == old_language_name)
        { 
            return CString(list->new_name);
        }
        list++;
    }

    return old_language_name;
}


///////////////////////////////////////////////////////////////////////////////
void config_languages_dlg::AnnounceLanguageNumberChange()
{
    GetDlgItem(IDC_NUM_LANGUAGES)->NotifyWinEvent(
        EVENT_OBJECT_LIVEREGIONCHANGED,
        OBJID_CLIENT,
        CHILDID_SELF);

    CMainFrame* pMain = (CMainFrame*)AfxGetApp()->GetMainWnd();

    if (pMain && (!mSavedMsg.IsEmpty()))
    {
        CString msg;
        msg.Format(L"Number of Languages %d", mNumLanguages);
        pMain->GetStatusMsgControl()->SetWindowText(msg);
        pMain->GetStatusMsgControl()->NotifyWinEvent(
            EVENT_OBJECT_LIVEREGIONCHANGED,
            OBJID_CLIENT,
            CHILDID_SELF);
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL config_languages_dlg::OnInitDialog()
{
    express_dialog::OnInitDialog();
    CSpinButtonCtrl * pb = (CSpinButtonCtrl *) GetDlgItem(IDC_SPIN_LANGUAGE_INDEX);
    CEdit *ps = (CEdit *) GetDlgItem(IDC_LANGUAGE_INDEX);
    pb->SetBuddy(ps);
    pb->SetRange(1, mNumLanguages);
    ps->SetWindowText(_T("1"));

    int index = 0;

    while(language_list[index].name)
    {
        CString lang_name;
        lang_name = BuildCombinedLanguageName(language_list[index].name, language_list[index].symbol);
        mLanguageNameCombo.AddString(lang_name);
        index++;
    }
    mLanguageNameCombo.SetCurSel(GetLanguageNameIndex(mLanguages[0].name));

    //set bidi text support information
    mBidiTextSupportBox.SetCheck(mLanguages[0].support_bidi_text);
    mBidiTextGenerateBox.SetCheck(mLanguages[0].gen_reordered_bidi_text);
    mStaticallyDefined.SetCheck(mLanguages[0].statically_defined);
    OnClickSupportBidiTextBox();

    mThaiGlyphShapingSupport.SetCheck(mLanguages[0].support_thai_glyph_shaping);
    mGenAdjustedThaiString.SetCheck(mLanguages[0].gen_adjusted_thai_string);
    OnClickedThaiGlyphShapingSupport();

    AddCancelButton();
    AddSaveButton();

    return TRUE; // return TRUE unless you set the focus to a control
}


///////////////////////////////////////////////////////////////////////////////
void config_languages_dlg::DoDataExchange(CDataExchange* pDX)
{
    express_dialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LANG_NAME_COMBO, mLanguageNameCombo);
    DDX_Text(pDX, IDC_NUM_LANGUAGES, mNumLanguages);
    SetLiveRegion(GetDlgItem(IDC_NUM_LANGUAGES)->GetSafeHwnd());

    CRect size;

    // Set the drop-down list size for each of the combo-boxes.
    // There doesn't seem to be any way to do it in resource editor.
    mLanguageNameCombo.GetWindowRect(&size);
    size.bottom = size.top + 200;
    ScreenToClient(&size);
    mLanguageNameCombo.MoveWindow(&size);

    DDX_Control(pDX, IDC_SPIN_LANGUAGE_INDEX, mSpinLanguageIndex);

    DDX_Control(pDX, IDC_CHECK_STATICALLY_DEFINED, mStaticallyDefined);
    DDX_Control(pDX, IDC_CHECK_SUPPORT_BIDI_TEXT, mBidiTextSupportBox);
    DDX_Control(pDX, IDC_CHECK_REORDER_BIDI_TEXT, mBidiTextGenerateBox);
    DDX_Control(pDX, IDC_THAI_GLYPH_SHAPING_SUPPORT, mThaiGlyphShapingSupport);
    DDX_Control(pDX, IDC_GEN_ADJUSTED_THAI_STRING, mGenAdjustedThaiString);
}

///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(config_languages_dlg, express_dialog)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LANGUAGE_INDEX, &config_languages_dlg::OnDeltaposSpinLanguageIndex)
    ON_BN_CLICKED(IDC_ADD_LANGUAGE, &config_languages_dlg::OnBnClickedAddLanguage)
    ON_BN_CLICKED(IDC_DELETE_LANGUAGE, &config_languages_dlg::OnBnClickedDeleteLanguage)
    ON_BN_CLICKED(IDCANCEL, &config_languages_dlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDOK, &config_languages_dlg::OnBnClickedOk)
    ON_MESSAGE(STUDIO_TEST, OnTestMessage)
    ON_BN_CLICKED(IDC_CHECK_SUPPORT_BIDI_TEXT, &config_languages_dlg::OnClickSupportBidiTextBox)
    ON_BN_CLICKED(IDC_THAI_GLYPH_SHAPING_SUPPORT, &config_languages_dlg::OnClickedThaiGlyphShapingSupport)
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// config_languages_dlg message handlers

void config_languages_dlg::OnDeltaposSpinLanguageIndex(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    // TODO: Add your control notification handler code here
    int upper;
    int lower;

    mSpinLanguageIndex.GetRange(lower, upper);

    int oldval = pNMUpDown ->iPos;
    int newval = oldval + pNMUpDown->iDelta;

    // save the current name, update to new name, if the value is within 
    // limits
    if (oldval <= upper && oldval >= lower &&
        newval <= upper && newval >= lower)
    {
        mLanguages[oldval - 1].name = language_list[mLanguageNameCombo.GetCurSel()].name;

        //bidi text
        mLanguages[oldval - 1].support_bidi_text = mBidiTextSupportBox.GetCheck();
        mLanguages[oldval - 1].gen_reordered_bidi_text = mBidiTextGenerateBox.GetCheck();

        //thai string
        mLanguages[oldval - 1].support_thai_glyph_shaping = mThaiGlyphShapingSupport.GetCheck();
        mLanguages[oldval - 1].gen_adjusted_thai_string = mGenAdjustedThaiString.GetCheck();

        mLanguages[oldval - 1].statically_defined = mStaticallyDefined.GetCheck();
        mLanguageNameCombo.SetCurSel(GetLanguageNameIndex(mLanguages[newval - 1].name));

        mBidiTextSupportBox.SetCheck(mLanguages[newval - 1].support_bidi_text);
        mBidiTextGenerateBox.SetCheck(mLanguages[newval - 1].gen_reordered_bidi_text);
        mStaticallyDefined.SetCheck(mLanguages[newval - 1].statically_defined);
        OnClickSupportBidiTextBox();

        mThaiGlyphShapingSupport.SetCheck(mLanguages[newval - 1].support_thai_glyph_shaping);
        mGenAdjustedThaiString.SetCheck(mLanguages[newval - 1].gen_adjusted_thai_string);
        OnClickedThaiGlyphShapingSupport();
    }
    *pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////
void config_languages_dlg::OnBnClickedAddLanguage()
{
    // TODO: Add your control notification handler code here
    if (mNumLanguages + 1 > MAX_LANGUAGES)
    {
        return;
    }

    mNumLanguages++;

    mSpinLanguageIndex.SetRange(1, mNumLanguages);

    language_info info;
    info.name = language_list[0].name;
    info.statically_defined = TRUE;
    info.support_bidi_text = FALSE;
    info.gen_reordered_bidi_text = FALSE;
    info.support_thai_glyph_shaping = FALSE;

    mLanguages.Add(info);

    SetDlgItemInt(IDC_NUM_LANGUAGES, mNumLanguages);

    AnnounceLanguageNumberChange();
}

///////////////////////////////////////////////////////////////////////////////
void config_languages_dlg::OnBnClickedDeleteLanguage()
{
    if (mNumLanguages > 1)
    {
        int Index = mSpinLanguageIndex.GetPos();
        mLanguages.RemoveAt(Index - 1);
        mNumLanguages--;
        SetDlgItemInt(IDC_NUM_LANGUAGES, mNumLanguages);

        AnnounceLanguageNumberChange();

        mSpinLanguageIndex.SetRange(1, mNumLanguages);

        if (GetDlgItemInt(IDC_LANGUAGE_INDEX) > (UINT) mNumLanguages)
        {
            SetDlgItemInt(IDC_LANGUAGE_INDEX, mNumLanguages);
        }

        Index = GetDlgItemInt(IDC_LANGUAGE_INDEX);
        mLanguageNameCombo.SetCurSel(GetLanguageNameIndex(mLanguages[Index - 1].name));
        mBidiTextSupportBox.SetCheck(mLanguages[Index - 1].support_bidi_text);
        mBidiTextGenerateBox.SetCheck(mLanguages[Index - 1].gen_reordered_bidi_text);
        mStaticallyDefined.SetCheck(mLanguages[Index - 1].statically_defined);
        OnClickSupportBidiTextBox();

        mThaiGlyphShapingSupport.SetCheck(mLanguages[Index - 1].support_thai_glyph_shaping);
        mGenAdjustedThaiString.SetCheck(mLanguages[Index - 1].gen_adjusted_thai_string);
        OnClickedThaiGlyphShapingSupport();
    }
    else
    {
        ErrorMsg("Encountered Improper Arguement.", this);
    }
}

///////////////////////////////////////////////////////////////////////////////
void config_languages_dlg::OnBnClickedCancel()
{
    // just return without doing anything
    express_dialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
void config_languages_dlg::OnBnClickedOk()
{
    int index;
    int display;
    int old_language;
    int new_language;
    BOOL copy_language;
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    // save the last change in case it has not been saved yet 
    index = GetDlgItemInt(IDC_LANGUAGE_INDEX);
    mLanguages[index - 1].name = language_list[mLanguageNameCombo.GetCurSel()].name;
    mLanguages[index - 1].support_bidi_text = mBidiTextSupportBox.GetCheck();
    mLanguages[index - 1].gen_reordered_bidi_text = mBidiTextGenerateBox.GetCheck();

    mLanguages[index - 1].support_thai_glyph_shaping = mThaiGlyphShapingSupport.GetCheck();
    mLanguages[index - 1].gen_adjusted_thai_string = mGenAdjustedThaiString.GetCheck();

    mLanguages[index - 1].statically_defined = mStaticallyDefined.GetCheck();

    // update the project header to hold our new langauge count and names:
    int old_num_languages = project->mHeader.num_languages;
    project->mHeader.num_languages = mNumLanguages;

    for (index = 0; index < mNumLanguages; index++)
    {
        project->mHeader.languages[index].name = mLanguages[index].name;

        project->mHeader.languages[index].support_bidi_text = mLanguages[index].support_bidi_text;
        project->mHeader.languages[index].gen_reordered_bidi_text = mLanguages[index].gen_reordered_bidi_text;
        project->mHeader.languages[index].support_thai_glyph_shaping = mLanguages[index].support_thai_glyph_shaping;
        project->mHeader.languages[index].gen_adjusted_thai_string = mLanguages[index].gen_adjusted_thai_string;
        project->mHeader.languages[index].statically_defined = mLanguages[index].statically_defined;
    }

    for (index = old_num_languages; index < mNumLanguages; index++)
    {
        //for the new added languages, enabled language generation for all displays
        for (int display = 0; display < project->mHeader.num_displays; display++)
        {
            project->mDisplays[display].gen_string_table[index] = TRUE;
        }
    }

    // make a new string tables for each display holding the new languages.
    // Copy what we can from the old tables (matching langauge names)

    int active_display = 0;
    if (GetProjectView())
    {
        active_display = GetProjectView()->GetActiveDisplay();
    }

    for (display = 0; display < project->mHeader.num_displays; display++)
    {
        string_table *pOldTable = project->mDisplays[display].stable;
        if (!pOldTable)
        {
            ErrorMsg("Internal Error: Source string table invalid", this);
            express_dialog::OnOK();
            return;
        }
        int num_strings = pOldTable->CountStrings();

        string_table *pNewTable = new string_table;
        pNewTable->Initialize(mNumLanguages, pOldTable->CountStrings());

        CString id_name;
        for (index = 1; index < num_strings; index++)
        {
            id_name = pOldTable->GetResourceIdName(index);
            pNewTable->AddToDictionary(id_name, index);
        }

        // copy the string_ids, fonts, and notes:
        for (index = 0; index < num_strings; index++)
        {
            string_table_record record = pOldTable->GetRecord(index);
            pNewTable->SetDisplayFont(index, record.font_id);
            pNewTable->SetStringId(index, record.id_name);
            if (!record.notes.IsEmpty())
            {
                pNewTable->SetNotes(index, record.notes);
            }
        }

        // for each language in the new table, see if we have that
        // language in the old table
        for (new_language = 0; new_language < mNumLanguages; new_language++)
        {
            copy_language = FALSE;
            for (old_language = 0; old_language  < mStartLanguages; old_language++)
            {
                if (mLanguages[old_language].name == mLanguages[new_language].name)
                {
                    copy_language = TRUE;
                    break;
                }
            }

            if (copy_language)
            {
                // we found a match, copy strings from old table to new
                for (index = 0; index < num_strings; index++)
                {
                    pNewTable->SetString(index, new_language, pOldTable->GetString(index, old_language));
                }
            }
        }

        if (display == active_display)
        {
            int active_language = pOldTable->GetActiveLanguage();

            if (active_language >= mNumLanguages)
            {
                active_language = 0;
            }

            pNewTable->SetActiveLanguage(active_language);
        }

        // swap out the string tables:
        project->mDisplays[display].stable = pNewTable;
        if (pOldTable)
        {
            delete pOldTable;
        }

        pNewTable->UpdateGuixLanguageTable(NULL, FALSE);
    }

    express_dialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
void config_languages_dlg::OnClickSupportBidiTextBox()
{
    BOOL enable = FALSE;

    if (mBidiTextSupportBox.GetCheck())
    {
        enable = TRUE;
    }

    mBidiTextGenerateBox.EnableWindow(enable);
}

///////////////////////////////////////////////////////////////////////////////
void config_languages_dlg::OnClickedThaiGlyphShapingSupport()
{
    BOOL enabed = FALSE;

    if (mThaiGlyphShapingSupport.GetCheck())
    {
        enabed = TRUE;
    }

    mGenAdjustedThaiString.EnableWindow(enabed);
}

///////////////////////////////////////////////////////////////////////////////
afx_msg LRESULT config_languages_dlg::OnTestMessage(WPARAM wParam, LPARAM lParam)
{
    CWnd *pWnd;

    switch (wParam)
    {
    case TEST_ADD_LANGUAGE:
        pWnd = GetDlgItem(IDC_ADD_LANGUAGE);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_ADD_LANGUAGE, BN_CLICKED), (LPARAM)pWnd->GetSafeHwnd());
        break;

    case TEST_DELETE_LANGEUGE:
        pWnd = GetDlgItem(IDC_DELETE_LANGUAGE);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_DELETE_LANGUAGE, BN_CLICKED), (LPARAM)pWnd->GetSafeHwnd());
        break;

    case TEST_SELECT_LANGUAGE_INDEX:
    {
        NMUPDOWN NMUpDown;
        NMHDR hdr;

        hdr.hwndFrom = mSpinLanguageIndex.m_hWnd;
        hdr.idFrom = IDC_SPIN_LANGUAGE_INDEX;
        hdr.code = UDN_DELTAPOS;
        
        NMUpDown.hdr = hdr;
        NMUpDown.iPos = mSpinLanguageIndex.GetPos();
        NMUpDown.iDelta = lParam - NMUpDown.iPos;

        mSpinLanguageIndex.SetPos(lParam);
        SetDlgItemInt(IDC_LANGUAGE_INDEX, lParam);

        SendMessage(WM_NOTIFY, IDC_SPIN_LANGUAGE_INDEX, (LPARAM)&NMUpDown);
        break;
    }

    case TEST_SELECT_LANGUAGE_ID:
    {
        int language_index = GetLanguageNameIndex(GetTestingParam(0));
        mLanguageNameCombo.SetCurSel(language_index);
        SendMessage(WM_COMMAND, MAKEWPARAM(IDC_LANG_NAME_COMBO, CBN_SELCHANGE), (LPARAM)(mLanguageNameCombo.m_hWnd));
        break;
    }

    case TEST_CHECK_SUPPORT_BIDI_TEXT:
        pWnd = (CWnd *)GetDlgItem(IDC_CHECK_SUPPORT_BIDI_TEXT);
        if (pWnd->IsWindowEnabled())
        {
            ((CButton *)pWnd)->SetCheck(lParam);
            SendMessage(WM_COMMAND, MAKEWPARAM(IDC_CHECK_SUPPORT_BIDI_TEXT, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        }
        break;

    case TEST_CHECK_REORDER_BIDI_TEXT:
        pWnd = (CWnd *)GetDlgItem(IDC_CHECK_REORDER_BIDI_TEXT);
        if (pWnd->IsWindowEnabled())
        {
            ((CButton *)pWnd)->SetCheck(lParam);
            SendMessage(WM_COMMAND, MAKEWPARAM(IDC_CHECK_REORDER_BIDI_TEXT, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        }
        break;

    case TEST_CHECK_SUPPORT_THAI_SHAPING:
        pWnd = (CWnd*)GetDlgItem(IDC_THAI_GLYPH_SHAPING_SUPPORT);
        if (pWnd->IsWindowEnabled())
        {
            ((CButton*)pWnd)->SetCheck(lParam);
            SendMessage(WM_COMMAND, MAKEWPARAM(IDC_THAI_GLYPH_SHAPING_SUPPORT, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        }
        break;

    case TEST_CHECK_GEN_ADJUSTED_THAI:
        pWnd = (CWnd*)GetDlgItem(IDC_GEN_ADJUSTED_THAI_STRING);
        if (pWnd->IsWindowEnabled())
        {
            ((CButton*)pWnd)->SetCheck(lParam);
            SendMessage(WM_COMMAND, MAKEWPARAM(IDC_GEN_ADJUSTED_THAI_STRING, BN_CLICKED), (LPARAM)pWnd->m_hWnd);
        }
        break;

    case TEST_SAVE_LANGUAGE_CONFIGURE:
        OnBnClickedOk();
        break;

    case TEST_CANCEL_LANGUAGE_CONFIGURE:
        OnBnClickedCancel();
        break;
    }

    return 0;
}