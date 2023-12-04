

#include "studiox_includes.h"
#include "xml_writer.h"
#include "xml_reader.h"
#include "xliff_read_write.h"
#include "config_languages_dlg.h"

///////////////////////////////////////////////////////////////////////////////
xliff_read_write::xliff_read_write()
{
}

///////////////////////////////////////////////////////////////////////////////
BOOL xliff_read_write::ExportLanguage(studiox_project *project)
{
    int index;
    int src_language = project->mHeader.string_export_src;
    int target_language = project->mHeader.string_export_target;
    CString tag_data;
    CString src_data;
    CString pathname = project->mHeader.string_export_path;
    pathname += "\\";
    pathname += project->mHeader.string_export_filename;
    
    xml_writer *writer = new xml_writer();

    GotoProjectDirectory();
    if (!writer->OpenFile(CString(pathname)))
    {
        delete writer;
        return FALSE;
    }
    writer->WriteHeader(NULL);

    int display = GetProjectView()->GetActiveDisplay();
    string_table *table = project->mDisplays[display].stable;

    if (project->mHeader.string_export_version >= 2)
    {
        tag_data.Format(_T("xmlns=\"urn:oasis:names:tc:xliff:document:2.0\" version=\"2.0\"\n  srcLang =\"%s\" trgLang=\"%s\""),
                config_languages_dlg::GetLanguageId(src_language),
                config_languages_dlg::GetLanguageId(target_language));
                
        writer->OpenTag("xliff", tag_data);
        tag_data.Format(_T("id=\"f1\" original=\"%s\\%s.gxp\""),
            project->mHeader.project_path,
            project->mHeader.project_name);

        // KGM FIXME: Are we going to keep one string table or one 
        // table for each display? Currently we are keeping a table for each display
        // int the project.... not sure but I'm not consistent about this currently.

        writer->OpenTag("file", tag_data);

        src_language = config_languages_dlg::GetStringTableIndexFromLanguageIndex(project, src_language);
        target_language = config_languages_dlg::GetStringTableIndexFromLanguageIndex(project, target_language);

        if (src_language == -1)
        {
            delete writer;
            return FALSE;
        }

        for (index = 0; index < table->CountStrings(); index++)
        {
            src_data = table->GetString(index, src_language);
            if (!src_data.IsEmpty())
            {
                tag_data.Format(_T("id=\"%d\" name=\"%s\""), index, table->GetStringId(index));
                writer->OpenTag("unit", tag_data);
                if (!(table->GetNotes(index).IsEmpty()))
                {
                    //write notes
                    writer->OpenTag("notes");
                    writer->WriteString("note", table->GetNotes(index));
                    writer->CloseTag("notes");
                }
                writer->OpenTag("segment");
                writer->WriteString("source", src_data);
                if (target_language >= 0)
                {
                    writer->WriteString("target", table->GetString(index, target_language));
                }
                writer->CloseTag("segment");
                writer->CloseTag("unit");
            }
        }
    }
    else
    {
        tag_data = "version=\"1.2\" xmlns=\"urn:oasis:names:tc:xliff:document:1.2\"";
        writer->OpenTag("xliff", tag_data);

        tag_data.Format(_T("original=\"%s\\%s.gxp\" source-language=\"%s\" target-language=\"%s\" datatype=\"plaintext\""),
                project->mHeader.project_path,
                project->mHeader.project_name,
                config_languages_dlg::GetLanguageId(src_language),
                config_languages_dlg::GetLanguageId(target_language));

        writer->OpenTag("file", tag_data);
        writer->OpenTag("body");

        src_language = config_languages_dlg::GetStringTableIndexFromLanguageIndex(project, src_language);
        target_language = config_languages_dlg::GetStringTableIndexFromLanguageIndex(project, target_language);

        for (index = 0; index < table->CountStrings(); index++)
        {
            src_data = table->GetString(index, src_language);
            if (!src_data.IsEmpty())
            {
                tag_data.Format(_T("id=\"%s\""), table->GetStringId(index));
                writer->OpenTag("trans-unit", tag_data);
                writer->WriteString("source", src_data);
                if (target_language >= 0)
                {
                    writer->WriteString("target", table->GetString(index, target_language));
                }

                if (!(table->GetNotes(index).IsEmpty()))
                {
                    //write notes
                    writer->WriteString("note", table->GetNotes(index));
                }
                writer->CloseTag("trans-unit");
            }
        }
        writer->CloseTag("body");
    }
    writer->CloseTag("file");
    writer->CloseTag("xliff");
    writer->CloseFile();
    delete writer;
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL xliff_read_write::ImportXliffFile(studiox_project *project, CString &pathname)
{
    CString src_language;
    CString target_language;
    CString string_id;
    CString src_string;
    CString target_string;
    CString notes;

    xml_reader reader;
    int language_index;
    int target_table_column;
    int src_table_column;

    GotoProjectDirectory();
    if (!reader.ReadFile(pathname))
    {
        return FALSE;
    }

    int display = GetProjectView()->GetActiveDisplay();
    string_table *table = project->mDisplays[display].stable;

    // Get the version information

    if (!reader.EnterSection("xliff"))
    {
        return FALSE;
    }

    double version = reader.GetTagValueFloat("version", -1.0);
    if (version < 0)
    {
        return false;
    }
    if (version >= 2.0)
    {
        if (!reader.GetTagString("srcLang", src_language) ||
            !reader.GetTagString("trgLang", target_language))
        {
            return FALSE;
        }

        language_index = config_languages_dlg::GetLanguageIdIndex(src_language);
        src_table_column = config_languages_dlg::GetStringTableIndexFromLanguageIndex(project, language_index);

        language_index = config_languages_dlg::GetLanguageIdIndex(target_language);
        target_table_column = config_languages_dlg::GetStringTableIndexFromLanguageIndex(project, language_index);
            
        if (src_table_column == -1)
        {
            return FALSE;
        }
        if (target_table_column == -1)
        {
            // the translation language doesn't exist in our project, so add it
            //target_table_column = AddNewLanguage(language_index);
            target_table_column = project->AddLanguage(language_index);
        }

        if (reader.EnterSection("file"))
        {
            while(reader.EnterSection("unit"))
            {
                reader.GetTagString("name", string_id);

                if (reader.EnterSection("notes"))
                {
                    reader.ReadString("note", notes);
                    reader.CloseSection(TRUE, TRUE);
                }
                else
                {
                    notes = "";
                }

                while (reader.EnterSection("segment"))
                {
                    reader.ReadString("source", src_string);
                    reader.ReadString("target", target_string);

                    table->ImportString(string_id, src_string, target_string, notes, src_table_column, target_table_column);
                    reader.CloseSection(TRUE, TRUE);
                }
                reader.CloseSection(TRUE, TRUE);
            }
            reader.CloseSection();
        }
    }
    else
    {
        // version 1.2- The file tag contains language info, and the body contains trans-unit tags:
        
        if (reader.EnterSection("file"))
        {
            reader.GetTagString("source-language", src_language);
            reader.GetTagString("target-language", target_language);

            language_index = config_languages_dlg::GetLanguageIdIndex(src_language);
            src_table_column = config_languages_dlg::GetStringTableIndexFromLanguageIndex(project, language_index);

            language_index = config_languages_dlg::GetLanguageIdIndex(target_language);
            target_table_column = config_languages_dlg::GetStringTableIndexFromLanguageIndex(project, language_index);
            
            if (src_table_column == -1)
            {
                return FALSE;
            }
            if (target_table_column == -1)
            {
                target_table_column = project->AddLanguage(language_index);
            }

            if (reader.EnterSection("body"))
            {
                while (reader.EnterSection("trans-unit"))
                {
                    reader.GetTagString("id", string_id);
                    reader.ReadString("source", src_string);
                    reader.ReadString("target", target_string);
                    reader.ReadString("note", notes);

                    table->ImportString(string_id, src_string, target_string, notes, src_table_column, target_table_column);

                    reader.CloseSection(TRUE, TRUE);
                }
                reader.CloseSection();
            }
            reader.CloseSection();
        }
    }
    reader.CloseSection();
    table->Sort();
    table->UpdateGuixLanguageTable();
    return TRUE;
}