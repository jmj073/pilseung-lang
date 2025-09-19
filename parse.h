#pragma once

#include <vector>
#include <string>
#include <codecvt>
#include <locale>

#include "ast.h"
#include "util.h"

namespace ps {
    Program parse(std::vector<std::wstring> tokens);

    struct SyntaxError: WStringError {
    };

    struct EOFError: SyntaxError {
        EOFError(const std::wstring& token)
            : SyntaxError(L"'" + token + L"' is expected")
        { }
    };

    struct Unexpected: SyntaxError {
        Unexpected(const std::wstring& un, const std::wstring& ex = L"")
            : SyntaxError(L"token '" + un + L"' is unexpected"
                + (ex.empty() ? ex : L", expected is '" + ex + L"'"))
        { }
    };

    struct NotYet {
    };
}
