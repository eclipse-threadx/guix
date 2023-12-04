
#ifndef _STUDIO_SOURCE_WRITER_
#define _STUDIO_SOURCE_WRITER_

class studio_source_writer {
    public:
        studio_source_writer() { m_outfile = NULL; m_written_size = 0; };

    protected:
        void WriteCommentBlock(CString &comment);
        void WriteComment(char *comment);
        void WriteAdditionalHeaders(CString &headers);
        void BlankLine();
        void FileWrite(CString &out);
        void FileWrite(const void *lpBuf, UINT nCount);

    protected:
        CFile *m_outfile;
        ULONG m_written_size;
};

#endif