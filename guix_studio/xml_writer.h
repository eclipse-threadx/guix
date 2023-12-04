
#ifndef _XML_WRITER_
#define _XML_WRITER_

class xml_writer {
    public:
        xml_writer();
        ~xml_writer();
        BOOL OpenFile(const CString &pathname);
        void OpenMemFile(CFile *file);
        void CloseFile(void);
        void WriteHeader(const char *docType);
        void WriteInt(const char *name, int val);
        void WriteUByte(const char *name, GX_UBYTE val);
        void WriteUnsigned(const char *name, ULONG val);
        void WriteHex(const char *name, ULONG val);
        void WriteBool(const char *name, BOOL val);
        void WriteString(const char *name, const char *val, BOOL bForce = FALSE);
        void WriteString(const char *name, const CString &val, BOOL bForce = FALSE);
        void WriteRect(const char *name, GX_RECTANGLE &rect);
        void WritePathInfo(const PATHINFO &info);
        void OpenTag(const char *tag, BOOL has_value = FALSE);
        void OpenTag(const char *tag, CString &tag_data);
        void CloseTag(const char *tag);



    private:
        CFile *mpFile;
        BOOL UsingMemFile;
        void StringToFile(LPCTSTR str);
};

#endif