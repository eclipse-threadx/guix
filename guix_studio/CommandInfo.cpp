
#include "studiox_includes.h"
#include <time.h>

CString DispHelp(
    "Usage: guix_studio [OPTION] [ARGUMENT]\n"
    "Open .gxp project.\n"
    "Load specified project and generate ALL output files.\n"
    "Load specified project and generate specified output files."
    "\n"
    "  -n, --nogui                                Run the command line, do not \n"
    "                                              start the Studio UI interface.\n"
    "  -o pathname, --log                         Specify a log file.\n"
    "  -b, --binary                               Generate binary format resource.\n"
    "  -d display_1, display_2, .. --display      Specify display names included in any generated reosurce file.\n"
    "                                              Otherwise all display are included.\n"
    "  -t theme1, theme2, ..  --theme             Specify theme names included in the generated reosource file,\n"
    "                                              if the option is not used, all themes will be included.\n"
    "  -l langage1, language2, ..  --language     Specify language names included in the generated resource file,\n"
    "                                              if the option is not used, all languages will be included.\n"
    "  -r filename, --resource                    Specify that studio should produce a resource file.\n"
    "  -s filename, --specification               Specify that studio should produce a specification file\n"
    "  -p project_pathname                        Specify project pathname.\n"
    "  -i pathname, --import                      Import string from xliff or csv format file\n"
    "  --big_endian                               Generate data as big endian.\n"
    "  --no_res_header                            Not generating resource header.\n"
    "  -x pathname, --xml                         Specify the input resource XML file.\n"
    "  --output_path pathname                     Specify the output directory. If not specified, the project directory will be used for the output files.\n"
    "\n"
    "Examples:\n"
    "  demo.gxp                                      Open demo.gxp project.\n"
    "  guix_studio -p demo.gxp                       Open demo.gxp project.\n"
    "  guix_studio -n -r -s -p demo.gxp              Generate All output files of demo.gxp project.\n"
    "  guix_studio -x resource.xml -b                Generate binary resource file from a resource project resource.xml.\n"
    );

// Options for internal use.
//     --clone_guix_repo local_path                   Run GUIX repository clone.
//     --write_pipe_handle handle                     Specify named pipe memory write handle.

CCommandInfo::CCommandInfo(void)
{
    nogui = FALSE;
    binary = FALSE;
    xml = FALSE;
    log_file = NULL;
    gen_resource = FALSE;
    gen_res_header = TRUE;
    gen_specification = FALSE;
    big_endian = FALSE;

    num_of_language = -1;
    num_of_theme = -1;
    num_of_display = -1;

    string_import_filename = "";
    output_path = "";
    guix_repo_local_path = "";
    guix_clone_write_pipe_handle = 0;
}


CCommandInfo::~CCommandInfo(void)
{
}

BOOL CCommandInfo::ReadArgument(CString &cmd, CString option, CString &argument)
{
    int index = cmd.Find(L" " + option + L" ");
    if (index >= 0)
    {
        cmd.Replace(option, _T(""));

        argument = cmd.Mid(index + 1);
        index = argument.Find(_T(" -"));

        if (index >= 0)
        {
            argument = argument.Left(index);
        }

        cmd.Replace(argument, _T(""));

        // Trim all leading and tailing whitespaces.
        argument.Trim();
        return TRUE;
    }

    return FALSE;
}

