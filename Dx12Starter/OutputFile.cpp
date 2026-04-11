#include "OutputFile.h"
#include "Logger.h"

OutputFile::OutputFile()
{
}

void OutputFile::Open(LPCSTR fileName)
{
    File::Open(fileName, GENERIC_WRITE, CREATE_ALWAYS);
    // If file could not be opened, silently fail to avoid circular dependency with Logger
    if (fileHandle == nullptr)
    {
        return;
    }
}

void OutputFile::Write(LPCVOID data, DWORD size)
{
    DWORD bytesWritten = 0;
    BOOL errorFlag = WriteFile(fileHandle, data, size, &bytesWritten, NULL);
    if (FALSE == errorFlag)
    {
        // Unable to write to file - silently fail to avoid circular dependency with Logger
        return;
    }
    else if (bytesWritten != size)
    {
        // Partial write - silently fail to avoid circular dependency with Logger
        return;
    }
}