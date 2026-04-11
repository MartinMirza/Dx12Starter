#include "File.h"

void File::Close()
{
    CloseHandle(fileHandle);
}

void File::Open(LPCSTR fileName, DWORD accessType, DWORD creationType)
{
    fileHandle = CreateFileA(fileName, accessType, 0, NULL, creationType, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        // File could not be opened - set handle to nullptr to indicate failure
        fileHandle = nullptr;
        return;
    }
}