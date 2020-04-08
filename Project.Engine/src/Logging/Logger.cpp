#include <Logging/Logger.hpp>
#include <iostream>
#include <string>
#include <cstdarg>

Logging::Verbosity minimal_verbose = Logging::Info;

Logging::Verbosity Logging::GetMinimalVerbosity()
{
    return minimal_verbose;
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
            printf("\033[0;31m");
            break;
        case Logging::Warning: // Warning
            printf("\033[0;33m");
            break;
        case Logging::Info: // Info
            printf("\033[0;32m");
            break;
        default:
            return;
    }

    printf("%s", message.c_str());
    printf("\033[0m");
}

void Logging::LogInternal(const char *file, int line, const char *category, int verbosity, const char *message, ...)
{
    va_list ap;
    va_start(ap, message);
    std::string str = va_print_str(message, ap);
    va_end(ap);
    std::string finalstr = string_printf("%s - %s:%d - %s", category, file, line, str.c_str());
    PrintMessage(verbosity, finalstr);
}