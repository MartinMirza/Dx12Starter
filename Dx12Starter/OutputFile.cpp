#include "OutputFile.h"
#include "Logger.h"

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
        Logger::GetInstance().Log("ERROR: Unable to write to file! (Error: %d)\n", GetLastError());
        exit(1);
    }
    else if (bytesWritten != size)
    {
        Logger::GetInstance().Log("ERROR: Unable to write entire string! (Written: %d, Expected: %d)\n", bytesWritten, size);
        exit(1);
    }
}