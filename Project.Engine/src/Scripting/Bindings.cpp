#define API extern "C"

#include <Logging/Logger.hpp>

API void Log(const char *classname, int line, int verbose, const char *message)
{
    Logging::LogInternalBis(classname, line, verbose, message);
    if(verbose == Logging::Verbosity::Fatal)
    {
        abort();
    }
}

