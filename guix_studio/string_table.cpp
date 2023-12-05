
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


static CString right_to_left_language_list[] = {
L"Arabic",
L"Hebrew",
L"Kurdish",
L"Persian",
L"Urdu",
L""
};

///////////////////////////////////////////////////////////////////////////////
string_table::string_table()
{
    m_languages = 0;
    m_active_language = 0;
    m_sort_column = 0;
    m_records.SetSize(0, 10);
    m_dictionary.SetSize(0, 10);
}

///////////////////////////////////////////////////////////////////////////////
string_table::string_table(const string_table &other)
{
    int record;
    int string;

    Initialize(other.m_languages, other.m_records.GetCount());
    
    for (record = 0; record < m_records.GetCount(); record++)
    {
        m_dictionary[record] = other.m_dictionary[record];

        string_table_record src = other.m_records[record];
        m_records[record].font_id = src.font_id;
        m_records[record].id_name = src.id_name;
        m_records[record].notes = src.notes;
        
        for (string = 0; string < m_languages; string++)
        {
            m_records[record].strings[string] = src.strings[string];
            char_map.AddToMap(m_records[record].strings[string]);
        }
    }
    m_sort_column = other.m_sort_column;
    m_active_language = other.m_active_language;
}

///////////////////////////////////////////////////////////////////////////////
string_table::~string_table()
{
    CleanupStringTable();

    //Clean up string dictionary
    m_dictionary.RemoveAll();

    CleanUpMLViewRefereceRecord();
}

