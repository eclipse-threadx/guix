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


#ifndef _CSV_READ_WRITE_
#define _CSV_READ_WRITE_

class csv_read_write
{
public:
    csv_read_write();
    BOOL ExportLanguage(studiox_project *project);
    BOOL ImportCsvFile(studiox_project *project, CString &pathname);

private:
    void ReadFile(CString pathname);
    BOOL ReadLine(CString &line);
    int FindDelimiterIndex(CString line, int start_index);
    void SplitCsvLine(CString &line, CStringArray &line_array);
    CString MakeCsvFormatValue(CString string);
    void WriteString(CStdioFile *file, CString string);

private:
    char *mpLine;
    char *mpBuffer;

};



#endif