#pragma once

#include <Windows.h>
#include "File.h"

class OutputFile : public File
{
public:
    OutputFile();
    virtual ~OutputFile() = default;
    virtual void Open(LPCSTR fileName) override;
    void Write(LPCVOID data, DWORD size);
};