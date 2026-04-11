#pragma once

#include <Windows.h>
#include "File.h"

class InputFile : public File
{
public:
    InputFile();
    virtual ~InputFile() override = default;
    virtual void Open(LPCSTR fileName) override;
    void Read();
    void* GetBuffer();
    DWORD GetSize();
    void Close();

private:
    DWORD fileSize;
    char* fileBuffer;
};