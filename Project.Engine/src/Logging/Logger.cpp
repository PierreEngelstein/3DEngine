#include <Logging/Logger.hpp>
#include <iostream>
#include <string>
#include <cstdarg>
#include <regex>

Logging::Verbosity minimal_verbose = Logging::Debug;
Logging::Verbosity maximal_verbose = Logging::Fatal;
std::string filterCategory = "";
bool useCompleteName = false;

std::string methodNameInternal(const std::string &prettyFunction) {
    size_t colons = prettyFunction.find("::");
    size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
    size_t end = prettyFunction.rfind("(") - begin;

    return prettyFunction.substr(begin,end) + "()";
}

std::string classNameInternal(const std::string &prettyFunction) {
    size_t colons = prettyFunction.find("::");
    if (colons == std::string::npos)
        return "::";
    size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
    size_t end = colons - begin;

    return prettyFunction.substr(begin,end);
}

std::string va_print_str(const char *format, va_list ap)
{
    char message[1024];
    char* buf = message;
    int mes_size = sizeof(message);

    std::string out_str;

    while(true)
    {
        va_list ap1;
        va_copy(ap1, ap);
        int min_buf_size = vsnprintf(buf, mes_size, format, ap1) + 1;
        va_end(ap1);

        if (min_buf_size > mes_size)
        {
            if (buf != message)
                delete[] buf;

            buf = new char[min_buf_size];
            mes_size = min_buf_size;
            continue;
        }

        out_str = buf;
        break;
    }

    if (buf != message)
        delete[] buf;

    return out_str;
}

std::string string_printf(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    std::string str = va_print_str(format, ap);
    va_end(ap);
    return str;
}

void PrintMessage(int val, std::string message)
{
    switch(val)
    {
        case Logging::Error : // Error
        case Logging::Fatal: // Fatal
            printf("\033[0;31m");
            break;
        case Logging::Warning: // Warning
            printf("\033[0;33m");
            break;
        case Logging::Info: // Info
            printf("\033[0;32m");
            break;
        case Logging::Debug: // Debug
            printf("\033[0;32m");
            break;
        default:
            return;
    }

    printf("%s", message.c_str());
    printf("\033[0m");
}

std::string VerbosityToString(int verbosity)
{
    switch (verbosity) {
        case Logging::Debug:    return "Debug";
        case Logging::Info:     return "Info";
        case Logging::Warning:  return "Warning";
        case Logging::Error:    return "Error";
        case Logging::Fatal:    return "Fatal";
        default: return "UNKNOWN VERBOSITY";
    }
}

void Logging::LogInternal(const char *file, const char *method, int line, int verbosity, const char *message, ...)
{
    if(verbosity < minimal_verbose) return;
    if(verbosity > maximal_verbose) return;
    std::string category = useCompleteName ? methodNameInternal(method) : classNameInternal(method);
    if(filterCategory != "" && !std::regex_match(category, std::regex(filterCategory))) return;
    va_list ap;
    va_start(ap, message);
    std::string str = va_print_str(message, ap);
    va_end(ap);
    std::string finalstr = string_printf("[%s] [%s]\t%s:%d - %s", VerbosityToString(verbosity).c_str(), category.c_str(), file, line, str.c_str());
    PrintMessage(verbosity, finalstr);
}

Logging::Verbosity Logging::GetMinimalVerbosity()
{
    return minimal_verbose;
}

Logging::Verbosity Logging::GetMaximalVerbosity() {
    return maximal_verbose;
}

void Logging::SetMinimalVerbosity(Logging::Verbosity minimal) {
    minimal_verbose = minimal;
}

void Logging::SetMaximalVerbosity(Logging::Verbosity maximal) {
    maximal_verbose = maximal;
}

void Logging::RemoveFilterCategory() {
    filterCategory = "";
}

void Logging::SetFilterCategory(const std::string &filter) {
    filterCategory = filter;
}

void Logging::UseCompleteName(bool use) {
    useCompleteName = use;
}






