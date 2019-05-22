//
// Created by daniel on 22.05.19.
//

#include "readfile.h"

void readFile(const char* filename, std::wstring &string)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::ifstream f(filename, std::ios::in);
    std::string file_contents{std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>()};
    std::wstring result = converter.from_bytes(file_contents);
    if (result.length() > 0) {
        string = result;
    }
}

void readFile(const char* filename, std::string &string)
{
    std::ifstream f(filename, std::ios::in);
    std::string result = {std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>()};
    if (result.length() > 0) {
        string = result;
    }
}