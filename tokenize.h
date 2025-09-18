#pragma once

#include <string>
#include <vector>

namespace ps {
    std::vector<std::wstring> tokenize(const std::wstring& code);

    bool is_endline_token(const std::wstring& token);
    bool is_callword_token(const std::wstring& token);
    bool is_pilseung_literal_token(const std::wstring& token);
    bool is_singo_token(const std::wstring& token);
    bool is_normal_token(const std::wstring& token);
    bool is_salute_token(const std::wstring& token);
    bool is_array_ex_mark_token(const std::wstring& token);
    bool is_ex_mark_token(const std::wstring& token);
    bool is_daehayeo_token(const std::wstring& token);
    bool is_baro_token(const std::wstring& token);
} // namespace ps
