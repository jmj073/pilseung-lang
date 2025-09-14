#pragma once

#include <vector>
#include <string>
#include <exception>
#include <codecvt>
#include <locale>

#include "ast.h"

namespace ps {
    Program parse(std::vector<std::wstring> tokens);

    class SyntaxError: public std::exception {
        std::wstring m_msg;
        std::string utf8_msg;

    public:
        explicit SyntaxError(const std::wstring& msg)
            : std::exception()
            , m_msg(msg)
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
            utf8_msg = conv.to_bytes(m_msg);
        }

        const char* what() const noexcept override {
            return utf8_msg.c_str();
        }

        virtual const std::wstring& wwhat() const noexcept {
            return m_msg;
        }
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
