
#ifndef _STUDIOX_UTIL_
#define _STUDIOX_UTIL_

#include "project_view.h"
#include "target_view.h"
#include "resource_view.h"
#include "undo_manager.h"
#include "CommandInfo.h"
#include "studiox_screen_driver.h"

#define INI_FILE_NAME   _T("GUIX_Studio.ini")

/* handy utility functions that are used at various places within studiox */

BOOL BrowseForFolder(HWND hwnd, LPCTSTR caption, LPCTSTR szCurrent, LPTSTR szPath);
BOOL BrowseForSingleFile(LPCTSTR caption, LPCTSTR filter,
    LPCTSTR def_extension, CString &szPath, CWnd *parent);
int BrowseForMultipleFiles(TCHAR *caption, TCHAR *filter,
                           TCHAR *def_extension, TCHAR **pathptr, TCHAR **nameptr, CWnd *parent);

BOOL GetOutputFileName(TCHAR *SavePathname, TCHAR *SaveFilename,
                       TCHAR *title, TCHAR *filter, TCHAR *initial_dir,
                       TCHAR *def_extension, CWnd *parent = NULL);

BOOL ConvertToProjectRelativePath(CString &path);
void SaveToPathInfo(PATHINFO &info, CString &dirname);
CString ConcatPathNames(CString &root, CString &extra);
CString MakeAbsolutePathname(PATHINFO &info);

void GotoProjectDirectory();
void SetProjectDirectory(CString &path);
CString RemoveFileExtension(CString &filename);
BOOL CheckOutputFileSecurity(CString &pathname, BOOL binary_mode = 0);


wchar_t *CStringToWideChar(CString &string);
void pixelmap_destroy(GX_PIXELMAP *pixelmap);
void pixelmap_list_destroy(CArray<GX_PIXELMAP *> &pixelmap_list);
void SelectDropListItem(CComboBox *box, long item_val);

void MakePath(CString path);
void WriteToDosFile(CFile &outfile, CString &out);
FILE *OpenWorkingDirFile(CString filename, const char *mode);
CString GetSamplesDir();
CString GetStudioInstallFolder();
CString GetMallocName();
CString GetTestingParam(int index);
CString GetAppDataPath();

void ErrorMsg(const char *msg, CWnd *parent = NULL);
void ErrorMsg(const CString &msg, CWnd *parent = NULL);
void Notify(const char *msg, CWnd *parent = NULL);
BOOL StartWorkThread(LPTHREAD_START_ROUTINE routine, LPVOID param, const char *msg, BOOL block_canvas_refresh = FALSE);
void BusyMsg(const char *msg, HANDLE thread);
void EndBusyMsg(void);
BOOL IsWaitDialogRunning();
BOOL IsGuixCanvasRefreshBlocked();

BOOL AskUser(const char *msg, CWnd *parent = NULL);
BOOL ReadIniInfo(void);
void WriteIniInfo(void);
void AddRecentProject(CString &pathname);

studiox_project *GetOpenProject();
void SetOpenProject(studiox_project *open_project);

project_view  *GetProjectView();
target_view   *GetTargetView();
target_screen *GetTargetScreen();
resource_view *GetResourceView();
properties_win *GetPropsWin();
CCommandInfo   *GetCmdInfo();
string_table *GetActiveStringTable();
FontCharMap *GetActiveCharacterMap(BOOL reset);
undo_manager *UndoManager();

GX_COLOR ColorRefToGxColor(COLORREF cr);
COLORREF GxColorToColorRef(GX_COLOR gx);
BOOL IsAlphaFormat(int color_format);

BOOL IsCpuWithDave2D(int cpu_type);                             // does the CPU support Dave2D?
BOOL IsCpuWithDave2D(studiox_project *);                        // does the CPU support Dave2D?
BOOL IsRenesasDave2D(studiox_project *);                        // Is the project configured with Dave2D enabled?
BOOL IsDave2dFontFormat(studiox_project* project, int display); // Are we using custom Dave2D font format?
BOOL IsSTChromeArt(studiox_project*);                           // Is the project configured with ST ChromeArt enabled?
BOOL IsRenesasHwJpeg(studiox_project *);
BOOL IsInNameList(CArray<CString> *name_list, CString &name);
int  project_lib_version();

