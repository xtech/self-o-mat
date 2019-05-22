//
// Created by daniel on 22.05.19.
//

#ifndef SELF_O_MAT_READFILE_H
#define SELF_O_MAT_READFILE_H

#include <fstream>
#include <string>
#include <locale>
#include <codecvt>

void readFile(const char* filename, std::wstring &string);
void readFile(const char* filename, std::string &string);

#endif //SELF_O_MAT_READFILE_H
