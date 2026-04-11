#include "InputFile.h"

InputFile::InputFile() : fileSize(0), fileBuffer(nullptr)
{
}

void InputFile::Open(LPCSTR fileName)
{
    File::Open(fileName, GENERIC_READ, OPEN_EXISTING);
}

void InputFile::Read()
{
    fileSize = GetFileSize(fileHandle, NULL);
    if (fileSize == INVALID_FILE_SIZE)
    {
        // File size could not be determined
        fileSize = 0;
        fileBuffer = nullptr;
        return;
    }
    
    fileBuffer = new char[fileSize];
    DWORD bytesRead = 0;
    if (!ReadFile(fileHandle, fileBuffer, fileSize, &bytesRead, NULL))
    {
        // Read failed
        delete[] fileBuffer;
        fileBuffer = nullptr;
        fileSize = 0;
        return;
    }
    
    if (bytesRead != fileSize)
    {
        // Partial read
        delete[] fileBuffer;
        fileBuffer = nullptr;
        fileSize = 0;
        return;
    }
}

void* InputFile::GetBuffer()
{
    return fileBuffer;
}

DWORD InputFile::GetSize()
{
    return fileSize;
}

void InputFile::Close()
{
    File::Close();
}