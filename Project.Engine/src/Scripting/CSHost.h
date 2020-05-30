#ifndef TESTEMBEDCS_CSHOST_H
#define TESTEMBEDCS_CSHOST_H

#include <string>
#include "hostfxr.h"
#include "coreclr_delegates.h"
namespace Scripting {
    class CSHost {
    public:
        CSHost(const std::string &rootpath, const std::string &config);

        virtual ~CSHost();

        component_entry_point_fn
        LoadLibrary(const std::string &dll, const std::string &classname, const std::string &ns,
                    const std::string &method);

    private:
        bool load_hostfxr();

        load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const std::string &config_path);

        hostfxr_initialize_for_runtime_config_fn init_fptr;
        hostfxr_get_runtime_delegate_fn get_delegate_fptr;
        hostfxr_close_fn close_fptr;
        load_assembly_and_get_function_pointer_fn load_assembly_and_get_function_pointer;
        std::string root_path;
    };
}

#endif //TESTEMBEDCS_CSHOST_H
