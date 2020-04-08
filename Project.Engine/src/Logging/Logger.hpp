#pragma once
#define STR(a) #a
#include <cstring>
#include <cassert>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

namespace Logging
{
    enum Verbosity
    {
        Fatal,
        Error,
        Warning,
        Info
    };

    Verbosity GetMinimalVerbosity();

    void LogInternal(const char *file, int line, const char *category, int verbosity, const char *message, ...);
    
    /**
     * @brief Prints a log to the standard output
     * 
     * @param Category : The category of the message, used for filtering
     * @param Verbosity : The verbosity of the message.
     * @param Message : The message to be printed.
     */
    #define LOG(Category, Verbosity, Message, ...)\
    {\
        LogInternal(__FILENAME__, __LINE__, STR(Category), Verbosity, Message, ##__VA_ARGS__);\
        if(Verbosity == Logging::Fatal)\
        {\
            abort();\
        }\
    }
}