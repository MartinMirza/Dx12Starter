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
    fileBuffer = new char[fileSize];
    if (!ReadFile(fileHandle, fileBuffer, fileSize, NULL, NULL))
    {
        exit(3);
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