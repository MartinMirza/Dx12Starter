#include "OutputFile.h"

OutputFile::OutputFile()
    : fileHandle(INVALID_HANDLE_VALUE)
{
}

OutputFile::~OutputFile()
{
}

void OutputFile::Open(LPCSTR fileName)
{
    fileHandle = CreateFileA(fileName, GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL, L"Unable to open file!",
            L"File Error", MB_OK | MB_ICONERROR);
        exit(1);
    }
}

void OutputFile::Close()
{
    CloseHandle(fileHandle);
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