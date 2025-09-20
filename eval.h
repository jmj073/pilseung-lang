#pragma once

#include <unordered_map>

#include "ast.h"
#include "memory.h"
#include "util.h"

namespace ps {
    using Env = std::unordered_map<std::wstring, uintptr_t>;

    extern Env env;
    extern Memory mem;

    struct RuntimeError: WStringError {
        template <typename ...Types>
        RuntimeError(Types&& ...args)
            : WStringError(std::forward<Types>(args)...)
        { }
    };

    void eval(const Program& prog);
} // namespace ps