void RleEncodeGlyphData(GX_COMPRESSED_GLYPH* glyph, int bits_per_pix);
GX_UBYTE* RleDecodeGlyphData(GX_COMPRESSED_GLYPH* glyph, int bits_per_pix);
GX_FONT *MakeFont(res_info *info, int display, BOOL warn_on_error);
GX_FONT *MakeOptimizedFont(res_info *info, int display, BOOL warn_on_error = FALSE);
BOOL InsertStringTableGlyphs(FontCharMap *map, res_info *info, int display);
INT  GetRowPitch(INT width, INT bits_per_pix);
INT  GetFontBits(INT font_format);
BOOL IsFontBitsSupported(INT font_bits);
INT  GetFontStorage(res_info *info, studiox_project *project, int display);
INT  GetPixelmapStorage(res_info *info);
VOID FormatPath(CString &path);
BOOL FileExists(INT pathtype, CString pathname);
BOOL FileExists(INT display_index, INT resource_type, CString pathname);
BOOL NameExists(INT display_index, INT resource_type, CString name);
BOOL SetImportIdName(INT display_index, CString &input_name);
BOOL TestInputName(CEdit *input_field, char *field_name, CString &original_value, CWnd *parent = NULL);
BOOL TestInputName(CString &name, char *field_name, CWnd *parent = NULL, BOOL show_error_message = TRUE);
BOOL IsFileNameFormat(CString name);
CString MakePixelmapName(res_info *info, int frame_id);
ULONG GetTextScaler();
int GetScaledValue(int value, int dpi = DEFAULT_DPI_96, int text_scaler = DEFAULT_TEXT_SCALER);
INT GetSubMenuIndex(CMenu *pMenu, CString string);
INT GetSubMenuIndex(CMenu *pMenu, INT id);

int CalculateStudioVersion();
int GuixVersionFieldsToVersionNumber(int major, int minor, int service_pack);
void GuixVersionNumberToVersionFields(int version_number, int &major, int &minor, int &service_pack);

void SetUtf8Text(CRichEditCtrl *edit, CString &text);
CString GetUtf8Text(CRichEditCtrl *edit);

void DestroyFont(GX_FONT *font);

void PaintBmp(CDC *dc, int x, int y, int icon_id);
CString NumberFormatWithCommas(long value);
void SplitString(CString str, CHAR splitter, CStringArray *list);
int GetSystemDPI();
GX_UBYTE ToUByte(INT value);
int FindPairVal(STRING_VAL_PAIR* entry, CString name);
CString FindPairString(STRING_VAL_PAIR* entry, int val);

template <class T>
bool IsItemInArray(const CArray<T> &target, const T &item)
{
    for (int index = 0; index < target.GetSize(); index++)
    {
        if (target.GetAt(index) == item) return true;
    }

    return false;
};

#define WIN_BACKGROUND_COLOR  RGB(255, 255, 255)
#define HIGH_TEXT_COLOR       RGB(0, 0, 255)
#define NORMAL_TEXT_COLOR     RGB(0, 0, 0)
#define LINE_COLOR            RGB(180, 180, 180)

#define MAX_RECENT_PROJECTS 5
#define MAX_OPEN_FILES      128

#define STUDIO_INI_VERSION  2

typedef struct {
    int ini_version;
    int studio_version;

    int first_run;
    int xpos;
    int ypos;
    int width;
    int height;
    int proj_view_width;
    int resource_view_width;
    int proj_view_height;
    CString samples_dir;        // guix repo clone dir, or dir to which we installed GUIX system resources.
    CString recent_project_paths[MAX_RECENT_PROJECTS];
} INI_INFO;

typedef struct{
    int ini_version;
    int xpos;
    int ypos;
    int width;
    int height;
    int proj_view_width;
    int resource_view_width;
    BOOL color_open;
    BOOL font_open;
    BOOL pixelmap_open;
    BOOL string_open;
    int  res_view_scrollpos;
    int  proj_view_scrollpos;
    int  props_win_scrollpos;
    int  proj_tree_item_count;
} MACRO_HEAD_INFO;

#endif