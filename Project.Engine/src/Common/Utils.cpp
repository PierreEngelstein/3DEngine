#include <fstream>

#include "Utils.h"


bool LoadFile(const std::string& path, std::string* text)
{
    std::ifstream file(path.c_str(), std::ios::in);
    if(file.is_open())
    {
        std::string text_tmp;
        file.seekg(0, std::ios::end);
        text_tmp.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&text_tmp[0], text_tmp.size());
        file.close();
        *text = text_tmp;
        return true;
    }
    return false;
}