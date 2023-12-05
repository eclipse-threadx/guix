
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