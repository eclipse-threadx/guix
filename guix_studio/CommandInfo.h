#pragma once
class CCommandInfo
{
public:
    CCommandInfo(void);
    ~CCommandInfo(void);

public:
    BOOL IsNoGui(){ return nogui; };
    BOOL IsBinaryMode(){ return binary; };
    BOOL IsXmlMode(){ return xml; };
    BOOL IsBigEndian(){ return big_endian; };
    BOOL GenResource(){ return gen_resource; };
    BOOL GenResHeader(){ return gen_res_header; };
    BOOL GenSpecification() { return gen_specification; };
    int  GetLanguageCount(){ return num_of_language; };
    int  GetThemCount(){ return num_of_theme; };
    int  GetDisplayCount(){ return num_of_display; };
    CString GetProjectPath(){ return project_path; };
    CString GetProjectName(){ return project_name; };
    CString GetResourceFileName(){ return resource_filename; };
    CString GetSpecificationFileName(){ return specification_filename; };
    CString GetStringImportFileName(){ return string_import_filename; };
    CString GetOutputPath() { return output_path; };
    CString GetGuixRepoLocalPath() { return guix_repo_local_path; }
    HANDLE GetGuixCloneWritePipeHandle() { return guix_clone_write_pipe_handle; }
    FILE   *GetLogFile(){ return log_file; };

private:
    BOOL    nogui;
    BOOL    binary;
    BOOL    xml;
    BOOL    big_endian;
    BOOL    gen_resource;
    BOOL    gen_res_header;
    BOOL    gen_specification;
    BOOL    playback_macro;
    int     num_of_language;
    int     num_of_theme;
    int     num_of_display;
    CString log_pathname;
    CString project_path;
    CString project_name;
    CString resource_filename;
    CString specification_filename;
    CString string_import_filename;
    CString output_path;
    CString guix_repo_local_path;
    HANDLE  guix_clone_write_pipe_handle;
    CString language[MAX_LANGUAGES];
    CString theme[MAX_THEMES];
    CString display[MAX_DISPLAYS];
    FILE   *log_file;

public:
    void ReadCommandLineInfo(LPTSTR m_lpCmdLine);
    void ReadMultiParameter(CString cmd, CString parameter[], int &num);
    void ReadPathName(CString cmd, CString &path, CString &name);
    void ReadPathName(CString cmd, CString &pathname);
    BOOL IsThemeEnabled(CString name);
    BOOL IsLanguageEnabled(CString name);
    BOOL IsDisplayEnabled(CString name);
    BOOL ReadArgument(CString &cmd, CString option, CString &argument);

private:
    BOOL FindString(CString str_table[], int table_size, CString find_str);
};

