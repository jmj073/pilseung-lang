#pragma once

#include <string>
#include <vector>

namespace ps {
    std::vector<std::wstring> tokenize(const std::wstring& code);

    bool is_endline_token(const std::wstring& token);
    bool is_callword_token(const std::wstring& token);
    bool is_pilseung_literal_token(const std::wstring& token);
    bool is_singo_token(const std::wstring& token);
} // namespace ps
