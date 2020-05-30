#ifndef INC_3DENGINE_CONFIGURATION_H
#define INC_3DENGINE_CONFIGURATION_H

#include <unordered_map>

namespace Core {
    class Configuration {
    public:

        ~Configuration();

        const std::string &GetDotnetPath() const;

        static Configuration &Get(const char **argv = nullptr);

    private:
        Configuration(const char **argv);

        Configuration(Configuration const &) = delete;

        void operator=(Configuration const &) = delete;

        std::string dotnetPath;
    };
}

#endif