///////////////////////////////////////////////////////////////////////////////
BOOL string_table::IsRight2LeftLanguage(int language_index)
{
    studiox_project* project = GetOpenProject();

    if (project)
    {

        int index = 0;

        while (!right_to_left_language_list[index].IsEmpty())
        {
            if (right_to_left_language_list[index] == project->mHeader.languages[language_index].name)
            {
                return TRUE;
            }

            index++;
        }
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void string_table::EnableDisableRuntimeBidiText(int language_index)
{
    studiox_project *project = GetOpenProject();
    if (project->mHeader.languages[language_index].support_bidi_text)
    {
        gx_system_bidi_text_enable();
    }
    else
    {
        gx_system_bidi_text_disable();
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL string_table::TestGenerateLanguageDirectionTable()
{
    studiox_project* project = GetOpenProject();

    if (project && project_lib_version() >= GX_VERSION_BIDI_TEXT_BASE_DIRECTION_FIX)
    {
        for (int language = 0; language < project->mHeader.num_languages; language++)
        {
            if (project->mHeader.languages[language].support_bidi_text &&
                !project->mHeader.languages[language].gen_reordered_bidi_text)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void string_table::CleanupStringTable()
{
    int index;
    char_map.Clear();

    if (m_records.GetCount() > 0)
    {
        for (index = 0; index < m_records.GetCount(); index++)
        {
            if (m_records[index].strings)
            {
                delete [] m_records[index].strings;
            }
        }
        m_records.RemoveAll();
    }

    m_languages = 0;
}

///////////////////////////////////////////////////////////////////////////////
void string_table::CreateId(CString &id_name, CString base_name)
{
    CString test;
    CString num;
    int val = 1;

    if (base_name.IsEmpty())
    {
        test = CString("STRING_");
    }
    else
    {
        test = base_name + CString("_");
    }

    while(1)
    {
        num.Format(_T("%d"), val);
        id_name = test + num;
        if (!FindStringIndex(id_name))
        {
            return;
        }
        val++;
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_table::Initialize(int num_languages, int num_strings)
{
    CleanupStringTable();
    m_records.SetSize(num_strings,  10);
    m_dictionary.SetSize(num_strings, 10);

    for (int record = 0; record < num_strings; record++)
    {
        m_records[record].strings = new CString[num_languages];
    }
    m_languages = num_languages;
}


///////////////////////////////////////////////////////////////////////////////
int string_table::AddString(CString &Id, CString &str, bool update_guix)
{
    string_table_record record;
    record.id_name = Id;
    record.strings = new CString[m_languages];
    record.strings[0] = str;
    record.font_id = 0;
    record.notes = "";
    m_records.Add(record);

    AddToDictionary(Id);

    if (update_guix)
    {
        UpdateGuixLanguageTable();
    }

    return m_records.GetCount() - 1;
}

///////////////////////////////////////////////////////////////////////////////
void string_table::ImportString(CString &string_id,
                                CString &src_string,
                                CString &target_string,
                                CString &notes,
                                int src_table_column,
                                int target_table_column)
{
    int string_index = FindStringIndex(string_id);

    if (string_index > 0)
    {
        SetString(string_index, target_table_column, target_string, FALSE);
    }
    else
    {
        string_index = AddString(string_id, CString(""), FALSE);
        SetString(string_index, src_table_column, src_string, FALSE);
        SetString(string_index, target_table_column, target_string, FALSE);
    }

    SetNotes(string_index, notes);
}

///////////////////////////////////////////////////////////////////////////////
int string_table::AddString()
{
    CString NewId;
    CreateId(NewId);
    int string_index = AddString(NewId, CString(""));
    return string_index;
}

///////////////////////////////////////////////////////////////////////////////
int string_table::CheckAddString(CString &str, CString base_name)
{
    int index;
    CString NewId;

    for (index = 1; index < m_records.GetCount(); index++)
    {
        if (m_records[index].strings[0] == str)
        {
            return index;
        }
    }
    
    CreateId(NewId, base_name);
    index = AddString(NewId, str);
    return index;
}

///////////////////////////////////////////////////////////////////////////////
int string_table::FindStringIndex(CString &id_name)
{
    int index = 1;

    while(index < m_records.GetCount())
    {
        if (m_records[index].id_name == id_name)
        {
            return index;
        }
        index++;
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
void string_table::RemoveString(CString &id_name)
{
    int index = FindStringIndex(id_name);
    if (index)
    {
        RemoveString(index);
        UpdateGuixLanguageTable();
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL string_table::RemoveString(int string_index)
{
    if (string_index >= 0 && string_index < m_records.GetCount())
    {
        CString *strings = m_records[string_index].strings;

        if (strings)
        {
            delete [] strings;
        }
        RemoveFromDictionary(m_records[string_index].id_name);
        m_records.RemoveAt(string_index);
        UpdateGuixLanguageTable();
        return TRUE;
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void string_table::SetString(CString &id_name, int language, CString &str, bool update_guix)
{
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    int index = FindStringIndex(id_name);
    
    if (index && language < m_languages)
    {
        if (m_records[index].strings[language] != str)
        {
            m_records[index].strings[language] = str;

            if (update_guix)
            {
                UpdateGuixLanguageTable();
            }

            int string_id = GetResourceId(id_name);
            widget_factory::UpdateInputWidgetText(project, string_id);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_table::SetString(int index, CString &id_name, int language, CString &str, bool update_guix)
{
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    if (index >= 0 && index < m_records.GetCount() && language < m_languages)
    {
        if (m_records[index].id_name != id_name)
        {
            UpdateDictionaryResourceName(m_records[index].id_name, id_name);
            m_records[index].id_name = id_name;
        }

        if (m_records[index].strings[language] != str)
        {
            m_records[index].strings[language] = str;

            if (update_guix)
            {
                UpdateGuixLanguageTable();
            }

            int string_id = GetResourceId(id_name);
            widget_factory::UpdateInputWidgetText(project, string_id);
        }
    }        
}

///////////////////////////////////////////////////////////////////////////////
BOOL string_table::SetStringId(int string_index, CString &id_name)
{
    int existing = FindStringIndex(id_name);

    if (!existing && string_index >= 0 && string_index < m_records.GetCount())
    {
        UpdateDictionaryResourceName(m_records[string_index].id_name, id_name);
        m_records[string_index].id_name = id_name;
        return TRUE;
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void string_table::SetNotes(int string_index, CString &notes)
{
    if (string_index >= 0 && string_index < m_records.GetCount())
    {
        m_records[string_index].notes = notes;
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_table::SetDisplayFont(int string_index, int font_id)
{
    if (string_index >= 0 && string_index < m_records.GetCount())
    {
        m_records[string_index].font_id = font_id;
    }
}


///////////////////////////////////////////////////////////////////////////////
void string_table::SetString(int string_index, int language, CString &str, bool update_guix)
{
    studiox_project *project = GetOpenProject();

    if (!project)
    {
        return;
    }

    if (string_index >= 0 && string_index < m_records.GetCount() && language < m_languages)
    {
        if (m_records[string_index].strings[language] != str)
        {
            m_records[string_index].strings[language] = str;

            if (update_guix)
            {
                UpdateGuixLanguageTable();
            }

            int string_id = GetResourceId(m_records[string_index].id_name);
            widget_factory::UpdateInputWidgetText(project, string_id);
        }
    }        
}

static int compare_id(string_table_record *record_1, string_table_record *record_2)
{
    return record_1->id_name.Compare(record_2->id_name);
}

static int language_id;
static int compare_string(string_table_record *record_1, string_table_record *record_2)
{
    return record_1->strings[language_id].Compare(record_2->strings[language_id]);
}

///////////////////////////////////////////////////////////////////////////////
void string_table::SortById()
{
    qsort(&m_records.GetAt(1), m_records.GetSize() - 1, sizeof(string_table_record), (int(*)(const void *, const void *))compare_id);
}

///////////////////////////////////////////////////////////////////////////////
void string_table::SortByString()
{
    language_id = m_sort_column - 1;
    qsort(&m_records.GetAt(1), m_records.GetSize() - 1, sizeof(string_table_record), (int(*)(const void *, const void *))compare_string);
}

void string_table::SortByReferenceCount()
{
    studiox_project *project = GetOpenProject();

    if (project)
    {
        int index_1;
        int index_2;
        int count_1;
        int count_2;
        int resource_id;
        string_table_record temp;

        for (index_1 = 1; index_1 < m_records.GetCount() - 1; index_1++)
        {
            resource_id = GetResourceId(m_records.GetAt(index_1).id_name);
            count_1 = widget_factory::CountReferences(project, RES_TYPE_STRING, resource_id);

            for (index_2 = index_1 + 1; index_2 < m_records.GetCount(); index_2++)
            {
                resource_id = GetResourceId(m_records.GetAt(index_2).id_name);
                count_2 = widget_factory::CountReferences(project, RES_TYPE_STRING, resource_id);

                if (count_2 < count_1)
                {
                    temp = m_records.GetAt(index_1);
                    m_records.SetAt(index_1, m_records.GetAt(index_2));
                    m_records.SetAt(index_2, temp);

                    count_1 = count_2;
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_table::Sort()
{
    if ((m_sort_column == -1) || (m_records.GetCount() < 3))
    {
        //no sort
        return;
    }
    if (m_sort_column == 0)
    {
        SortById();
    }
    else if ((m_sort_column >= 1 && m_sort_column <= m_languages))
    {
        SortByString();
    }
    else if ((m_sort_column == m_languages + 1))
    {
        SortByReferenceCount();
    }
    else
    {
        ErrorMsg("Internal Error: Invalid sorting type.");
        return;
    }
}

CString NullString("");
string_table_record NullRecord;

///////////////////////////////////////////////////////////////////////////////
string_table_record &string_table::GetRecord(int string_index)
{
    if (string_index >= 0 && string_index < m_records.GetCount())
    {
        return m_records.GetAt(string_index);
    }
    return NullRecord;
}

///////////////////////////////////////////////////////////////////////////////
string_table_record &string_table::GetRecord(CString id_name)
{
    for (int index = 0; index < m_records.GetCount(); index++)
    {
        if (m_records.GetAt(index).id_name == id_name)
        {
            return m_records.GetAt(index);
        }
    }
    return NullRecord;
}

///////////////////////////////////////////////////////////////////////////////
CString &string_table::GetString(CString &id_name, int language)
{
    int index = FindStringIndex(id_name);

    if (index && language < m_languages)
    {
        return GetString(index, language);
    }  
    return NullString;  
}

///////////////////////////////////////////////////////////////////////////////
CString &string_table::GetString(int string_index, int language)
{
    if (string_index >= 0 && string_index < m_records.GetCount() && language < m_languages)
    {
        return (m_records[string_index].strings[language]);
    }
    return NullString;  
}

///////////////////////////////////////////////////////////////////////////////
CString &string_table::GetStringId(int string_index)
{
    if (string_index >= 0 && string_index < m_records.GetCount())
    {
        return m_records[string_index].id_name;
    }
    return NullString;
}

///////////////////////////////////////////////////////////////////////////////
CString &string_table::GetNotes(int string_index)
{
    if (string_index >= 0 && string_index < m_records.GetCount())
    {
        return m_records[string_index].notes;
    }
    return NullString;
}

///////////////////////////////////////////////////////////////////////////////
int string_table::CountStrings()
{
    return (int) m_records.GetCount();
}

///////////////////////////////////////////////////////////////////////////////
int string_table::CountLanguages()
{
    return m_languages;
}

///////////////////////////////////////////////////////////////////////////////
FontCharMap *string_table::GenerateCleanCharacterMap()
{
    char_map.Clear();

    int record;
    int language;
    
    for (record = 0; record < m_records.GetCount(); record++)
    {
        for (language = 0; language < m_languages; language++)
        {
            if (!m_records[record].strings[language].IsEmpty())
            {
                char_map.AddToMap(m_records[record].strings[language]);
            }
        }
    }
    return &char_map;
}

// This function builds a string table for GUIX and installs it
// using this object's data. First, it makes sure that this
// table is the active string table being displayed by GUIX

///////////////////////////////////////////////////////////////////////////////
void string_table::UpdateGuixLanguageTable(GX_DISPLAY *Display, BOOL update_resource_view)
{
    // first, make certain that this table instance is the one
    // that is actively used by GUIX:
    studiox_project *project = GetOpenProject();    
    if (!project)
    {
        return;
    }
    int display = GetProjectView()->GetActiveDisplay();

    if (display < 0)
    {
        return;
    }
    if (project->mDisplays[display].stable != this)
    {
      return;
    }

    // yes, this table is the active table being displayed.
    // keep pointer to old table to delete later:
    GX_STRING **pOldTable;
    GX_UBYTE old_language_count;
    UINT old_string_count;
    int StringId;
    int language;

    if (!Display)
    {
        Display = get_target_view_display();
    }

    gx_display_language_table_get_ext(Display, &pOldTable, &old_language_count, &old_string_count);

    GX_UBYTE *pDirectionTable = new GX_UBYTE[m_languages];
    GX_STRING **pLangTable = new GX_STRING *[m_languages];
    memset(pLangTable, 0, m_languages * sizeof(GX_STRING *));
    BOOL new_char = FALSE;
    string_table_record record;
    CString InString;

    for (language = 0; language < m_languages; language++)
    {
        GX_STRING *pStringTable = new GX_STRING[m_dictionary.GetCount()];
        pStringTable[0].gx_string_ptr = NULL;
        pStringTable[0].gx_string_length = 0;

        pLangTable[language] = pStringTable;

        if (string_table::IsRight2LeftLanguage(language))
        {
            pDirectionTable[language] = GX_LANGUAGE_DIRECTION_RTL;
        }
        else
        {
            pDirectionTable[language] = GX_LANGUAGE_DIRECTION_LTR;
        }

        for (StringId = 1; StringId < m_dictionary.GetCount(); StringId++)
        {
            record = GetRecord(GetResourceIdName(StringId));
            InString = record.strings[language];

            if (InString.IsEmpty())
            {
                pStringTable[StringId].gx_string_ptr = NULL;
                pStringTable[StringId].gx_string_length = 0;
            }
            else
            {
                if (char_map.AddToMap(InString))
                {
                    new_char = TRUE;
                }
                resource_gen::MakeUtf8String(project, InString, language, &pStringTable[StringId], display, FALSE);
            }
        }
    }

    if (new_char)
    {
        SendMessage(GetResourceView()->GetSafeHwnd(), USR_MSG_UPDATE_STRING_TABLE_FONTS, (WPARAM)&char_map, 0);
    }

    gx_display_language_table_set_ext(Display, (GX_CONST GX_STRING **) pLangTable, m_languages, (UINT)m_records.GetCount());

    // delete old direction table
    DeleteGuixLanguageDirectionTable(Display);

    gx_display_language_direction_table_set(Display, (GX_CONST GX_UBYTE *)pDirectionTable, m_languages);

    // now delete the old table:

    DeleteGuixLanguageTable(pOldTable, old_language_count, old_string_count);

    if (update_resource_view)
    {
        // now tell the resource view about the new string table:
        SendMessage(GetResourceView()->GetSafeHwnd(), USR_MSG_REBUILD_STIRNG_ITEMS, (WPARAM)FALSE, 0);
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_table::DeleteGuixLanguageDirectionTable(GX_DISPLAY* display)
{
    if (display->gx_display_language_direction_table)
    {
        delete display->gx_display_language_direction_table;
        display->gx_display_language_direction_table = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_table::DeleteGuixLanguageTable(GX_STRING **pOldTable, int language_count, int string_count)
{
    INT StringId;
    INT language;
    GX_STRING *pStringTable;

    if (pOldTable && language_count > 0)
    {
        for (language = 0; language < language_count; language++)
        {
            pStringTable = pOldTable[language];

            if (pStringTable)
            {
                for (StringId = 0; StringId < string_count; StringId++)
                {
                    if (pStringTable[StringId].gx_string_ptr)
                    {
                        delete [] pStringTable[StringId].gx_string_ptr;
                        pStringTable[StringId].gx_string_ptr = NULL;
                        pStringTable[StringId].gx_string_length = 0;
                    }
                }
                delete [] pStringTable;
                pOldTable[language] = NULL;
            }
        }
        delete [] pOldTable;
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_table::AddToDictionary(CString &id_name, int res_id)
{
    if (res_id == -1)
    {
        m_dictionary.Add(id_name);
    }
    else
    {
        m_dictionary.SetAtGrow(res_id, id_name);
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_table::RemoveFromDictionary(CString &id_name)
{
    for (int index = 1; index < m_dictionary.GetCount(); index++)
    {
        if (m_dictionary.GetAt(index) == id_name)
        {
            m_dictionary.RemoveAt(index);
            return;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
CString string_table::UpdateRichTextResourceName(CString& val, int res_type, CString old_id_name, CString new_id_name)
{
    CString out("");
    CString tag;
    CString id_name;
    int index = 0;

    if (res_type == RES_TYPE_FONT)
    {
        old_id_name.Insert(0, _T("GX_FONT_ID_"));
        new_id_name.Insert(0, _T("GX_FONT_ID_"));
    }
    else
    {
        old_id_name.Insert(0, _T("GX_COLOR_ID_"));
        new_id_name.Insert(0, _T("GX_COLOR_ID_"));
    }

    while (1)
    {
        // Find tag start flag
        index = val.Find('<');

        if (index >= 0)
        {
            out.Append(val.Left(index + 1));
            val = val.Mid(index + 1);

            index = val.Find(_T(" "));

            if (index >= 0)
            {
                // Get tag
                tag = val.Left(index);

                if ((tag != "f") && (tag != "c") && (tag != "hc"))
                {
                    continue;
                }

                out.Append(val.Left(index + 1));
                val = val.Mid(index + 1);

                index = val.Find(_T(">"));
                if (index >= 0)
                {
                    // Get resource id name
                    id_name = val.Left(index);

                    if (id_name == old_id_name)
                    {
                        val = val.Mid(index + 1);
                        out.Append(new_id_name);
                        out.Append(_T(">"));
                    }
                }
            }
        }
        else
        {
            out.Append(val);
            break;
        }
    }

    return out;
}

///////////////////////////////////////////////////////////////////////////////
void string_table::UpdateStringTableRichTextResourceName(int res_type, CString& old_id_name, CString& new_id_name)
{
    int num_strings = m_records.GetCount();

    for (int record = 0; record < num_strings; record++)
    {
        for (int language = 0; language < m_languages; language++)
        {
            if (!m_records[record].strings[language].IsEmpty())
            {
                m_records[record].strings[language] = UpdateRichTextResourceName(m_records[record].strings[language], res_type, old_id_name, new_id_name);
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_table::UpdateDictionaryResourceName(CString& old_id_name, CString& new_id_name)
{
    for (int index = 1; index < m_dictionary.GetCount(); index++)
    {
        if (m_dictionary.GetAt(index) == old_id_name)
        {
            m_dictionary.SetAt(index, new_id_name);
            return;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
CString string_table::GetResourceIdName(int res_id, widget_info* info)
{
    CString id_name("");

    if (res_id > 0 && res_id < m_dictionary.GetCount())
    {
        id_name = m_dictionary.GetAt(res_id);
    }

    if (info)
    {
        string_reference_record *record = GetMLViewReferenceRecord(id_name);

        if (record)
        {
            for (int index = 0; index < record->reference_list.GetCount(); index++)
            {
                if ((record->reference_list.GetAt(index) == info) && (record->reference_list.GetCount() > 1))
                {
                    id_name.Format(L"%s_%s", id_name, info->app_name);
                    id_name.MakeUpper();
                    if (FindStringIndex(id_name))
                    {
                        CreateId(id_name, id_name);
                    }
                }
            }
        }
    }

    return id_name;
}

///////////////////////////////////////////////////////////////////////////////
GX_RESOURCE_ID string_table::GetResourceId(CString& id_name)
{
    for (int index = 1; index < m_dictionary.GetCount(); index++)
    {
        if (m_dictionary.GetAt(index) == id_name)
        {
            return index;
        }
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////
bool string_table::MakeMLViewReferenceRecord(widget_info* child)
{
    string_reference_record* record;
    CString id_name;

    while (child)
    {
        if ((child->basetype == GX_TYPE_MULTI_LINE_TEXT_VIEW) && child->string_id[0])
        {
            id_name = GetResourceIdName(child->string_id[0]);
            record = GetMLViewReferenceRecord(id_name);

            if (record)
            {
                record->reference_list.Add(child);
            }
            else
            {
                record = new string_reference_record;
                record->id_name = id_name;
                record->reference_list.Add(child);
                m_reference_records.Add(record);
            }
        }

        MakeMLViewReferenceRecord(child->GetChildWidgetInfo());

        child = child->GetNextWidgetInfo();
    }

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
void string_table::MakeMLViewReferenceRecord(int display)
{
    studiox_project *project = GetOpenProject();
    bool generate_unique_reference = FALSE;

    for (int lang = 0; lang < project->mHeader.num_languages; lang++)
    {
        if (project->mHeader.languages[lang].support_bidi_text &&
            project->mHeader.languages[lang].gen_reordered_bidi_text)
        {
            generate_unique_reference = TRUE;
            break;
        }
    }

    if (!generate_unique_reference)
    {
        return;
    }

    // Clear reference record.
    CleanUpMLViewRefereceRecord();

    // Make reference record.
    folder_info *folder;

    folder = project->mDisplays[display].GetFirstChildFolder();
    while (folder)
    {
        MakeMLViewReferenceRecord(folder->GetFirstChildWidget());

        folder = folder->GetNextFolder();
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_table::CleanUpMLViewRefereceRecord()
{
    string_reference_record* record = NULL;

    for (int index = 0; index < m_reference_records.GetCount(); index++)
    {
        record = m_reference_records.GetAt(index);
        delete record;
    }

    m_reference_records.RemoveAll();
}

///////////////////////////////////////////////////////////////////////////////
string_reference_record * string_table::GetMLViewReferenceRecord(CString& id_name)
{
    string_reference_record *record = NULL;

    for (int index = 0; index < m_reference_records.GetCount(); index++)
    {
        record = m_reference_records.GetAt(index);
        if (record->id_name == id_name)
        {
            return record;
        }
    }

    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
CArray<widget_info *> *string_table::GetMLViewReferenceWidgetInfoList(CString& id_name)
{
    string_reference_record* record = GetMLViewReferenceRecord(id_name);

    if (record)
    {
        return &record->reference_list;
    }

    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
int string_table::CountGeneratedStrings()
{
    string_reference_record* record;
    int count = CountStrings();

    for (int index = 0; index < m_reference_records.GetCount(); index++)
    {
        record = m_reference_records.GetAt(index);
        count += (record->reference_list.GetCount() - 1);
    }

    return count;
}

///////////////////////////////////////////////////////////////////////////////
void string_table::AddLanguage()
{
    int old_languages = m_languages;
    m_languages++;
    int num_strings = m_records.GetCount();

    for (int record = 0; record < num_strings; record++)
    {
        CString *old_strings = m_records[record].strings;
        m_records[record].strings = new CString[m_languages];

        for (int language = 0; language < old_languages; language++)
        {
            if (!old_strings[language].IsEmpty())
            {
                m_records[record].strings[language] = old_strings[language];
            }
        }
        delete [] old_strings;
    }
}

///////////////////////////////////////////////////////////////////////////////
void string_table::SetActiveLanguage(int language)
{
    studiox_project* project = GetOpenProject();

    if (!project)
    {
        return;
    }

    m_active_language = language;

    GX_DISPLAY* display = get_target_view_display();

    if (!display)
    {
        return;
    }

    if (language >= display->gx_display_language_table_size)
    {
        UpdateGuixLanguageTable();
    }
    gx_display_active_language_set(display, language);
    EnableDisableRuntimeBidiText(language);
}