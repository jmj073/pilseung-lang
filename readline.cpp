// #include <readline/readline.h>
// #include <readline/tilde.h>

#include <cstdlib>
#include <codecvt>
#include <locale>
#include <iostream>

#include "readline.h"

using namespace std;

/*
optional<wstring> ReadLine::get(const wstring& prompt) {
    wstring_convert<codecvt_utf8_utf16<wchar_t>> conv;

    auto str_prompt = conv.to_bytes(prompt);
    char* line = readline(str_prompt.c_str());
    if (line == NULL) {
        return nullopt;
    }

    string ret = line;
    free(line);

    return conv.from_bytes(ret);
}
*/


optional<wstring> ReadLine::get(const wstring& prompt) {
    wstring line;
    wcout << prompt;
    getline(wcin, line);
    if (!wcin) return nullopt;
    return line;
}
