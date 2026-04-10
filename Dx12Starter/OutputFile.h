#pragma once

#include <Windows.h>
#include "File.h"

class OutputFile : public File
{
public:
    OutputFile();
    ~OutputFile();
    virtual void Open(LPCSTR fileName) override;
    void Close();
    void Write(LPCVOID data, DWORD size);
};