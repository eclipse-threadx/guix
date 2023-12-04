
#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define COMMENT_START_COLUMN 45
#define COMMENT_END_COLUMN   80
extern CString studiox_version_string;

CString CommentBlockEnd(""
    "/*******************************************************************************/\n"
    "\n\n"
);


void studio_source_writer::WriteCommentBlock(CString &comment)
{
    CString out;
    time_t osTime;
    time(&osTime);
    CTime time(osTime);

    FileWrite(comment);

    out.Format(_T("/*  GUIX Studio Revision %s     */\n"), studiox_version_string);
    FileWrite(out);
    
    out.Format(_T("/*  Date (dd.mm.yyyy): %2d.%2d.%4d   Time (hh:mm): %02d:%02d */\n"),
        time.GetDay(), time.GetMonth(), time.GetYear(), time.GetHour(), time.GetMinute());
    FileWrite(out);

    FileWrite(CommentBlockEnd);
}

void studio_source_writer::FileWrite(CString &out)
{
    CString copy(out);
    CString line;
    CString formatted;
    BOOL AddLineEnd;

    while(1)
    {
        if (copy.IsEmpty())
        {
            return;
        }

        int newline = copy.Find('\n');

        if (newline == 0)
        {
            m_outfile->Write("\r\n", 2);
            copy = copy.Mid(1);
            continue;
        }

        if (newline > 0)
        {
            line = copy.Left(newline);
            copy = copy.Mid(newline + 1);
            AddLineEnd = TRUE;
        }
        else
        {
            line = copy;
            copy.Empty();
            AddLineEnd = FALSE;
        }

        formatted = line;
        int index = line.Find(_T("/*"));
        int space;

        if (index > 2 && index < COMMENT_START_COLUMN)
        {
            formatted = line.Left(index);
            space = index;

            while(space < COMMENT_START_COLUMN)
            {
                formatted += " ";
                space++;
            }

            formatted += line.Mid(index);
        }
   
        line = formatted;
        index = line.Find(_T("*/"));

        if (index > 0 && index < (COMMENT_END_COLUMN - 1))
        {
            formatted = line.Left(index);
            space = index;

            while(space < (COMMENT_END_COLUMN - 1))
            {
                formatted += " ";
                space++;
            }

            formatted += line.Mid(index);
        }
        if (AddLineEnd)
        {
            formatted += "\r\n";
        }

        m_outfile->Write(CT2A(formatted.GetString()), formatted.GetLength());
    }
}

void studio_source_writer::FileWrite(const void *lpBuf, UINT nCount)
{
    m_outfile->Write(lpBuf, nCount);
}

void studio_source_writer::BlankLine()
{
    FileWrite(CString("\n"));
}

void studio_source_writer::WriteComment(char *comment)
{
    CString out("\n/* ");
    out += CString(comment);
    out += CString(" */\n\n");
    FileWrite(out);
}

void studio_source_writer::WriteAdditionalHeaders(CString &headers)
{
    int index;
    CString str = headers;
    CString one_header;

    while (!str.IsEmpty())
    {
        index = str.Find(_T(";"));

        if (index < 0)
        {
            index = str.GetLength();
        }

        one_header = str.Left(index);
        str = str.Mid(index + 1);

        while (one_header.GetAt(0) == ' ')
        {
            one_header = one_header.Mid(1);
        }

        while (one_header.GetAt(one_header.GetLength() - 1) == ' ')
        {
            one_header = one_header.Left(one_header.GetLength() - 1);
        }

        CString write;
        write.Format(CString("#include \"%s\"\n"), one_header);
        FileWrite(write);
    }
}