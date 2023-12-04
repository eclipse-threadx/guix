

#include "studiox_includes.h"
#include "xml_writer.h"
#include "xml_reader.h"
#include "csv_read_write.h"
#include "config_languages_dlg.h"

///////////////////////////////////////////////////////////////////////////////
csv_read_write::csv_read_write()
{
    mpLine = NULL;
    mpBuffer = NULL;
}

///////////////////////////////////////////////////////////////////////////////
BOOL csv_read_write::ExportLanguage(studiox_project *project)
{
    int index;
    int src_language = project->mHeader.string_export_src;
    int target_language = project->mHeader.string_export_target;
    CString tag_data;
    CString src_data;
    CString pathname = project->mHeader.string_export_path;
    pathname += "\\";
    pathname += project->mHeader.string_export_filename;

    GotoProjectDirectory();
    CStdioFile file;

    if (!file.Open(pathname, CFile::modeCreate | CFile::modeWrite))
    {
        return FALSE;
    }

    int display = GetProjectView()->GetActiveDisplay();
    string_table *table = project->mDisplays[display].stable;

    tag_data.Format(_T("name,%s,%s\n"),
        config_languages_dlg::GetLanguageId(src_language),
        config_languages_dlg::GetLanguageId(target_language));

    //write BOM mark
    unsigned char BOM_mark[] = { 0xef, 0xbb, 0xbf };
    file.Write(BOM_mark, 3);

    //write header line
    WriteString(&file, tag_data);

    src_language = config_languages_dlg::GetStringTableIndexFromLanguageIndex(project, src_language);
    target_language = config_languages_dlg::GetStringTableIndexFromLanguageIndex(project, target_language);

    if (src_language == -1)
    {
        return FALSE;
    }

    CString csv_line;

    for (index = 0; index < table->CountStrings(); index++)
    {
        src_data = table->GetString(index, src_language);
        if (!src_data.IsEmpty())
        {
            //string id
            csv_line = table->GetStringId(index);
            csv_line.Append(_T(","));

            //source language string
            src_data = MakeCsvFormatValue(src_data);
            csv_line.Append(src_data);

            //target language string
            if (target_language >= 0)
            {
                csv_line.Append(_T(","));
                tag_data = table->GetString(index, target_language);
                tag_data = MakeCsvFormatValue(tag_data);
                csv_line.Append(tag_data);
            }
            csv_line.Append(_T("\n"));

            WriteString(&file, csv_line);
        }
    }

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL csv_read_write::ImportCsvFile(studiox_project *project, CString &pathname)
{
    CString src_language;
    CString target_language;
    CString string_id;
    CString src_string;
    CString target_string;
    CString notes("");
    BOOL status = TRUE;

    int language_index;
    int target_table_column;
    int src_table_column;

    int display = GetProjectView()->GetActiveDisplay();
    string_table *table = project->mDisplays[display].stable;

    ReadFile(pathname);

    CString line;
    CStringArray header_array;
    GotoProjectDirectory();

    ReadLine(line);
    SplitString(line, ',', &header_array);

    // Get the version information

    if (header_array.GetCount() >= 3)
    {
        //there should be at lease 3 columns 
        int col, index;
        CStringArray string_array;

        src_language = header_array.GetAt(1);
        language_index = config_languages_dlg::GetLanguageIdIndex(src_language);
        src_table_column = config_languages_dlg::GetStringTableIndexFromLanguageIndex(project, language_index);

        if (src_table_column == -1)
        {
            delete[]mpBuffer;
            mpBuffer = NULL;
            mpLine = NULL;
            return FALSE;
        }

        while (ReadLine(line))
        {
            index = 0;
            string_array.RemoveAll();
            SplitCsvLine(line, string_array);

            if (string_array.GetCount() < 2)
            {
                //string id and source language should be valid
                status = FALSE;
                break;
            }

            string_id = string_array.GetAt(0);
            src_string = string_array.GetAt(1);

            for (col = 2; col < header_array.GetCount(); col++)
            {
                target_language = header_array.GetAt(col);
                language_index = config_languages_dlg::GetLanguageIdIndex(target_language);
                target_table_column = config_languages_dlg::GetStringTableIndexFromLanguageIndex(project, language_index);

                if (target_table_column == -1)
                {
                    // the translation language doesn't exist in our project, so add it
                    //target_table_column = AddNewLanguage(language_index);
                    target_table_column = project->AddLanguage(language_index);
                }

                if (col < string_array.GetCount())
                {
                    target_string = string_array.GetAt(col);
                }
                else
                {
                    target_string = _T("");
                }
                table->ImportString(string_id, src_string, target_string, notes, src_table_column, target_table_column);
            }
        }
    }
    else
    {
        status = FALSE;
    }

    delete[]mpBuffer;
    mpBuffer = NULL;
    mpLine = NULL;

    table->Sort();
    table->UpdateGuixLanguageTable();
    return status;
}

///////////////////////////////////////////////////////////////////////////////
CString csv_read_write::MakeCsvFormatValue(CString string)
{
    int index;
    CString format_string;
    BOOL embedded_special_character = FALSE;

    for (index = 0; index < string.GetLength(); index++)
    {
        switch (string.GetAt(index))
        {
        case '\"':
        case '\r':
        case '\n':
        case ',':
            embedded_special_character = TRUE;
            break;
        }
    }

    //replace single double quotation with a pair of consecutive double quotes
    string.Replace(_T("\""), _T("\"\""));

    if (embedded_special_character)
    {
        //put double quotation around string
        string.Insert(0, '\"');
        string.Append(_T("\""));
    }

    return string;
}

///////////////////////////////////////////////////////////////////////////////
void csv_read_write::WriteString(CStdioFile *file, CString string)
{
    // The maximun UTF8 character is 6 bytes, calculate the maximun utf8 buffer size needed for the string.
    int char_count = _tcslen(string) * 6 + 1;
    char *utf8buf = new char[char_count];
    strcpy_s(utf8buf, char_count, CT2A(string, CP_UTF8));
    file->Write(utf8buf, strlen(utf8buf));

    delete utf8buf;
}

///////////////////////////////////////////////////////////////////////////////
int csv_read_write::FindDelimiterIndex(CString line, int start_index)
{
    int quatation_mark_count = 0;

    while (start_index < line.GetLength())
    {
        switch (line.GetAt(start_index))
        {
        case '\"':
            quatation_mark_count++;
            break;

        case ',':
            if (!(quatation_mark_count & 0x1))
            {
                //quotation mark should be paired
                return start_index;
            }
            break;
        }

        start_index++;
    }

    if (!(quatation_mark_count & 0x1))
    {
        //quotation mark should be paired
        return start_index;
    }

    return -1;
}

///////////////////////////////////////////////////////////////////////////////
void csv_read_write::SplitCsvLine(CString &line, CStringArray &line_array)
{
    int index = 0;
    int length = line.GetLength();
    int start, end;
    CString string;

    while (index < length)
    {
        start = index;
        end = FindDelimiterIndex(line, start) - 1;

        if (end < 0)
        {
            //should not happen
            return;
        }

        index = end + 2;

        if (line.GetAt(start) == '\"')
        {
            //remove double quotation mark around the string
            start += 1;
            end -= 1;
        }

        string = line.Mid(start, end - start + 1);

        //replace 2 double colon to 1 double colon
        string.Replace(_T("\"\""), _T("\""));

        line_array.Add(string);
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL csv_read_write::ReadLine(CString &line)
{
    int double_quotation_count = 0;

    if (!mpLine)
    {
        return FALSE;
    }

    while (mpLine[0] && ((mpLine[0] == '\r') || (mpLine[0] == '\n')))
    {
        mpLine++;
    }

    int line_end = 0;

    while (mpLine[line_end])
    {
        if ((mpLine[line_end] == '\r') || (mpLine[line_end] == '\n'))
        {
            if (!(double_quotation_count & 0x01))
            {
                //double quotation mark should be paired
                break;
            }
        }
        else if (mpLine[line_end] == '\"')
        {
            double_quotation_count++;
        }

        line_end++;
    }

    if (line_end)
    {
        char *buf = new char[line_end + 1];
        strncpy(buf, mpLine, line_end);
        buf[line_end] = '\0';

        line = CA2T(buf, CP_UTF8);
        mpLine += line_end + 1;

        delete[] buf;
        return TRUE;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void csv_read_write::ReadFile(CString pathname)
{
    CStdioFile file(pathname, CStdioFile::modeRead | CFile::shareExclusive);
    ULONG size = (ULONG)file.GetLength();
    mpBuffer = new char[size + 1];
    memset(mpBuffer, 0, size + 1);
    file.Read(mpBuffer, size);

    mpLine = mpBuffer;

    if (size >= 3)
    {
        if ((unsigned char)mpLine[0] == 0xef &&
            (unsigned char)mpLine[1] == 0xbb &&
            (unsigned char)mpLine[2] == 0xbf)
        {
            //skip BOM mark
            mpLine += 3;
        }
    }
}