/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 Eclipse ThreadX contributors
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 **************************************************************************/


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