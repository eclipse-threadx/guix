
#ifndef _XML_READER_
#define _XML_READER_

#define MAX_SECTION_NESTING 40

typedef struct {
    char *start;
    char *end;
    char *string;
    char *tag;
} XML_SECTION;

class xml_reader
{
    public:
        xml_reader();
        ~xml_reader();

        BOOL ReadFile(CString &pathname);
        BOOL ReadFile(CFile *memfile);
        BOOL ReadBool(const char *tagname, BOOL &put);
        BOOL ReadInt(const char *tagname, int &put, int defval = 0);
        BOOL ReadUByte(const char *tagname, GX_UBYTE &put, GX_UBYTE defval = 0);
        BOOL ReadUnsigned(const char *tagname, ULONG &put, ULONG defval = 0);
        BOOL ReadHex(const char *tagname, ULONG &put, ULONG defval = 0);
        BOOL ReadUShort(const char *tagname, USHORT &put, USHORT defval = 0);
        BOOL ReadValue(const char *tagname, GX_VALUE &put, GX_VALUE defval = 0);
        BOOL ReadRect(const char *tagname, GX_RECTANGLE &rect);
        BOOL ReadString(const char *tagname, char *put, int bufsize);
        BOOL ReadString(const char *tagname, CString &put);

        void ReadPathInfo(PATHINFO &put);

        void CloseSection(BOOL skip_to_end = FALSE, BOOL MarkRead = FALSE);
        void CloseTag();

        char *FindTag(const char *tagname, BOOL mark_used = FALSE);
        char *FindTag(char *start, char *stop,
            const char *tagname, BOOL mark_used = FALSE);

        char *EndTag(char *start, char *stop);

        BOOL EnterSection(const char *tagname);
        BOOL EnterTag(const char *tagname);

        char *GetString(void);
        float GetTagValueFloat(char *name, float def_val);
        BOOL  GetTagString(char *name, CString &put);

    private:

        BOOL PopSection(void);
        BOOL PushSection(void);
        
        char *mpBuffer;
        XML_SECTION mSectionStack[MAX_SECTION_NESTING];
        XML_SECTION *mpSectionStackPtr;
        XML_SECTION mSection;
        int mSectionLevel;
};
#endif
