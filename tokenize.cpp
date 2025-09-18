
#include "tokenize.h"
#include "util.h"

using namespace std;

namespace ps {
    std::vector<std::wstring> tokenize(const std::wstring& code) {
        wregex re(LR"((!+|님|께|신!!?고!합니다!|경례!|대하여!|바로!|(?:필!*)+승!+|[\n\r\f]+|[^님께!\s]+))");
        auto tokens = wregex_view(code, re)
            | views::transform([](auto&& m) { return m.str(1); });
        return std::vector<std::wstring>(tokens.begin(), tokens.end());
    }

    bool is_endline_token(const std::wstring& token) {
        wregex re(LR"([\n\r\f]+)");
        return regex_match(token, re);
    }

    bool is_callword_token(const std::wstring& token) {
        return token == L"님" || token == L"께";
    }

    bool is_pilseung_literal_token(const std::wstring& token) {
        wregex re(LR"((?:필!*)+승!+)");
        return regex_match(token, re);
    }

    bool is_singo_token(const std::wstring& token) {
        wregex re(LR"(신!!?고!합니다!)");
        return regex_match(token, re);
    }

    bool is_normal_token(const std::wstring& token) {
        wregex re(LR"([^님께!\s]+)");
        return regex_match(token, re);
    }

    bool is_salute_token(const std::wstring& token) {
        return token == L"경례!";
    }

    bool is_array_ex_mark_token(const std::wstring& token) {
        wregex re(LR"(!!+)");
        return regex_match(token, re);
    }

    bool is_ex_mark_token(const std::wstring& token) {
        wregex re(LR"(!+)");
        return regex_match(token, re);
    }

    bool is_daehayeo_token(const std::wstring& token) {
        return token == L"대하여!";
    }

    bool is_baro_token(const std::wstring& token) {
        return token == L"바로!";
    }

} // namespace ps
