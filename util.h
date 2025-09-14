#pragma once

#include <unistd.h>
#include <cstdio>

bool stdin_is_tty() {
    return isatty(fileno(stdin));
}

bool stdout_is_tty() {
    return isatty(fileno(stdout));
}
