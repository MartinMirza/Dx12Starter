#pragma once

#include <windows.h>

class File
{
public:
    virtual ~File() = default;
    virtual void Open(LPCSTR fileName) = 0;
    void Close();

protected:
    void Open(LPCSTR fileName, DWORD accessType, DWORD creationType);
    HANDLE fileHandle = nullptr;
};