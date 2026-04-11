#include "Logger.h"
#include <cstdio>
#include <ctime>
#include <stdarg.h>

Logger& Logger::GetInstance()
{
    static Logger logger;
    return logger;
}

Logger::Logger()
{
    logFile.Open("dx12starter_log.txt");
}

Logger::~Logger()
{
    Log("%s", "----- LOG ENDED -----\n");
    logFile.Close();
}

void Logger::Log(LPCSTR format, ...)
{
    CHAR messageBuffer[STRING_BUFFER_SIZE];
    va_list variableArgumentList;
    va_start(variableArgumentList, format);
    int numOfWrittenCharacters = vsnprintf(messageBuffer, STRING_BUFFER_SIZE, format, variableArgumentList);
    va_end(variableArgumentList);

    tm newTime;
    time_t currentTime = time(nullptr);
    errno_t errorNumber = localtime_s(&newTime, &currentTime);

    char lineBuffer[STRING_BUFFER_SIZE];
    int numOfDataToWrite = snprintf(lineBuffer, STRING_BUFFER_SIZE, "%.2d:%.2d:%.2d - %s", newTime.tm_hour, newTime.tm_min, newTime.tm_sec, messageBuffer);
    
    OutputDebugStringA(lineBuffer);
    logFile.Write(lineBuffer, numOfDataToWrite);
}