#include "CSHost.h"

#include "nethost.h"
#include <assert.h>
#include <iostream>

#ifdef WINDOWS
    #include <Windows.h>
    #define STR(s) L ## s
    #define CH(c) L ## c
    #define DIR_SEPARATOR L'\\'
#else
    #include <dlfcn.h>
    #include <limits.h>
    #define STR(s) s
    #define CH(c) c
    #define DIR_SEPARATOR '/'
#endif

#define MAX_PATH PATH_MAX

void *load_library(const char_t *);
void *get_export(void *, const char *);

#ifdef WINDOWS
void *load_library(const char_t *path)
{
    HMODULE h = ::LoadLibraryW(path);
    assert(h != nullptr);
    return (void*)h;
}
void *get_export(void *h, const char *name)
{
    void *f = ::GetProcAddress((HMODULE)h, name);
    assert(f != nullptr);
    return f;
}
#else
void *load_library(const char_t *path)
{
    void *h = dlopen(path, RTLD_LAZY | RTLD_LOCAL);
    assert(h != nullptr);
    return h;
}
void *get_export(void *h, const char *name)
{
    void *f = dlsym(h, name);
    assert(f != nullptr);
    return f;
}
#endif
namespace Scripting {
    CSHost::CSHost(const std::string &rootpath, const std::string &config) : root_path(rootpath) {
        // Step 1 : load hostfxr
        assert(load_hostfxr() != false);

        // Step 2 : load .NET core runtime
        std::string config_path = root_path + config;
        load_assembly_and_get_function_pointer = nullptr;
        load_assembly_and_get_function_pointer = get_dotnet_load_assembly(config_path.c_str());
        assert(load_assembly_and_get_function_pointer != nullptr && "Failure: get_dotnet_load_assembly()");

    }

    CSHost::~CSHost() {

    }

    component_entry_point_fn CSHost::LoadLibrary
            (const std::string &dll,
             const std::string &classname,
             const std::string &ns,
             const std::string &method) {
        component_entry_point_fn function = nullptr;
        std::string dll_path = root_path + dll;
        std::string dotnet_type = classname + ", " + ns;
        int result = load_assembly_and_get_function_pointer(dll_path.c_str(), dotnet_type.c_str(), method.c_str(),
                                                            nullptr, nullptr, (void **) &function);
        assert(result == 0 && function != nullptr && "Failure: load_assembly_and_get_function_pointer()");
        return function;
    }

    bool CSHost::load_hostfxr() {
        char_t buffer[MAX_PATH];
        size_t buffer_size = sizeof(buffer) / sizeof(char_t);
        int rc = get_hostfxr_path(buffer, &buffer_size, nullptr);
        if (rc != 0)
            return false;

        // Load hostfxr and get desired exports
        void *lib = load_library(buffer);
        init_fptr = (hostfxr_initialize_for_runtime_config_fn) get_export(lib, "hostfxr_initialize_for_runtime_config");
        get_delegate_fptr = (hostfxr_get_runtime_delegate_fn) get_export(lib, "hostfxr_get_runtime_delegate");
        close_fptr = (hostfxr_close_fn) get_export(lib, "hostfxr_close");

        return (init_fptr && get_delegate_fptr && close_fptr);
    }

    load_assembly_and_get_function_pointer_fn CSHost::get_dotnet_load_assembly(const std::string &config_path) {
        // Load .NET Core
        void *load_assembly_and_get_function_pointer = nullptr;
        hostfxr_handle cxt = nullptr;
        int rc = init_fptr(config_path.c_str(), nullptr, &cxt);
        if (rc != 0 || cxt == nullptr) {
            std::cerr << "Init failed: " << std::hex << std::showbase << rc << std::endl;
            close_fptr(cxt);
            return nullptr;
        }

        // Get the load assembly function pointer
        rc = get_delegate_fptr(
                cxt,
                hdt_load_assembly_and_get_function_pointer,
                &load_assembly_and_get_function_pointer);
        if (rc != 0 || load_assembly_and_get_function_pointer == nullptr)
            std::cerr << "Get delegate failed: " << std::hex << std::showbase << rc << std::endl;

        close_fptr(cxt);
        return (load_assembly_and_get_function_pointer_fn) load_assembly_and_get_function_pointer;
    }

}
