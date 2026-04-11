#include "OutputFile.h"

OutputFile::OutputFile()
{
}

void OutputFile::Open(LPCSTR fileName)
{
    File::Open(fileName, GENERIC_WRITE, CREATE_ALWAYS);
}

void OutputFile::Write(LPCVOID data, DWORD size)
{
    DWORD bytesWritten = 0;
    BOOL errorFlag = WriteFile(fileHandle, data, size, &bytesWritten, NULL);
    if (FALSE == errorFlag)
    {
        MessageBox(NULL, L"Unable to write to file!",
            L"File Error", MB_OK | MB_ICONERROR);
        exit(1);
    }
    else if (bytesWritten != size)
    {
        MessageBox(NULL, L"Unable to write entire string!",
            L"File Error", MB_OK | MB_ICONERROR);
        exit(1);
    }
}