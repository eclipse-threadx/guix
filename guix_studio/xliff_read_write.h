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


#ifndef _XLIFF_READ_WRITE_
#define _XLIFF_READ_WRITE_

class xliff_read_write
{
    public: 

        xliff_read_write();
        BOOL ExportLanguage(studiox_project *project);
        BOOL ImportXliffFile(studiox_project *project, CString &pathname);
};



#endif