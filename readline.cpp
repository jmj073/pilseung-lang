// #include <readline/readline.h>
// #include <readline/tilde.h>

#include <cstdlib>
#include <unistd.h>
#include <cstdio>

#include "readline.h"
#include "util.h"

using namespace std;

ReadLine::ReadLine()
    : m_term("/dev/tty")
{ }

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
    if (stdin_is_tty()) {
        m_term << prompt << flush;
    }

    wstring line;

    if (!getline(wcin, line)) {
        return nullopt;
    }
    return line;
}
