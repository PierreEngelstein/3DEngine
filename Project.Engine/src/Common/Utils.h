#pragma once
#include <string>

/**
 * @brief Loads a file from disk into a string.
 * 
 * @param path : The path to the file.
 * @param text : Pointer to the file string.
 * @return true if no error occured.
 * @return false otherwise.
 */
bool LoadFile(const std::string& path, std::string* text);