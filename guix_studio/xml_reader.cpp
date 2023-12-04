

#include "studiox_includes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
xml_reader::xml_reader()
{
    mpBuffer = NULL;
    mpSectionStackPtr = mSectionStack;
    mpSectionStackPtr += MAX_SECTION_NESTING;
    mSectionLevel = 0;
}

///////////////////////////////////////////////////////////////////////////////
xml_reader::~xml_reader()
{
    if (mpBuffer)
    {
        delete [] mpBuffer;
    }
}

///////////////////////////////////////////////////////////////////////////////
BOOL xml_reader::ReadFile(CString &pathname)
{
    CStdioFile file;
    if (file.Open(pathname, CFile::modeRead))
    {
        ULONG size = (ULONG) file.GetLength();
        mpBuffer = new char[size];
        file.Read(mpBuffer, size);
        file.Close();

        // my current section is whole file
        mSection.start = mpBuffer;
        mSection.end = mpBuffer + size - 1;

        // no local data copy
        mSection.string = NULL;
        return TRUE;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL xml_reader::ReadFile(CFile *memfile)
{
    ULONG size = (ULONG) memfile->GetLength();
    mpBuffer = new char[size];
    memfile->Read(mpBuffer, size);

    // my current section is whole file
    mSection.start = mpBuffer;
    mSection.end = mpBuffer + size - 1;

    // no local data copy
    mSection.string = NULL;
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL xml_reader::ReadBool(const char *tagname, BOOL &put)
{
    BOOL found = FALSE;
    put = FALSE;
    if (EnterTag(tagname))
    {
        if (mSection.string)
        {
            if (strcmp(mSection.string, "TRUE") == 0 ||
                strcmp(mSection.string, "true") == 0)
            {
                put = TRUE;
            }
            found = TRUE;
        }
        CloseTag();
    }
    return found;
}

///////////////////////////////////////////////////////////////////////////////
BOOL xml_reader::ReadInt(const char *tagname, int &put, int defval)
{
    BOOL found = FALSE;
    put = defval;

    if (EnterTag(tagname))
    {
        if (mSection.string)
        {
            put = strtol(mSection.string, NULL, 10);
            found = TRUE;
        }
        CloseTag();
    }
    return found;
}

///////////////////////////////////////////////////////////////////////////////
BOOL xml_reader::ReadUnsigned(const char *tagname, ULONG &put, ULONG defval)
{
    BOOL found = FALSE;
    ULONG val;
    put = defval;

    if (EnterTag(tagname))
    {
        if (mSection.string)
        {
            val = strtoul(mSection.string, NULL, 10);
            put = val;
            found = TRUE;
        }
        CloseTag();
    }
    return found;
}

///////////////////////////////////////////////////////////////////////////////
BOOL xml_reader::ReadUByte(const char *tagname, GX_UBYTE &put, GX_UBYTE defval)
{
    BOOL found = FALSE;
    GX_UBYTE val;
    put = defval;

    if (EnterTag(tagname))
    {
        if (mSection.string)
        {
            val = (GX_UBYTE) strtoul(mSection.string, NULL, 10);
            put = val;
            found = TRUE;
        }
        CloseTag();
    }
    return found;

}


///////////////////////////////////////////////////////////////////////////////
BOOL xml_reader::ReadHex(const char *tagname, ULONG &put, ULONG defval)
{
    BOOL found = FALSE;
    ULONG val;
    put = defval;

    if (EnterTag(tagname))
    {
        if (mSection.string)
        {
            val = strtoul(mSection.string, NULL, 16);
            put = val;
            found = TRUE;
        }
        CloseTag();
    }
    return found;
}

///////////////////////////////////////////////////////////////////////////////
BOOL xml_reader::ReadUShort(const char *tagname, USHORT &put, USHORT defval)
{
    ULONG longval;
    if (ReadUnsigned(tagname, longval, defval))
    {
        put = (USHORT) longval;
        return TRUE;
    }
    put = defval;
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL xml_reader::ReadValue(const char *tagname, GX_VALUE &put, GX_VALUE defval)
{
    ULONG longval;
    if (ReadUnsigned(tagname, longval, defval))
    {
        put = (GX_VALUE)longval;
        return TRUE;
    }
    put = defval;
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL xml_reader::ReadRect(const char *tagname, GX_RECTANGLE &put)
{
    put.gx_rectangle_bottom = put.gx_rectangle_top = 0;
    put.gx_rectangle_left = put.gx_rectangle_right = 0;

    if (EnterTag(tagname))
    {
        ReadValue("left", put.gx_rectangle_left);
        ReadValue("top", put.gx_rectangle_top);
        ReadValue("right", put.gx_rectangle_right);
        ReadValue("bottom", put.gx_rectangle_bottom);

        CloseTag();
        return TRUE;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL xml_reader::ReadString(const char *tagname, char *put, int bufsize)
{
    BOOL found = FALSE;
    CString val;
    found = ReadString(tagname, val);

       
    if (found)
    {
        strncpy_s(put, bufsize, CT2A(val.GetString()), bufsize - 1);
    }
    else
    {
        *put = '\0';
    }
    return found;
}

///////////////////////////////////////////////////////////////////////////////
BOOL xml_reader::ReadString(const char *tagname, CString &put)
{
    BOOL found = FALSE;
    CString val;

    if (EnterTag(tagname))
    {
        if (mSection.string)
        {
            CString escaped(CA2T(mSection.string, CP_UTF8));
            escaped.Replace(_T("&amp;"), _T("&"));
            escaped.Replace(_T("&quot;"), _T("\""));
            escaped.Replace(_T("&apos;"), _T("\'"));
            escaped.Replace(_T("&lt;"), _T("<"));
            escaped.Replace(_T("&gt;"), _T(">"));

            put = escaped;
            found = TRUE;
        }
        CloseTag();
    }
   
    if (!found)
    {
        put.Empty();
    }
    return found;
}

///////////////////////////////////////////////////////////////////////////////
void xml_reader::ReadPathInfo(PATHINFO &info)
{
    info.pathname = "";
    info.pathtype = PATH_TYPE_PROJECT_RELATIVE;

    if (EnterSection("pathinfo"))
    {
        ReadString("pathname", info.pathname);

        CString pathtype;
        ReadString("pathtype", pathtype);

        if (pathtype == "project_relative")
        {
            info.pathtype = PATH_TYPE_PROJECT_RELATIVE;
        }
        else
        {
            if (pathtype == "studio_relative")
            {
                info.pathtype = PATH_TYPE_INSTALL_RELATIVE;
            }
            else
            {
                info.pathtype = PATH_TYPE_ABSOLUTE;
            }
        }
        CloseSection();
    }
    else
    {
        ReadString("path", info.pathname);
    }

    if (info.pathtype != PATH_TYPE_ABSOLUTE)
    {
        if (info.pathname[0] == '.' &&
            info.pathname[1] == '\\')
        {
            info.pathname.TrimLeft(_T(".\\"));
        }
        else
        {
            if (info.pathname[0] == '\\')
            {
                info.pathname.TrimLeft('\\');
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
void xml_reader::CloseSection(BOOL skip_to_end, BOOL mark_read)
{
    if (mark_read)
    {
        *(mSection.start + 1) = '.';
        *(mSection.end + 1) = '.';
    }

    if (skip_to_end)
    {
        char *pStart = mSection.end + 1;
        PopSection();
        mSection.start = pStart;

        // don't point the start at a closing tag, 
        // walk to the start of next tag:
        if (*pStart == '<' && *(pStart + 1) == '/')
        {
            pStart += 2;

            while(pStart < mSection.end)
            {
                if (*pStart == '>')
                {
                    mSection.start = pStart + 1;
                    break;
                }
                pStart++;
            }
        }
    }
    else
    {
        PopSection();
    }
}

///////////////////////////////////////////////////////////////////////////////
void xml_reader::CloseTag()
{
    PopSection();
}

///////////////////////////////////////////////////////////////////////////////
char *xml_reader::FindTag(const char *tagname, BOOL mark_used)
{
    return FindTag(mSection.start, mSection.end, tagname, mark_used);
}

///////////////////////////////////////////////////////////////////////////////
char *xml_reader::FindTag(char *start, char *stop,
            const char *tagname, BOOL mark_used)
{
    int nest_level = 0;
    int taglen = (int) strlen(tagname);

    while(start < stop)
    {
        // this will only happen if the XML is mal-formed, but we have
        // to guard against it.
        if (nest_level < 0)
        {
            return NULL;
        }

        if (*start == '<')
        {
            if (!nest_level)
            { 
                // FIXME this is NOT very robust, it will fail if there is
                // any whitespace around the tagname, but it will
                // work for now.

                if (strncmp(start + 1, tagname, taglen) == 0)
                {
                    if (*(start + taglen + 1) == '>' || *(start + taglen + 1) == ' ')
                    {
                        // were we search for the end tag?
                        if (*tagname == '/')
                        {
                            // we found the end tag
                            return (start - 1);
                        }

                        // we found the start tag
                        // are we supposed to mark this unreadable?
                        if (mark_used)
                        {
                            *(start + 1) = '.';
                        }
                        // return pointer to first character after the tag:
                        //start += taglen + 2;

                        //while(*start == ' ' || *start == 0x0d || *start == 0x0a)
                        //{
                        //    start++;
                        //}
                        return start;
                    }
                }
            }

            char test = *(start + 1);

            // check for </ sequence
            if (test == '/')
            {
                nest_level--;
            }
            else
            {
                // if we hit a '<' and it's not a comment, then
                // bump up our nest level:
                if (test != '?' && test != '!')
                {
                    nest_level++;
                }
            }
        }
        start++;
    }

    // didn't find what we were looking for
    return NULL;
}
 
///////////////////////////////////////////////////////////////////////////////
char *xml_reader::EndTag(char *start, char *stop)
{
    while(start < stop)
    {
        if (*start == '>')
        {
            return start;
        }
        start++;
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
BOOL xml_reader::EnterSection(const char *tagname)
{
    char *end_section;
    char *end_tag;
    char close[MAX_PATH];
    close[0] = '/';

    if (!tagname || *tagname == '\0')
    {
        return FALSE;
    }

    if (mSectionLevel >= MAX_SECTION_NESTING)
    {
        return FALSE;
    }

    char *start_tag = FindTag(tagname);
    
    if (!start_tag)
    {
        return FALSE;
    }
    end_tag = EndTag(start_tag, mSection.end);

    if (!end_tag)
    {
        return FALSE;
    }

    strcpy_s(close + 1, MAX_PATH -1, tagname);
    end_section = FindTag(end_tag + 1, mSection.end, close);

    if (end_section)
    {
        int datalen = (int) (end_section - end_tag) + 1;

        if (datalen >= 0)
        {
            if (PushSection())
            {
                mSection.start = end_tag + 1;
                mSection.end = end_section;
                mSection.string = NULL;

                // copy the section tagname into the section marker
                // just for debugging purposes.
                int len = end_tag - start_tag + 1;
                mSection.tag = new char[len + 1];
                strncpy_s(mSection.tag, len + 1, start_tag, len);
                mSection.tag[len] = 0;
                return TRUE;
            }
        }
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL xml_reader::EnterTag(const char *tagname)
{
    char *end_data;
    char close[MAX_PATH];
    close[0] = '/';

    if (mSectionLevel >= MAX_SECTION_NESTING)
    {
        return FALSE;
    }

    char *start_tag = FindTag(tagname, TRUE);
    char *end_tag;
    
    if (!start_tag)
    {
        return FALSE;
    }
    end_tag = EndTag(start_tag, mSection.end);

    if (!end_tag)
    {
        return FALSE;
    }

    // the tag close is the same as the tag, with a leading '/' char
    strcpy_s(close + 1, MAX_PATH -1, tagname);
    end_data = FindTag(end_tag, mSection.end, close);

    if (end_data)
    {
        int datalen = (int) (end_data - end_tag);

        if (datalen >= 0)
        {
            // this is just a safety check, we don't use any
            // values that are huge, so if we think we found a huge tag
            // something is wrong with the XML:
            if (datalen > (10 * 1024))
            {
                ErrorMsg("Internal error: Invalid XML data length.");
                return FALSE;
            }

            if (PushSection())
            {
                mSection.start = end_tag + 1;
                mSection.end = end_data;
                mSection.tag = NULL;

                // copy the tag (and any embedded elements) into local array:

                int tag_len = end_tag - start_tag + 1;
                if (tag_len > 0)
                {
                    mSection.tag = new char[tag_len + 1];
                    strncpy_s(mSection.tag, tag_len + 1, start_tag, tag_len);
                    mSection.tag[tag_len] = 0;
                }
                // copy the tag contents to a new array, so that we
                // can NULL terminate the content string
                mSection.string = new char[datalen + 1];
                strncpy_s(mSection.string, datalen + 1, end_tag + 1, datalen);
                // null terminate our local copy
                mSection.string[datalen] = 0;

                return TRUE;
            }
        }
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
char *xml_reader::GetString(void)
{
    return mSection.string;
}

///////////////////////////////////////////////////////////////////////////////
float xml_reader::GetTagValueFloat(char *name, float def_val)
{
    float return_val = def_val;

    // search within the tag itself for the requested value
    if (mSection.tag)
    {
        char *marker = strstr(mSection.tag, name);
        if (marker)
        {
            while(*marker)
            {
                if (*marker == '=')
                {
                    marker++;
                    while(*marker == '\"' || *marker == '\'' || *marker == ' ')
                    {
                        marker++;
                    }
                    return_val = (float) atof(marker);
                    return return_val;
                }
                marker++;
            }
        }
    }
    return return_val;
}

///////////////////////////////////////////////////////////////////////////////
BOOL xml_reader::GetTagString(char *name, CString &value)
{
    // search within the tag itself for the requested value
    value = "";

    char temp[MAX_PATH];

    if (mSection.tag)
    {
        char *marker = strstr(mSection.tag, name);
        if (marker)
        {
            while(*marker)
            {
                if (*marker == '=')
                {
                    marker++;
                    while(*marker == '\"' || *marker == '\'' || *marker == ' ')
                    {
                        marker++;
                    }
                    char *end = marker + 1;

                    while(*end)
                    {
                        if (*end == '\"' || *end == '\'')
                        {
                            break;
                        }
                        end++;
                    }
                    int len = end - marker;

                    if (len > 0)
                    {
                        if (len >= MAX_PATH)
                        {
                            ErrorMsg("Invalid XLIFF content.");
                            return FALSE;
                        }
                        strncpy(temp, marker, len);                    
                        temp[len] = 0;
                        value = CString(temp);
                        return TRUE;
                    }
                    else
                    {
                        return FALSE;
                    }
                }
                marker++;
            }
        }
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL xml_reader::PopSection(void)
{
    if (mSectionLevel > 0)
    {
        if (mSection.string)
        {
            delete [] mSection.string;
        }
        if (mSection.tag)
        {
            delete [] mSection.tag;
        }
        mSection = *mpSectionStackPtr;
        mpSectionStackPtr++;
        mSectionLevel--;
        return TRUE;
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL xml_reader::PushSection(void)
{
    if (mSectionLevel < MAX_SECTION_NESTING)
    {
        mpSectionStackPtr--;
        *mpSectionStackPtr = mSection;
        mSectionLevel++;
        return TRUE;
    }
    return FALSE;
}

        
