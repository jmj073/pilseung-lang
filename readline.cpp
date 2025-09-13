#include <cstdlib>
#include <readline/readline.h>
#include <readline/tilde.h>

#include "readline.h"

using namespace std;

optional<string> ReadLine::get(const string& prompt) {
    char* line = readline(prompt.c_str());
    if (line == NULL) {
        return nullopt;
    }

    string ret = line;
    free(line);

    return ret;
}

