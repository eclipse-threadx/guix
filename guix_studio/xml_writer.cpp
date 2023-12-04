
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
xml_writer::xml_writer()
{
    mpFile = NULL;
    UsingMemFile = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
xml_writer::~xml_writer()
{
    if (mpFile)
    {
        CloseFile();
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL xml_writer::OpenFile(const CString &pathname)
{
    CStdioFile *ofile = new CStdioFile();
    if (!ofile->Open(pathname,
        CFile::modeCreate|CFile::modeWrite|CFile::typeText))
    {
        delete ofile;
        return FALSE;
    }
    mpFile = ofile;
    UsingMemFile = FALSE;
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void xml_writer::OpenMemFile(CFile *file)
{
    mpFile = file;
    UsingMemFile = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void xml_writer::CloseFile(void)
{
    if (mpFile)
    {
        if (!UsingMemFile)
        {
            mpFile->Flush();
            mpFile->Close();
            delete mpFile;
        }
        mpFile = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
void xml_writer::WriteHeader(const char *docType)
{
    StringToFile(_T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"));

    if (docType)
    {
        CString cs(docType);

        StringToFile(_T("<!DOCTYPE "));
        StringToFile(cs);
        StringToFile(_T(">\n"));
    }
}

///////////////////////////////////////////////////////////////////////////////
void xml_writer::WriteInt(const char *name, int val)
{
    CString cVal;
    cVal.Format(_T("%d"), val);
    WriteString(name, cVal);
}

///////////////////////////////////////////////////////////////////////////////
void xml_writer::WriteUnsigned(const char *name, ULONG val)
{
    CString cVal;
    cVal.Format(_T("%u"), val);
    WriteString(name, cVal);
}

///////////////////////////////////////////////////////////////////////////////
void xml_writer::WriteUByte(const char *name, GX_UBYTE val)
{
    CString cVal;
    cVal.Format(_T("%u"), val);
    WriteString(name, cVal);
}


///////////////////////////////////////////////////////////////////////////////
void xml_writer::WriteHex(const char *name, ULONG val)
{
    CString cVal;
    cVal.Format(_T("%08x"), val);
    WriteString(name, cVal);
}

///////////////////////////////////////////////////////////////////////////////
void xml_writer::WriteRect(const char *name, GX_RECTANGLE &rect)
{
    OpenTag(name);
    WriteInt("left", rect.gx_rectangle_left);
    WriteInt("top", rect.gx_rectangle_top);
    WriteInt("right", rect.gx_rectangle_right);
    WriteInt("bottom", rect.gx_rectangle_bottom);
    CloseTag(name);
}

///////////////////////////////////////////////////////////////////////////////
void xml_writer::WriteBool(const char *name, BOOL val)
{
    OpenTag(name, TRUE);

    if (val)
    {
        StringToFile(_T("TRUE"));
    }
    else
    {
        StringToFile(_T("FALSE"));
    }
    CloseTag(name);
}

///////////////////////////////////////////////////////////////////////////////
void xml_writer::WriteString(const char *name, const char *val, BOOL bForce)
{
    CString cVal(val);
    WriteString(name, cVal, bForce);
}

///////////////////////////////////////////////////////////////////////////////
void xml_writer::WriteString(const char *name, const CString &cVal, BOOL bForce)
{
    if (!bForce && cVal.IsEmpty())
    {
        return;
    }

    // escape out the special characters
    // " &quot
    // ' &apos
    // < &lt
    // > &gt
    // & &amp
    CString escaped(cVal);
    escaped.Replace(_T("&"), _T("&amp;"));
    escaped.Replace(_T("\""), _T("&quot;"));
    escaped.Replace(_T("\'"), _T("&apos;"));
    escaped.Replace(_T("<"), _T("&lt;"));
    escaped.Replace(_T(">"), _T("&gt;"));
    
    OpenTag(name, TRUE);
    if (!escaped.IsEmpty())
    {
        // convert unicode to utf8
        // The maximun UTF8 character is 6 bytes, calculate the maximun utf8 buffer size needed for the string.
        int buffer_size = escaped.GetLength() * 6 + 1;
        char *utf8buf = new char[buffer_size];
        strcpy_s(utf8buf, buffer_size, CT2A(escaped.GetString(), CP_UTF8));
        mpFile->Write(utf8buf, strlen(utf8buf));
        delete[] utf8buf;

    }
    CloseTag(name);
}

///////////////////////////////////////////////////////////////////////////////
void xml_writer::WritePathInfo(const PATHINFO &info)
{
    OpenTag("pathinfo");
    if (!info.pathname.IsEmpty())
    {
        WriteString("pathname", info.pathname);
    }
    switch(info.pathtype)
    {
    case PATH_TYPE_PROJECT_RELATIVE:
        WriteString("pathtype", "project_relative");
        break;

    case PATH_TYPE_INSTALL_RELATIVE:
        WriteString("pathtype", "studio_relative");
        break;

    default:
        WriteString("pathtype", "absolute");
        break;
    }
    CloseTag("pathinfo");
}

///////////////////////////////////////////////////////////////////////////////
void xml_writer::OpenTag(const char *tag, BOOL has_content)
{
    CString cTag(tag);
    StringToFile(_T("<"));
    StringToFile(cTag);

    if (has_content)
    {
        StringToFile(_T(">"));
    }
    else
    {
        StringToFile(_T(">\n"));    
    }
}

///////////////////////////////////////////////////////////////////////////////
void xml_writer::OpenTag(const char *tag, CString &content)
{
    CString cTag(tag);
    StringToFile(_T("<"));
    StringToFile(cTag);

    if (!content.IsEmpty())
    {
        cTag = " ";
        cTag += content;
        StringToFile(cTag);
    }
    StringToFile(_T(">\n"));    
}

///////////////////////////////////////////////////////////////////////////////
void xml_writer::CloseTag(const char *tag)
{
    CString cTag(tag);
    StringToFile(_T("</"));
    StringToFile(cTag);
    StringToFile(_T(">\n"));    
}

///////////////////////////////////////////////////////////////////////////////
void xml_writer::StringToFile(LPCTSTR str)
{
    if (mpFile && str)
    {
        // Convert Unicode string to UTF8 string:

        // The maximun UTF8 character is 6 bytes, calculate the maximun utf8 buffer size needed for the string.
        int char_count = _tcslen(str) * 6 + 1;
        char *utf8buf = new char[char_count];
        strcpy_s(utf8buf, char_count, CT2A(str, CP_UTF8));
        mpFile->Write(utf8buf, strlen(utf8buf));
        delete[] utf8buf;
    }
}
