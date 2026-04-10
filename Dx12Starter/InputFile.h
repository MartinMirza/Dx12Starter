#pragma once

#include <Windows.h>
#include "Logger.h"
#include "File.h"

class InputFile : public File
{
public:
    InputFile();
    ~InputFile();
    virtual void Open(LPCSTR fileName) override;
    void Read();
    void* GetBuffer();
    DWORD GetSize();

private:
    DWORD fileSize;
    char* fileBuffer;
};