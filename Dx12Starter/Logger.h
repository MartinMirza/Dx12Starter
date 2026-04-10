#pragma once

#include "OutputFile.h"

#define STRING_BUFFER_SIZE 1024

class Logger
{
public:
    void Log(LPCSTR format, ...);
    static Logger& GetInstance();
    Logger(Logger& other) = delete;
    Logger& operator=(const Logger&) = delete;
private:
    Logger();
    ~Logger();
    OutputFile logFile;
};