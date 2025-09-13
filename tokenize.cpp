#include <regex>
#include <ranges>

#include "tokenize.h"

using namespace std;

struct wregex_view: ranges::view_interface<wregex_view> {
    using iterator = wsregex_iterator;

    wregex_view(const wstring& str, const wregex& re)
        : m_str(&str), m_re(&re) {}

    iterator begin() const {
        return wsregex_iterator(m_str->begin(), m_str->end(), *m_re);
    }

    iterator end() const {
        return wsregex_iterator();
    }

private:
    const wstring* m_str;
    const wregex* m_re;
};

std::vector<std::wstring> tokenize(const std::wstring& code) {
    wregex re(LR"([ \t]*(!+|님|께|신!!?고!합니다!|경례!|대하여!|바로!|(?:필!*)+승!+|[\n\r\f]+|[^님께!\s]+))");
    auto tokens = wregex_view(code, re)
        | views::transform([](auto&& m) { return m.str(1); });
    return std::vector<std::wstring>(tokens.begin(), tokens.end());
}