void CCommandInfo::ReadCommandLineInfo(LPTSTR m_lpCmdLine)
{
int index;

    CString cmd(m_lpCmdLine);
    CString option;
    CString argument;
    CString error_msg("");
    BOOL show_help = FALSE;
    BOOL set_project = FALSE;

    cmd.Insert(0, _T(" "));
    cmd.Append(_T(" "));

    // Read GUIX repo local path.
    if(ReadArgument(cmd, L"--clone_guix_repo", guix_repo_local_path))
    {
        // Trim all leading and trailing quotation marks.
        guix_repo_local_path.Trim(L"\"");

        // Read named pipe memory write handle.
        if(ReadArgument(cmd, L"--write_pipe_handle", argument))
        {
            guix_clone_write_pipe_handle = (HANDLE)_wtoi(argument);
        }
        return;
    }

    /* Read .gxp project pathname. */
    index = cmd.Find(_T(".gxp"));
    if (index >= 0)
    {
        CString pathname = cmd.Left(index + 4);
        index = pathname.Find(_T(" -p "));

        if (index < 0)
        {
            index = pathname.ReverseFind(' ');
            pathname = pathname.Mid(index);

            cmd.Replace(pathname, _T(""));

            ReadPathName(pathname, project_path, project_name);
        }
    }

    /* Find first option. */
    index = cmd.Find(_T(" -"));

    while (index >= 0)
    {
        cmd = cmd.Mid(index + 1);

        index = cmd.Find(_T(" "));

        // pick up option
        option = cmd.Left(index);

        // remove option from command line string
        cmd = cmd.Mid(index);

        index = cmd.Find(_T(" -"));

        if (index >= 0)
        {
            // pick up argument
            argument = cmd.Left(index);
        }
        else
        {
            argument = cmd;
        }

        if (option == "-h" || option == "--help")
        {
            show_help = TRUE;
        }
        else if (option == "-o" || option == "--log")
        {
            ReadPathName(argument, log_pathname);
        }
        else if (option == "-p" || option == "--project")
        {
            ReadPathName(argument, project_path, project_name);
        }
        else if (option == "-n" || option == "--nogui")
        {
            nogui = TRUE;
        }
        else if (option == "-b" ||option == "--binary")
        {
            binary = TRUE;
        }
        else if (option == "-x" || option == "--xml")
        {
            ReadPathName(argument, project_path, project_name);

            xml = TRUE;
            nogui = TRUE;
            gen_specification = FALSE;
            gen_resource = TRUE;
            gen_res_header = FALSE;

            // Set default output resource file name.
            if (resource_filename.IsEmpty())
            {
                resource_filename = RemoveFileExtension(project_name);
            }
        }
        else if (option == "--big_endian")
        {
            big_endian = TRUE;
        }
        else if (option == "-r" || option == "--resource")
        {
            ReadPathName(argument, resource_filename);
            gen_resource = TRUE;
        }
        else if (option == "--no_res_header")
        {
            gen_res_header = FALSE;
        }
        else if (option == "-s" || option == "--specification")
        {
            ReadPathName(argument, specification_filename);
            gen_specification = TRUE;
        }
        else if (option == "-l" || option == "--language")
        {
            ReadMultiParameter(argument, language, num_of_language);
        }
        else if (option == "-t" || option == "--theme")
        {
            ReadMultiParameter(argument, theme, num_of_theme);
        }
        else if (option == "-d" || option == "--display")
        {
            ReadMultiParameter(argument, display, num_of_display);
        }
        else if (option == "-i" || option == "--import")
        {
            ReadPathName(argument, string_import_filename);
        }
        else if (option == "--output_path")
        {
            ReadPathName(argument, output_path);
        }
        else
        {
            error_msg.Format(L"\nCommand: %s\nNo such option: [%s]\n", m_lpCmdLine, option);
        }
    }

    if (!log_pathname.IsEmpty())
    {
        if (log_pathname.GetAt(1) != ':')
        {
            CString path = output_path;

            if (!path.IsEmpty())
            {
                if (path.GetAt(1) != ':')
                {
                    // If output path is a relative path, concat it to project path as the log file path.
                    path = ConcatPathNames(project_path, path);
                }
            }
            else
            {
                // Use project path as the log file path.
                path = project_path;
            }
            
            log_pathname = ConcatPathNames(path, log_pathname);
        }

        /* Open the log file. */
        log_file = _tfopen(log_pathname, _T("a"));

        if (log_file)
        {
            struct tm* newTime;
            time_t      szClock;

            // Get time in seconds
            time(&szClock);

            // Convert time to struct tm form
            newTime = localtime(&szClock);

            // Print local time as a string.
            fprintf(log_file, "\nDate: %s\n", asctime(newTime));
        }
    }

    if (log_file && show_help)
    {
        // Log help information.
        fprintf(log_file, "\n%s\n", (char*)CW2A(DispHelp));

        exit(0);
    }

    if (project_name.IsEmpty())
    {
        if (xml)
        {
            error_msg.Append(L"\nMissing xml filename!\n");
        }
        else
        {
            error_msg.Append(L"\nMissing gxp project filename!\n");
        }
    }

    if (!error_msg.IsEmpty())
    {
        if (log_file)
        {
            fprintf(log_file, "\n%s\n", (char*)CW2A(error_msg));
        }
        exit(0);
    }

    if ((gen_resource == FALSE) && (gen_specification == FALSE))
    {
        gen_resource = TRUE;
        gen_specification = TRUE;
    }
}

void CCommandInfo::ReadMultiParameter(CString argument, CString parameter[], int &num)
{
    num = 0;

    argument.Remove(' ');

    CStringArray arrays;
    SplitString(argument, ',', &arrays);

    for (int index = 0; index < arrays.GetCount(); index++)
    {
        parameter[num++] = arrays.GetAt(index);
    }
}

void CCommandInfo::ReadPathName(CString cmd, CString &path, CString &name)
{
int index;

    // Trims leading and trailing whitespace characters from the string.
    cmd.Trim();

    // Trims leading and trailing quatation mark characters from the string.
    cmd.Trim('\"');

    name = cmd;

    FormatPath(name);
    index = name.ReverseFind('\\');

    if (index > 0)
    {
        path = name.Left(index);
        name = name.Mid(index + 1);
    }
    else
    {
        path = ".";
    }
}

void CCommandInfo::ReadPathName(CString argument, CString &pathname)
{
    // Trims leading and trailing whitespace characters from the string.
    argument.Trim();

    // Trims leading and trailing quatation mark characters from the string.
    argument.Trim('\"');

    pathname = argument;
}

BOOL CCommandInfo::IsThemeEnabled(CString name)
{
    if ((num_of_theme == -1) || FindString(theme, num_of_theme, name))
    {
        return TRUE;
    }

    return FALSE;
}

BOOL CCommandInfo::IsLanguageEnabled(CString name)
{
    if ((num_of_language == -1) || FindString(language, num_of_language, name))
    {
        return TRUE;
    }

    return FALSE;
}

BOOL CCommandInfo::IsDisplayEnabled(CString name)
{
    if ((num_of_display == -1) || FindString(display, num_of_display, name))
    {
        return TRUE;
    }

    return FALSE;
}

BOOL CCommandInfo::FindString(CString str_table[], int table_size, CString find_str)
{
    int index;

    for (index = 0; index < table_size; index++)
    {
        if (!find_str.CompareNoCase(str_table[index]))
        {
            return TRUE;
        }
    }
    return FALSE;
}

