//
// Created by pierre on 5/23/20.
//

#include <assert.h>
#include "Configuration.h"
namespace Core {
    std::string getrootpath(const char **argv) {
        char host_path[4096];
#if WINDOWS
        auto size = ::GetFullPathNameW(argv[0], sizeof(host_path) / sizeof(char_t), host_path, nullptr);
        assert(size != 0);
#else
        auto resolved = realpath(argv[0], host_path);
        assert(resolved != nullptr);
#endif

        std::string root_path = host_path;
        auto pos = root_path.find_last_of("/");
        assert(pos != std::string::npos);
        root_path = root_path.substr(0, pos + 1);
        return root_path;
    }

    Configuration::Configuration(const char **argv) {
        dotnetPath = getrootpath(argv);
    }

    Configuration::~Configuration() {

    }

    const std::string &Configuration::GetDotnetPath() const {
        return dotnetPath;
    }

    Configuration &Configuration::Get(const char **argv) {
        static Configuration config(argv);
        return config;
    }
}