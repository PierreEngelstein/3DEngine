#pragma once
#define STR(a) #a
#include <cstring>
#include <string>
#include <cassert>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

namespace Logging
{
    enum Verbosity
    {
        Debug,
        Info,
        Warning,
        Error,
        Fatal,
    };

    /** Gets the minimal verbosity of the logger. */
    Verbosity GetMinimalVerbosity();
    /** Gets the maximal verbosity of the logger. */
    Verbosity GetMaximalVerbosity();
    /** Sets the minimal verbosity of the logger. */
    void SetMinimalVerbosity(Verbosity minimal);
    /** Sets the maximal verbosity of the logger. */
    void SetMaximalVerbosity(Verbosity maximal);
    /** Sets a filter for the log category (regular expression). */
    void SetFilterCategory(const std::string& filter);
    /** Removes the category filter. */
    void RemoveFilterCategory();
    /** Tells the logger if it should log the full namespace::class::method() or just the class name. */
    void UseCompleteName(bool use);

    void LogInternal(const char *file, const char *method, int line, int verbosity, const char *message, ...);

#pragma region Macros Definitions

    /**
     * @brief Prints a log to the standard output
     * 
     * @param Category : The category of the message, used for filtering
     * @param Verbosity : The verbosity of the message.
     * @param Message : The message to be printed.
     */
    #define LOG(Verbosity, Message, ...)\
    {\
        Logging::LogInternal(__FILENAME__, __PRETTY_FUNCTION__, __LINE__, Verbosity, Message, ##__VA_ARGS__);\
        if(Verbosity == Logging::Fatal)\
        {\
            abort();\
        }\
    }

    #define LOG_DEBUG(Message, ...)   LOG(Logging::Debug,   Message, ##__VA_ARGS__)
    #define LOG_INFO(Message, ...)    LOG(Logging::Info,    Message, ##__VA_ARGS__)
    #define LOG_WARNING(Message, ...) LOG(Logging::Warning, Message, ##__VA_ARGS__)
    #define LOG_ERROR(Message, ...)   LOG(Logging::Error,   Message, ##__VA_ARGS__)
    #define LOG_FATAL(Message, ...)   LOG(Logging::Fatal,   Message, ##__VA_ARGS__)
#pragma endregion
}